varying vec3 vVertexNormal;

varying vec3 vNormal;
varying vec3 lightDir, eyeVec;

const vec4 matAmbient = vec4(0.0, 0.1, 0.3, 0.2);
const vec4 matDiffuse = vec4(0.0, 0.4, 0.6, 0.7);
const vec4 matSpecular = vec4(1.0, 1.0, 1.0, 0.2);
const float matShininess = 64.0;

const vec4 lightAmbient = vec4(1.0, 1.0, 1.0, 0.5);
const vec4 lightDiffuse = vec4(1.0, 1.0, 1.0, 0.5);
const vec4 lightSpecular = vec4(1.0, 1.0, 1.0, 0.5);

void main (void)
{
	vec4 final_color = lightAmbient * matAmbient;
							
	vec3 N = normalize(vNormal);
	vec3 L = normalize(lightDir);
	
	float lambertTerm = dot(N,L);
	
	if(lambertTerm > 0.0)
	{
		final_color += lightDiffuse * 
		               matDiffuse * 
					   lambertTerm;	
		
		vec3 E = normalize(eyeVec);
		vec3 R = reflect(-L, N);
		float specular = pow( max(dot(R, E), 0.0), matShininess );
		
		final_color += lightSpecular * 
		               matSpecular * 
					   specular;	
	}
	
	gl_FragColor = vec4(vVertexNormal, 1.0);
}