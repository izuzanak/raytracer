
#include "tests.h"
#include "shape_plane.h"

using render::shape::Plane;
using render::Vector;

void test_shape_plane_construction()
{/*{{{*/
  Plane<float> plane1{{0,0,0},{0,0,1},0,{}};
  Plane<float> plane2{0,0,0,0,0,1,0,{}};
  assert(plane1 == plane2);

  std::ostringstream oss;
  oss << plane1;
  assert(oss.str() == "Plane{point=Point{0,0,0},normal=Vector{0,0,1}}");
}/*}}}*/

void test_shape_plane_ray_hit()
{/*{{{*/
  Plane<float> plane1{{0,0,1000},{0,0,-1},0,{}};
  Plane<float> plane2{{0,0,1000},{0,1,0},0,{}};

  float distance;
  assert(plane1.hit({{0,0,0},{0,0,1}},&distance));
  assert(distance == 1000);

  assert(!plane2.hit({{0,0,0},{0,0,1}},&distance));
}/*}}}*/

void test_shape_plane_normal()
{/*{{{*/
  Plane<float> plane1{{0,0,0},{0,0,1},0,{}};
  assert(plane1.normal({0,0,0}) == (Vector<float>{0,0,1}));
}/*}}}*/

void test_shape_plane_all()
{/*{{{*/
  test_shape_plane_construction();
  test_shape_plane_ray_hit();
  test_shape_plane_normal();
}/*}}}*/

int main(int argc,char **argv)
{/*{{{*/
  for (int arg_idx = 1;arg_idx < argc;++arg_idx)
  {
    if (std::string("all") == argv[arg_idx])
    {
      test_shape_plane_all();
    }
    else if (std::string("construction") == argv[arg_idx])
    {
      test_shape_plane_construction();
    }
    else if (std::string("ray_hit") == argv[arg_idx])
    {
      test_shape_plane_ray_hit();
    }
    else if (std::string("normal") == argv[arg_idx])
    {
      test_shape_plane_normal();
    }
    else
    {
      // - ERROR -
      throw std::runtime_error{"unrecognized argument"};
    }
  }

  return 0;
}/*}}}*/

