Importante: Tipos de cámara
-----------------------------
- SC modelo: 
	focus = view * TG * vec4(posFocus, 1.0)

- SC escena:
	focus = view * vec4(posFocus, 1.0)

- SC OBS (de cámara):
	focus = vec4(posFocus, 1.0)

Sólo se modifican VS y FS:

En el VS:
-----------------------------
En el VS hay que declarar las variables de material que saldrán hacia el FS.
Es decir: matambFrag, matdiffFrag, matspecFrag y matshinFrag (saldrán todas como out)

Estas han de asignar el valor que reciben desde el código cpp, es decir:
    matambFrag = matamb;
    matdiffFrag = matdiff;
    matspecFrag = matspec;
    matshinFrag = matshin;

También hay que declarar vertSCO, normalMatrix (saldrán como out) y normalizarla, para pasarselo al FS:
    vertSCO = (view * TG * vec4(vertex, 1.0)).xyz;
    mat3 normalMatrix = inverse(transpose(mat3(view*TG)));
    normalSCO = normalize(normalMatrix*normal);

Traspasar las funciones de Lambert y Phong al FS.

En el FS:
------------------------------

Recibe vertSCO, normalSCO, y todas las propiedas de material (recibidas como in).

Se declaran colFocus, llumAmbient y posFocus, también se podrían pasar como uniforms desde el .cpp si se requiere, pasando por el VS también,
pero llevaría más rato.

En las funciones de Lambert y Phong habria que adaptar las propiedades de material a las recibidas del VS (matXXXFrag).

En el main, hay que declarar posFocusFrag, L y fragmentColor, que hace una llamada a la función Phong.
