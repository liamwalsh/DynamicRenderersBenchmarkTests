#version 150
uniform sampler2D tex;
in vec4 vColor;
in vec2 texCoordVarying;

out vec4 outputColor;

void main (void) {
    
    outputColor = texture(tex, gl_PointCoord) * vColor;
}