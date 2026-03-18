#if defined(GL_ES)
    precision mediump float;
#endif

#if !defined(GL_ES) && __VERSION__ >= 130
    #define IN in
    #define OUT out
    #define TEXTURE texture
#else
    #define IN attribute
    #define OUT varying
    #define TEXTURE texture2D
#endif

varying vec2 fragTexCoord;
varying vec4 fragColor;

uniform sampler2D texture0;
uniform vec4 colDiffuse;
uniform vec4 colVignette;

void main() {
    vec4 texelColor = TEXTURE(texture0, fragTexCoord);

    vec4 finalColor = texelColor * fragColor * colDiffuse;

    vec2 uv = fragTexCoord - 0.5;
    float dist = length(uv);
    float mask = smoothstep(0.3, 0.95, dist);
    gl_FragColor = vec4(mix(finalColor.rgb, colVignette.rgb, mask * 0.3), 1.0);
}