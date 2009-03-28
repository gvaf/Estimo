CONFIG += assistant
TEMPLATE = app
TARGET = sharpeye
DEPENDPATH += . estimo
INCLUDEPATH += . estimo

unix:OBJECTS_DIR = obj
unix:MOC_DIR = obj

SUBDIRS = estimo

# Input
HEADERS += estimohighlight.h \
           projecttree.h \
           sharpeye.h \
           sourcebrowser.h \
           sourceview.h \
           patternview.h \
           printpreview.h \
           timedelay.h \
           settings.h

SOURCES += estimohighlight.cpp \
           main.cpp \
           projecttree.cpp \
           sharpeye.cpp \
           sourcebrowser.cpp \
           sourceview.cpp \
           patternview.cpp \
           printpreview.cpp \
           timedelay.cpp \
           settings.cpp

RESOURCES += sharpeye.qrc

RC_FILE = sharpeye.rc