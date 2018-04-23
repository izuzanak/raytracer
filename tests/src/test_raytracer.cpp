
#include "tests.h"
#include "raytracer.h"

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

void test_raytracer_construction()
{/*{{{*/
  uint32_t width = 640;
  uint32_t height = 480;

  Raytracer raytracer1{
    {-500.0,-1000.0,-250.0},
    {0.0,0.0,-1150.0},
    {0.0,0.0,1.0},
    width,height,
    1750.0,1
  };

  uint32_t mat_idx = raytracer1.add_material({{0.15,0.15,0.0},0.3,0.3,3,0.0,0.0});

  raytracer1.add_shape(std::make_unique<sphere_t>(-150.0, 150.0,-1000.0,145.0,mat_idx,tex_idxs_t{}));
  raytracer1.add_shape(std::make_unique<sphere_t>( 150.0, 150.0,-1000.0,145.0,mat_idx,tex_idxs_t{}));
  raytracer1.add_shape(std::make_unique<sphere_t>(-150.0,-150.0,-1000.0,145.0,mat_idx,tex_idxs_t{}));
  raytracer1.add_shape(std::make_unique<sphere_t>( 150.0,-150.0,-1000.0,145.0,mat_idx,tex_idxs_t{}));
}/*}}}*/

void test_raytracer_render()
{/*{{{*/
  uint32_t width = 640;
  uint32_t height = 480;
  float distance = 800.0;

  //uint32_t width = 1280;
  //uint32_t height = 1024;
  //float distance = 1750.0;

  Raytracer raytracer1{
    {-500.0,-1000.0,1000.0},
    {0.0,0.0,-100.0},
    {0.0,0.0,1.0},
    width,height,
    distance,1
  };

  raytracer1.add_light({{-600.0, 600.0,1000.0},{ 0.7, 0.7, 0.7}});
  raytracer1.add_light({{ 600.0,-600.0,1000.0},{0.25,0.25, 0.0}});

  uint32_t tex1_idx = raytracer1.add_texture(
      texture_t{texture_t::TYPE_1,texture_t::NORMAL,0.12,0.3});
  uint32_t tex2_idx = raytracer1.add_texture(
      texture_t{texture_t::TYPE_4,texture_t::AMBIENT,0.025,1.0});
  uint32_t tex3_idx = raytracer1.add_texture(
      texture_t{texture_t::TYPE_3,texture_t::AMBIENT,0.01,1.0});
  uint32_t tex4_idx = raytracer1.add_texture(
      texture_t{texture_t::TYPE_1,texture_t::AMBIENT,0.04,1.0});

  uint32_t mat1_idx = raytracer1.add_material({{0.15,0.15,0.0},0.3,0.3,15,0.0,0.0});
  uint32_t mat2_idx = raytracer1.add_material({{0.0,0.0,0.0},0.05,0.95,15,0.75,0.0});
  //uint32_t mat3_idx = raytracer1.add_material({{0.2,0.0,0.0},0.3,0.3,3,0.0,0.0});
  uint32_t mat4_idx = raytracer1.add_material({{0.2,0.0,0.0},0.3,0.3,3,0.0,0.0});
  uint32_t mat5_idx = raytracer1.add_material({{0.0,0.1,0.0},0.05,0.95,15,0.25,0.0});

  raytracer1.add_shape(std::make_unique<sphere_t>(-150.0,-150.0,0.0,145.0,mat1_idx,tex_idxs_t{tex2_idx}));
  raytracer1.add_shape(std::make_unique<sphere_t>( 150.0,-150.0,0.0,145.0,mat2_idx,tex_idxs_t{}));
  raytracer1.add_shape(std::make_unique<sphere_t>( 150.0, 150.0,0.0,145.0,mat4_idx,tex_idxs_t{tex3_idx}));
  raytracer1.add_shape(std::make_unique<sphere_t>(-150.0, 150.0,0.0,145.0,mat2_idx,tex_idxs_t{tex1_idx}));

  raytracer1.add_shape(std::make_unique<plane_t>(0.0,0.0,-200.0,0.0,0.0,1.0,mat5_idx,tex_idxs_t{tex4_idx}));

  //raytracer1.add_shape(std::make_unique<triangle_t>(
  //     0,   0,200, 
  //   120,   0,200, 
  //   100, 100,200,
  //   mat3_idx,tex_idxs_t{tex2_idx}));

  raytracer1.render();
  raytracer1.save_bmp_file("render.bmp");
}/*}}}*/

