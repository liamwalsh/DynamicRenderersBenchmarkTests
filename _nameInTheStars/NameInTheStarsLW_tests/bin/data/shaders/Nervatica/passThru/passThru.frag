varying vec2 textureCoordinate;

uniform sampler2D inputImageTexture;
uniform sampler2D inputImageTexture2;

uniform float thresholdSensitivity;
uniform float smoothing;
uniform vec3 keyColor;

uniform float width;
uniform float height;

float luminV4(vec4 tCol)
{
    return dot(tCol.rgb, vec3(0.299, 0.587, 0.114) * 2.0);
    
}

float getMeAlpha(vec4 tCol)
{
    float mult = dot(tCol.rgb, vec3(0.299, 0.587, 0.114));
    return smoothstep(0.0, 0.10, mult) * mult * 3.0;
}

void main()
{
    vec2 u_resolution = vec2(3596,2657);
    vec2 st = gl_FragCoord.xy/u_resolution;
    vec4 colBlack = texture2D(inputImageTexture, st);
    vec4 colBlended = texture2D(inputImageTexture2, st);
    
    float maskY = 0.2989 * keyColor.r + 0.5866 * keyColor.g + 0.1145 * keyColor.b;
    float maskCr = 0.7132 * (keyColor.r - maskY);
    float maskCb = 0.5647 * (keyColor.b - maskY);
    
    float Y = 0.2989 * colBlack.r + 0.5866 * colBlack.g + 0.1145 * colBlack.b;
    float Cr = 0.7132 * (colBlack.r - Y);
    float Cb = 0.5647 * (colBlack.b - Y);
    float alpha = getMeAlpha(colBlack);
    float blendValue = 1.0 - smoothstep(thresholdSensitivity, thresholdSensitivity + smoothing, distance(vec2(Cr, Cb), vec2(maskCr, maskCb)));
//    gl_FragColor = mix(colBlack, colBlended, blendValue);
    
    gl_FragColor = vec4(colBlended.rgb, colBlack.r);// vec4(colBlended.rgb, blendValue);//vec4(colBlended.rgb, alpha);
    
    
//    gl_FragColor = vec4(keyColor.rgb, 1.0);//vec4(colBlended.rgb, alpha);//vec4(st.x, st.y, 0.0, 1.0);
    
 
}
//
//
//float lumin(float R, float G, float B)
//{
//    //return (R+R+B+G+G+G)/5.0;
//    return (R+R+B+G+G+G)/6.0;
//}
//
