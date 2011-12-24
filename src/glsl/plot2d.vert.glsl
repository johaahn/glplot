uniform mat4 m_mvp;

void main()
{	
	//Transform the vertex (ModelViewProj matrix)
	gl_Position = m_mvp*gl_Vertex;
	gl_FrontColor = gl_Color;

}
