#define GLM_FORCE_RADIANS
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLWidget>
#include <QOpenGLShader>
#include <QOpenGLShaderProgram>
#include <QKeyEvent>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "/home/hector/Escritorio/INDI/OpenGL/Bloc2/models/Model/model.h"

#include <QMouseEvent>

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

  private:
    void creaBuffers ();
    void carregaShaders ();
    void modelTransform ();
    void projectTransform();
    void viewTransform();
    void modelTransformTerra();
    void ini_camera();
    void pintaPatri();
    void pintaTerra();
    void carregaTerra();
    void carregaPatri();

    void calculaCapsaCont();
    void calculaRadiEscena();

    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);

    typedef enum {ROTATE, NONE, ZOOM} Action;
    Action CurrentAction;

    void modelTransform2();
    void modelTransform3();
    void pintaPatri2();
    void pintaPatri3();

    // attribute locations
    GLuint vertexLoc, colorLoc;
    // uniform locations
    GLuint transLoc, projLoc, viewLoc;
    // VAO i VBO names
    GLuint VAO_patri, VBO_patri, VBO_patri2;    // El primer VBO es para los vértices y el segundo para colores
    GLuint VAO_terra, VBO_terra, VBO_terra2;
    // Program
    QOpenGLShaderProgram *program;
    // Viewport
    GLint ample, alt;
    // Internal vars
    float scale;
    glm::vec3 pos;

    Model homer;
    Model patri;
    float rota = M_PI/2;

    // Perspectiva
    float FOV, ra, raV, alphaV, zN, zF;
    float FOVorig;
    // Ortogonal
    float left, bottom, right, top;

    glm::vec3 OBS, VRP, UP;
    glm::vec3 Pmin, Pmax, PMIN, PMAX,  centre;
    float radi;
    float dist;
    bool decideix=true; // Perspectiva=true / Ortgonal=false

    float antigaX, antigaY;
    float tetha = 1.0, phi = 1.0;
};

