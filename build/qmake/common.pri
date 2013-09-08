
QT -= core gui
CONFIG += link_prl debug
win32:QT += core
win32:CONFIG+=console

TEMPLATE = app

OBBLIGATO = $$TOP

HEADERS += \
    $$OBBLIGATO/include/Obbligato/Tests_*.hpp

INCLUDEPATH += \
    $$OBBLIGATO/include

DEPENDPATH +=  \
    $$OBBLIGATO/include \
    $$OBBLIGATO/include/Obbligato/*.hpp 

macx:QMAKE_CXXFLAGS+=-std=c++11
macx:QMAKE_CXXFLAGS+=-stdlib=libc++
macx:QMAKE_LFLAGS+=-stdlib=libc++

unix:cpp11:QMAKE_CXXFLAGS+=-std=c++11
unix:cpp0x:QMAKE_CXXFLAGS+=-std=c++0x

INCLUDEPATH += $$TOP/include 

DEPENDPATH += $$INCLUDEPATH

unix:LIBS += -L../Obbligato -lObbligato 

macx:pcap:LIBS += -lpcap

win32 {
  CONFIG(release):LIBS += -L../lib/release -lObbligato
  CONFIG(debug):LIBS += -L../lib/debug -lObbligato
  pcap:LIBS += pcap
  CONFIG(release):PRE_TARGETDEPS += ../lib/release/Obbligato.lib
  CONFIG(debug):PRE_TARGETDEPS += ../lib/debug/Obbligato.lib
}

win32:LIBS+=
macx:LIBS+=

unix:PRE_TARGETDEPS += ../Obbligato/libObbligato.a

macx {
 QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.8
}


