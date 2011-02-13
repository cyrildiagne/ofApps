varying vec3 vLightWeighting;

void main(void) {
	//vec4 textureColor = texture2D(uSampler, vec2(vTextureCoord.s, vTextureCoord.t));
	gl_FragColor = vec4(vec3(0.5, 0.5, 0.5) * vLightWeighting, 1.0);
}