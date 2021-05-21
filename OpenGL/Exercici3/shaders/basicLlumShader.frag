#version 330 core
//in vec3 color;
out vec4 FragColor1;
out vec4 FragColor2;

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

//vec3 posLlumReactor = vec3(-1.9, 3.60, -5.65);
//vec3 colorLlumReactor = vec3(1.0,0.2,0.0);

// Propiedades material
in vec3 matambFrag;
in vec3 matdiffFrag;
in vec3 matspecFrag;
in float matshinFrag;

in mat4 viewMat;

// Transferencia funciones Lambert y Phong con adaptación a las propiedades de material

// -----------------------------------------------------
// ---------------- FOCUS DE LLUM ----------------------
// -----------------------------------------------------
vec3 LambertFocus (vec3 NormSCO, vec3 L)
{
    // S'assumeix que els vectors que es reben com a paràmetres estan normalitzats

    // Inicialitzem color a component ambient
    vec3 colRes = llumAmbient * matambFrag;

    // Afegim component difusa, si n'hi ha
    if (dot (L, NormSCO) > 0)
      colRes = colRes + colFocus * matdiffFrag * dot (L, NormSCO);
    return (colRes);
}

vec3 PhongFocus (vec3 NormSCO, vec3 L, vec4 vertSCO)
{
    // Els vectors estan normalitzats

    // Inicialitzem color a Lambert
    vec3 colRes = LambertFocus (NormSCO, L);

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



// -----------------------------------------------------
// ----------------- REACTOR AVIÓ ----------------------
// -----------------------------------------------------

vec3 LambertReactor (vec3 NormSCO, vec3 L)
{
    // S'assumeix que els vectors que es reben com a paràmetres estan normalitzats

    // Inicialitzem color a component ambient
    vec3 colRes = llumAmbient * matambFrag;

    // Afegim component difusa, si n'hi ha
    if (dot (L, NormSCO) > 0)
      colRes = colRes + colorLlumReactor * matdiffFrag * dot (L, NormSCO);
    return (colRes);
}

vec3 PhongReactor (vec3 NormSCO, vec3 L, vec4 vertSCO)
{
    // Els vectors estan normalitzats

    // Inicialitzem color a Lambert
    vec3 colRes = LambertReactor (NormSCO, L);

    // Calculem R i V
    if ((dot(NormSCO,L) < 0) || (matshinFrag == 0))
      return colRes;  // no hi ha component especular

    vec3 R = reflect(-L, NormSCO); // equival a: 2.0*dot(NormSCO,L)*NormSCO - L;
    vec3 V = normalize(-vertSCO.xyz); // perquè la càmera està a (0,0,0) en SCO

    if (dot(R, V) < 0)
      return colRes;  // no hi ha component especular

    float shine = pow(max(0.0, dot(R, V)), matshinFrag);
    return (colRes + matspecFrag * colorLlumReactor * shine);
}


void main()
{
        // FOCUS
        vec4 posFocusFrag1 = viewMat * vec4(posFocus, 1.0);
        vec4 posFocusFrag2 = viewMat * vec4(posFocus2, 1.0);
        vec4 posFocusFrag3 = viewMat * vec4(posFocus3, 1.0);
        vec3 L1 = normalize(posFocusFrag1.xyz - vertSCO.xyz);
        vec3 L2 = normalize(posFocusFrag2.xyz - vertSCO.xyz);
        vec3 L3 = normalize(posFocusFrag3.xyz - vertSCO.xyz);
        vec3 fragmentColor1 = PhongFocus(normalSCO, L1, vertSCO);
        vec3 fragmentColor2 = PhongFocus(normalSCO, L2, vertSCO);
        vec3 fragmentColor3 = PhongFocus(normalSCO, L3, vertSCO);

        FragColor1 = vec4(fragmentColor1,1);
        FragColor1 += vec4(fragmentColor2,1);
        FragColor1 += vec4(fragmentColor3,1);

        // AVIÓ
        vec4 posLlumReacFrag = viewMat * vec4(posLlumReactor,1.0);
        vec3 L4 = normalize(posLlumReacFrag.xyz - vertSCO.xyz);
        vec3 fragmentColor4 = PhongReactor(normalSCO, L4, vertSCO);

        FragColor1 += vec4(fragmentColor4,1);
}

