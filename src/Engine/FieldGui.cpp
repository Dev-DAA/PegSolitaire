#include "FieldGui.h"

FieldGui::FieldGui()
{
  // Обновляем поле
  m_field = Singleton<IGame>::GetInstance().GetField();
  // Рассчитываем размер ячейки
  m_cellSize = std::min(ACTIVE_RECT_WIDTH, ACTIVE_RECT_HEIGHT) / MAX_CELLS;
  // Размер закрашиваемой ячейки
  m_sizeColorCell = m_cellSize - 2;
  // Рассчитываем смещение поля в центр экрана
  auto middle_w = ACTIVE_RECT_WIDTH / m_cellSize / 2;
  auto middle_h = ACTIVE_RECT_HEIGHT / m_cellSize / 2;
  m_OffsetScreen_W = m_cellSize * (middle_w - std::floor((float)m_field.GetWidth() / 2));
  m_OffsetScreen_H = m_cellSize * (middle_h - std::round((float)m_field.GetHeight() / 2));
  // Рассчитываем смещение в ячейках
  m_OffsetCell_W = m_OffsetScreen_W / m_cellSize;
  m_OffsetCell_H = m_OffsetScreen_H / m_cellSize;
  // Создаём ячейку на коотрую указывает курсор
  m_MouseCell.setSize(sf::Vector2f(m_sizeColorCell, m_sizeColorCell));
}

void
FieldGui::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
  COMMON::ECell typeCell;
  sf::RectangleShape Cell(sf::Vector2f(m_sizeColorCell, m_sizeColorCell));

  for (size_t i = 0; i < m_field.GetWidth(); i++) {
    for (size_t j = 0; j < m_field.GetHeight(); j++) {
      typeCell = m_field.GetCell(i, j);
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
      Cell.setPosition(m_cellSize * i + m_OffsetScreen_W, m_cellSize * j + m_OffsetScreen_H);
      target.draw(Cell, states);
    }
  }

  for (auto& activeCell : m_ActiveCells) {
    target.draw(activeCell, states);
  }

  // Отрисовываем ячейку под курсором
  target.draw(m_MouseCell, states);
}

void
FieldGui::update()
{
  // Обновляем поле
  m_field = Singleton<IGame>::GetInstance().GetField();
}

void
FieldGui::SetAlpha(uint8_t alpha)
{
  m_MouseCell.setFillColor(sf::Color(255, 255, 255, alpha));

  for (auto& activeCell : m_ActiveCells) {
    sf::Color color = activeCell.getFillColor();
    color.a = alpha;
    activeCell.setFillColor(color);
  }
}

void
FieldGui::SetPositionMouse(sf::Vector2f pos)
{
  // TODO если этого не сделать, то верхняя координата ячейки находится ровно под курсором
  sf::Vector2i aciveCellPos = sf::Vector2i(pos.x / m_cellSize, pos.y / m_cellSize);
  m_MouseCell.setPosition(m_cellSize * aciveCellPos.x, m_cellSize * aciveCellPos.y);
}

void
FieldGui::AddActiveCell(sf::Vector2f pos, bool firstCell)
{
  sf::RectangleShape ActiveCell(sf::Vector2f(m_sizeColorCell, m_sizeColorCell));
  ActiveCell.setPosition(pos);
  if (firstCell) {
    ActiveCell.setFillColor(sf::Color(255, 0, 200, 255));
  } else {
    ActiveCell.setFillColor(sf::Color(0, 255, 0, 255));
  }
  m_ActiveCells.push_back(ActiveCell);
}

bool
FieldGui::GetEmptyActiveCells()
{
  return m_ActiveCells.empty();
}

void
FieldGui::ClearActiveCells()
{
  m_ActiveCells.clear();
}

bool
FieldGui::CheckLastClick(sf::Vector2i posActiveCell)
{
  if (m_ActiveCells.empty()) {
    return false;
  }

  sf::Vector2f lastPos = m_ActiveCells.back().getPosition();
  auto last_cell_x = lastPos.x / m_cellSize;
  auto last_cell_y = lastPos.y / m_cellSize;

  return (last_cell_x == posActiveCell.x) && (last_cell_y == posActiveCell.y);
}

sf::Vector2f
FieldGui::GetFirstActiveCell()
{
  if (m_ActiveCells.empty()) {
    std::cout << "Error: don't have first active cell" << std::endl;
    return sf::Vector2f{ 0.f, 0.f };
  }

  sf::Vector2f FirstPosCell = m_ActiveCells.front().getPosition();

  return sf::Vector2f{ (FirstPosCell.x / m_cellSize) - m_OffsetCell_W,
                       (FirstPosCell.y / m_cellSize) - m_OffsetCell_H };
}