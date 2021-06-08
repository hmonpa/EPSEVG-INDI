#version 330 core

in vec3 vertex;
in vec3 normal;

in vec3 matamb;
in vec3 matdiff;
in vec3 matspec;
in float matshin;

uniform mat4 proj;
uniform mat4 view;
uniform mat4 TG;
uniform vec3 posFocus;

//out vec3 fcolor;
out vec3 matambFrag;
out vec3 matdiffFrag;
out vec3 matspecFrag;
out float matshinFrag;

out vec3 vertSCO;
out vec3 normalSCO;


void main()
{	
    vertSCO = (view * TG * vec4(vertex, 1.0)).xyz;
    mat3 normalMatrix = inverse(transpose(mat3(view*TG)));
    normalSCO = normalize(normalMatrix*normal);

    matambFrag = matamb;
    matdiffFrag = matdiff;
    matspecFrag = matspec;
    matshinFrag = matshin;

    gl_Position = proj * view * TG * vec4 (vertex, 1.0);
}
