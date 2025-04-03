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


# Common sources
add_executable (Server
  "src/common/Main.cpp"
  "src/common/infrastructure/Server.h"
  "src/common/infrastructure/Server.cpp"
  "src/common/handler/IRequestHandler.h"
  "src/common/handler/LoginRequestHandler.h"
  "src/common/infrastructure/CommonCommunicator.hpp"
  "src/common/infrastructure/Client.hpp"
  "src/common/infrastructure/Communicator.h"
  "src/common/exception/ForcedDisconnectionException.h"
  "src/common/exception/ForcedDisconnectionException.cpp"
  "src/common/exception/SocketTimeoutException.h"
  "src/common/exception/SocketTimeoutException.cpp"
  "src/common/Constants.h"
  "src/common/Constants.cpp"

  "src/common/infrastructure/codec/s2c/Response.h"
  "src/common/infrastructure/codec/s2c/Response.cpp"
  "src/common/infrastructure/codec/s2c/JsonResponsePacketSerializer.h"
  "src/common/infrastructure/codec/s2c/JsonResponsePacketSerializer.cpp"

  "src/common/infrastructure/codec/c2s/Request.h"
  "src/common/infrastructure/codec/c2s/Request.cpp"
  "src/common/infrastructure/codec/c2s/JsonRequestPacketDeserializer.h"
  "src/common/infrastructure/codec/c2s/JsonRequestPacketDeserializer.cpp"
)

# Platform-specific sources
if (WIN32)
  target_sources(Server PRIVATE
    "src/windows/infrastructure/WindowsCommunicator.h"
    "src/windows/infrastructure/WindowsCommunicator.cpp"
    "src/windows/exception/WSAException.cpp"
  )
else()
  target_sources(Server PRIVATE
    "src/unix/infrastructure/UnixCommunicator.h"
    "src/unix/infrastructure/UnixCommunicator.cpp"
  )
endif()


if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET Server PROPERTY CXX_STANDARD 20)
endif()


# Libraries
include(FetchContent)

#nlohmann_json
FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz)
FetchContent_MakeAvailable(json)

target_link_libraries(Server PRIVATE nlohmann_json::nlohmann_json)


# Support includes starting from relative paths
target_include_directories(Server PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_include_directories(Server PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src/common)