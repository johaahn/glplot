/*
 * curve.hh
 *
 *  Created on: 2 sept. 2011
 *
 * Copyright (C) 2010 <Johann Baudy> johann.baudy@gnu-log.net
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 3.0 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA
 */

#include <glplot/core.hh>
#include <map>

#ifndef CURVE_HH_
#define CURVE_HH_

namespace glplot {

enum et_curve_type {
	E_TYPE_STD,
};

struct st_vertex_curve {
	GLdouble d_x;
	GLdouble d_y;
	GLdouble d_z;
};

class curve  {
	st_vertex_curve * _as_data;
	size_t _sz_data;
	enum et_curve_type _e_type;

	double _f_min_x;
	double _f_min_y;
	double _f_min_z;
	double _f_max_x;
	double _f_max_y;
	double _f_max_z;

	uint32_t _i_data_update;


	uint32_t _i_vbo_update;
	bool _b_vbo_created;
	GLuint _i_vbo;
	size_t _sz_vbo;

	double _f_vbo_min_x;
	double _f_vbo_min_y;
	double _f_vbo_min_z;
	double _f_vbo_max_x;
	double _f_vbo_max_y;
	double _f_vbo_max_z;

public:
	curve();
	~curve();

	void f_init(GLdouble * in_ad_x, GLdouble * in_ad_y, size_t in_sz_data, enum et_curve_type e_type = E_TYPE_STD);
	void f_gl_render(void);
	void f_gl_update(void);
	void f_gl_update_minmax(double & in_f_min_x,double & in_f_max_x,double & in_f_min_y,double & in_f_max_y,double & in_f_min_z,double & in_f_max_z);

};
}
#endif /* PLOT2D_HH_ */
