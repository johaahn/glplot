uniform mat4 m_mvp;
varying vec2 texCoord;
uniform int i_sz_x;
uniform int i_sz_y;

void main()
{	
	//Transform the vertex (ModelViewProj matrix)
	gl_Position = m_mvp*gl_Vertex;
	gl_FrontColor = gl_Color;
	gl_TexCoord[0] = gl_MultiTexCoord0;
	
	texCoord.x = gl_Vertex.x;// * (i_sz_x-1);
	texCoord.y = gl_Vertex.y;// * (i_sz_y-1);
}
