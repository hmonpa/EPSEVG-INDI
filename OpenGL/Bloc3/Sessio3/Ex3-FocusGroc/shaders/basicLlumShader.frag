#version 330 core

//in vec3 fcolor;
out vec4 FragColor;

// Nuevos atributos pasados al FS
in vec4 vertSCO;
in vec3 normalSCO;

uniform vec3 posFocus;
uniform vec3 colFocus;
//vec3 llumAmbient = vec3(0.2, 0.2, 0.2);         // luz ambiente
uniform vec3 llumAmbient;

// Propiedades material
in vec3 matambFrag;
in vec3 matdiffFrag;
in vec3 matspecFrag;
in float matshinFrag;

in mat4 viewMat;

// Transferencia funciones Lambert y Phong con adaptación a las propiedades de material

vec3 Lambert (vec3 NormSCO, vec3 L)
{
    // S'assumeix que els vectors que es reben com a paràmetres estan normalitzats

    // Inicialitzem color a component ambient
    vec3 colRes = llumAmbient * matambFrag;

    // Afegim component difusa, si n'hi ha
    if (dot (L, NormSCO) > 0)
      colRes = colRes + colFocus * matdiffFrag * dot (L, NormSCO);
    return (colRes);
}

vec3 Phong (vec3 NormSCO, vec3 L, vec4 vertSCO)
{
    // Els vectors estan normalitzats

    // Inicialitzem color a Lambert
    vec3 colRes = Lambert (NormSCO, L);

    // Calculem R i V
    if ((dot(NormSCO,L) < 0) || (matshinFrag == 0))
      return colRes;  // no hi ha component especular

    vec3 R = reflect(-L, NormSCO); // equival a: 2.0*dot(NormSCO,L)*NormSCO - L;
    vec3 V = normalize(-vertSCO.xyz); // perquè la càmera està a (0,0,0) en SCO

    if (dot(R, V) < 0)
      return colRes;  // no hi ha component especular

    float shine = pow(max(0.0, dot(R, V)), matshinFrag);
    return (colRes + matspecFrag * colFocus * shine);
}

void main()
{	vec4 posFocusFrag = viewMat * vec4(posFocus, 1.0);
        vec3 L = normalize(posFocusFrag.xyz - vertSCO.xyz);
        vec3 fragmentColor = Phong(normalSCO, L, vertSCO);
        FragColor = vec4(fragmentColor,1);
}
