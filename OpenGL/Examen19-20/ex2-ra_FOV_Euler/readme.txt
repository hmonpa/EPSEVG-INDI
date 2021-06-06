Hay que visualizar la escena en 3a persona con una cámara perspectiva.
La escena no se tiene que recortar ni deformar, y tiene que estar centrada ocupando el máximo del Viewport.

La cámara inicial tiene que tener como ángulos X (theta) e Y (phi):
	- Ángulo X = M_PI/3.0 (60 grados)
	- Ángulo Y = 0

Los nuevos atributos y funciones para este ejercicio son:
    float FOV, FOV_orig, ra;
    glm::vec3 centreEsc;

    void calculaRadiEsc();	// No es necesaria pero más eficiente que calcular el radio a ojo

La variable centreEsc podemos calcularla a ojo, ya que queremos que en los ejes X y Z el centro sea 0.
Y sabemos que la altura máxima de la escena es la de los modelos Legoman, que tienen una altura = 3.
Por tanto, el centreEsc = (0, 1.5, 0).

Funciones a modificar:
- iniCamera():
Tenemos dos variables para calcular el FOV (ángulo abertura).
Si la relación de aspecto (ra) es menor a 1, la fórmula es:
	FOV = 2.0 * atan(tan(0.5*FOV_orig/ra));

Si no es menor a 1:
	FOV coge el valor de FOV_orig, el cual es:
		FOV_orig = 2.0 * asin (radiEsc / (radiEsc*2.0));

- resizeGL():
Cálculo automático de la relación de aspecto y llamada a projectTransform().

- projectTransform():
Se vuelve a comprobar la relación de aspecto y se calcula el FOV:
    if (ra < 1.0) FOV = 2.0 * atan(tan(0.5*FOV_orig/ra));
    else FOV = FOV_orig;

y la cámara es perspectiva:
	Proj = glm::perspective(FOV, ra, radiEsc, 3.0f*radiEsc);

- viewTransform():
Se tralada la cámara al centro de la escena:
	View = glm::translate(View, glm::vec3(-centreEsc));
