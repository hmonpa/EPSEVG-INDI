#define GLM_FORCE_RADIANS
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLWidget>
#include <QOpenGLShader>
#include <QOpenGLShaderProgram>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QTimer>
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
    void creaBuffersModel(Model &model, const char *fileName, GLuint *VAO, float &escala, glm::vec3 &centreBase);
    void creaBuffersLegoman ();
    void creaBuffersPilota ();
    void creaBuffersCub ();
    void creaBuffersTerra ();
    void carregaShaders ();
    void iniEscena ();
    void iniCamera ();
    void projectTransform ();
    void viewTransform ();
    void modelTransformIdent ();
    void modelTransformModel ();
    void modelTransformPilota ();
    void calculaCapsaModel (Model &p, float &escala, glm::vec3 &CentreBase);

    // VAO i VBO names
    GLuint VAO_Lego, VAO_Pil, VAO_Cub, VAO_Terra;
    // Program
    QOpenGLShaderProgram *program;
    // Viewport
    GLint ample, alt;
    // uniform locations
    GLuint transLoc, projLoc, viewLoc;
    // attribute locations
    GLuint vertexLoc, normalLoc, matambLoc, matdiffLoc, matspecLoc, matshinLoc;

    // model
    Model Lego, pil;
    // paràmetres calculats a partir de la capsa contenidora del model
    glm::vec3 centreBaseModel, centreBasePil;
    float escalaModel, escalaPil;
    // radi de l'escena
    float radiEsc;
    glm::vec3 posPilota;

    typedef  enum {NONE, ROTATE} InteractiveAction;
    InteractiveAction DoingInteractive;
    int xClick, yClick;
    float angleY, angleX;

    glm::vec3 posNinot;

    float escalarModel (const Model &m, float f);
    bool legoman;
    glm::vec3 centreEsc;
    void calculaRadiEsc();

    void modelTransformParet1();
    void modelTransformParet2();
    void modelTransformParet3();

    float FOV, FOV_orig, ra;
    bool primerapersona;
    int value;

public slots:
    void canviaCamera();

signals:
    void signalCamera(int value);
};

