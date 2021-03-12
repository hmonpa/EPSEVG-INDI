#version 330 core

out vec4 FragColor;
in vec4 vertex_color;
uniform float transparencia;

void main() {

      FragColor = vertex_color;

}

