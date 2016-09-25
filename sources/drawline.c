/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   drawline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpirsch <jpirsch@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/12/30 14:38:59 by jpirsch           #+#    #+#             */
/*   Updated: 2016/09/18 14:28:43 by fjanoty          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <mlx.h>
#include <math.h>
#include "fdf.h"
#include "libft.h"
#include <unistd.h>
#include <stdio.h>

//void	adapt_point(t_cam *c, t_matrix ***pt, int size_x, int size_y);


void	draw_line(t_env *e, t_matrix *mat_line)
{
	int			i;
	int			size;
	t_matrix	*diff;
	t_matrix	*org;
	t_matrix	*print;

//	dprintf(1, "\n");
	if (!(mat_line)
		|| !(diff = matrix_init(6, 1))
		|| (!(org = matrix_init(6, 1))))
		return ;
	i = -1;
	ft_memmove(org->m, mat_line->m, sizeof(double) * 6);
	ft_memmove(diff->m, mat_line->m + 6, sizeof(double) * 6);
	size = (int)(mat_line->m[NORME] + 0.5);
	while (++i < size)
	{
		print = matrix_add(org, diff); 
		vectpx_to_img(e, print);
		matrix_free(&org);
		org = print;
	}
	matrix_free(&diff);
	matrix_free(&org);
}

t_matrix	*init_mat_line(t_matrix *pt1, t_matrix *pt2
			, t_matrix *c1, t_matrix *c2)
{
//	dprintf(1, "SIR! \n");
	t_matrix	*mat_line;
	t_matrix	*diff;
	double		norme;


	mat_line = NULL;
	diff = NULL;
	if (!(mat_line = matrix_init(14, 1))
		|| !pt1 || !pt2 || !c1 || !c2
		|| ((!(diff = matrix_sub(pt2, pt1)) && matrix_free(&mat_line))))
	{
		if (!(mat_line))
			dprintf(1, "no mat_line\n");
		if (!(pt1))
			dprintf(1, "no pt1\n");
		if (!(pt2))
			dprintf(1, "no pt2\n");
		if (!(c1))
			dprintf(1, "no c1\n");
		if (!(c2))
			dprintf(1, "no c2\n");
		if (!(diff))
			dprintf(1, "no diff\n");
	//	return (NULL);
	}
	diff->m[Z] = 0;
	norme = matrix_dot_product(diff, diff);
	norme = sqrt(norme);
	mat_line->m[NORME] = norme;
	matrix_scalar_product(diff, 1 / norme);
	ft_memmove(mat_line->m, pt1->m, sizeof(double) * 3);
	ft_memmove(mat_line->m + 3, c1->m, sizeof(double) * 3);
	ft_memmove(mat_line->m + 6, diff->m, sizeof(double) * 3);
	matrix_free(&diff);
	matrix_scalar_product(diff = matrix_sub(c2, c1), 1 / norme);
	ft_memmove(mat_line->m + 9, diff->m, sizeof(double) * 3);
	matrix_free(&diff);
//	dprintf(1, "Yes SIR!\n");
	return (mat_line);
}

t_matrix	*init_mat_line2(t_matrix *pt_color, t_matrix *pt3, t_matrix *c3)
{
	t_matrix	*pt_inter;
	t_matrix	*color_inter;
	t_matrix	*mat_line;
	t_matrix	*diff;
	double		norme;

	if (!(pt_color))
		dprintf(1, "	pt_color\n");
	if (!(pt3))
		dprintf(1, "	pt3\n");
	if (!(c3))
		dprintf(1, "	c3\n");
	if (!(mat_line = matrix_init(14, 1))
		|| !pt_color || !pt3 || !c3
		|| !(pt_inter = matrix_init(1, 3))
		|| !(color_inter = matrix_init(1, 3)))
		return (NULL);
	ft_memmove(pt_inter->m, pt_color->m, sizeof(double) * 3);
	if (!(diff = matrix_sub(pt_inter, pt3)) && matrix_free(&mat_line))
		return (NULL);
	diff->m[Z] = 0;
	norme = matrix_dot_product(diff, diff);
	norme = sqrt(norme);
	mat_line->m[NORME] = norme;
	diff = matrix_scalar_product(diff, 1 / norme);
	ft_memmove(mat_line->m, pt3->m, sizeof(double) * 3);
	ft_memmove(mat_line->m + 3, c3->m, sizeof(double) * 3);
	ft_memmove(mat_line->m + 6, diff->m, sizeof(double) * 3);
	matrix_free(&diff);
	ft_memmove(color_inter->m, pt_color->m + 3, sizeof(double) * 3);
	matrix_scalar_product(diff = matrix_sub(color_inter, c3), 1 / norme);
	ft_memmove(mat_line->m + 9, diff->m, sizeof(double) * 3);
	matrix_free(&diff);
	matrix_free(&color_inter);
	matrix_free(&pt_inter);
	return (mat_line);
}


int		draw_triangle(t_env *e, t_matrix *mat_line, t_matrix *pt3, t_matrix *c3)
{
	int			i;
	int			size;
	t_matrix	*diff;
	t_matrix	*org;
	t_matrix	*print;
	t_matrix	*mat_line2;

	if ((!(mat_line))
		|| (!(diff = matrix_init(6, 1)))
		|| (((!(org = matrix_init(6, 1)) && matrix_free(&diff)))))
	{
		ft_putstr("y a embrouille\n");
		return (0);
	}
	ft_memmove(org->m, mat_line->m, sizeof(double) * 6);
	ft_memmove(diff->m, mat_line->m + 6, sizeof(double) * 6);
	size = (int)(mat_line->m[NORME] + 0.5);
	i = -1;
	while (++i < size)
	{
		print = matrix_add(org, diff); 
		if (!(mat_line2 = init_mat_line2(print, pt3, c3)))
		{
			ft_putstr("Yapa\n");
		}
		draw_line(e, mat_line2);
		matrix_free(&mat_line2);
		matrix_free(&org);
		org = print;
	}
	matrix_free(&diff);
	matrix_free(&print);
	return (1);
}

void	free_vertice(t_matrix ***vertice)
{
	int	i;

	if (!vertice || !*vertice || !**vertice)
		return ;
	i = 0;
	while (i < 16)
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


void	draw_link4d(t_env *e, t_matrix **vertice)
{
	int	i;
	int	j;
	int	id;
	int	coef;
	t_matrix	*c1;
	t_matrix	*c2;
	t_matrix	*mat_line;

	if (!(c1 = vect_new_vertfd(200, 200, 200)))
		return ;
	if (!(c2 = vect_new_vertfd(200, 200, 200)))
		return ;
	i = 0;
	while (i < 16)
	{
		j = 0;
		while (j < 4)
		{
			coef = ((i & (1 << j)) == 0) ? 1 : -1; 
			id = i + (coef * (1 << j));
			if (!(mat_line = init_mat_line(vertice[i], vertice[id], c1, c2)))
				;
			else
			{
				draw_line(e, mat_line);
				matrix_free(&mat_line);
			}
			j++;
		}
		i++;
	}
}

void	main_work(t_env *e)
{
	e->list_pt = creat_vertice();
	rotate_vertice(e->list_pt, e->ang);	
//	define_link(e->list_pt);
	draw_link4d(e, e->list_pt);
	mlx_put_image_to_window(e->mlx, e->win, e->img, 0, 0);
	mlx_do_sync(e->mlx);
	free_vertice(&(e->list_pt));
}
