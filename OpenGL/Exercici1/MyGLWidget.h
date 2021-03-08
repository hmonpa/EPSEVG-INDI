#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLWidget>
#include <QOpenGLShader>
#include <QOpenGLShaderProgram>
#include <QKeyEvent>
#include "glm/glm.hpp"

#define MUNTANYA_NUM_VERTEXS 8

class MyGLWidget : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core
{
  Q_OBJECT

  public:
    MyGLWidget (QWidget *parent=0);
    ~MyGLWidget ();
    
  protected:
    // initializeGL - Aqui incluim les inicialitzacions del contexte grafic.
    virtual void initializeGL ();

    // paintGL - Mètode cridat cada cop que cal refrescar la finestra.
    // Tot el que es dibuixa es dibuixa aqui.
    virtual void paintGL ();
 
    // resize - Es cridat quan canvia la mida del widget
    virtual void resizeGL (int width, int height);  

    // keyPressEvent - Es cridat quan es prem una tecla
     virtual void keyPressEvent (QKeyEvent *event);

  private:
    //-------------------------------------------------
    // Funcions generals de pintat per objectes
    void pintaMuntanyes();
    void pintaGronxador();
    void pintaPesos();
    void pintaBaseGronxador();
    //-------------------------------------------------
    //  Creació de buffers
    void creaBuffers ();    
    void creaBufferMuntanya();
    void creaBufferTriangle();
    void creaBufferQuadrat();
    //-------------------------------------------------
    // Funcions de transformació pels objectes
    void transformacioMuntanya(double h, double xPos, float transparencia);
    void transformacioGronxador();
    void transformacioPes(int left);
    void transformacioBaseGronxador();
    //-------------------------------------------------
    void carregaShaders ();

    //-------------------------------------------------
    // attribute locations
    GLuint vertexLoc, colorLoc;
    // uniforms
    GLuint transLoc;
    GLuint transparenciaLoc;
    
    // Program
    QOpenGLShaderProgram *program;

    GLuint VAO1, VAO_MUNTANYA, VAO_QUADRAT, VAO_TRIANGLE;
    GLint ample, alt;

    //status
    float xPrimeraMuntanya,xSegonaMuntanya;

    float anglePalanca;
};
