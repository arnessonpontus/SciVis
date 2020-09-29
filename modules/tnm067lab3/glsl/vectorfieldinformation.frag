#include "utils/structs.glsl"

uniform sampler2D vfColor;
uniform ImageParameters vfParameters;
in vec3 texCoord_;

vec2 partialDerivateX(vec2 coord, vec2 pixelSize){
    float dx = pixelSize.x;
    vec2 plusx = coord.xy + vec2(dx,0);
    vec2 minusx = coord.xy + vec2(-dx,0);

    vec2 partial_derivate = (plusx + minusx) / (2.0 * dx);
    return partial_derivate;
}

vec2 partialDerivateY(vec2 coord, vec2 pixelSize){
    float dy = pixelSize.y;
    vec2 plusy = coord.xy + vec2(0,dy);
    vec2 minusy = coord.xy + vec2(0,-dy);

    vec2 partial_derivate = (plusy - minusy) / (2.0 * dy);
    return partial_derivate;
}

float passThrough(vec2 coord){
    return texture(vfColor,coord).x;
}

float magnitude( vec2 coord ){
    //TODO find the magnitude of the vectorfield at the position coords
    return sqrt(coord.x * coord.x + coord.y * coord.y);
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
    //float v = OUTPUT(texCoord_.xy);
    vec2 coord = texture2D(vfColor, texCoord_.xy).xy;
    float m = magnitude(coord);
    float d = divergence(coord);
    float r = rotation(coord);
    FragData0 = (vec4(d) + vec4(r)) + vec4(m);
}
