
#ifndef RENDER_LIGHT_H
#define RENDER_LIGHT_H

#include <iostream>
#include "point.h"
#include "color.h"

namespace render {

template <typename TYPE>
class Light
{/*{{{*/
public:
  Light() = delete;
  Light(const Point<TYPE> &a_position,const Color<TYPE> &a_color) :
    m_position{a_position}, m_color{a_color} {}
  Light(const Light &a_light) = default;
  Light &operator = (const Light &a_light) = delete;

  const Point<TYPE> &position() const { return m_position; }
  const Color<TYPE> &color() const { return m_color; }

  void format_to_stream(std::ostream &a_os) const
  {/*{{{*/
    a_os << "Light{position=" << m_position << ",color=" << m_color << "}";
  }/*}}}*/

private:
  Point<TYPE> m_position;
  Color<TYPE> m_color;
};/*}}}*/

template <typename TYPE>
std::ostream &operator << (std::ostream &a_os,const Light<TYPE> &a_light)
{/*{{{*/
  a_light.format_to_stream(a_os);
  return a_os;
}/*}}}*/

} // namespace render

#endif

