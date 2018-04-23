
#include "tests.h"
#include "color.h"

using render::Color;

void test_color3_construction()
{/*{{{*/
  Color<float> color1;
  Color<float> color2{0,0,0};
  assert(color1 == color2);

  std::ostringstream oss;
  oss << color1;
  assert(oss.str() == "Color{0,0,0}");
}/*}}}*/

void test_color3_all()
{/*{{{*/
  test_color3_construction();
}/*}}}*/

int main(int argc,char **argv)
{/*{{{*/
  for (int arg_idx = 1;arg_idx < argc;++arg_idx)
  {
    if (std::string("all") == argv[arg_idx])
    {
      test_color3_all();
    }
    else if (std::string("construction") == argv[arg_idx])
    {
      test_color3_construction();
    }
    else
    {
      // - ERROR -
      throw std::runtime_error{"unrecognized argument"};
    }
  }

  return 0;
}/*}}}*/

