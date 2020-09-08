out mat2 vJ;

void main() {
    vJ = mat2(in_Color);

    vec2 pos = in_Vertex.xy;
    pos *= 2;
    pos -= 1;
    
    gl_Position = vec4(pos,0,1);       
}
