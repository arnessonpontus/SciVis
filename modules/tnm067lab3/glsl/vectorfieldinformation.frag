#include "utils/structs.glsl"

uniform sampler2D vfColor;
uniform ImageParameters vfParameters;
in vec3 texCoord_;

vec2 partialDerivateX(vec2 coord, vec2 pixelSize){
    float dx = pixelSize.x;
    vec2 plusx = texture(vfColor, coord + vec2(dx,0)).xy;
    vec2 minusx = texture(vfColor, coord + vec2(-dx,0)).xy;

    vec2 partial_derivate = (plusx - minusx) / (2.0 * dx);
    return partial_derivate;
}

vec2 partialDerivateY(vec2 coord, vec2 pixelSize){
    float dy = pixelSize.y;
    vec2 plusy = texture(vfColor, coord + vec2(0,dy)).xy;
    vec2 minusy = texture(vfColor, coord + vec2(0,-dy)).xy;

    vec2 partial_derivate = (plusy - minusy) / (2.0 * dy);
    return partial_derivate;
}

float passThrough(vec2 coord){
    return texture(vfColor, coord).x;
}

float magnitude( vec2 coord ){
    vec2 texCoord = texture(vfColor, coord).xy;
    //TODO find the magnitude of the vectorfield at the position coords
    return sqrt(texCoord.x * texCoord.x + texCoord.y * texCoord.y);
}

float divergence(vec2 coord){
    //TODO find the divergence of the vectorfield at the position coords
    vec2 pixelSize = vfParameters.reciprocalDimensions;
    vec2 pdx = partialDerivateX(coord, pixelSize);
    vec2 pdy = partialDerivateY(coord, pixelSize);
    return pdx.x + pdy.y;
}

float rotation(vec2 coord){
    //TODO find the curl of the vectorfield at the position coords
    vec2 pixelSize = vfParameters.reciprocalDimensions;
    vec2 pdx = partialDerivateX(coord, pixelSize);
    vec2 pdy = partialDerivateY(coord, pixelSize);
    return pdx.y - pdy.x;
}

void main(void) {
    float v = OUTPUT(texCoord_.xy);
    FragData0 = vec4(v);
}
