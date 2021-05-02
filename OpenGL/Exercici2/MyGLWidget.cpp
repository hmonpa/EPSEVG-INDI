#include "MyGLWidget.h"

#include <iostream>

MyGLWidget::MyGLWidget (QWidget* parent) : QOpenGLWidget(parent), program(NULL)
{
  setFocusPolicy(Qt::StrongFocus);  // per rebre events de teclat
  //connect(this, SIGNAL(QDial::valueChanged(int)), this, SLOT(giraPatri1()));
  //connect(this, SIGNAL(signalcanviaDial1(int)), this, SLOT(QDial::setValue(int)));
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

  // Càlcul radi Escena
  calculaRadiEsc();

  // Càlcul centre Escena
  calculaCentreEsc();

  patriX1=15.0;
  patriZ1=15.0;
  patriX2=-15.0;
  patriZ2=-15.0;

  std::cout << "Radi calculat: " << radiEsc << std::endl;
  std::cout << "Centre escena: " << centreEsc.x << "," << centreEsc.y << "," << centreEsc.z << std::endl;
  //posHomer = glm::vec3(0, 1, 0);
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

  canvicamera=false;                    // Per defecte false = Estat inicial escena

  dist = radiEsc * 2.0;
  fov_orig = 2.0*asin(radiEsc/dist);    // 60 graus

  zn = radiEsc;
  zf = 3.0*radiEsc;

  if (ra < 1.0) fov = 2.0 * atan(tan(0.5*fov_orig/ra));
  else fov = fov_orig;

  rotacio1 = M_PI/180*45;
  rotacio2 = M_PI/180*45;

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
    TG = glm::translate(TG, glm::vec3(patriX1,0,patriZ1));
    TG = glm::rotate(TG, rotacio1, glm::vec3(0, 1, 0));
    TG = glm::scale(TG, glm::vec3(escalaPatri, escalaPatri, escalaPatri));
    TG = glm::translate(TG, -centreBasePatri);
    glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}


void MyGLWidget::modelTransformPatri2()
{
    glm::mat4 TG(1.f);
    TG = glm::translate(TG, glm::vec3(patriX2,0,patriZ2));
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
      //std::cout << "FOV segona càmera: " << fov_orig << std::endl;
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
      torna_inici();
	  break;
    }
    case Qt::Key_C: {
        canvi_cam();
        break;
    }
    case Qt::Key_R: {   // Patricio 1
      if (value1 < 12) value1+=1;
      else value1=1;
      emit signalcanviaDial1(value1);
      break;
    }
    case Qt::Key_T: {   // Patricio 2
      if (value2 < 12) value2+=1;
      else value2=1;
      emit signalcanviaDial2(value2);
      break;
    }
    default: event->ignore(); break;
  }
  //viewTransform();
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
  if (DoingInteractive == ROTATE && !canvicamera)
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
}

void MyGLWidget::calculaRadiEsc()
{
    glm::vec3 Pmax;
    glm::vec3 Pmin;
    float modX, modY, modZ;
    Pmax.x = 20, Pmax.y = 4, Pmax.z = 20;
    Pmin.x = -20, Pmin.y = 0, Pmin.z = -20;

    centreEsc[0] = (Pmax.x+Pmin.x)/2;
    centreEsc[1] = (Pmax.y+Pmin.y)/2;
    centreEsc[2] = (Pmax.z+Pmin.z)/2;

    modX = pow(Pmax.x - centreEsc.x, 2);
    modY = pow(Pmax.y - centreEsc.y, 2);
    modZ = pow(Pmax.z - centreEsc.z, 2);

    radiEsc = sqrt(modX + modY + modZ);
}

