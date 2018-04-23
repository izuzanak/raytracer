
#include "tests.h"
#include "ray.h"

using render::Ray;

void test_ray_construction()
{/*{{{*/
  Ray<float> ray1{{},{1.0,1.0,1.0}};

  std::ostringstream oss;
  oss << ray1;
  assert(oss.str() == "Ray{origin=Point{0,0,0},direction=Vector{1,1,1}}");
}/*}}}*/

void test_ray_all()
{/*{{{*/
  test_ray_construction();
}/*}}}*/

int main(int argc,char **argv)
{/*{{{*/
  for (int arg_idx = 1;arg_idx < argc;++arg_idx)
  {
    if (std::string("all") == argv[arg_idx])
    {
      test_ray_all();
    }
    else if (std::string("construction") == argv[arg_idx])
    {
      test_ray_construction();
    }
    else
    {
      // - ERROR -
      throw std::runtime_error{"unrecognized argument"};
    }
  }

  return 0;
}/*}}}*/

