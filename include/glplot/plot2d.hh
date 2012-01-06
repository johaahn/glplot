/*
 * plot2d.hh
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

#include <glplot/core2d.hh>
#include <glplot/curve.hh>
#include <glplot/font.hh>

#include <map>

#ifndef PLOT2D_HH_
#define PLOT2D_HH_

namespace glplot {

class plot2d : public core2d {

	std::map<uint, curve> _m_curves;

    /* Shader stuffs */
    GLuint _i_shader_program;
    GLuint _i_uniform_f_min_x;
    GLuint _i_uniform_f_max_x;
    GLuint _i_uniform_m_mvp;

public :
	plot2d();

	/* Core child methods */
	virtual void f_gl_render_scaled(void);
	virtual void f_gl_update(double & in_f_min_x,double & in_f_max_x,double & in_f_min_y,double & in_f_max_y,double & in_f_min_z,double & in_f_max_z);

	/* Shader methods */
	//void f_gl_init_shaders(void);
	//void f_gl_compile_shader(std::string const & in_str_filename, GLenum in_e_shader, GLuint in_i_shader_program);

	/* External functions */
	void f_clear(void);
	void f_plot(GLdouble * in_ad_gnuplot, size_t in_sz_data, enum et_curve_type e_type = E_TYPE_STD, uint32_t in_i_index = 0);
};

}
#endif /* PLOT2D_HH_ */
