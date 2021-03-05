
//#include <GL/glew.h>
#include "MyGLWidget.h"

#include <iostream>

MyGLWidget::MyGLWidget (QWidget* parent) : QOpenGLWidget(parent), program(NULL)
{
  setFocusPolicy(Qt::StrongFocus);  // per rebre events de teclat
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
  
  glClearColor (0.5, 0.7, 1.0, 1.0); // defineix color de fons (d'esborrat)
  carregaShaders();
  creaBuffers1();
  //creaBuffers2();
}

void MyGLWidget::paintGL ()
{
// Aquest codi és necessari únicament per a MACs amb pantalla retna.
#ifdef __APPLE__
  GLint vp[4];
  glGetIntegerv (GL_VIEWPORT, vp);
  ample = vp[2];
  alt = vp[3];
#endif

// En cas de voler canviar els paràmetres del viewport, descomenteu la crida següent i
// useu els paràmetres que considereu (els que hi ha són els de per defecte)
//  glViewport (0, 0, ample, alt);
  
  glClear (GL_COLOR_BUFFER_BIT);  // Esborrem el frame-buffer

  // Activem l'Array a pintar 
  glBindVertexArray(VAO1);

  //glDrawArrays(GL_TRIANGLES, 0, 3)        // Ej 1.1: Número de vértices a pintar!
  //glDrawArrays(GL_TRIANGLES, 0, 6);       // Ej 1.2
  //glDrawArrays(GL_TRIANGLES, 0, 9);       // Ej 1.3
  //glDrawArrays(GL_TRIANGLES, 0, 12);      // Ej 1.4 y 1.5a)
    glDrawArrays(GL_TRIANGLES, 0, 24);      // Ej 1.5b)
  // Pintem l'escena



  //glBindVertexArray(VAO2);
  //glDrawArrays(GL_TRIANGLES, 0, 3);

  // Desactivem el VAO
  glBindVertexArray(0);
}

void MyGLWidget::resizeGL (int w, int h)
{
  ample = w;
  alt = h;
}

