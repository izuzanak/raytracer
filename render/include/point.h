
#ifndef RENDER_POINT3_H
#define RENDER_POINT3_H

#include <iostream>
#include "vector.h"

namespace render {

template <typename TYPE>
class Vector;

template <typename TYPE>
class Point
{/*{{{*/
public:
  TYPE x;
  TYPE y;
  TYPE z;

  Point() : x{}, y{}, z{} {}
  Point(TYPE a_x,TYPE a_y,TYPE a_z) : x{a_x}, y{a_y}, z{a_z} {}
  Point(const Point &a_point) = default;
  Point &operator = (const Point &a_point) = default;

  bool operator == (const Point &a_point) const
  {/*{{{*/
    return a_point.x == x && a_point.y == y && a_point.z == z;
  }/*}}}*/

  Vector<TYPE> operator - (const Point &a_point) const { return {x - a_point.x,y - a_point.y,z - a_point.z}; }
  Point operator * (TYPE a_val) const { return {x*a_val,y*a_val,z*a_val}; }
  Point operator + (const Vector<TYPE> &a_vec) const { return {x + a_vec.x,y + a_vec.y,z + a_vec.z}; }
  Point absolute() const { return {std::abs(x),std::abs(y),std::abs(z)}; }

  void format_to_stream(std::ostream &a_os) const
  {/*{{{*/
    a_os << "Point{" << x << ',' << y << ',' << z << "}";
  }/*}}}*/
};/*}}}*/

template <typename TYPE>
std::ostream &operator << (std::ostream &a_os,const Point<TYPE> &a_point)
{/*{{{*/
  a_point.format_to_stream(a_os);
  return a_os;
}/*}}}*/

} // namespace render

#endif

