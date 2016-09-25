#include "c_maths.h"
#include "fdf.h"

t_matrix	*matrix_identity(int size)
{
	int			i;
	t_matrix	*identity;

	if (!(identity = matrix_init(size, size)))
		return (NULL);
	i = 0;
	while (i < size)
	{
		identity->m[i + i * size] = 1;
		i++;
	}
	return (identity);
}

t_matrix	*set_one_rot_4d(int c1, int c2, double ang)
{
	t_matrix	*rot;

	if (!(rot = matrix_identity(4)))
		return (NULL);
	rot->m[c1 + c1 * 4] = cos(ang);
	rot->m[c2 + c2 * 4] = cos(ang);
	rot->m[c2 + c1 * 4] = -sin(ang) * ((c1 == 0) ? 1 : -1);
	rot->m[c1 + c2 * 4] = sin(ang) * ((c1 == 0) ? 1 : -1);
	return (rot);
}

t_matrix	**get_the_rot4d(t_matrix *angle)
{
	int			i;
	int			j;
	int			id_rot;
	t_matrix	**rot;

	if (!angle || !(rot = (t_matrix**)malloc(sizeof(t_matrix*) * 6)))
		return (NULL);
	i = 0;
	while (i < 4)
	{
		j = i + 1;
		while (j < 4)
		{
			id_rot = (i == 0) ? j - 1: i + j; 
			if (!(rot[id_rot] = set_one_rot_4d(i, j, angle->m[id_rot])))
				return (NULL);	
			j++;
		}
		i++;
	}
	return (rot);
}

int			free_rotation_4d(t_matrix **rot)
{
	int	i;

	i = 0;
	while (i < 6)
	{
		matrix_free(rot + i);
		i++;
	}
	free(rot);
	return (1);
}

t_matrix	*set_rot_4d(t_matrix *angle)
{
	int			i;
	t_matrix	**rot;
	t_matrix	*result;
	t_matrix	*tmp;

	if (!angle || !(rot = get_the_rot4d(angle))
		|| !(result = matrix_identity(4)))
		return (NULL);
	i = 0;
	while (i < 6)
	{
		tmp = matrix_product(result, rot[i]);
		matrix_free(&result);
		result = tmp;
		i++;
	}
	free_rotation_4d(rot);
	return (result);
}

t_matrix	**creat_vertice()
{
	int			i;
	int			j;
	t_matrix	**vertice;

	if (!(vertice = (t_matrix**)malloc(sizeof(t_matrix*) * 16)))
		return (NULL);
	i = 0;
	while (i < 16)
	{
		if (!(vertice[i] = matrix_init(1, 4)))
			return (NULL);
		j = 0;
		while (j < 4)
		{
			vertice[i]->m[j] = ((double)((i & (1 << j)) != 0) - 0.5) * 200;
			j++;
		}
		i++;
	}
	return (vertice);
}

/*
	La ca serai cool de donner les lien en fonction de l'index
*/

void	define_link(t_matrix **vertice)
{
	int	i;
	int	j;
	int	id;
	int	coef;

	dprintf(1, "**************************************************\n");
	i = 0;
	while (i < 16)
	{
		j = 0;
		dprintf(1, "point[%d] (%d.%d.%d.%d)is linked with:", i, (!!(i & 8)), (!!(i & 4)), (!!(i & 2)), (!!(i & 1)));
		while (j < 4)
		{
			coef = ((i & (1 << j)) == 0) ? 1 : -1; 
			id = i + (coef * (1 << j));
			dprintf(1, "	{%d:	%d.%d.%d.%d}", id, (!!(id & 8)), (!!(id & 4)), (!!(id & 2)), (!!(id & 1)));
			(void)vertice;
			//	il exist un liens entre vertice[i] et vertice[id]
			j++;
		}
		dprintf(1, "\n");
		i++;
	}
}


void	conique_adapte(t_matrix *vect)
{
	double	norme;

	norme = vect->m[2] / 200;
	vect->m[0] /= norme;
	vect->m[1] /= norme;
}

void	rotate_vertice(t_matrix **vertice, double *ang, t_cam *cam)
{
	int			i;
	t_matrix	*ang_rot;
	t_matrix	*rot;
	t_matrix	*rot_cam;
	t_matrix	*tmp;
	t_matrix	*tmp2;

	if (!vertice || !*vertice || !ang || !(ang_rot = vect_new_vert(ang, 6))
		|| !(rot = set_rot_4d(ang_rot))
		|| !(rot_cam = set_rotate(cam->rot->m[0], cam->rot->m[1], cam->rot->m[1])))
		return ;
//dprintf(1, "*******\n");
//	matrix_display(cam->rot);
	i = 0;
	while (i < 16)
	{
		if (!(tmp = matrix_product(rot, vertice[i])))
			return ;
		matrix_free(vertice + i);
		matrix_sub_in(tmp, cam->pos, tmp);
		//	ici il faudrait faire la rotation
		tmp->y = 3;
		tmp2 = matrix_product(rot_cam, tmp);
		conique_adapte(tmp2);
//	matrix_display(tmp2);
		matrix_free(&tmp);
		vertice[i] = tmp2;
		i++;
	}
	matrix_free(&ang_rot);
	matrix_free(&rot);
}


/*
	-on initialise les point de base
	
	-on defini une rotation
	-on l'applique a tout les points
	-on 

*/


/*

c ->  cos(ang)
s ->  sin(ang)
S -> -sin(ang)

[0] [1] = [0][0] [0][1] [1][0] [1][1] 
cS..
sc..
..1.
...1

[0] [2] = [0][0] [0][2] [2][0] [2][2]
c.S.
.1..
s.c.
...1

[0] [3] = [0][0] [0][3] [3][0] [3][3]
c..S
.1..
..1.
s..c

[1] [2] = [1][1] [1][2] [2][1] [2][2]
1...
.cs.
.SC.
...1

[1] [3] = [1][1] [1][3] [3][1] [3][3]
1...
.c.s
..1.
.S.C

[2] [3] = [2][2] [2][3] [3][2] [3][3]
1...
.1..
..cs
..Sc

01	->	0
02	->	1
03	->	2
12	->	3
13	->	4
32	->	5

(i == 0) ? i + j - 1: i + j ;


0000
0000
0000
0000

*/
