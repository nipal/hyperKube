/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hyperKube_degn.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjanoty <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/09/27 06:47:13 by fjanoty           #+#    #+#             */
/*   Updated: 2016/09/27 12:27:38 by fjanoty          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

t_matrix	*set_one_rot_degn(int c1, int c2, double ang, int deg)
{
	t_matrix	*rot;

	if (!(rot = matrix_identity(deg)))
		return (NULL);
	rot->m[c1 + c1 * deg] = cos(ang);
	rot->m[c2 + c2 * deg] = cos(ang);
	rot->m[c2 + c1 * deg] = -sin(ang) * ((c1 == 0) ? 1 : -1);
	rot->m[c1 + c2 * deg] = sin(ang) * ((c1 == 0) ? 1 : -1);
	return (rot);
}

t_matrix	**get_the_rot_degn(t_matrix *angle, int deg)
{
	int			i;
	int			j;
	int			id_rot;
	t_matrix	**rot;
	int			max;

	max = ((deg - 1) * deg) / 2;
	if (!angle || !(rot = (t_matrix**)malloc(sizeof(t_matrix*) * max)))
//dprintf(1, "\nmax of max:%d\n\n", max);
		return (NULL);
	i = 0;
	id_rot = 0;
	while (i < deg)
	{
		j = i + 1;
		while (j < deg)
		{
			if (!(rot[id_rot] = set_one_rot_degn(i, j, angle->m[id_rot], deg)))
				return (NULL);	
			id_rot++; 
			j++;
		}
		i++;
	}
	return (rot);
}

int			free_rotation_degn(t_matrix **rot, int deg)
{
	int	i;
	int	max;

	max = ((deg - 1) * deg) / 2;
	i = 0;
	while (i < max)
	{
		matrix_free(rot + i);
		i++;
	}
	free(rot);
	return (1);
}

t_matrix	*set_rot_degn(t_matrix *angle, int deg)
{
	int			i;
	t_matrix	**rot;
	t_matrix	*result;
	t_matrix	*tmp;
	int			max;

	max = ((deg - 1) * deg) / 2;
	if (!angle || !(rot = get_the_rot_degn(angle, deg))
		|| !(result = matrix_identity(deg)))
		return (NULL);
	i = 0;
	while (i < max)
	{
		tmp = matrix_product(result, rot[i]);
		matrix_free(&result);
		result = tmp;
		i++;
	}
free_rotation_degn(rot, deg);
	return (result);
}

t_matrix	**creat_vertice_degn(int deg)
{
	int			i;
	int			j;
	t_matrix	**vertice;
	int			max;

	max = 1 << deg;
	if (!(vertice = (t_matrix**)malloc(sizeof(t_matrix*) * max)))
		return (NULL);
	i = 0;
	while (i < max)
	{
		if (!(vertice[i] = matrix_init(1, deg)))
			return (NULL);
		j = 0;
		while (j < deg)
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

void	define_link_degn(t_matrix **vertice, int deg)
{
	int	i;
	int	j;
	int	id;
	int	coef;
	int	max;

	dprintf(1, "**********************\n");
	max = 1 << deg; 
	i = 0;
	while (i < max)
	{
		j = 0;
		dprintf(1, "point[%d] (%d.%d.%d.%d)is linked with:", i, (!!(i & 8)), (!!(i & 4)), (!!(i & 2)), (!!(i & 1)));
		while (j < deg)
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

void	rotate_vertice_degn(t_matrix **vertice, double *ang, t_cam *cam, int deg)
{
	int			i;
	t_matrix	*ang_rot;
	t_matrix	*rot;
	t_matrix	*rot_cam;
	t_matrix	*tmp;
	t_matrix	*tmp2;
	int			max;

	max = 1 << deg;
	if (!vertice || !*vertice || !ang || !(ang_rot = vect_new_vert(ang, ((deg * (deg - 1)) / 2)))
		|| !(rot = set_rot_degn(ang_rot, deg))
		|| !(rot_cam = set_rotate(cam->rot->m[0], cam->rot->m[1], cam->rot->m[1])))
		return ;
//dprintf(1, "*******\n");
//	matrix_display(cam->rot);
	i = 0;
	while (i < max)
	{
		if (!(tmp = matrix_product(rot, vertice[i])))
			return ;
		matrix_free(vertice + i);
		tmp->y = cam->pos->y;
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


int		they_out(t_matrix *p1, t_matrix *p2)
{
	if (!p1 || !p2 || p1->m[2] < 0 || p2->m[2] < 10)
		return (1);
	return (0);
}

void	draw_link_degn(t_env *e, t_matrix **vertice, int deg)
{
	int			i;
	int			j;
	int			id;
//	int			coef;
	t_matrix	*c1;
	t_matrix	*c2;
	t_matrix	*mat_line;
	int			max;

	max = 1 << deg;
	if (!(c1 = vect_new_vertfd(150, 150, 150)))
		return ;
	if (!(c2 = vect_new_vertfd(150, 150, 150)))
		return ;
	i = 0;
	while (i < max)
	{
		j = 0;
		while (j < deg)
		{
//			coef = ((i & (1 << j)) == 0) ? 1 : -1; 
			id = i + ((1 << j));
			if (i & (1 << j) || (they_out(vertice[i], vertice[id]))
				|| (!(mat_line = init_mat_line(vertice[i], vertice[id], c1, c2))))
				;//dprintf(1, "i:%d	j:%d\n", i, j);
			else
			{
				//dprintf(1, "i:%d	j:%d\n", i, j);
				draw_line(e, mat_line);
				matrix_free(&mat_line);
			}
			j++;
		}
		i++;
	}
}



void	free_vertice_degn(t_matrix ***vertice, int deg)
{
	int	i;
	int	max;

	max = 1 << deg;
	if (!vertice || !*vertice || !**vertice)
		return ;
	i = 0;
	while (i < max)
	{
		matrix_free(*vertice + i);
		i++;
	}
	if (*vertice)
	{
		free(*vertice);
		*vertice = NULL;
	}
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
