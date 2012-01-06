/*
 * font.cc
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

/*
 A quick and simple opengl font library that uses GNU freetype2, written
 and distributed as part of a tutorial for nehe.gamedev.net.
 Sven Olsen, 2003
 */

#include <glplot/font.hh>
#include <glplot/debug.hh>
using namespace std;

/* FreeType Headers */
#include <ft2build.h>
#include <freetype/freetype.h>
#include <freetype/ftglyph.h>
#include <freetype/ftoutln.h>
#include <freetype/fttrigon.h>

// GLUT
#include <fstream>
#include <streambuf>
//#define LF_OUTLINE_DBG

GLuint _i_uniform_f_sign;

namespace font {

/* Outline call back */
static int f_outline_move_to(const FT_Vector *to, void *user);
static int f_outline_line_to(const FT_Vector *to, void *user);
static int f_outline_conic_to(const FT_Vector *control, const FT_Vector *to,
		void *user);
static int f_outline_cubic_to(const FT_Vector* control1,
		const FT_Vector *control2, const FT_Vector* to, void *user);

font::font() {

}

/* Tesselator callbacks */
void f_tessel_vertex_cb(font_vertex * in_pc_vertex) {
	//cout << __FUNCTION__ << "pos: " << in_pc_vertex->_c_pos << endl;

	glVertex3dv(in_pc_vertex->_c_pos);
}

void f_tessel_begin_cb(GLenum in_e_begin) {
	//cout << __FUNCTION__ << endl;
	glBegin(in_e_begin);
}

void f_tessel_end_cb(void) {
	//cout << __FUNCTION__ << endl;
	glEnd();
}

void f_tessel_combine_cb(GLdouble in_af_coords[3], void* in_pv_data[4],
		GLfloat in_af_weight[4], void** out_ppv_vertex,
		font_contour* in_pc_contour) {
	/* Add extra point */
	*out_ppv_vertex = in_pc_contour->f_add_extra_point(in_af_coords);
}

void f_tessel_error_cb(GLenum in_e_error_code) {
	cerr << "Error during glut tessellation:"
			<< gluErrorString(in_e_error_code) << endl;
}

/* Freetype callback */

int f_outline_move_to(const FT_Vector *to, void *user) {
	Vector2i c_to(to->x, to->y);
	reinterpret_cast<font_char*> (user)->f_move_to(c_to);
	return 0;
}
int f_outline_line_to(const FT_Vector *to, void *user) {
	Vector2i c_to(to->x, to->y);
	reinterpret_cast<font_char*> (user)->f_line_to(c_to);
	return 0;
}
int f_outline_conic_to(const FT_Vector *control, const FT_Vector *to,
		void *user) {
	Vector2i c_to(to->x, to->y);
	Vector2i c_c1(control->x, control->y);
	reinterpret_cast<font_char*> (user)->f_conic_to(c_to, c_c1);
	return 0;
}
int f_outline_cubic_to(const FT_Vector* control1, const FT_Vector *control2,
		const FT_Vector* to, void *user) {

	Vector2i c_to(to->x, to->y);
	Vector2i c_c1(control1->x, control1->y);
	Vector2i c_c2(control2->x, control2->y);

	reinterpret_cast<font_char*> (user)->f_cubic_to(c_to, c_c1, c_c2);
	return 0;
}

FT_Outline_Funcs s_outline_cb = { f_outline_move_to, f_outline_line_to,
		f_outline_conic_to, f_outline_cubic_to, 0, 0 };

font_contour::font_contour() {
	_i_segment_index = 0;
	_i_cnt_line = 0;
	_i_cnt_conic = 0;
	_i_cnt_cubic = 0;
}
void font_contour::f_start_to(Vector2i & in_c_to) {
	_c_current_point = in_c_to;
	_v_points.push_back(in_c_to);
}
void font_contour::f_line_to(Vector2i & in_c_to) {
	font_segment c_segment;
	c_segment._c_start = _c_current_point;
	c_segment._c_stop = in_c_to;
	c_segment._e_type = E_SEGMENT_TYPE_LINE;

#ifdef LF_OUTLINE_DBG
	cout << "Adding new line " << c_segment._c_start << " to "
	<< c_segment._c_stop << endl;
#endif

	_v_segment.push_back(c_segment);
	_v_points.push_back(in_c_to);
	_c_current_point = in_c_to;
	_i_cnt_line++;
}
void font_contour::f_conic_to(Vector2i & in_c_to, Vector2i &in_c_bezier_1) {
	font_segment c_segment;
	c_segment._c_start = _c_current_point;
	c_segment._c_stop = in_c_to;
	c_segment._c_bezier_1 = in_c_bezier_1;
	c_segment._e_type = E_SEGMENT_TYPE_CONIC;

	Vector2i c_vect_on = c_segment._c_stop - c_segment._c_start;
	Vector2i c_vect_control = c_segment._c_bezier_1 - c_segment._c_start;
	c_segment._i_cross = c_vect_on.x * c_vect_control.y - c_vect_on.y
			* c_vect_control.x;

#ifdef LF_OUTLINE_DBG
	cout << "Adding new conic " << c_segment._c_start << " to "
	<< c_segment._c_stop << " with " << c_segment._c_bezier_1 << endl;
#endif
	_v_segment.push_back(c_segment);
	_v_points.push_back(in_c_to);
	_c_current_point = in_c_to;
	_i_cnt_conic++;
}
void font_contour::f_cubic_to(Vector2i & in_c_to, Vector2i &in_c_bezier_1,
		Vector2i &in_c_bezier_2) {
	font_segment c_segment;
	c_segment._c_start = _c_current_point;
	c_segment._c_stop = in_c_to;
	c_segment._c_bezier_1 = in_c_bezier_1;
	c_segment._c_bezier_2 = in_c_bezier_2;
	c_segment._e_type = E_SEGMENT_TYPE_CUBIC;

#ifdef LF_OUTLINE_DBG
	cout << "Adding new cubic " << c_segment._c_start << " to "
	<< c_segment._c_stop << " with " << c_segment._c_bezier_1
	<< c_segment._c_bezier_2 << endl;
#endif

	_v_segment.push_back(c_segment);
	_v_points.push_back(in_c_to);
	_c_current_point = in_c_to;
	_i_cnt_cubic++;
}

/* http://debian.fmi.uni-sofia.bg/~sergei/cgsr/docs/clockwise.htm */
enum ET_CLOCKWISE_TYPE font_contour::f_is_clockwise(void) {
	int i, j, k;
	int count = 0;
	double z;

