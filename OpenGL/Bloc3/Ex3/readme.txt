El único cambio es, en la función createBuffersTerraIParet.
Hay que cambiar las definiciones del material del suelo (valores RGB):
- Luz ambiente: glm::vec3 amb(0,0,1);
- Difusa: glm::vec3 diff(0,0,1); 
- Especular: glm::vec3 spec(0,0,1); 
