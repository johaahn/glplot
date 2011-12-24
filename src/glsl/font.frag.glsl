#version 400
in vec2 v_frag_conic;
in float f_frag_sign;
void main() {

	//if(v_frag_conic.x < 0) discard;
	//if(v_frag_conic.y < 0) discard;

	float res = v_frag_conic.y- (1.0- 2.0*sqrt(abs(v_frag_conic.x)) + v_frag_conic.x);
	float res2 = v_frag_conic.x- (1.0- 2.0*sqrt(abs(v_frag_conic.y)) + v_frag_conic.y);

	float omega = 0.1;

	res *= f_frag_sign;
	res2 *= f_frag_sign;

#if 0
	if (res2 >= 0){
			gl_FragColor.w = 1.0;
	} else {
		gl_FragColor.w = 0.0;
	}
#else

	if ((res2 <= 0) || (res <= 0)){
			gl_FragColor.w = 0.0;
	} else if ((res2 < omega) || (res < omega)){
			gl_FragColor.w = mix(1.0,0.0,min(omega-res,omega-res2)/(omega));;
			//gl_FragColor.w = 1.0;
	} else {
		gl_FragColor.w = 1.0;
	}
#endif

	gl_FragColor.rgb = vec3(0.0, 0.0, 0.0);

}

