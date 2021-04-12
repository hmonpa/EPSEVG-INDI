TEMPLATE    = app
QT         += opengl 

INCLUDEPATH +=  /usr/include/glm

# Adicció includepath directori Model
INCLUDEPATH += /home/hector/Escritorio/INDI/OpenGL/Bloc2/models/Model

FORMS += MyForm.ui

HEADERS += MyForm.h MyGLWidget.h

SOURCES += main.cpp MyForm.cpp \
        MyGLWidget.cpp 

# Adicció source model.cpp
SOURCES += /home/hector/Escritorio/INDI/OpenGL/Bloc2/models/Model/model.cpp

DISTFILES += \
    shaders/basicShader.frag \
    shaders/basicShader.vert
