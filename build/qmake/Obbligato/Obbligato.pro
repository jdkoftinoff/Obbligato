QT       -= core gui

TARGET = Obbligato
TEMPLATE = lib
CONFIG += staticlib

TOP = ../../..
OBBLIGATO = $$TOP

SOURCES += \
    $$OBBLIGATO/src/*.cpp

HEADERS += \
    $$OBBLIGATO/include/*.hpp \
    $$OBBLIGATO/include/Obbligato/*.hpp 

INCLUDEPATH += \
    $$OBBLIGATO/include

DEPENDPATH +=  \
    $$OBBLIGATO/include \
    $$OBBLIGATO/include/Obbligato/*.hpp 


unix:cpp11:QMAKE_CXXFLAGS+=-std=c++11
unix:cpp0x:QMAKE_CXXFLAGS+=-std=c++0x
