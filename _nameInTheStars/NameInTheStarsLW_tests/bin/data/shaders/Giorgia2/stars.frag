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

uniform float paperWidth;
uniform float paperHeight;
//TODO - shrink this shit down

uniform int   time;
uniform float timeMultiplier;
//name things
uniform sampler2DRect nameTexture;

void main()
{
    
    //name stuff
    vec2 iResolution = vec2(paperWidth, paperHeight);
    
    vec2 namePos = gl_FragCoord.xy / iResolution.xy;
//    namePos.y = (windowHeight / 2) - (namePos.y * -.5);
//    namePos.y = windowHeight - namePos.y;
//    namePos.y = 1 - namePos.y - 0.5;
    
    namePos *= iResolution;
    vec4 rName = texture(nameTexture, namePos);
    //end name stuff
    //multiply uniform
    
    int iterationsCombined = iterations + int(rName.r * float(iterationsMultiplier));
    int volstepsCombined = volsteps + int(rName.r * float(volstepsMultiplier));
        
    vec2 uv = gl_FragCoord.xy/iResolution.xy-.5;
    uv.y *= iResolution.y/iResolution.x;
    
    vec3 dir=vec3(uv*zoom,1.);
    
    vec3 from=vec3(1.,.5,0.5);
    
    vec3 forward = vec3(0.,0.,1.);
    
    float a1 = 0.3;
    mat2 rot1 = mat2(cos(a1),sin(a1),-sin(a1),cos(a1));
    float a2 = .6;
    mat2 rot2 = mat2(cos(a2),sin(a2),-sin(a2),cos(a2));
    dir.xz*=rot1;
    forward.xz *= rot1;
    dir.yz*=rot1;
    forward.yz *= rot1;
    float dFading = distfading + (rName.r * distfadingMultiplier);
//    distfading *= rName.r;
    //zoom
//    float time = 500.00;
    float zooom = float(time);
    from += forward * time;
    float sampleShift = mod( zooom, stepsize );
    float zoffset = -sampleShift;
    sampleShift /= stepsize; // make from 0 to 1
    float fParamName = formuparam + (rName.r * forumparamMultiplier);
    //volumetric rendering
    float s=0.1;
    
//    float s = 0.1 + (rName.r * 0.1);
    
    vec3 v=vec3(0.0);
    for (int r=0; r<volstepsCombined; r++) {
        float nZ_offset = zoffset;// * rName.r;
        vec3 p=from+(s+nZ_offset)*dir;// + vec3(0.,0.,zoffset);
        p = abs(vec3(tile)-mod(p,vec3(tile*2.))); // tiling fold
        float pa,a=pa=0.;
        for (int i=0; i<iterationsCombined; i++) {
//            if (rName.r > 0.5)
//            {
              p=abs(p)/dot(p,p)-fParamName; // the magic formula
//            }
//            else{
//                p=abs(p)/max(dot(p,p),0.005)-formuparam; // another interesting way to reduce noise
//            }
//            p=abs(p)/dot(p,p)-fParamName; // the magic formula
            
            float D = abs(length(p)-pa); // absolute sum of average change
            a += i > 7 ? min( 12., D) : D;
            pa=length(p);
        }
        float dm=max(0.,darkmatter-a*a*.001); //dark matter
        a*=a*a; // add contrast
        
        // brightens stuff up a bit
        float s1 = s+zoffset;
        // need closed form expression for this, now that we shift samples
        float fade = pow(dFading,max(0.,float(r)-sampleShift));
        
        //if (r>3) fade*=1.-dm; // dark matter, don't render near
        
//        fade = rName.r * .045;
        
        v+=fade;
        
        // fade out samples as they approach the camera
        if( r == 0 )
        {
            fade *= 1. - sampleShift;
        }
//
        // fade in samples as they approach from the distance
        if( r == volsteps-1 )
            fade *= sampleShift;
        v+=vec3(2.*s1,4.*s1*s1,16.*s1*s1*s1*s1)*a*brightness*fade; // coloring based on distance
        s += stepsize + (rName.r * stepsizeMultiplier);// + (rName.r * stepsize * 0.0004);
//        s+=stepsize;
    }
    v=mix(vec3(length(v)),v,saturation); //color adjust
    vec4 starResult = vec4(v * .01, rName.b);
    outputColor = starResult;
//    outputColor = vec4(1.0, 1.0, 1.0, rName.b);
    //this was working
//    outputColor = vec4(v*.01,1.);
    
//    float r = gl_FragCoord.x / windowWidth;
//    float g = gl_FragCoord.y / windowHeight;
//    float b = 1.0;
//    float a = 1.0;
//    outputColor = vec4(r, g, b, a);
}