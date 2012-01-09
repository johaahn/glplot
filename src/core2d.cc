/*
 * core2d.cc

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
#include <glplot/misc.hh>
#include <limits>
#include <string>
#include <string.h>
#include <glplot/vmath.hh>

using namespace std;
using namespace glplot;

core2d::core2d() {
	_b_zoom_draw = false;
	_b_zoom_auto = true;

	_str_glsl_path = "/usr/local/share/glplot/";
}

core2d::~core2d() {
	cout << _str_glsl_path << endl;
}

void core2d::f_zoom_pos_start(uint32_t in_i_x, uint32_t in_i_y) {
	cout << __FUNCTION__ << " " << in_i_x << " " << in_i_y << endl;
	GLdouble f_obj_x, f_obj_y, f_obj_z;
	gluUnProject(in_i_x, f_get_height() - in_i_y, 0, _af_model_matrix,
			_af_proj_matrix, _af_viewport, &f_obj_x, &f_obj_y, &f_obj_z);
	cout << "Project" << f_obj_x << " " << f_obj_y << " " << f_obj_z << " "
			<< endl;

	/* Save zoom start */
	_f_zoom_start_x = f_obj_x;
	_f_zoom_start_y = f_obj_y;
	_f_zoom_start_z = f_obj_z;

	_b_zoom_draw = true;
}
void core2d::f_zoom_pos_update(uint32_t in_i_x, uint32_t in_i_y) {
	if (_b_zoom_draw) {
		cout << __FUNCTION__ << " " << in_i_x << " " << in_i_y << endl;
		GLdouble f_obj_x, f_obj_y, f_obj_z;
		gluUnProject(in_i_x, f_get_height() - in_i_y, 0, _af_model_matrix,
				_af_proj_matrix, _af_viewport, &f_obj_x, &f_obj_y, &f_obj_z);
		cout << "Project" << f_obj_x << " " << f_obj_y << " " << f_obj_z << " "
				<< endl;
		/* Save zoom start */
		_f_zoom_end_x = f_obj_x;
		_f_zoom_end_y = f_obj_y;
		_f_zoom_end_z = f_obj_z;
	}

}
void core2d::f_zoom_pos_end(uint32_t in_i_x, uint32_t in_i_y) {
	if (_b_zoom_draw) {
		cout << __FUNCTION__ << " " << in_i_x << " " << in_i_y << endl;

		GLdouble f_obj_x, f_obj_y, f_obj_z;
		gluUnProject(in_i_x, f_get_height() - in_i_y, 0, _af_model_matrix,
				_af_proj_matrix, _af_viewport, &f_obj_x, &f_obj_y, &f_obj_z);
		cout << "Project" << f_obj_x << " " << f_obj_y << " " << f_obj_z << " "
				<< endl;

		/* Save zoom start */
		_f_zoom_end_x = f_obj_x;
		_f_zoom_end_y = f_obj_y;
		_f_zoom_end_z = f_obj_z;

		_f_zoom_min_x = min(_f_zoom_start_x, _f_zoom_end_x);
		_f_zoom_min_y = min(_f_zoom_start_y, _f_zoom_end_y);
		_f_zoom_min_z = min(_f_zoom_start_z, _f_zoom_end_z);

		_f_zoom_max_x = max(_f_zoom_start_x, _f_zoom_end_x);
		_f_zoom_max_y = max(_f_zoom_start_y, _f_zoom_end_y);
		_f_zoom_max_z = max(_f_zoom_start_z, _f_zoom_end_z);

		_b_zoom_draw = false;
		_b_zoom_auto = false;
	}
}

void core2d::f_zoom_reset() {
	_b_zoom_auto = true;
}

