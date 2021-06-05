#version 330 core

in vec3 fcolor;
out vec4 FragColor;

uniform int pintaFranjas;

void main()
{	
    if (pintaFranjas == 1)
    {
        int n = int(gl_FragCoord.y)/10;
        if (n % 2 == 0)
        {
            FragColor = vec4(1., 1., 1., 1.);
        }
        else
        {
            FragColor = vec4(0., 0., 0., 0.);
        }
    }
    else FragColor = vec4(fcolor,1);
}
