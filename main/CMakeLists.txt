cmake_minimum_required(VERSION 3.15)

set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake"
    CACHE STRING "Vcpkg toolchain file")

project(ATOMIC LANGUAGES CXX)

# Use C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON) # -fPIC equivalent

# Enable automatic Qt processing
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Compiler flags (Currently debugging so...)
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(-fdiagnostics-color=always -Wall -g)
elseif(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    add_compile_options(/W4 /Z7 /DEBUG)
endif()

# ---- Dependencies ----
find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)
find_package(OpenSSL REQUIRED)
find_package(xlnt REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
find_package(Boost REQUIRED COMPONENTS asio)
include_directories("include/")

# ---- Sources ----
set(SOURCES
    panel.cpp
    timezone_stub.c
    qres.qrc
    CST_Listing.cpp
    CST_PlainTextDialog.cpp
    CST_RadioButtonDialog.cpp
    CST_RichTextEdit.cpp
    CST_Separator.cpp
    CST_TestCaseDialog.cpp
    CST_TextEditorDialog.cpp
    WIN_ClassesSettings.cpp
    WIN_ContestsSettings.cpp
    WIN_UsersSettings.cpp
    WIN_GenerateTestCasesDialog.cpp
)

# ---- Executable ----
add_executable(panel ${SOURCES})

# ---- Link dependencies ----
target_link_libraries(panel PRIVATE
    Qt6::Widgets
    Qt6::Core
    Qt6::Gui
    OpenSSL::SSL
    OpenSSL::Crypto
    xlnt::xlnt
    Boost::asio
)

# ---- Platform specifics ----
if(WIN32)
    target_link_libraries(panel PRIVATE ws2_32)
elseif(UNIX)
    target_link_libraries(panel PRIVATE pthread)
endif()

# ---- Output settings ----
set_target_properties(panel PROPERTIES
    OUTPUT_NAME "panel"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

# ---- Auto-deploy Qt DLLs after build (Windows only) ----
if(WIN32)
    # Find windeployqt from vcpkg
    find_program(WINDEPLOYQT_EXECUTABLE windeployqt
        HINTS
            "${VCPKG_ROOT}/installed/x64-windows/tools/Qt6/bin"
            "${CMAKE_SOURCE_DIR}/vcpkg_installed/x64-windows/tools/Qt6/bin"
            "${CMAKE_BINARY_DIR}/vcpkg_installed/x64-windows/tools/Qt6/bin"
    )

    if(WINDEPLOYQT_EXECUTABLE)
        add_custom_command(TARGET panel POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E echo "Running windeployqt for $<CONFIG> build..."
            COMMAND "${WINDEPLOYQT_EXECUTABLE}"
                    --no-translations
                    $<$<CONFIG:Debug>:--debug>
                    $<$<CONFIG:Release>:--release>
                    --dir \"$<TARGET_FILE_DIR:panel>\"
                    \"$<TARGET_FILE:panel>\"
            WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
            COMMENT "Deploying Qt runtime for panel"
        )
    else()
        message(WARNING "windeployqt not found!")
    endif()
endif()
message(STATUS "Configured ATOMIC")
