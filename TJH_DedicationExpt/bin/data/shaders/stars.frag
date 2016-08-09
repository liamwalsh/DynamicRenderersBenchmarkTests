#version 150

out vec4 outputColor;
//star field things
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
uniform sampler2DRect facesTexture;

//#define iterations 19
//#define formuparam 0.53

//#define volsteps 22
//#define stepsize 0.050

//#define zoom   0.800
//#define tile   0.850
//#define speed  0.0000001

//#define brightness 0.0015
//#define darkmatter 0.300
//#define distfading 0.760
//#define saturation 0.800

float lumin(float R, float G, float B)
{
    return (R+R+B+G+G+G)/6;
}

void main()
{
    float windowWidth = 1198.0;
    float windowHeight = 866.0;
    //name stuff
    vec2 iResolution = vec2(windowWidth, windowHeight);
    
    vec2 facePos = gl_FragCoord.xy / iResolution.xy;
    //    namePos.y = (windowHeight / 2) - (namePos.y * -.5);
    //    namePos.y = windowHeight - namePos.y;
    facePos.y = 1 - facePos.y;
    
    facePos *= iResolution;
    vec4 rFace = texture(facesTexture, facePos);
    float luminVal = lumin(rFace.r, rFace.g, rFace.b);
    float dMatter = 0.9 - (luminVal * 2);//darkmatter + (0.6 * (0.5 - luminVal));
    vec2 uv = gl_FragCoord.xy/iResolution.xy-.5;
    uv.y *= iResolution.y/iResolution.x;
    
    vec3 dir=vec3(uv*zoom,1.);
    
//    float time=0.0;//iGlobalTime*speed+.25;
    
    //mouse rotation
    float a1=.15;//+iMouse.x/iResolution.x*2.;
    float a2=.1;//+iMouse.y/iResolution.y*2.;
    mat2 rot1=mat2(cos(a1),sin(a1),-sin(a1),cos(a1));
    mat2 rot2=mat2(cos(a2),sin(a2),-sin(a2),cos(a2));
    dir.xz*=rot1;
    dir.xy*=rot2;
    vec3 from=vec3(1.,.5,0.5);
    from+=vec3(time*2.,time,-2.);
    from.xz*=rot1;
    from.xy*=rot2;
    
    int totalIterations = iterations + int(float(iterationsMultiplier) * luminVal);
    int totalVolumeSteps = volsteps + int(float(volstepsMultiplier) * luminVal);
    float totalStepSize = stepsize + (stepsizeMultiplier * luminVal);
    //volumetric rendering
    float s=0.1,fade=1.;
    vec3 v=vec3(0.);
    for (int r=0; r<totalVolumeSteps; r++) {
        vec3 p=from+s*dir*.5;
        p = abs(vec3(tile)-mod(p,vec3(tile*2.))); // tiling fold
        float pa,a=pa=0.;
        for (int i=0; i<totalIterations; i++) {
            p=abs(p)/dot(p,p)-formuparam; // the magic formula
            a+=abs(length(p)-pa); // absolute sum of average change
            pa=length(p);
        }
        float dm=max(0.,dMatter-a*a*.001); //dark matter
        a*=a*a; // add contrast
        if (r>6) fade*=1.-dm; // dark matter, don't render near
        //v+=vec3(dm,dm*.5,0.);
        v+=fade;
        v+=vec3(s,s*s,s*s*s*s)*a*brightness*fade; // coloring based on distance
        fade*=distfading; // distance fading
        s+=totalStepSize;
    }
    v=mix(vec3(length(v)),v,saturation);
    //v=mix(vec3(length(v)),v,saturation); //color adjust
//    outputColor = vec4(luminVal, luminVal, luminVal, 1.0);
    //True output below
    outputColor = vec4(v*.01,1.);
    
    
    
//    float r = gl_FragCoord.x / windowWidth;
//    float g = gl_FragCoord.y / windowHeight;
//    float b = 1.0;
//    float a = 1.0;
    //outputColor = vec4(rFace.r, rFace.g, rFace.b, 1.0);
//    outputColor = vec4(r, g, b, a);
}