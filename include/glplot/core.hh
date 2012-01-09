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
#include <stdint.h>

#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glext.h>

#ifndef CORE_HH_
#define CORE_HH_

#define M_DBGLINE cout << __FUNCTION__ << "@"<<  __LINE__ << endl;

typedef void (*FT_CORE_REDRAW_CB) (void*);

namespace glplot {

class core {

	float _f_aspect;
	uint32_t _i_width;
	uint32_t _i_heigth;

	/* Redraw stuff */
	FT_CORE_REDRAW_CB _pf_cb;
	void * _pv_cb_arg;

public:
	/* EXT */
	PFNGLTEXBUFFEREXTPROC _f_glTexBufferEXT;

public:
	core();

	// redraw stuff
	void f_redraw(void);
	void f_redraw_set_cb(FT_CORE_REDRAW_CB in_pf_cb, void* in_pv_arg);

	virtual void f_gl_init(void);
	virtual void f_gl_resize(uint32_t in_i_w, uint32_t in_i_h);
	virtual void f_gl_render(void);

	//void f_zoom(uint32_t in_i_x1, uint32_t in_i_y1, uint32_t in_i_x2, uint32_t in_i_y2)
	virtual void f_zoom_pos_start(uint32_t in_i_x, uint32_t in_i_y);
	virtual void f_zoom_pos_update(uint32_t in_i_x, uint32_t in_i_y);
	virtual void f_zoom_pos_end(uint32_t in_i_x, uint32_t in_i_y);
	virtual void f_zoom_reset();

	/* Quick Get */
	uint32_t f_get_width(void) { return _i_width; }
	uint32_t f_get_height(void) { return _i_heigth; }
};

}
#endif /* CORE_HH_ */
