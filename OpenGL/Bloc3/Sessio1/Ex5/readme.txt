Los cambios en este ejercicio se producen en el VS y en los programas:
- VS: 
Hay que comentar las declaraciones de los actuales colFocus y posFocus
y convertirlos en declaraciones de uniform:
	uniform vec3 colFocus;
	uniform vec3 posFocus;

En el .h, habrá que declarar las variables que usaremos como uniforms desde el .cpp
y los identificadores para dichos uniforms:

    GLuint colFocusLoc, posFocusLoc;
    glm::vec3 colFocus, posFocus;

En el .cpp, por ejemplo en la función iniEscena(), llamada desde initializeGL(),
vamos a declarar los uniforms y a encadenarlos con sus identificadores:

  colFocus = glm::vec3(0.8, 0.8, 0.8);      // colFocus
  posFocus = glm::vec3(1, 0, 1);            // posFocus
  glUniform3fv(colFocusLoc, 1, &colFocus[0]);
  glUniform3fv(posFocusLoc, 1, &posFocus[0]);

Y por último, en carregaShaders(), hay que enviar los uniforms al VS:

  colFocusLoc = glGetUniformLocation (program->programId(), "colFocus");
  posFocusLoc = glGetUniformLocation (program->programId(), "posFocus");
