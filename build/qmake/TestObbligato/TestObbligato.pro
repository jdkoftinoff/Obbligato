cache()

TOP= ../../..

! include( ../common.pri ) {
  error( need common.pri file )
}


TARGET=TestObbligato

SOURCES += $$OBBLIGATO/tests/TestObbligato.cpp
