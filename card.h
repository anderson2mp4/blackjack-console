#pragma once
#include <iostream>
#include <vector>
#include <string>

/**
 * @brief Масти игральных карт
 */
enum class Suit {
    Hearts,    // Червы
    Diamonds,  // Бубны
    Clubs,     // Трефы
    Spades     // Пики
};

/**
 * @brief Достоинства игральных карт
 */
enum class Rank {
    Two = 2,   // Двойка
    Three,     // Тройка
    Four,      // Четверка
    Five,      // Пятерка
    Six,       // Шестерка
    Seven,     // Семерка
    Eight,     // Восьмерка
    Nine,      // Девятка
    Ten,       // Десятка
    Jack,      // Валет
    Queen,     // Дама
    King,      // Король
    Ace        // Туз
};

/**
 * @brief Класс представляющий игральную карту
 */
class Card {
public:
    // Конструктор
    Card(Suit s, Rank r);

    // Геттеры
    Suit getSuit() const;
    Rank getRank() const;

    // Игровые методы
    int getValue() const;                    // Получить значение карты в Blackjack
    bool isAce() const;                      // Проверить является ли карта тузом

    // Методы отображения
    std::string toString() const;            // Текстовое представление (например "AH")
    std::vector<std::string> getAsASCII() const; // ASCII-графическое представление карты
    friend std::ostream& operator<<(std::ostream& os, const Card& card); // Оператор вывода

private:
    // Приватные вспомогательные методы
    std::string getRankSymbol() const;       // Символьное представление достоинства
    std::string getSuitSymbol() const;       // Символьное представление масти

    Suit suit_;  // Масть карты
    Rank rank_;  // Достоинство карты
};