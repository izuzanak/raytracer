
#ifndef RENDER_IMAGE_H
#define RENDER_IMAGE_H

#include <iostream>
#include <fstream>
#include <vector>
#include "bytes.h"
#include "color.h"

namespace render {

template <typename TYPE>
class Image
{/*{{{*/
public:
  Image() = delete;
  Image(uint32_t a_width,uint32_t a_height) :
    m_width{a_width}, m_height{a_height}, m_data(a_width*a_height) {};
  Image(const Image &a_image) = delete;
  Image &operator = (const Image &a_image) = delete;

  void fill(const Color<TYPE> &a_color) { std::fill(m_data.begin(),m_data.end(),a_color); }

  Color<TYPE> &operator () (uint32_t a_x,uint32_t a_y) { return m_data[m_width*a_y + a_x]; }
  Color<TYPE> &at(uint32_t a_x,uint32_t a_y);

  uint32_t width() { return m_width; }
  uint32_t height() { return m_height; }
  Color<TYPE> *data() { return m_data.data(); }

  void save_bmp_file(const std::string &a_file_name);

  void format_to_stream(std::ostream &a_os) const
  {/*{{{*/
    a_os << "Image{width=" << m_width << ",height=" << m_height << "}";
  }/*}}}*/

private:
  uint32_t m_width;
  uint32_t m_height;
  std::vector<Color<TYPE>> m_data;
};/*}}}*/

template <typename TYPE>
Color<TYPE> &Image<TYPE>::at(uint32_t a_x,uint32_t a_y)
{/*{{{*/
  if (a_x >= m_width || a_y >= m_height)
  {
    throw std::runtime_error{"Image: invalid coordinates"};
  }

  return m_data.at(m_width*a_y + a_x);
}/*}}}*/

template <typename TYPE>
void Image<TYPE>::save_bmp_file(const std::string &a_file_name)
{/*{{{*/
#pragma pack(1)
  struct BmpFileHeader
  {/*{{{*/
    uint16_t type;
    uint32_t size;
    uint16_t res_1;
    uint16_t res_2;
    uint32_t off_bits;
  };/*}}}*/
#pragma pack()

#pragma pack(1)
  struct BmpImageHeader
  {/*{{{*/
    uint32_t size;
    uint32_t width;
    uint32_t height;
    uint16_t planes;
    uint16_t bit_cnt;
    uint32_t compression;
    uint32_t image_size;
    uint32_t x_pel_per_meter;
    uint32_t y_pel_per_meter;
    uint32_t clr_used;
    uint32_t clr_important;
  };/*}}}*/
#pragma pack()

  // - scanline aligned to 4 bytes -
  uint32_t alignment = (4 - ((m_width*3) & 3)) & 3;
  uint32_t image_size = m_width*m_height*3 + m_height*alignment;

  // - fill bmp file header -
  BmpFileHeader bmp_fh{
    Bytes::little16(19778),
    Bytes::little32(image_size + 54),
    Bytes::little16(0),
    Bytes::little16(0),
    Bytes::little32(54)};

  // - fill bmp image header -
  BmpImageHeader bmp_ih{
    Bytes::little32(40),
    Bytes::little32(m_width),
    Bytes::little32(m_height),
    Bytes::little16(1),
    Bytes::little16(24),
    Bytes::little32(0),
    Bytes::little32(image_size),
    Bytes::little32(0),
    Bytes::little32(0),
    Bytes::little32(0),
    Bytes::little32(0)};

  // - create bitmap data b8g8r8 -
  std::vector<uint8_t> bmp_data(image_size);

  if (m_width > 0 && m_height > 0)
  {
    auto i_bmp = bmp_data.begin();
    auto i_img = m_data.begin();
    auto i_img_end = m_data.end();
    do {
      auto i_img_w_end = i_img + m_width;
      do {
        i_bmp[0] = i_img->b*255.0;
        i_bmp[1] = i_img->g*255.0;
        i_bmp[2] = i_img->r*255.0;
      } while(i_bmp += 3,++i_img < i_img_w_end);

      i_bmp += alignment;
    } while(i_img < i_img_end);
  }

  // - write binary data to file -
  std::ofstream file(a_file_name,std::fstream::out | std::fstream::binary);
  file.write(reinterpret_cast<char *>(&bmp_fh),sizeof(bmp_fh));
  file.write(reinterpret_cast<char *>(&bmp_ih),sizeof(bmp_ih));
  file.write(reinterpret_cast<char *>(bmp_data.data()),bmp_data.size());
  file.close();

  // - ERROR -
  if (!file)
  {
    throw std::runtime_error{"Image: error while writing image to file"};
  }
}/*}}}*/

template <typename TYPE>
std::ostream &operator << (std::ostream &a_os,const Image<TYPE> &a_image)
{/*{{{*/
  a_image.format_to_stream(a_os);
  return a_os;
}/*}}}*/

} // namespace render

#endif

