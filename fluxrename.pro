QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    src/fscanner.cpp \
    src/fscannerworker.cpp \
    src/ui/configurewidget.cpp \
    src/ui/filterwidget.cpp \
    src/ui/fwidgetbase.cpp \
    src/ui/inputwidget.cpp \
    src/main.cpp \
    src/ui/mainwindow.cpp

HEADERS += \
    src/fscanner.h \
    src/fscannerworker.h \
    src/global.h \
    src/ui/configurewidget.h \
    src/ui/filterwidget.h \
    src/ui/fwidgetbase.h \
    src/ui/inputwidget.h \
    src/ui/mainwindow.h

FORMS += \
    src/forms/configurewidget.ui \
    src/forms/filterwidget.ui \
    src/forms/inputwidget.ui \
    src/forms/mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
