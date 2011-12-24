/*
 * core.hh
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

#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glext.h>

#ifndef CORE_HH_
#define CORE_HH_

#define M_DBGLINE cout << __FUNCTION__ << "@"<<  __LINE__ << endl;

namespace glplot {

class core {

	float _f_aspect;
	uint _i_width;
	uint _i_heigth;

public:
	/* EXT */
	PFNGLTEXBUFFEREXTPROC _f_glTexBufferEXT;

public:
	core();

	virtual void f_gl_init(void);
	virtual void f_gl_resize(uint in_i_w, uint in_i_h);
	virtual void f_gl_render(void);

	//void f_zoom(uint in_i_x1, uint in_i_y1, uint in_i_x2, uint in_i_y2)
	virtual void f_zoom_pos_start(uint in_i_x, uint in_i_y);
	virtual void f_zoom_pos_update(uint in_i_x, uint in_i_y);
	virtual void f_zoom_pos_end(uint in_i_x, uint in_i_y);
	virtual void f_zoom_reset();

	/* Quick Get */
	uint f_get_width(void) { return _i_width; }
	uint f_get_height(void) { return _i_heigth; }
};

}
#endif /* CORE_HH_ */
