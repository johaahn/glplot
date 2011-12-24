/*
 * figure.hh
 *
 *  Created on: 22 déc. 2011
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

#ifndef FIGURE_HH_
#define FIGURE_HH_

#include <glplot/core.hh>

namespace glplot {
class figure {
	pthread_t _s_thread;
	core * _pc_core;

public:
	figure(core & in_c_core);
	void * f_main_loop(void);
	int f_wait(void);
};
}

#endif /* FIGURE_HH_ */
