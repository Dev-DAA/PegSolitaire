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
  return COMMON::EDirect::UNKNOW;
}