void MyGLWidget::calculaCentreEsc()
{
    glm::vec3 Pmax;
    glm::vec3 Pmin;
    Pmax.x = 20, Pmax.y = 4, Pmax.z = 20;
    Pmin.x = -20, Pmin.y = 0, Pmin.z = -20;

    // centreEsc = Pmax.x+Pmin.x/2, Pmin.y, Pmax.z+Pmin.z/2 = (0,0,0)

    centreEsc[0] = (Pmax.x+Pmin.x)/2;
    centreEsc[1] = Pmin.y;
    centreEsc[2] = (Pmax.z+Pmin.z)/2;
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

// -----------------------------------------------------------------
// Funcions utilitzades per tecles i utilitzades com a slots per interactuar amb els elements d'interficie
// -----------------------------------------------------------------
void MyGLWidget::torna_inici()
{
    makeCurrent();
    int val=1;
    emit signalinici(val);
    iniEscena();
    iniCamera();
    update();
}

// Explicació de les funcions giraPatriX
// -----------------------------
/* Ubicació inicial Patricio 1 = (15,0,15) -> podriem dir que es el Pmax de la circumferència
 * Centre rotació = (10,0,10) -> l'aconseguiriem fent (Pmax.x+Pmin.x)/2, Pmin.y, (Pmax.z+Pmin.z)/2
 * per tant, sabem que Pmin (o punt final a on arribar per deixar el centre rotació fixat) es = (5,0,5)
 * Per altra banda, es vol que el Patricio roti M_PI/6 (30º) seguint aquest recorregut, per tant,
 * per aconseguir deixar al Patricio en el seu estat de rotació inicial (girar-lo 360º) tindrà que fer 12 moviments (360/30).
 * La distancia entre el "punt inicial" i el "punt final" es de 10 d'anada i 10 de tornada, en total 20 de distància entre punts.
 * Per tant, sabem que el Patricio ha de moures 20 punts en 12 moviments, el que es igual a 1.66 punts per moviment.
 * Encara que, no terminarà de ser una circumferencia ja que només tenim 12 moviments, per tant, mes bé seria un dodecagon semblant a la figura de sota:
 *
 *                              *   *
 *                          *           *
 *
 *                      *                   *
 *
 *                      *                   *
 *
 *                          *           *
 *                              *   *
 *
 * Per al Patricio 2, es exactament el mateix però canviant el sentit de rotació i els punts de coordenades.
 */
void MyGLWidget::giraPatri1()
{
    makeCurrent();
    if (patriX1 > 5.1 and patriZ1 >= 15.0)
    {
        patriX1=patriX1-(1.66*2);
        if (patriX1>11.5) patriZ1+=1.66;
        else if (patriX1<8) patriZ1-=1.66;
    }
    else if (patriX1 <= 5.1 and patriZ1 >= 5.1)
    {
        patriZ1=patriZ1-(1.66*2);
        if (patriZ1>11.5) patriX1-=1.66;
        else if (patriZ1<8) patriX1+=1.66;
    }
    else if (patriX1 < 15.0 and patriZ1 <= 5.1)
    {
        patriX1=patriX1+(1.66*2);
        if (patriX1>12) patriZ1+=1.66;
        else if (patriX1<9) patriZ1-=1.66;
    }
    else if (patriX1 >= 15.0 and patriZ1 < 15)
    {
        patriZ1=patriZ1+(1.66*2);
        if (patriZ1<9) patriX1+=1.66;
        else if (patriZ1>12) patriX1-=1.66;
    }
    rotacio1 -= float(M_PI/6);
    modelTransformPatri1();
    update();
}

void MyGLWidget::giraPatri2()
{
    makeCurrent();
    if (patriZ2 <= -5.1 and patriX2 <= -15.0)
    {
        patriZ2=patriZ2+(1.66*2);
        if (patriX2==-15) patriX2-=1.66;
        else if (patriZ2>-8) patriX2+=1.66;
    }
    else if (patriX2 <= -5.1 and patriZ2 >= -5.1)
    {
        patriX2=patriX2+(1.66*2);
        if (patriX2<-11) patriZ2+=1.66;
        else if (patriX2>-8) patriZ2-=1.66;
    }
    else if (patriX2 >= -5.1 and patriZ2 > -15.0)
    {
        patriZ2=patriZ2-(1.66*2);
        if (patriZ2>-9) patriX2+=1.66;
        else if (patriZ2<-12) patriX2-=1.66;
    }
    else if (patriX2 >= -15.0 and patriZ2 <= -15.0)
    {
        patriX2=patriX2-(1.66*2);
        if (patriX2>-9) patriZ2-=1.66;
        else if (patriX2<-12) patriZ2+=1.66;
    }
    rotacio2 += float(M_PI/6);
    modelTransformPatri2();
    update();
}

void MyGLWidget::canvi_cam()
{
    makeCurrent();
    canvicamera=!canvicamera;
    viewTransform();
    update();
}
