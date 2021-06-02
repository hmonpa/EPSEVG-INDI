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

// Valors per als components que necessitem dels focus de llum
/*vec3 colFocus = vec3(0.8, 0.8, 0.8);
vec3 llumAmbient = vec3(0.2, 0.2, 0.2);
vec3 posFocus = vec3(1, 1, 1);*/

//out vec3 fcolor;
out vec3 matambFrag;
out vec3 matdiffFrag;
out vec3 matspecFrag;
out float matshinFrag;

out vec3 vertSCO;
out vec3 normalSCO;

void main()
{	
    //fcolor = matdiff;
    // Añadidos

    vertSCO = (view * TG * vec4(vertex, 1.0)).xyz;
    mat3 normalMatrix = inverse(transpose(mat3(view*TG)));
    normalSCO = normalize(normalMatrix*normal);

    matambFrag = matamb;
    matdiffFrag = matdiff;
    matspecFrag = matspec;
    matshinFrag = matshin;

    gl_Position = proj * view * TG * vec4 (vertex, 1.0);
}
