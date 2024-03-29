######################################################################
# Automatically generated by qmake (3.1) Thu Nov 19 12:34:28 2020
######################################################################

# Running in Debug Mode: qmake CONFIG+=debug
TARGET = QVocabulary
TEMPLATE = app

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
QT += core gui sql help xml texttospeech


#CONFIG += static
CONFIG += c++11

# Compile with:
# qmake-mingw CONFIG+=debug
CONFIG(debug, debug|release) {
    # For qDebug to work
	CONFIG += console
}

DEFINES += QT_DEPRECATED_WARNINGS

# You can make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# Please consult the documentation of the deprecated API in order to know
# how to port your code away from it.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
########################################################################################################
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x050F00    # disables all the APIs deprecated before Qt 5.15.0

#DEFINES += _QX_ENABLE_BOOST
DEFINES += _BUILDING_QX_VOCABULARY

!contains(DEFINES, _QX_NO_PRECOMPILED_HEADER) {
    PRECOMPILED_HEADER = src/precompiled.h
}

INCLUDEPATH += /usr/include/QxOrm/include
LIBPATH     += /usr/lib

CONFIG(debug, debug|release) {
    LIBS += -lQxOrmd
} else {
    LIBS += -lQxOrm
}

###########################################################
# QtWaitingSpinner
###########################################################
INCLUDEPATH += /usr/include/QtWaitingSpinner/include
LIBS += -L/usr/lib -lqtwaitingspinner

###########################################################
# Qt Mustache
###########################################################
INCLUDEPATH += /home/vagrant/tools/qt-mustache/src

INCLUDEPATH += /projects/VS_QVocabulary/src

###############################
# SOURCES
###############################
SOURCES += $$files(lib/*.cpp, true)
SOURCES += $$files(src/*.cpp, true)
HEADERS += $$files(lib/*.h, true)
HEADERS += $$files(src/*.h, true)
FORMS += $$files(Forms/*.ui, true) \
    Forms/SettingsWidgetQuiz.ui
RESOURCES = QVocabulary.qrc
RC_ICONS = dictionary_transparent.ico
TRANSLATIONS += $$files(translations/*.ts, true)

###############################
# BUILD
###############################
CONFIG(debug, debug|release) {
    DESTDIR = build/debug
}
CONFIG(release, debug|release) {
    DESTDIR = build/release
}

OBJECTS_DIR = $$DESTDIR/.obj
MOC_DIR = $$DESTDIR/.moc
RCC_DIR = $$DESTDIR/.qrc
UI_DIR = $$DESTDIR/.ui
