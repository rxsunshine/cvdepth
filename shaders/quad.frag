#version 400 core

in vec2 pass_coord;

out vec4 out_color;

uniform sampler2D u_image;

void main() {
    vec3 col = texture(u_image, vec2(pass_coord.x, 1.0 - pass_coord.y)).bgr;
    out_color = vec4(col, 1.0);
}