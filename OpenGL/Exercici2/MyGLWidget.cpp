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

  glClearColor(0.5, 0.7, 1.0, 1.0); // defineix color de fons (d'esborrat)
  glEnable(GL_DEPTH_TEST);
  carregaShaders();
  iniEscena ();
  iniCamera ();
}

void MyGLWidget::iniEscena ()
{
  creaBuffersTerra();
  creaBuffersCub();
  //creaBuffersHomer();
  creaBuffersPatricio();

  centreEsc = glm::vec3(0,0,0);
  //radiEsc = 5;

  // Càlcul radi Escena
  /*  Pmax.x = 20, Pmax.y = 4, Pmax.z = 20
  Pmin.x = -20, Pmin.y = 0, Pmin.z = -20
  centreEsc = Pmax.x+Pmin.x/2, Pmax.y+Pmin.y/2, Pmax.z+Pmin.z/2

  modX = (Pmax.x - centreEsc.x)^2 = 400
  modY = (Pmax.y - centreEsc.y)^2 = 16
  modZ = (Pmax.z - centreEsc.z)^2 = 400

  radi = sqrt (modX + modY + modZ) = 28.56*/

  radiEsc = 28.56;
  posHomer = glm::vec3(0, 1, 0);
}

void MyGLWidget::iniCamera ()
{
  angleY = angleX = 0.0;

  // Estat inicial
  obs = centreEsc + glm::vec3(0, 2, 2*radiEsc);
  vrp = glm::vec3(centreEsc);
  up = glm::vec3(0,1,0);

  tetha = M_PI/9.0;
  phi = M_PI/9.0;
  canvicamera=false;

  ra = 1.0;
  dist = radiEsc * 2.0;
  fov_orig = 2.0*asin(radiEsc/dist);
  rotacio1 = M_PI/180*45;
  rotacio2 = M_PI/180*45;
  zn = radiEsc;
  zf = 3.0*radiEsc;

  projectTransform ();
  viewTransform ();
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
  
  // Esborrem el frame-buffer i el depth-buffer
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Activem el VAO per a pintar el Terra 
  glBindVertexArray (VAO_Terra);
  modelTransformTerra ();
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

  // Activem el VAO per a pintar la Cub
  glBindVertexArray (VAO_Cub);
  modelTransformParet ();
  glDrawArrays(GL_TRIANGLES, 0, 36);
  
  // Activem el VAO per a pintar el Homer
  glBindVertexArray (VAO_Homer);
  modelTransformHomer ();
  glDrawArrays(GL_TRIANGLES, 0, homer.faces().size()*3);

  glBindVertexArray(VAO_Patri);
  // Visualització primer Patricio
  modelTransformPatri1();
  glDrawArrays(GL_TRIANGLES, 0, patri.faces().size()*3);

  // Visualització segon Patricio
  modelTransformPatri2();
  glDrawArrays(GL_TRIANGLES, 0, patri.faces().size()*3);
  
  glBindVertexArray(0);
}

void MyGLWidget::resizeGL (int w, int h) 
{
  ample = w;
  alt = h;

  ra = float(ample)/float(alt);
  projectTransform ();
}

