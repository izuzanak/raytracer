
#ifndef RENDER_CAMERA_H
#define RENDER_CAMERA_H

#include <iostream>
#include "ray.h"

namespace render {

template <typename TYPE>
class Camera
{/*{{{*/
public:
  Camera() = delete;
  Camera(const Point<TYPE> &a_position,const Point<TYPE> &a_look_at,const Vector<TYPE> &a_up,
      TYPE a_left,TYPE a_bottom,TYPE a_distance);
  Camera(const Camera &a_cam) = delete;
  Camera &operator = (const Camera &a_cam) = delete;

  Ray<TYPE> get_ray(TYPE a_x,TYPE a_y)
  {/*{{{*/
    return {m_position,((m_uv_begin + m_u*a_x + m_v*a_y) - m_position).normalize()};
  }/*}}}*/

  void format_to_stream(std::ostream &a_os) const
  {/*{{{*/
    a_os << "Camera{position=" << m_position
         << ",look_at=" << m_look_at
         << ",up=" << m_up << "}";
  }/*}}}*/

private:
  void recompute_uvw()
  {/*{{{*/
    m_w = (m_position - m_look_at).normalize();
    m_u = m_up.cross(m_w).normalize();
    m_v = m_w.cross(m_u);
    m_uv_begin = m_position + (m_u*m_left + m_v*m_bottom - m_w*m_distance);
  }/*}}}*/

private:
  Point<TYPE> m_position;
  Point<TYPE> m_look_at;
  Vector<TYPE> m_up;
  TYPE m_left;
  TYPE m_bottom;
  TYPE m_distance;

  Vector<TYPE> m_u;
  Vector<TYPE> m_v;
  Vector<TYPE> m_w;
  Point<TYPE> m_uv_begin;
};/*}}}*/

template <typename TYPE>
std::ostream &operator << (std::ostream &a_os,const Camera<TYPE> &a_cam)
{/*{{{*/
  a_cam.format_to_stream(a_os);
  return a_os;
}/*}}}*/

template <typename TYPE>
Camera<TYPE>::Camera(const Point<TYPE> &a_position,const Point<TYPE> &a_look_at,const Vector<TYPE> &a_up,
    TYPE a_left,TYPE a_bottom,TYPE a_distance) :
  m_position{a_position},m_look_at{a_look_at},m_up{a_up},m_left{a_left},
  m_bottom{a_bottom},m_distance{a_distance}
{/*{{{*/
  recompute_uvw();
}/*}}}*/

} // namespace render

#endif

