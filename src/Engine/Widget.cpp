#include "Widget.h"

Widget::Widget()
{
  // Подписываем виджет на уведомления
  SubEventWidget& subEventWidget = Singleton<SubEventWidget>::GetInstance();
  subEventWidget.Attach(this);
  // Дефолтные настройки фона и обводки
  m_widgetForm.setOutlineColor(sf::Color::White);
  m_widgetForm.setFillColor(sf::Color::Transparent);
  m_widgetForm.setOutlineThickness(3);
  m_text.setColor(sf::Color::Black);
  m_text.setCharacterSize(15);

  // Загрузка шрифта
  if (!m_font.loadFromFile("font.ttf")) {
    std::cout << ".Error while loading font" << std::endl;
  }
  m_text.setFont(m_font);
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
        callbackFunc(true);
        m_pressed = false;
        // std::cout << "Was ckick" << std::endl;
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
Widget::SetCallbackFunc(void (func)(bool wasClick))
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
  sf::Vector2f pos = m_widgetForm.getPosition();
  sf::Vector2f size = m_widgetForm.getSize();

  if (((m_posMouse.x >= pos.x) && (m_posMouse.y >= pos.y)) &&
      ((m_posMouse.x <= pos.x + size.x) && (m_posMouse.y <= pos.y + size.y))) {
    m_focus = true;
    sf::Color color(sf::Color::White);
    color.a = 200;
    m_widgetForm.setFillColor(color);
  } else {
    m_focus = false;
    m_widgetForm.setFillColor(sf::Color::Transparent);
  }
}

void
Widget::SetFont(const sf::Font font)
{
  m_font = font;
}

void
Widget::SetCharacterSize(uint8_t size)
{
  m_text.setCharacterSize(size);
}

void
Widget::SetColorText(sf::Color color)
{
  m_colorText = color;
}

void
Widget::SetText(std::string str)
{
  m_text.setString(str);

  sf::FloatRect rect = m_text.getLocalBounds();
  sf::Vector2f sizeWidget = m_widgetForm.getSize();

  auto offsetW = (float)(sizeWidget.x - rect.width) / 2;
  auto offsetH = (float)(sizeWidget.y - rect.height) / 2;

  sf::Vector2f posWidget = m_widgetForm.getPosition();

  m_text.setPosition(sf::Vector2f(posWidget.x + offsetW, posWidget.y + offsetH));
}

void
Widget::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
  target.draw(m_widgetForm, states);
  target.draw(m_text, states);
}

void
Widget::SetPosition(sf::Vector2f pos)
{
  m_widgetForm.setPosition(pos);
}
void
Widget::SetSize(sf::Vector2f size)
{
  m_widgetForm.setSize(size);
}