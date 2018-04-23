
#ifndef RENDER_RAY_H
#define RENDER_RAY_H

#include <iostream>
#include "point.h"
#include "vector.h"

namespace render {

template <typename TYPE>
class Ray
{/*{{{*/
public:
  Ray() = delete;
  Ray(const Point<TYPE> &a_origin,const Vector<TYPE> &a_direction) :
    m_origin{a_origin}, m_direction{a_direction} {}
  Ray(const Ray &a_ray) = default;
  Ray &operator = (const Ray &a_ray) = delete;

  bool operator == (const Ray &a_ray) const
  {/*{{{*/
    return m_origin == a_ray.m_origin && m_direction == a_ray.m_direction;
  }/*}}}*/

  const Point<TYPE> &origin() const { return m_origin; }
  const Vector<TYPE> &direction() const { return m_direction; }
  Point<TYPE> hit_point(TYPE a_distance) const { return m_origin + m_direction*a_distance; }

  void format_to_stream(std::ostream &a_os) const
  {/*{{{*/
    a_os << "Ray{origin=" << m_origin << ",direction=" << m_direction << "}";
  }/*}}}*/

private:
  Point<TYPE> m_origin;
  Vector<TYPE> m_direction;
};/*}}}*/

template <typename TYPE>
std::ostream &operator << (std::ostream &a_os,const Ray<TYPE> &a_ray)
{/*{{{*/
  a_ray.format_to_stream(a_os);
  return a_os;
}/*}}}*/

} // namespace render

#endif

