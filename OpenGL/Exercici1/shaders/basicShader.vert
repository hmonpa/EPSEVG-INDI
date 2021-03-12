#version 330 core

in vec3 vertex;
in vec3 color;
uniform mat4 transform;
uniform mat4 transformGronx;
out vec4 vertex_color;

void main()  {
    //gl_Position = transformGronx * vec4 (vertex, 1.0);
    gl_Position = transform * vec4 (vertex, 1.0);
    vertex_color = vec4 (color,1.0);

}
