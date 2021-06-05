Hay que añadir un nuevo uniform integer, el cual cuando se puse la tecla X, cambie a 1 o 0 en función de su valor actual.
Este uniform lo recibe el FS, dónde, si su valor es 1, pinta franjas blancas o negras en la escena en función de la altura.
La altura se sabe con la coordenada gl_FragCoord.y.
