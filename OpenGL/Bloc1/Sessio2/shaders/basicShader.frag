#version 330 core

out vec4 FragColor;
in vec4 varColor;
void main()
{

    // Exercici 1 i 3 (Per al 3, s'ha canviat el fitxer .vert)
    //FragColor = vec4(1.);
    /*if(gl_FragCoord.y<370 && gl_FragCoord.x<354)
      FragColor= vec4(1.,1.,0.,1.);             // Groc
    else if(gl_FragCoord.y<370 && gl_FragCoord.x>=354)
      FragColor= vec4(0.,1.,0.,1.);             // Verd
    else if(gl_FragCoord.y>=370 && gl_FragCoord.x>=354)
      FragColor= vec4(0.,0.,1.,0.);             // Blau
     else if(gl_FragCoord.y>=370 && gl_FragCoord.x<354)
      FragColor= vec4(1.,0.,0.,1.);             // Vermell
    */

    // Exercici 2
    //FragColor = vec4(1.);
    /*if((int(gl_FragCoord.y) % 24) <= 12){
        if(gl_FragCoord.y<370 && gl_FragCoord.x<354)
            FragColor= vec4(1.,1.,0.,1.);             // Groc
        else if(gl_FragCoord.y<370 && gl_FragCoord.x>=354)
            FragColor= vec4(0.,1.,0.,1.);             // Verd
        else if(gl_FragCoord.y>=370 && gl_FragCoord.x>=354)
            FragColor= vec4(0.,0.,1.,0.);             // Blau
         else if(gl_FragCoord.y>=370 && gl_FragCoord.x<354)
            FragColor= vec4(1.,0.,0.,1.);             // Vermell
    }
    else FragColor= vec4(0.5, 0.7, 1.0, 1.0);*/

    // Exercici 4
    FragColor=varColor;
}


