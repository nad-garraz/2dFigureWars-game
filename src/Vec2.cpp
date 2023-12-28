#include "Vec2.h"

Vec2::Vec2(float xin, float yin)
    : x(xin),
      y(yin){};

// booleans
bool Vec2::operator==(const Vec2 &rhs) const
{
  return (x == rhs.x && y == rhs.y);
};

bool Vec2::operator!=(const Vec2 &rhs) const
{
  return (x != rhs.x || y != rhs.y);
};

// basic arithmetics
Vec2 Vec2::operator+(const Vec2 &rhs) const
{
  return Vec2(x + rhs.x, y + rhs.y);
};

Vec2 Vec2::operator-(const Vec2 &rhs) const
{
  return Vec2(x - rhs.x, y - rhs.y);
};

Vec2 Vec2::operator*(const float val) const
{
  return Vec2(val * x, val * y);
};

Vec2 Vec2::operator/(const float val) const
{
  return Vec2(x / val, y / val);
};

// basic arithmetics unary operators
Vec2 &Vec2::operator+=(const Vec2 &v)
{
  x += v.x;
  y += v.y;
  return *this;
};

Vec2 &Vec2::operator-=(const Vec2 &v)
{
  x -= v.x;
  y -= v.y;
  return *this;
};

Vec2 &Vec2::operator*=(const float val)
{
  x *= val;
  y *= val;
  return *this;
};

Vec2 &Vec2::operator/=(const float val)
{
  x /= val;
  y /= val;
  return *this;
};

// Distance between to points or vectors
float Vec2::dist(const Vec2 &v) const
{
  return sqrtf((x - v.x) * (x - v.x) + (y - v.y) * (y - v.y));
};

// Vector length
float Vec2::length() const
{
  return sqrtf(x * x + y * y);
};

void Vec2::normalize()
{
  float a = 1.0 / this->length();
  this->x *= a;
  this->y *= a;
};


void Vec2::print() const
{
  printf("(%f, %f)\n",this->x, this->y);
}
