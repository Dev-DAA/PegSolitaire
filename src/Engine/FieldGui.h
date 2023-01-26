#pragma once

#include "../Utils/Singletone.h"
#include "Common.h"
#include "Field.h"
#include "IGame.h"
#include "Math.h"
#include <SFML/Graphics.hpp>
#include <iostream>

class FieldGui : public sf::Drawable
{
public:
  FieldGui();
  // Перегруженный метод для отрисовки
  void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
  // Метод для обновления всех отрисовывающихся элементов
  void update();
  // Обновляем альфу для подсветки ячеек
  void SetAlpha(uint8_t alpha);
  // Устанавливает позицию ячейки на которую указывает курсор
  void SetPositionMouse(sf::Vector2f pos);
  // Добавить выбранную ячейку
  void AddActiveCell(sf::Vector2f pos, bool firstCell = false);
  // Возвращает true если нет активных ячеек
  bool GetEmptyActiveCells();
  // Очищаем массив с активными ячейками
  void ClearActiveCells();
  // Вернёт true если последня выбранная ячейка была нажата второй раз
  bool CheckLastClick(sf::Vector2i posActiveCell);
  // Получить ячейку с которой началось направление
  sf::Vector2f GetFirstActiveCell();

private:
  // Поле которое мы отрисовываем
  Field m_field;
  // Размер ячейки в пикселях
  uint32_t m_cellSize;
  // Размеры закрашиваемой ячейки в пикселях
  uint32_t m_sizeColorCell;
  // Смещение поля в пикселях
  uint32_t m_OffsetScreen_W;
  uint32_t m_OffsetScreen_H;
  // Смещеение поля в ячейках
  uint32_t m_OffsetCell_W;
  uint32_t m_OffsetCell_H;
  // Ячейка на которую указывает курсор
  sf::RectangleShape m_MouseCell;

  // Массив выбранных ячеек
  std::vector<sf::RectangleShape> m_ActiveCells;
};