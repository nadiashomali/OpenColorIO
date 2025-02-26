# SPDX-License-Identifier: BSD-3-Clause
# Copyright Contributors to the OpenColorIO Project.

set(SOURCES
    OCIOColorSpace.cpp
    OCIODisplayView.cpp
    OCIOMain.cpp
    OCIOProcessor.cpp
    OCIOUtils.cpp
)
set(OFXS_SOURCES
    Support/Library/ofxsCore.cpp
    Support/Library/ofxsImageEffect.cpp
    Support/Library/ofxsInteract.cpp
    Support/Library/ofxsLog.cpp
    Support/Library/ofxsMultiThread.cpp
    Support/Library/ofxsParams.cpp
    Support/Library/ofxsProperty.cpp
    Support/Library/ofxsPropertyValidation.cpp
)
set(PLIST
    ${CMAKE_SOURCE_DIR}/share/openfx/Info.plist
)
set(RESOURCES
    ${CMAKE_SOURCE_DIR}/share/openfx/resources/OpenColorIO.OCIOColorSpace.png
    ${CMAKE_SOURCE_DIR}/share/openfx/resources/OpenColorIO.OCIOColorSpace.svg
    ${CMAKE_SOURCE_DIR}/share/openfx/resources/OpenColorIO.OCIODisplayView.png
    ${CMAKE_SOURCE_DIR}/share/openfx/resources/OpenColorIO.OCIODisplayView.svg
)

add_library(ofxplugin MODULE ${SOURCES} ${OFXS_SOURCES})

if(NOT BUILD_SHARED_LIBS)
    target_compile_definitions(ofxplugin
        PRIVATE
            OpenColorIO_SKIP_IMPORTS
    )
endif()

# Disable known compiler warnings from OpenFX Support Library
if(MSVC)
    set(PLATFORM_COMPILE_FLAGS "${PLATFORM_COMPILE_FLAGS} /wd4996 /wd4101")
else()
    # Some reported unused parameters in openfx are used when DEBUG macro is 
    # defined
    set(PLATFORM_COMPILE_FLAGS "${PLATFORM_COMPILE_FLAGS} -Wno-unused-parameter")
endif()

set_target_properties(ofxplugin PROPERTIES
    PREFIX ""
    OUTPUT_NAME "OpenColorIO"
    SUFFIX ".ofx"
    COMPILE_FLAGS "${PLATFORM_COMPILE_FLAGS}"
)

target_include_directories(ofxplugin
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        Support/include
        ${openfx_INCLUDE_DIR}
        ${CMAKE_SOURCE_DIR}/include/
        ${CMAKE_BINARY_DIR}/include/
)

target_link_libraries(ofxplugin
    PRIVATE
        openfx::module
        OpenColorIO
        pystring::pystring
)

# Detect OFX architecture
if(WIN32)
    set(OFX_ARCH "Win32")
    if(CMAKE_SIZEOF_VOID_P EQUAL 8)
        set(OFX_ARCH "Win64")
    endif()
elseif(APPLE)
    set(OFX_ARCH "MacOS")
    if(CMAKE_SIZEOF_VOID_P EQUAL 8)
        set(OFX_ARCH "MacOS-x86-64")
    endif()
elseif(UNIX)
    set(OFX_ARCH "Linux-x86")
    if(CMAKE_SIZEOF_VOID_P EQUAL 8)
        set(OFX_ARCH "Linux-x86-64")
    endif()
else()
    message(FATAL_ERROR 
            "The OpenColorIO OFX plugins are unsupported for the current system architecture")
endif()

# Install to OFX package structure
set(OFX_INSTALL_PREFIX "${CMAKE_INSTALL_LIBDIR}/OpenColorIO.ofx.bundle/Contents")

install(DIRECTORY 
    DESTINATION ${OFX_INSTALL_PREFIX}
)
install(FILES ${PLIST}
    DESTINATION ${OFX_INSTALL_PREFIX}
)
install(FILES ${RESOURCES}
    DESTINATION ${OFX_INSTALL_PREFIX}/Resources
)
install(TARGETS ofxplugin LIBRARY 
    DESTINATION ${OFX_INSTALL_PREFIX}/${OFX_ARCH}
)
