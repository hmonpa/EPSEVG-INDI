#version 330 core

out vec4 FragColor;
in vec4 varColor;
uniform float val;
void main()
{
    FragColor=varColor;
}


