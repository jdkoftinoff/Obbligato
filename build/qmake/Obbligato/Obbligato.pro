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


unix:QMAKE_CXXFLAGS+=-std=c++0x
