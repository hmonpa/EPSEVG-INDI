#define GLM_FORCE_RADIANS
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLWidget>
#include <QOpenGLShader>
#include <QOpenGLShaderProgram>
#include <QKeyEvent>
#include <QMouseEvent>
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "model.h"

class MyGLWidget : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core 
{
  Q_OBJECT

  public:
    MyGLWidget (QWidget *parent=0);
    ~MyGLWidget ();



  protected:
    // initializeGL - Aqui incluim les inicialitzacions del contexte grafic.
    virtual void initializeGL ( );
    // paintGL - Mètode cridat cada cop que cal refrescar la finestra.
    // Tot el que es dibuixa es dibuixa aqui.
    virtual void paintGL ( );
    // resizeGL - És cridat quan canvia la mida del widget
    virtual void resizeGL (int width, int height);
    // keyPressEvent - Es cridat quan es prem una tecla
    virtual void keyPressEvent (QKeyEvent *event);
    // mouse{Press/Release/Move}Event - Són cridades quan es realitza l'event 
    // corresponent de ratolí
    virtual void mousePressEvent (QMouseEvent *event);
    virtual void mouseReleaseEvent (QMouseEvent *event);
    virtual void mouseMoveEvent (QMouseEvent *event);

  private:

    void creaBuffersModel(Model &model, const char *fileName, GLuint *VAO, float &escala, glm::vec3 &centreBase);
    void creaBuffersHangar();
    void creaBuffersPersonatge();
    void creaBuffersTerra ();
    void carregaShaders ();
    void projectTransform ();
    void viewTransform ();
    void modelTransformTerra ();
    void modelTransformHangar ();
    void modelTransformAvio ();
    void calculaCapsaModel (Model &m, float &escala, glm::vec3 &centreBase);
    void iniEscena ();
    void iniCamera ();
    void calculaSCA();


    // VAO names
    GLuint VAO_avio, VAO_hangar, VAO_Terra ;
    // Program
    QOpenGLShaderProgram *program;
    // uniform locations
    GLuint transLoc, projLoc, viewLoc;
    // attribute locations
    GLuint vertexLoc, normalLoc, matambLoc, matdiffLoc, matspecLoc, matshinLoc;
    GLint ample, alt;

    // model
    Model hangarModel, avioModel;
    // paràmetres calculats a partir de la capsa contenidora del model
    glm::vec3 centreBaseAvio, centreBaseHangar;
    float escalaAvio, escalaArc;
    glm::vec3 centreEsc;
    float radiEsc, ra;

    glm::vec3 avioPos;


    typedef  enum {NONE, ROTATE} InteractiveAction;
    InteractiveAction DoingInteractive;
    int xClick, yClick;
    float angleX, angleY;

    glm::mat4 View;

    // Declaració de variables que utilitzo com a uniforms
    GLuint colFocusLoc, posFocusLoc, posFocusLoc2, posFocusLoc3;
    GLuint llumLoc;
    glm::vec3 colFocus, llumAmbient;
    glm::vec3 posFocus, posFocus2, posFocus3;

    glm::vec3 off;
    glm::vec3 posLlumReactor, colorLlumReactor;

    GLuint posLlumReactorLoc, colorLlumReactorLoc;



public slots:
    void controlallums0();
    void controlallums1();
    void controlallums2();
    void controlallums3();

signals:
    void signalPolsa0();
    void signalPolsa1();
    void signalPolsa2();
    void signalPolsa3();

};

