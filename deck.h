#pragma once
#include "card.h"
#include <vector>
#include <random>

/**
 * @brief Класс представляющий колоду игральных карт
 */
class Deck {
public:
    /**
     * @brief Конструктор создает полную колоду из 52 карт
     */
    Deck();

    /**
     * @brief Тщательно перемешивает колоду
     */
    void shuffle();

    /**
     * @brief Взятие верхней карты из колоды
     * @return Карта с вершины колоды
     * @throws std::runtime_error если колода пуста
     */
    Card drawCard();

    /**
     * @brief Проверяет пуста ли колода
     * @return true если колода пуста, иначе false
     */
    bool isEmpty() const;

    /**
     * @brief Выводит все карты в колоде (для отладки)
     */
    void printDeck() const;

private:
    std::vector<Card> cards_;  ///< Вектор карт в колоде
};