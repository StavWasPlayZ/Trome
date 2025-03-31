# CMakeList.txt : CMake project for Server, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("Server")


# Add source to this project's executable.
add_executable (Server "src/Main.cpp" "src/infrastructure/Server.h" "src/infrastructure/Server.cpp" "src/exception/WSAException.cpp" "src/handler/IRequestHandler.h" "src/handler/LoginRequestHandler.h" "src/infrastructure/Communicator.hpp" "src/infrastructure/Client.hpp")

# Platform-specific sources
if (WIN32)
  target_sources(Server PRIVATE "src/platform/windows/infrastructure/WindowsCommunicator.h" "src/platform/windows/infrastructure/WindowsCommunicator.cpp")
else()
  #TODO: Linux
  # add_executable(Server )
endif()


if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET Server PROPERTY CXX_STANDARD 20)
endif()

# Support inclues starting from relative paths
target_include_directories(Server PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)