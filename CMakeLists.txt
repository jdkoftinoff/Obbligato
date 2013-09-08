cmake_minimum_required (VERSION 2.8) 
project (Obbligato)
enable_testing()

add_definitions(
    -std=c++11 -stdlib=libc++
)


set(LIBS ${LIBS} ${CHECK_LIBRARIES} Obbligato)

include_directories( include )

add_library (Obbligato src/Action.cpp src/Atomic.cpp src/Concept.cpp src/Config.cpp src/Config_Option.cpp src/Config_OptionBase.cpp src/Config_OptionGroup.cpp src/Config_OptionGroups.cpp src/Config_OptionGroupsIOS.cpp src/Config_OptionGroupsMacOSX.cpp src/Config_OptionGroupsWin32.cpp src/Config_RegistryWin32.cpp src/Constants.cpp src/Deleter.cpp src/Endian.cpp src/Form.cpp src/Function.cpp src/FunctionList.cpp src/IEEE.cpp src/IEEE_Types.cpp src/IOStream.cpp src/IOStream_Fmt.cpp src/IOStream_InputState.cpp src/IOStream_OutputState.cpp src/LexicalCast.cpp src/Logger.cpp src/Logger_Base.cpp src/Logger_File.cpp src/Logger_IOStream.cpp src/Logger_Syslog.cpp src/Logger_Win32.cpp src/Net.cpp src/Net_Address.cpp src/Net_AddressList.cpp src/Net_EUI48.cpp src/Net_EUI64.cpp src/Net_Handler.cpp src/Net_LinkStatus.cpp src/Net_LinkStatusBase.cpp src/Net_LinkStatusLinux.cpp src/Net_LinkStatusMacOSX.cpp src/Net_LinkStatusWin32.cpp src/Net_MAC48.cpp src/Net_Packet.cpp src/Net_PacketPayload.cpp src/Net_QueuedRawSocket.cpp src/Net_QueuedSocket.cpp src/Net_QueuedTCPSocket.cpp src/Net_QueuedUDPSocket.cpp src/Net_RawHandler.cpp src/Net_RawSocket.cpp src/Net_RawSocketLinux.cpp src/Net_RawSocketMacOSX.cpp src/Net_RawSocketWin32.cpp src/Net_Reactor.cpp src/Net_ReactorBase.cpp src/Net_Socket.cpp src/Net_TCPHandler.cpp src/Net_TCPHandlerFactory.cpp src/Net_TCPServerHandler.cpp src/Net_TCPServerSocket.cpp src/Net_TCPSocket.cpp src/Net_UDPHandler.cpp src/Net_UDPSocket.cpp src/Noncopyable.cpp src/Obbligato.cpp src/Operations.cpp src/Operations_NotificationTarget.cpp src/Operations_Operation.cpp src/Operations_OperationBase.cpp src/Platform.cpp src/Platform_Compiler_Clang.cpp src/Platform_Compiler_GCC.cpp src/Platform_Compiler_MSVC.cpp src/Platform_IOS.cpp src/Platform_Linux.cpp src/Platform_MacOSX.cpp src/Platform_Posix.cpp src/Platform_Win32.cpp src/Pool.cpp src/RangeCheck.cpp src/SIMD.cpp src/SIMD_Vector.cpp src/SIMD_VectorAVX32x8.cpp src/SIMD_VectorAVX64x4.cpp src/SIMD_VectorNEON32x4.cpp src/SIMD_VectorSSE32x4.cpp src/SIMD_VectorSSE64x2.cpp src/Service.cpp src/Service_Base.cpp src/Service_MacOSX.cpp src/Service_ManagerWin32.cpp src/Service_Posix.cpp src/Service_Win32.cpp src/SharedPtr.cpp src/StaticAssert.cpp src/Test.cpp src/Tests_Config.cpp src/Tests_IOStream.cpp src/Tests_LexicalCast.cpp src/Tests_Logger.cpp src/Tests_Net.cpp src/Tests_Operations.cpp src/Tests_Pool.cpp src/Tests_SIMD.cpp src/Tests_Time.cpp src/Tests_Transaction.cpp src/Time.cpp src/Time_Linux.cpp src/Time_MacOSX.cpp src/Time_Ticker.cpp src/Time_TimeVal.cpp src/Time_Win32.cpp src/Traits.cpp src/Transaction.cpp src/World.cpp)

link_directories( ${OBBLIGATO_BINARY_DIR} )

add_executable(TestObbligato tests/TestObbligato.cpp)
target_link_libraries(TestObbligato ${LIBS})
add_test(TestObbligato ${CMAKE_CURRENT_BINARY_DIR}/TestObbligato.cpp)

