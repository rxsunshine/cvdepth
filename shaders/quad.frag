#version 400 core

in vec2 pass_coord;

out vec4 out_color;

uniform sampler2D u_image1;
uniform sampler2D u_image2;
uniform vec2 u_res;

void main() {
    vec3 col1 = texture(u_image1, vec2(pass_coord.x, 1.0 - pass_coord.y)).rgb;

    float sum = 0.0;
    vec2 pos = vec2(0.0);
    for (float a = -128.0; a < 128.0; a += 1.0) {
        for (float b = -128.0; b < 128.0; b += 1.0) {
            vec2 disp = vec2(a, b) / u_res;
            vec3 col2 = texture(u_image2, vec2(pass_coord.x, 1.0 - pass_coord.y) + disp).rgb;

            float fac = 1.0 / (dot(abs(col2 - col1), vec3(1.0)) + 0.0001);
            sum += fac;
            pos += disp * fac;
        }
    }
    pos /= sum;

    out_color = vec4(vec3(length(pos)), 1.0);
}