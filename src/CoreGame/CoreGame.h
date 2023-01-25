#pragma once

#include "../Engine/Common.h"
#include "../Engine/Field.h"
#include "../Engine/Grid.h"
#include "../Engine/IGame.h"
#include "../Engine/Math.h"
#include "../Engine/Player.h"
#include "../Utils/Singletone.h"

#include <SFML/Graphics.hpp>
#include <algorithm>
#include <cmath>
#include <iostream>
#include <sstream>
#include <vector>

class CoreGame
{
public:
  // Конструктор
  CoreGame(/* args */);
  // Деструктор
  ~CoreGame();
  // Метод для старта игры
  void StartGame();

private:
  // Проверка выбранной повторной ячейки
  bool CheckRepeatedCell(sf::Vector2i ActiveCell);

private:
  // Координаты последней выбранной ячейки
  sf::Vector2f m_PosLastCell;
  // Массив выбранных ячеек
  std::vector<sf::RectangleShape> m_ActiveCells;
  // Массив направления хода
  std::vector<COMMON::EDirect> m_DirectCells;
  // Размер ячейки
  uint32_t m_cellSize;
  // Шрифт
  sf::Font m_font;
  // Текст для очков
  sf::Text m_TextScore;
  // Текст game over
  sf::Text m_TextGameOver;
  // Размер шрифта
  const uint8_t SIZE_FONT = 25;
  // Проверка конца игры
  bool m_IsGameOver = false;

  COMMON::EGameState m_StateGame = COMMON::EGameState::MENU;

  sf::RenderWindow m_window;
  sf::FloatRect m_screenRect;
  sf::FloatRect m_activeRectAfterTransform;

  sf::Vector2f m_activeRectSize;
  sf::Vector2f m_coverSize;

  sf::Transform m_gridTr;
  sf::Transform m_activeTr;

  sf::FloatRect m_RectCellMouse;
};