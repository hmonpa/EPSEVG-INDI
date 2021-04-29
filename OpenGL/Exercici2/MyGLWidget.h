#define GLM_FORCE_RADIANS
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLWidget>
#include <QOpenGLShader>
#include <QOpenGLShaderProgram>
#include <QKeyEvent>
#include "glm/glm.hpp"
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
    void creaBuffersTerra ();
    void creaBuffersCub ();
    void creaBuffersHomer ();
    void creaBuffersPatricio ();
    void carregaShaders ();
    void projectTransform ();
    void viewTransform ();
    void modelTransformTerra ();
    void modelTransformParet ();
    void modelTransformHomer ();
    void calculaCapsaModel (Model &p, float &escala, glm::vec3 &centreBase);
    void iniEscena ();
    void iniCamera ();

    void modelTransformPatri1();
    void modelTransformPatri2();



    // VAO names
    GLuint VAO_Cub, VAO_Terra, VAO_Homer;
    GLuint VAO_Patri;

    // Program
    QOpenGLShaderProgram *program;
    // uniform locations
    GLuint transLoc, projLoc, viewLoc;
    // attribute locations
    GLuint vertexLoc, colorLoc;

    GLint ample, alt;

    // model
    Model homer;
    Model patri;

    // paràmetres calculats a partir de la capsa contenidora del model
    glm::vec3 posHomer, centreBaseHomer;
    glm::vec3 posPatri, centreBasePatri;
    float escalaPatri;

    glm::vec3 centreEsc, obs, vrp, up;
    float radiEsc, ra, fov, zn, zf;
    float fov_orig;
    float dist;
    float angleX, angleY;
    float rotacio1 = M_PI/180*45, rotacio2 = M_PI/180*45;

    typedef  enum {NONE, ROTATE} InteractiveAction;
    InteractiveAction DoingInteractive;
    int xClick, yClick;

    float antigaX, antigaY;
    float tetha, phi;

    bool canvicamera;

public slots:
    void torna_inici();
    void giraPatri1();
    void giraPatri2();
};

