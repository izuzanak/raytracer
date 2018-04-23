
#include "tests.h"
#include "bytes.h"

using render::Bytes;

void test_bytes_swap()
{/*{{{*/
  uint16_t original16 = 0xaabb;
  uint16_t swapped16 = 0xbbaa;
  assert(original16 == Bytes::swap16(swapped16));

  uint32_t original32 = 0xaabbccdd;
  uint32_t swapped32 = 0xddccbbaa;
  assert(original32 == Bytes::swap32(swapped32));
}/*}}}*/

void test_bytes_endianness()
{/*{{{*/
  uint16_t original16 = 0xaabb;
  uint32_t original32 = 0xaabbccdd;

  if (render::kLittleEndian)
  {
    assert(Bytes::little16(original16) == original16);
    assert(Bytes::little32(original32) == original32);
  }
  else
  {
    assert(Bytes::little16(original16) == Bytes::swap16(original16));
    assert(Bytes::little32(original32) == Bytes::swap32(original32));
  }
}/*}}}*/

void test_bytes_all()
{/*{{{*/
  test_bytes_swap();
  test_bytes_endianness();
}/*}}}*/

int main(int argc,char **argv)
{/*{{{*/
  for (int arg_idx = 1;arg_idx < argc;++arg_idx)
  {
    if (std::string("all") == argv[arg_idx])
    {
      test_bytes_all();
    }
    else if (std::string("swap") == argv[arg_idx])
    {
      test_bytes_swap();
    }
    else if (std::string("endianness") == argv[arg_idx])
    {
      test_bytes_endianness();
    }
    else
    {
      // - ERROR -
      throw std::runtime_error{"unrecognized argument"};
    }
  }

  return 0;
}/*}}}*/

