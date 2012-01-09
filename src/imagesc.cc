/*
 * imagesc.cc
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

#include <glplot/imagesc.hh>
#include <glplot/misc.hh>
#include <string>
#include <limits>

#define C_COLORMAP_SIZE 64

GLfloat
		af_rgb_jet[][3] = { { 0, 0, 0.5 }, { 0, 0, 0.5634920634920635 }, { 0,
				0, 0.626984126984127 }, { 0, 0, 0.6904761904761905 }, { 0, 0,
				0.753968253968254 }, { 0, 0, 0.8174603174603174 }, { 0, 0,
				0.8809523809523809 }, { 0, 0, 0.9444444444444444 }, { 0,
				0.007936507936507908, 1 }, { 0, 0.0714285714285714, 1 }, { 0,
				0.1349206349206349, 1 }, { 0, 0.1984126984126984, 1 }, { 0,
				0.2619047619047619, 1 }, { 0, 0.3253968253968254, 1 }, { 0,
				0.3888888888888888, 1 }, { 0, 0.4523809523809523, 1 }, { 0,
				0.5158730158730158, 1 }, { 0, 0.5793650793650793, 1 }, { 0,
				0.6428571428571428, 1 }, { 0, 0.7063492063492063, 1 }, { 0,
				0.7698412698412698, 1 }, { 0, 0.8333333333333333, 1 }, { 0,
				0.8968253968253967, 1 }, { 0, 0.9603174603174602, 1 }, {
				0.02380952380952372, 1, 0.9761904761904763 }, {
				0.08730158730158721, 1, 0.9126984126984128 }, {
				0.1507936507936507, 1, 0.8492063492063493 }, {
				0.2142857142857142, 1, 0.7857142857142858 }, {
				0.2777777777777777, 1, 0.7222222222222223 }, {
				0.3412698412698412, 1, 0.6587301587301588 }, {
				0.4047619047619047, 1, 0.5952380952380953 }, {
				0.4682539682539681, 1, 0.5317460317460319 }, {
				0.5317460317460316, 1, 0.4682539682539684 }, {
				0.5952380952380949, 1, 0.4047619047619051 }, {
				0.6587301587301586, 1, 0.3412698412698414 }, {
				0.7222222222222223, 1, 0.2777777777777777 }, {
				0.7857142857142856, 1, 0.2142857142857144 }, {
				0.8492063492063489, 1, 0.1507936507936511 }, {
				0.9126984126984126, 1, 0.08730158730158744 }, {
				0.9761904761904763, 1, 0.02380952380952372 }, { 1,
				0.9603174603174605, 0 }, { 1, 0.8968253968253972, 0 }, { 1,
				0.8333333333333335, 0 }, { 1, 0.7698412698412698, 0 }, { 1,
				0.7063492063492065, 0 }, { 1, 0.6428571428571432, 0 }, { 1,
				0.5793650793650795, 0 }, { 1, 0.5158730158730158, 0 }, { 1,
				0.4523809523809526, 0 }, { 1, 0.3888888888888893, 0 }, { 1,
				0.3253968253968256, 0 }, { 1, 0.2619047619047619, 0 }, { 1,
				0.1984126984126986, 0 }, { 1, 0.1349206349206353, 0 }, { 1,
				0.07142857142857162, 0 }, { 1, 0.007936507936507908, 0 }, {
				0.9444444444444446, 0, 0 }, { 0.8809523809523814, 0, 0 }, {
				0.8174603174603177, 0, 0 }, { 0.753968253968254, 0, 0 }, {
				0.6904761904761907, 0, 0 }, { 0.6269841269841274, 0, 0 }, {
				0.5634920634920637, 0, 0 }, { 0.5, 0, 0 } };

using namespace std;
using namespace glplot;
/* Constructor */
imagesc::imagesc() :
	core2d() {
	_b_first = false;
	_i_data_update = 0;
	_i_tbo_update = 0;
	_sz_data = 0;
	_as_data = NULL;

	_af_colormap = (GLfloat*) af_rgb_jet;
	_sz_colormap = C_COLORMAP_SIZE;

	_sz_x = 0;
	_sz_y = 0;
}

