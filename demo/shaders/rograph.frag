#version 430 core
#pragma debug(on)

uniform sampler2D tex;
uniform float opacity;

in vec2 tvTex;

out vec4 fColor;

void main() {
    vec4 texel = texture(tex, tvTex.st);
    texel.a = opacity * texel.a;
    fColor = texel;
    //fColor = vec4(1.0, 1.0, 1.0, 1.0);
}
