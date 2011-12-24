/*
 * main.cpp
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

#include <glplot/imagesc.hh>
#include <glplot/figure.hh>

int main(int argc, char *argv[]) {

	glplot::imagesc c_image;
	glplot::figure figure(c_image);

#if 1
	while(1) {
		double af_tmp[512][16];
		for(int i = 0; i<512; i++) {
			for(int j = 0; j<16; j++) {
			af_tmp[i][j] = 512.0*sin(M_PI*2.0*i/512)/*+100.0*rand()/RAND_MAX*/; //sin(i);
			}
		}
		//_c_plot.f_plot(af_tmp, 512);
		c_image.f_plot((double*)af_tmp, 512, 16);
		break;
	}
#endif

	return figure.f_wait();
}


