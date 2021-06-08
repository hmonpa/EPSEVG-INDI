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

void MyGLWidget::iniEscena () // Cal modificar aquest mètode...
{
  creaBuffersTerra();
  creaBuffersCub();
  creaBuffersCub2();
  creaBuffersCotxe();
  
  //radiEsc = 7;
  calculaRadiEsc();
  focusEsc = false;
  posFocus = glm::vec3(1,1,1);
  posPorta = glm::vec3(-5, 0, 0);
  posCotxe = glm::vec3(0,0,0);
  posCotxeIni = glm::vec3(0,0,0);
  glUniform3fv(posFocusLoc, 1, &posFocus[0]);
}

void MyGLWidget::iniCamera () // Cal modificar aquest mètode...
{
  angleY = 0.5;
  angleX = 0.5;
  ra = 1.0;
  FOVini = M_PI/3.0;
  fov = FOVini;
  zn = radiEsc;
  zf = 3.0*radiEsc;

  if (ra < 1.0) fov = 2.0 * atan(tan(0.5*FOVini)/ra);
  else fov = FOVini;

  projectTransform ();
  viewTransform ();
}

void MyGLWidget::paintGL () // Cal modificar aquest mètode...
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

  // Activem el VAO per a pintar el terra 
  glBindVertexArray (VAO_Terra);
  modelTransformTerra ();
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

  // Activem el VAO per a pintar la Porta
  glBindVertexArray (VAO_Cub2);
  modelTransformPorta ();
  glDrawArrays(GL_TRIANGLES, 0, 36);

  // Activem el VAO per a pintar la Paret
  glBindVertexArray (VAO_Cub);
  modelTransformParet1 ();
  glDrawArrays(GL_TRIANGLES, 0, 36);
  modelTransformParet2 ();
  glDrawArrays(GL_TRIANGLES, 0, 36);
  modelTransformParet3 ();
  glDrawArrays(GL_TRIANGLES, 0, 36);

  // Activem el VAO per a pintar el Patricio
  glBindVertexArray (VAO_Cotxe);
  modelTransformCotxe ();
  glDrawArrays(GL_TRIANGLES, 0, cotxe.faces().size()*3);
  
  glBindVertexArray(0);
}

void MyGLWidget::resizeGL (int w, int h) 
{
  ample = w;
  alt = h;

  ra = float(ample)/float(alt);

  if (ra < 1.0) fov = 2.0 * atan(tan(0.5*FOVini)/ra);
  else fov = FOVini;

  projectTransform ();
}

void MyGLWidget::calculaRadiEsc()
{
    glm::vec3 Pmax;
    glm::vec3 Pmin;
    float modX, modY, modZ;
    Pmax.x = 15, Pmax.y = 3, Pmax.z = 10;
    Pmin.x = -15, Pmin.y = 0, Pmin.z = -10;

    centreEsc[0] = (Pmax.x+Pmin.x)/2;
    centreEsc[1] = (Pmax.y+Pmin.y)/2;
    centreEsc[2] = (Pmax.z+Pmin.z)/2;

    modX = pow(Pmax.x - centreEsc.x, 2);
    modY = pow(Pmax.y - centreEsc.y, 2);
    modZ = pow(Pmax.z - centreEsc.z, 2);

    radiEsc = sqrt(modX + modY + modZ);
}


void MyGLWidget::modelTransformCotxe()
{
  glm::mat4 TG(1.f);  // Matriu de transformació
  TG = glm::translate(TG, posCotxe);
  TG = glm::scale(TG, glm::vec3(escalaCotx, escalaCotx, escalaCotx));
  TG = glm::rotate(TG, float(M_PI/2), glm::vec3(0, 1, 0));
  TG = glm::translate(TG, -centreBaseCotx);
    
  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}


void MyGLWidget::modelTransformPorta()  // Cal modificar aquest mètode...
{
  glm::mat4 TG(1.f);  // Matriu de transformació
  TG = glm::translate(TG, posPorta);
  TG = glm::scale(TG, glm::vec3(0.3*2, 5*2, 10*2));             // Els cubs son 0.5 originalment, s'escalen al doble porta i parets
  
  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}


