cache()

QT       -= core gui

TARGET = Obbligato
TEMPLATE = lib
CONFIG += staticlib

TOP = ../../..
OBBLIGATO = $$TOP

SOURCES += \
    $$files( $$OBBLIGATO/src/*.cpp )

HEADERS += \
    $$files( $$OBBLIGATO/include/*.hpp )\
    $$files( $$OBBLIGATO/include/Obbligato/*.hpp  )

INCLUDEPATH += \
    $$OBBLIGATO/include

DEPENDPATH +=  \
    $$files( $$OBBLIGATO/include ) \
    $$files( $$OBBLIGATO/include/Obbligato/*.hpp )


macx:QMAKE_CXXFLAGS+=-std=c++11
macx:QMAKE_CXXFLAGS+=-stdlib=libc++

unix:cpp11:QMAKE_CXXFLAGS+=-std=c++11
unix:cpp0x:QMAKE_CXXFLAGS+=-std=c++0x

macx {
 QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.8
}
