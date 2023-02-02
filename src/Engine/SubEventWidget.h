#pragma once

#include "IObserver.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <list>

class SubEventWidget
{
public:
  void Attach(IObserver* observer);
  void Detach(IObserver* observer);
  void UpdateEvent(sf::Event event);
  void UpdatePositionMouse(sf::Vector2f pos);

private:
  std::list<IObserver*> m_list_observer;
};
