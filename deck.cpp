#include "deck.h"
#include <algorithm>
#include <iostream>
#include <random>
#include <stdexcept>

/**
 * @brief Конструктор создает полную колоду из 52 карт
 *
 * Создает все комбинации мастей и достоинств:
 * - 4 масти: Hearts, Diamonds, Clubs, Spades
 * - 13 достоинств: от Two до Ace
 */
Deck::Deck() {
    // Создаем все 52 карты: 4 масти × 13 достоинств
    for (int suit = static_cast<int>(Suit::Hearts); suit <= static_cast<int>(Suit::Spades); ++suit) {
        for (int rank = static_cast<int>(Rank::Two); rank <= static_cast<int>(Rank::Ace); ++rank) {
            cards_.emplace_back(static_cast<Suit>(suit), static_cast<Rank>(rank));
        }
    }
}

/**
 * @brief Тщательно перемешивает колоду используя Mersenne Twister
 */
void Deck::shuffle() {
    std::random_device rd;  // Источник энтропии
    std::mt19937 generator(rd());  // Генератор Mersenne Twister

    std::shuffle(cards_.begin(), cards_.end(), generator);

    std::cout << "The deck is shuffled!\n";
}

/**
 * @brief Взятие верхней карты из колоды
 * @return Карта с вершины колоды
 * @throws std::runtime_error если колода пуста
 */
Card Deck::drawCard() {
    if (cards_.empty()) {
        throw std::runtime_error("Cannot draw card: deck is empty!");
    }

    Card topCard = cards_.back();
    cards_.pop_back();
    return topCard;
}

/**
 * @brief Проверяет пуста ли колода
 * @return true если колода пуста, иначе false
 */
bool Deck::isEmpty() const {
    return cards_.empty();
}

/**
 * @brief Выводит все карты в колоде (для отладки)
 *
 * Формат вывода: "2H 3H 4H ... AS"
 * Полезно для тестирования и отладки
 */
void Deck::printDeck() const {
    std::cout << "Deck contents (" << cards_.size() << " cards): ";

    for (size_t i = 0; i < cards_.size(); ++i) {
        std::cout << cards_[i];
        if (i < cards_.size() - 1) {
            std::cout << " ";
        }
    }
    std::cout << "\n";
}