void imagesc::f_gl_update(double & in_f_min_x, double & in_f_max_x,
		double & in_f_min_y, double & in_f_max_y, double & in_f_min_z,
		double & in_f_max_z) {
	in_f_min_x = 0.0;
	in_f_min_y = 0.0;
	if (_sz_x && _sz_y) {
		in_f_max_x = (float) (_sz_x - 1);
		in_f_max_y = (float) (_sz_y - 1);
	} else {
		in_f_max_x = 0.0;
		in_f_max_y = 0.0;
	}
	in_f_min_z = 0.0;
	in_f_max_z = 1.0;

	cout << "fmax" << in_f_max_x << "sz:" << _sz_x << endl;
}

/* Main rendering method */
void imagesc::f_gl_render_scaled(void) {

	glEnable(GL_TEXTURE);
	//glEnable(GL_TEXTURE_2D);


	// Size update
	if (_i_data_update != _i_tbo_update) {

		if (_b_first) {
			_b_first = false;
		} else {
			glBindBuffer(1, _i_tbo);
			glDeleteBuffers(1, &_i_tbo);
			glDeleteTextures(1, &_i_tex);
			glBindBuffer(1, _i_colormap_tbo);
			glDeleteBuffers(1, &_i_colormap_tbo);
			glDeleteTextures(1, &_i_colormap_tex);
		}
		unsigned int sz_tex = _sz_x * _sz_y * sizeof(GLfloat);
		//cout << "Size:" << sizeof(af_rgb_jet) << endl;

		/*** COLORMAP ***/
		glGenBuffers(1, &_i_colormap_tbo);
		glGenTextures(1, &_i_colormap_tbo);

		glBindBuffer(GL_TEXTURE_BUFFER_EXT, _i_colormap_tbo);
		glBufferData(GL_TEXTURE_BUFFER_EXT, _sz_colormap * 3 * sizeof(GLfloat),
				_af_colormap, GL_STATIC_READ);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_BUFFER_EXT, _i_colormap_tex);
		_f_glTexBufferEXT(GL_TEXTURE_BUFFER_EXT, GL_RGB32F, _i_colormap_tbo);

		glBindBuffer(GL_TEXTURE_BUFFER_EXT, 0);
		glBindTexture(GL_TEXTURE_BUFFER_EXT, 0);

		/*** DATA TBO ***/
		// create buffer object
		glGenBuffers(1, &_i_tbo);
		glGenTextures(1, &_i_tex);

		glBindBuffer(GL_TEXTURE_BUFFER_EXT, _i_tbo);
		glBufferData(GL_TEXTURE_BUFFER_EXT, sz_tex, _as_data, GL_DYNAMIC_DRAW);

		//tex

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_BUFFER_EXT, _i_tex);
		_f_glTexBufferEXT(GL_TEXTURE_BUFFER_EXT, GL_R32F, _i_tbo);

		glBindBuffer(GL_TEXTURE_BUFFER_EXT, 0);
		glBindTexture(GL_TEXTURE_BUFFER_EXT, 0);
		_i_tbo_update++;
	}

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_BUFFER_EXT, _i_tex);
#if 1
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_BUFFER_EXT, _i_colormap_tex);

#endif
#if 1
	/* Initialize shader */
	glUseProgram(_i_shader_program);
	{
		Matrix4<GLdouble> m_modelview;
		Matrix4<GLdouble> m_projection;
		Matrix4<GLfloat> m_mvp;
		glGetDoublev(GL_MODELVIEW_MATRIX, m_modelview);
		glGetDoublev(GL_PROJECTION_MATRIX, m_projection);
		m_mvp = m_projection * m_modelview;
		cout << "RANGE: " << _f_max << "<->" << _f_min << endl;
		/* Load shaders params */
		{
			glUniform1f(_i_uniform_f_min, _f_min);
			glUniform1f(_i_uniform_f_max, _f_max);
			glUniform1i(_i_uniform_sz_x, _sz_x);
			glUniform1i(_i_uniform_sz_y, _sz_y);
			glUniform1i(_i_uniform_sz_colormap, _sz_colormap);
			glUniform1i(_i_uniform_i_colormap_tbo, 1);
			glUniform1i(_i_uniform_i_tbo, 0);
			glUniformMatrix4fv(_i_uniform_m_mvp, 1, 0, m_mvp.data);
		}
	}
