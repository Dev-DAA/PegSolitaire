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

  // Загрузка шрифта
  if (!m_font.loadFromFile("font.ttf")) {
    std::cout << ".Error while loading font" << std::endl;
  }

  m_TextScore.setFont(m_font);
  m_TextGameOver.setFont(m_font);

  m_TextGameOver.setString("Game Over!");
  m_TextGameOver.setCharacterSize(SIZE_FONT * 2);
  m_TextGameOver.setColor(sf::Color::Black);
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
  // Параметры для плавного изменения альфы
  uint8_t alpha = 0;
  uint16_t angle = 0;
  // Смещение поля в экранных координатах
  uint32_t OffsetScreen_W;
  uint32_t OffsetScreen_H;
  uint32_t OffsetCell_W;
  uint32_t OffsetCell_H;
  // Время для анимации ячейки
  sf::Clock clock;
  while (m_window.isOpen()) {
    m_window.clear(sf::Color(0, 187, 255, 255));

#pragma region "Состояние игры"
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

        OffsetScreen_W = m_cellSize * ((m_activeRectSize.x / m_cellSize / 2) -
                                       std::floor((float)field.GetWidth() / 2));
        OffsetScreen_H = m_cellSize * ((m_activeRectSize.y / m_cellSize / 2) -
                                       std::round((float)field.GetHeight() / 2));

        OffsetCell_W = OffsetScreen_W / m_cellSize;
        OffsetCell_H = OffsetScreen_H / m_cellSize;
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
            Cell.setPosition(m_cellSize * i + OffsetScreen_W, m_cellSize * j + OffsetScreen_H);
            m_window.draw(Cell, m_activeTr);
          }
        }
      }
    }
#pragma endregion

#pragma region "Отрисовка всех элементов"
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
      // activeCell.setFillColor(sf::Color(0, 255, 0, alpha));
      sf::Color color = activeCell.getFillColor();
      color.a = alpha;
      activeCell.setFillColor(color);
      m_window.draw(activeCell, m_activeTr);
    }

    m_window.draw(grid, m_gridTr);
    m_window.draw(activeRectShape, m_activeTr);
    m_window.draw(centerPnt);

#pragma region "Тестовый код для шрифтов"

    std::vector<Player> listPlayer = Singleton<IGame>::GetInstance().GetPlayerList();
    if (!listPlayer.empty()) {
      std::ostringstream playerScoreString;
      playerScoreString << listPlayer[0].GetScore();
      m_TextScore.setString("Score:" + playerScoreString.str());
      m_TextScore.setCharacterSize(SIZE_FONT);
      m_TextScore.setColor(sf::Color::Black);
      m_TextScore.setPosition(activeRectShape.getPosition());
      m_window.draw(m_TextScore, m_activeTr);
    }

    if (m_IsGameOver) {
      auto act_size_w = ACTIVE_RECT_WIDTH / 2;
      auto act_size_h = ACTIVE_RECT_HEIGHT / 2;
      auto count_symb = m_TextGameOver.getString().getSize();
      auto size_pix_symb = m_TextGameOver.getCharacterSize();
      auto offset_center_x = (count_symb / 2) * size_pix_symb;
      auto offset_center_y = size_pix_symb / 2;
      m_TextGameOver.setPosition(act_size_w - offset_center_x, act_size_h - offset_center_y);
      m_window.draw(m_TextGameOver, m_activeTr);
    }

#pragma endregion

    m_window.display();

#pragma endregion

#pragma region "Обработка событий игры"
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
            // Переводим координаты выбранной ячейки в координаты поля и получаем тип выбранной
            // ячейки
            COMMON::ECell TypeCell = Singleton<IGame>::GetInstance().GetField().GetCell(
              aciveCellPos.x - OffsetCell_W, aciveCellPos.y - OffsetCell_H);
            // Если ещё не было выбрано ячейки, то выбираем ту с которой будем ходитьы
            if (m_ActiveCells.empty()) {
              if (TypeCell == COMMON::ECell::SET) {
                // Запоминаем координаты последей выбранной ячейки
                m_PosLastCell.x = aciveCellPos.x;
                m_PosLastCell.y = aciveCellPos.y;
                // Создаём ячейку и помещаем в массив
                sf::RectangleShape ActiveCell(sf::Vector2f(SizeCell, SizeCell));
                ActiveCell.setPosition(m_cellSize * aciveCellPos.x, m_cellSize * aciveCellPos.y);
                ActiveCell.setFillColor(sf::Color(255, 0, 200, 255));
                m_ActiveCells.push_back(ActiveCell);
              }
            } else {
              sf::RectangleShape LastCell = m_ActiveCells.back();
              auto last_cell_x = LastCell.getPosition().x / m_cellSize;
              auto last_cell_y = LastCell.getPosition().y / m_cellSize;
              auto clickLastCell =
                (aciveCellPos.x == last_cell_x) && (aciveCellPos.y == last_cell_y);
              if (clickLastCell) {
                sf::Vector2f FirstPosCell = m_ActiveCells.front().getPosition();
                sf::Vector2f PosCell2Field = { (FirstPosCell.x / m_cellSize) - OffsetCell_W,
                                               (FirstPosCell.y / m_cellSize) - OffsetCell_H };

                Singleton<IGame>::GetInstance().DoMove(
                  PosCell2Field.x, PosCell2Field.y, m_DirectCells);
                m_ActiveCells.clear();
                m_DirectCells.clear();
                m_IsGameOver = Singleton<IGame>::GetInstance().IsGameOver();
                break;
              }

              if ((TypeCell == COMMON::ECell::FREE) && (!CheckRepeatedCell(aciveCellPos))) {

                COMMON::EDirect Direct = GetDirectNextCell(m_PosLastCell.x - OffsetCell_W,
                                                           m_PosLastCell.y - OffsetCell_H,
                                                           aciveCellPos.x - OffsetCell_W,
                                                           aciveCellPos.y - OffsetCell_H);
                if (Direct != COMMON::EDirect::UNKNOWN) {
                  // Запоминаем координаты последей выбранной ячейки
                  m_PosLastCell.x = aciveCellPos.x;
                  m_PosLastCell.y = aciveCellPos.y;
                  // Создаём ячейку и помещаем в массив
                  sf::RectangleShape ActiveCell(sf::Vector2f(SizeCell, SizeCell));
                  ActiveCell.setPosition(m_cellSize * aciveCellPos.x, m_cellSize * aciveCellPos.y);
                  ActiveCell.setFillColor(sf::Color(0, 255, 0, 255));
                  m_ActiveCells.push_back(ActiveCell);
                  // Запоминаем направление которое выбрали
                  m_DirectCells.push_back(Direct);
                }
              }
            }
          }
        default:
          break;
      }
    }
#pragma endregion
  } // m_window.isOpen()
}

bool
CoreGame::CheckRepeatedCell(sf::Vector2i ActiveCell)
{
  for (auto cell : m_ActiveCells) {
    auto cell_x = cell.getPosition().x / m_cellSize;
    auto cell_y = cell.getPosition().y / m_cellSize;

    if ((ActiveCell.x == cell_x) && (ActiveCell.y == cell_y)) {
      return true;
    }
  }

  return false;
}