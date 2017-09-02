// Подключаем заголовочный файл
#include <gtest/gtest.h>

// Описываем тест
// Sample1 — группа, к которой относится тест
// TestThatPasses — название теста
//
// Имя группы название и теста должны быть валидными идентификаторами,
// при этом они не могут содержать символ подчёркивания.
// Группа указывается для логического объединения «близких» тестов.
TEST(Sample1, TestThatPasses)
{
    // Выполняем проверки при помощи макросов ASSERT_*
    // Все проверки ниже будут успешно пройдены

    ASSERT_TRUE(true);
    ASSERT_FALSE(false);

    ASSERT_EQ(1, 1);
    ASSERT_EQ("123", "123");

    ASSERT_NE(1, 2);

    ASSERT_FLOAT_EQ(1.0, 1.0);

    ASSERT_LE(1, 2);
    ASSERT_LE(2, 2);

    ASSERT_GE(2, 1);
    ASSERT_GE(2, 2);
}

// Описываем ещё один тест
TEST(Sample2, TestThatFails)
{
    // Выполняем проверки при помощи макросов ASSERT_*

    ASSERT_FALSE(false);
    // Эта проверка не будет пройдена
    ASSERT_FALSE(true);
}

int main(int argc, char* argv[])
{
    // Запускаем все тесты на исполнение
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}