	if (_v_points.size() < 3)
		return (E_CLOCKWISE_TYPE_UNDEF);

	for (i = 0; i < _v_points.size(); i++) {
		j = (i + 1) % _v_points.size();
		k = (i + 2) % _v_points.size();
		z = (_v_points[j].x - _v_points[i].x) * (_v_points[k].y
				- _v_points[j].y);
		z -= (_v_points[j].y - _v_points[i].y) * (_v_points[k].x
				- _v_points[j].x);
		if (z < 0)
			count--;
		else if (z > 0)
			count++;
	}
	if (count > 0) {
		_e_clock_wise = E_CLOCKWISE_TYPE_COUNTERCLOCKWISE;
	} else if (count < 0) {
		_e_clock_wise = E_CLOCKWISE_TYPE_CLOCKWISE;
	} else {
		_e_clock_wise = E_CLOCKWISE_TYPE_UNDEF;
	}

	return _e_clock_wise;
}
font_vertex * font_contour::f_add_extra_point(GLdouble * in_af_coords) {
	font_vertex * pc_new_vertex = new font_vertex;
	pc_new_vertex->_c_pos.x = in_af_coords[0];
	pc_new_vertex->_c_pos.y = in_af_coords[1];
	pc_new_vertex->_c_pos.z = in_af_coords[2];
	//cout << "New: " << pc_new_vertex->_c_pos << endl;
	//_v_extra.push_back(c_new_vertex);
	return pc_new_vertex;
}

void font_contour::f_tessel_point(GLUtesselator * in_ps_tess,
		Vector2i & in_c_point) {
	font_vertex * pc_vert = new font_vertex();

	pc_vert->_c_pos.x = in_c_point.x;
	pc_vert->_c_pos.y = in_c_point.y;
	pc_vert->_c_pos.z = 0.0;

	_v_vertex.push_back(pc_vert);
	//cout << "Tessel :" << pc_vert->_c_pos << endl;
	gluTessVertex(in_ps_tess, pc_vert->_c_pos, (void*) pc_vert);
}

void font_contour::f_tessel(GLUtesselator * in_ps_tess,
		enum ET_FILL_TYPE in_e_fill_mode) {

	gluTessBeginContour(in_ps_tess);
	//cout << _e_clock_wise << endl;

	/* Add first point */
	//f_tessel_point(in_ps_tess, _v_segment[0]._c_start);

	for (uint32_t i = 0; i < _v_segment.size(); i++) {
		font_segment & c_seg = _v_segment[i];

		switch (c_seg._e_type) {
		case E_SEGMENT_TYPE_LINE:
			f_tessel_point(in_ps_tess, c_seg._c_stop);
			break;
		case E_SEGMENT_TYPE_CONIC: {
			if (c_seg._i_cross > 0) {
				f_tessel_point(in_ps_tess, c_seg._c_stop);
			} else if (c_seg._i_cross < 0) {
				f_tessel_point(in_ps_tess, c_seg._c_bezier_1);
				f_tessel_point(in_ps_tess, c_seg._c_stop);
			} else {
				f_tessel_point(in_ps_tess, c_seg._c_stop);
				//cout << c_seg._c_stop << c_seg._c_start << c_seg._c_bezier_1 << endl;
				//throw std::runtime_error("Cross equal to 0");
			}
			break;
		}
		case E_SEGMENT_TYPE_CUBIC:
			cerr << "CUBIC not implemented yet" << endl;
			assert(0);
			break;
		}
	}

