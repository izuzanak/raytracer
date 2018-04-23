
#include <iostream>
#include "raytracer_demo.h"

int main(int argc,char **argv)
{
  // - check command line arguments -
  if (argc < 3)
  {
    std::cerr << argv[0]
      << ": expecting exactly two arguments: <scene_file> <output_file>"
      << std::endl;

    std::exit(1);
  }

  // - render scene to output file -
  if (!rt_demo::RaytracerDemo{argv[0],argv[1],argv[2]}.render())
  {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

