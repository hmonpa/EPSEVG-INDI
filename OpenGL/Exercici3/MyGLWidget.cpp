#include "MyGLWidget.h"

#include <iostream>

MyGLWidget::MyGLWidget (QWidget* parent) : QOpenGLWidget(parent), program(NULL)
{
  setFocusPolicy(Qt::StrongFocus);  // per rebre events de teclat
  xClick = yClick = 0;
  DoingInteractive = NONE;
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

  glClearColor(0.0, 0.0, 0.0, 1.0); // defineix color de fons (d'esborrat)
  glEnable(GL_DEPTH_TEST);
  carregaShaders();
  iniEscena ();
  iniCamera ();
}




void MyGLWidget::iniEscena ()
{
  creaBuffersHangar();
  creaBuffersPersonatge();
  creaBuffersTerra();


  avioPos = glm::vec3(0.0, 0.0, -5.0);
  centreEsc = glm::vec3 (0, 0, 0);  
  radiEsc = 15;

  colFocus = glm::vec3(0.8,0.8,0.8);            // colFocus
  glUniform3fv(colFocusLoc, 1, &colFocus[0]);

  llumAmbient = glm::vec3(0.1, 0.1, 0.1);       // llumambient passada com a uniform
  glUniform3fv(llumLoc, 1, &llumAmbient[0]);

  posFocus = glm::vec3(0.f,5.f,-10.f);          // posicioFocus: On apunta el focus de llum?
  glUniform3fv(posFocusLoc, 1, &posFocus[0]);

  posFocus2 = glm::vec3(0.f,5.f,0.f);
  glUniform3fv(posFocusLoc2, 1, &posFocus2[0]);

  posFocus3 = glm::vec3(0.f,5.f,10.f);
  glUniform3fv(posFocusLoc3, 1, &posFocus3[0]);

  off = glm::vec3(-100.f,-100.f,-100.f);        // Utilitzat per apuntar un focus de llum i simular que està apagat

  posLlumReactor = glm::vec3(-1.9, 3.60, -5.65);
  colorLlumReactor = glm::vec3(1.0,0.2,0.0);
  calculaSCA();
}

void MyGLWidget::iniCamera ()
{
  ra = 1.0;
  angleY = angleX = 0.0;

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

    projectTransform ();
    viewTransform ();
    // Esborrem el frame-buffer i el depth-buffer
    glClearColor(0.4f, 0.4f, 0.5f, 1.f);                            // Ex1 : Modificació del color de fons
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //--------------------------------------------------------
    // Activem el VAO per a pintar el terra
    glBindVertexArray (VAO_Terra);
    // pintem terra
    modelTransformTerra ();
    glDrawArrays(GL_TRIANGLES, 0, 12);

    //--------------------------------------------------------
    // Activem el VAO per a pintar l'avió
    glBindVertexArray (VAO_avio);
    // pintem l'avió
    modelTransformAvio();
    glDrawArrays(GL_TRIANGLES, 0, avioModel.faces().size()*3);

    //--------------------------------------------------------
    // Activem el VAO per pintar l'hangar
    glBindVertexArray (VAO_hangar);
    // pintem l'hangar
    modelTransformHangar();
    glDrawArrays(GL_TRIANGLES, 0, hangarModel.faces().size()*3);

    //--------------------------------------------------------
    glBindVertexArray(0);
}

void MyGLWidget::resizeGL (int w, int h)
{
  ample = w;
  alt = h;
  projectTransform ();
}

