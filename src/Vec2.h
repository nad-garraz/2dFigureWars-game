#ifndef VEC2_H
#define VEC2_H

#include <cmath>
#include <iostream>
#include <string>

class Vec2
{
public:
  float x{}, y{};

  // Constructors
  Vec2(){};
  Vec2(float xin, float yin);

  // Boolean Operators
  bool  operator==(const Vec2 &rhs) const;
  bool  operator!=(const Vec2 &rhs) const;

  Vec2  operator+(const Vec2 &rhs) const;
  Vec2  operator-(const Vec2 &rhs) const;
  Vec2  operator/(const float val) const;
  Vec2  operator*(const float val) const;

  Vec2 &operator+=(const Vec2 &rhs);
  Vec2 &operator-=(const Vec2 &rhs);
  Vec2 &operator*=(const float val);
  Vec2 &operator/=(const float val);
  Vec2 &scale(const float &val);

  float distQ(const Vec2 &rhs) const;
  float dist(const Vec2 &v) const;
  float length() const;
  Vec2  normalize();

  void  print() const;
  void  print(const std::string &s) const;
};

#endif // !VEC2_H
