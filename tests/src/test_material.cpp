
#include <vector>
#include "tests.h"
#include "material.h"

using render::Color;
using render::Material;

void test_material_construction()
{/*{{{*/
  Material<float> material1{{0.15,0.15,0.0},0.3,0.3,3,0.0,0.0};

  std::ostringstream oss;
  oss << material1;
  assert(oss.str() == "Material{ambient=Color{0.15,0.15,0},diffuse=0.3,specular_k=0.3,specular_n=3,reflection=0,refraction=0}");
}/*}}}*/

void test_material_vector()
{/*{{{*/
  std::vector<Material<float>> materials;

  materials.emplace_back(Color<float>{0.15,0.15,0.0},0.3,0.3,3,0.0,0.0);
  materials.push_back({Color<float>{0.15,0.15,0.0},0.3,0.3,3,0.0,0.0});
}/*}}}*/

void test_material_all()
{/*{{{*/
  test_material_construction();
  test_material_vector();
}/*}}}*/

int main(int argc,char **argv)
{/*{{{*/
  for (int arg_idx = 1;arg_idx < argc;++arg_idx)
  {
    if (std::string("all") == argv[arg_idx])
    {
      test_material_all();
    }
    else if (std::string("construction") == argv[arg_idx])
    {
      test_material_construction();
    }
    else if (std::string("vector") == argv[arg_idx])
    {
      test_material_vector();
    }
    else
    {
      // - ERROR -
      throw std::runtime_error{"unrecognized argument"};
    }
  }

  return 0;
}/*}}}*/

