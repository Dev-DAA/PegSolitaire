#include "Player.h"

Player::Player(const std::string& name, uint16_t score, COMMON::EPlayerType type)
{
  SetName(name);
  SetBeginScore(score);
  SetEntity(type);
}

Player::~Player() {}

bool
Player::SetName(const std::string& name)
{
  // Проверяем пустая ли строка
  if (name.empty()) {
    LOG(ERROR) << "Player::SetName : Empty name!";
    return false;
  }
  // Записываем имя
  m_name = name;
  return true;
}

bool
Player::SetEntity(const COMMON::EPlayerType type)
{
  // Проверка типа
  if ((type != COMMON::EPlayerType::HUMAN) && (type != COMMON::EPlayerType::COMPUTER)) {
    LOG(ERROR) << "Player::SetEssence : Invalid field type value in SetEssence()!";
    return false;
  }
  // Устанавливаем сущность
  m_essence = type;
  return true;
}

bool
Player::SetBeginScore(const uint16_t score)
{
  if (score > MAX_SCORE) {
    LOG(ERROR) << "Player::SetBeginScore : Ivalid begin data score < 0 ";
    return false;
  }
  // Обновляем счёт
  m_score = score;
  return true;
}

bool
Player::AddPoints(const int16_t score)
{
  if (((int16_t)m_score + score) < 0) {
    LOG(ERROR) << "Player::AddPoints : Ivalid data score < 0 ";
    return false;
  }
  // Обновляем счёт
  m_score = (int16_t)m_score + score;
  return true;
}

std::string
Player::GetName() const
{
  // Возвращаем имя
  return m_name;
}

COMMON::EPlayerType
Player::GetEntity() const
{
  // Возвращаем сущность
  return m_essence;
}

uint16_t
Player::GetScore() const
{
  // Возвращаем счёт
  return m_score;
}

bool
operator==(const Player& lhs, const Player& rhs)
{
  return lhs.m_name == rhs.m_name && lhs.m_essence == rhs.m_essence && lhs.m_score == rhs.m_score;
}