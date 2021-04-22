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

  //rota=float(M_PI/4);
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
  //modelTransformY();

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

  // Creació del Vertex Array Object per pintar
  glGenVertexArrays(1, &VAO_homer);
  glBindVertexArray(VAO_homer);

  // Construcció de dos VBOs
  // 1 - a partir del mètode VBO_vertices()
  glGenBuffers(1, &VBO_homer);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_homer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*homer.faces().size()*3*3, homer.VBO_vertices(), GL_STATIC_DRAW);

  // Activem l'atribut vertexLoca
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

  glm::mat4 Proj=glm::perspective((float)M_PI/2.0f, 1.0f, 0.3f, 3.0f);
  glUniformMatrix4fv(projLoc, 1, GL_FALSE, &Proj[0][0]);

}

void MyGLWidget::viewTransform(){
  glm::mat4 View = glm::lookAt(glm::vec3(0,0,1),glm::vec3(0,0,0),glm::vec3(0,1,0));
  glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &View[0][0]);

}
