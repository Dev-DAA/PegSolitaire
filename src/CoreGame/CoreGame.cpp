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
  FieldGui fieldGui;

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

  uint32_t SizeCell = m_cellSize - 2;
  // Параметры для плавного изменения альфы
  uint8_t alpha = 0;
  uint16_t angle = 0;

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
        std::vector<Player> playerList;
        player.SetName("Alex");
        player.SetEntity(COMMON::EPlayerType::HUMAN);
        playerList.push_back(player);

        field.Create("ENGLISH");

        game.SetPlayerList(playerList);
        game.SetField(field);

        // Вызываем апдейт у отрисовывающегося поля и рассчитываем смещения
        fieldGui.update();
        CalculateOffset();
        //
        if (!game.CreateNewGame()) {
          std::cout << "Game Failed!" << std::endl;
          return;
        }
        m_StateGame = COMMON::EGameState::GAME;
        break;
      }
      case COMMON::EGameState::GAME: {
        // TODO
      }
    }
#pragma endregion

#pragma region "Отрисовка всех элементов"
    // Получаем экранные координаты
    sf::Vector2i pixelPos = sf::Mouse::getPosition(m_window);
    // Трансформируем координаты в активную область
    sf::Vector2f activePos = m_activeTr.getInverse().transformPoint(pixelPos.x, pixelPos.y);
    fieldGui.SetPositionMouse(activePos);
    // Получаем координаты ячейки на которую указывает курсор
    sf::Vector2i aciveCellPos = sf::Vector2i(activePos.x / m_cellSize, activePos.y / m_cellSize);

    // Делаем подсветку плавной для эффекта выбора клетки
    sf::Time elapsed1 = clock.getElapsedTime();
    if (elapsed1.asMilliseconds() > 5) {
      angle = (angle + 1) % 360;
      clock.restart();
    }
    float radians = DEG2RAD(angle);
    alpha = 255 * (sin(radians) + 1) / 2.f;
    fieldGui.SetAlpha(alpha);

    // Отрисовываем фон
    m_window.draw(grid, m_gridTr);
    // Отрисовываем игровое поле и всю активную область
    m_window.draw(fieldGui, m_activeTr);
    // TODO Отрисовываем активную область для разработки
    m_window.draw(activeRectShape, m_activeTr);
    // TODO Отрисовываем центр экрана для разработки
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
              aciveCellPos.x - m_OffsetCell_W, aciveCellPos.y - m_OffsetCell_H);
            // Если ещё не было выбрано ячейки, то выбираем ту с которой будем ходить
            if (fieldGui.GetEmptyActiveCells()) {
              if (TypeCell == COMMON::ECell::SET) {
                // Запоминаем координаты последей выбранной ячейки
                m_PosLastCell.x = aciveCellPos.x;
                m_PosLastCell.y = aciveCellPos.y;
                // Создаём ячейку и помещаем в массив
                fieldGui.AddActiveCell(
                  sf::Vector2f(m_cellSize * aciveCellPos.x, m_cellSize * aciveCellPos.y), true);
              }
            } else {
              if (fieldGui.CheckLastClick(aciveCellPos)) {
                sf::Vector2f PosCell2Field = fieldGui.GetFirstActiveCell();
                Singleton<IGame>::GetInstance().DoMove(
                  PosCell2Field.x, PosCell2Field.y, m_DirectCells);
                fieldGui.update();
                fieldGui.ClearActiveCells();
                m_DirectCells.clear();
                m_IsGameOver = Singleton<IGame>::GetInstance().IsGameOver();
                break;
              }

              if (TypeCell == COMMON::ECell::FREE) {
                // Получаем направление куда планируем сделать ход
                COMMON::EDirect Direct = GetDirectNextCell(m_PosLastCell.x - m_OffsetCell_W,
                                                           m_PosLastCell.y - m_OffsetCell_H,
                                                           aciveCellPos.x - m_OffsetCell_W,
                                                           aciveCellPos.y - m_OffsetCell_H);
                if (Direct != COMMON::EDirect::UNKNOWN) {
                  // Запоминаем координаты последей выбранной ячейки
                  m_PosLastCell.x = aciveCellPos.x;
                  m_PosLastCell.y = aciveCellPos.y;
                  // Создаём ячейку и помещаем в массив
                  fieldGui.AddActiveCell(
                    sf::Vector2f(m_cellSize * aciveCellPos.x, m_cellSize * aciveCellPos.y));
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

void
CoreGame::CalculateOffset()
{
  // Получаем поле
  Field field = Singleton<IGame>::GetInstance().GetField();
  // Рассчитываем смещение в пикселях
  auto OffsetScreen_W =
    m_cellSize * ((m_activeRectSize.x / m_cellSize / 2) - std::floor((float)field.GetWidth() / 2));
  auto OffsetScreen_H =
    m_cellSize * ((m_activeRectSize.y / m_cellSize / 2) - std::round((float)field.GetHeight() / 2));
  // Рассчитываем смещеение в ячейках
  m_OffsetCell_W = OffsetScreen_W / m_cellSize;
  m_OffsetCell_H = OffsetScreen_H / m_cellSize;
}
