#version 400
in vec4 v_position;
//in vec4 v_color;
in vec3 v_coord_0;

out vec4 v_vertex_color;
out float f_vertex_sign;
uniform mat4 m_mvp;
void main()
{	
	//Transform the vertex (ModelViewProj matrix)
	gl_Position = m_mvp*vec4(v_position.x,v_position.y,0.0, 1.0);
	f_vertex_sign = v_position.z;
	//v_vertex_tex_coord_0.x = 1.0;
	//v_vertex_color = v_color;
}
