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
    testresultsparser.cpp \
    jtftestrunworker.cpp \
    jtftestrunner.cpp \
    addenvvartotrack.cpp \
    listofenvironmentvarstotrack.cpp

HEADERS  += mainwindow.h \
    jtftestparser.h \
    jtftestscanner.h \
    jtftestscannerworker.h \
    common.h \
    testresultsparser.h \
    jtftestrunworker.h \
    jtftestrunner.h \
    addenvvartotrack.h \
    listofenvironmentvarstotrack.h

FORMS    += mainwindow.ui \
    addenvvartotrack.ui \
    listofenvironmentvarstotrack.ui

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

mkcommands += $$DESTDIR/platforms
win32:mkcommands ~= s,/,\\,g
mkpath($$mkcommands)

!exists($$QT_PATH_FOR_COPY\..\bin\icudt53.dll){
    message(File does not exist: $$QT_PATH_FOR_COPY\..\bin\icudt53.dll)
}
QMAKE_POST_LINK += $$QMAKE_COPY $$QT_PATH_FOR_COPY\..\bin\icudt53.dll $$DESTDIR_PATH &&

!exists($$QT_PATH_FOR_COPY\..\bin\icuin53.dll){
    message(File does not exist: $$QT_PATH_FOR_COPY\..\bin\icuin53.dll)
}
QMAKE_POST_LINK += $$QMAKE_COPY $$QT_PATH_FOR_COPY\..\bin\icuin53.dll $$DESTDIR_PATH &&

!exists($$QT_PATH_FOR_COPY\..\bin\icuuc53.dll){
    message(File does not exist: $$QT_PATH_FOR_COPY\..\bin\icuuc53.dll)
}
QMAKE_POST_LINK += $$QMAKE_COPY $$QT_PATH_FOR_COPY\..\bin\icuuc53.dll $$DESTDIR_PATH &&

CONFIG (debug,debug|release){

    !exists($$DESTDIR_PATH\platforms\qwindowsd.dll){
        QMAKE_POST_LINK += $$QMAKE_COPY $$QT_PATH_FOR_COPY\..\plugins\platforms\qwindowsd.dll $$DESTDIR_PATH\platforms &&
    }
    QMAKE_POST_LINK += $$QMAKE_COPY $$QT_PATH_FOR_COPY\..\bin\Qt5Cored.dll $$DESTDIR_PATH &&
    QMAKE_POST_LINK += $$QMAKE_COPY $$QT_PATH_FOR_COPY\..\bin\Qt5Guid.dll $$DESTDIR_PATH &&
    QMAKE_POST_LINK += $$QMAKE_COPY $$QT_PATH_FOR_COPY\..\bin\Qt5Widgetsd.dll $$DESTDIR_PATH &&
    QMAKE_POST_LINK += $$QMAKE_COPY $$QT_PATH_FOR_COPY\..\bin\libGLESv2d.dll $$DESTDIR_PATH &&
    QMAKE_POST_LINK += $$QMAKE_COPY $$QT_PATH_FOR_COPY\..\bin\libEGLd.dll $$DESTDIR_PATH
}

CONFIG (release,debug|release){
    !exists($$DESTDIR_PATH\platforms\qwindows.dll){
        QMAKE_POST_LINK += $$QMAKE_COPY $$QT_PATH_FOR_COPY\..\plugins\platforms\qwindows.dll $$DESTDIR_PATH\platforms &&
    }
    QMAKE_POST_LINK += $$QMAKE_COPY $$QT_PATH_FOR_COPY\..\bin\Qt5Core.dll $$DESTDIR_PATH &&
    QMAKE_POST_LINK += $$QMAKE_COPY $$QT_PATH_FOR_COPY\..\bin\Qt5Gui.dll $$DESTDIR_PATH &&
    QMAKE_POST_LINK += $$QMAKE_COPY $$QT_PATH_FOR_COPY\..\bin\Qt5Widgets.dll $$DESTDIR_PATH &&
    QMAKE_POST_LINK += $$QMAKE_COPY $$QT_PATH_FOR_COPY\..\bin\libGLESv2.dll $$DESTDIR_PATH &&
    QMAKE_POST_LINK += $$QMAKE_COPY $$QT_PATH_FOR_COPY\..\bin\libEGL.dll $$DESTDIR_PATH
}


