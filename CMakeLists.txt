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


file(GLOB OBBLIGATO_TESTS "tests/*.cpp")
foreach(item ${OBBLIGATO_TESTS})
  GET_FILENAME_COMPONENT(testname ${item} NAME_WE )
  add_executable(${testname} ${item})
  target_link_libraries(${testname} ${LIBS} )
  add_test(NAME ${testname} COMMAND ${testname} )
endforeach(item)


file(GLOB OBBLIGATO_EXAMPLES "examples/*.cpp")
foreach(item ${OBBLIGATO_EXAMPLES})
  GET_FILENAME_COMPONENT(examplename ${item} NAME_WE )
  add_executable(${examplename} ${item})
  target_link_libraries(${examplename} ${LIBS} )
endforeach(item)



export( TARGETS Obbligato FILE Obbligato-exports.cmake )

install( FILES ${OBBLIGATO_INCLUDES}  DESTINATION include )

