
#ifndef RENDER_SHAPE_TRIANGLE_H
#define RENDER_SHAPE_TRIANGLE_H

#include <iostream>
#include <limits>
#include "shape.h"

namespace render {
namespace shape {

template <typename TYPE>
class Triangle : public Shape<TYPE>
{/*{{{*/
public:
  Triangle() = delete;
  Triangle(const Point<TYPE> &a_v0,
      const Point<TYPE> &a_v1,
      const Point<TYPE> &a_v2,
      uint32_t a_mat_idx,
      tex_idxs_t &&a_tex_idxs) :
    Shape<TYPE>{a_mat_idx,std::move(a_tex_idxs)},
    m_v0{a_v0},m_v1{a_v1},m_v2{a_v2},
    m_edge1{m_v1 - m_v0},
    m_edge2{m_v2 - m_v0},
    m_normal{m_edge2.cross(m_edge1).normalize()} {}
  Triangle(TYPE a_v0x,TYPE a_v0y,TYPE a_v0z,
           TYPE a_v1x,TYPE a_v1y,TYPE a_v1z,
           TYPE a_v2x,TYPE a_v2y,TYPE a_v2z,uint32_t a_mat_idx,tex_idxs_t &&a_tex_idxs) :
    Triangle{{a_v0x,a_v0y,a_v0z},{a_v1x,a_v1y,a_v1z},{a_v2x,a_v2y,a_v2z},
      a_mat_idx,std::move(a_tex_idxs)} {}

  Triangle(const Triangle &a_triangle) = delete;
  Triangle &operator = (const Triangle &a_triangle) = delete;

  bool operator == (const Triangle &a_triangle) const
  {/*{{{*/
    return m_v0 == a_triangle.m_v0 && m_v1 == a_triangle.m_v1 && m_v2 == a_triangle.m_v2;
  }/*}}}*/

  ~Triangle() override = default;

  bool hit(const Ray<TYPE> &a_ray,TYPE *a_distance) override;
  Vector<TYPE> normal(const Point<TYPE> &a_point) override // NOLINT
  {/*{{{*/
    return m_normal;
  }/*}}}*/

  void format_to_stream(std::ostream &a_os) const override
  {/*{{{*/
    a_os << "Triangle{v0=" << m_v0 << ",v1=" << m_v1 << ",v2=" << m_v2 << "}";
  }/*}}}*/

private:
  Point<TYPE> m_v0;
  Point<TYPE> m_v1;
  Point<TYPE> m_v2;

  Vector<TYPE> m_edge1;
  Vector<TYPE> m_edge2;
  Vector<TYPE> m_normal;
};/*}}}*/

template <typename TYPE>
bool Triangle<TYPE>::hit(const Ray<TYPE> &a_ray,TYPE *a_distance)
{/*{{{*/
  Vector<TYPE> pvec = a_ray.direction().cross(m_edge2);
  TYPE det = m_edge1.dot(pvec);

  if (det == 0)
  {
    return false;
  }

  TYPE inv_det = 1/det;
  Vector<TYPE> tvec = a_ray.origin() - m_v0;
  TYPE u = tvec.dot(pvec)*inv_det;

  if (u < 0 || u > 1)
  {
    return false;
  }

  Vector<TYPE> qvec = tvec.cross(m_edge1);
  TYPE v = a_ray.direction().dot(qvec)*inv_det;

  if (v < 0 || u + v > 1)
  {
    return false;
  }

  *a_distance = m_edge2.dot(qvec)*inv_det;

  return *a_distance > 0;
};/*}}}*/

template <typename TYPE>
std::ostream &operator << (std::ostream &a_os,const Triangle<TYPE> &a_triangle)
{/*{{{*/
  a_triangle.format_to_stream(a_os);
  return a_os;
}/*}}}*/

} // namespace shape
} // namespace render

#endif

