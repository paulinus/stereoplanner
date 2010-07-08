
CMAKE_MINIMUM_REQUIRED(VERSION 2.2)

IF (COMMAND cmake_policy)
  cmake_policy(SET CMP0003 OLD)
  cmake_policy(SET CMP0005 OLD)
ENDIF (COMMAND cmake_policy)

PROJECT(stereoplanner C CXX)

ENABLE_TESTING()

#########################
# Qt
FIND_PACKAGE(Qt4 REQUIRED)
FIND_PACKAGE(OpenGL REQUIRED)
SET(QT_USE_QTOPENGL ON)
INCLUDE(${QT_USE_FILE})
#########################

INCLUDE_DIRECTORIES(
  .
  ../third_party/eigen
  ${stereoplanner_BINARY_DIR}
)

SET(MOC_HEADERS main_window.h
                3D_viewer.h
                document.h
                parameters_form.h)

SET(SOURCES main.cc
            main_window.cc
            3D_viewer.cc
            trackball.cc
            document.cc
            geometry.cc
            parameters_form.cc)

QT4_WRAP_CPP(SOURCES ${MOC_HEADERS})
QT4_WRAP_UI(SOURCES parameters_form.ui)

ADD_EXECUTABLE(sp ${SOURCES} ${MOC_HEADERS})
TARGET_LINK_LIBRARIES(sp
                      ${QT_QTCORE_LIBRARY}
                      ${QT_QTGUI_LIBRARY}
                      ${QT_QTOPENGL_LIBRARY}
                      ${OPENGL_gl_LIBRARY}
                      ${OPENGL_glu_LIBRARY})

