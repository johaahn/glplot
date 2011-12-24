#version 400
in vec4 v_vertex_color[3];
in float f_vertex_sign[3];

out vec2 v_frag_conic;
out float f_frag_sign;

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

void main() {



	for (int i = 0; i < gl_in.length(); i++) {
		gl_Position = gl_in[i].gl_Position;
		f_frag_sign = f_vertex_sign[i];

		if(i==0) {
			v_frag_conic = vec2(1.0,0.0);
		} else if(i == 1) {
			v_frag_conic = vec2(0.0,1.0);
		} else {
			v_frag_conic = vec2(0.0,0.0);
		}
		EmitVertex();
	}
	EndPrimitive();
}
