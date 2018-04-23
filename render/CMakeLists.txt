
include_directories(include)

add_library(render ${RENDER_LIB_MODE} src/raytracer.cpp)
target_link_libraries(render pthread)

