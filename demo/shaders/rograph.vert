#version 430 core
#pragma debug(on)

in vec4 ivPos;
in vec2 ivTex;

uniform mat4 backZero;
uniform mat4 rotate;
uniform mat4 translate;

uniform mat4 ortho;

out vec2 tvTex;

void main(){
    gl_Position = ortho * translate * rotate * backZero * ivPos; // ortho * translate * rotate * backZero * 
    tvTex = ivTex;
}
