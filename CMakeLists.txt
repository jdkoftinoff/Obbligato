cmake_minimum_required (VERSION 2.8) 
include(CPack)

project (Obbligato)
enable_testing()

# Compiler-specific C++11 activation.
if ("${CMAKE_CXX_COMPILER_ID}" MATCHES "GNU")
    execute_process(
        COMMAND ${CMAKE_CXX_COMPILER} -dumpversion OUTPUT_VARIABLE GCC_VERSION)
    if (NOT (GCC_VERSION VERSION_GREATER 4.7 OR GCC_VERSION VERSION_EQUAL 4.7))
        message(FATAL_ERROR "${PROJECT_NAME} requires g++ 4.7 or greater.")
    endif ()
    set(CMAKE_CXX_FLAGS                "-Wall -std=c++11")
    set(CMAKE_CXX_FLAGS_DEBUG          "-O0 -g")
    set(CMAKE_CXX_FLAGS_MINSIZEREL     "-Os -DNDEBUG")
    set(CMAKE_CXX_FLAGS_RELEASE        "-O4 -DNDEBUG")
    set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O2 -g")

elseif ("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
    set(CMAKE_CXX_FLAGS                "-Wall -std=c++11")
    set(CMAKE_CXX_FLAGS_DEBUG          "-O0 -g")
    set(CMAKE_CXX_FLAGS_MINSIZEREL     "-Os -DNDEBUG")
    set(CMAKE_CXX_FLAGS_RELEASE        "-O4 -DNDEBUG")
    set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O2 -g")

    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
#else ()
#    message(FATAL_ERROR "Your C++ compiler does not support C++11.")
endif ()


set(LIBS ${LIBS} ${CHECK_LIBRARIES} Obbligato)

if(WIN32)
set(LIBS ${LIBS} "ws2_32")
endif()

include_directories( include )

file(GLOB_RECURSE OBBLIGATO_INCLUDES include  "*.hpp" )

file(GLOB_RECURSE OBBLIGATO_SRC src  "*.cpp" )

add_library (Obbligato STATIC    
    ${OBBLIGATO_SRC}
    ${OBBLIGATO_INCLUDES}
)


link_directories( ${OBBLIGATO_BINARY_DIR} )

add_executable(TestObbligato tests/TestObbligato.cpp)
target_link_libraries(TestObbligato ${LIBS})
add_test(TestObbligato ${CMAKE_CURRENT_BINARY_DIR}/TestObbligato)

export( TARGETS Obbligato FILE Obbligato-exports.cmake )

install( FILES ${OBBLIGATO_INCLUDES}  DESTINATION include )

