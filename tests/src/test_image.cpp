
#include "tests.h"
#include "image.h"

using render::Image;
using render::Color;

void test_image_construction()
{/*{{{*/
  Image<float> image1{640,480};

  assert(image1.width() == 640);
  assert(image1.height() == 480);

  std::ostringstream oss;
  oss << image1;
  assert(oss.str() == "Image{width=640,height=480}");
}/*}}}*/

void test_image_indexing()
{/*{{{*/
  uint32_t width = 640;
  uint32_t height = 480;

  Color<float> col1{1.0,0.0,0.0};
  Image<float> image1{width,height};

  image1.fill(col1);
  assert(image1(0,0) == col1);
  assert(image1.at(0,0) == col1);

  assert(image1(width - 1,height - 1) == col1);
  assert(image1.at(width - 1,height - 1) == col1);

  try { image1.at(width,0); assert(false); } catch (...) {}
  try { image1.at(0,height); assert(false); } catch (...) {}
}/*}}}*/

void test_image_save_bmp_file()
{/*{{{*/
  uint32_t width = 640;
  uint32_t height = 480;

  Color<float> col1{0.0,0.0,1.0};
  Image<float> image1{width,height};

  image1.fill(col1);
  image1.save_bmp_file("test.bmp");
}/*}}}*/

void test_image_all()
{/*{{{*/
  test_image_construction();
  test_image_indexing();
  test_image_save_bmp_file();
}/*}}}*/

int main(int argc,char **argv)
{/*{{{*/
  for (int arg_idx = 1;arg_idx < argc;++arg_idx)
  {
    if (std::string("all") == argv[arg_idx])
    {
      test_image_all();
    }
    else if (std::string("construction") == argv[arg_idx])
    {
      test_image_construction();
    }
    else if (std::string("indexing") == argv[arg_idx])
    {
      test_image_indexing();
    }
    else if (std::string("save_bmp_file") == argv[arg_idx])
    {
      test_image_save_bmp_file();
    }
    else
    {
      // - ERROR -
      throw std::runtime_error{"unrecognized argument"};
    }
  }

  return 0;
}/*}}}*/

