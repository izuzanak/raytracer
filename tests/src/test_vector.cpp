
#include "tests.h"
#include "vector.h"

using render::Vector;

void test_vector_construction()
{/*{{{*/
  Vector<float> vec1;
  Vector<float> vec2{0,0,0};
  assert(vec1 == vec2);

  Vector<float> vec3{1.0,2.0,3.0};

  std::ostringstream oss;
  oss << vec3;
  assert(oss.str() == "Vector{1,2,3}");

  assert(vec3.x == 1.0f);
  assert(vec3.y == 2.0f);
  assert(vec3.z == 3.0f);

  auto vec4{vec3};
  assert(vec4 == vec3);
}/*}}}*/

void test_vector_arithmetic()
{/*{{{*/
  Vector<int> vec1{1,2,3};
  Vector<int> vec2{3,2,1};

  assert(-vec1 == (Vector<int>{-1,-2,-3}));
  assert(-(-vec1) == (Vector<int>{1,2,3}));
  assert(vec1 * 10 == (Vector<int>{10,20,30})); 
  assert((Vector<float>{1.0,2.0,3.0}) / 2.0 == (Vector<float>{0.5,1.0,1.5})); 
  assert(vec1 - vec2 == (Vector<int>{-2,0,2})); 
  assert(vec1 + vec2 == (Vector<int>{4,4,4})); 
  assert(vec1.dot(vec2) == 10); 
  assert(vec1.cross(vec2) == (Vector<int>{-4,8,-4})); 

  bool res1 = (Vector<int>{1,2,3} += Vector<int>{1,2,3}) == Vector<int>{2,4,6};
  assert(res1);
}/*}}}*/

void test_vector_length()
{/*{{{*/
  Vector<int> vec1{3,4,5};
  assert(vec1.length() == 7);
  assert(vec1.length_pow2() == 50);
}/*}}}*/

void test_vector_normalize()
{/*{{{*/
  Vector<float> vec1{4,5,6};

  float length = std::sqrt(vec1.x*vec1.x + vec1.y*vec1.y + vec1.z*vec1.z);
  assert(vec1.normalize() == (Vector<float>{vec1.x/length,vec1.y/length,vec1.z/length}));
}/*}}}*/

void test_vector_all()
{/*{{{*/
  test_vector_construction();
  test_vector_arithmetic();
  test_vector_length();
  test_vector_normalize();
}/*}}}*/

int main(int argc,char **argv)
{/*{{{*/
  for (int arg_idx = 1;arg_idx < argc;++arg_idx)
  {
    if (std::string("all") == argv[arg_idx])
    {
      test_vector_all();
    }
    else if (std::string("construction") == argv[arg_idx])
    {
      test_vector_construction();
    }
    else if (std::string("arithmetic") == argv[arg_idx])
    {
      test_vector_arithmetic();
    }
    else if (std::string("length") == argv[arg_idx])
    {
      test_vector_length();
    }
    else if (std::string("normalize") == argv[arg_idx])
    {
      test_vector_normalize();
    }
    else
    {
      // - ERROR -
      throw std::runtime_error{"unrecognized argument"};
    }
  }

  return 0;
}/*}}}*/

