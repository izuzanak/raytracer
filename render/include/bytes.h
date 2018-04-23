
#ifndef RENDER_BYTES_H
#define RENDER_BYTES_H

namespace render {

const uint16_t kTwoBytes = 0xff00;
const bool kLittleEndian = reinterpret_cast<const uint8_t *>(&kTwoBytes)[1] == 0xff;

class Bytes
{/*{{{*/
public:
  static uint16_t swap16(uint16_t a_value)
  {/*{{{*/
    return ((a_value >> 8) & 0xff) |
           ((a_value & 0xff) << 8);
  }/*}}}*/

  static uint32_t swap32(uint32_t a_value)
  {/*{{{*/
    return ((a_value & 0xff000000) >> 24) |
           ((a_value & 0x00ff0000) >>  8) |
           ((a_value & 0x0000ff00) <<  8) |
           ((a_value & 0x000000ff) << 24);
  }/*}}}*/

  static uint16_t little16(uint16_t a_value)
  {/*{{{*/
    return kLittleEndian ? a_value : swap16(a_value);
  }/*}}}*/

  static uint32_t little32(uint32_t a_value)
  {/*{{{*/
    return kLittleEndian ? a_value : swap32(a_value);
  }/*}}}*/
};/*}}}*/

} // namespace render

#endif

