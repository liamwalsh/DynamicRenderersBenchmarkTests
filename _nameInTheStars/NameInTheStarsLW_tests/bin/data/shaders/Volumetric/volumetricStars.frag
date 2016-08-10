#version 120
#extension GL_ARB_texture_rectangle : enable
uniform sampler2DRect  tex0;
uniform int iterations;
uniform int iterationsMultiplier;
uniform int volsteps;
uniform int volstepsMultiplier;
////
uniform float formuparam;
uniform float forumparamMultiplier;

uniform float stepsize;
uniform float stepsizeMultiplier;

uniform float zoom;
uniform float zoomMultiplier;

uniform float brightness;
uniform float brightnessMultiplier;

uniform float darkmatter;
uniform float distfading;
uniform float distfadingMultiplier;

uniform float saturation;
uniform float saturationMultiplier;

uniform float tile;

uniform float   time;
uniform float timeMultiplier;
//name things


//uniform vec2 renderDimensions;

float lumin(float R, float G, float B)
{
    return (R+R+B+G+G+G)/6.0;
}

void main()
{
    //TODO: uniform for renderDimensions also
    vec2 renderDimensions = vec2(3596.0, 2657.0);
    vec2 pos = gl_FragCoord.xy;// / renderDimensions;//gl_FragCoord.st;
    vec4 rName = texture2DRect(tex0, pos).rgba;
    
    
    gl_FragColor = vec4(pos.x, pos.y, 0.0, 1.0);
    
}