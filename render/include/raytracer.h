
#ifndef RENDER_RAYTRACER_H
#define RENDER_RAYTRACER_H

#include <iostream>
#include <vector>
#include <memory>
#include <limits>
#include <thread>
#include "camera.h"
#include "image.h"
#include "light.h"
#include "texture.h"
#include "shape.h"
#include "shape_plane.h"
#include "shape_sphere.h"
#include "shape_triangle.h"

namespace render {

using shape_t = shape::Shape<float>;
using plane_t = shape::Plane<float>;
using sphere_t = shape::Sphere<float>;
using triangle_t = shape::Triangle<float>;
using point_t = Point<float>;
using vector_t = Vector<float>;
using color_t = Color<float>;
using light_t = Light<float>;
using ray_t = Ray<float>;
using texture_t = Texture<float>;
using material_t = Material<float>;
using camera_t = Camera<float>;
using image_t = Image<float>;

class Raytracer
{/*{{{*/
public:
  Raytracer() = delete;
  Raytracer(const point_t &a_cam_position,const point_t  &a_cam_look_at,
      const vector_t &a_cam_up,uint32_t a_width,uint32_t a_height,float a_distance,
      uint32_t a_sample_side) :
    m_camera{
      a_cam_position,
      a_cam_look_at,
      a_cam_up,
      -static_cast<float>(a_width)/2.0f,
      -static_cast<float>(a_height)/2.0f,
      a_distance},
    m_image{a_width,a_height},
    m_sample_side{a_sample_side},
    m_sample_step{1.0f/m_sample_side},
    m_sample_mult{m_sample_step*m_sample_step} {}

  Raytracer(const Raytracer &a_raytracer) = delete;
  Raytracer &operator = (const Raytracer &a_raytracer) = delete;

  uint32_t add_light(const light_t &a_light)
  {/*{{{*/
    m_lights.push_back(a_light);

    return m_lights.size() - 1;
  }/*}}}*/

  uint32_t add_texture(const texture_t &a_texture)
  {/*{{{*/
    m_textures.push_back(a_texture);
    return m_textures.size() - 1;
  }/*}}}*/

  uint32_t add_material(const material_t &a_material)
  {/*{{{*/
    m_materials.push_back(a_material);
    return m_materials.size() - 1;
  }/*}}}*/

  uint32_t add_shape(std::unique_ptr<shape_t> a_shape)
  {/*{{{*/

    // - ERROR -
    if (a_shape->material_idx() >= m_materials.size())
    {
      throw std::runtime_error{"Raytracer: invalid shape material index"};
    }

    // - check texture indexes -
    for (auto texture_idx : a_shape->texture_idxs())
    {
      // - ERROR -
      if (texture_idx >= m_textures.size())
      {
        throw std::runtime_error{"Raytracer: invalid shape texture index"};
      }
    }

    m_shapes.push_back(std::move(a_shape));

    return m_shapes.size() - 1;
  }/*}}}*/

  void render();
  void render_parallel(uint32_t a_thread_cnt);
  void save_bmp_file(const std::string &a_file_name)
  {/*{{{*/
    m_image.save_bmp_file(a_file_name);
  }/*}}}*/

  void format_to_stream(std::ostream &a_os) const
  {/*{{{*/
    a_os << "Raytracer{}";
  }/*}}}*/

private:
  bool ray_hit(const ray_t &a_ray,shape_t *ignore_ptr,float *a_distance,shape_t **a_shape_ptr);
  bool shadow_ray_hit(const ray_t &a_ray,shape_t *ignore_ptr,float a_max_distance);
  color_t ray_color(const ray_t &a_ray,shape_t *ignore_ptr);
  color_t image_color(float a_x,float a_y);
  static void render_worker(Raytracer *a_raytracer,uint32_t a_offset,uint32_t a_step);

private:
  camera_t m_camera;
  image_t m_image;
  uint32_t m_sample_side;
  float m_sample_step;
  float m_sample_mult;
  std::vector<light_t> m_lights;
  std::vector<texture_t> m_textures;
  std::vector<material_t> m_materials;
  std::vector<std::unique_ptr<shape_t>> m_shapes;
};/*}}}*/

} // namespace render

#endif

