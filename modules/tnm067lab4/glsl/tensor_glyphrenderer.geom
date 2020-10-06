layout(points) in;
layout(triangle_strip, max_vertices = 96) out;

uniform float radius;

in mat2 vJ[1];

out vec3 color;

const float pi = 3.14159265359;
const float twoPi = 6.28318530718;

void main(void) {
    vec2 center = gl_in[0].gl_Position.xy;

    mat2 J = vJ[0];
    mat2 Jsym = 0.5*(J + transpose(J));

    for (int i = 0; i < 32; i++) {
        if (i < 4 || i > 27) color = vec3(0.851, 0.373, 0.008);
        else if (i < 12) color = vec3(0.106, 0.620, 0.467);
        else if (i < 20) color = vec3(0.906,0.61,0.541);
        else color = vec3(0.459,0.439,0.702);
        
        float a1 = twoPi * (i / 32.0);        // angle from y-axis of second point in the triangle
        float a2 = twoPi * ((i + 1) / 32.0);  // angle from y-axis of third point in the triangle

        gl_Position = vec4(center, 0, 1);  // first point in triangle is in the origin of the circle
        EmitVertex();

        vec2 o1 = Jsym * vec2(cos(a1), sin(a1));  // calculate an offset vector for the second point in the triangle based on a1

        gl_Position = vec4(center + o1 * radius, 0, 1);
        EmitVertex();

        vec2 o2 = Jsym * vec2(cos(a2), sin(a2));  // calculate an offset vector for the second point in the triangle based on a2

        gl_Position = vec4(center + o2 * radius, 0, 1);
        EmitVertex();

        EndPrimitive();
    }
}
