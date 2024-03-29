#include "MyGLWidget.h"

#include <iostream>

MyGLWidget::MyGLWidget (QWidget* parent) : QOpenGLWidget(parent), program(NULL)
{
  setFocusPolicy(Qt::StrongFocus);  // per rebre events de teclat
  scale = 1.0f;
}

MyGLWidget::~MyGLWidget ()
{
  if (program != NULL)
    delete program;
}

void MyGLWidget::initializeGL ()
{
  // Cal inicialitzar l'ús de les funcions d'OpenGL
  initializeOpenGLFunctions();  

  glClearColor(0.5, 0.7, 1.0, 1.0); // defineix color de fons (d'esborrat)

  // Activació Z-Buffer
  glEnable(GL_DEPTH_TEST);

  carregaShaders();
  creaBuffers();

  raV = 1.0;

}

void MyGLWidget::paintGL () 
{
// Aquest codi és necessari únicament per a MACs amb pantalla retina.
#ifdef __APPLE__
  GLint vp[4];
  glGetIntegerv (GL_VIEWPORT, vp);
  ample = vp[2];
  alt = vp[3];
#endif

// En cas de voler canviar els paràmetres del viewport, descomenteu la crida següent i
// useu els paràmetres que considereu (els que hi ha són els de per defecte)
//  glViewport (0, 0, ample, alt);
  
  // Esborrem el frame-buffer a la vegada que el buffer de profunditats
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

  // Carreguem la transformació de model
  projectTransform();
  viewTransform();
  modelTransform ();
  ini_camera();

  // Activem el VAO per a pintar el Homer
  glBindVertexArray (VAO_homer);
  // pintem
  glDrawArrays(GL_TRIANGLES, 0, homer.faces().size() * 3);


  modelTransformTerra();
  // Activación VAO para pintar tierra
  glBindVertexArray(VAO_terra);
  // Pintado
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

  glBindVertexArray (0);
}

void MyGLWidget::modelTransform () 
{
  // Matriu de transformació de model
  glm::mat4 transform (1.0f);
  transform = glm::scale(transform, glm::vec3(scale));
  transform = glm::rotate(transform, rota, glm::vec3(0,1,0));
  glUniformMatrix4fv(transLoc, 1, GL_FALSE, &transform[0][0]);
}

void MyGLWidget::modelTransformTerra()
{
    // Segona transformació per a que es dibuxi el terra (només matriu identitat)
      glm::mat4 transform (1.0f);
      transform = glm::translate(transform, glm::vec3(0,0,0));
      //transform = glm::rotate(transform, rota, glm::vec3(0,1,0));
      glUniformMatrix4fv(transLoc, 1, GL_FALSE, &transform[0][0]);
}

void MyGLWidget::resizeGL (int w, int h) 
{
  ample = w;
  alt = h;

  raV = double(ample)/double(alt);
  ra = raV;

  if (raV < 1.0)
  {
      FOV = 2.0 * atan(tan(float((M_PI))/4.0)/raV);
  }
  projectTransform();

}

void MyGLWidget::ini_camera()
{
    //FOV = M_PI/2.0;

    //raV = 1.0;
    //zN = 0.4;
    //zF = 3.0;


    dist = 2 * radi;
    FOV = 2 * asin(radi / dist);
    ra = 1.0;
    zN = radi;
    zF = dist + radi;

    VRP = glm::vec3(0,0,0);
    OBS = VRP + dist * glm::vec3(0.f, 0.f, 1.f);
    UP = glm::vec3(0,1,0);


    projectTransform();
    viewTransform();
}

void MyGLWidget::keyPressEvent(QKeyEvent* event) 
{
  makeCurrent();
  switch (event->key()) {
    case Qt::Key_S: { // escalar a més gran
      scale += 0.05;
      //modelTransform();
      break;
    }
    case Qt::Key_D: { // escalar a més petit
      scale -= 0.05;
      //modelTransform();
      break;
    }
    case Qt::Key_R: {
      rota += float(M_PI/4);
      break;
    }
    default: event->ignore(); break;
  }
  update();
}

