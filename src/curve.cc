/*
 * curve.cc

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

#include <glplot/curve.hh>
#include <glplot/misc.hh>
#include <limits>

using namespace std;
using namespace glplot;

curve::curve() {
	_as_data = NULL;
	_sz_data = 0;
	_e_type = E_TYPE_STD;
	_i_data_update = 0;

	_b_vbo_created = false;
	_i_vbo = 0;
	_sz_vbo = 0;
	_i_vbo_update = 0;
}

curve::~curve() {
	if (_as_data) {
		delete[] _as_data;
	}
}

void curve::f_init(GLdouble * in_ad_x, GLdouble * in_ad_y, size_t in_sz_data,
		enum et_curve_type e_type) {

	/* Allocation of vertex buffer */
	if (_sz_data != in_sz_data) {
		if (_as_data) {
			delete[] _as_data;
		}
		_as_data = new st_vertex_curve[in_sz_data];
		_sz_data = in_sz_data;
	}

	/* Reset min max */
	_f_min_x = std::numeric_limits<double>::infinity();
	_f_max_x = -std::numeric_limits<double>::infinity();
	_f_min_y = std::numeric_limits<double>::infinity();
	_f_max_y = -std::numeric_limits<double>::infinity();
	_f_min_z = std::numeric_limits<double>::infinity();
	_f_max_z = -std::numeric_limits<double>::infinity();

	/* Fill vertex buffer with new data */
	for (uint i = 0; i < in_sz_data; i++) {
		double f_x, f_y, f_z;

		/* Get values */
		if (in_ad_x) {
			f_x = in_ad_x[i];
		} else {
			f_x = i / 512.0;
		}
		f_y = in_ad_y[i] + 0.5;
		f_z = 0;

		//cout << f_x << " " << f_y << " " << f_z << endl;

		/* Store value */
		_as_data[i].d_x = f_x;
		_as_data[i].d_y = f_y;
		_as_data[i].d_z = f_z;

		/* Update mins */
		_f_min_x = _f_min_x > f_x ? f_x : _f_min_x;
		_f_min_y = _f_min_y > f_y ? f_y : _f_min_y;
		_f_min_z = _f_min_z > f_z ? f_z : _f_min_z;

		/* Update max */
		_f_max_x = _f_max_x < f_x ? f_x : _f_max_x;
		_f_max_y = _f_max_y < f_y ? f_y : _f_max_y;
		_f_max_z = _f_max_z < f_z ? f_z : _f_max_z;
	}

	/* Update counter */
	_i_data_update++;
}

void curve::f_gl_update(void) {
	if (!_b_vbo_created) {
		glGenBuffers(1, &_i_vbo);
		_b_vbo_created = true;
	}
	glBindBuffer(GL_ARRAY_BUFFER, _i_vbo);

	if (_i_data_update != _i_vbo_update) {
		cout << "VBO Update" << endl;
		glBufferData(GL_ARRAY_BUFFER, sizeof(st_vertex_curve) * _sz_data,
				&_as_data[0].d_x, GL_STATIC_DRAW);
		_sz_vbo = _sz_data;
		_i_vbo_update++;

		/* Update vbo data */
		_f_vbo_min_x = _f_min_x;
		_f_vbo_min_y = _f_min_y;
		_f_vbo_min_z = _f_min_z;
		_f_vbo_max_x = _f_max_x;
		_f_vbo_max_y = _f_max_y;
		_f_vbo_max_z = _f_max_z;
		cout << "VBO Updated" << endl;
	}
}

void curve::f_gl_render(void) {
	glPushMatrix();

	glColor3f(1.0, 0.5, 0.5);

	if (_sz_vbo) {
		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(3, GL_DOUBLE, 0, 0);
		glDrawArrays(GL_LINE_STRIP, 0, _sz_vbo);
		glDisableClientState(GL_VERTEX_ARRAY);
		cout << "Draw VBO " << _sz_vbo << endl;
	}

	glPopMatrix();

}

void curve::f_gl_update_minmax(double & in_f_min_x, double & in_f_max_x,
		double & in_f_min_y, double & in_f_max_y, double & in_f_min_z,
		double & in_f_max_z) {
	/* Update mins */
	in_f_min_x = in_f_min_x > _f_vbo_min_x ? _f_vbo_min_x : in_f_min_x;
	in_f_min_y = in_f_min_y > _f_vbo_min_y ? _f_vbo_min_y : in_f_min_y;
	in_f_min_z = in_f_min_z > _f_vbo_min_z ? _f_vbo_min_z : in_f_min_z;

	/* Update max */
	in_f_max_x = in_f_max_x < _f_vbo_max_x ? _f_vbo_max_x : in_f_max_x;
	in_f_max_y = in_f_max_y < _f_vbo_max_y ? _f_vbo_max_y : in_f_max_y;
	in_f_max_z = in_f_max_z < _f_vbo_max_z ? _f_vbo_max_z : in_f_max_z;
}

