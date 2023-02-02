#pragma once

#include "../Utils/Singletone.h"
#include "IObserver.h"
#include "SubEventWidget.h"
#include <SFML/Graphics.hpp>

class Widget
  : public sf::RectangleShape
  , public IObserver
{
public:
  Widget();

public:
  void UpdateEvent(sf::Event event) override;
  void UpdatePositionMouse(sf::Vector2f pos) override;

  // TODO возможно понадобится для обратки клика на сцене
  void SetCallbackFunc(void (*func)(bool wasClick));

  void RemoveFromTheList();
  bool GetFocus() const;

private:
  void CheckFocus();

private:
  // Положение мыши на экране
  sf::Vector2f m_posMouse;
  // Фокус
  bool m_focus = false;
  // Кнопка была зажата
  bool m_pressed = false;
  // Указатель на callback функцию
  void (*callbackFunc)(bool wasClick);
};