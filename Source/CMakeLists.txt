# CMakeList.txt: LearnSomething 的 CMake 项目，在此处包括源代码并定义
# 项目特定的逻辑。
#

set(PROJECT_NAME QtJsonSerializer)

find_package(Qt6 COMPONENTS Core Widgets Gui)

# 将源代码添加到此项目的可执行文件。
file(GLOB_RECURSE headers "${CMAKE_CURRENT_SOURCE_DIR}/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.hpp"
)
file(GLOB_RECURSE sources "${CMAKE_CURRENT_SOURCE_DIR}/*.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
)
list(APPEND all_files ${headers} ${sources})

add_library(${PROJECT_NAME} SHARED ${all_files})
add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

target_include_directories(
    ${PROJECT_NAME} 
    PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
            $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
)

target_compile_definitions(
    ${PROJECT_NAME} PRIVATE ${Qt6Widgets_DEFINITIONS} QT_BUILD_JSONSERIALIZER_LIB
)

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang" AND CMAKE_CXX_SIMULATE_ID MATCHES "MSVC")
    # 使用的是Clang for MSVC
else()
    target_compile_options(${PROJECT_NAME} PRIVATE /bigobj)
endif()

target_link_libraries(${PROJECT_NAME} 
    PUBLIC Qt6::Core Qt6::Widgets Qt6::Gui Qt6::CorePrivate
)

# set qt moc sources
qt_wrap_cpp(
    PROJECT_NAME_MOC
    ${headers}
    TARGET
    ${PROJECT_NAME}
    OPTIONS
    --no-notes # Don't display a note for the headers which don't produce a
                # moc_*.cpp
)

# add qt moc sources
target_sources(${PROJECT_NAME} PRIVATE ${PROJECT_NAME_MOC})

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 23)
endif()

# copy qt dependency
add_custom_command(
    TARGET ${PROJECT_NAME}
    POST_BUILD
    COMMAND Qt6::windeployqt "--no-translations"
            $<TARGET_FILE:${PROJECT_NAME}>
    WORKING_DIRECTORY $<TARGET_FILE_DIR:${PROJECT_NAME}>
)

# copy other dependency
add_custom_command(
    TARGET ${PROJECT_NAME}
    POST_BUILD
    COMMAND
    ${CMAKE_COMMAND} -E copy_if_different $<TARGET_RUNTIME_DLLS:${PROJECT_NAME}>
    $<TARGET_FILE_DIR:${PROJECT_NAME}>
    COMMAND_EXPAND_LISTS
)

include(GNUInstallDirs)
install( 
    TARGETS ${PROJECT_NAME}
    EXPORT ${PROJECT_NAME}Targets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(
    EXPORT ${PROJECT_NAME}Targets
    DESTINATION cmake/${PROJECT_NAME}
    NAMESPACE ${PROJECT_NAME}::
    FILE ${PROJECT_NAME}Targets.cmake # Not sure if this is still needed
)

install(
    DIRECTORY QtJsonSerializer/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

