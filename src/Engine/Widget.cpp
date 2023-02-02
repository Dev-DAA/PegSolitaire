#include "Widget.h"

Widget::Widget()
{
  // Подписываем виджет на уведомления
  SubEventWidget& subEventWidget = Singleton<SubEventWidget>::GetInstance();
  subEventWidget.Attach(this);
  // Дефолтные настройки фона и обводки
  setOutlineColor(sf::Color::White);
  setFillColor(sf::Color::Transparent);
  setOutlineThickness(3);
}

void
Widget::UpdateEvent(sf::Event event)
{
  if (!m_focus) {
    m_pressed = false;
    return;
  }

  switch (event.type) {
    case sf::Event::MouseButtonPressed: {
      m_pressed = true;
      break;
    }
    case sf::Event::MouseButtonReleased: {
      if (m_pressed) {
        // callbackFunc(true);
        m_pressed = false;
        std::cout << "Was ckick" << std::endl;
      }
      break;
    }
  }
}

void
Widget::UpdatePositionMouse(sf::Vector2f pos)
{
  m_posMouse = pos;
  CheckFocus();
}

void
Widget::SetCallbackFunc(void (*func)(bool wasClick))
{
  // Устанавливаем указатель на callback функцию
  callbackFunc = func;
}

void
Widget::RemoveFromTheList()
{
  // Отписываем виджет от уведомлений
  SubEventWidget& subEventWidget = Singleton<SubEventWidget>::GetInstance();
  subEventWidget.Detach(this);
}

bool
Widget::GetFocus() const
{
  return m_focus;
}

void
Widget::CheckFocus()
{
  sf::Vector2f pos = getPosition();
  sf::Vector2f size = getSize();

  if (((m_posMouse.x >= pos.x) && (m_posMouse.y >= pos.y)) &&
      ((m_posMouse.x <= pos.x + size.x) && (m_posMouse.y <= pos.y + size.y))) {
    m_focus = true;
    sf::Color color(sf::Color::White);
    color.a = 200;
    setFillColor(color);
  } else {
    m_focus = false;
    setFillColor(sf::Color::Transparent);
  }
}