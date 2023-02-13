#include "SubEventWidget.h"

void
SubEventWidget::Attach(IObserver* observer)
{
  m_list_observer.push_back(observer);
}

void
SubEventWidget::Detach(IObserver* observer)
{
  m_list_observer.remove(observer);
}

void
SubEventWidget::UpdateEvent(sf::Event event)
{
  for (auto observe : m_list_observer) {
    observe->UpdateEvent(event);
  }
}

void
SubEventWidget::UpdatePositionMouse(sf::Vector2f pos)
{
  for (auto observe : m_list_observer) {
    observe->UpdatePositionMouse(pos);
  }
}