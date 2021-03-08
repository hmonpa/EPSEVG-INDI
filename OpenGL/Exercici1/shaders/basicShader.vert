#version 330 core

in vec3 vertex;
in vec3 color;

out vec4 vertex_color;

void main()  {
    gl_Position = vec4 (vertex, 1.0);
    vertex_color = vec4 (color,1.0);
}
