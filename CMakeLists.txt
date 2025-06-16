cmake_minimum_required(VERSION 2.8...3.5)

project(cosc363project2)

add_executable(RayTracer.out RayTracer.cpp Ray.cpp SceneObject.cpp Sphere.cpp Cone.cpp plane.cpp Cylinder.cpp TextureOpenIL.cpp)

set(OpenGL_GL_PREFERENCE GLVND)
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
find_package(glm REQUIRED)
find_package(DevIL REQUIRED)

include_directories( ${OPENGL_INCLUDE_DIRS}  ${GLUT_INCLUDE_DIRS} ${GLM_INCLUDE_DIR} ${IL_INCLUDE_DIR})

target_link_libraries( RayTracer.out ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES} ${GLM_LIBRARY} ${IL_LIBRARIES})
