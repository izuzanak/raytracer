
#ifndef RENDER_VECTOR3_H
#define RENDER_VECTOR3_H

#include <iostream>
#include <cmath>
#include "point.h"

namespace render {

template <typename TYPE>
class Vector
{/*{{{*/
public:
  TYPE x;
  TYPE y;
  TYPE z;

  Vector() : x{}, y{}, z{} {}
  Vector(TYPE a_x,TYPE a_y,TYPE a_z) : x{a_x}, y{a_y}, z{a_z} {}
  Vector(const Vector &a_vec) = default;
  Vector &operator = (const Vector &a_vec) = default;

  bool operator == (const Vector &a_vec) const
  {/*{{{*/
    return a_vec.x == x && a_vec.y == y && a_vec.z == z;
  }/*}}}*/

  Vector operator - () const { return {-x,-y,-z}; }
  Vector operator * (TYPE a_val) const { return {x*a_val,y*a_val,z*a_val}; }
  Vector operator / (TYPE a_val) const { return {x/a_val,y/a_val,z/a_val}; }
  Vector operator - (const Vector &a_vec) const { return {x - a_vec.x,y - a_vec.y,z - a_vec.z}; }
  Vector operator + (const Vector &a_vec) const { return {x + a_vec.x,y + a_vec.y,z + a_vec.z}; }
  TYPE dot(const Vector &a_vec) const { return x*a_vec.x + y*a_vec.y + z*a_vec.z; }
  Vector cross(const Vector &a_vec) const
  {/*{{{*/
    return {y*a_vec.z - z*a_vec.y,
            z*a_vec.x - x*a_vec.z,
            x*a_vec.y - y*a_vec.x};
  }/*}}}*/
  Vector &operator += (const Vector &a_vec) { x += a_vec.x; y += a_vec.y; z += a_vec.z; return *this; }
  Vector &operator += (TYPE a_val) { x += a_val; y += a_val; z += a_val; return *this; }
  Vector &operator /= (TYPE a_val) { x /= a_val; y /= a_val; z /= a_val; return *this; }

  TYPE length_pow2() const { return x*x + y*y + z*z; }
  TYPE length() const { return std::sqrt(x*x + y*y + z*z); }
  Vector normalize() const { return (*this)/length(); }

  void format_to_stream(std::ostream &a_os) const
  {/*{{{*/
    a_os << "Vector{" << x << ',' << y << ',' << z << "}";
  }/*}}}*/
};/*}}}*/

template <typename TYPE>
std::ostream &operator << (std::ostream &a_os,const Vector<TYPE> &a_vec)
{/*{{{*/
  a_vec.format_to_stream(a_os);
  return a_os;
}/*}}}*/

} // namespace render

#endif

