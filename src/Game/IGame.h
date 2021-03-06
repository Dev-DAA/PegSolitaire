#pragma once

#include "../Common/Common.h"
#include "../Field/Field.h"
#include "Player.h"
#include "stdint.h"

#include <functional>
#include <glog/logging.h>
#include <iostream>
#include <memory>
#include <vector>

class IGame
{
public:
  // Конструктор
  IGame() = default;
  // Деструктор
  virtual ~IGame(){};

public:
  // Получить информацию о игровом режиме
  COMMON::EGameMode GetGameMode() const;
  // Создание новой игры
  virtual bool CreateNewGame();
  // Выполнение хода
  virtual bool DoMove(uint8_t x,
                      uint8_t y,
                      const std::vector<COMMON::EDirect>& directions);
  // Загрузить копию поля
  void SetField(const Field& field);
  // Получить копию поля
  Field GetField();
  // Проверка возможности указанного хода/последовательности ходов
  bool CheckMove(uint8_t x,
                 uint8_t y,
                 const std::vector<COMMON::EDirect>& directions);
  // Проверка конца игры
  bool IsGameOver();
  // Установить список игроков
  void SetPlayerList(const std::vector<Player>& lists_player);
  // Получить список игроков
  std::vector<Player> GetPlayerList() const;
  // Получить номер актиавного пользователя
  uint8_t GetActivePlayerId() const;

protected:
  // Проверить одиночный ход
  bool CheckMove(uint8_t x, uint8_t y, COMMON::EDirect direction, Field& field);
  // Выполнить ход в копии поля
  bool DoMove(uint8_t x, uint8_t y, COMMON::EDirect direction, Field& field);
  // Сдвинуть координаты на ход куда походили
  bool MoveShift(uint8_t& x, uint8_t& y, COMMON::EDirect direction) const;
  // Установить вспомогательные координаты в зависимости от направления
  bool CheckShift(int8_t& x, int8_t& y, COMMON::EDirect direction) const;

protected:
  // Копия экземпляра Field
  Field m_field;
  // Cписок пользователей
  std::vector<Player> m_list_player;
  // Активный пользователь
  uint8_t m_active_user = 0;
  // Режим игры
  COMMON::EGameMode m_mode_game;
};