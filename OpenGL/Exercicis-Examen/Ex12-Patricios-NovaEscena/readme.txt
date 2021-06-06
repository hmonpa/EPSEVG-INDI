Funciones modelTransformPatricio y modelTransformPatricio2.

Se colocan en el centro de su caja, y después se rotan, escalan y colocan en la posición indicada.
Para diferenciar las escalas de los dos modelos, y para no tener que copiar entero el createBuffersPatricio,
se ha implementado una función escalarModel, a la que le paso el modelo y el factor de escala desde el createBuffers.
La función simplemente hace lo mismo que calculaCapsaModel, pero sólo para minY y maxY.
Devuelve factor de escala / (maxy-miny).

En cuanto a la escena, se indica correctamente FOV, FOV_orig y ra.
También se indica el angleX para poder girar alrededor del eje X con el ratón.

También verificamos Pmin y Pmax para los tres ejes (en función de los modelos y del suelo):
Pmin.x = -2	-	Pmin.y: -1	- Pmin.z = -2
Pmax.y = 2	-	Pmax.y: 2.5	- Pmax.z = 2
