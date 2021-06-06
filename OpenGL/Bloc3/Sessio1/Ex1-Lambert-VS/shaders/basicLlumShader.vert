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
vec3 colFocus = vec3(0.8, 0.8, 0.8);        // iluminación focus
vec3 llumAmbient = vec3(0.2, 0.2, 0.2);     // luz ambiente
vec3 posFocus = vec3(1, 1, 1);              // posición focus en SCA

out vec3 fcolor;

vec3 Lambert (vec3 NormSCO, vec3 L) 
{
    // S'assumeix que els vectors que es reben com a paràmetres estan normalitzats

    // Inicialitzem color a component ambient
    vec3 colRes = llumAmbient * matamb;

    // Afegim component difusa, si n'hi ha
    if (dot (L, NormSCO) > 0)
      colRes = colRes + colFocus * matdiff * dot (L, NormSCO);
    return (colRes);
}

vec3 Phong (vec3 NormSCO, vec3 L, vec4 vertSCO) 
{
    // Els vectors estan normalitzats

    // Inicialitzem color a Lambert
    vec3 colRes = Lambert (NormSCO, L);

    // Calculem R i V
    if ((dot(NormSCO,L) < 0) || (matshin == 0))
      return colRes;  // no hi ha component especular

    vec3 R = reflect(-L, NormSCO); // equival a: 2.0*dot(NormSCO,L)*NormSCO - L;
    vec3 V = normalize(-vertSCO.xyz); // perquè la càmera està a (0,0,0) en SCO

    if (dot(R, V) < 0)
      return colRes;  // no hi ha component especular
    
    float shine = pow(max(0.0, dot(R, V)), matshin);
    return (colRes + matspec * colFocus * shine); 
}

// Hay que pasar a coordenadas de observador (SCO)
void cridalambert(){    // Lambert - model empiric difus, hay que enviar los vectores a lambert normalizados.
    vec4 vertSCO = view * TG * vec4(vertex, 1.0);
    mat3 normalMatrix = inverse(transpose (mat3 (view*TG)));
    vec3 normalSCO = normalize (normalMatrix * normal);             // Por el momento en el VS lo declararemos así, calcula la matriz para cada vértice, poco eficiente, pero facil
    vec4 focusSCO = view * vec4(posFocus, 1.0);
    vec3 L = normalize (focusSCO.xyz - vertSCO.xyz);
    fcolor = Lambert(normalSCO, L);
    gl_Position = proj * vertSCO;
}

void main()
{	
    //fcolor = matdiff;
    //gl_Position = proj * view * TG * vec4 (vertex, 1.0);
    cridalambert();
}