void MyGLWidget::modelTransformHangar ()
{
  glm::mat4 TG(1.f);  // Matriu de transformació
  float alcada = 8;
  TG = glm::scale(TG, alcada*glm::vec3 ( escalaArc,  escalaArc,  escalaArc));
  TG = glm::translate(TG, -centreBaseHangar);
  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::modelTransformTerra ()
{
  glm::mat4 TG(1.f);  // Matriu de transformació
  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::modelTransformAvio ()
{
  // Codi per a la TG necessària
  float alcada = 3;
  glm::mat4 TG =glm::mat4(1.0f);
  TG = glm::translate(TG, avioPos);
  TG = glm::scale(TG, alcada*glm::vec3(escalaAvio, escalaAvio, escalaAvio));
  TG = glm::translate(TG, -centreBaseAvio);
  glUniformMatrix4fv(transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::projectTransform ()
{
  float fov, zn, zf;
  glm::mat4 Proj;  // Matriu de projecció
  
  fov = float(M_PI/4.0);
  zn = radiEsc*0.25;
  zf = 3*radiEsc;

  float rav =  width() /  (float)height() ;
  if(rav<ra){
      fov= atan(tan(0.5*fov)*radiEsc/rav);
  }

  Proj = glm::perspective(fov, rav, zn, zf);

  glUniformMatrix4fv (projLoc, 1, GL_FALSE, &Proj[0][0]);
}

void MyGLWidget::viewTransform ()
{
  // Matriu de posició i orientació
  View = glm::translate(glm::mat4(1.f), glm::vec3(0, 0, -2*radiEsc));
  View = glm::rotate(View, (float) M_PI/10.0f, glm::vec3(1, 0, 0));
  View = glm::rotate(View, -angleY, glm::vec3(0, 1, 0));
  View = glm::translate(View, -centreEsc);

  glUniformMatrix4fv (viewLoc, 1, GL_FALSE, &View[0][0]);
}


void MyGLWidget::keyPressEvent(QKeyEvent* event)  // Cal modificar aquesta funció...
{
    makeCurrent();

    switch (event->key()) {

        case Qt::Key_S: {
            if(avioPos.z<10) avioPos += glm::vec3(0.f,0.f,0.2f);
            break;
        }
        case Qt::Key_W: {
            if(avioPos.z>-6) avioPos -= glm::vec3(0.f,0.f,0.2f);
            break;
        }
        case Qt::Key_0: {
            controlallums0();
            emit signalPolsa0();
            break;
        }
        case Qt::Key_1: {
            controlallums1();
            emit signalPolsa1();
            break;
        }
        case Qt::Key_2: {
            controlallums2();
            emit signalPolsa2();
            break;
        }
        case Qt::Key_3: {
            controlallums3();
            emit signalPolsa3();
            break;
        }

        default: event->ignore(); break;
    }
    update();
}

// Exs 3 i 4
void MyGLWidget::controlallums0()
{
    makeCurrent();
    colFocus = glm::vec3(0,0,0);
    glUniform3fv(colFocusLoc, 1, &colFocus[0]);
    update();
}

void MyGLWidget::controlallums1()
{
    makeCurrent();
    posFocus = glm::vec3(0.f,5.f,-10.f);
    posFocus2 = off;
    posFocus3 = off;
    colFocus = glm::vec3(0.8,0.8,0.8);
    glUniform3fv(posFocusLoc, 1, &posFocus[0]);
    glUniform3fv(posFocusLoc2, 1, &posFocus2[0]);
    glUniform3fv(posFocusLoc3, 1, &posFocus3[0]);
    glUniform3fv(colFocusLoc, 1, &colFocus[0]);
    update();
}

void MyGLWidget::controlallums2()
{
    makeCurrent();
    posFocus = glm::vec3(0.f,5.f,-10.f);
    posFocus2 = glm::vec3(0.f,5.f,0.f);
    posFocus3 = off;
    colFocus = glm::vec3(0.8,0.8,0.8);
    glUniform3fv(posFocusLoc, 1, &posFocus[0]);
    glUniform3fv(posFocusLoc2, 1, &posFocus2[0]);
    glUniform3fv(posFocusLoc3, 1, &posFocus3[0]);
    glUniform3fv(colFocusLoc, 1, &colFocus[0]);
    update();
}

void MyGLWidget::controlallums3()
{
    makeCurrent();
    posFocus = glm::vec3(0.f,5.f,-10.f);
    posFocus2 = glm::vec3(0.f,5.f,0.f);
    posFocus3 = glm::vec3(0.f,5.f,10.f);
    colFocus = glm::vec3(0.8,0.8,0.8);
    glUniform3fv(posFocusLoc, 1, &posFocus[0]);
    glUniform3fv(posFocusLoc2, 1, &posFocus2[0]);
    glUniform3fv(posFocusLoc3, 1, &posFocus3[0]);
    glUniform3fv(colFocusLoc, 1, &colFocus[0]);
    update();
}

// Ex5
void MyGLWidget::calculaSCA()
{
    posLlumReactor = glm::vec3(View * glm::vec4(posLlumReactor, 1.));
    glUniform3fv(posLlumReactorLoc, 1, &posLlumReactor[0]);
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
    // Fem la rotació
    angleY += (e->x() - xClick) * M_PI / 180.0;    
    viewTransform ();
  }

  xClick = e->x();
  yClick = e->y();

  update ();
}

void MyGLWidget::calculaCapsaModel (Model &m, float &escala, glm::vec3 &centreBase)
{
  // Càlcul capsa contenidora i valors transformacions inicials
  float minx, miny, minz, maxx, maxy, maxz;
  minx = maxx = m.vertices()[0];
  miny = maxy = m.vertices()[1];
  minz = maxz = m.vertices()[2];
  for (unsigned int i = 3; i < m.vertices().size(); i+=3)
  {
    if (m.vertices()[i+0] < minx)
      minx = m.vertices()[i+0];
    if (m.vertices()[i+0] > maxx)
      maxx = m.vertices()[i+0];
    if (m.vertices()[i+1] < miny)
      miny = m.vertices()[i+1];
    if (m.vertices()[i+1] > maxy)
      maxy = m.vertices()[i+1];
    if (m.vertices()[i+2] < minz)
      minz = m.vertices()[i+2];
    if (m.vertices()[i+2] > maxz)
      maxz = m.vertices()[i+2];
  }
  escala = 1.0/(maxy-miny);
  centreBase[0] = (minx+maxx)/2.0; centreBase[1] = miny; centreBase[2] = (minz+maxz)/2.0;
}

void MyGLWidget::creaBuffersHangar(){
  creaBuffersModel(hangarModel,"./models/MINITUNNEL.obj", &VAO_hangar, escalaArc, centreBaseHangar);
}

void MyGLWidget::creaBuffersPersonatge(){
  creaBuffersModel(avioModel,"./models/f-16.obj", &VAO_avio, escalaAvio, centreBaseAvio);
}

void MyGLWidget::creaBuffersModel(Model &model, const char *fileName, GLuint *VAO,  float &escala, glm::vec3 &centreBase )
{
  // Carreguem el model de l'OBJ - Atenció! Abans de crear els buffers!
  model.load(fileName);

  // Calculem la capsa contenidora del model
  calculaCapsaModel(model, escala, centreBase);
  
  // Creació del Vertex Array Object del model
  glGenVertexArrays(1, VAO);
  glBindVertexArray(*VAO);

  // Creació dels buffers del model
  GLuint VBO_Model[6];
  // Buffer de posicions
  glGenBuffers(6, VBO_Model);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_Model[0]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*model.faces().size()*3*3, model.VBO_vertices(), GL_STATIC_DRAW);

  // Activem l'atribut vertexLoc
  glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(vertexLoc);

  // Buffer de normals
  glBindBuffer(GL_ARRAY_BUFFER, VBO_Model[1]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*model.faces().size()*3*3, model.VBO_normals(), GL_STATIC_DRAW);

  glVertexAttribPointer(normalLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(normalLoc);

  // Buffer de component ambient
  glBindBuffer(GL_ARRAY_BUFFER, VBO_Model[2]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*model.faces().size()*3*3, model.VBO_matamb(), GL_STATIC_DRAW);

  glVertexAttribPointer(matambLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(matambLoc);

  // Buffer de component difusa
  glBindBuffer(GL_ARRAY_BUFFER, VBO_Model[3]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*model.faces().size()*3*3, model.VBO_matdiff(), GL_STATIC_DRAW);

  glVertexAttribPointer(matdiffLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(matdiffLoc);

  // Buffer de component especular
  glBindBuffer(GL_ARRAY_BUFFER, VBO_Model[4]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*model.faces().size()*3*3, model.VBO_matspec(), GL_STATIC_DRAW);

  glVertexAttribPointer(matspecLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(matspecLoc);

  // Buffer de component shininness
  glBindBuffer(GL_ARRAY_BUFFER, VBO_Model[5]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*model.faces().size()*3, model.VBO_matshin(), GL_STATIC_DRAW);

  glVertexAttribPointer(matshinLoc, 1, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(matshinLoc);

  glBindVertexArray(0);

}

void MyGLWidget::creaBuffersTerra ()
{
  // VBO amb la posició dels vèrtexs
    int dim = 1000;
  glm::vec3 posterra[6] = {
    glm::vec3(-dim, 0.0, dim),
    glm::vec3(dim, 0.0, dim),
    glm::vec3(-dim, 0.0, -dim),
    glm::vec3(-dim, 0.0, -dim),
    glm::vec3(dim, 0.0, dim),
    glm::vec3(dim, 0.0, -dim)
  }; 

  // VBO amb la normal de cada vèrtex
  glm::vec3 norm1 (0,1,0);
  glm::vec3 normterra[6] = {
    norm1, norm1, norm1, norm1, norm1, norm1
  };

  // Definim el material del terra
  glm::vec3 amb(0.000000, 0.000000 ,0.100000);
  glm::vec3 diff(87/255.0, 70/255.0,45/255.0);
  glm::vec3 spec(1.0,1.0,1.0);
  float shin = 200;

  // Fem que aquest material afecti a tots els vèrtexs per igual
  glm::vec3 matamb[6] = {
    amb, amb, amb, amb, amb, amb
  };
  glm::vec3 matdiff[6] = {
    diff, diff, diff, diff, diff
  };
  glm::vec3 matspec[6] = {
    spec, spec, spec, spec, spec, spec
  };
  float matshin[6] = {
    shin, shin, shin, shin, shin, shin
  };

// Creació del Vertex Array Object del terra
  glGenVertexArrays(1, &VAO_Terra);
  glBindVertexArray(VAO_Terra);

  GLuint VBO_Terra[6];
  glGenBuffers(6, VBO_Terra);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_Terra[0]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(posterra), posterra, GL_STATIC_DRAW);

  // Activem l'atribut vertexLoc
  glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(vertexLoc);

  glBindBuffer(GL_ARRAY_BUFFER, VBO_Terra[1]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(normterra), normterra, GL_STATIC_DRAW);

  // Activem l'atribut normalLoc
  glVertexAttribPointer(normalLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(normalLoc);

  // Buffer de component ambient
  glBindBuffer(GL_ARRAY_BUFFER, VBO_Terra[2]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(matamb), matamb, GL_STATIC_DRAW);

  glVertexAttribPointer(matambLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(matambLoc);

  // Buffer de component difusa
  glBindBuffer(GL_ARRAY_BUFFER, VBO_Terra[3]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(matdiff), matdiff, GL_STATIC_DRAW);

  glVertexAttribPointer(matdiffLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(matdiffLoc);

  // Buffer de component especular
  glBindBuffer(GL_ARRAY_BUFFER, VBO_Terra[4]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(matspec), matspec, GL_STATIC_DRAW);

  glVertexAttribPointer(matspecLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(matspecLoc);

  // Buffer de component shininness
  glBindBuffer(GL_ARRAY_BUFFER, VBO_Terra[5]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(matshin), matshin, GL_STATIC_DRAW);

  glVertexAttribPointer(matshinLoc, 1, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(matshinLoc);

  glBindVertexArray(0);
}

void MyGLWidget::carregaShaders()
{
  // Creem els shaders per al fragment shader i el vertex shader
  QOpenGLShader fs (QOpenGLShader::Fragment, this);
  QOpenGLShader vs (QOpenGLShader::Vertex, this);
  // Carreguem el codi dels fitxers i els compilem
  fs.compileSourceFile("./shaders/basicLlumShader.frag");
  vs.compileSourceFile("./shaders/basicLlumShader.vert");
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
  // Obtenim identificador per a l'atribut “normal” del vertex shader
  normalLoc = glGetAttribLocation (program->programId(), "normal");
  // Obtenim identificador per a l'atribut “matamb” del vertex shader
  matambLoc = glGetAttribLocation (program->programId(), "matamb");
  // Obtenim identificador per a l'atribut “matdiff” del vertex shader
  matdiffLoc = glGetAttribLocation (program->programId(), "matdiff");
  // Obtenim identificador per a l'atribut “matspec” del vertex shader
  matspecLoc = glGetAttribLocation (program->programId(), "matspec");
  // Obtenim identificador per a l'atribut “matshin” del vertex shader
  matshinLoc = glGetAttribLocation (program->programId(), "matshin");

  // Demanem identificadors per als uniforms del vertex shader
  transLoc = glGetUniformLocation (program->programId(), "TG");
  projLoc = glGetUniformLocation (program->programId(), "proj");
  viewLoc = glGetUniformLocation (program->programId(), "view");


  colFocusLoc = glGetUniformLocation (program->programId(), "colFocus");
  //posFocusLoc = glGetUniformLocation (program->programId(), "posFocus"), 3, glm::value_ptr(posFocus[0]);
  llumLoc = glGetUniformLocation (program->programId(), "llumAmbient");
  posFocusLoc = glGetUniformLocation (program->programId(), "posFocus");
  posFocusLoc2 = glGetUniformLocation (program->programId(), "posFocus2");
  posFocusLoc3 = glGetUniformLocation (program->programId(), "posFocus3");

  posLlumReactorLoc = glGetUniformLocation (program->programId(), "posLlumReactor");
  colorLlumReactorLoc = glGetUniformLocation (program->programId(), "colorLlumReactor");
}