void MyGLWidget::modelTransformParet1()  // Cal modificar aquest mètode...
{
  glm::mat4 TG(1.f);  // Matriu de transformació
  TG = glm::translate(TG, glm::vec3(-15, 0, 0));
  TG = glm::scale(TG, glm::vec3(0.5*2, 5*2, 10*2));
  //TG = glm::translate(TG, glm::vec3(2,0,0));
  
  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}


void MyGLWidget::modelTransformParet2()  // Cal modificar aquest mètode...
{
    glm::mat4 TG(1.f);  // Matriu de transformació
    TG = glm::translate(TG, glm::vec3(-10, 0, 5));
    TG = glm::scale(TG, glm::vec3(10*2, 5*2, 0.5*2));
    //TG = glm::translate(TG, glm::vec3(2,0,0));

    glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}


void MyGLWidget::modelTransformParet3()  // Cal modificar aquest mètode...
{
    glm::mat4 TG(1.f);  // Matriu de transformació
    TG = glm::translate(TG, glm::vec3(-10, 0, -5));
    TG = glm::scale(TG, glm::vec3(10*2, 5*2, 0.5*2));

    glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::modelTransformTerra ()  // Cal modificar aquest mètode...
{
  glm::mat4 TG(1.f);  // Matriu de transformació
  TG = glm::translate(TG, glm::vec3(-15,0,-10));
  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::projectTransform ()   // Cal modificar aquest mètode...
{
  glm::mat4 Proj;  // Matriu de projecció
  Proj = glm::perspective(fov, ra, radiEsc, 3.0f*radiEsc);
  glUniformMatrix4fv (projLoc, 1, GL_FALSE, &Proj[0][0]);
}

void MyGLWidget::viewTransform ()  // Cal modificar aquest mètode...
{
  glm::mat4 View;  // Matriu de posició i orientació
  View = glm::translate(glm::mat4(1.f), glm::vec3(0, 0, -2*radiEsc));
  View = glm::rotate(View, angleY, glm::vec3(0, 1, 0));
  View = glm::rotate(View, angleX, glm::vec3(1, 0, 0));
  glUniformMatrix4fv (viewLoc, 1, GL_FALSE, &View[0][0]);
}

void MyGLWidget::keyPressEvent(QKeyEvent* event)  // Cal modificar aquest mètode...
{
  makeCurrent();
  switch (event->key()) {
    case Qt::Key_Up: {
        if (posCotxe.x < 10)
        {
            if (posCotxe.x < -8.5)
            {
                if (posPorta.y >= 2.25)
                {
                    posCotxe.x += 0.5;
                    actFocus();
                }
            }
            else
            {
                posCotxe.x += 0.5;
                actFocus();
            }
        }
	  break;
	}
    case Qt::Key_Down: {
        if (posCotxe.x > -11)
        {
            if (posCotxe.x < -1)
            {
                if (posPorta.y >= 2.25)
                {
                    posCotxe.x -= 0.5;
                    actFocus();
                }
            }
            else
            {
                posCotxe.x -= 0.5;
                actFocus();
            }
        }


	  break;
	}
    case Qt::Key_U: {
      if (posPorta.y < 5) posPorta.y += 0.25;
	  break;
	}
    case Qt::Key_D: {
      if (posPorta.y > 0) posPorta.y -= 0.25;
	  break;
	}
	case Qt::Key_I: {
      posCotxe = posCotxeIni;
      focusEsc = false;
      actFocus();
      modelTransformCotxe();
      emit signalCam();
      escullFocusCam();
	  break;
	}
  case Qt::Key_F: {
    focusEsc = !focusEsc;
    if (focusEsc) emit signalEsc();
    else emit signalCam();
    actFocus();
    break;
  }
    default: 
      event->ignore(); 
      break;
  }

  update();
}

void MyGLWidget::escullFocusEsc()
{
    makeCurrent();
    focusEsc = true;
    actFocus();
    update();
}

void MyGLWidget::escullFocusCam()
{
    makeCurrent();
    focusEsc = false;
    actFocus();
    update();
}


void MyGLWidget::actFocus()
{
    if (focusEsc) posFocus = glm::vec3(posCotxe.x, 15, posCotxe.z);
    else posFocus = glm::vec3(1,1,1);
    glUniform3fv(posFocusLoc, 1, &posFocus[0]);
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
  // Aqui cal que es calculi i s'apliqui la rotacio o el zoom com s'escaigui...
  if (DoingInteractive == ROTATE)
  {
    // Fem la rotació
    angleY += (e->x() - xClick) * M_PI / 180.0;
    angleX += (e->y() - yClick) * M_PI / 180.0;
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
      minx = p.vertices()[i];
    if (p.vertices()[i+0] > maxx)
      maxx = p.vertices()[i];
    if (p.vertices()[i+1] < miny)
      miny = p.vertices()[i+1];
    if (p.vertices()[i+1] > maxy)
      maxy = p.vertices()[i+1];
    if (p.vertices()[i+2] < minz)
      minz = p.vertices()[i+2];
    if (p.vertices()[i+2] > maxz)
      maxz = p.vertices()[i+2];
  }
  escala = 2.0/(maxy-miny);                 // canvia a l'escala uniforma, ja que el cotxe es l'unic model carregat
  centreBase = glm::vec3((minx+maxx)/2.0, miny, (minz+maxz)/2.0);
}

void MyGLWidget::creaBuffersCub ()
{
  // Dades del cub
  // Vèrtexs del cub
  glm::vec3 vertexs[8] = {
       /* 0*/ glm::vec3( -0.25, 0.0, -0.25),  /* 1*/ glm::vec3( 0.25, 0.0, -0.25),
       /* 2*/ glm::vec3( -0.25, 0.5, -0.25),  /* 3*/ glm::vec3( 0.25, 0.5, -0.25),
       /* 4*/ glm::vec3( -0.25, 0.0, 0.25),  /* 5*/ glm::vec3( 0.25, 0.0, 0.25),
       /* 6*/ glm::vec3( -0.25, 0.5, 0.25),  /* 7*/ glm::vec3( 0.25, 0.5, 0.25)
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

  // VBO amb la normal de cada vèrtex
  glm::vec3 normals[6] = {
       /* 0*/ glm::vec3( 1.0, 0.0,  0.0),  /* 1*/ glm::vec3( -1.0, 0.0, 0.0),
       /* 2*/ glm::vec3( 0.0, 1.0,  0.0),  /* 3*/ glm::vec3( 0.0, -1.0, 0.0),
       /* 4*/ glm::vec3( 0.0, 0.0,  1.0),  /* 5*/ glm::vec3( 0.0, 0.0, -1.0)
  };
  glm::vec3 normcub[36] = {
       normals[5], normals[5], normals[5],
       normals[5], normals[5], normals[5],
       normals[0], normals[0], normals[0],
       normals[0], normals[0], normals[0],
       normals[2], normals[2], normals[2],
       normals[2], normals[2], normals[2],
       normals[1], normals[1], normals[1],
       normals[1], normals[1], normals[1],
       normals[3], normals[3], normals[3],
       normals[3], normals[3], normals[3],
       normals[4], normals[4], normals[4],
       normals[4], normals[4], normals[4]
  };

  // inicialitzem el material del cub
  glm::vec3 amb(0.3,0.2,0.1), diff(0.6,0.2,0.1), spec(0.3,0.3,0.3);
  float shin = 0;
  
  // Fem que aquest material afecti a tots els vèrtexs per igual
  glm::vec3 matambcub[36] = {
	amb, amb, amb, amb, amb, amb,
	amb, amb, amb, amb, amb, amb,
	amb, amb, amb, amb, amb, amb,
	amb, amb, amb, amb, amb, amb,
	amb, amb, amb, amb, amb, amb,
	amb, amb, amb, amb, amb, amb
  };
  glm::vec3 matdiffcub[36] = {
	diff, diff, diff, diff, diff, diff,
	diff, diff, diff, diff, diff, diff,
	diff, diff, diff, diff, diff, diff,
	diff, diff, diff, diff, diff, diff,
	diff, diff, diff, diff, diff, diff,
	diff, diff, diff, diff, diff, diff
  };
  glm::vec3 matspeccub[36] = {
	spec, spec, spec, spec, spec, spec,
	spec, spec, spec, spec, spec, spec,
	spec, spec, spec, spec, spec, spec,
	spec, spec, spec, spec, spec, spec,
	spec, spec, spec, spec, spec, spec,
	spec, spec, spec, spec, spec, spec
  };
  float matshincub[36] = {
	shin, shin, shin, shin, shin, shin,
	shin, shin, shin, shin, shin, shin,
	shin, shin, shin, shin, shin, shin,
	shin, shin, shin, shin, shin, shin,
	shin, shin, shin, shin, shin, shin,
	shin, shin, shin, shin, shin, shin
  };

// Creació del Vertex Array Object del cub
  glGenVertexArrays(1, &VAO_Cub);
  glBindVertexArray(VAO_Cub);

  GLuint VBO_Cub[6];
  glGenBuffers(6, VBO_Cub);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_Cub[0]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(poscub), poscub, GL_STATIC_DRAW);

  // Activem l'atribut vertexLoc
  glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(vertexLoc);

  glBindBuffer(GL_ARRAY_BUFFER, VBO_Cub[1]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(normcub), normcub, GL_STATIC_DRAW);

  // Activem l'atribut normalLoc
  glVertexAttribPointer(normalLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(normalLoc);

  // En lloc del color, ara passem tots els paràmetres dels materials
  // Buffer de component ambient
  glBindBuffer(GL_ARRAY_BUFFER, VBO_Cub[2]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(matambcub), matambcub, GL_STATIC_DRAW);

  glVertexAttribPointer(matambLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(matambLoc);

  // Buffer de component difusa
  glBindBuffer(GL_ARRAY_BUFFER, VBO_Cub[3]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(matdiffcub), matdiffcub, GL_STATIC_DRAW);

  glVertexAttribPointer(matdiffLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(matdiffLoc);

  // Buffer de component especular
  glBindBuffer(GL_ARRAY_BUFFER, VBO_Cub[4]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(matspeccub), matspeccub, GL_STATIC_DRAW);

  glVertexAttribPointer(matspecLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(matspecLoc);

  // Buffer de component shininness
  glBindBuffer(GL_ARRAY_BUFFER, VBO_Cub[5]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(matshincub), matshincub, GL_STATIC_DRAW);

  glVertexAttribPointer(matshinLoc, 1, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(matshinLoc);

  glBindVertexArray(0);
}

void MyGLWidget::creaBuffersCub2 ()
{
  // Dades del cub
  // Vèrtexs del cub
  glm::vec3 vertexs[8] = {
       /* 0*/ glm::vec3( -0.25, 0.0, -0.25),  /* 1*/ glm::vec3( 0.25, 0.0, -0.25),
       /* 2*/ glm::vec3( -0.25, 0.5, -0.25),  /* 3*/ glm::vec3( 0.25, 0.5, -0.25),
       /* 4*/ glm::vec3( -0.25, 0.0, 0.25),  /* 5*/ glm::vec3( 0.25, 0.0, 0.25),
       /* 6*/ glm::vec3( -0.25, 0.5, 0.25),  /* 7*/ glm::vec3( 0.25, 0.5, 0.25)
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

  // VBO amb la normal de cada vèrtex
  glm::vec3 normals[6] = {
       /* 0*/ glm::vec3( 1.0, 0.0,  0.0),  /* 1*/ glm::vec3( -1.0, 0.0, 0.0),
       /* 2*/ glm::vec3( 0.0, 1.0,  0.0),  /* 3*/ glm::vec3( 0.0, -1.0, 0.0),
       /* 4*/ glm::vec3( 0.0, 0.0,  1.0),  /* 5*/ glm::vec3( 0.0, 0.0, -1.0)
  };
  glm::vec3 normcub[36] = {
       normals[5], normals[5], normals[5],
       normals[5], normals[5], normals[5],
       normals[0], normals[0], normals[0],
       normals[0], normals[0], normals[0],
       normals[2], normals[2], normals[2],
       normals[2], normals[2], normals[2],
       normals[1], normals[1], normals[1],
       normals[1], normals[1], normals[1],
       normals[3], normals[3], normals[3],
       normals[3], normals[3], normals[3],
       normals[4], normals[4], normals[4],
       normals[4], normals[4], normals[4]
  };

  // inicialitzem el material del cub
  glm::vec3 amb(0.1,0.1,0.1), diff(0.3,0.3,0.3), spec(0.9,0.9,0.9);
  float shin = 2;
  
  // Fem que aquest material afecti a tots els vèrtexs per igual
  glm::vec3 matambcub[36] = {
	amb, amb, amb, amb, amb, amb,
	amb, amb, amb, amb, amb, amb,
	amb, amb, amb, amb, amb, amb,
	amb, amb, amb, amb, amb, amb,
	amb, amb, amb, amb, amb, amb,
	amb, amb, amb, amb, amb, amb
  };
  glm::vec3 matdiffcub[36] = {
	diff, diff, diff, diff, diff, diff,
	diff, diff, diff, diff, diff, diff,
	diff, diff, diff, diff, diff, diff,
	diff, diff, diff, diff, diff, diff,
	diff, diff, diff, diff, diff, diff,
	diff, diff, diff, diff, diff, diff
  };
  glm::vec3 matspeccub[36] = {
	spec, spec, spec, spec, spec, spec,
	spec, spec, spec, spec, spec, spec,
	spec, spec, spec, spec, spec, spec,
	spec, spec, spec, spec, spec, spec,
	spec, spec, spec, spec, spec, spec,
	spec, spec, spec, spec, spec, spec
  };
  float matshincub[36] = {
	shin, shin, shin, shin, shin, shin,
	shin, shin, shin, shin, shin, shin,
	shin, shin, shin, shin, shin, shin,
	shin, shin, shin, shin, shin, shin,
	shin, shin, shin, shin, shin, shin,
	shin, shin, shin, shin, shin, shin
  };

// Creació del Vertex Array Object del cub
  glGenVertexArrays(1, &VAO_Cub2);
  glBindVertexArray(VAO_Cub2);

  GLuint VBO_Cub[6];
  glGenBuffers(6, VBO_Cub);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_Cub[0]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(poscub), poscub, GL_STATIC_DRAW);

  // Activem l'atribut vertexLoc
  glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(vertexLoc);

  glBindBuffer(GL_ARRAY_BUFFER, VBO_Cub[1]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(normcub), normcub, GL_STATIC_DRAW);

  // Activem l'atribut normalLoc
  glVertexAttribPointer(normalLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(normalLoc);

  // En lloc del color, ara passem tots els paràmetres dels materials
  // Buffer de component ambient
  glBindBuffer(GL_ARRAY_BUFFER, VBO_Cub[2]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(matambcub), matambcub, GL_STATIC_DRAW);

  glVertexAttribPointer(matambLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(matambLoc);

  // Buffer de component difusa
  glBindBuffer(GL_ARRAY_BUFFER, VBO_Cub[3]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(matdiffcub), matdiffcub, GL_STATIC_DRAW);

  glVertexAttribPointer(matdiffLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(matdiffLoc);

  // Buffer de component especular
  glBindBuffer(GL_ARRAY_BUFFER, VBO_Cub[4]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(matspeccub), matspeccub, GL_STATIC_DRAW);

  glVertexAttribPointer(matspecLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(matspecLoc);

  // Buffer de component shininness
  glBindBuffer(GL_ARRAY_BUFFER, VBO_Cub[5]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(matshincub), matshincub, GL_STATIC_DRAW);

  glVertexAttribPointer(matshinLoc, 1, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(matshinLoc);

  glBindVertexArray(0);
}

void MyGLWidget::creaBuffersCotxe ()
{
  // Carreguem el model de l'OBJ - Atenció! Abans de crear els buffers!
  cotxe.load("./models/porsche.obj");

  // Calculem la capsa contenidora del model
  calculaCapsaModel (cotxe, escalaCotx, centreBaseCotx);
  
  // Creació del Vertex Array Object del Patricio
  glGenVertexArrays(1, &VAO_Cotxe);
  glBindVertexArray(VAO_Cotxe);

  // Creació dels buffers del model patr
  GLuint VBO[6];
  glGenBuffers(6, VBO);

  // Buffer de posicions
  glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*cotxe.faces().size()*3*3, cotxe.VBO_vertices(), GL_STATIC_DRAW);

  glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(vertexLoc);

  // Buffers normals
  glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*cotxe.faces().size()*3*3, cotxe.VBO_normals(), GL_STATIC_DRAW);

  glVertexAttribPointer(normalLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(normalLoc);

  // En lloc del color, ara passem tots els paràmetres dels materials
  // Buffer de component ambient
  glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*cotxe.faces().size()*3*3, cotxe.VBO_matamb(),  GL_STATIC_DRAW);

  glVertexAttribPointer(matambLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(matambLoc);

  // Buffer de component difusa
  glBindBuffer(GL_ARRAY_BUFFER, VBO[3]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*cotxe.faces().size()*3*3, cotxe.VBO_matdiff(), GL_STATIC_DRAW);

  glVertexAttribPointer(matdiffLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(matdiffLoc);

  // Buffer de component especular
  glBindBuffer(GL_ARRAY_BUFFER, VBO[4]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*cotxe.faces().size()*3*3, cotxe.VBO_matspec(), GL_STATIC_DRAW);

  glVertexAttribPointer(matspecLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(matspecLoc);

  // Buffer de component shininness
  glBindBuffer(GL_ARRAY_BUFFER, VBO[5]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*cotxe.faces().size()*3, cotxe.VBO_matshin(), GL_STATIC_DRAW);

  glVertexAttribPointer(matshinLoc, 1, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(matshinLoc);

  glBindVertexArray(0);
}

void MyGLWidget::creaBuffersTerra ()
{
  // VBO amb la posició dels vèrtexs
  glm::vec3 posterra[] = {
	glm::vec3(0.0, 0.0, 20.0),
	glm::vec3(30.0, 0.0, 20.0),
	glm::vec3(0.0, 0.0, 0.0),
	glm::vec3(30.0, 0.0, 0.0),
  }; 

  // VBO amb la normal de cada vèrtex
  glm::vec3 norm (0,1,0);
  glm::vec3 normterra[6] = {
	norm, norm, norm, norm
  };

  // Definim el material del terra
  glm::vec3 amb(0.1,0.15,0.1);
  glm::vec3 diff(0.3,0.5,0.3);
  glm::vec3 spec(1,1,1);
  float shin = 500;

  // Fem que aquest material afecti a tots els vèrtexs per igual
  glm::vec3 matamb[] = {
	amb, amb, amb, amb
  };
  glm::vec3 matdiff[6] = {
	diff, diff, diff, diff
  };
  glm::vec3 matspec[6] = {
	spec, spec, spec, spec
  };
  float matshin[6] = {
	shin, shin, shin, shin
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
  fs.compileSourceFile("shaders/basicLlumShader.frag");
  vs.compileSourceFile("shaders/basicLlumShader.vert");
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

  // Demanem identificadors per als uniforms dels shaders
  transLoc = glGetUniformLocation (program->programId(), "TG");
  projLoc = glGetUniformLocation (program->programId(), "proj");
  viewLoc = glGetUniformLocation (program->programId(), "view");

  posFocusLoc = glGetUniformLocation (program->programId(), "posFocus");
}

