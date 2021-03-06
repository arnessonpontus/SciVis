uniform sampler2D vfColor;
uniform sampler2D noiseColor;

uniform int steps;
uniform float stepSize;

in vec3 texCoord_;

/*
* Traverse the vector field and sample the noise image
* @param posF Starting position
* @param stepSize length of each step
* @param steps the number of steps to traverse
* @param v the accumulated value from sampling the noise image
* @param c the number of samples used for v
*/
void traverse(vec2 posF, float stepSize, int steps, inout float v, inout int c){
    // traverse the vectorfield staring at `posF for `steps` with using `stepSize` and sample the noiseColor texture for each position
    // store the accumulated value in v and the amount of samples in c
    
    c += steps;
    vec2 pos = posF;
    vec2 direction;
    for(int i = 0; i < steps; ++i) {
        direction = (texture(vfColor, pos).xy) * stepSize;
        pos += direction;
        v += texture(noiseColor, pos).r;
    }
}

void main(void) {
    float v = texture(noiseColor,texCoord_.xy).r;
    int c = 1;
    traverse(texCoord_.xy, 0.001, 50, v, c);
    traverse(texCoord_.xy, -0.001, 50, v, c);
    v = v/c;
    //traverse the vector field both forward and backwards to calculate the output color

    FragData0 = vec4(v,v,v,1);
}