#endif

	// Draw quads
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(0.0, 0.0, 0.0);

	glTexCoord2f(0.0, 1.0);
	glVertex3f(0.0, _sz_y, 0.0);

	glTexCoord2f(1.0, 1.0);
	glVertex3f(_sz_x, _sz_y, 0.0);

	glTexCoord2f(1.0, 0.0);
	glVertex3f(_sz_x, 0.0, 0.0);
	glEnd();

	glUseProgram(0);
	//glBindTexture(GL_TEXTURE_BUFFER_EXT, 0);
}

void imagesc::f_gl_init(void) {
	core2d::f_gl_init();

	/* Create shader program */
	_i_shader_program = glCreateProgram();

	/* Compile shader */
#if 1
	f_gl_shader_compile("imagesc.vert.glsl", GL_VERTEX_SHADER,
			_i_shader_program);
#endif
#if 1
	f_gl_shader_compile("imagesc.frag.glsl", GL_FRAGMENT_SHADER,
			_i_shader_program);
#endif

	f_gl_shader_link(_i_shader_program);

	/* Uniform */
	_i_uniform_m_mvp = glGetUniformLocation(_i_shader_program, "m_mvp");
	_i_uniform_f_min = glGetUniformLocation(_i_shader_program, "f_min");
	_i_uniform_f_max = glGetUniformLocation(_i_shader_program, "f_max");
	_i_uniform_i_tbo = glGetUniformLocation(_i_shader_program, "t_image");
	_i_uniform_i_colormap_tbo = glGetUniformLocation(_i_shader_program,
			"t_colormap");
	_i_uniform_sz_x = glGetUniformLocation(_i_shader_program, "i_sz_x");
	_i_uniform_sz_y = glGetUniformLocation(_i_shader_program, "i_sz_y");
	_i_uniform_sz_colormap = glGetUniformLocation(_i_shader_program,
			"i_sz_colormap");
}

/* Main plot function */
void imagesc::f_plot(double * in_ad_gnuplot, size_t in_sz_x, size_t in_sz_y) {

	/* Allocation of vertex buffer */
	size_t sz_data = in_sz_x * in_sz_y;
	if (_sz_data != sz_data) {
		if (_as_data) {
			delete[] _as_data;
		}
		_as_data = new vertex_image[sz_data];
		_sz_data = sz_data;
		_sz_x = in_sz_x;
		_sz_y = in_sz_y;
	}

	/* Reset min max */
	_f_min = std::numeric_limits<double>::infinity();
	_f_max = -std::numeric_limits<double>::infinity();

	/* Fill vertex buffer with new data */
	for (uint32_t i = 0; i < sz_data; i++) {
		double f_value;
		/* Load value */
		f_value = in_ad_gnuplot[i];

		/* Store value */
		_as_data[i].d_value = f_value;
		//cout << "i:" << i <<", value:"<<f_value<<endl;
		/* Update mins */
		_f_min = _f_min > f_value ? f_value : _f_min;
		/* Update max */
		_f_max = _f_max < f_value ? f_value : _f_max;

	}
	//cout << "EXTREM" << _f_max << " " << _f_min <<endl;

	/* Update counter */
	_i_data_update++;

	/* request redraw */
	f_redraw();
}

/* Main plot function */
void imagesc::f_plot(float * in_af_gnuplot, size_t in_sz_x, size_t in_sz_y) {

	/* Allocation of vertex buffer */
	size_t sz_data = in_sz_x * in_sz_y;
	if (_sz_data != sz_data) {
		if (_as_data) {
			delete[] _as_data;
		}
		_as_data = new vertex_image[sz_data];
		_sz_data = sz_data;
		_sz_x = in_sz_x;
		_sz_y = in_sz_y;
	}

	/* Reset min max */
	_f_min = std::numeric_limits<double>::infinity();
	_f_max = -std::numeric_limits<double>::infinity();

	/* Fill vertex buffer with new data */
	for (uint32_t i = 0; i < sz_data; i++) {
		double f_value;
		/* Load value */
		f_value = in_af_gnuplot[i];

		/* Store value */
		_as_data[i].d_value = f_value;
		//cout << "i:" << i <<", value:"<<f_value<<endl;
		/* Update mins */
		_f_min = _f_min > f_value ? f_value : _f_min;
		/* Update max */
		_f_max = _f_max < f_value ? f_value : _f_max;

	}
	//cout << "EXTREM" << _f_max << " " << _f_min <<endl;

	/* Update counter */
	_i_data_update++;

	/* request redraw */
	f_redraw();

}