	gluTessEndContour(in_ps_tess);
}

void font_contour::f_render_conic(enum ET_FILL_TYPE in_e_fill_mode) {
	if (_i_cnt_conic) {
		glBegin(GL_TRIANGLES);
		for (uint32_t i = 0; i < _v_segment.size(); i++) {
			font_segment & c_seg = _v_segment[i];
			float f_sign = 1.0;
#if 0
			float f_sign = _e_clock_wise == E_CLOCKWISE_TYPE_CLOCKWISE ? 1.0
			: -1.0;
#endif

			if (c_seg._i_cross < 0) {
				f_sign *= -1.0;
			}

			if (c_seg._e_type == E_SEGMENT_TYPE_CONIC) {
				glVertex3d(c_seg._c_start.x, c_seg._c_start.y, f_sign);
				glVertex3d(c_seg._c_stop.x, c_seg._c_stop.y, f_sign);
				glVertex3d(c_seg._c_bezier_1.x, c_seg._c_bezier_1.y, f_sign);
			}
		}
		glEnd();
	}
}

font_char::font_char() {
	_i_contour_index = -1;
}

void font_char::f_move_to(Vector2i & in_c_to) {
	//cout << "Creating new contour" << endl;
	font_contour c_new_contour;
	c_new_contour.f_start_to(in_c_to);
	_m_contour.push_back(c_new_contour);
}
void font_char::f_line_to(Vector2i & in_c_to) {
	_m_contour.back().f_line_to(in_c_to);
}
void font_char::f_conic_to(Vector2i & in_c_to, Vector2i &in_c_bezier_1) {
	_m_contour.back().f_conic_to(in_c_to, in_c_bezier_1);
}
void font_char::f_cubic_to(Vector2i & in_c_to, Vector2i &in_c_bezier_1,
		Vector2i &in_c_bezier_2) {
	_m_contour.back().f_cubic_to(in_c_to, in_c_bezier_1, in_c_bezier_2);
}
void font_char::f_tessel(void) {

	GLUtesselator * ps_tess = gluNewTess();

	gluTessCallback(ps_tess, GLU_TESS_VERTEX, (_GLUfuncptr) f_tessel_vertex_cb);
	gluTessCallback(ps_tess, GLU_TESS_BEGIN, (_GLUfuncptr) f_tessel_begin_cb);
	gluTessCallback(ps_tess, GLU_TESS_END, (_GLUfuncptr) f_tessel_end_cb);
	gluTessCallback(ps_tess, GLU_TESS_COMBINE_DATA,
			(_GLUfuncptr) f_tessel_combine_cb);
	gluTessCallback(ps_tess, GLU_TESS_ERROR, (_GLUfuncptr) f_tessel_error_cb);

	bool b_poly_open = false;
	for (vector<font_contour>::iterator pc_it = _m_contour.begin(); pc_it
			< _m_contour.end(); pc_it++) {

		if ((pc_it->f_is_clockwise() == E_CLOCKWISE_TYPE_CLOCKWISE)
				&& (_e_fill_mode == E_FILL_TYPE_RIGHT)) {
			if (b_poly_open) {

				gluTessEndPolygon(ps_tess);
				b_poly_open = false;
			}
		}

		if (!b_poly_open) {
			gluTessBeginPolygon(ps_tess, (void*) &(*pc_it));
			b_poly_open = true;
		}

		pc_it->f_tessel(ps_tess, _e_fill_mode);
	}
	//gluTessVertex(ps_tess, )
	if (b_poly_open) {
		gluTessEndPolygon(ps_tess);
	}

	gluDeleteTess(ps_tess);
}

void font_char::f_render_conic(void) {
	for (vector<font_contour>::iterator pc_it = _m_contour.begin(); pc_it
			< _m_contour.end(); pc_it++) {
		pc_it->f_render_conic(_e_fill_mode);
	}
}

/* This function gets the first power of 2 >= the int that we pass it. */
inline int f_next_p2(int a) {
	int rval = 1;
	while (rval < a)
		rval <<= 1;
	return rval;
}

void font_char::f_init_as_outline(wchar_t in_i_char, FT_Face & in_c_face,
		GLuint in_i_gl_list) {
	_i_char_code = in_i_char;


	FT_GlyphSlot glyph = in_c_face->glyph;
	FT_Outline_Decompose(&glyph->outline, &s_outline_cb, (void *) this);

	FT_Orientation e_orientation;
	e_orientation = FT_Outline_Get_Orientation(&glyph->outline);
	switch (e_orientation) {
	case FT_ORIENTATION_FILL_RIGHT:
		_e_fill_mode = E_FILL_TYPE_RIGHT;
		break;
	case FT_ORIENTATION_FILL_LEFT:
		_e_fill_mode = E_FILL_TYPE_LEFT;
		break;
	default:
		throw std::runtime_error("Unknown orientation");
		break;
	}

	//cout << "Orientation = " << e_orientation << endl;

	/* GEt boarder */
	FT_BBox s_cbox;
	FT_Outline_Get_CBox(&glyph->outline, &s_cbox);
	_c_control_min.x = s_cbox.xMin;
	_c_control_min.y = s_cbox.yMin;
	_c_control_max.x = s_cbox.xMax;
	_c_control_max.y = s_cbox.yMax;

	if (in_i_char == L'!') {
		cout << _c_control_min << _c_control_max << endl;
	}

	/* Tesselate control points... */
	glNewList(in_i_gl_list, GL_COMPILE);
	glColor3f(0.0, 0.0, 0.0);
	f_tessel();
	glEndList();

	glNewList(in_i_gl_list + 1, GL_COMPILE);
	glColor3f(0.5, 0.5, 0.5);
	f_render_conic();
	glEndList();
}

void font_char::f_init_as_bitmap(wchar_t in_i_char, FT_Face &in_c_face, GLuint in_i_gl_tex, GLuint in_i_gl_list) {
	_i_char_code = in_i_char;

	//Move the face's glyph into a Glyph object.
	FT_Glyph glyph;
	if (FT_Get_Glyph(in_c_face->glyph, &glyph))
		throw std::runtime_error("FT_Get_Glyph failed");

	// convert to a bitmap (default render mode + destroying old)
	if (glyph->format != FT_GLYPH_FORMAT_BITMAP) {
		if (FT_Glyph_To_Bitmap(&glyph, FT_RENDER_MODE_NORMAL, 0, 1)) {
			throw std::runtime_error("FT_Get_Glyph failed");
		}
	}

	FT_BitmapGlyph s_bitmap_glyph = (FT_BitmapGlyph) glyph;
	FT_Bitmap& s_bitmap = s_bitmap_glyph->bitmap;

	//Use our helper function to get the widths of
	//the bitmap data that we will need in order to create
	//our texture.
	uint32_t i_bitmap_width = f_next_p2(s_bitmap.width);
	uint32_t i_bitmap_height = f_next_p2(s_bitmap.rows);

	/* Allocate memory for the texture data */
	size_t sz_bitmap = 2 * i_bitmap_width * i_bitmap_height;
	GLubyte * ac_bitmap = new GLubyte[sz_bitmap];

	/* Adapt FT buffer to expanded buffer (P2 size) */
	for (int j = 0; j < i_bitmap_height; j++) {
		for (int i = 0; i < i_bitmap_width; i++) {
			GLubyte i_tmp = (i >= s_bitmap.width || j >= s_bitmap.rows) ? 0
					: s_bitmap.buffer[i + s_bitmap.width * j];
			ac_bitmap[2 * (i + j * i_bitmap_width)] = ac_bitmap[2 * (i + j
					* i_bitmap_width) + 1] = i_tmp;
		}
	}

	//Now we just setup some texture paramaters.
	glBindTexture(GL_TEXTURE_2D, in_i_gl_tex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	//Here we actually create the texture itself, notice
	//that we are using GL_LUMINANCE_ALPHA to indicate that
	//we are using 2 channel data.
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, i_bitmap_width, i_bitmap_height, 0,
			GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE, ac_bitmap);

	//With the texture created, we don't need to expanded data anymore
	delete ac_bitmap;

	/* Compile OpenGL list */
	glNewList(in_i_gl_list, GL_COMPILE);

	/* Bind texture */
	glBindTexture(GL_TEXTURE_2D, in_i_gl_tex);

	/* first we need to move over a little so that
	 *the character has the right amount of space
	 * between it and the one before it. */
	glTranslatef(s_bitmap_glyph->left, 0, 0);

	/* Now we move down a little in the case that the
	 bitmap extends past the bottom of the line
	 (this is only true for characters like 'g' or 'y'. */
	glPushMatrix();
	glTranslatef(0, s_bitmap_glyph->top - s_bitmap.rows, 0);

	/* Now we need to account for the fact that many of
	 our textures are filled with empty padding space.
	 We figure what portion of the texture is used by
	 the actual character and store that information in
	 the x and y variables, then when we draw the
	 quad, we will only reference the parts of the texture
	 that we contain the character itself. */
	float x = (float) s_bitmap.width / (float) i_bitmap_width, y =
			(float) s_bitmap.rows / (float) i_bitmap_height;

	/* Here we draw the texturemaped quads.
	 The bitmap that we got from FreeType was not
	 oriented quite like we would like it to be,
	 so we need to link the texture to the quad
	 so that the result will be properly aligned. */
#if 0
	cout << "x:" << x << " y:" << y << " w:" << s_bitmap.width << " r:"
	<< s_bitmap.rows << " a:" << float(glyph->advance.x) / (pow(2, 16))
	<< endl;
#endif
	glBegin(GL_QUADS);
	glTexCoord2d(0, 0);
	glVertex2f(0, s_bitmap.rows);
	glTexCoord2d(0, y);
	glVertex2f(0, 0);
	glTexCoord2d(x, y);
	glVertex2f(s_bitmap.width, 0);
	glTexCoord2d(x, 0);
	glVertex2f(s_bitmap.width, s_bitmap.rows);
	glEnd();
	glPopMatrix();
	glTranslatef(glyph->advance.x >> 16, 0, 0);

	//Finnish the display list
	glEndList();

	FT_Done_Glyph(glyph);

}

void font::f_bitmap_set_screen_projection() {
	/* Add transform matrix during push */
	glPushAttrib(GL_TRANSFORM_BIT);
	/* Get current view port */
	GLint ai_viewport[4];
	glGetIntegerv(GL_VIEWPORT, ai_viewport);
	/* Move to Proj matrix */
	glMatrixMode(GL_PROJECTION);
	/* Push current matrix */
	glPushMatrix();

	/* Set a screen projection matrix : 1 -> 1 pixel */
	glLoadIdentity();
	gluOrtho2D(ai_viewport[0], ai_viewport[2], ai_viewport[1], ai_viewport[3]);
	glPopAttrib();
}

void font::f_bitmap_unset_screen_projection() {
	glPushAttrib(GL_TRANSFORM_BIT);
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glPopAttrib();
}

void font::f_render_as_bitmap(wchar_t * in_pc_char) {

	size_t sz_str = wcslen(in_pc_char);

	/* Project 2 points in order to determine bitmap length */
	GLdouble af_modelview_matrix[16];
	GLdouble af_projection_matrix[16];
	GLint ai_viewport[4];

	GLdouble f_p1_win_x, f_p1_win_y, f_p1_win_z;
	GLdouble f_p2_win_x, f_p2_win_y, f_p2_win_z;

	glGetDoublev(GL_MODELVIEW_MATRIX, af_modelview_matrix);
	glGetDoublev(GL_PROJECTION_MATRIX, af_projection_matrix);
	glGetIntegerv(GL_VIEWPORT, ai_viewport);

	gluProject(0.0, 0.0, 0.0, af_modelview_matrix, af_projection_matrix,
			ai_viewport, &f_p1_win_x, &f_p1_win_y, &f_p1_win_z);
	gluProject(0.0, 1.0, 0.0, af_modelview_matrix, af_projection_matrix,
			ai_viewport, &f_p2_win_x, &f_p2_win_y, &f_p2_win_z);

	size_t i_size = f_p2_win_y - f_p1_win_y;

	cout << "Requesting font size :" << i_size << endl;

	/* Set a screen projection matrix : 1 -> 1 pixel */
	f_bitmap_set_screen_projection();

	glPushAttrib(GL_LIST_BIT | GL_CURRENT_BIT | GL_ENABLE_BIT
			| GL_TRANSFORM_BIT);
	glMatrixMode(GL_MODELVIEW);
	glDisable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glListBase(_i_gl_bitmap_list);

	glPushMatrix();
	glLoadIdentity();
	glTranslated(f_p1_win_x, f_p1_win_y, 0);
	//glMultMatrixd(af_modelview_matrix);
	cout << "Rendering " << sz_str << " chars at " << f_p1_win_x << ","
			<< f_p1_win_y << endl;

	glCallLists(sz_str, GL_UNSIGNED_SHORT, in_pc_char);
	glPopMatrix();

	glPopAttrib();

	/* Go back to previous projection */
	f_bitmap_unset_screen_projection();
}

void font::f_render(wchar_t * in_pc_char) {
	if(_sz_bitmap) {
		f_render_as_bitmap(in_pc_char);
	} else {
		f_render_as_outline(in_pc_char);
	}
}

void font::f_render_as_outline(wchar_t * in_pc_char) {

	glEnable(GL_MULTISAMPLE);

	glPushMatrix();
	glScalef(1.0 / _i_width_max, 1.0 / _i_height_max, 1.0);
	glPushMatrix();

	wchar_t * pc_char = in_pc_char;
	wchar_t i_char;
	while ((i_char = *pc_char++)) {
		font_char & c_char = _m_chars[(wchar_t) i_char];

		glCallList(_i_gl_list + 2 * i_char);
		//cout << "Rendering " << _i_gl_list + i_char << " " << *pc_char << " " << c_char.f_get_width() <<endl;
		if (c_char.f_get_width() == 0.0) {
			glTranslatef(_i_width_max / 4, 0, 0);
		} else {
			glTranslatef(c_char.f_get_width(), 0, 0);
		}
	}
	glPopMatrix();
	glPushMatrix();

	pc_char = in_pc_char;

	/* Initialize shader */
	glUseProgram(_i_shader_program);

	while ((i_char = *pc_char++)) {
		font_char & c_char = _m_chars[(wchar_t) i_char];

		{
			Matrix4<GLdouble> m_modelview;
			Matrix4<GLdouble> m_projection;
			Matrix4<GLfloat> m_mvp;
			glGetDoublev(GL_MODELVIEW_MATRIX, m_modelview);
			glGetDoublev(GL_PROJECTION_MATRIX, m_projection);
			m_mvp = m_projection * m_modelview;
			/* Load shaders params */
			//glUniform1f(_i_uniform_f_sign, 1.0);
			glUniformMatrix4fv(_i_uniform_m_mvp, 1, 0, m_mvp.data);
		}

		glCallList(_i_gl_list + 2 * i_char + 1);

		if (c_char.f_get_width() == 0.0) {
			glTranslatef(_i_width_max / 4, 0, 0);
		} else {
			glTranslatef(c_char.f_get_width(), 0, 0);
		}

	}
	glUseProgram(0);
	glPopMatrix();
	glPopMatrix();

	glDisable(GL_MULTISAMPLE);
}

void font::f_gl_shader_compile(string const & in_str_filename,
		GLenum in_e_shader, GLuint in_i_shader_program) {
	/* Create shader */
	GLuint i_shader = glCreateShader(in_e_shader);
	std::string str_shader;

	/* Read file content */
	const char * pc_tmp;
	try {
		cout << "Opening " << in_str_filename << endl;
		std::ifstream t(in_str_filename, std::ios_base::in);

		t.seekg(0, std::ios::end);
		str_shader.reserve(t.tellg());
		t.seekg(0, std::ios::beg);

		str_shader.assign((std::istreambuf_iterator<char>(t)),
				std::istreambuf_iterator<char>());

	} catch (...) {
		cerr << "Unable to open " << in_str_filename << endl;
		exit(-1);
	}

	//pc_tmp = new char[pc_tmp];

	/* Load shader source code */
	pc_tmp = strdup(str_shader.c_str());
	cout << pc_tmp << endl;
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

void font::f_gl_shader_init(void) {

	/* Create shader program */
	GLuint i_shader_program = glCreateProgram();

	/* Geometry parameters */
#if 0
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
#endif

	/* Compile shader */
#if 1
	f_gl_shader_compile("src/glsl/font.vert.glsl", GL_VERTEX_SHADER,
			i_shader_program);
#endif
#if 1
	f_gl_shader_compile("src/glsl/font.geom.glsl", GL_GEOMETRY_SHADER,
			i_shader_program);
#endif

#if 1
	f_gl_shader_compile("src/glsl/font.frag.glsl", GL_FRAGMENT_SHADER,
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
	//_i_uniform_f_sign = glGetUniformLocation(_i_shader_program, "f_sign");
}

void font::f_init(const char * fname,size_t in_i_size_bitmap) {
	_sz_bitmap = in_i_size_bitmap;

	//Create and initilize a freetype font library.
	FT_Library library;
	if (FT_Init_FreeType(&library))
		throw std::runtime_error("FT_Init_FreeType failed");

	//The object in which Freetype holds information on a given
	//font is called a "face".
	FT_Face c_face;

	//This is where we load in the font information from the file.
	//Of all the places where the code might die, this is the most likely,
	//as FT_New_Face will die if the font file does not exist or is somehow broken.
	if (FT_New_Face(library, fname, 0, &c_face))
		throw std::runtime_error(
				"FT_New_Face failed (there is probably a problem with your font file)");

	uint32_t i_nb_chars = 65536;

	if (_sz_bitmap) {
		FT_Set_Char_Size(c_face, _sz_bitmap << 6, _sz_bitmap << 6, 96, 96);

		_ai_gl_bitmap_tex = new GLuint[i_nb_chars];

		/* Generate bitmap lists */
		_i_gl_bitmap_list = glGenLists(i_nb_chars);
		if (!_i_gl_list) {
			throw std::runtime_error("failed to create a list");
		}

		glGenTextures(i_nb_chars, _ai_gl_bitmap_tex);

	} else {
		/* Check if font is scalable */
		if (!c_face->face_flags & FT_FACE_FLAG_SCALABLE) {
			throw std::runtime_error("Font not scalable");
		}

		/* Generate outlines list */
		_i_gl_list = glGenLists(2 * i_nb_chars); // create a display list
		if (!_i_gl_list) {
			throw std::runtime_error("failed to create a list");
		}

	}

	/* Set char size */

	FT_UInt glyph_index;
	FT_ULong char_code = FT_Get_First_Char(c_face, &glyph_index);

	_i_width_max = 0;
	_i_height_max = 0;
	_i_width_avg = 0;
	uint32_t i_width_avg_cnt = 0;
	cout << __LINE__ << endl;
	while (glyph_index != 0) {
		//cout << "CHAR : " << std::hex << char_code << std::dec << " " << endl;
		try {
			font_char & c_new_char = _m_chars[char_code];

			// NO_SCALE = NO_HINTING, NO_BITMAP, and keeps font in design units
			if (FT_Load_Glyph(c_face, glyph_index, (_sz_bitmap ? FT_LOAD_DEFAULT : FT_LOAD_NO_SCALE))) {
				throw std::runtime_error("FT_Load_Glyph failed");
			}

			if (!_sz_bitmap) {
				/* Initialize */
				c_new_char.f_init_as_outline(char_code, c_face, _i_gl_list + 2 * char_code);

				/* Update scale */
				_i_width_max = max(c_new_char.f_get_width(), _i_width_max);
				_i_height_max = max(c_new_char.f_get_width(), _i_height_max);
				_i_width_avg += c_new_char.f_get_width();
				i_width_avg_cnt++;

			} else {
				/* Init bitmap font */
				c_new_char.f_init_as_bitmap(char_code, c_face, _ai_gl_bitmap_tex[char_code], _i_gl_bitmap_list
								+ char_code);
			}

		} catch (std::exception &e) {
			cerr << e.what() << endl;
			exit(-1);
		}

		char_code = FT_Get_Next_Char(c_face, char_code, &glyph_index);
	}
	if (!_sz_bitmap) {
		_i_width_avg /= i_width_avg_cnt;
	}

	//We don't need the face information now that the display
	//lists have been created, so we free the assosiated resources.
	FT_Done_Face(c_face);

	//Ditto for the library.
	FT_Done_FreeType(library);

	{
		GLint bufs;
		GLint samples;
		glGetIntegerv(GL_SAMPLE_BUFFERS, &bufs);
		glGetIntegerv(GL_SAMPLES, &samples);
		cout << "Have " << bufs << " buffers and " << samples << " samples"
				<< endl;
	}

	/* Initialize bezier curve shaders */
	f_gl_shader_init();

	//exit(-1);

}

#if 0
///Create a display list coresponding to the give character.
void make_dlist(FT_Face face, char ch, GLuint list_base, GLuint * tex_base) {

	//The first thing we do is get FreeType to render our character
	//into a bitmap.  This actually requires a couple of FreeType commands:

	//Load the Glyph for our character.
	if (FT_Load_Glyph(face, FT_Get_Char_Index(face, ch), FT_LOAD_DEFAULT))
	throw std::runtime_error("FT_Load_Glyph failed");

	//Move the face's glyph into a Glyph object.
	FT_Glyph glyph;
	if (FT_Get_Glyph(face->glyph, &glyph))
	throw std::runtime_error("FT_Get_Glyph failed");

#if 1
	FT_Set_Char_Size(face, 200 << 6, 200 << 6, 96, 96);
#endif

	//Convert the glyph to a bitmap.
	FT_Glyph_To_Bitmap(&glyph, ft_render_mode_normal, 0, 1);
	FT_BitmapGlyph bitmap_glyph = (FT_BitmapGlyph) glyph;

	//This reference will make accessing the bitmap easier
	FT_Bitmap& bitmap = bitmap_glyph->bitmap;

	//Use our helper function to get the widths of
	//the bitmap data that we will need in order to create
	//our texture.
	int width = next_p2(bitmap.width);
	int height = next_p2(bitmap.rows);

	//Allocate memory for the texture data.
	GLubyte* expanded_data = new GLubyte[2 * width * height];

	//Here we fill in the data for the expanded bitmap.
	//Notice that we are using two channel bitmap (one for
	//luminocity and one for alpha), but we assign
	//both luminocity and alpha to the value that we
	//find in the FreeType bitmap.
	//We use the ?: operator so that value which we use
	//will be 0 if we are in the padding zone, and whatever
	//is the the Freetype bitmap otherwise.
	for (int j = 0; j < height; j++) {
		for (int i = 0; i < width; i++) {
			expanded_data[2 * (i + j * width)] = expanded_data[2 * (i + j
					* width) + 1] = (i >= bitmap.width || j >= bitmap.rows) ? 0
			: bitmap.buffer[i + bitmap.width * j];
		}
	}

	//Now we just setup some texture paramaters.
	glBindTexture(GL_TEXTURE_2D, tex_base[ch]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	//Here we actually create the texture itself, notice
	//that we are using GL_LUMINANCE_ALPHA to indicate that
	//we are using 2 channel data.
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0,
			GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE, expanded_data);

	//With the texture created, we don't need to expanded data anymore
	delete[] expanded_data;

	//So now we can create the display list
	glNewList(list_base + ch, GL_COMPILE);

	glBindTexture(GL_TEXTURE_2D, tex_base[ch]);

	//first we need to move over a little so that
	//the character has the right amount of space
	//between it and the one before it.
	glTranslatef(bitmap_glyph->left, 0, 0);

	//Now we move down a little in the case that the
	//bitmap extends past the bottom of the line
	//(this is only true for characters like 'g' or 'y'.
	glPushMatrix();
	glTranslatef(0, bitmap_glyph->top - bitmap.rows, 0);

	//Now we need to account for the fact that many of
	//our textures are filled with empty padding space.
	//We figure what portion of the texture is used by
	//the actual character and store that information in
	//the x and y variables, then when we draw the
	//quad, we will only reference the parts of the texture
	//that we contain the character itself.
	float x = (float) bitmap.width / (float) width, y = (float) bitmap.rows
	/ (float) height;

	//Here we draw the texturemaped quads.
	//The bitmap that we got from FreeType was not
	//oriented quite like we would like it to be,
	//so we need to link the texture to the quad
	//so that the result will be properly aligned.
	glBegin(GL_QUADS);
	glTexCoord2d(0, 0);
	glVertex2f(0, bitmap.rows);
	glTexCoord2d(0, y);
	glVertex2f(0, 0);
	glTexCoord2d(x, y);
	glVertex2f(bitmap.width, 0);
	glTexCoord2d(x, 0);
	glVertex2f(bitmap.width, bitmap.rows);
	glEnd();
	glPopMatrix();
	glTranslatef(face->glyph->advance.x >> 6, 0, 0);

	//increment the raster position as if we were a bitmap font.
	//(only needed if you want to calculate text length)
	//glBitmap(0,0,0,0,face->glyph->advance.x >> 6,0,NULL);

	//Finnish the display list
	glEndList();
}

void font_data::init(const char * fname, unsigned int h) {
	//Allocate some memory to store the texture ids.
	textures = new GLuint[128];

	this->h = h;

	//Create and initilize a freetype font library.
	FT_Library library;
	if (FT_Init_FreeType(&library))
	throw std::runtime_error("FT_Init_FreeType failed");

	//The object in which Freetype holds information on a given
	//font is called a "face".
	FT_Face face;

	//This is where we load in the font information from the file.
	//Of all the places where the code might die, this is the most likely,
	//as FT_New_Face will die if the font file does not exist or is somehow broken.
	if (FT_New_Face(library, fname, 0, &face))
	throw std::runtime_error(
			"FT_New_Face failed (there is probably a problem with your font file)");

	//For some twisted reason, Freetype measures font size
	//in terms of 1/64ths of pixels.  Thus, to make a font
	//h pixels high, we need to request a size of h*64.
	//(h << 6 is just a prettier way of writting h*64)
	FT_Set_Char_Size(face, h << 6, h << 6, 96, 96);

	//Here we ask opengl to allocate resources for
	//all the textures and displays lists which we
	//are about to create.
	list_base = glGenLists(128);
	glGenTextures(128, textures);

	//This is where we actually create each of the fonts display lists.
	for (unsigned char i = 0; i < 128; i++)
	make_dlist(face, i, list_base, textures);

	//We don't need the face information now that the display
	//lists have been created, so we free the assosiated resources.
	FT_Done_Face(face);

	//Ditto for the library.
	FT_Done_FreeType(library);
}

void font_data::clean() {
	glDeleteLists(list_base, 128);
	glDeleteTextures(128, textures);
	delete[] textures;
}

/// A fairly straight forward function that pushes
/// a projection matrix that will make object world
/// coordinates identical to window coordinates.
inline void pushScreenCoordinateMatrix() {
	glPushAttrib(GL_TRANSFORM_BIT);
	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(viewport[0], viewport[2], viewport[1], viewport[3]);
	glPopAttrib();
}

/// Pops the projection matrix without changing the current
/// MatrixMode.
inline void pop_projection_matrix() {
	glPushAttrib(GL_TRANSFORM_BIT);
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glPopAttrib();
}

///Much like Nehe's glPrint function, but modified to work
///with freetype fonts.
void print(const font_data &ft_font, float x, float y, const char *fmt, ...) {

	// We want a coordinate system where things coresponding to window pixels.
	pushScreenCoordinateMatrix();

	GLuint font = ft_font.list_base;
	float h = ft_font.h / .63f; //We make the height about 1.5* that of

	char text[256]; // Holds Our String
	va_list ap; // Pointer To List Of Arguments

	if (fmt == NULL) // If There's No Text
	*text = 0; // Do Nothing

	else {
		va_start(ap, fmt); // Parses The String For Variables
		vsprintf(text, fmt, ap); // And Converts Symbols To Actual Numbers
		va_end(ap); // Results Are Stored In Text
	}

	//Here is some code to split the text that we have been
	//given into a set of lines.
	//This could be made much neater by using
	//a regular expression library such as the one avliable from
	//boost.org (I've only done it out by hand to avoid complicating
	//this tutorial with unnecessary library dependencies).
	const char *start_line = text;
	vector<string> lines;

	const char * c = text;
	;

	//for(const char *c=text;*c;c++) {
	for (; *c; c++) {
		if (*c == '\n') {
			string line;
			for (const char *n = start_line; n < c; n++)
			line.append(1, *n);
			lines.push_back(line);
			start_line = c + 1;
		}
	}
	if (start_line) {
		string line;
		for (const char *n = start_line; n < c; n++)
		line.append(1, *n);
		lines.push_back(line);
	}

	glPushAttrib(GL_LIST_BIT | GL_CURRENT_BIT | GL_ENABLE_BIT
			| GL_TRANSFORM_BIT);
	glMatrixMode(GL_MODELVIEW);
	glDisable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glListBase(font);

	float modelview_matrix[16];
	glGetFloatv(GL_MODELVIEW_MATRIX, modelview_matrix);

	//This is where the text display actually happens.
	//For each line of text we reset the modelview matrix
	//so that the line's text will start in the correct position.
	//Notice that we need to reset the matrix, rather than just translating
	//down by h. This is because when each character is
	//draw it modifies the current matrix so that the next character
	//will be drawn immediatly after it.
	for (unsigned int i = 0; i < lines.size(); i++) {

		glPushMatrix();
		glLoadIdentity();
		glTranslatef(x, y - h * i, 0);
		glMultMatrixf(modelview_matrix);

		//  The commented out raster position stuff can be useful if you need to
		//  know the length of the text that you are creating.
		//  If you decide to use it make sure to also uncomment the glBitmap command
		//  in make_dlist().
		//	glRasterPos2f(0,0);
		glCallLists(lines[i].length(), GL_UNSIGNED_BYTE, lines[i].c_str());
		//	float rpos[4];
		//	glGetFloatv(GL_CURRENT_RASTER_POSITION ,rpos);
		//	float len=x-rpos[0];

		glPopMatrix();

	}

	glPopAttrib();

	pop_projection_matrix();
}

///Much like Nehe's glPrint function, but modified to work
///with freetype fonts.
void print2(const font_data &ft_font, const char *fmt, ...) {

	// We want a coordinate system where things coresponding to window pixels.
	//pushScreenCoordinateMatrix();
	glPushMatrix();
#if 0
	glTranslatef(0.2, 0.2, 0);

	glBegin(GL_QUADS);
	glVertex3f(0.0,0.0,0.0);
	glVertex3f(0.0,1.0,0.0);
	glVertex3f(1.0,1.0,0.0);
	glVertex3f(1.0,0.0,0.0);
	glEnd();
#endif

	glScalef(1.0 / ft_font.h, 1.0 / ft_font.h, 1.0 / ft_font.h);

	GLuint font = ft_font.list_base;
	float h = ft_font.h / .63f; //We make the height about 1.5* that of

	char text[256]; // Holds Our String
	va_list ap; // Pointer To List Of Arguments

	if (fmt == NULL) // If There's No Text
	*text = 0; // Do Nothing

	else {
		va_start(ap, fmt); // Parses The String For Variables
		vsprintf(text, fmt, ap); // And Converts Symbols To Actual Numbers
		va_end(ap); // Results Are Stored In Text
	}

	//Here is some code to split the text that we have been
	//given into a set of lines.
	//This could be made much neater by using
	//a regular expression library such as the one avliable from
	//boost.org (I've only done it out by hand to avoid complicating
	//this tutorial with unnecessary library dependencies).
	const char *start_line = text;
	vector<string> lines;

	const char * c = text;
	;

	//for(const char *c=text;*c;c++) {
	for (; *c; c++) {
		if (*c == '\n') {
			string line;
			for (const char *n = start_line; n < c; n++)
			line.append(1, *n);
			lines.push_back(line);
			start_line = c + 1;
		}
	}
	if (start_line) {
		string line;
		for (const char *n = start_line; n < c; n++)
		line.append(1, *n);
		lines.push_back(line);
	}

	glPushAttrib(GL_LIST_BIT | GL_CURRENT_BIT | GL_ENABLE_BIT
			| GL_TRANSFORM_BIT);
	glMatrixMode(GL_MODELVIEW);
	glDisable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glListBase(font);

	float modelview_matrix[16];
	glGetFloatv(GL_MODELVIEW_MATRIX, modelview_matrix);

	//This is where the text display actually happens.
	//For each line of text we reset the modelview matrix
	//so that the line's text will start in the correct position.
	//Notice that we need to reset the matrix, rather than just translating
	//down by h. This is because when each character is
	//draw it modifies the current matrix so that the next character
	//will be drawn immediatly after it.
	for (unsigned int i = 0; i < lines.size(); i++) {

		glPushMatrix();
		//glLoadIdentity();
		//glTranslatef(x, y - h * i, 0);
		//glMultMatrixf(modelview_matrix);

		//  The commented out raster position stuff can be useful if you need to
		//  know the length of the text that you are creating.
		//  If you decide to use it make sure to also uncomment the glBitmap command
		//  in make_dlist().
		//	glRasterPos2f(0,0);
		glCallLists(lines[i].length(), GL_UNSIGNED_BYTE, lines[i].c_str());
		//	float rpos[4];
		//	glGetFloatv(GL_CURRENT_RASTER_POSITION ,rpos);
		//	float len=x-rpos[0];

		glPopMatrix();

	}

	glPopAttrib();

	glPopMatrix();
	//pop_projection_matrix();
}
#endif
}

