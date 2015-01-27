cmake_minimum_required (VERSION 2.8)

option(TODO "Enable TODO items that are in progress" OFF)
option(TESTS "Enable building of extended test code in library" ON)
option(EXAMPLES "Enable building of example programs" ON)
option(TOOLS "Enable building of tools" ON)
option(TOOLS_DEV "Enable building of tools-dev" ON)

enable_testing()

INCLUDE (CPack)
INCLUDE (CTest)

set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
FIND_PACKAGE (Threads)

if(CMAKE_USE_PTHREADS_INIT)
    if( ${CMAKE_SYSTEM_NAME} MATCHES "Linux" )
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pthread")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pthread")
    endif()
endif()

IF (APPLE)
    FIND_LIBRARY(COREFOUNDATION_LIBRARY CoreFoundation )
    set(LIBS ${LIBS} ${COREFOUNDATION_LIBRARY})
ENDIF (APPLE)

if (${CXX11} MATCHES "ON")
    # Compiler-specific C++11 activation.
    if (${CMAKE_CXX_COMPILER_ID} MATCHES "GNU")
        set(CMAKE_CXX_FLAGS                "${CMAKE_CXX_FLAGS} -Wall -std=c++11")
        set(CMAKE_CXX_FLAGS_DEBUG          "${CMAKE_CXX_FLAGS_DEBUG} -O0 -g")
        set(CMAKE_CXX_FLAGS_MINSIZEREL     "${CMAKE_CXX_FLAGS_MINSIZEREL} -Os -DNDEBUG")
        set(CMAKE_CXX_FLAGS_RELEASE        "${CMAKE_CXX_FLAGS_RELEASE} -O4 -DNDEBUG")
        set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} -O2 -g")

        execute_process(
            COMMAND ${CMAKE_CXX_COMPILER} -dumpversion OUTPUT_VARIABLE GCC_VERSION)
        if (NOT (GCC_VERSION VERSION_GREATER 4.7 OR GCC_VERSION VERSION_EQUAL 4.7))
            message(FATAL_ERROR "${PROJECT_NAME} requires g++ 4.7 or greater.")
        endif ()
    elseif (${CMAKE_CXX_COMPILER_ID} MATCHES "Clang")

        set(CMAKE_CXX_FLAGS                "${CMAKE_CXX_FLAGS} -Wall -std=c++11")
        set(CMAKE_CXX_FLAGS_DEBUG          "${CMAKE_CXX_FLAGS_DEBUG} -O0 -g")
        set(CMAKE_CXX_FLAGS_MINSIZEREL     "${CMAKE_CXX_FLAGS_MINSIZEREL} -Os -DNDEBUG")
        set(CMAKE_CXX_FLAGS_RELEASE        "${CMAKE_CXX_FLAGS_RELEASE} -O4 -DNDEBUG")
        set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} -O2 -g")

        if( ${CMAKE_SYSTEM_NAME} MATCHES "Linux" )
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libstdc++")
        else()
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
        endif()
    elseif(${CMAKE_MAKE_PROGRAM} MATCHES "(msdev|devenv|nmake|MSBuild)")
        add_definitions("/W3")
    else ()
        message(FATAL_ERROR "Your C++ compiler does not support C++11.")
    endif ()
endif ()


if(TODO MATCHES "ON")
   add_definitions("-DTODO=1")
   message(STATUS "TODO items that are in progress are enabled for compiling")
endif()


set(LIBS ${PROJECT} ${LIBS} ${CHECK_LIBRARIES} ${PROJECT})

include_directories( include  )

set(PROJECT_INCLUDES_GLOBS ${PROJECT_INCLUDES_GLOBS} "include/*.h" "include/*.hpp" "include/*/*.h" "include/*/*.hpp" "include/*/*/*.h" "include/*/*/*.hpp")
set(PROJECT_SRC_GLOBS ${PROJECT_SRC_GLOBS} "src/*.c" "src/*.cpp" "src/*/*.c" "src/*/*.cpp" "src/*/*/*.c" "src/*/*/*.cpp")

file(GLOB PROJECT_INCLUDES ${PROJECT_INCLUDES_GLOBS} )
file(GLOB PROJECT_SRC ${PROJECT_SRC_GLOBS} )

add_library (${PROJECT} ${PROJECT_SRC} ${PROJECT_INCLUDES})

link_directories( ${PROJECT_BINARY_DIR} )


if(EXAMPLES MATCHES "ON")
    file(GLOB PROJECT_EXAMPLES "examples/*.c" "examples/*.cpp")
    foreach(item ${PROJECT_EXAMPLES})
      GET_FILENAME_COMPONENT(examplename ${item} NAME_WE )
      add_executable(${examplename} ${item})
      target_link_libraries(${examplename} ${LIBS} )
    endforeach(item)
endif()

if(TOOLS MATCHES "ON")
    file(GLOB PROJECT_TOOLS "tools/*.c" "tools/*.cpp")
    foreach(item ${PROJECT_TOOLS})
      GET_FILENAME_COMPONENT(toolname ${item} NAME_WE )
      add_executable(${toolname} ${item})
      target_link_libraries(${toolname} ${LIBS} )
    endforeach(item)
endif()

if(TOOLS_DEV MATCHES "ON")
    file(GLOB PROJECT_TOOLS_DEV "tools-dev/*.c" "tools-dev/*.cpp")
    foreach(item ${PROJECT_TOOLS_DEV})
      GET_FILENAME_COMPONENT(toolname ${item} NAME_WE )
      add_executable(${toolname} ${item})
      target_link_libraries(${toolname} ${LIBS} )
    endforeach(item)
endif()

if(TESTS MATCHES "ON")
   file(GLOB PROJECT_TESTS "tests/*.c" "tests/*.cpp")
   foreach(item ${PROJECT_TESTS})
      GET_FILENAME_COMPONENT(testname ${item} NAME_WE )
      add_executable(${testname} ${item})
      target_link_libraries(${testname} ${LIBS} )
      add_test(NAME ${testname} COMMAND ${testname} )
   endforeach(item)
endif()


