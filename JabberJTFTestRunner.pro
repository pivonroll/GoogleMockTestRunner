#-------------------------------------------------
#
# Project created by QtCreator 2014-09-24T14:56:03
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = JabberJTFTestRunner
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    jtftestparser.cpp \
    jtftestscanner.cpp \
    jtftestscannerworker.cpp \
    testresultsparser.cpp

HEADERS  += mainwindow.h \
    jtftestparser.h \
    jtftestscanner.h \
    jtftestscannerworker.h \
    common.h \
    testresultsparser.h

FORMS    += mainwindow.ui

RESOURCES += \
    Resources.qrc

DESTDIR = $$OUT_PWD/bin
RCC_DIR = $$OUT_PWD/resources
MOC_DIR = $$OUT_PWD/moc
OBJECTS_DIR = $$OUT_PWD/objects

QT_PATH_FOR_COPY = $$[QT_INSTALL_LIBS]
DESTDIR_PATH = $$DESTDIR

win32:QT_PATH_FOR_COPY ~= s,/,\\,g
win32:DESTDIR_PATH ~= s,/,\\,g

QMAKE_POST_LINK += $$QMAKE_COPY $$QT_PATH_FOR_COPY\..\bin\icuin52.dll $$DESTDIR_PATH &&
QMAKE_POST_LINK += $$QMAKE_COPY $$QT_PATH_FOR_COPY\..\bin\icuuc52.dll $$DESTDIR_PATH &&
QMAKE_POST_LINK += $$QMAKE_COPY $$QT_PATH_FOR_COPY\..\bin\icudt52.dll $$DESTDIR_PATH &&

CONFIG (debug,debug|release){
    QMAKE_POST_LINK += $$QMAKE_COPY $$QT_PATH_FOR_COPY\..\bin\Qt5Cored.dll $$DESTDIR_PATH &&
    QMAKE_POST_LINK += $$QMAKE_COPY $$QT_PATH_FOR_COPY\..\bin\Qt5Guid.dll $$DESTDIR_PATH &&
    QMAKE_POST_LINK += $$QMAKE_COPY $$QT_PATH_FOR_COPY\..\bin\Qt5Widgetsd.dll $$DESTDIR_PATH
}

CONFIG (release,debug|release){
    QMAKE_POST_LINK += $$QMAKE_COPY $$QT_PATH_FOR_COPY\..\bin\Qt5Core.dll $$DESTDIR_PATH &&
    QMAKE_POST_LINK += $$QMAKE_COPY $$QT_PATH_FOR_COPY\..\bin\Qt5Gui.dll $$DESTDIR_PATH &&
    QMAKE_POST_LINK += $$QMAKE_COPY $$QT_PATH_FOR_COPY\..\bin\Qt5Widgets.dll $$DESTDIR_PATH
}


