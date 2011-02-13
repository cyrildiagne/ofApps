attribute vec3 aVertexPosition;
attribute vec3 aVertexNormal;

uniform mat4 uMVMatrix;
uniform mat4 uPMatrix;
uniform mat4 uNMatrix;

varying vec3 vVertexNormal;

varying vec3 vNormal;
varying vec3 lightDir, eyeVec;

void main()
{	
	vec3 vVertex = vec3(uMVMatrix * vec4(aVertexPosition, 1.0));

	lightDir = vec3(vec3(1.0, 1.0, 1.0) - vVertex);
	eyeVec = -vVertex;
	
	vVertexNormal = aVertexNormal;
	vNormal = (uNMatrix*vec4(aVertexNormal, 1.0)).xyz;
	
	gl_Position = uPMatrix * uMVMatrix * vec4(aVertexPosition, 1.0);
}
