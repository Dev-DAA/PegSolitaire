#include "CoreGame.h"

CoreGame::CoreGame(/* args */)
{
#pragma region "Размеры, прямоугольники, области и т.д."
  /// Видео режим, размеры которого совпадают с текущим разрешением
  sf::VideoMode screenMode = sf::VideoMode::getDesktopMode();

  /// Прямоугольник размером с экран
  m_screenRect = sf::FloatRect(0, 0, screenMode.width, screenMode.height);

  /**
   * @brief Активная область
   * @note Координаты фиксированы и условны, т.е. не зависят от разрешения
   * экрана. Все положения и размеры элементов (меню, игровое поле)
   * расчитываются в этих координатах
   */
  sf::FloatRect activeRect(0, 0, ACTIVE_RECT_WIDTH, ACTIVE_RECT_HEIGHT);
  m_activeRectSize = RectSize(activeRect);

  /**
   * @brief Матрица скоса
   * @note Применение этого преобразования не зависит от размеров объекта и
   * потому может быть рассчитано лишь раз
   */
  sf::Transform trSkew = Skew(SKEW_ANGLE_H, SKEW_ANGLE_V);

  /***** Расчёт главной трансформации *****/

  /// Прямоугольник активной области, после применения скоса
  m_activeRectAfterTransform = (trSkew * ToOrigin(m_activeRectSize)).transformRect(activeRect);

  /// Рассчитываем коэффициент маштабирования так, чтобы активная область после
  /// применения скоса, была вписана в экран
  float scaleFactor = std::min(m_screenRect.width / m_activeRectAfterTransform.width,
                               m_screenRect.height / m_activeRectAfterTransform.height);

  /**
   * @brief Матрица масштабирования
   */
  sf::Transform trScale = sf::Transform().scale(scaleFactor, scaleFactor);

  /***** Расчёт размеров области, покрывающей экран *****/
  sf::FloatRect coverRect = ToOrigin(RectSize(m_screenRect)).transformRect(m_screenRect);
  coverRect = (trScale * trSkew).getInverse().transformRect(coverRect);

  m_coverSize = RectSize(coverRect);

  m_cellSize = std::min(ACTIVE_RECT_WIDTH, ACTIVE_RECT_HEIGHT) / MAX_CELLS;
#pragma endregion

#pragma region "Инициализация окна"
  sf::ContextSettings settings;
  settings.antialiasingLevel = 8;
  m_window.create(screenMode, "Back", sf::Style::Fullscreen, settings);
#pragma endregion

  m_gridTr = AlignToCenter(coverRect, m_screenRect) * trScale * trSkew * ToOrigin(m_coverSize);
  m_activeTr = AlignToCenter(activeRect, m_screenRect) * FromOrigin(m_activeRectSize) * trScale *
               trSkew * ToOrigin(m_activeRectSize);
}

CoreGame::~CoreGame() {}

