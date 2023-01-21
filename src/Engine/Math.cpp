#include "Math.h"

sf::Transform
Skew(float angleH, float angleV)
{
  return sf::Transform{ 1.f,          sinf(angleH), 0.f, /**/
                        sinf(angleV), 1.f,          0.f, /**/
                        0.f,          0.f,          1.f };
}

COMMON::EDirect
GetDirectNextCell(uint8_t x, uint8_t y, uint8_t x_new, uint8_t y_new)
{
  auto x_diff = x_new - x;
  auto y_diff = y_new - y;

  if ((x_diff == 2) && (y_diff == 0)) {
    return COMMON::EDirect::RIGHT;
  }
  if ((x_diff == -2) && (y_diff == 0)) {
    return COMMON::EDirect::LEFT;
  }
  if ((x_diff == 0) && (y_diff == -2)) {
    return COMMON::EDirect::UP;
  }
  if ((x_diff == 0) && (y_diff == 2)) {
    return COMMON::EDirect::DOWN;
  }

  return COMMON::EDirect::UNKNOWN;
}