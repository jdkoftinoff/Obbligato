cmake_minimum_required (VERSION 2.8) 
project (Obbligato)
enable_testing()

set(CMAKE_CXX_FLAGS                "-Wall -std=c++11")
set(CMAKE_CXX_FLAGS_DEBUG          "-O0 -g")
set(CMAKE_CXX_FLAGS_MINSIZEREL     "-Os -DNDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE        "-O4 -DNDEBUG")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O2 -g")

# Compiler-specific C++11 activation.
if ("${CMAKE_CXX_COMPILER_ID}" MATCHES "GNU")
    execute_process(
        COMMAND ${CMAKE_CXX_COMPILER} -dumpversion OUTPUT_VARIABLE GCC_VERSION)
    if (NOT (GCC_VERSION VERSION_GREATER 4.7 OR GCC_VERSION VERSION_EQUAL 4.7))
        message(FATAL_ERROR "${PROJECT_NAME} requires g++ 4.7 or greater.")
    endif ()
elseif ("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
else ()
    message(FATAL_ERROR "Your C++ compiler does not support C++11.")
endif ()


set(LIBS ${LIBS} ${CHECK_LIBRARIES} Obbligato)

include_directories( include )

add_library (Obbligato STATIC
    
	src/Action.cpp 
	src/Atomic.cpp 
	src/Concept.cpp 
	src/Config.cpp 
	src/Config_Option.cpp 
	src/Config_OptionBase.cpp 
	src/Config_OptionGroup.cpp 
	src/Config_OptionGroups.cpp 
	src/Config_OptionGroupsIOS.cpp 
	src/Config_OptionGroupsMacOSX.cpp 
	src/Config_OptionGroupsWin32.cpp 
	src/Config_RegistryWin32.cpp 
	src/Constants.cpp 
	src/Deleter.cpp 
	src/Endian.cpp 
	src/Form.cpp 
	src/Function.cpp 
	src/FunctionList.cpp 
	src/IEEE.cpp 
	src/IEEE_Types.cpp 
	src/IOStream.cpp 
	src/IOStream_Fmt.cpp 
	src/IOStream_InputState.cpp 
	src/IOStream_OutputState.cpp 
	src/LexicalCast.cpp 
	src/Logger.cpp 
	src/Logger_Base.cpp 
	src/Logger_File.cpp 
	src/Logger_IOStream.cpp 
	src/Logger_Syslog.cpp 
	src/Logger_Win32.cpp 
	src/Net.cpp 
	src/Net_Address.cpp 
	src/Net_AddressList.cpp 
	src/Net_EUI48.cpp 
	src/Net_EUI64.cpp 
	src/Net_Handler.cpp 
	src/Net_LinkStatus.cpp 
	src/Net_LinkStatusBase.cpp 
	src/Net_LinkStatusLinux.cpp 
	src/Net_LinkStatusMacOSX.cpp 
	src/Net_LinkStatusWin32.cpp 
	src/Net_MAC48.cpp 
	src/Net_Packet.cpp 
	src/Net_PacketPayload.cpp 
	src/Net_QueuedRawSocket.cpp 
	src/Net_QueuedSocket.cpp 
	src/Net_QueuedTCPSocket.cpp 
	src/Net_QueuedUDPSocket.cpp 
	src/Net_RawHandler.cpp 
	src/Net_RawSocket.cpp 
	src/Net_RawSocketLinux.cpp 
	src/Net_RawSocketMacOSX.cpp 
	src/Net_RawSocketWin32.cpp 
	src/Net_Reactor.cpp 
	src/Net_ReactorBase.cpp 
	src/Net_Socket.cpp 
	src/Net_TCPHandler.cpp 
	src/Net_TCPHandlerFactory.cpp 
	src/Net_TCPServerHandler.cpp 
	src/Net_TCPServerSocket.cpp 
	src/Net_TCPSocket.cpp 
	src/Net_UDPHandler.cpp 
	src/Net_UDPSocket.cpp 
	src/Noncopyable.cpp 
	src/Obbligato.cpp 
	src/Operations.cpp 
	src/Operations_NotificationTarget.cpp 
	src/Operations_Operation.cpp 
	src/Operations_OperationBase.cpp 
	src/Platform.cpp 
	src/Platform_Compiler_Clang.cpp 
	src/Platform_Compiler_GCC.cpp 
	src/Platform_Compiler_MSVC.cpp 
	src/Platform_IOS.cpp 
	src/Platform_Linux.cpp 
	src/Platform_MacOSX.cpp 
	src/Platform_Posix.cpp 
	src/Platform_Win32.cpp 
	src/Pool.cpp 
	src/RangeCheck.cpp 
	src/SIMD.cpp 
	src/SIMD_Vector.cpp 
	src/SIMD_VectorAVX32x8.cpp 
	src/SIMD_VectorAVX64x4.cpp 
	src/SIMD_VectorNEON32x4.cpp 
	src/SIMD_VectorSSE32x4.cpp 
	src/SIMD_VectorSSE64x2.cpp 
	src/Service.cpp 
	src/Service_Base.cpp 
	src/Service_MacOSX.cpp 
	src/Service_ManagerWin32.cpp 
	src/Service_Posix.cpp 
	src/Service_Win32.cpp 
	src/SharedPtr.cpp 
	src/StaticAssert.cpp 
	src/Test.cpp 
	src/Tests_Config.cpp 
	src/Tests_IOStream.cpp 
	src/Tests_LexicalCast.cpp 
	src/Tests_Logger.cpp 
	src/Tests_Net.cpp 
	src/Tests_Operations.cpp 
	src/Tests_Pool.cpp 
	src/Tests_SIMD.cpp 
	src/Tests_Time.cpp 
	src/Tests_Transaction.cpp 
	src/Time.cpp 
	src/Time_Linux.cpp 
	src/Time_MacOSX.cpp 
	src/Time_Ticker.cpp 
	src/Time_TimeVal.cpp 
	src/Time_Win32.cpp 
	src/Traits.cpp 
	src/Transaction.cpp 
  	src/World.cpp
	include/Obbligato.hpp
	include/Obbligato/Action.hpp
	include/Obbligato/Atomic.hpp
	include/Obbligato/Concept.hpp
	include/Obbligato/Config.hpp
	include/Obbligato/Config_Option.hpp
	include/Obbligato/Config_OptionBase.hpp
	include/Obbligato/Config_OptionGroup.hpp
	include/Obbligato/Config_OptionGroups.hpp
	include/Obbligato/Config_OptionGroupsIOS.hpp
	include/Obbligato/Config_OptionGroupsMacOSX.hpp
	include/Obbligato/Config_OptionGroupsWin32.hpp
	include/Obbligato/Config_RegistryWin32.hpp
	include/Obbligato/Constants.hpp
	include/Obbligato/Deleter.hpp
	include/Obbligato/Endian.hpp
	include/Obbligato/Form.hpp
	include/Obbligato/Function.hpp
	include/Obbligato/FunctionList.hpp
	include/Obbligato/IEEE.hpp
	include/Obbligato/IEEE_Types.hpp
	include/Obbligato/IOStream.hpp
	include/Obbligato/IOStream_Fmt.hpp
	include/Obbligato/IOStream_InputState.hpp
	include/Obbligato/IOStream_OutputState.hpp
	include/Obbligato/LexicalCast.hpp
	include/Obbligato/Logger.hpp
	include/Obbligato/Logger_Base.hpp
	include/Obbligato/Logger_File.hpp
	include/Obbligato/Logger_IOStream.hpp
	include/Obbligato/Logger_Syslog.hpp
	include/Obbligato/Logger_Win32.hpp
	include/Obbligato/Net.hpp
	include/Obbligato/Net_Address.hpp
	include/Obbligato/Net_AddressList.hpp
	include/Obbligato/Net_EUI48.hpp
	include/Obbligato/Net_EUI64.hpp
	include/Obbligato/Net_Handler.hpp
	include/Obbligato/Net_LinkStatus.hpp
	include/Obbligato/Net_LinkStatusBase.hpp
	include/Obbligato/Net_LinkStatusLinux.hpp
	include/Obbligato/Net_LinkStatusMacOSX.hpp
	include/Obbligato/Net_LinkStatusWin32.hpp
	include/Obbligato/Net_MAC48.hpp
	include/Obbligato/Net_Packet.hpp
	include/Obbligato/Net_PacketPayload.hpp
	include/Obbligato/Net_QueuedRawSocket.hpp
	include/Obbligato/Net_QueuedSocket.hpp
	include/Obbligato/Net_QueuedTCPSocket.hpp
	include/Obbligato/Net_QueuedUDPSocket.hpp
	include/Obbligato/Net_RawHandler.hpp
	include/Obbligato/Net_RawSocket.hpp
	include/Obbligato/Net_RawSocketLinux.hpp
	include/Obbligato/Net_RawSocketMacOSX.hpp
	include/Obbligato/Net_RawSocketWin32.hpp
	include/Obbligato/Net_Reactor.hpp
	include/Obbligato/Net_ReactorBase.hpp
	include/Obbligato/Net_Socket.hpp
	include/Obbligato/Net_TCPHandler.hpp
	include/Obbligato/Net_TCPHandlerFactory.hpp
	include/Obbligato/Net_TCPServerHandler.hpp
	include/Obbligato/Net_TCPServerSocket.hpp
	include/Obbligato/Net_TCPSocket.hpp
	include/Obbligato/Net_UDPHandler.hpp
	include/Obbligato/Net_UDPSocket.hpp
	include/Obbligato/Noncopyable.hpp
	include/Obbligato/Operations.hpp
	include/Obbligato/Operations_NotificationTarget.hpp
	include/Obbligato/Operations_Operation.hpp
	include/Obbligato/Operations_OperationBase.hpp
	include/Obbligato/Operations_decl.hpp
	include/Obbligato/Platform.hpp
	include/Obbligato/Platform_Compiler_Clang.hpp
	include/Obbligato/Platform_Compiler_GCC.hpp
	include/Obbligato/Platform_Compiler_MSVC.hpp
	include/Obbligato/Platform_IOS.hpp
	include/Obbligato/Platform_Linux.hpp
	include/Obbligato/Platform_MacOSX.hpp
	include/Obbligato/Platform_Posix.hpp
	include/Obbligato/Platform_Win32.hpp
	include/Obbligato/Pool.hpp
	include/Obbligato/RangeCheck.hpp
	include/Obbligato/SIMD.hpp
	include/Obbligato/SIMD_Vector.hpp
	include/Obbligato/SIMD_VectorAVX32x8.hpp
	include/Obbligato/SIMD_VectorAVX64x4.hpp
	include/Obbligato/SIMD_VectorNEON32x4.hpp
	include/Obbligato/SIMD_VectorSSE32x4.hpp
	include/Obbligato/SIMD_VectorSSE64x2.hpp
	include/Obbligato/Service.hpp
	include/Obbligato/Service_Base.hpp
	include/Obbligato/Service_MacOSX.hpp
	include/Obbligato/Service_ManagerWin32.hpp
	include/Obbligato/Service_Posix.hpp
	include/Obbligato/Service_Win32.hpp
	include/Obbligato/SharedPtr.hpp
	include/Obbligato/StaticAssert.hpp
	include/Obbligato/Test.hpp
	include/Obbligato/Tests_Config.hpp
	include/Obbligato/Tests_IOStream.hpp
	include/Obbligato/Tests_LexicalCast.hpp
	include/Obbligato/Tests_Logger.hpp
	include/Obbligato/Tests_Net.hpp
	include/Obbligato/Tests_Operations.hpp
	include/Obbligato/Tests_Pool.hpp
	include/Obbligato/Tests_SIMD.hpp
	include/Obbligato/Tests_Time.hpp
	include/Obbligato/Tests_Transaction.hpp
	include/Obbligato/Time.hpp
	include/Obbligato/Time_Linux.hpp
	include/Obbligato/Time_MacOSX.hpp
	include/Obbligato/Time_Ticker.hpp
	include/Obbligato/Time_TimeVal.hpp
	include/Obbligato/Time_Win32.hpp
	include/Obbligato/Traits.hpp
	include/Obbligato/Transaction.hpp
	include/Obbligato/World.hpp
)

link_directories( ${OBBLIGATO_BINARY_DIR} )

add_executable(TestObbligato tests/TestObbligato.cpp)
target_link_libraries(TestObbligato ${LIBS})
add_test(TestObbligato ${CMAKE_CURRENT_BINARY_DIR}/TestObbligato)

INSTALL(TARGETS Obbligato DESTINATION "lib")

INSTALL(FILES 
	include/Obbligato.hpp
	include/Obbligato/Action.hpp
	include/Obbligato/Atomic.hpp
	include/Obbligato/Concept.hpp
	include/Obbligato/Config.hpp
	include/Obbligato/Config_Option.hpp
	include/Obbligato/Config_OptionBase.hpp
	include/Obbligato/Config_OptionGroup.hpp
	include/Obbligato/Config_OptionGroups.hpp
	include/Obbligato/Config_OptionGroupsIOS.hpp
	include/Obbligato/Config_OptionGroupsMacOSX.hpp
	include/Obbligato/Config_OptionGroupsWin32.hpp
	include/Obbligato/Config_RegistryWin32.hpp
	include/Obbligato/Constants.hpp
	include/Obbligato/Deleter.hpp
	include/Obbligato/Endian.hpp
	include/Obbligato/Form.hpp
	include/Obbligato/Function.hpp
	include/Obbligato/FunctionList.hpp
	include/Obbligato/IEEE.hpp
	include/Obbligato/IEEE_Types.hpp
	include/Obbligato/IOStream.hpp
	include/Obbligato/IOStream_Fmt.hpp
	include/Obbligato/IOStream_InputState.hpp
	include/Obbligato/IOStream_OutputState.hpp
	include/Obbligato/LexicalCast.hpp
	include/Obbligato/Logger.hpp
	include/Obbligato/Logger_Base.hpp
	include/Obbligato/Logger_File.hpp
	include/Obbligato/Logger_IOStream.hpp
	include/Obbligato/Logger_Syslog.hpp
	include/Obbligato/Logger_Win32.hpp
	include/Obbligato/Net.hpp
	include/Obbligato/Net_Address.hpp
	include/Obbligato/Net_AddressList.hpp
	include/Obbligato/Net_EUI48.hpp
	include/Obbligato/Net_EUI64.hpp
	include/Obbligato/Net_Handler.hpp
	include/Obbligato/Net_LinkStatus.hpp
	include/Obbligato/Net_LinkStatusBase.hpp
	include/Obbligato/Net_LinkStatusLinux.hpp
	include/Obbligato/Net_LinkStatusMacOSX.hpp
	include/Obbligato/Net_LinkStatusWin32.hpp
	include/Obbligato/Net_MAC48.hpp
	include/Obbligato/Net_Packet.hpp
	include/Obbligato/Net_PacketPayload.hpp
	include/Obbligato/Net_QueuedRawSocket.hpp
	include/Obbligato/Net_QueuedSocket.hpp
	include/Obbligato/Net_QueuedTCPSocket.hpp
	include/Obbligato/Net_QueuedUDPSocket.hpp
	include/Obbligato/Net_RawHandler.hpp
	include/Obbligato/Net_RawSocket.hpp
	include/Obbligato/Net_RawSocketLinux.hpp
	include/Obbligato/Net_RawSocketMacOSX.hpp
	include/Obbligato/Net_RawSocketWin32.hpp
	include/Obbligato/Net_Reactor.hpp
	include/Obbligato/Net_ReactorBase.hpp
	include/Obbligato/Net_Socket.hpp
	include/Obbligato/Net_TCPHandler.hpp
	include/Obbligato/Net_TCPHandlerFactory.hpp
	include/Obbligato/Net_TCPServerHandler.hpp
	include/Obbligato/Net_TCPServerSocket.hpp
	include/Obbligato/Net_TCPSocket.hpp
	include/Obbligato/Net_UDPHandler.hpp
	include/Obbligato/Net_UDPSocket.hpp
	include/Obbligato/Noncopyable.hpp
	include/Obbligato/Operations.hpp
	include/Obbligato/Operations_NotificationTarget.hpp
	include/Obbligato/Operations_Operation.hpp
	include/Obbligato/Operations_OperationBase.hpp
	include/Obbligato/Operations_decl.hpp
	include/Obbligato/Platform.hpp
	include/Obbligato/Platform_Compiler_Clang.hpp
	include/Obbligato/Platform_Compiler_GCC.hpp
	include/Obbligato/Platform_Compiler_MSVC.hpp
	include/Obbligato/Platform_IOS.hpp
	include/Obbligato/Platform_Linux.hpp
	include/Obbligato/Platform_MacOSX.hpp
	include/Obbligato/Platform_Posix.hpp
	include/Obbligato/Platform_Win32.hpp
	include/Obbligato/Pool.hpp
	include/Obbligato/RangeCheck.hpp
	include/Obbligato/SIMD.hpp
	include/Obbligato/SIMD_Vector.hpp
	include/Obbligato/SIMD_VectorAVX32x8.hpp
	include/Obbligato/SIMD_VectorAVX64x4.hpp
	include/Obbligato/SIMD_VectorNEON32x4.hpp
	include/Obbligato/SIMD_VectorSSE32x4.hpp
	include/Obbligato/SIMD_VectorSSE64x2.hpp
	include/Obbligato/Service.hpp
	include/Obbligato/Service_Base.hpp
	include/Obbligato/Service_MacOSX.hpp
	include/Obbligato/Service_ManagerWin32.hpp
	include/Obbligato/Service_Posix.hpp
	include/Obbligato/Service_Win32.hpp
	include/Obbligato/SharedPtr.hpp
	include/Obbligato/StaticAssert.hpp
	include/Obbligato/Test.hpp
	include/Obbligato/Tests_Config.hpp
	include/Obbligato/Tests_IOStream.hpp
	include/Obbligato/Tests_LexicalCast.hpp
	include/Obbligato/Tests_Logger.hpp
	include/Obbligato/Tests_Net.hpp
	include/Obbligato/Tests_Operations.hpp
	include/Obbligato/Tests_Pool.hpp
	include/Obbligato/Tests_SIMD.hpp
	include/Obbligato/Tests_Time.hpp
	include/Obbligato/Tests_Transaction.hpp
	include/Obbligato/Time.hpp
	include/Obbligato/Time_Linux.hpp
	include/Obbligato/Time_MacOSX.hpp
	include/Obbligato/Time_Ticker.hpp
	include/Obbligato/Time_TimeVal.hpp
	include/Obbligato/Time_Win32.hpp
	include/Obbligato/Traits.hpp
	include/Obbligato/Transaction.hpp
	include/Obbligato/World.hpp
    DESTINATION include
)

