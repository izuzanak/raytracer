
#include "tests.h"
#include "light.h"

using render::Light;

void test_light_construction()
{/*{{{*/
  Light<float> light1{{0.0,0.0,0.0},{1.0,1.0,1.0}};

  std::ostringstream oss;
  oss << light1;
  assert(oss.str() == "Light{position=Point{0,0,0},color=Color{1,1,1}}");
}/*}}}*/

void test_light_all()
{/*{{{*/
  test_light_construction();
}/*}}}*/

int main(int argc,char **argv)
{/*{{{*/
  for (int arg_idx = 1;arg_idx < argc;++arg_idx)
  {
    if (std::string("all") == argv[arg_idx])
    {
      test_light_all();
    }
    else if (std::string("construction") == argv[arg_idx])
    {
      test_light_construction();
    }
    else
    {
      // - ERROR -
      throw std::runtime_error{"unrecognized argument"};
    }
  }

  return 0;
}/*}}}*/

