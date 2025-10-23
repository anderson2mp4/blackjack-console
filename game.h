#pragma once
#include "player.h"
#include "dealer.h"
#include "deck.h"
#include <vector>
#include <fstream>
#include <sstream>
#include <windows.h>

/**
 * @brief Основной класс игры Blackjack
 *
 * Управляет игровым процессом, координацией между игроками и дилером,
 * отображением игрового стола и статистикой
 */
class Game {
public:
    /**
     * @brief Конструктор игры
     */
    Game();

    /**
     * @brief Запуск основной игровой сессии
     */
    void startGame();

    /**
     * @brief Выполнение одного игрового раунда
     */
    void playRound();

    /**
     * @brief Сохранение статистики игроков в файл
     */
    void saveStatistics();

    /**
     * @brief Загрузка статистики игроков из файла
     */
    void loadStatistics();

    // ==================== ЦВЕТОВЫЕ МЕТОДЫ ====================

    /**
     * @brief Установка цвета текста в консоли
     * @param color Код цвета Windows
     */
    static void setColor(int color) {
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(hConsole, color);
    }

    static void resetColor() { setColor(7); }      ///< Сброс к стандартному цвету
    static void setTitleColor() { setColor(13); }  ///< Фиолетовый для заголовков
    static void setSuccessColor() { setColor(10); } ///< Зеленый для успешных действий
    static void setErrorColor() { setColor(12); }   ///< Красный для ошибок

private:
    // ==================== МЕТОДЫ ОТОБРАЖЕНИЯ ====================

    /**
     * @brief Отрисовка полного игрового стола со всеми картами
     */
    void drawGameTable() const;

    /**
     * @brief Отрисовка стола в начале раунда (скрытые карты дилера)
     */
    void drawGameTableFirstDeal() const;

    // ==================== ИГРОВАЯ ЛОГИКА ====================

    /**
     * @brief Настройка игроков перед началом игры
     */
    void setupPlayers();

    /**
     * @brief Начальная раздача карт
     */
    void dealInitialCards();

    /**
     * @brief Очередь ходов игроков
     */
    void playerTurns();

    /**
     * @brief Ход дилера (автоматический)
     */
    void dealerTurn();

    /**
     * @brief Определение победителей раунда
     */
    void determineWinner();

    /**
     * @brief Обработка разделения карт (Split)
     * @param player Игрок выполняющий разделение
     * @param newSplitPlayers Вектор для новых игроков после разделения
     */
    void handleSplit(Player& player, std::vector<Player>& newSplitPlayers);

    // ==================== СИСТЕМА СТАТИСТИКИ ====================

    /**
     * @brief Парсинг строки статистики из файла
     * @param line Строка для парсинга
     */
    void parsePlayerStats(const std::string& line);

private:
    Deck deck_;                     ///< Игровая колода карт
    std::vector<Player> players_;   ///< Список игроков за столом
    Dealer dealer_;                 ///< Дилер (крупье)
};