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

uniform int pintaVaca;
uniform int cambiaCamara;

// Hay que crear componentes de material auxiliares, ya que son solo de lectura
vec3 matambF = matamb;
vec3 matdiffF = matdiff;
vec3 matspecF = matspec;
float matshinF = matshin;

// Valors per als components que necessitem dels focus de llum
vec3 colFocus = vec3(0.0, 0.8, 0.8);        // Cyan
vec3 llumAmbient = vec3(0.2, 0.2, 0.2);
vec3 posFocus = vec3(1, 1, 1);

vec3 colFocusF = colFocus;
vec3 posFocusF = posFocus;

out vec3 fcolor;

vec3 Lambert (vec3 NormSCO, vec3 L)
{
    // S'assumeix que els vectors que es reben com a paràmetres estan normalitzats

    // Inicialitzem color a component ambient
    vec3 colRes = llumAmbient * matambF;

    // Afegim component difusa, si n'hi ha
    if (dot (L, NormSCO) > 0)
      colRes = colRes + colFocus * matdiffF * dot (L, NormSCO);
    return (colRes);
}

vec3 Phong (vec3 NormSCO, vec3 L, vec4 vertSCO)
{
    // Els vectors estan normalitzats

    // Inicialitzem color a Lambert
    vec3 colRes = Lambert (NormSCO, L);

    // Calculem R i V
    if (dot(NormSCO,L) < 0)
      return colRes;  // no hi ha component especular

    vec3 R = reflect(-L, NormSCO); // equival a: normalize (2.0*dot(NormSCO,L)*NormSCO - L);
    vec3 V = normalize(-vertSCO.xyz);

    if ((dot(R, V) < 0) || (matshinF == 0))
      return colRes;  // no hi ha component especular

    // Afegim la component especular
    float shine = pow(max(0.0, dot(R, V)), matshinF);
    return (colRes + matspecF * colFocus * shine);
}

void main()
{
    // Pintar vaca gris brillante
    if (pintaVaca == 1)
    {
        matspecF = vec3(1,1,1);
        matdiffF = vec3(0.3, 0.3, 0.3);
        matambF = vec3(0.3, 0.3, 0.3);
        //matshinF = 100;
    }

    vec4 vertSCO = view * TG * vec4(vertex, 1.0);
    mat3 normalMatrix = inverse(transpose(mat3 (view*TG)));
    vec3 normalSCO = normalize(normalMatrix*normal);
    //vec4 focusSCO = vec4(posFocus, 1.0);
    //vec3 L = normalize(focusSCO.xyz - vertSCO.xyz);

    vec4 focusSCO;
    if (cambiaCamara == 1)
    {
        posFocusF = vec3(1,1,1);
        colFocusF = vec3(0.8, 0.8, 0.8);
        focusSCO = view * vec4(posFocusF, 1.0);
    }
    else focusSCO = vec4(posFocusF, 1.0);

    vec3 L = normalize(focusSCO.xyz - vertSCO.xyz);
    fcolor = Phong(normalSCO, L, vertSCO);
    gl_Position = proj * vertSCO;
}
