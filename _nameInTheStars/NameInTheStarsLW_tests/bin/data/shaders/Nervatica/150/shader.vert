#version 150
uniform mat4 modelViewProjectionMatrix;
in vec4 position;
in vec3 normal;
in vec4 color;
in vec2 texcoord;

out vec4 vColor;
out vec2 texCoordVarying;


void main() {

    gl_Position   = modelViewProjectionMatrix * position;
	float size    = normal.x;
    gl_PointSize  = size;
    vColor      = color;

}