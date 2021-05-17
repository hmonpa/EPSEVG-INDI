Se cambia el código de la función cridaPhong() en el VS.
Únicamente para que, en la declaración de focusSCO, simplemente reciba la información del vector posFocus y no se multiplique por viewMatrix como hacia hasta ahora:
	vec4 focusSCO = vec4(posFocus, 1.0);
