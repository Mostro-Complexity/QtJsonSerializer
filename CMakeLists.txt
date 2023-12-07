cmake_minimum_required(VERSION 3.21)

set(PROJECT_NAME QtJsonSerializer)

project(${PROJECT_NAME})

add_subdirectory("Source")
add_subdirectory("Example")

include(CMakePackageConfigHelpers)
configure_package_config_file( 
    "Config.cmake.in" 
    "${PROJECT_NAME}Config.cmake"
    INSTALL_DESTINATION cmake/${PROJECT_NAME}
)

write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
    VERSION 1.0.0
    COMPATIBILITY SameMajorVersion
)

### Install Config and ConfigVersion files
install(
    FILES "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
          "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
    DESTINATION "."
)
