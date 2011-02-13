uniform sampler2DRect src_tex_unit0;

float normalStrength = 16.0;

void main(void) {
	
	vec2 st = gl_TexCoord[0].st;
	
	float tl = texture2DRect (src_tex_unit0, st + vec2(-1.0, -1.0) ).x;   // top left
	float  l = texture2DRect (src_tex_unit0, st + vec2(-1.0,  0.0) ).x;   // left
	float bl = texture2DRect (src_tex_unit0, st + vec2(-1.0,  1.0) ).x;   // bottom left
	float  t = texture2DRect (src_tex_unit0, st + vec2( 0.0, -1.0) ).x;   // top
	float  b = texture2DRect (src_tex_unit0, st + vec2( 0.0,  1.0) ).x;   // bottom
	float tr = texture2DRect (src_tex_unit0, st + vec2( 1.0, -1.0) ).x;   // top right
	float  r = texture2DRect (src_tex_unit0, st + vec2( 1.0,  0.0) ).x;   // right
	float br = texture2DRect (src_tex_unit0, st + vec2( 1.0,  1.0) ).x;   // bottom right
	
	// Compute dx using Sobel:
	//           -1 0 1 
	//           -2 0 2
	//           -1 0 1
	float dX = tr + 2.0*r + br -tl - 2.0*l - bl;
 
	// Compute dy using Sobel:
	//           -1 -2 -1 
	//            0  0  0
	//            1  2  1
	float dY = bl + 2.0*b + br -tl - 2.0*t - tr;
 
	// Build the normalized normal
	vec4 N = vec4(normalize(vec3(dX, 1.0 / normalStrength, dY)), 1.0);
 
	//convert (-1.0 , 1.0) to (0.0 , 1.0), if needed
	gl_FragColor = N * 0.5 + 0.5;
 }