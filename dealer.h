#pragma once
#include "player.h"
#include <windows.h>

/**
 * @brief Стратегии поведения дилера
 */
enum class DealerStrategy {
    Standard,   ///< Останавливается на 17+ (стандартные правила)
    Aggressive, ///< Останавливается на 18+ (агрессивная)
    Cautious    ///< Останавливается на 16+ (осторожная)
};

/**
 * @brief Класс представляющий дилера (крупье)
 *
 * Наследует от Player и добавляет AI-логику для автоматической игры
 * по правилам казино и различным стратегиям
 */
class Dealer : public Player {
public:
    /**
     * @brief Конструктор дилера
     */
    Dealer();

    /**
     * @brief Установка стратегии поведения дилера
     * @param newStrategy Новая стратегия
     */
    void setStrategy(DealerStrategy newStrategy);

    /**
     * @brief Автоматическая игра дилера по правилам
     * @param deck Колода из которой берутся карты
     */
    void playTurn(Deck& deck);

    /**
     * @brief Показывает только первую карту дилера (правила Blackjack)
     */
    void showFirstCard() const;

    /**
     * @brief Определяет должен ли дилер брать еще карту
     * @return true если должен брать карту, false если остановиться
     */
    bool mustDrawCard() const;

    /**
     * @brief Показывает все карты дилера
     */
    void showHand() const;

    // ==================== ЦВЕТОВЫЕ МЕТОДЫ ====================

    /**
     * @brief Установка цвета текста в консоли
     * @param color Код цвета Windows
     */
    static void setColor(int color) {
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(hConsole, color);
    }

    static void resetColor() { setColor(7); }           ///< Сброс к стандартному цвету
    static void setDealerColor() { setColor(12); }      ///< Красный для дилера
    static void setTitleColor() { setColor(13); }       ///< Фиолетовый для заголовков
    static void setErrorColor() { setColor(12); }       ///< Красный для ошибок

    /**
     * @brief Создает ASCII-арт для скрытой карты
     * @return Вектор строк представляющих скрытую карту
     */
    std::vector<std::string> getHiddenCardArt() const;

private:
    DealerStrategy strategy_ = DealerStrategy::Standard;  ///< Текущая стратегия дилера
};
