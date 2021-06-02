Hay que añadir dos modelos Legoman situados en (-7,0,3) y (-7,0,-3) respectivamente.
Ambos de altura 3 (escalado) y con orientación hacia el eje X+.

También hay que crear tres paredes, en base al modelo cubo calculado por defecto.
Las paredes tienen las siguientes dimensiones y centros, respectivamente:
	- Pared1:
		Tamaño: 20x2x0.2
		Centro: 0, 0, -6.9

	- Pared2:
		Tamaño: 20x2x0.2
		Centro: 0, 0, 6.9

	- Pared3:
		Tamaño: 0.2x2x14
		Centro: -9.9, 0, 0

Por último hay que modificar la posición inicial de una pelota y situarla en la posición (9, 0, 0).

Las funciones a crear y modificar en este ejercicio son:
	void creaBuffersLego();
    	void modelTransformLego1();
    	void modelTransformLego2();
    	void modelTransformParet1();
    	void modelTransformParet2();
    	void modelTransformParet3();
	void calculaCapsaModel();		// Se modifica la escala para que sea 3
