
#ifndef RENDER_SHAPE_H
#define RENDER_SHAPE_H

#include <iostream>
#include <vector>
#include "ray.h"
#include "material.h"

namespace render {
namespace shape {

using tex_idxs_t = std::vector<uint32_t>;

template <typename TYPE>
class Shape
{/*{{{*/
public:
  Shape() = delete;
  explicit Shape(uint32_t a_mat_idx,tex_idxs_t &&a_tex_idxs) : 
    m_mat_idx{a_mat_idx}, m_tex_idxs{a_tex_idxs} {}
  Shape(const Shape &a_shape) = delete;

  Shape &operator = (const Shape &a_shape) = delete;
  virtual ~Shape() = default;

  uint32_t material_idx() { return m_mat_idx; }
  const tex_idxs_t &texture_idxs() { return m_tex_idxs; }

  virtual bool hit(const Ray<TYPE> &a_ray,TYPE *a_distance) = 0;
  virtual Vector<TYPE> normal(const Point<TYPE> &a_point) = 0;

  virtual void format_to_stream(std::ostream &a_os) const
  {/*{{{*/
    a_os << "Shape{material_idx=" << m_mat_idx << "}";
  }/*}}}*/

private:
  uint32_t m_mat_idx;
  tex_idxs_t m_tex_idxs;
};/*}}}*/

template <typename TYPE>
std::ostream &operator << (std::ostream &a_os,const Shape<TYPE> &a_shape)
{/*{{{*/
  a_shape.format_to_stream(a_os);
  return a_os;
}/*}}}*/

} // namespace shape
} // namespace render

#endif

