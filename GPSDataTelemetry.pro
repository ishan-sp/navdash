QT       += quick core gui location positioning quickwidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

SOURCES += \
    Led.cpp \
    main.cpp \
    mainwindow.cpp \
    qcustomplot.cpp

HEADERS += \
    Led.h \
    mainwindow.h \
    qcustomplot.h

FORMS += \
    mainwindow.ui

QT += widgets serialport printsupport
QMAKE_CXXFLAGS += -Wa,-mbig-obj

RESOURCES += \
    qml.qrc \
    qml.qrc

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    map.qml