void core2d::f_gl_render(void) {
	/* Reset min max */
	double f_min_x = std::numeric_limits<double>::infinity();
	double f_max_x = -std::numeric_limits<double>::infinity();
	double f_min_y = std::numeric_limits<double>::infinity();
	double f_max_y = -std::numeric_limits<double>::infinity();
	double f_min_z = std::numeric_limits<double>::infinity();
	double f_max_z = -std::numeric_limits<double>::infinity();

	double f_scale_x, f_scale_y, f_scale_z;

	f_gl_update(f_min_x, f_max_x, f_min_y, f_max_y, f_min_z, f_max_z);

	if (!_b_zoom_auto) {
		f_min_x = _f_zoom_min_x;
		f_min_y = _f_zoom_min_y;
		f_min_z = _f_zoom_min_z;

		f_max_x = _f_zoom_max_x;
		f_max_y = _f_zoom_max_y;
		f_max_z = _f_zoom_max_z;
	}

	f_min_z = -1.0;
	f_max_z = 1.0;

	//cout << "X - min: " << f_min_x << ", max: " << f_max_x << endl;
	//cout << "Y - min: " << f_min_y << ", max: " << f_max_y << endl;
	//cout << "Z - min: " << f_min_z << ", max: " << f_max_z << endl;

	f_scale_x = 1.0 / (f_max_x - f_min_x);
	f_scale_y = 1.0 / (f_max_y - f_min_y);
	f_scale_z = 1.0 / (f_max_z - f_min_z);

	if (f_scale_x == std::numeric_limits<double>::infinity()) {
		f_scale_x = 1.0;
	}
	if (f_scale_y == std::numeric_limits<double>::infinity()) {
		f_scale_y = 1.0;
	}
	if (f_scale_z == std::numeric_limits<double>::infinity()) {
		f_scale_z = 1.0;
	}

	//cout << "Scale " << f_scale_x << ", " << f_scale_y << ", " << f_scale_z << endl;
	//cout << f_scale_z << endl;

	double f_scale_ui = 1.0 / 1.2;

	glPushMatrix();
	glColor3f(0.0, 0.0, 0.0);
	glTranslatef(0.2, 0.2, 0);

	glScalef(1.0 / 3.0, 1.0 / 3.0, 1.0 / 3.0);

	//glScalef(1.0/10.0, 1.0/10.0, 1.0/10.0);
	//glScalef(1.0/22.0, 1.0/22.0, 1.0/22.0);
	//glScalef(1.0/30.0, 1.0/30.0, 1.0/30.0);
	//glScalef(1.0/800.0, 1.0/800.0, 1.0/800.0);
	//glTranslatef(200, 200, 0);

	//_s_font_2.f_render_wchar(L'Ḁ');
	_s_font_2.f_render((wchar_t*) L"WHAOU abcdef !!!/a// || a/a ~'\"");
	glTranslatef(0.3, 0.3, 0);
	//_s_font_2.f_bitmap_render((wchar_t*)L"WHAOU abcdef !!!/a// || a/a ~'\"");
#if 0
	glBegin(GL_QUADS);
	glVertex3f(-500.0, -500.0, 0.0);
	glVertex3f(-500.0, 500.0, 0.0);
	glVertex3f(500.0, 500.0, 0.0);
	glVertex3f(500.0, -500.0, 0.0);
	glEnd();
#endif
	glPopMatrix();
	//return;

	/* Apply ui scale */
	glPushMatrix();
	glScalef(f_scale_ui, f_scale_ui, f_scale_ui);
	glTranslatef(0.1, 0.1, 0.1);

	/* Push UI state */
	glPushMatrix();

	/* Apply scale and translation of current state */
	glScalef(f_scale_x, f_scale_y, f_scale_z);
	glTranslatef(-f_min_x, -f_min_y, -f_min_z);

	{
		glGetDoublev(GL_MODELVIEW_MATRIX, _af_model_matrix);
		glGetDoublev(GL_PROJECTION_MATRIX, _af_proj_matrix);
		glGetIntegerv(GL_VIEWPORT, _af_viewport);
	}

	{
		double af_equ0[] = { 1.0, 0.0, 0.0, -f_min_x };
		glClipPlane(GL_CLIP_PLANE0, af_equ0);

		double af_equ1[] = { -1.0, 0.0, 0.0, f_max_x };
		glClipPlane(GL_CLIP_PLANE1, af_equ1);

		double af_equ2[] = { 0.0, 1.0, 0.0, -f_min_y };
		glClipPlane(GL_CLIP_PLANE2, af_equ2);

		double af_equ3[] = { 0.0, -1.0, 0.0, f_max_y };
		glClipPlane(GL_CLIP_PLANE3, af_equ3);

		double af_equ4[] = { 0.0, 0.0, 1.0, -f_min_z };
		glClipPlane(GL_CLIP_PLANE4, af_equ4);

		double af_equ5[] = { 0.0, 0.0, 1.0, f_max_z };
		glClipPlane(GL_CLIP_PLANE5, af_equ5);

		for (int i = 0; i < 6; i++) {
			glEnable(GL_CLIP_PLANE0 + i);
		}
	}

	f_gl_render_scaled();

	for (int i = 0; i < 6; i++) {
		glDisable(GL_CLIP_PLANE0 + i);
	}

	/* Draw Zoom zone */
	if (_b_zoom_draw) {
		glColor3f(0.0, 0.0, 0.0);
		glEnable(GL_LINE_STIPPLE);
		glLineStipple(5, 0xAAAA);
		glLineWidth(2);
		glBegin(GL_LINE_LOOP);
		glVertex3f(_f_zoom_start_x, _f_zoom_start_y, 0);
		glVertex3f(_f_zoom_end_x, _f_zoom_start_y, 0);
		glVertex3f(_f_zoom_end_x, _f_zoom_end_y, 0);
		glVertex3f(_f_zoom_start_x, _f_zoom_end_y, 0);
		glEnd();
		glDisable(GL_LINE_STIPPLE);
	}

	/* Draw Rules */
	double f_range_x = f_max_x - f_min_x;
	uint32_t i_rule_k = 1;
	int i_rule_exp = 0;

	bool b_decrease = false;
	bool b_increase = false;

	while (1) {
		double f_step = f_range_x / (i_rule_k * exp10(i_rule_exp));
		//cout << "step: "<< f_step << " range:" << f_range_x << " k:"<< i_rule_k <<" exp:"<< i_rule_exp << endl;
		//cout << "fmax_x" << f_max_x << " fminx" << f_min_x << endl;

		if (f_step < 5) {
			/* Halt on first decrease if increase */
			if (b_increase)
				break;

			switch (i_rule_k) {
			case 1:
				i_rule_k = 5;
				i_rule_exp--;
				break;
			case 2:
				i_rule_k = 1;
				break;
			case 5:
				i_rule_k = 2;
				break;
			}

			b_decrease = true;
		} else if (f_step > 10) {
			/* Halt on first decrease if increase */
			if (b_decrease)
				break;

			switch (i_rule_k) {
			case 1:
				i_rule_k = 2;
				break;
			case 2:
				i_rule_k = 5;
				break;
			case 5:
				i_rule_k = 1;
				i_rule_exp++;
				break;
			}

			b_increase = true;
		} else {
			break;
		}
	}

	double f_rule_step = (double(i_rule_k) * exp10(i_rule_exp));
	int i_min = (int) ceil(f_min_x / f_rule_step);
	int i_max = (int) ceil(f_max_x / f_rule_step);
	cout << "Rule " << (i_rule_k * exp10(i_rule_exp)) << endl;

	cout << "imin" << i_min << endl;
	cout << i_min << " pos:" << i_min * f_rule_step << " max: " << f_max_x
			<< endl;

	if (i_min != i_max) {
		glColor3f(0.0, 0.0, 0.0);
		for (int i = i_min; i * f_rule_step < f_max_x; i++) {
			cout << "min:" << i_min << " max:" << i_max << " i: " << i
					<< " pos:" << i * f_rule_step << "min" << f_min_x << "max:"
					<< f_max_x << " rulestep:" << f_rule_step << " i_rule_k"
					<< i_rule_k << "exp:" << i_rule_exp << endl;
			glBegin(GL_LINES);
			glVertex3f(i * f_rule_step, (0.0/f_scale_y) + f_min_y, 1.1);
			glVertex3f(i * f_rule_step, (-0.015/f_scale_y) + f_min_y, 1.1);
			glEnd();
		}
	}

	/* Return to UI modelview */
	glPopMatrix();

	/* Draw mask */
	glColor3f(1.0, 1.0, 1.0);

	glBegin(GL_QUADS);
	glVertex3f(-0.1, 0.0, 1.0);
	glVertex3f(-0.1, -0.1, 1.0);
	glVertex3f(1.1, -0.1, 1.0);
	glVertex3f(1.1, 0.0, 1.0);
	glEnd();

	glBegin(GL_QUADS);
	glVertex3f(-0.1, 1.0, 1.0);
	glVertex3f(-0.1, 1.1, 1.0);
	glVertex3f(1.1, 1.1, 1.0);
	glVertex3f(1.1, 1.0, 1.0);
	glEnd();

	glBegin(GL_QUADS);
	glVertex3f(-0.1, 1.1, 1.0);
	glVertex3f(0.0, 1.1, 1.0);
	glVertex3f(0.0, -0.1, 1.0);
	glVertex3f(-0.1, -0.1, 1.0);
	glEnd();

	glBegin(GL_QUADS);
	glVertex3f(1.1, 1.1, 1.0);
	glVertex3f(1.0, 1.1, 1.0);
	glVertex3f(1.0, -0.1, 1.0);
	glVertex3f(1.1, -0.1, 1.0);
	glEnd();

	/* Draw Border */
	glColor3f(0.0, 0.0, 0.0);
	glLineWidth(2);

	glBegin(GL_LINE_LOOP);
	glVertex3f(0.0, 0.0, 1.1);
	glVertex3f(0.0, 1.0, 1.1);
	glVertex3f(1.0, 1.0, 1.1);
	glVertex3f(1.0, 0.0, 1.1);
	glEnd();

	glPopMatrix();
}

