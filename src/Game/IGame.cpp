#include "IGame.h"

COMMON::EGameMode
IGame::GetGameMode() const
{
  // Вернём режим игры
  return m_mode_game;
}

bool
IGame::CreateNewGame()
{
  // Проверим пуст ли список игроков
  if (m_list_player.empty()) {
    LOG(WARNING) << "IGame::CreateNewGame : m_player_list empty!";
    return false;
  }
  // Должен быть один игрок в одиночном режиме
  if ((m_list_player.size() != 1) &&
      (m_mode_game == COMMON::EGameMode::SINGLE)) {
    LOG(WARNING)
      << "IGame::CreateNewGame : m_list_player != 1 and single mode!";
    return false;
  }
  // Должно быть больше одного игрока в мультиплеере
  if (((m_list_player.size() <= 1) || (m_list_player.size() > 4)) &&
      (m_mode_game == COMMON::EGameMode::MULTIPLAYER)) {
    LOG(WARNING)
      << "IGame::CreateNewGame : m_list_player < 1 and multiplayer mode!";
    return false;
  }
  // Проверим поле на нулевые размеры
  if ((m_field.GetHeight() == 0) || (m_field.GetWidth() == 0)) {
    LOG(WARNING) << "IGame::CreateNewGame : sizes field = 0!";
    return false;
  }

  return true;
}

bool
IGame::DoMove(uint8_t x,
              uint8_t y,
              const std::vector<COMMON::EDirect>& directions)
{
  // Проверим что последовательность ходов возможна
  if (!CheckMove(x, y, directions))
    return false;
  // Проходимся по списку направлений и меняем фишки
  for (auto dir = directions.begin(); dir != directions.end(); ++dir) {
    // В случае если нет возможности куда то походить будет false
    if (!DoMove(x, y, *dir, m_field))
      return false;
    // Сдвигаем координаты на ход который походили
    if (!MoveShift(x, y, *dir))
      return false;
  }

  return true;
}

void
IGame::SetField(const Field& field)
{
  m_field = field;
}

Field
IGame::GetField()
{
  return m_field;
}

bool
IGame::CheckMove(uint8_t x,
                 uint8_t y,
                 const std::vector<COMMON::EDirect>& directions)
{
  // скопируем объект
  Field copy_field = m_field;

  for (auto dir = directions.begin(); dir != directions.end(); ++dir) {
    // В случае если нет возможности куда то походить будет false
    if (!DoMove(x, y, *dir, copy_field))
      return false;
    // Сдвигаем координаты на ход который походили
    if (!MoveShift(x, y, *dir))
      return false;
  }

  return true;
}

bool
IGame::CheckMove(uint8_t x, uint8_t y, COMMON::EDirect direction, Field& field)
{
  int8_t xShift = 0;
  int8_t yShift = 0;

  if (!CheckShift(xShift, yShift, direction)) {
    LOG(WARNING) << "IGame::CheckMove : Checking one move!";
    return false;
  }

  int8_t xFar = (int8_t)x + 2 * xShift;
  int8_t yFar = (int8_t)y + 2 * yShift;

  if ((xFar < 0) || (xFar >= field.GetWidth()) || (yFar < 0) ||
      (yFar >= field.GetHeight())) {
    LOG(WARNING)
      << "IGame::CheckMove : Checking for negative numbers and out of range";
    return false;
  }

  // Проверим соответствие фишки, в случае выхода за диапазон будет false
  return (field.GetCell(x, y) == COMMON::ECell::SET) &&
         (field.GetCell(x + xShift, y + yShift) == COMMON::ECell::SET) &&
         (field.GetCell(x + 2 * xShift, y + 2 * yShift) == COMMON::ECell::FREE);
}

bool
IGame::DoMove(uint8_t x, uint8_t y, COMMON::EDirect direction, Field& field)
{
  uint8_t width = field.GetWidth();
  uint8_t height = field.GetHeight();
  int8_t xShift = 0;
  int8_t yShift = 0;

  // Проверяем диапазон
  if ((width <= x) || (height <= y)) {
    LOG(WARNING) << "IGame::DoMove : The entered coordinates are "
                    "out of range!";
    return false;
  }

  if (!CheckMove(x, y, direction, field)) {
    LOG(WARNING) << "IGame::DoMove : Checking one move!";
    return false;
  }

  if (!CheckShift(xShift, yShift, direction)) {
    LOG(WARNING) << "IGame::DoMove : Something with a direction!";
    return false;
  }

  field.SetCell(x, y, COMMON::ECell::FREE);
  field.SetCell(x + xShift, y + yShift, COMMON::ECell::FREE);
  field.SetCell(x + 2 * xShift, y + 2 * yShift, COMMON::ECell::SET);

  return true;
}

bool
IGame::MoveShift(uint8_t& x, uint8_t& y, COMMON::EDirect direction) const
{
  switch (direction) {
    case COMMON::EDirect::RIGHT:
      x += 2;
      break;
    case COMMON::EDirect::LEFT:
      x -= 2;
      break;
    case COMMON::EDirect::UP:
      y -= 2;
      break;
    case COMMON::EDirect::DOWN:
      y += 2;
      break;
    default:
      return false;
  }

  return true;
}

bool
IGame::CheckShift(int8_t& x, int8_t& y, COMMON::EDirect direction) const
{
  switch (direction) {
    case COMMON::EDirect::RIGHT:
      x = 1;
      break;
    case COMMON::EDirect::LEFT:
      x = -1;
      break;
    case COMMON::EDirect::UP:
      y = -1;
      break;
    case COMMON::EDirect::DOWN:
      y = 1;
      break;
    default:
      return false;
  }

  return true;
}

bool
IGame::IsGameOver()
{
  uint8_t width = m_field.GetWidth();
  uint8_t height = m_field.GetHeight();

  // Максимальное возможное кол-во направлений
  const int MAX_DIRECT = 4;

  // Проверим возможность хода куда нибудь, если такой вариант имеется, то хотя
  // бы один false вернётся
  for (int i(0); i < width; i++) {
    for (int j(0); j < height; j++) {
      for (int direct(0); direct < MAX_DIRECT; direct++) {
        if (CheckMove(i, j, static_cast<COMMON::EDirect>(direct), m_field))
          return false;
      }
    }
  }

  return true;
}

void
IGame::SetPlayerList(const std::vector<Player>& lists_player)
{
  // Установим список игроков
  m_list_player = lists_player;
}

std::vector<Player>
IGame::GetPlayerList() const
{
  // Вернём список игроков
  return m_list_player;
}

uint8_t
IGame::GetActivePlayerId() const
{
  return m_active_user;
}