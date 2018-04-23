
#ifndef RT_DEMO_RAYTRACER_DEMO_H
#define RT_DEMO_RAYTRACER_DEMO_H

#include <string>
#include "raytracer.h"
#include "cclsys.h"
#include "ccljson.h"
#include "cclvarval.h"

namespace rt_demo {

using render::shape_t;
using render::plane_t;
using render::sphere_t;
using render::triangle_t;
using render::point_t;
using render::vector_t;
using render::color_t;
using render::light_t;
using render::ray_t;
using render::texture_t;
using render::material_t;
using render::camera_t;
using render::image_t;

using render::shape::tex_idxs_t;
using render::Raytracer;

class RaytracerDemo
{/*{{{*/
public:
  RaytracerDemo() = delete;
  RaytracerDemo(std::string &&a_prog_name,std::string &&a_scene_file,std::string &&a_output_file) :
    m_prog_name{a_prog_name},m_scene_file{a_scene_file},m_output_file{a_output_file} {}
  RaytracerDemo(const RaytracerDemo &a_rt_demo) = delete;
  RaytracerDemo &operator = (const RaytracerDemo &a_rt_demo) = delete;

  bool render();

private:
  std::string m_prog_name;
  std::string m_scene_file;
  std::string m_output_file;
};/*}}}*/

} // namespace rt_demo

#endif

