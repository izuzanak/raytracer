
#ifndef RENDER_COLOR3_H
#define RENDER_COLOR3_H

#include <iostream>

namespace render {

template <typename TYPE>
class Color
{/*{{{*/
public:
  TYPE r;
  TYPE g;
  TYPE b;

  Color() : r{}, g{}, b{} {}
  Color(TYPE a_r,TYPE a_g,TYPE a_b) : r{a_r}, g{a_g}, b{a_b} {}
  Color(const Color &a_color) = default;
  Color &operator = (const Color &a_color) = default;

  bool operator == (const Color &a_color) const
  {/*{{{*/
    return a_color.r == r && a_color.g == g && a_color.b == b;
  }/*}}}*/

  Color operator * (TYPE a_val) const { return {r*a_val,g*a_val,b*a_val}; }
  Color operator + (const Color &a_color) const { return {r + a_color.r,g + a_color.g,b + a_color.b}; }
  Color &operator += (const Color &a_color) { r += a_color.r; g += a_color.g; b += a_color.b; return *this; }
  Color &crop_to_ones()
  {/*{{{*/
    r = std::min<TYPE>(r,1);
    g = std::min<TYPE>(g,1);
    b = std::min<TYPE>(b,1);
    return *this;
  }/*}}}*/

  void format_to_stream(std::ostream &a_os) const
  {/*{{{*/
    a_os << "Color{" << r << ',' << g << ',' << b << "}";
  }/*}}}*/
};/*}}}*/

template <typename TYPE>
std::ostream &operator << (std::ostream &a_os,const Color<TYPE> &a_color)
{/*{{{*/
  a_color.format_to_stream(a_os);
  return a_os;
}/*}}}*/

} // namespace render

#endif

