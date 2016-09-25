/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eq_solve_2nd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpirsch <jpirsch@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/09/14 00:25:54 by jpirsch           #+#    #+#             */
/*   Updated: 2016/03/02 04:25:19 by jpirsch          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "c_maths.h"
#include <math.h>

t_roots	eq_solve_2nd(t_coefs coefs)
{
	double	delta;
	t_roots	r;

	r.x3 = 0;
	r.x4 = 0;
	r.is_real = 1;
	delta = coefs.b * coefs.b - 4 * coefs.a * coefs.c;
	if (delta > 0)
	{
		r.x1 = (-coefs.b - sqrt(delta)) / (2 * coefs.a);
		r.x2 = (-coefs.b + sqrt(delta)) / (2 * coefs.a);
	}
	else if (delta == 0)
	{
		r.x1 = (-coefs.b) / (2 * coefs.a);
		r.x2 = 0;
	}
	else if (delta < 0)
		r.is_real = 0;
	return (r);
}
