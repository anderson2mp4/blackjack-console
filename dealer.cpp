#include "dealer.h"
#include <iostream>

/**
 * @brief Конструктор дилера
 *
 * Инициализирует дилера с именем "Dealer" и стандартной стратегией
 */
Dealer::Dealer() : Player("Dealer") {}

/**
 * @brief Показывает только первую карту дилера (правила Blackjack)
 *
 * Первая карта отображается нормально, остальные - как скрытые
 * Это стандартное поведение в Blackjack
 */
void Dealer::showFirstCard() const {
    if (getHand().empty()) return;

    setDealerColor();
    std::cout << "Dealer's cards:\n";
    resetColor();

    std::vector<std::vector<std::string>> cardsArt;

    // Первая карта отображается нормально
    cardsArt.push_back(getHand()[0].getAsASCII());

    // Остальные карты отображаются как скрытые
    for (size_t i = 1; i < getHand().size(); ++i) {
        cardsArt.push_back(getHiddenCardArt());
    }

    // Вывод всех карт построчно
    for (size_t line = 0; line < 5; ++line) {
        for (size_t i = 0; i < cardsArt.size(); ++i) {
            setCardColor();
            std::cout << cardsArt[i][line];
            if (i < cardsArt.size() - 1) {
                std::cout << "  "; // Отступ между картами
            }
            resetColor();
        }
        std::cout << std::endl;
    }
}

/**
 * @brief Создает ASCII-арт для скрытой карты
 * @return Вектор строк представляющих скрытую карту
 */
std::vector<std::string> Dealer::getHiddenCardArt() const {
    return {
        "+-----+",
        "|#####|",
        "|#####|",
        "|#####|",
        "+-----+"
    };
}

/**
 * @brief Показывает все карты дилера
 *
 * Используется в конце раунда когда все карты дилера должны быть видны
 */
void Dealer::showHand() const {
    setDealerColor();
    std::cout << "Dealer's cards:\n";
    resetColor();

    auto hand = getHand();
    std::vector<std::vector<std::string>> cardsArt;

    // Получаем ASCII-представление всех карт
    for (const auto& card : hand) {
        cardsArt.push_back(card.getAsASCII());
    }

    // Вывод карт построчно
    for (size_t line = 0; line < 5; ++line) {
        for (size_t i = 0; i < cardsArt.size(); ++i) {
            setCardColor();
            std::cout << cardsArt[i][line];
            if (i < cardsArt.size() - 1) {
                std::cout << "  "; // Отступ между картами
            }
            resetColor();
        }
        std::cout << std::endl;
    }

    // Дополнительная текстовая информация
    setDealerColor();
    std::cout << "Cards: ";
    for (const auto& card : hand) {
        std::cout << card << " ";
    }
    std::cout << "(score: " << calculateScore() << ")";
    resetColor();
    std::cout << std::endl;
}

/**
 * @brief Установка стратегии поведения дилера
 * @param newStrategy Новая стратегия
 */
void Dealer::setStrategy(DealerStrategy newStrategy) {
    strategy_ = newStrategy;

    // Информационное сообщение о смене стратегии
    setColor(14); // Желтый для информации
    switch (strategy_) {
    case DealerStrategy::Standard:
        std::cout << "The dealer switched to the STANDARD strategy (17+ stop)\n";
        break;
    case DealerStrategy::Aggressive:
        std::cout << "The dealer switched to an AGGRESSIVE strategy (18+ stop)\n";
        break;
    case DealerStrategy::Cautious:
        std::cout << "The dealer switched to a CAUTIOUS strategy (16+ stop)\n";
        break;
    }
    resetColor();
}

/**
 * @brief Определяет должен ли дилер брать еще карту по текущей стратегии
 * @return true если должен брать карту, false если остановиться
 */
bool Dealer::mustDrawCard() const {
    if (isBusted()) {
        return false;
    }

    int score = calculateScore();

    // Логика принятия решений по стратегиям
    switch (strategy_) {
    case DealerStrategy::Standard:
        return score <= 16;  // Берет до 16, останавливается на 17+
    case DealerStrategy::Aggressive:
        return score <= 17;  // Берет до 17, останавливается на 18+
    case DealerStrategy::Cautious:
        return score <= 15;  // Берет до 15, останавливается на 16+
    default:
        return score <= 16;  // Fallback на стандартную стратегию
    }
}

/**
 * @brief Автоматическая игра дилера по правилам
 * @param deck Колода из которой берутся карты
 */
void Dealer::playTurn(Deck& deck) {
    setTitleColor();
    std::cout << "\n--- Dealer's Move ---\n";
    resetColor();

    showHand(); // Показываем все карты дилера

    // Автоматический ход дилера по правилам
    while (mustDrawCard() && !isBusted()) {
        setColor(11); // Голубой для действий
        std::cout << "The dealer takes the card...\n";
        resetColor();

        takeCard(deck);
        showHand();

        if (isBusted()) {
            setErrorColor();
            std::cout << "Dealer is busted!\n";
            resetColor();
        }
    }

    // Сообщение об окончании хода дилера
    if (!isBusted()) {
        setColor(10); // Зеленый для завершения
        std::cout << "The dealer has stopped.\n";
        resetColor();
    }
}