void MyGLWidget::creaBuffers1 ()
{
    // Exercici 1.1
      /*glm::vec3 Vertices[3];  // Tres vèrtexs amb X, Y i Z
      Vertices[0] = glm::vec3(-2.0, -1.0, 0.0);
      Vertices[1] = glm::vec3(1.0, -1.0, 0.0);
      Vertices[2] = glm::vec3(0.0, 1.0, 0.0);*/


    // Exercici 1.2
    /*glm::vec3 Vertices[6];  // Sis vèrtexs amb X, Y i Z
    Vertices[0] = glm::vec3(1, -1, 0.0);
    Vertices[1] = glm::vec3(1, 1, 0.0);
    Vertices[2] = glm::vec3(-1, -1, 0.0);

    Vertices[3] = glm::vec3(1, 1, 0.0);
    Vertices[4] = glm::vec3(-1, 1, 0.0);
    Vertices[5] = glm::vec3(-1, -1, 0.0);*/

  
    // Exercici 1.3
    /*glm::vec3 Vertices[9];  // Nou vèrtexs amb X, Y i Z
    // Parte cuadrada
    Vertices[0] = glm::vec3(0, -1, 0.0);
    Vertices[1] = glm::vec3(0, 0, 0.0);
    Vertices[2] = glm::vec3(-1, -1, 0.0);
    Vertices[3] = glm::vec3(0, 0, 0.0);
    Vertices[4] = glm::vec3(-1, 0.0, 0.0);
    Vertices[5] = glm::vec3(-1, -1, 0.0);
    // Techo
    Vertices[6] = glm::vec3(0.0, 0.0, 0.0);
    Vertices[7] = glm::vec3(-0.5, 1, 0.0);
    Vertices[8] = glm::vec3(-1, 0.0, 0.0);*/

    // Exercici 1.4
    /*glm::vec3 Vertices[12];  // Nou vèrtexs amb X, Y i Z
    // Parte cuadrada
    Vertices[0] = glm::vec3(0, -1, 0.0);
    Vertices[1] = glm::vec3(0, 0, 0.0);
    Vertices[2] = glm::vec3(-1, -1, 0.0);
    Vertices[3] = glm::vec3(0, 0, 0.0);
    Vertices[4] = glm::vec3(-1, 0.0, 0.0);
    Vertices[5] = glm::vec3(-1, -1, 0.0);
    // Techo 1
    Vertices[6] = glm::vec3(0.0, 0.0, 0.0);
    Vertices[7] = glm::vec3(-0.5, 1, 0.0);
    Vertices[8] = glm::vec3(-1, 0.0, 0.0);
    // Techo 2
    Vertices[9] = glm::vec3(0.0, 0.0, 0.0);
    Vertices[10] = glm::vec3(0.5, 1, 0.0);
    Vertices[11] = glm::vec3(1, 0.0, 0.0);*/

    // Exercici 1.5a)
    /*glm::vec3 Vertices[12];  // Nou vèrtexs amb X, Y i Z
    // Parte cuadrada
    Vertices[0] = glm::vec3(-1, -0.5, 0.0);
    Vertices[1] = glm::vec3(-1, -1, 0.0);
    Vertices[2] = glm::vec3(-0.5, -1, 0.0);
    Vertices[3] = glm::vec3(-0.5, -1, 0.0);
    Vertices[4] = glm::vec3(-1, -0.5, 0.0);
    Vertices[5] = glm::vec3(-0.5, -0.5, 0.0);
    // Techo 1
    Vertices[6] = glm::vec3(-1, -0.5, 0.0);
    Vertices[7] = glm::vec3(-0.5, -0.5, 0.0);
    Vertices[8] = glm::vec3(-0.75, 0.0, 0.0);
    // Techo 2
    Vertices[9] = glm::vec3(-0.5, -0.5, 0.0);
    Vertices[10] = glm::vec3(0, -0.5, 0.0);
    Vertices[11] = glm::vec3(-0.25, 0.0, 0.0);*/

    // Exercici 1.5b)
    glm::vec3 Vertices[24];  // Nou vèrtexs amb X, Y i Z
    // Parte cuadrada 1
    Vertices[0] = glm::vec3(-1, -0.5, 0.0);
    Vertices[1] = glm::vec3(-1, -1, 0.0);
    Vertices[2] = glm::vec3(-0.5, -1, 0.0);
    Vertices[3] = glm::vec3(-0.5, -1, 0.0);
    Vertices[4] = glm::vec3(-1, -0.5, 0.0);
    Vertices[5] = glm::vec3(-0.5, -0.5, 0.0);
    // Techo 1
    Vertices[6] = glm::vec3(-1, -0.5, 0.0);
    Vertices[7] = glm::vec3(-0.5, -0.5, 0.0);
    Vertices[8] = glm::vec3(-0.75, 0.0, 0.0);
    // Techo 2
    Vertices[9] = glm::vec3(-0.5, -0.5, 0.0);
    Vertices[10] = glm::vec3(0, -0.5, 0.0);
    Vertices[11] = glm::vec3(-0.25, 0.0, 0.0);

    // Parte cuadrada 2
    Vertices[12] = glm::vec3(0.5, 0, 0.0);
    Vertices[13] = glm::vec3(0.5, 0.5, 0.0);
    Vertices[14] = glm::vec3(0, 0.5, 0.0);
    Vertices[15] = glm::vec3(0, 0.5, 0.0);
    Vertices[16] = glm::vec3(0.5, 0, 0.0);
    Vertices[17] = glm::vec3(0, 0, 0.0);
    // Techo 1
    Vertices[18] = glm::vec3(0.5, 0.5, 0.0);
    Vertices[19] = glm::vec3(0, 0.5, 0.0);
    Vertices[20] = glm::vec3(0.25, 1, 0.0);
    // Techo 2
    Vertices[21] = glm::vec3(0.5, 0.5, 0.0);
    Vertices[22] = glm::vec3(1, 0.5, 0.0);
    Vertices[23] = glm::vec3(0.75, 1, 0.0);

  // Creació del Vertex Array Object (VAO) que usarem per pintar
  glGenVertexArrays(1, &VAO1);
  glBindVertexArray(VAO1);

  // Creació del buffer amb les dades dels vèrtexs
  GLuint VBO1;
  glGenBuffers(1, &VBO1);
  glBindBuffer(GL_ARRAY_BUFFER, VBO1);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
  // Activem l'atribut que farem servir per vèrtex (només el 0 en aquest cas)	
  glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(vertexLoc);

  // Desactivem el VAO
  glBindVertexArray(0);
}
/*
void MyGLWidget::creaBuffers2 ()
{
    glm::vec3 Vertices[3];  // Nou vèrtexs amb X, Y i Z
    Vertices[0] = glm::vec3(0.5, 0.5, 0.0);
    Vertices[1] = glm::vec3(0.0, 1.0, 0.0);
    Vertices[2] = glm::vec3(-0.5, 0.5, 0.0);

  // Creació del Vertex Array Object (VAO) que usarem per pintar
  glGenVertexArrays(1, &VAO2);
  glBindVertexArray(VAO2);

  // Creació del buffer amb les dades dels vèrtexs
  GLuint VBO1;
  glGenBuffers(1, &VBO1);
  glBindBuffer(GL_ARRAY_BUFFER, VBO1);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
  // Activem l'atribut que farem servir per vèrtex (només el 0 en aquest cas)
  glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(vertexLoc);

  // Desactivem el VAO
  glBindVertexArray(0);
}*/

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
}
