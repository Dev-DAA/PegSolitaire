#pragma once

#include "../CoreGame/CoreGame.h"
#include "../Utils/Singletone.h"
#include "IObserver.h"
#include "SubEventWidget.h"
#include <SFML/Graphics.hpp>

class CoreGame;

class Widget
  : public sf::Drawable
  , public IObserver
{
public:
  Widget();

public:
  // Фукнция обновления событий
  void UpdateEvent(sf::Event event) override;
  // Фукнция обновления позиции курсора
  void UpdatePositionMouse(sf::Vector2f pos) override;
  // Переопределённый метод для отрисовки
  void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
  // TODO возможно понадобится для обратки клика на сцене
  void SetCallbackFunc(void(func)(bool wasClick));
  // Функция отписки от обновлений
  void RemoveFromTheList();
  // Функция возвращает true если фокус установлен
  bool GetFocus() const;
  // Устанавливает шрифт
  void SetFont(const sf::Font font);
  // Устанавливает размер символа
  void SetCharacterSize(uint8_t size);
  // Устанавливает цвет текста
  void SetColorText(sf::Color color);
  // Устанавливает строку для виджета
  void SetText(std::string str);
  // Устанавливает позицию
  void SetPosition(sf::Vector2f pos);
  // Устанавлиет размеры
  void SetSize(sf::Vector2f size);

private:
  void CheckFocus();
  void SetPositionText();

private:
  // Положение мыши на экране
  sf::Vector2f m_posMouse;
  // Фокус
  bool m_focus = false;
  // Кнопка была зажата
  bool m_pressed = false;
  // Указатель на callback функцию
  void (*callbackFunc)(bool wasClick);
  // Текст для виджета
  sf::Text m_text;
  // Шрифт
  sf::Font m_font;
  // Цвет текста
  sf::Color m_colorText;
  // Прямоугольник виджета
  sf::RectangleShape m_widgetForm;
};