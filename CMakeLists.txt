cmake_minimum_required (VERSION 2.8)
set(PROJECT Obbligato)
project(${PROJECT} C CXX)
set(CXX11 "ON")

IF (APPLE)
    FIND_LIBRARY(COREFOUNDATION_LIBRARY CoreFoundation )
    set(LIBS ${LIBS} ${COREFOUNDATION_LIBRARY})
ENDIF (APPLE)


INCLUDE (common.cmake)


