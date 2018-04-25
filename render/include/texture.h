
#ifndef RENDER_TEXTURE_H
#define RENDER_TEXTURE_H

#include <iostream>
#include <cmath>
#include "vector.h"

namespace render {

template <typename TYPE>
class Texture
{/*{{{*/
public:
  enum class Type
  {/*{{{*/
    TYPE_0 = 0,
    TYPE_1,
    TYPE_2,
    TYPE_3,
    TYPE_4,
    TYPE_5,
  };/*}}}*/

  enum class Target
  {/*{{{*/
    AMBIENT = 0,
    NORMAL,
  };/*}}}*/

  Texture() = delete;
  Texture(Type a_type,Target a_target,TYPE a_frequency,TYPE a_amplitude) :
    m_type{a_type},m_target{a_target},m_frequency{a_frequency},m_amplitude{a_amplitude} {}
  Texture(const Texture &a_texture) = default;
  Texture &operator = (const Texture &a_texture) = delete;

  bool operator == (const Texture &a_texture) const
  {/*{{{*/
    return m_type == a_texture.m_type && m_target == a_texture.m_target &&
      m_frequency == a_texture.m_frequency && m_amplitude == a_texture.m_amplitude;
  }/*}}}*/

  Target target() { return m_target; }
  TYPE frequency() { return m_frequency; }
  TYPE amplitude() { return m_amplitude; }
  TYPE sample(const Point<TYPE> &a_point) const;

  void format_to_stream(std::ostream &a_os) const
  {/*{{{*/
    a_os << "Texture{type=" << static_cast<int>(m_type)
         << ",target=" << static_cast<int>(m_target)
         << ",frequency=" << m_frequency
         << ",amplitude=" << m_amplitude << "}";
  }/*}}}*/

private:
  static TYPE cosine_interpolation(TYPE a,TYPE b,TYPE x);
  static TYPE integer_noise(const Point<int> &a_point);
  static TYPE interpolated_noise(const Point<TYPE> &a_point);
  static TYPE perlin_noise(const Point<TYPE> &a_point);

private:
  Type m_type;
  Target m_target;
  TYPE m_frequency;
  TYPE m_amplitude;
};/*}}}*/

template <typename TYPE>
TYPE Texture<TYPE>::sample(const Point<TYPE> &a_point) const
{/*{{{*/
  switch (m_type)
  {
  case Type::TYPE_0:
    {
      return perlin_noise(a_point);
    }
  case Type::TYPE_1:
    {
      return std::abs(2*(interpolated_noise(a_point) - 0.5));
    }
  case Type::TYPE_2:
    {
      TYPE value = perlin_noise(a_point)*20.0;
      return value - std::floor(value);
    }
  case Type::TYPE_3:
    {
      TYPE value = perlin_noise(a_point)*20.0;
      value -= std::floor(value);
      TYPE value1 = interpolated_noise(a_point*250.0);
      return 0.65*value + 0.35*value1;
    }
  case Type::TYPE_4:
    {
      return 0.5*(1.0 + std::cos(10*a_point.x + 30.0*perlin_noise(a_point)));
    }
  case Type::TYPE_5:
    {
      return 0.5*(1.0 + std::cos(a_point.x + 300.0*perlin_noise(a_point)));
    }
  default:
    return 0.0;
  }
}/*}}}*/

template <typename TYPE>
std::ostream &operator << (std::ostream &a_os,const Texture<TYPE> &a_texture)
{/*{{{*/
  a_texture.format_to_stream(a_os);
  return a_os;
}/*}}}*/

template <typename TYPE>
TYPE Texture<TYPE>::cosine_interpolation(TYPE a,TYPE b,TYPE x)
{/*{{{*/
  TYPE r1 = (1.0 - std::cos(x*3.1415927))*0.5;
  return a*(1.0 - r1) + b*r1;
}/*}}}*/

template <typename TYPE>
TYPE Texture<TYPE>::integer_noise(const Point<int> &a_point)
{/*{{{*/
  int n = a_point.x + a_point.y * 1249 + a_point.z * 26237;

  n = (n << 13) ^ n;
  return ((n * (n * n * 15731 + 789221) + 1376312589) & 0x7fffffff) / 2147483648.0;
}/*}}}*/

template <typename TYPE>
TYPE Texture<TYPE>::interpolated_noise(const Point<TYPE> &a_point)
{/*{{{*/
  int ix = a_point.x;
  int iy = a_point.y;
  int iz = a_point.z;

  TYPE rfx = a_point.x - ix;
  TYPE rfy = a_point.y - iy;
  TYPE rfz = a_point.z - iz;

  return cosine_interpolation(
    cosine_interpolation(
      cosine_interpolation(
        integer_noise({ix,iy,iz}),
        integer_noise({ix + 1,iy,iz}),
        rfx),
      cosine_interpolation(
        integer_noise({ix,iy + 1,iz}),
        integer_noise({ix + 1,iy + 1,iz}),
        rfx),
      rfy),
    cosine_interpolation(
      cosine_interpolation(
        integer_noise({ix,iy,iz + 1}),
        integer_noise({ix + 1,iy,iz + 1}),
        rfx),
      cosine_interpolation(
        integer_noise({ix,iy + 1,iz + 1}),
        integer_noise({ix + 1,iy + 1,iz + 1}),
        rfx),
      rfy),
    rfz);
}/*}}}*/

template <typename TYPE>
TYPE Texture<TYPE>::perlin_noise(const Point<TYPE> &a_point)
{/*{{{*/
  TYPE result = 0.0;

  result += interpolated_noise(a_point)*0.5;
  result += interpolated_noise(a_point*2)*0.25;
  result += interpolated_noise(a_point*4)*0.125;
  result += interpolated_noise(a_point*8)*0.0625;

  return result;
}/*}}}*/

} // namespace render

#endif

