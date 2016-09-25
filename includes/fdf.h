/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpirsch <jpirsch@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/12/29 02:21:11 by jpirsch           #+#    #+#             */
/*   Updated: 2016/09/18 08:04:39 by fjanoty          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FDF_H
# define FDF_H

# include "c_maths.h"
# include <errno.h>
# include <string.h>

# define SIZE_Y 640
# define SIZE_X 640

# define R 0
# define G 1
# define B 2

# define X 0
# define Y 1
# define Z 2

# define NORME 12

typedef struct			s_key
{
	int					echap;
	int					decal_down;
	int					decal_up;
	int					decal_right;
	int					decal_left;
	int					zoom;
	int					zoom_back;
	int					cte1_increase;
	int					cte1_decrease;
	int					isometric;
	int					parallel;
	int					conic;
	int					scale_increase;
	int					scale_decrease;
	int					r;
	int					g;
	int					b;
	int					rot_x1;
	int					rot_y1;
	int					rot_z1;
	int					rot_x2;
	int					rot_y2;
	int					rot_z2;
	int					rot_cam_x1;
	int					rot_cam_x2;
	int					rot_cam_y1;
	int					rot_cam_y2;
	int					rot_cam_z1;
	int					rot_cam_z2;
	int					speed_up;
	int					speed_down;
}						t_key;


typedef	struct			s_cam
{
	t_matrix			**corner;
	t_matrix			**base;
	t_matrix			*pos;
	t_matrix			*dir;
	t_matrix			*rot;
}						t_cam;
//*/


# define PRINT_DIAG 0
typedef struct			s_env
{
	void				*mlx;
	void				*win;
	void				*img;
	char				*data;
	double				*z_buffer;
	int					size_line;
	int					ecr_x;
	int					ecr_y;
	int					depth;
	int					endian;
	int					proj;
	double				scale;
	double				cte1;
	int					decalx;
	int					decaly;
	int					zoom;
	unsigned char		r;
	unsigned char		g;
	unsigned char		b;
	t_key				key;
	int					size_map_x;
	int					size_map_y;
	double				z_min;
	double				z_max;
	double				rot_x;
	double				rot_y;
	double				rot_z;
	double				speed;
	t_cam				*cam;
	t_matrix			**list_pt;
	double				ang[6];
}						t_env;

typedef struct			s_point
{
	int					x;
	int					y;
	int					z;
	unsigned int		color;
}						t_point;


/*
** hook
*/
void					init_t_key(t_key *key);
int						key_press(int keycode, t_env *e);
int						key_release(int keycode, t_env *e);

//	redessine et set les vars en fonction des inputs
int						loop_hook(t_env *e);

/*
** mlx_env
*/
void					px_to_img(t_env *e, int x, int y, int color);
void					vectpx_to_img(t_env *e, t_matrix *pos_color);
void					print_state(t_env *e);

//	Fonction preparant l'env et qui lance le loop hook
void					env();
/*
** coord
*/

/*
 **	cam
 **/
t_cam					*init_cam(double fov_y, double fov_x, t_env *e);
void					describe_cam(t_cam *cam);
/*
 ** print_map	
 * */
void					print_map(t_env *e, t_cam *cam, t_matrix ***map);

/*
** draw
*/
//fonctions de dessin (putline, put map of vect)
void					draw_point(t_env *e);
/*
** parse
*/
int						**parse(int fd);
char					*maps_name(char *str);


//t_matrix	***get_map(double *z_max, double *z_min);
t_matrix				***get_map(t_env *e);
void					draw_line(t_env *e, t_matrix *mat_line);
t_matrix				*init_mat_line(t_matrix *pt1, t_matrix *pt2, t_matrix *c1, t_matrix *c2);
int						**get_the_map(int fd, int *x_max, int *y_max);


void					free_map(t_matrix	****map, t_env *e);
int						free_int_map(t_env *e);
int						free_cam(t_env *e);
int						ft_exit(t_env **e);

/*
**	fonction pour l'hypercube et des amelioration de c_math
*/
t_matrix				*matrix_identity(int size);
t_matrix				*set_one_rot_4d(int c1, int c2, double ang);
t_matrix				**get_the_rot4d(t_matrix *angle);
int						free_rotation_4d(t_matrix **rot);
t_matrix				*set_rot_4d(t_matrix *angle);
t_matrix				**creat_vertice();
void					define_link(t_matrix **vertice);
void					rotate_vertice(t_matrix **vertice, double *ang);

#endif
