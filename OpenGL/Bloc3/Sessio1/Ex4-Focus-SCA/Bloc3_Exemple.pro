TEMPLATE    = app
QT         += opengl

INCLUDEPATH +=  /usr/include/glm
INCLUDEPATH += /home/hector/Escritorio/INDI/OpenGL/Bloc3/models/Model

FORMS += MyForm.ui

HEADERS += MyForm.h MyGLWidget.h

SOURCES += main.cpp  MyForm.cpp \
        MyGLWidget.cpp /home/hector/Escritorio/INDI/OpenGL/Bloc3/models/Model/model.cpp

DISTFILES += \
    shaders/basicLlumShader.frag \
    shaders/basicLlumShader.vert
