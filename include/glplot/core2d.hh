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

#include <glplot/core.hh>
#include <glplot/font.hh>
#include <string>

#ifndef CORE2D_HH_
#define CORE2D_HH_

namespace glplot {

class core2d : public core {

    GLdouble _af_model_matrix[16];
    GLdouble _af_proj_matrix[16];
    int _af_viewport[4];


    /* Border stuff */
    font::font_data _s_font;
    font::font _s_font_2;

    /* GLSL */
    std::string _str_glsl_path;
public:
    /* Zoom stuff */
    double _f_zoom_start_x, _f_zoom_start_y, _f_zoom_start_z;
    double _f_zoom_end_x, _f_zoom_end_y, _f_zoom_end_z;
    bool _b_zoom_draw;
    bool _b_zoom_auto;
    double _f_zoom_min_x, _f_zoom_min_y, _f_zoom_min_z;
    double _f_zoom_max_x, _f_zoom_max_y, _f_zoom_max_z;



public :
	core2d();

	/* Child methods */
	virtual void f_gl_init(void);
	virtual void f_gl_render(void);
	virtual void f_gl_render_scaled(void);
	virtual void f_gl_update(double & in_f_min_x,double & in_f_max_x,double & in_f_min_y,double & in_f_max_y,double & in_f_min_z,double & in_f_max_z);

	/* Zoom methods */
	void f_zoom_pos_start(uint32_t in_i_x, uint32_t in_i_y);
	void f_zoom_pos_update(uint32_t in_i_x, uint32_t in_i_y);
	void f_zoom_pos_end(uint32_t in_i_x, uint32_t in_i_y);
	void f_zoom_reset();

	/* shaders methods */
	void f_gl_shader_compile(std::string const & in_str_filename, GLenum in_e_shader, GLuint in_i_shader_program);
	void f_gl_shader_link(GLuint in_i_shader_program);
	void f_gl_shader_set_glsl_path(std::string const & in_str_path);
};

}
#endif /* CORE2D_HH_ */
