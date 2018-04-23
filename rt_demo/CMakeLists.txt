
include_directories(include)

add_executable(rt_demo src/raytracer_demo.cpp src/main.cpp)
target_link_libraries(rt_demo render cclstr cclsys cclvar ccljson cclvarval)
add_test(rt_demo_test rt_demo "../../tests/data/rt_demo/scene.json" "scene.bmp")

