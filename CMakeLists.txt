cmake_minimum_required (VERSION 2.8)

IF(BIICODE)
    ADD_BIICODE_TARGETS()
ELSE(NOT BIICODE)
    set(PROJECT Obbligato)
    project(${PROJECT} C CXX)
    set(CXX11 "OFF")

    INCLUDE (common.cmake)
ENDIF(BIICODE)
