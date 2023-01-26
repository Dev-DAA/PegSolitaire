#pragma once

#include <SFML/Graphics.hpp>

class Grid : public sf::Drawable
{
public:
  /**
   * @brief Конструктор
   *
   * @param [in] coverSize Размер области, покрывающей экран
   * @param [in] cellSize Размер клетки в пикселях
   */
  Grid(sf::Vector2f coverSize, uint32_t cellSize);

  void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
  /**
   * @brief Рассчитывает координаты точек сетки
   */
  void CalculatePoints();

private:
  /// Размер области, покрывающей экран
  sf::Vector2f m_size;

  /// Размер клетки в пикселях
  uint32_t m_cellSize;

  sf::VertexArray m_points;
};