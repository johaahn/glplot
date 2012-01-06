/*
 * font.hh
 *
 *  Created on: 4 sept. 2011
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

#ifndef FONT_HH_
#define FONT_HH_

#include <glplot/vmath.hh>

//OpenGL Headers
#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glext.h>

//Some STL headers
#include <map>
#include <vector>
#include <string>


//Using the STL exception library increases the
//chances that someone else using our code will corretly
//catch any exceptions that we throw.
#include <stdexcept>

struct FT_FaceRec_;
typedef struct FT_FaceRec_* FT_Face;
struct FT_LibraryRec_;
typedef struct FT_LibraryRec_  *FT_Library;

///Wrap everything in a namespace, that we can use common
///function names like "print" without worrying about
///overlapping with anyone else's code.
namespace font {

//Inside of this namespace, give ourselves the ability
//to write just "vector" instead of "std::vector"
using std::vector;

typedef class Vector2<int> Vector2i;

enum ET_SEGMENT_TYPE {
	E_SEGMENT_TYPE_LINE, E_SEGMENT_TYPE_CUBIC, E_SEGMENT_TYPE_CONIC
};

enum ET_CLOCKWISE_TYPE {
	E_CLOCKWISE_TYPE_CLOCKWISE = 1,
	E_CLOCKWISE_TYPE_COUNTERCLOCKWISE,
	E_CLOCKWISE_TYPE_UNDEF
};

enum ET_FILL_TYPE {
	E_FILL_TYPE_RIGHT, E_FILL_TYPE_LEFT
};

class font_vertex {
public:
	Vector3d _c_pos;
	Vector4f _c_color;
};

class font_segment {
public:
	/* Start stop */
	Vector2i _c_start;
	Vector2i _c_stop;

	/* Bezier point */
	Vector2i _c_bezier_1;
	Vector2i _c_bezier_2;

	/* Conic cross product */
	int _i_cross;

	/* Segment type */
	enum ET_SEGMENT_TYPE _e_type;
};

class font_contour {
	std::vector<font_segment> _v_segment;
	std::vector<Vector2i> _v_points;

	/* Current contour */
	int _i_segment_index;

	Vector2i _c_current_point;

	/* clock wise state */
	enum ET_CLOCKWISE_TYPE _e_clock_wise;

	/* font vertex pointer */
	std::vector<font_vertex*> _v_vertex;

	std::vector<font_segment> _v_conic;

	uint32_t _i_cnt_line;
	uint32_t _i_cnt_cubic;
	uint32_t _i_cnt_conic;

public:
	font_contour();

	void f_start_to(Vector2i & in_c_to);
	void f_line_to(Vector2i & in_c_to);
	void f_conic_to(Vector2i & in_c_to, Vector2i &in_c_bezier_1);
	void f_cubic_to(Vector2i & in_c_to, Vector2i &in_c_bezier_1,
			Vector2i &in_c_bezier_2);

	/* Tesselation */
	void f_tessel_point(GLUtesselator * in_ps_tess, Vector2i & in_c_point);
	void f_tessel(GLUtesselator * in_ps_tess, enum ET_FILL_TYPE in_e_fill_mode);
	void f_render_conic(enum ET_FILL_TYPE in_e_fill_mode);
	enum ET_CLOCKWISE_TYPE f_is_clockwise(void);
	font_vertex * f_add_extra_point(GLdouble * in_af_coords);
	uint32_t f_nb_segment(void) {return  _i_cnt_line + _i_cnt_cubic + _i_cnt_conic; }
};

class font_char {
	/* Char code of font */
	wchar_t _i_char_code;

	/* Contour list */
	std::vector<font_contour> _m_contour;

	/* Current contour */
	int _i_contour_index;
public:
	/* Control box */
	Vector2i _c_control_min, _c_control_max;

	/* Boundary box */
	Vector2i _c_boundary_min, _c_boundary_max;

	/* Fill mode */
	enum ET_FILL_TYPE _e_fill_mode;


	font_char();

	/* Freetype event */
	void f_move_to(Vector2i & in_c_to);
	void f_line_to(Vector2i & in_c_to);
	void f_conic_to(Vector2i & in_c_to, Vector2i &in_c_bezier_1);
	void f_cubic_to(Vector2i & in_c_to, Vector2i &in_c_bezier_1,
			Vector2i &in_c_bezier_2);

	/* Tesselator events */
	void f_tessel(void);
	void f_render_conic(void);

	/* Init function */
	void f_init_as_outline(wchar_t in_i_char, FT_Face & in_ps_face, GLuint in_i_gllist);
	void f_init_as_bitmap(wchar_t in_i_char, FT_Face & in_ps_face, GLuint in_i_gl_tex, GLuint in_i_gl_list);

	/* Get */
	uint32_t f_get_width() {return (_c_control_max.x>0.0 ? _c_control_max.x: 0) - (_c_control_min.x<0.0 ? _c_control_min.x: 0);}
	uint32_t f_get_height() {return (_c_control_max.y>0.0 ? _c_control_max.y: 0) - (_c_control_min.y<0.0 ? _c_control_min.y: 0);}
};



class font {
	size_t _sz_bitmap;

	GLuint _i_gl_list;
	GLuint _i_gl_bitmap_list;
	GLuint * _ai_gl_bitmap_tex;

	/* Shader stuffs */
	GLuint _i_shader_program;
	GLuint _i_uniform_m_mvp;

	/* Map font_char */
	std::map<wchar_t, font_char> _m_chars;

	/* Scale parameters */
	uint32_t _i_width_max, _i_height_max;

	uint32_t _i_width_avg;

public:
	font();

	void f_init(const char * fname, size_t in_i_size_bitmap = 0);
	void f_render(wchar_t * in_pc_char);
	void f_render_as_outline(wchar_t * in_pc_char);

	/* Bitmaps */

	void f_bitmap_set_screen_projection();
	void f_bitmap_unset_screen_projection();
	void f_render_as_bitmap(wchar_t * in_pc_char);

	/* shader */
	void f_gl_shader_compile(std::string const & in_str_filename,
			GLenum in_e_shader, GLuint in_i_shader_program);
	void f_gl_shader_init(void);
};

//This holds all of the information related to any
//freetype font that we want to create.
struct font_data {
	float h; ///< Holds the height of the font.
	GLuint * textures; ///< Holds the texture id's
	GLuint list_base; ///< Holds the first display list id

	//The init function will create a font of
	//of the height h from the file fname.
	void init(const char * fname, unsigned int h);

	//Free all the resources assosiated with the font.
	void clean();
};

//The flagship function of the library - this thing will print
//out text at window coordinates x,y, using the font ft_font.
//The current modelview matrix will also be applied to the text.
void print(const font_data &ft_font, float x, float y, const char *fmt, ...);
void print2(const font_data &ft_font, const char *fmt, ...);

}

#endif /* FONT_HH_ */