void core2d::f_gl_render_scaled(void) {
	assert(0);
}

void core2d::f_gl_update(double & in_f_min_x, double & in_f_max_x,
		double & in_f_min_y, double & in_f_max_y, double & in_f_min_z,
		double & in_f_max_z) {
	assert(0);
}

void core2d::f_gl_init(void) {
	/* Call base class init method */
	core::f_gl_init();
}

void core2d::f_gl_shader_set_glsl_path(string const & in_str_path) {
	_str_glsl_path = in_str_path;
}

void core2d::f_gl_shader_compile(string const & in_str_filename,
		GLenum in_e_shader, GLuint in_i_shader_program) {
	/* Create shader */
	GLuint i_shader = glCreateShader(in_e_shader);

	/* Read file content */
	const char * pc_tmp;
	try {
		pc_tmp = strdup(
				f_misc_file_content(_str_glsl_path + in_str_filename).c_str());
	} catch (...) {
		cerr << "Unable to open " << _str_glsl_path + in_str_filename << endl;
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

		cerr << "Could not compile shader: " << _str_glsl_path
				+ in_str_filename << endl;
		cerr << ac_log << endl;
		glDeleteShader(i_shader);
		exit(-1);
	}

	/* Attach shader to program */
	glAttachShader(in_i_shader_program, i_shader);

	free((void*) pc_tmp);
}

void core2d::f_gl_shader_link(GLuint in_i_shader_program) {
	/* Link shaders */
	glLinkProgram(in_i_shader_program);

	/* Get link status */
	GLint ec;
	glGetProgramiv(in_i_shader_program, GL_LINK_STATUS, &ec);
	if (!ec) {
		GLchar ac_log[1024];
		glGetProgramInfoLog(in_i_shader_program, sizeof(ac_log), 0, ac_log);

		cerr << "Could not link roam program: " << endl;
		cerr << ac_log << endl;
		glDeleteProgram(in_i_shader_program);
		exit(-1);
	}
}
