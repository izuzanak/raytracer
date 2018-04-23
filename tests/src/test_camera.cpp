
#include "tests.h"
#include "camera.h"

using render::Camera;

void test_camera_construction()
{/*{{{*/
  Camera<float> cam1{{0.0,0.0,0.0},{0.0,0.0,1.0},{0.0,1.0,0.0},-320.0,-240.0,1000.0};

  std::ostringstream oss;
  oss << cam1;
  assert(oss.str() == "Camera{position=Point{0,0,0},look_at=Point{0,0,1},up=Vector{0,1,0}}");
}/*}}}*/

void test_camera_get_ray()
{/*{{{*/
  float width = 64;
  float height = 48;

  Camera<float> cam1{{-500.0,-1000.0,-250.0},{0.0,0.0,-1150.0},{0.0,0.0,1.0},-width/2.0f,-height/2.0f,1750.0};

  for (int y = 0;y < height;++y)
  {
    for (int x = 0;x < width;++x)
    {
      cam1.get_ray(x,y);
    }
  }
}/*}}}*/

void test_camera_all()
{/*{{{*/
  test_camera_construction();
  test_camera_get_ray();
}/*}}}*/

int main(int argc,char **argv)
{/*{{{*/
  for (int arg_idx = 1;arg_idx < argc;++arg_idx)
  {
    if (std::string("all") == argv[arg_idx])
    {
      test_camera_all();
    }
    else if (std::string("construction") == argv[arg_idx])
    {
      test_camera_construction();
    }
    else if (std::string("get_ray") == argv[arg_idx])
    {
      test_camera_get_ray();
    }
    else
    {
      // - ERROR -
      throw std::runtime_error{"unrecognized argument"};
    }
  }

  return 0;
}/*}}}*/

