cmake_minimum_required(VERSION 3.20)

project(cpp_manual_logger VERSION 0.1.0 LANGUAGES CXX)

include(GNUInstallDirs)

option(MANUEL_LOGGER_FETCH_SPDLOG "Fetch spdlog automatically when it is not installed" ON)
option(MANUEL_LOGGER_BUILD_EXAMPLE "Build the example program" ON)

find_package(spdlog CONFIG QUIET)

if(NOT spdlog_FOUND AND MANUEL_LOGGER_FETCH_SPDLOG)
  include(FetchContent)

  FetchContent_Declare(
    spdlog
    GIT_REPOSITORY https://github.com/gabime/spdlog.git
    GIT_TAG v1.14.1
  )
  FetchContent_MakeAvailable(spdlog)
endif()

if(TARGET spdlog_header_only AND NOT TARGET spdlog::spdlog_header_only)
  add_library(spdlog::spdlog_header_only ALIAS spdlog_header_only)
endif()

if(TARGET spdlog AND NOT TARGET spdlog::spdlog)
  add_library(spdlog::spdlog ALIAS spdlog)
endif()

add_library(manuel_logger INTERFACE)
add_library(manuel_logger::manuel_logger ALIAS manuel_logger)

target_compile_features(manuel_logger INTERFACE cxx_std_17)
target_include_directories(
  manuel_logger
  INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

if(TARGET spdlog::spdlog_header_only)
  set(MANUEL_LOGGER_SPDLOG_TARGET spdlog::spdlog_header_only)
elseif(TARGET spdlog::spdlog)
  set(MANUEL_LOGGER_SPDLOG_TARGET spdlog::spdlog)
else()
  message(FATAL_ERROR "spdlog was not found. Install it first or enable MANUEL_LOGGER_FETCH_SPDLOG.")
endif()

target_link_libraries(
  manuel_logger
  INTERFACE
    $<BUILD_INTERFACE:${MANUEL_LOGGER_SPDLOG_TARGET}>
    $<INSTALL_INTERFACE:${MANUEL_LOGGER_SPDLOG_TARGET}>
)

if(MANUEL_LOGGER_BUILD_EXAMPLE)
  add_executable(manuel_logger_example examples/basic.cpp)
  target_link_libraries(manuel_logger_example PRIVATE manuel_logger::manuel_logger)
endif()

include(CMakePackageConfigHelpers)

write_basic_package_version_file(
  "${CMAKE_CURRENT_BINARY_DIR}/manuel_loggerConfigVersion.cmake"
  VERSION ${PROJECT_VERSION}
  COMPATIBILITY SameMajorVersion
)

configure_package_config_file(
  "${CMAKE_CURRENT_SOURCE_DIR}/cmake/manuel_loggerConfig.cmake.in"
  "${CMAKE_CURRENT_BINARY_DIR}/manuel_loggerConfig.cmake"
  INSTALL_DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/manuel_logger"
  PATH_VARS CMAKE_INSTALL_INCLUDEDIR
)

install(
  DIRECTORY include/
  DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
)

install(
  FILES
    "${CMAKE_CURRENT_BINARY_DIR}/manuel_loggerConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/manuel_loggerConfigVersion.cmake"
  DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/manuel_logger"
)