void
CoreGame::StartGame()
{
  Grid grid(m_coverSize, m_cellSize);

  sf::RectangleShape activeRectShape(m_activeRectSize);
  activeRectShape.setFillColor(sf::Color::Transparent);
  activeRectShape.setOutlineColor(sf::Color::Green);
  activeRectShape.setOutlineThickness(4.f);

  //   sf::RectangleShape SetCell(sf::Vector2f(m_cellSize, m_cellSize));
  //   sf::RectangleShape LockCell(sf::Vector2f(m_cellSize, m_cellSize));
  //   sf::RectangleShape FreeCell(sf::Vector2f(m_cellSize, m_cellSize));

  //   SetCell.setFillColor(sf::Color::Red);
  //   LockCell.setFillColor(sf::Color::Green);
  //   FreeCell.setFillColor(sf::Color(0, 187, 255, 255));

  //   int selectX = 3;
  //   int selectY = 2;
  //   SetCell.move(m_cellSize * selectX, m_cellSize * selectY);

#pragma region "[DEBUG] Центр экрана"
  sf::VertexArray centerPnt(sf::Lines, 4);

  centerPnt[0].position = sf::Vector2f(0, m_screenRect.height / 2);
  centerPnt[1].position = sf::Vector2f(m_screenRect.width, m_screenRect.height / 2);
  centerPnt[2].position = sf::Vector2f(m_screenRect.width / 2, 0);
  centerPnt[3].position = sf::Vector2f(m_screenRect.width / 2, m_screenRect.height);

  centerPnt[0].color = sf::Color::Magenta;
  centerPnt[1].color = sf::Color::Magenta;
  centerPnt[2].color = sf::Color::Magenta;
  centerPnt[3].color = sf::Color::Magenta;
#pragma endregion

  std::vector<Player> playerList;
  uint32_t SizeCell = m_cellSize - 2;
  sf::RectangleShape MouseCell(sf::Vector2f(SizeCell, SizeCell));
  uint8_t alpha = 0;
  uint16_t angle = 0;
  bool directionAlpha = false;
  MouseCell.setFillColor(sf::Color(255, 255, 255, alpha));
  sf::Clock clock; // starts the clock
  while (m_window.isOpen()) {
    m_window.clear(sf::Color(0, 187, 255, 255));

    m_window.draw(grid, m_gridTr);
    m_window.draw(activeRectShape, m_activeTr);
    // m_window.draw(SetCell, m_activeTr);
    switch (m_StateGame) {
      case COMMON::EGameState::MENU: {
        Field& field = Singleton<Field>::GetInstance();
        IGame& game = Singleton<IGame>::GetInstance();
        Player player;
        player.SetName("Alex");
        player.SetEntity(COMMON::EPlayerType::HUMAN);
        playerList.push_back(player);
        field.Create("ENGLISH");
        game.SetField(field);
        game.SetPlayerList(playerList);
        if (!game.CreateNewGame()) {
          std::cout << "Game Failed!" << std::endl;
          return;
        }
        m_StateGame = COMMON::EGameState::GAME;
        break;
      }
      case COMMON::EGameState::GAME: {
        IGame& game = Singleton<IGame>::GetInstance();
        Field field = game.GetField();

        COMMON::ECell typeCell;

        uint32_t Offset_W = m_cellSize * ((m_activeRectSize.x / m_cellSize / 2) -
                                          std::floor((float)field.GetWidth() / 2));
        uint32_t Offset_H = m_cellSize * ((m_activeRectSize.y / m_cellSize / 2) -
                                          std::round((float)field.GetHeight() / 2));
        sf::RectangleShape Cell(sf::Vector2f(SizeCell, SizeCell));
        for (size_t i = 0; i < field.GetWidth(); i++) {
          for (size_t j = 0; j < field.GetHeight(); j++) {
            typeCell = field.GetCell(i, j);
            switch (typeCell) {
              case COMMON::ECell::FREE: {
                Cell.setFillColor(sf::Color(0, 187, 255, 255));
                break;
              }
              case COMMON::ECell::SET: {
                Cell.setFillColor(sf::Color::Red);
                break;
              }
              case COMMON::ECell::LOCK: {
                Cell.setFillColor(sf::Color(99, 99, 99, 255));
                break;
              }
            }
            Cell.setPosition(m_cellSize * i + Offset_W, m_cellSize * j + Offset_H);
            m_window.draw(Cell, m_activeTr);
          }
        }
      }
    }

    // Получаем экранные координаты
    sf::Vector2i pixelPos = sf::Mouse::getPosition(m_window);
    // Трансформируем координаты в активную область
    sf::Vector2f activePos = m_activeTr.getInverse().transformPoint(pixelPos.x, pixelPos.y);
    // Получаем координаты ячейки на которую указывает курсор
    sf::Vector2i aciveCellPos = sf::Vector2i(activePos.x / m_cellSize, activePos.y / m_cellSize);
    // Выставляем ячейку под курсор
    MouseCell.setPosition(m_cellSize * aciveCellPos.x, m_cellSize * aciveCellPos.y);
    // Делаем подсветку плавной для эффекта выбора клетки
    sf::Time elapsed1 = clock.getElapsedTime();
    if (elapsed1.asMilliseconds() > 5) {
      angle = (angle + 1) % 360;
      clock.restart();
    }
    float radians = DEG2RAD(angle);
    alpha = 255 * (sin(radians) + 1) / 2.f;
    MouseCell.setFillColor(sf::Color(255, 255, 255, alpha));
    // Отрисовываем ячейку под курсором
    m_window.draw(MouseCell, m_activeTr);
    // Отрисовываем все выбранные ячейки
    for (auto& activeCell : m_ActiveCells) {
      activeCell.setFillColor(sf::Color(0, 255, 0, alpha));
      m_window.draw(activeCell, m_activeTr);
    }
    m_window.draw(centerPnt);
    m_window.display();

    sf::Event event;
    while (m_window.pollEvent(event)) {
      switch (event.type) {
        case sf::Event::Closed:
          m_window.close();
          break;
        case sf::Event::KeyPressed:
          if (event.key.code == sf::Keyboard::Escape)
            m_window.close();
          break;
        case sf::Event::MouseButtonPressed:
          if (event.key.code == sf::Mouse::Left) {
            // TODO продумать логику загрузки направления на бэк и связать это с гуи
            sf::RectangleShape ActiveCell(sf::Vector2f(SizeCell, SizeCell));
            // Запоминаем кординаты ячейки на которую нажали
            ActiveCell.setPosition(m_cellSize * aciveCellPos.x, m_cellSize * aciveCellPos.y);
            // Добавляем в массив выбранную ячейку
            // TODO сделать условие и сделать check хода, так же нужна возможность правильно
            // определять тип выбранной ячейки
            m_ActiveCells.push_back(ActiveCell);
          }
        default:
          break;
      }
    }
  }
}