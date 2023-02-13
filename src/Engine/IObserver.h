#pragma once

#include <SFML/Graphics.hpp>

class IObserver
{
public:
  virtual ~IObserver(){};
  virtual void UpdateEvent(sf::Event event) = 0;
  virtual void UpdatePositionMouse(sf::Vector2f pos) = 0;
};