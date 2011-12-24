/*
 * plot2d.cc

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

#include <glplot/plot2d.hh>
#include <glplot/misc.hh>
#include <string>
#include <limits>
#include <string.h>
#include <vmath/vmath.h>

using namespace std;
using namespace glplot;
/* Constructor */
plot2d::plot2d() : core2d() {

}

void plot2d::f_gl_update(double & in_f_min_x,double & in_f_max_x,double & in_f_min_y,double & in_f_max_y,double & in_f_min_z,double & in_f_max_z) {
	for (std::map<uint, curve>::iterator pc_it = _m_curves.begin(); pc_it
			!= _m_curves.end(); pc_it++) {
		(*pc_it).second.f_gl_update();
	}


	for (std::map<uint, curve>::iterator pc_it = _m_curves.begin(); pc_it
			!= _m_curves.end(); pc_it++) {
		(*pc_it).second.f_gl_update_minmax(in_f_min_x, in_f_max_x, in_f_min_y,
				in_f_max_y, in_f_min_z, in_f_max_z);
	} 
}

/* Main rendering method */
void plot2d::f_gl_render_scaled(void) {

	/* Render curves */
	glLineWidth(1);

	/* Core rendering */
	for (std::map<uint, curve>::iterator pc_it = _m_curves.begin(); pc_it
			!= _m_curves.end(); pc_it++) {
		(*pc_it).second.f_gl_render();
	}

}


/* External functions */
void plot2d::f_clear(void) {
	_m_curves.clear();
}

/* Main plot function */
void plot2d::f_plot(GLdouble * in_ad_gnuplot, size_t in_sz_data,
		enum et_curve_type e_type, uint in_i_index) {
	_m_curves[in_i_index].f_init(NULL, in_ad_gnuplot, in_sz_data, e_type);
}
#if 0
void plot2d::f_init(void) {
	/* Call base class init method */
	core2d::f_init();

	/* Initialize font */



	/* Initialize shaders */
	//f_gl_init_shaders();

}
#endif
#if 0
void plot2d::f_gl_compile_shader(string const & in_str_filename,
		GLenum in_e_shader, GLuint in_i_shader_program) {
	/* Create shader */
	GLuint i_shader = glCreateShader(in_e_shader);

	/* Read file content */
	const char * pc_tmp;
	try {
		pc_tmp = strdup(f_misc_file_content(in_str_filename).c_str());
	} catch (...) {
		cerr << "Unable to open " << in_str_filename << endl;
		exit(-1);
	}
	cout << pc_tmp << endl;
	//pc_tmp = new char[pc_tmp];

	/* Load shader source code */
	glShaderSource(i_shader, 1, &pc_tmp, NULL);

	/* Compile shader */
	glCompileShader(i_shader);

	/* Get result of compilation */
	GLint ec;
	glGetShaderiv(i_shader, GL_COMPILE_STATUS, &ec);
	if (!ec) {
		GLchar ac_log[1024];
		glGetShaderInfoLog(i_shader, sizeof(ac_log), 0, ac_log);

		cerr << "Could not compile shader: " << in_str_filename << endl;
		cerr << ac_log << endl;
		glDeleteShader(i_shader);
		exit(-1);
	}

	/* Attach shader to program */
	glAttachShader(in_i_shader_program, i_shader);

	free((void*) pc_tmp);
}

void plot2d::f_gl_init_shaders(void) {

	/* Create shader program */
	GLuint i_shader_program = glCreateProgram();

	/* Geometry parameters */
	{
		PFNGLPROGRAMPARAMETERIEXTPROC glProgramParameteriEXT =
				(PFNGLPROGRAMPARAMETERIEXTPROC) glXGetProcAddress(
						(GLubyte*) "glPatchParameteri");
		glProgramParameteriEXT(i_shader_program, GL_GEOMETRY_INPUT_TYPE_EXT,
				GL_LINES);
		glProgramParameteriEXT(i_shader_program, GL_GEOMETRY_OUTPUT_TYPE_EXT,
				GL_LINES);

		int temp;
		glGetIntegerv(GL_MAX_GEOMETRY_OUTPUT_VERTICES_EXT, &temp);
		cout << temp << endl;
		glProgramParameteriEXT(i_shader_program, GL_GEOMETRY_VERTICES_OUT_EXT,
				temp);
	}

	/* Compile shader */
#if 1
	f_gl_compile_shader("src/glsl/plot2d.vert.glsl", GL_VERTEX_SHADER,
			i_shader_program);
#endif
#if 0
	f_gl_compile_shader("src/glsl/plot2d.frag.glsl", GL_FRAGMENT_SHADER,
			i_shader_program);
#endif
#if 1
	f_gl_compile_shader("src/glsl/plot2d.geom.glsl", GL_GEOMETRY_SHADER_EXT,
			i_shader_program);
#endif

	/* Link shaders */
	glLinkProgram(i_shader_program);

	/* Get link status */
	GLint ec;
	glGetProgramiv(i_shader_program, GL_LINK_STATUS, &ec);
	if (!ec) {
		GLchar ac_log[1024];
		glGetProgramInfoLog(i_shader_program, sizeof(ac_log), 0, ac_log);

		cerr << "Could not link roam program: " << endl;
		cerr << ac_log << endl;
		glDeleteProgram(i_shader_program);
		exit(-1);
	}
	_i_shader_program = i_shader_program;

	_i_uniform_m_mvp = glGetUniformLocation(_i_shader_program, "m_mvp");
	_i_uniform_f_min_x = glGetUniformLocation(_i_shader_program, "f_min_x");
	_i_uniform_f_max_x = glGetUniformLocation(_i_shader_program, "f_max_x");


	try {
		_s_font_2.f_init("/usr/share/fonts/truetype/freefont/FreeSerif.ttf");
	} catch (std::exception e) {
		cout << "Standard exception: " << e.what() << endl;
	}
}
#endif
