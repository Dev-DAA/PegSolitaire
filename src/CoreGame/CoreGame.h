#pragma once

#include "../Engine/Common.h"
#include "../Engine/Field.h"
#include "../Engine/Grid.h"
#include "../Engine/IGame.h"
#include "../Engine/Math.h"
#include "../Engine/Player.h"
#include "../Utils/Singletone.h"

#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>
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
  sf::Vector2f m_PosAciveCell;
  std::vector<sf::RectangleShape> m_ActiveCells;
  // Размер ячейки
  uint32_t m_cellSize;

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