void MyGLWidget::creaBuffers () 
{
  // Càrrega del objecte Model
  homer.load("/home/hector/Escritorio/INDI/OpenGL/Bloc2/models/HomerProves.obj");
  calculaCapsaCont();

  // Creació del Vertex Array Object per pintar
  glGenVertexArrays(1, &VAO_homer);
  glBindVertexArray(VAO_homer);

  // Construcció de dos VBOs
  // 1 - a partir del mètode VBO_vertices()
  glGenBuffers(1, &VBO_homer);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_homer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*homer.faces().size()*3*3, homer.VBO_vertices(), GL_STATIC_DRAW);

  // Activem l'atribut vertexLoc
  glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(vertexLoc);

  // 2 - a partir del mètode VBO_matdiff()
  glGenBuffers(1, &VBO_homer2);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_homer2);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*homer.faces().size()*3*3, homer.VBO_matdiff(), GL_STATIC_DRAW);

  // Activem l'atribut colorLoc
  glVertexAttribPointer(colorLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(colorLoc);

  // Primer VBO - posición | Segundo VBO - color
  glm::vec3 pos_terra[4] = {
      glm::vec3(-1.0, -1.0, -1.0),
      glm::vec3(-1.0, -1.0, 1.0),
      glm::vec3(1.0, -1.0, -1.0),
      glm::vec3(1.0, -1.0, 1.0)
  };
  glm::vec3 col_terra[4] = {
      glm::vec3(1,0,1),
      glm::vec3(1,0,1),
      glm::vec3(1,0,1),
      glm::vec3(1,0,1)
  };

  glGenVertexArrays(1, &VAO_terra);
  glBindVertexArray(VAO_terra);

  glGenBuffers(1, &VBO_terra);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_terra);
  glBufferData(GL_ARRAY_BUFFER, sizeof(pos_terra), pos_terra, GL_STATIC_DRAW);

  // Activem l'atribut vertexLoc
  glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(vertexLoc);

  glGenBuffers(1, &VBO_terra2);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_terra2);
  glBufferData(GL_ARRAY_BUFFER, sizeof(col_terra), col_terra, GL_STATIC_DRAW);

  // Activem l'atribut colorLoc
  glVertexAttribPointer(colorLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(colorLoc);


  glBindVertexArray (0);
}

void MyGLWidget::carregaShaders()
{
  // Creem els shaders per al fragment shader i el vertex shader
  QOpenGLShader fs (QOpenGLShader::Fragment, this);
  QOpenGLShader vs (QOpenGLShader::Vertex, this);
  // Carreguem el codi dels fitxers i els compilem
  fs.compileSourceFile("shaders/basicShader.frag");
  vs.compileSourceFile("shaders/basicShader.vert");
  // Creem el program
  program = new QOpenGLShaderProgram(this);
  // Li afegim els shaders corresponents
  program->addShader(&fs);
  program->addShader(&vs);
  // Linkem el program
  program->link();
  // Indiquem que aquest és el program que volem usar
  program->bind();

  // Obtenim identificador per a l'atribut “vertex” del vertex shader
  vertexLoc = glGetAttribLocation (program->programId(), "vertex");
  // Obtenim identificador per a l'atribut “color” del vertex shader
  colorLoc = glGetAttribLocation (program->programId(), "color");
  // Uniform locations
  transLoc = glGetUniformLocation(program->programId(), "TG");
  projLoc = glGetUniformLocation(program->programId(), "proj");
  viewLoc = glGetUniformLocation(program->programId(), "view");
}


void MyGLWidget::projectTransform(){
    glm::mat4 Proj = glm::perspective(FOV, raV, zN, zF);
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, &Proj[0][0]);

}

void MyGLWidget::viewTransform(){
    //glm::mat4 View = glm::lookAt(OBS, VRP, UP);
    glm::mat4 View = glm::translate(glm::mat4(1.0f), glm::vec3(-0, -0, -(radi*1.5)));
    View = glm::translate(View, -VRP);

    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &View[0][0]);


}

void MyGLWidget::calculaCapsaCont(){
    // Pmin de la capsa - inicialització
    Pmin.x = homer.vertices()[0];
    Pmin.y = homer.vertices()[1];
    Pmin.z = homer.vertices()[2];

    // Pmax de la capsa - inicialització
    Pmax.x = homer.vertices()[0];
    Pmax.y = homer.vertices()[1];
    Pmax.z = homer.vertices()[2];

    int mida = homer.vertices().size();

    // Cerca de Pmin i Pmax
    for (int i=3; i<mida; i+=3)
    {
        float nouX = homer.vertices()[i];
        float nouY = homer.vertices()[i+1];
        float nouZ = homer.vertices()[i+2];

        // Pmin i Pmax - coordenada x
        Pmin.x = std::min(Pmin.x, nouX);
        Pmax.x = std::max(Pmax.x, nouX);

        // Pmin i Pmax - coordenada y
        Pmin.y = std::min(Pmin.y, nouY);
        Pmax.y = std::max(Pmax.y, nouY);

        // Pmin i Pmax - coordenada z
        Pmin.z = std::min(Pmin.z, nouZ);
        Pmax.z = std::max(Pmax.z, nouZ);
    }
  //  std::cout << "Capsa contenidora:" << std::endl;
  //  std::cout << "Pmin: (" << Pmin.x << ", " << Pmin.y << ", " << Pmin.z << ")" << std::endl;
  //  std::cout << "Pmax: (" << Pmax.x << ", " << Pmax.y << ", " << Pmax.z << ")" << std::endl << std::endl;
    calculaRadiCapsa();
}

void MyGLWidget::calculaRadiCapsa()
{
    // Fórmula para obtener el centro de la caja
    centre = glm::vec3((Pmax.x + Pmin.x)/2, Pmin.y, (Pmax.z + Pmin.z)/2);
    float modX = pow(Pmax.x - centre.x, 2);
    float modY = pow(Pmax.y - centre.y, 2);
    float modZ = pow(Pmax.z - centre.z, 2);

    radi = sqrt(modX + modY + modZ);

   // std::cout << "Radi: " << radi << std::endl;
}
