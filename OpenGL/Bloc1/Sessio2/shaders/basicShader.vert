#version 330 core

in vec3 vertex;
in vec3 col;
out vec4 varColor;

void main()  {
    gl_Position = vec4 (vertex*0.5, 1.0);
    varColor=vec4(col, 1.0);
}
