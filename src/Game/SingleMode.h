#pragma once

#include "IGame.h"

class SingleMode : public IGame
{
public:
  // Коструктор
  SingleMode();
  // Деструктор
  ~SingleMode() = default;

public:
  // Переопределяем создание новой игры
  bool CreateNewGame() override;
  // Вызываем сначала метод IGame, после свой функционал
  bool DoMove(uint8_t x,
              uint8_t y,
              const std::vector<COMMON::EDirect>& directions) override;
};