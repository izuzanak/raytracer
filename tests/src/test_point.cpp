
#include "tests.h"
#include "point.h"

using render::Point;
using render::Vector;

void test_point_construction()
{/*{{{*/
  Point<float> point1;
  Point<float> point2{0,0,0};
  assert(point1 == point2);

  Point<float> point3{1.0,2.0,3.0};

  std::ostringstream oss;
  oss << point3;
  assert(oss.str() == "Point{1,2,3}");

  assert(point3.x == 1.0f);
  assert(point3.y == 2.0f);
  assert(point3.z == 3.0f);
}/*}}}*/

void test_point_arithmetic()
{/*{{{*/
  Point<int> point1{1,2,3};
  assert(point1 - (Point<int>{1,2,3}) == (Vector<int>{0,0,0}));
  assert(point1 - (Point<int>{-1,-2,-3}) == (Vector<int>{2,4,6}));
  assert(point1 + (Vector<int>{-1,-2,-3}) == (Point<int>{0,0,0}));
  assert(point1 + (Vector<int>{1,2,3}) == (Point<int>{2,4,6}));
  assert(point1 + (-Vector<int>{1,2,3}) == (Point<int>{0,0,0}));

  assert((point1 - Point<int>{1,2,5}).length() == 2);
  assert((point1 - Point<int>{4,2,7}).length() == 5);
}/*}}}*/

void test_point_all()
{/*{{{*/
  test_point_construction();
  test_point_arithmetic();
}/*}}}*/

int main(int argc,char **argv)
{/*{{{*/
  for (int arg_idx = 1;arg_idx < argc;++arg_idx)
  {
    if (std::string("all") == argv[arg_idx])
    {
      test_point_all();
    }
    else if (std::string("construction") == argv[arg_idx])
    {
      test_point_construction();
    }
    else if (std::string("arithmetic") == argv[arg_idx])
    {
      test_point_arithmetic();
    }
    else
    {
      // - ERROR -
      throw std::runtime_error{"unrecognized argument"};
    }
  }

  return 0;
}/*}}}*/

