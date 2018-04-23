
#ifndef RENDER_MATERIAL_H
#define RENDER_MATERIAL_H

#include <iostream>
#include "color.h"
#include "ray.h"
#include "light.h"

namespace render {

template <typename TYPE>
class Material
{/*{{{*/
public:
  Material() = delete;
  Material(const Color<TYPE> &a_ambient,TYPE a_diffuse_k,TYPE a_specular_k,
      uint32_t a_specular_n,TYPE a_reflection,TYPE a_refraction) :
    m_ambient{a_ambient},
    m_diffuse_k{a_diffuse_k},
    m_specular_k{a_specular_k},
    m_specular_n{a_specular_n},
    m_reflection{a_reflection},
    m_refraction{a_refraction} {}
  Material(const Material &a_mat) = default;
  Material &operator = (const Material &a_mat) = delete;

  Color<TYPE> &ambient() { return m_ambient; }
  TYPE reflection() { return m_reflection; }

  Color<TYPE> phong_model(const Vector<TYPE> &a_light_vec,
      const Light<TYPE> &a_light,const Ray<TYPE> &a_ray,const Vector<TYPE> &a_normal);

  void format_to_stream(std::ostream &a_os) const
  {/*{{{*/
    a_os << "Material{ambient=" << m_ambient
         << ",diffuse=" << m_diffuse_k
         << ",specular_k=" << m_specular_k
         << ",specular_n=" << m_specular_n
         << ",reflection=" << m_reflection
         << ",refraction=" << m_refraction << "}";
  }/*}}}*/

private:
  Color<TYPE> m_ambient;
  TYPE m_diffuse_k;
  TYPE m_specular_k;
  uint32_t m_specular_n;
  TYPE m_reflection;
  TYPE m_refraction;
};/*}}}*/

template <typename TYPE>
Color<TYPE> Material<TYPE>::phong_model(const Vector<TYPE> &a_light_vec,
    const Light<TYPE> &a_light,const Ray<TYPE> &a_ray,const Vector<TYPE> &a_normal)
{/*{{{*/
  Color<TYPE> color;

  TYPE nkl = a_light_vec.dot(a_normal);
  if (nkl > 0)
  {
    color += a_light.color()*(m_diffuse_k*nkl);

    TYPE ekrtn_tmp = (a_light_vec - a_normal*2*nkl).dot(a_ray.direction());
    if (ekrtn_tmp > 0)
    {
      color += a_light.color()*(m_specular_k*std::pow(ekrtn_tmp,m_specular_n));
    }
  }

  return color;
}/*}}}*/

template <typename TYPE>
std::ostream &operator << (std::ostream &a_os,const Material<TYPE> &a_mat)
{/*{{{*/
  a_mat.format_to_stream(a_os);
  return a_os;
}/*}}}*/

} // namespace render

#endif

