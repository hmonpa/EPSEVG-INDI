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
//vec3 colFocus = vec3(0.8, 0.8, 0.8);          // iluminación focus
//vec3 llumAmbient = vec3(0.2, 0.2, 0.2);         // luz ambiente
//vec3 posFocus = vec3(1, 0, 1);                // posición focus en SCA
uniform vec3 llumAmbient;

uniform vec3 colFocus;                          // CONVERSIÓN A UNIFORM
uniform vec3 posFocus;                          // CONVERSIÓN A UNIFORM


// Se envian del VS al FS...
out vec3 matambFrag;                                // ... propiedades de material al FS...
out vec3 matdiffFrag;
out vec3 matspecFrag;
out float matshinFrag;
out mat4 viewMat;

out vec3 fcolor;
out vec4 vertSCO;                                   // ... posiicón del vértex al FS
out vec3 normalSCO;                                 // ... normal del vértex en SCO al FS


void main()
{	
    //fcolor = matdiff;
    //gl_Position = proj * view * TG * vec4 (vertex, 1.0);
    //cridalambert();
    //cridaphong();
    vertSCO = view * TG * vec4(vertex, 1.0);
    mat3 normalMatrix = inverse(transpose (mat3 (view*TG)));
    normalSCO = normalize (normalMatrix * normal);             // Por el momento en el VS lo declararemos así, calcula la matriz para cada vértice, poco eficiente, pero facil
    fcolor = colFocus;

    gl_Position = proj * vertSCO;

    matambFrag = matamb;
    matdiffFrag = matdiff;
    matspecFrag = matspec;
    matshinFrag = matshin;
    viewMat = view;
}


