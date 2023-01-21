#pragma once

#include "stdint.h"
#include <string>

namespace COMMON {

// Возможные состояния ячейки
enum class ECell : uint8_t
{
  LOCK = 0,
  FREE = 1,
  SET = 2
};

// Игрок или компьютер
enum class EPlayerType
{
  HUMAN = 0,
  COMPUTER = 1
};

enum class EGameMode
{
  SINGLE = 0,
  MULTIPLAYER = 1
};

enum class EDirect
{
  RIGHT = 0,
  LEFT = 1,
  UP = 2,
  DOWN = 3,
  UNKNOWN = 4,
};

enum class EGameState
{
  MENU = 0,
  GAME = 1,
};
}