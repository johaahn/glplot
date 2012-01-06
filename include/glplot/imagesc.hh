/*
 * imagesc.hh
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

#ifndef IMAGESC_HH_
#define IMAGESC_HH_

namespace glplot {

struct vertex_image {
	GLfloat d_value;
};

class imagesc : public core2d {
	bool _b_first;

	/* Min , Max of buffer */
	double _f_min;
	double _f_max;

	/* Data value */
	vertex_image * _as_data;
	size_t _sz_data;

	/* Colormap */
	GLfloat * _af_colormap;
	size_t _sz_colormap;

	/* image size */
	GLint _sz_x;
	GLint _sz_y;

	/* TBO */
	GLuint _i_tbo;
	GLuint _i_colormap_tbo;

	/* TEX */
	GLuint _i_tex;
	GLuint _i_colormap_tex;

	 /* Shader stuffs */
	 GLuint _i_shader_program;
	 GLuint _i_uniform_f_min;
	 GLuint _i_uniform_f_max;
	 GLuint _i_uniform_sz_x;
	 GLuint _i_uniform_sz_y;
	 GLuint _i_uniform_sz_colormap;
	 GLuint _i_uniform_m_mvp;
	 GLuint _i_uniform_i_tbo;
	 GLuint _i_uniform_i_colormap_tbo;

	 /* Update counter */
	uint32_t _i_data_update;
	uint32_t _i_tbo_update;

public :
	/* Constructor */
	imagesc();

	/* GL calls */
	virtual void f_gl_render_scaled(void);
	virtual void f_gl_init(void);
	virtual void f_gl_update(double & in_f_min_x,double & in_f_max_x,double & in_f_min_y,double & in_f_max_y,double & in_f_min_z,double & in_f_max_z);
	/* External functions */
	void f_plot(double * in_ad_gnuplot, size_t in_sz_x, size_t in_sz_y);
	void f_plot(float * in_af_gnuplot, size_t in_sz_x, size_t in_sz_y);
};

}
#endif /* PLOT2D_HH_ */
