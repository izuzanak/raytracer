
#include "tests.h"
#include "shape_triangle.h"

using render::shape::Triangle;
using render::Vector;
using render::Ray;

void test_shape_triangle_construction()
{/*{{{*/
  Triangle<float> triangle1{{0,0,0},{1,0,0},{1,1,0},0,{}};
  Triangle<float> triangle2{0,0,0,1,0,0,1,1,0,0,{}};
  assert(triangle1 == triangle2);

  std::ostringstream oss;
  oss << triangle1;
  assert(oss.str() == "Triangle{v0=Point{0,0,0},v1=Point{1,0,0},v2=Point{1,1,0}}");
}/*}}}*/

void test_shape_triangle_ray_hit()
{/*{{{*/
  Triangle<float> triangle1{{0,0,1000},{1,0,1000},{1,1,1000},0,{}};
  Ray<float> ray1{{0,0,0},{0,0,1}};

  float distance;

  assert(triangle1.hit(ray1,&distance));
  assert(distance == 1000);
  assert(triangle1.normal(ray1.hit_point(distance)) == (Vector<float>{0,0,-1}));

  assert(!triangle1.hit({{-1,0,0},{0,0,1}},&distance));
}/*}}}*/

void test_shape_triangle_normal()
{/*{{{*/
  Triangle<float> triangle1{{0,0,0},{1,0,0},{1,1,0},0,{}};
  assert(triangle1.normal({0,0,0}) == (Vector<float>{0,0,-1}));
}/*}}}*/

void test_shape_triangle_all()
{/*{{{*/
  test_shape_triangle_construction();
  test_shape_triangle_ray_hit();
  test_shape_triangle_normal();
}/*}}}*/

int main(int argc,char **argv)
{/*{{{*/
  for (int arg_idx = 1;arg_idx < argc;++arg_idx)
  {
    if (std::string("all") == argv[arg_idx])
    {
      test_shape_triangle_all();
    }
    else if (std::string("construction") == argv[arg_idx])
    {
      test_shape_triangle_construction();
    }
    else if (std::string("ray_hit") == argv[arg_idx])
    {
      test_shape_triangle_ray_hit();
    }
    else if (std::string("normal") == argv[arg_idx])
    {
      test_shape_triangle_normal();
    }
    else
    {
      // - ERROR -
      throw std::runtime_error{"unrecognized argument"};
    }
  }

  return 0;
}/*}}}*/

