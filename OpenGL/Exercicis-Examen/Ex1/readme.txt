// Nuevas funciones y atributos
1. void modelTransformPatricio2();

	- Se rota Patricio2, dandole la vuelta (180 grados eje Z)
	- Se mueve al punto (0,2,0): El patricio de debajo tiene tamaño 2 y esta sobre el 0,0,0

2. glm::vec3 centreEsc
3. void calculaRadiEsc();
	- Se calcula el centreEsc y el radio.
	Para calcular centreEsc hay que tener en cuenta (mirando createBuffersTerraIParet),
	que el suelo tiene vértices en el -1, y el patricio de debajo (posado sobre el suelo) tiene una altura de 2.
	Por tanto, con el segundo Patricio añadido, Pmin.y es -1 y Pmax.y es 3
	De esta forma, la escena estará en el centro de la escena.

4. float angleX;
	Usado para mover la escena en el eje X.

5. float ra, FOV, FOV_orig;
	Usados para que la escena no se recorte ni deforme, en caso de redimensionamiento del Viewport.

