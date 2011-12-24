#version 330
//#extension EXT_gpu_shader4 : enable
uniform samplerBuffer t_image;
uniform samplerBuffer t_colormap;
uniform int i_sz_x;
uniform int i_sz_y;
uniform int i_sz_colormap;
uniform float f_min;
uniform float f_max;

in vec2 texCoord;
void main()
{
	int coord = int(texCoord.x)*i_sz_y + int(texCoord.y);
    float f_value = texelFetch (t_image, coord).r;
    int i_value = int(((f_value - f_min)*float(i_sz_colormap-1))/(f_max-f_min));
    gl_FragColor.rgb = texelFetch (t_colormap, clamp(i_value,0,i_sz_colormap-1)).rgb;
    //gl_FragColor.r = clamp(i_value,0,255)/255.0;
	gl_FragColor.a = 1.0;
}
