#include "Field.h"

Field::Field()
  : m_width(7)
  , m_height(7)
{}

Field::~Field() {}

bool
Field::Create(const std::string& nameField)
{
  // Получаем чертёж поля
  std::string ready_bp = GetBlueprint(nameField);

  if (ready_bp.empty()) {
    return false;
  }

  uint8_t CountCell = ready_bp.length();

  if ((CountCell > MAX_COUNT_CELL) || (CountCell < MIN_COUNT_CELL)) {
    LOG(ERROR) << "Field::Create : Сustom field is larger than maximum constants!";
    return false;
  }

  // Меняем длину вектора под размер поля
  m_field.resize(ready_bp.length());

  // ДОСТУПНЫЕ СИМВОЛЫ
  const int MIN_NUM_AVAILABLE = 48;
  const int MAX_NUM_AVAILABLE = 50;

  int i = 0;
  for (char& ch : ready_bp) {
    if ((int)ch >= MIN_NUM_AVAILABLE && (int)ch <= MAX_NUM_AVAILABLE) {
      m_field[i] = static_cast<COMMON::ECell>((int)(ch - '0'));
      ++i;
    }
  }

  return true;
}

// DEPRECATED
// bool
// Field::Create(uint8_t width, uint8_t height, const std::vector<COMMON::ECell>& pattern)
// {
//   if ((width > MAX_WIDTH) || (height > MAX_HEIGHT)) {
//     LOG(ERROR) << "Field::Create : Сustom field is larger than maximum constants!";
//     return false;
//   }

//   // Проверяем вектор на наличие данных
//   if (pattern.size() == 0) {
//     LOG(ERROR) << "Field::Create : There is no data!";
//     return false;
//   }

//   // Проверяем вектор на соответствие размера
//   if (pattern.size() != (width * height)) {
//     LOG(ERROR) << "Field::Create : Vector size does not fit the generated field!";
//     return false;
//   }

//   if (!std::all_of(pattern.begin(), pattern.end(), [](COMMON::ECell item) {
//         return (item == COMMON::ECell::FREE) || (item == COMMON::ECell::LOCK) ||
//                (item == COMMON::ECell::SET);
//       })) {
//     LOG(ERROR) << "Field::Create : Invalid data type in pattern!";
//     return false;
//   }

//   // Перепишем размеры поля
//   m_width = width;
//   m_height = height;
//   // Меняем длину вектора под размер поля
//   m_field.resize(width * height);
//   // Копируем данные из шаблона в поле
//   std::copy(pattern.begin(), pattern.end(), m_field.begin());
//   return true;
// }

bool
Field::SetCell(uint8_t x_field, uint8_t y_field, const COMMON::ECell value)
{
  // Проверка типа ячейки
  if ((value != COMMON::ECell::FREE) && (value != COMMON::ECell::LOCK) &&
      (value != COMMON::ECell::SET)) {
    LOG(ERROR) << "Field::SetCell : Invalid field type value!";
    return false;
  }

  // Проверяем диапазон
  if ((m_width <= x_field) || (m_height <= y_field)) {
    LOG(ERROR) << "Field::SetCell : The entered coordinates are out of range!";
    return false;
  }

  // Устанавливаем значение в выбранную ячейку
  m_field[y_field * m_width + x_field] = value;

  return true;
}

std::vector<COMMON::ECell>
Field::GetField() const
{
  // Возвращаем поле
  return m_field;
}

uint8_t
Field::GetWidth() const
{
  // Возвращаем ширину поля
  return m_width;
}

uint8_t
Field::GetHeight() const
{
  // Возвращаем высоту поля
  return m_height;
}

COMMON::ECell
Field::GetCell(uint8_t x, uint8_t y) const
{
  // Проверяем диапазон
  if ((m_width <= x) || (m_height <= y)) {
    LOG(ERROR) << "Field::GetCell : The entered coordinates are out of range!";
    return COMMON::ECell::LOCK;
  }

  return m_field[y * m_width + x];
}

std::string
Field::GetBlueprint(std::string nameFile)
{
  std::string blueprintB64;
  // окрываем файл для чтения
  std::ifstream istrm(nameFile.append(".b64"));
  if (istrm.is_open()) {
    getline(istrm, blueprintB64);
  }
  // закрываем файл
  istrm.close();

  std::cout << blueprintB64 << std::endl;

  return base64_decode(blueprintB64);
}