void test_raytracer_render_parallel()
{/*{{{*/
  uint32_t width = 640;
  uint32_t height = 480;
  float distance = 800.0;

  //uint32_t width = 1280;
  //uint32_t height = 1024;
  //float distance = 1750.0;

  Raytracer raytracer1{
    {-500.0,-1000.0,1000.0},
    {0.0,0.0,-100.0},
    {0.0,0.0,1.0},
    width,height,
    distance,1
  };

  raytracer1.add_light({{-600.0, 600.0,1000.0},{ 0.7, 0.7, 0.7}});
  raytracer1.add_light({{ 600.0,-600.0,1000.0},{0.25,0.25, 0.0}});

  uint32_t tex1_idx = raytracer1.add_texture(
      texture_t{texture_t::TYPE_1,texture_t::NORMAL,0.12,0.3});
  uint32_t tex2_idx = raytracer1.add_texture(
      texture_t{texture_t::TYPE_4,texture_t::AMBIENT,0.025,1.0});
  uint32_t tex3_idx = raytracer1.add_texture(
      texture_t{texture_t::TYPE_3,texture_t::AMBIENT,0.01,1.0});
  uint32_t tex4_idx = raytracer1.add_texture(
      texture_t{texture_t::TYPE_1,texture_t::AMBIENT,0.04,1.0});

  uint32_t mat1_idx = raytracer1.add_material({{0.15,0.15,0.0},0.3,0.3,15,0.0,0.0});
  uint32_t mat2_idx = raytracer1.add_material({{0.0,0.0,0.0},0.05,0.95,15,0.75,0.0});
  //uint32_t mat3_idx = raytracer1.add_material({{0.2,0.0,0.0},0.3,0.3,3,0.0,0.0});
  uint32_t mat4_idx = raytracer1.add_material({{0.2,0.0,0.0},0.3,0.3,3,0.0,0.0});
  uint32_t mat5_idx = raytracer1.add_material({{0.0,0.1,0.0},0.05,0.95,15,0.25,0.0});

  raytracer1.add_shape(std::make_unique<sphere_t>(-150.0,-150.0,0.0,145.0,mat1_idx,tex_idxs_t{tex2_idx}));
  raytracer1.add_shape(std::make_unique<sphere_t>( 150.0,-150.0,0.0,145.0,mat2_idx,tex_idxs_t{}));
  raytracer1.add_shape(std::make_unique<sphere_t>( 150.0, 150.0,0.0,145.0,mat4_idx,tex_idxs_t{tex3_idx}));
  raytracer1.add_shape(std::make_unique<sphere_t>(-150.0, 150.0,0.0,145.0,mat2_idx,tex_idxs_t{tex1_idx}));

  raytracer1.add_shape(std::make_unique<plane_t>(0.0,0.0,-200.0,0.0,0.0,1.0,mat5_idx,tex_idxs_t{tex4_idx}));

  //raytracer1.add_shape(std::make_unique<triangle_t>(
  //     0,   0,200, 
  //   120,   0,200, 
  //   100, 100,200,
  //   mat3_idx,tex_idxs_t{tex2_idx}));

  raytracer1.render_parallel(4);
  raytracer1.save_bmp_file("render_parallel.bmp");
}/*}}}*/

void test_raytracer_all()
{/*{{{*/
  test_raytracer_construction();
  test_raytracer_render();
  test_raytracer_render_parallel();
}/*}}}*/

int main(int argc,char **argv)
{/*{{{*/
  for (int arg_idx = 1;arg_idx < argc;++arg_idx)
  {
    if (std::string("all") == argv[arg_idx])
    {
      test_raytracer_all();
    }
    else if (std::string("construction") == argv[arg_idx])
    {
      test_raytracer_construction();
    }
    else if (std::string("render") == argv[arg_idx])
    {
      test_raytracer_render();
    }
    else if (std::string("render_parallel") == argv[arg_idx])
    {
      test_raytracer_render_parallel();
    }
    else
    {
      // - ERROR -
      throw std::runtime_error{"unrecognized argument"};
    }
  }

  return 0;
}/*}}}*/

