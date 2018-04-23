
#ifndef RENDER_SHAPE_SPHERE_H
#define RENDER_SHAPE_SPHERE_H

#include <iostream>
#include <limits>
#include "shape.h"

namespace render {
namespace shape {

template <typename TYPE>
class Sphere : public Shape<TYPE>
{/*{{{*/
public:
  Sphere() = delete;
  Sphere(const Point<TYPE> &a_center,TYPE a_radius,uint32_t a_mat_idx,tex_idxs_t &&a_tex_idxs) :
    Shape<TYPE>{a_mat_idx,std::move(a_tex_idxs)},
    m_center{a_center},
    m_radius{a_radius},
    m_radius_pow{a_radius*a_radius} {}
  Sphere(TYPE a_x,TYPE a_y,TYPE a_z,TYPE a_radius,uint32_t a_mat_idx,tex_idxs_t &&a_tex_idxs) :
    Sphere{{a_x,a_y,a_z},a_radius,a_mat_idx,std::move(a_tex_idxs)} {}

  Sphere(const Sphere &a_sphere) = delete;
  Sphere &operator = (const Sphere &a_sphere) = delete;

  bool operator == (const Sphere &a_sphere) const
  {/*{{{*/
    return m_center == a_sphere.m_center && m_radius == a_sphere.m_radius;
  }/*}}}*/

  ~Sphere() override = default;

  bool hit(const Ray<TYPE> &a_ray,TYPE *a_distance) override;
  Vector<TYPE> normal(const Point<TYPE> &a_point) override
  {/*{{{*/
    return (a_point - m_center)/m_radius;
  }/*}}}*/

  void format_to_stream(std::ostream &a_os) const override
  {/*{{{*/
    a_os << "Sphere{center=" << m_center << ",radius=" << m_radius << "}";
  }/*}}}*/

private:
  Point<TYPE> m_center;
  TYPE m_radius;
  TYPE m_radius_pow;
};/*}}}*/

template <typename TYPE>
bool Sphere<TYPE>::hit(const Ray<TYPE> &a_ray,TYPE *a_distance)
{/*{{{*/
  Vector<TYPE> L = m_center - a_ray.origin();
  TYPE tca = L.dot(a_ray.direction());
  TYPE d_pow = L.length_pow2() - tca*tca;

  if (d_pow > m_radius_pow)
  {
    return false;
  }

  TYPE thc = std::sqrt(m_radius_pow - d_pow);
  TYPE t0 = tca - thc;
  TYPE t1 = tca + thc;

  // - sort intersections -
  if (t0 > t1)
  {
    std::swap(t0,t1);
  }

  // - first intersection is negative -
  if (t0 < 0)
  {
    // - second intersection is negative -
    if (t1 < 0)
    {
      return false;
    }

    *a_distance = t1;
  }
  else
  {
    *a_distance = t0;
  }

  return true;
};/*}}}*/

template <typename TYPE>
std::ostream &operator << (std::ostream &a_os,const Sphere<TYPE> &a_sphere)
{/*{{{*/
  a_sphere.format_to_stream(a_os);
  return a_os;
}/*}}}*/

} // namespace shape
} // namespace render

#endif

