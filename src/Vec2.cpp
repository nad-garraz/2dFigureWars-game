#include "Vec2.h"

Vec2::Vec2(float xin, float yin)
    : x(xin)
    , y(yin){};

// booleans
bool  Vec2::operator==(const Vec2 &rhs) const { return (x == rhs.x && y == rhs.y); };

bool  Vec2::operator!=(const Vec2 &rhs) const { return (x != rhs.x || y != rhs.y); };

// basic arithmetics
Vec2  Vec2::operator+(const Vec2 &rhs) const { return Vec2(x + rhs.x, y + rhs.y); };

Vec2  Vec2::operator-(const Vec2 &rhs) const { return Vec2(x - rhs.x, y - rhs.y); };

Vec2  Vec2::operator*(const float val) const { return Vec2(val * this->x, val * this->y); };

Vec2  Vec2::operator/(const float val) const { return Vec2(x / val, y / val); };

// basic arithmetics unary operators
Vec2 &Vec2::operator+=(const Vec2 &v)
{
  this->x += v.x;
  this->y += v.y;
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
float Vec2::dist(const Vec2 &v) const { return sqrtf((x - v.x) * (x - v.x) + (y - v.y) * (y - v.y)); };

// Quadratic Distance between to points or vectors
float Vec2::distQ(const Vec2 &v) const { return (x - v.x) * (x - v.x) + (y - v.y) * (y - v.y); };

// Vector length
float Vec2::length() const { return sqrtf(this->x * this->x + this->y * this->y); };

Vec2  Vec2::normalize()
{
  if (!this->length()) return *this;
  float a  = 1.0 / this->length();
  this->x *= a;
  this->y *= a;

  return Vec2(this->x, this->y);
};

void Vec2::print() const { printf("VEC: (%f, %f)\n", this->x, this->y); };

void Vec2::print(const std::string &s) const { printf("%s: (%f, %f)\n", s.c_str(), this->x, this->y); };
