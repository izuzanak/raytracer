
#include "tests.h"
#include "texture.h"
#include "image.h"

using render::Texture;
using render::Image;

void test_texture_construction()
{/*{{{*/
  Texture<float> tex1{Texture<float>::Type::TYPE_0,Texture<float>::Target::AMBIENT,0.15,0.2};

  std::ostringstream oss;
  oss << tex1;
  assert(oss.str() == "Texture{type=0,target=0,frequency=0.15,amplitude=0.2}");
}/*}}}*/

void test_texture_sample()
{/*{{{*/
  uint32_t width = 800;
  uint32_t height = 600;

  Texture<float> tex1{Texture<float>::Type::TYPE_3,Texture<float>::Target::NORMAL,0.01,1.0};
  Image<float> image1{width,height};

  for (uint32_t y = 0;y < height;++y)
  {
    for (uint32_t x = 0;x < width;++x)
    {
      float value = tex1.sample({
        static_cast<float>(x*0.01),
        static_cast<float>(y*0.01),
        1000});
      image1(x,y) = {value,value,value};
    }
  }

  image1.save_bmp_file("texture.bmp");
}/*}}}*/

void test_texture_all()
{/*{{{*/
  test_texture_construction();
  test_texture_sample();
}/*}}}*/

int main(int argc,char **argv)
{/*{{{*/
  for (int arg_idx = 1;arg_idx < argc;++arg_idx)
  {
    if (std::string("all") == argv[arg_idx])
    {
      test_texture_all();
    }
    else if (std::string("construction") == argv[arg_idx])
    {
      test_texture_construction();
    }
    else if (std::string("sample") == argv[arg_idx])
    {
      test_texture_sample();
    }
    else
    {
      // - ERROR -
      throw std::runtime_error{"unrecognized argument"};
    }
  }

  return 0;
}/*}}}*/

