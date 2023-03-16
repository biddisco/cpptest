cmake_minimum_required(VERSION 3.25)
project(tinyegl)

set(OpenGL_GL_PREFERENCE GLVND)
find_package(OpenGL)

if (OpenGL_OpenGL_FOUND AND OpenGL_EGL_FOUND)
  add_executable(tinyegl tinyegl.cxx)
  target_link_libraries(tinyegl OpenGL::EGL)
endif()