void MyGLWidget::modelTransformHomer ()
{
  glm::mat4 TG(1.f);  // Matriu de transformació
  TG = glm::translate(TG, glm::vec3 (2, 1, 0.5));

  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::modelTransformPatri1()
{
    glm::mat4 TG(1.f);
    TG = glm::translate(TG, glm::vec3(15,0,15));
    TG = glm::rotate(TG, rotacio1, glm::vec3(0, 1, 0));
    TG = glm::scale(TG, glm::vec3(escalaPatri, escalaPatri, escalaPatri));
    TG = glm::translate(TG, -centreBasePatri);
    glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}


void MyGLWidget::modelTransformPatri2()
{
    glm::mat4 TG(1.f);
    TG = glm::translate(TG, glm::vec3(-15,0,-15));
    TG = glm::rotate(TG, rotacio2, glm::vec3(0, 1, 0));
    TG = glm::scale(TG, glm::vec3(escalaPatri, escalaPatri, escalaPatri));
    TG = glm::translate(TG, -centreBasePatri);
    glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::modelTransformParet ()  // Cal modificar aquesta funció...
{
  glm::mat4 TG(1.f);  // Matriu de transformació
  TG = glm::scale(TG,glm::vec3(0.6,2.0,40.0));
  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::modelTransformTerra ()
{
  glm::mat4 TG(1.f);  // Matriu de transformació
  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::projectTransform () // Cal modificar aquesta funció...
{
  if (ra < 1.0) fov = 2.0 * atan(tan(0.5*fov_orig/ra));
  else fov = fov_orig;

  //std::cout << "Ra: " << ra << std::endl;

  glm::mat4 Proj;  // Matriu de projecció
  Proj = glm::perspective(fov, ra, zn, zf);
  glUniformMatrix4fv (projLoc, 1, GL_FALSE, &Proj[0][0]);
}

void MyGLWidget::viewTransform () // Cal modificar aquesta funció...
{
   // Matriu de posició i orientació
  if(canvicamera){
      obs = glm::vec3(0,40,0);
      vrp = glm::vec3(centreEsc);
      up = glm::vec3(-1,0,0);
      glm::mat4 View;
      View = glm::lookAt(obs, vrp, up);
      glUniformMatrix4fv (viewLoc, 1, GL_FALSE, &View[0][0]);
  }
  else
  {
      glm::mat4 View = glm::translate(glm::mat4(1.0f), glm::vec3(0., 0., -dist));
      View = glm::rotate(View, tetha, glm::vec3(1,0,0));
      View = glm::rotate(View, -phi, glm::vec3(0,1,0));
      View = glm::translate(View, -vrp);
      glUniformMatrix4fv (viewLoc, 1, GL_FALSE, &View[0][0]);
  }
}

void MyGLWidget::keyPressEvent(QKeyEvent* event)  // Cal modificar aquesta funció...
{
  makeCurrent();
  switch (event->key()) {
	case Qt::Key_I: {
      iniCamera();
	  break;
    }
    case Qt::Key_C: {
        canvicamera=!canvicamera;
        break;
    }
    case Qt::Key_R: {
        rotacio1 -= float(M_PI/6);
        modelTransformPatri1();
        break;
    }
    case Qt::Key_T: {
        rotacio2 += float(M_PI/6);
        modelTransformPatri2();
        break;
    }
    default: event->ignore(); break;
  }
  viewTransform();
  update();
}

void MyGLWidget::mousePressEvent (QMouseEvent *e)
{
  xClick = e->x();
  yClick = e->y();

  if (e->button() & Qt::LeftButton &&
      ! (e->modifiers() & (Qt::ShiftModifier|Qt::AltModifier|Qt::ControlModifier)))
  {
    DoingInteractive = ROTATE;
  }
}

void MyGLWidget::mouseReleaseEvent( QMouseEvent *)
{
  DoingInteractive = NONE;
}

void MyGLWidget::mouseMoveEvent(QMouseEvent *e)
{
  makeCurrent();
  if (DoingInteractive == ROTATE)
  {
    float angleX = e->x();
    float angleY = e->y();

    float x = angleX - xClick;
    float y = angleY - yClick;

    phi += (x/width())*10;
    tetha += (y/height())*10;

    viewTransform ();
  }

  xClick = e->x();
  yClick = e->y();

  update ();
}

void MyGLWidget::calculaCapsaModel (Model &p, float &escala, glm::vec3 &centreBase)
{
  // Càlcul capsa contenidora i valors transformacions inicials
  float minx, miny, minz, maxx, maxy, maxz;
  minx = maxx = p.vertices()[0];
  miny = maxy = p.vertices()[1];
  minz = maxz = p.vertices()[2];
  for (unsigned int i = 3; i < p.vertices().size(); i+=3)
  {
    if (p.vertices()[i+0] < minx)
      minx = p.vertices()[i+0];
    if (p.vertices()[i+0] > maxx)
      maxx = p.vertices()[i+0];
    if (p.vertices()[i+1] < miny)
      miny = p.vertices()[i+1];
    if (p.vertices()[i+1] > maxy)
      maxy = p.vertices()[i+1];
    if (p.vertices()[i+2] < minz)
      minz = p.vertices()[i+2];
    if (p.vertices()[i+2] > maxz)
      maxz = p.vertices()[i+2];
  }
  escala = 1.0/(maxy-miny);
  centreBase[0] = (minx+maxx)/2.0; 
  centreBase[1] = miny; 
  centreBase[2] = (minz+maxz)/2.0;

  //std::cout << "Capsa contenidora real:" << std::endl;
  //std::cout << "Pmin real: (" << minx << ", " << miny << ", " << minz << ")" << std::endl;
  //std::cout << "Pmax real: (" << maxx << ", " << maxy << ", " << maxz << ")" << std::endl << std::endl;

  // PRUEBAS
  glm::vec3 centre = glm::vec3((maxx + minx)/2, miny, (maxz+minz)/2);
  float modX = pow(maxx - centre.x, 2);
  float modY = pow(maxy - centre.y, 2);
  float modZ = pow(maxz - centre.z, 2);

  radiEsc = sqrt(modX + modY + modZ);
  std::cout << "radi original model: " << radiEsc << std::endl;
/*
  glm::vec3 PMAX = glm::vec3(2.5, 4.0, 2.5);
  glm::vec3 PMIN = glm::vec3(-2.5, 0.0, -2.5);
  glm::vec3 centreEsc = glm::vec3((PMAX.x + PMIN.x)/2, (PMAX.y + PMIN.y)/2, (PMAX.z + PMIN.z)/2);

  std::cout << "Centre caixa Patricio: " << centreEsc.x << "," << centreEsc.y << "," << centreEsc.z << std::endl;
  float modX = pow(PMAX.x - centreEsc.x, 2);
  float modY = pow(PMAX.y - centreEsc.y, 2);
  float modZ = pow(PMAX.z - centreEsc.z, 2);
  radiEsc = sqrt(modX + modY + modZ);
*/
  //escalaPatri = glm::vec3(escala);
  //centreBasePatri = PMIN;

  //std::cout << "Centre base Patricio: " << centreBasePatri.x << "," << centreBasePatri.y << "," << centreBasePatri.z << std::endl;*/
}

void MyGLWidget::creaBuffersCub ()
{
  // Dades del cub
  // Vèrtexs del cub
  glm::vec3 vertexs[8] = {
       /* 0*/ glm::vec3( -0.5, 0.0, -0.5),  /* 1*/ glm::vec3( 0.5, 0.0, -0.5),
       /* 2*/ glm::vec3( -0.5, 1.0, -0.5),  /* 3*/ glm::vec3( 0.5, 1.0, -0.5),
       /* 4*/ glm::vec3( -0.5, 0.0, 0.5),  /* 5*/ glm::vec3( 0.5, 0.0, 0.5),
       /* 6*/ glm::vec3( -0.5, 1.0, 0.5),  /* 7*/ glm::vec3( 0.5, 1.0, 0.5)
  };

  // VBO amb la posició dels vèrtexs
  glm::vec3 poscub[36] = {  // 12 triangles
       vertexs[0], vertexs[2], vertexs[1],
       vertexs[1], vertexs[2], vertexs[3],
       vertexs[5], vertexs[1], vertexs[7],
       vertexs[1], vertexs[3], vertexs[7],
       vertexs[2], vertexs[6], vertexs[3],
       vertexs[3], vertexs[6], vertexs[7],
       vertexs[0], vertexs[4], vertexs[6],
       vertexs[0], vertexs[6], vertexs[2],
       vertexs[0], vertexs[1], vertexs[4],
       vertexs[1], vertexs[5], vertexs[4],
       vertexs[4], vertexs[5], vertexs[6],
       vertexs[5], vertexs[7], vertexs[6]
  };
  
  glm::vec3 color(0, 0, 1.0);

  glm::vec3 colorcub[36] = {
        color, color, color, color, color, color,
        color, color, color, color, color, color,
        color, color, color, color, color, color,
        color, color, color, color, color, color,
        color, color, color, color, color, color,
        color, color, color, color, color, color
  };
  
  // Creació del Vertex Array Object del cub
  glGenVertexArrays(1, &VAO_Cub);
  glBindVertexArray(VAO_Cub);

  GLuint VBO_Cub[2];
  glGenBuffers(2, VBO_Cub);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_Cub[0]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(poscub), poscub, GL_STATIC_DRAW);

  // Activem l'atribut vertexLoc
  glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(vertexLoc);

  glBindBuffer(GL_ARRAY_BUFFER, VBO_Cub[1]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(colorcub), colorcub, GL_STATIC_DRAW);

  // Activem l'atribut normalLoc
  glVertexAttribPointer(colorLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(colorLoc);

  glBindVertexArray(0);
}

void MyGLWidget::creaBuffersHomer ()
{
  // Carreguem el model de l'OBJ - Atenció! Abans de crear els buffers!
  homer.load("./models/HomerProves.obj");

  // Creació del Vertex Array Object del Patricio
  glGenVertexArrays(1, &VAO_Homer);
  glBindVertexArray(VAO_Homer);

  // Creació dels buffers del model
  GLuint VBO_Homer[2];
  // Buffer de posicions
  glGenBuffers(2, VBO_Homer);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_Homer[0]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*homer.faces().size()*3*3, homer.VBO_vertices(), GL_STATIC_DRAW);

  // Activem l'atribut vertexLoc
  glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(vertexLoc);

  // Buffer de component difusa usada com a color
  glBindBuffer(GL_ARRAY_BUFFER, VBO_Homer[1]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*homer.faces().size()*3*3, homer.VBO_matdiff(), GL_STATIC_DRAW);

  glVertexAttribPointer(colorLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(colorLoc);

  glBindVertexArray(0);
}


void MyGLWidget::creaBuffersPatricio ()
{
  // Cal completar aquest mètode afegint també en el .h els atributs que 
  // siguin necessaris...
  patri.load("./models/Patricio.obj");

  // Càlcul capsa contenidora Patricio
  calculaCapsaModel(patri, escalaPatri, centreBasePatri);
  escalaPatri = escalaPatri * 4.f;

  glGenVertexArrays(1, &VAO_Patri);
  glBindVertexArray(VAO_Patri);

  GLuint VBO_Patri[2];

  glGenBuffers(2, VBO_Patri);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_Patri[0]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*patri.faces().size()*3*3, patri.VBO_vertices(), GL_STATIC_DRAW);

  glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(vertexLoc);

  glBindBuffer(GL_ARRAY_BUFFER, VBO_Patri[1]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*patri.faces().size()*3*3, patri.VBO_matdiff(), GL_STATIC_DRAW);

  glVertexAttribPointer(colorLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(colorLoc);
  glBindVertexArray(0);
}


void MyGLWidget::creaBuffersTerra ()
{
  // Dades del terra
  // VBO amb la posició dels vèrtexs
  glm::vec3 posterra[4] = {
    glm::vec3(-20.0, 0.0, -20.0),
    glm::vec3(-20.0, 0.0, 20.0),
    glm::vec3(20.0, 0.0, -20.0),
    glm::vec3(20.0, 0.0, 20.0),
  }; 

  glm::vec3 color(0.6,0.6,0.0);

  // Fem que aquest material afecti a tots els vèrtexs per igual
  glm::vec3 colterra[4] = {
	color, color, color, color
  };

  // Creació del Vertex Array Object del terra
  glGenVertexArrays(1, &VAO_Terra);
  glBindVertexArray(VAO_Terra);

  GLuint VBO_Terra[2];
  glGenBuffers(2, VBO_Terra);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_Terra[0]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(posterra), posterra, GL_STATIC_DRAW);

  // Activem l'atribut vertexLoc
  glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(vertexLoc);

  glBindBuffer(GL_ARRAY_BUFFER, VBO_Terra[1]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(colterra), colterra, GL_STATIC_DRAW);

  glVertexAttribPointer(colorLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(colorLoc);

  glBindVertexArray(0);
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

  // Demanem identificadors per als uniforms del vertex shader
  transLoc = glGetUniformLocation (program->programId(), "TG");
  projLoc = glGetUniformLocation (program->programId(), "proj");
  viewLoc = glGetUniformLocation (program->programId(), "view");
}

void MyGLWidget::torna_inici()
{
    makeCurrent();
    iniCamera();
    update();
}

void MyGLWidget::giraPatri1()
{
    makeCurrent();
    rotacio1 -= float(M_PI/6);
    modelTransformPatri1();
    update();
}

void MyGLWidget::giraPatri2()
{
    makeCurrent();
    rotacio2 += float(M_PI/6);
    modelTransformPatri2();
    update();
}
