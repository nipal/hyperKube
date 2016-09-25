/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mlx_env.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpirsch <jpirsch@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/01/10 04:08:06 by jpirsch           #+#    #+#             */
/*   Updated: 2016/09/18 07:48:42 by fjanoty          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <mlx.h>
#include "libft.h"
#include "fdf.h"
#include <stdio.h>

void	px_to_img(t_env *e, int x, int y, int color)
{
//	(void)color;
//	printf("color:%0x\n", color);
//	printf("	r:%0x\n", (color & 0x00FF0000) >> 16);
//	printf("	v:%0x\n", (color & 0x0000FF00) >> 8);
//	printf("	b:%0x\n", (color & 0x000000FF) );
	y = 100;
	e->data[y * e->size_line + x * 4 + 2] = (color & 0x00FF0000) >> 16;
	e->data[y * e->size_line + x * 4 + 1] = (color & 0x0000FF00) >> 8;
	e->data[y * e->size_line + x * 4] = (color & 0x000000FF);
//	dprintf(1, "x:%d y:%d  color:%d\n", x, y, color);
}

void	vectpx_to_img(t_env *e, t_matrix *pos_color)
{
	int	x;
	int	y;
	int	r;
	int	g;
	int	b;
	double	coef;

	coef = 1;
	x = (int) pos_color->m[0];
	y = (int) pos_color->m[1];
	x += SIZE_Y / 2;
	y += SIZE_X / 2;
	if ( x < 0 || x >= e->ecr_x || y < 0 || y >= e->ecr_y || pos_color->m[2] < 0)
	//	|| (pos_color->m[2] >= e->z_buffer[x + y * e->ecr_x] && e->z_buffer[x + y * e->ecr_x]))
	{
//		dprintf(1, "x_max:%d y_max:%d\n", e->size_line  / 4, 990);
//		dprintf(1, "out of window x:%d y:%d\n", x, y);
		return ;
	}
	if (pos_color->m[2] != 0)
	{
		coef =  pos_color->m[2] / 300;
	}
	e->z_buffer[x + y * e->ecr_x] = pos_color->m[2];
	r = (int) ((pos_color->m[3] + 0.5)) / (coef);
	g = (int) ((pos_color->m[4] + 0.5)) / coef;
	b = (int) ((pos_color->m[5] + 0.5)) / coef;
	r = (r > 255) ? 255 : r;
	g = (g > 255) ? 255 : g;
	b = (b > 255) ? 255 : b;
	r = (r < 0) ? 0 : r;
	g = (g < 0) ? 0 : g;
	b = (b < 0) ? 0 : b;
	e->data[y * e->size_line + x * 4 + 2] = r;
	e->data[y * e->size_line + x * 4 + 1] = g;
	e->data[y * e->size_line + x * 4] = b;
//	dprintf(1, "======= YEAHHHH BABY  ====== x:%d y:%d  r:%d v:%d b:%d ilne:%d\n", x, y, r, g, b, e->size_line);
}

void	string_put(char *s1, char *s2, t_env *e, int y)
{
	char *str;

	str = ft_strnew(60);
	str = ft_strcpy(str, s1);
	str = ft_strcat(str, s2);
	mlx_string_put(e->mlx, e->win, 1200, y, 0xFFFF00, str);
	free(str);
}

void	print_state(t_env *e)
{
	char	*str;

	string_put("Zoom : ", ft_itoa(e->zoom), e, 50);
	(e->proj == 0) ? str = ft_strdup("isometric") : (void)e->proj;
	(e->proj == 1) ? str = ft_strdup("parallel") : (void)e->proj;
	(e->proj == 2) ? str = ft_strdup("conic") : (void)e->proj;
	string_put("Projection : ", str, e, 60);
	ft_strdel(&str);
	string_put("Offset X : ", ft_itoa(e->decalx), e, 70);
	string_put("Offset Y : ", ft_itoa(e->decaly), e, 80);
	str = ft_strnew(20);
	str = ft_strcat(str, " R = ");
	str = ft_strcat(str, ft_itoa((int)e->r));
	str = ft_strcat(str, " G = ");
	str = ft_strcat(str, ft_itoa((int)e->g));
	str = ft_strcat(str, " B = ");
	str = ft_strcat(str, ft_itoa((int)e->b));
	string_put(str, "", e, 90);
	ft_strdel(&str);
}

/*	ici il faudrait stoquer les dimetion maximale de la map
 * 	on a deja le max x et y il maque le max et le min en z*/

void	env()
{
	t_env	e;
	t_cam	*cam;

	e.rot_x = (20.0 / 360.0)  * 2 * M_PI;
	e.rot_y = (20.0 / 360.0) * 2 * M_PI;
	e.rot_z = (0.0 / 360.0) * 2 * M_PI;
	if (!(e.mlx = mlx_init()))
		return ;
	e.win = mlx_new_window(e.mlx, SIZE_Y, SIZE_X, "Leu test");
	e.img = mlx_new_image(e.mlx, SIZE_Y, SIZE_X);
	e.data = mlx_get_data_addr(e.img, &e.depth, &e.size_line, &e.endian);
	e.z_buffer = (double*)malloc(sizeof(double) * SIZE_X * SIZE_Y);
	e.ecr_x = SIZE_Y;
	e.ecr_y = SIZE_X;
	e.speed = 0;
	if (!(cam = init_cam(60.0/360.0 * M_PI , 60.0/360.0 * M_PI, &e)))
		return ;
	ft_bzero(e.ang, sizeof(double) * 16);
//	if (!(e.list_pt = creat_vertice()))
//		return (NULL);
	e.cam = cam;
	init_t_key(&e.key);
	mlx_hook(e.win, 2, 1, key_press, &e);
	mlx_hook(e.win, 3, 2, key_release, &e);
	mlx_loop_hook(e.mlx, loop_hook, &e);
	mlx_loop(e.mlx);
}
