#version 330 core
//in vec3 color;
out vec4 FragColor;

// Nuevos atributos pasados al FS
in vec4 vertSCO;
in vec3 normalSCO;

uniform vec3 posFocus;
uniform vec3 colFocus;
uniform vec3 posFocus2;
uniform vec3 posFocus3;

uniform vec3 llumAmbient;

uniform vec3 posLlumReactor;
uniform vec3 colorLlumReactor;

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
{
        vec4 posFocusFrag1 = viewMat * vec4(posFocus, 1.0);
        vec4 posFocusFrag2 = viewMat * vec4(posFocus2, 1.0);
        vec4 posFocusFrag3 = viewMat * vec4(posFocus3, 1.0);
        vec3 L1 = normalize(posFocusFrag1.xyz - vertSCO.xyz);
        vec3 L2 = normalize(posFocusFrag2.xyz - vertSCO.xyz);
        vec3 L3 = normalize(posFocusFrag3.xyz - vertSCO.xyz);
        vec3 fragmentColor1 = Phong(normalSCO, L1, vertSCO);
        vec3 fragmentColor2 = Phong(normalSCO, L2, vertSCO);
        vec3 fragmentColor3 = Phong(normalSCO, L3, vertSCO);

        vec4 posLlumReacFrag = viewMat * vec4(posLlumReactor,1.0);
        vec3 L4 = normalize(posLlumReacFrag.xyz - vertSCO.xyz);
        vec3 fragmentColor4 = Phong(normalSCO, L4, vertSCO);

        FragColor = vec4(fragmentColor1,1);
        FragColor += vec4(fragmentColor2,1);
        FragColor += vec4(fragmentColor3,1);
        FragColor += vec4(fragmentColor4,1);
}

