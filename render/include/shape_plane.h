
#ifndef RENDER_SHAPE_PLANE_H
#define RENDER_SHAPE_PLANE_H

#include <iostream>
#include <limits>
#include "shape.h"

namespace render {
namespace shape {

template <typename TYPE>
class Plane : public Shape<TYPE>
{/*{{{*/
public:
  Plane() = delete;
  Plane(const Point<TYPE> a_point,const Vector<TYPE> &a_normal,
      uint32_t a_mat_idx,tex_idxs_t &&a_tex_idxs) :
    Shape<TYPE>{a_mat_idx,std::move(a_tex_idxs)},
    m_point{a_point},m_normal{a_normal.normalize()} {}
  Plane(TYPE a_px,TYPE a_py,TYPE a_pz,TYPE a_nx,TYPE a_ny,TYPE a_nz,
      uint32_t a_mat_idx,tex_idxs_t &&a_tex_idxs) :
    Plane{{a_px,a_py,a_pz},{a_nx,a_ny,a_nz},a_mat_idx,std::move(a_tex_idxs)} {};

  Plane(const Plane &a_plane) = delete;
  Plane &operator = (const Plane &a_plane) = delete;

  bool operator == (const Plane &a_plane) const
  {/*{{{*/
    return m_point == a_plane.m_point && m_normal == a_plane.m_normal;
  }/*}}}*/

  ~Plane() override = default;

  bool hit(const Ray<TYPE> &a_ray,TYPE *a_distance) override;
  Vector<TYPE> normal(const Point<TYPE> &a_point) override // NOLINT
  {/*{{{*/
    return m_normal;
  }/*}}}*/

  void format_to_stream(std::ostream &a_os) const override
  {/*{{{*/
    a_os << "Plane{point=" << m_point << ",normal=" << m_normal << "}";
  }/*}}}*/

private:
  Point<TYPE> m_point;
  Vector<TYPE> m_normal;
};/*}}}*/

template <typename TYPE>
bool Plane<TYPE>::hit(const Ray<TYPE> &a_ray,TYPE *a_distance)
{/*{{{*/
  TYPE denom = -m_normal.dot(a_ray.direction());

  if (denom >= 0)
  {
    *a_distance = -(m_point - a_ray.origin()).dot(m_normal) / denom;

    return *a_distance > 0;
  }

  return false;
};/*}}}*/

template <typename TYPE>
std::ostream &operator << (std::ostream &a_os,const Plane<TYPE> &a_plane)
{/*{{{*/
  a_plane.format_to_stream(a_os);
  return a_os;
}/*}}}*/

} // namespace shape
} // namespace render

#endif

