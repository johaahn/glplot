/*
 * core.cc
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
#include <glplot/debug.hh>
#include <glplot/core.hh>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glx.h>

using namespace std;
using namespace glplot;

core::core() {
	_pf_cb = NULL;
	_pv_cb_arg = NULL;
}

void core::f_gl_init(void) {

	int s;

	glShadeModel(GL_SMOOTH);
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	/* Enable anti-aliasing */
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);



#if 1

#else

	glEnable(GL_POLYGON_SMOOTH);
	glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
#endif
	glEnable(GL_LINE_SMOOTH);
	cout << __FUNCTION__ << "@"<<  __LINE__ << endl;
	/* fill EXT */
	_f_glTexBufferEXT = (PFNGLTEXBUFFEREXTPROC) glXGetProcAddress((GLubyte*) "glTexBufferEXT");
	assert(_f_glTexBufferEXT);
}

void core::f_gl_resize(uint32_t in_i_w, uint32_t in_i_h) {

	_i_width = in_i_w;
	_i_heigth = in_i_h;

	glViewport(0, 0, _i_width, _i_heigth);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, 1.0, 0.0, 1.0);
	//gluPerspective(45.0f, (GLfloat) width / (GLfloat) height, 0.1f, 100.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void core::f_gl_render(void) {
	assert(0);
}

void core::f_zoom_pos_start(uint32_t in_i_x, uint32_t in_i_y) {
	cout << __FUNCTION__ << " " << in_i_x << " " << in_i_y << endl;
}
void core::f_zoom_pos_update(uint32_t in_i_x, uint32_t in_i_y) {
	cout << __FUNCTION__ << " " << in_i_x << " " << in_i_y << endl;
}
void core::f_zoom_pos_end(uint32_t in_i_x, uint32_t in_i_y) {
	cout << __FUNCTION__ << " " << in_i_x << " " << in_i_y << endl;
}
void core::f_zoom_reset() {
	cout << __FUNCTION__ <<endl;
}

void core::f_redraw_set_cb(FT_CORE_REDRAW_CB in_pf_cb, void* in_pv_arg) {
	_pf_cb = in_pf_cb;
	_pv_cb_arg = in_pv_arg;
}

void core::f_redraw(void) {
	if(_pf_cb) {
		_pf_cb(_pv_cb_arg);
	}
}
