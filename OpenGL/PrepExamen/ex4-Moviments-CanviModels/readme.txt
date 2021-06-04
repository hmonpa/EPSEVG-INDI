Hay que modificar en primer lugar la función KeyPressEvent, para hacer mover la pelota en las direcciones pedidas.
Para que cuando la pelota toque a uno de los legoman, este se cambie por el modelo legoman-assegut.obj,
se ha duplicado el VAO para que cada modelo utilice uno, es decir, VAO_Lego y VAO_Lego2.

Se ha liberado un cout para poder ver por pantalla las zonas delicadas donde la pelota toca a cada legoman.
Cuando sobre el papel tenemos las coordenadas conflictivas, se ha creado una función comprovaLego(), en la cual si la pelota está en ciertas coordenadas,
activa un booleano legoAss1 o legoAss2 (en función del VAO) y llama a la función creaBuffersLego().

En esta función se ha añadido una condición para cada estado, de cada booleano.
NOTA: No acaba de funcionar muy bien, pero por el momento funciona parcialmente.

Por último, para volver a la posición inicial, se puede hacer con la tecla "i", o con un botón añadido a MyForm.ui.
También se ha añadido un signal, pese a que quizás no seria necesario.

