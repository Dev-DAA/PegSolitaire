#pragma once

#include "Common.h"
#include <SFML/Graphics.hpp>

// Размер поля
const int SIZE = 7;

/// Минимальный размер игрового поля, ячеек
const int MIN_CELLS = 7;
/// Максимальный размер игрового поля, ячеек
const int MAX_CELLS = 16;

/// Макрос пересчёта градусов в радианы
#define DEG2RAD(ang) ((ang)*3.14159f / 180.f)

/// Горизонтальный угол наклона, радианы
const float SKEW_ANGLE_H = DEG2RAD(15.f);
/// Вертикальный угол наклона, радианы
const float SKEW_ANGLE_V = DEG2RAD(-5.f);

#pragma region "Размеры активной области"
/**
 * Размеры активной области подобраны так, чтобы получить целое количество точек
 * для размера ячеек. Иниыми словами размеры активной области должны нацело
 * делиться на { @see MAX_CELLS }
 */
/// Ширина активной области
const int ACTIVE_RECT_WIDTH = 1440;
/// Высота активной области
const int ACTIVE_RECT_HEIGHT = 960;
#pragma endregion

#pragma region "Трансформации"
template<class T>
sf::Transform
ToOrigin(sf::Vector2<T> size)
{
  sf::Transform tr;
  tr.translate(-size.x / 2.f, -size.y / 2.f);
  return tr;
}

template<class T>
sf::Transform
FromOrigin(sf::Vector2<T> size)
{
  return ToOrigin(size).getInverse();
}

template<class T>
sf::Transform
AlignToCenter(sf::Rect<T> obj, sf::Rect<T> dest)
{
  sf::Transform tr;

  sf::Vector2<T> destCenter(dest.left + dest.width / 2.f, dest.top + dest.height / 2.f);
  sf::Vector2<T> objCenter(obj.left + obj.width / 2.f, obj.top + obj.height / 2.f);

  tr.translate(destCenter.x - objCenter.x, destCenter.y - objCenter.y);
  return tr;
}

// DEPRECATED, DUMMY
// template<class T>
// sf::Transform
// MoveToScreenCenter(sf::Rect<T> obj, sf::Rect<T> screen){ return AlignToCenter(obj, screen) }

sf::Transform
Skew(float angleH, float angleV);

COMMON::EDirect
GetDirectNextCell(uint8_t x, uint8_t y, uint8_t x_new, uint8_t y_new);

#pragma endregion

/**
 * @brief Получение размеров прямоугольника
 *
 * @tparam T Тип данных
 * @param [in] rect Прямоугольник
 * @return Возвращает вектор с размерами указанного прямоугольника
 */
template<class T>
sf::Vector2<T>
RectSize(const sf::Rect<T>& rect)
{
  return sf::Vector2<T>(rect.width, rect.height);
}

template<class T>
void
MoveToScreenCenter(sf::RectangleShape* obj, sf::Rect<T> screen)
{
  sf::Vector2<T> screenCenter(screen.left + screen.width / 2.f, screen.top + screen.height / 2.f);

  sf::Vector2f positionObj = obj->getPosition();
  sf::Vector2f sizeObj = obj->getSize();

  sf::Vector2<T> objCenter(positionObj.x + sizeObj.x / 2.f, positionObj.y + sizeObj.y / 2.f);

  float offset_x = screenCenter.x - objCenter.x;
  float offset_y = screenCenter.y - objCenter.y;

  obj->setPosition(offset_x, offset_y);
}

// void
// CalculatePoints(sf::VertexArray& m_points,
//                 sf::Vector2f m_size,
//                 sf::Vector2f position,
//                 uint32_t m_cellSize)
// {
//   /// Кол-во точек для всех линий
//   m_points.resize(2 * ((m_size.x + m_size.y) / m_cellSize + 2));

//   uint32_t index = 0;

//   /// Смещение к центру по оси х
//   uint32_t offset = uint32_t(m_size.x / 2) % m_cellSize;
//   for (float i = position.x; i < (m_size.x + position.x); i += m_cellSize, index += 2) {
//     m_points[index].position = sf::Vector2f(i + offset, 0);
//     m_points[index + 1].position = sf::Vector2f(i + offset, m_size.y);
//   }
//   /// Смещение к центру по оси y
//   offset = uint32_t(m_size.y / 2) % m_cellSize;
//   for (float i = position.y; i < m_size.y + position.y; i += m_cellSize, index += 2) {
//     m_points[index].position = sf::Vector2f(0, i + offset);
//     m_points[index + 1].position = sf::Vector2f(m_size.x, i + offset);
//   }
// }