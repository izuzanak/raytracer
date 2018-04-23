
#include "raytracer.h"

namespace render {

void Raytracer::render()
{/*{{{*/

  // - fill target image -
  m_image.fill({0.0,0.0,0.0});

  for (uint32_t y_int = 0;y_int < m_image.height();++y_int)
  {
    float y = y_int;

    for (uint32_t x_int = 0;x_int < m_image.width();++x_int)
    {
      float x = x_int;

      // - set image color -
      m_image(x_int,y_int) = image_color(x,y);;
    }
  }
}/*}}}*/

void Raytracer::render_parallel(uint32_t a_thread_cnt)
{/*{{{*/

  // - fill target image -
  m_image.fill({0.0,0.0,0.0});

  std::thread threads[a_thread_cnt];

  // - execute worker threads -
  for (uint32_t t_idx = 0;t_idx < a_thread_cnt;++t_idx)
  {
    threads[t_idx] = std::thread(Raytracer::render_worker,this,t_idx,a_thread_cnt);
  }

  // - join to worker threads -
  for (uint32_t t_idx = 0;t_idx < a_thread_cnt;++t_idx)
  {
    threads[t_idx].join();
  }
}/*}}}*/

bool Raytracer::ray_hit(const ray_t &a_ray,shape_t *ignore_ptr,float *a_distance,shape_t **a_shape_ptr)
{/*{{{*/
  *a_distance = std::numeric_limits<float>::max();
  *a_shape_ptr = nullptr;

  for (auto &shape : m_shapes)
  {
    float distance;
    if (shape.get() != ignore_ptr && shape->hit(a_ray,&distance) && distance < *a_distance)
    {
      *a_distance = distance;
      *a_shape_ptr = shape.get();
    }
  }

  return *a_shape_ptr != nullptr;
}/*}}}*/

bool Raytracer::shadow_ray_hit(const ray_t &a_ray,shape_t *ignore_ptr,float a_max_distance)
{/*{{{*/
  for (auto &shape : m_shapes)
  {
    float distance;
    if (shape.get() != ignore_ptr && shape->hit(a_ray,&distance) && distance < a_max_distance)
    {
      return true;
    }
  }

  return false;
}/*}}}*/

color_t Raytracer::ray_color(const ray_t &a_ray,shape_t *ignore_ptr)
{/*{{{*/
  color_t color;

  float distance;
  shape_t *shape_ptr;
  if (ray_hit(a_ray,ignore_ptr,&distance,&shape_ptr))
  {
    auto point{a_ray.hit_point(distance)};
    auto abs_point = point.absolute();
    auto normal{shape_ptr->normal(point)};
    auto &material = m_materials[shape_ptr->material_idx()];

    // - process normal textures -
    bool has_normal_texture = false;
    for (auto texture_idx : shape_ptr->texture_idxs())
    {
      texture_t &texture = m_textures[texture_idx];

      // - if texture target is ambient color -
      if (texture.target() == texture_t::Target::NORMAL)
      {
        normal += texture.amplitude()*(-0.5f + texture.sample(abs_point*texture.frequency()));
      }
    }

    // - normalize modified normal texture -
    if (has_normal_texture)
    {
      normal = normal.normalize();
    }

    // - process all lights -
    for (auto &light : m_lights)
    {
      auto light_vec = light.position() - point;
      auto light_distance = light_vec.length();
      light_vec /= light_distance;

      // - test path to light -
      if (!shadow_ray_hit({point,light_vec},shape_ptr,light_distance))
      {
        // - process phong lighting model -
        color += material.phong_model(light_vec,light,a_ray,normal);
      }

      // - process reflection -
      if (material.reflection() > 0)
      {
        color += ray_color({point,a_ray.direction() -
            normal*(2*a_ray.direction().dot(normal))},shape_ptr)*material.reflection();
      }

      // - process ambient textures -
      bool has_ambient_texture = false;
      for (auto texture_idx : shape_ptr->texture_idxs())
      {
        texture_t &texture = m_textures[texture_idx];

        // - if texture target is ambient color -
        if (texture.target() == texture_t::Target::AMBIENT)
        {
          color += material.ambient()*texture.amplitude()*texture.sample(abs_point*texture.frequency());
          has_ambient_texture = true;
        }
      }

      // - shape without ambient texture -
      if (!has_ambient_texture)
      {
        color += material.ambient();
      }

      // - crop color components to ones -
      color.crop_to_ones();
    }
  }

  return color;
}/*}}}*/

color_t Raytracer::image_color(float a_x,float a_y)
{/*{{{*/
  switch (m_sample_side)
  {
  case 0:
    return {};
  case 1:
    return ray_color({m_camera.get_ray(a_x,a_y)},nullptr);
  case 2:
    return (ray_color({m_camera.get_ray(a_x      ,a_y      )},nullptr) +
            ray_color({m_camera.get_ray(a_x + 0.5,a_y      )},nullptr) +
            ray_color({m_camera.get_ray(a_x,      a_y + 0.5)},nullptr) +
            ray_color({m_camera.get_ray(a_x + 0.5,a_y + 0.5)},nullptr)) * 0.25;
  default:
  {/*{{{*/
    color_t color;

    for (uint32_t y_sub = 0;y_sub < m_sample_side;++y_sub)
    {
      float y = a_y + y_sub*m_sample_step;

      for (uint32_t x_sub = 0;x_sub < m_sample_side;++x_sub)
      {
        float x = a_x + x_sub*m_sample_step;

        color += ray_color({m_camera.get_ray(x,y)},nullptr);
      }
    }

    return color*m_sample_mult;
  }/*}}}*/
  }
}/*}}}*/

void Raytracer::render_worker(Raytracer *a_raytracer,uint32_t a_offset,uint32_t a_step)
{/*{{{*/
  uint32_t width = a_raytracer->m_image.width();
  uint32_t height = a_raytracer->m_image.height();

  uint32_t x_int = a_offset;

  for (uint32_t y_int = 0;y_int < height;++y_int)
  {
    float y = y_int;

    for (;x_int < width;x_int += a_step)
    {
      // - set image color -
      a_raytracer->m_image(x_int,y_int) = a_raytracer->image_color(x_int,y);;
    }

    x_int -= width;
  }
}/*}}}*/

std::ostream &operator << (std::ostream &a_os,const Raytracer &a_raytracer)
{/*{{{*/
  a_raytracer.format_to_stream(a_os);
  return a_os;
}/*}}}*/

} // namespace render

