
#include "tests.h"
#include "shape_sphere.h"

using render::shape::Sphere;
using render::Ray;
using render::Vector;

void test_shape_sphere_construction()
{/*{{{*/
  Sphere<float> sphere1{{1.0,2.0,3.0},10.0,0,{}};
  Sphere<float> sphere2{1.0,2.0,3.0,10.0,0,{}};
  assert(sphere1 == sphere2);

  std::ostringstream oss;
  oss << sphere1;
  assert(oss.str() == "Sphere{center=Point{1,2,3},radius=10}");
}/*}}}*/

void test_shape_sphere_ray_hit()
{/*{{{*/
  float distance;

  assert((Sphere<float>{{0.0,0.0,0.0},10.0,0,{}}).hit({{0.0,0.0,0.0},{1.0,0.0,0.0}},&distance));
  assert(distance == 10.0);

  assert((Sphere<float>{{1000.0,0.0,0.0},10.0,0,{}}).hit({{0.0,0.0,0.0},{1.0,0.0,0.0}},&distance));
  assert(distance == 990.0);

  assert((Sphere<float>{{0.0,1000.0,0.0},10.0,0,{}}).hit({{0.0,0.0,0.0},{0.0,1.0,0.0}},&distance));
  assert(distance == 990.0);

  assert((Sphere<float>{{0.0,0.0,1000.0},10.0,0,{}}).hit({{0.0,0.0,0.0},{0.0,0.0,1.0}},&distance));
  assert(distance == 990.0);
}/*}}}*/

void test_shape_sphere_normal()
{/*{{{*/
  Sphere<float> sphere1{{0.0,0.0,0.0},10.0,0,{}};
  Ray<float> ray1{{0.0,0.0,-1000.0},{0.0,0.0,1.0}};
  float distance;

  assert(sphere1.hit(ray1,&distance));
  assert(sphere1.normal(ray1.hit_point(distance)) == (Vector<float>{0,0,-1}));
}/*}}}*/

void test_shape_sphere_all()
{/*{{{*/
  test_shape_sphere_construction();
  test_shape_sphere_ray_hit();
  test_shape_sphere_normal();
}/*}}}*/

int main(int argc,char **argv)
{/*{{{*/
  for (int arg_idx = 1;arg_idx < argc;++arg_idx)
  {
    if (std::string("all") == argv[arg_idx])
    {
      test_shape_sphere_all();
    }
    else if (std::string("construction") == argv[arg_idx])
    {
      test_shape_sphere_construction();
    }
    else if (std::string("ray_hit") == argv[arg_idx])
    {
      test_shape_sphere_ray_hit();
    }
    else if (std::string("normal") == argv[arg_idx])
    {
      test_shape_sphere_normal();
    }
    else
    {
      // - ERROR -
      throw std::runtime_error{"unrecognized argument"};
    }
  }

  return 0;
}/*}}}*/

