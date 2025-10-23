#pragma once
#include "card.h"
#include "deck.h"
#include <vector>
#include <string>
#include <windows.h>

/**
 * @brief Действия доступные игроку в Blackjack
 */
enum class PlayerAction {
    Hit,        ///< Взять карту
    Stand,      ///< Остановиться
    DoubleDown, ///< Удвоить ставку
    Split       ///< Разделить карты
};

/**
 * @brief Класс представляющий игрока в Blackjack
 *
 * Управляет состоянием игрока, его картами, статистикой
 * и доступными действиями в зависимости от ситуации
 */
class Player {
public:
    /**
     * @brief Конструктор игрока
     * @param playerName Имя игрока
     */
    Player(const std::string& playerName);

    // ==================== ОСНОВНЫЕ ИГРОВЫЕ МЕТОДЫ ====================

    /**
     * @brief Взять карту из колоды
     * @param deck Колода из которой берется карта
     */
    void takeCard(Deck& deck);

    /**
     * @brief Рассчитать текущий счет руки
     * @return Счет руки с учетом тузов
     */
    int calculateScore() const;

    /**
     * @brief Показать карты игрока в ASCII-формате
     */
    void showHand() const;

    /**
     * @brief Получить имя игрока
     * @return Имя игрока
     */
    std::string getName() const;

    /**
     * @brief Получить действие игрока (интерактивный ввод)
     * @return Выбранное действие
     */
    PlayerAction getPlayerAction() const;

    /**
     * @brief Проверить перебор (счет > 21)
     * @return true если перебор, иначе false
     */
    bool isBusted() const;

    /**
     * @brief Проверить возможность разделения карт
     * @return true если можно разделить, иначе false
     */
    bool canSplit() const;

    /**
     * @brief Проверить возможность удвоения ставки
     * @return true если можно удвоить, иначе false
     */
    bool canDoubleDown() const;

    /**
     * @brief Получить текстовое представление руки
     * @return Строка с картами руки
     */
    std::string getHandAsString() const;

    /**
     * @brief Получить текстовое представление статистики
     * @return Строка со статистикой игрока
     */
    std::string getStatsAsString() const;

    // ==================== МЕТОДЫ ДЛЯ СЕТЕВОЙ ИГРЫ ====================

    /**
     * @brief Получить список доступных действий
     * @return Вектор доступных действий
     */
    std::vector<PlayerAction> getAvailableActions() const;

    /**
     * @brief Получить текстовое представление доступных действий
     * @return Строка с описанием доступных действий
     */
    std::string getActionsAsString() const;

    /**
     * @brief Конвертировать сетевой выбор в действие
     * @param networkChoice Числовой выбор из сети
     * @return Соответствующее действие
     */
    PlayerAction convertNetworkChoice(int networkChoice) const;

    // ==================== ВСПОМОГАТЕЛЬНЫЕ МЕТОДЫ ====================

    /**
     * @brief Показать доступные действия (интерактивный вывод)
     */
    void showAvailableActions() const;

    /**
     * @brief Получить выбор игрока (интерактивный ввод)
     * @return Числовой выбор игрока
     */
    int getPlayerChoice() const;

    /**
     * @brief Конвертировать выбор в действие
     * @param choice Числовой выбор
     * @return Соответствующее действие
     */
    PlayerAction convertChoiceToAction(int choice) const;

    // ==================== МЕТОДЫ ДЛЯ РАБОТЫ С РУКОЙ ====================

    /**
     * @brief Получить константную ссылку на руку
     * @return Константная ссылка на вектор карт
     */
    const std::vector<Card>& getHand() const;

    /**
     * @brief Очистить руку (для нового раунда)
     */
    void clearHand();

    /**
     * @brief Установить новую руку (для Split)
     * @param newHand Новая рука
     */
    void setHand(const std::vector<Card>& newHand);

    /**
     * @brief Разделить руку на две
     * @param deck Колода для взятия дополнительных карт
     * @return Вторая рука после разделения
     */
    std::vector<Card> splitHand(Deck& deck);

    // ==================== СТАТИСТИКА И РЕЗУЛЬТАТЫ ====================

    /// @name Геттеры статистики
    /// @{
    int getGamesPlayed() const { return gamesPlayed_; }
    int getGamesWon() const { return gamesWon_; }
    int getGamesLost() const { return gamesLost_; }
    int getGamesPushed() const { return gamesPushed_; }
    int getMaxScore() const { return maxScore_; }
    double getWinRate() const { return winRate_; }
    /// @}

    /// @name Методы обновления статистики
    /// @{
    void recordWin();                          ///< Записать победу
    void recordLoss();                         ///< Записать поражение
    void recordPush();                         ///< Записать ничью
    void updateMaxScore(int score);            ///< Обновить максимум очков
    void showStats() const;                    ///< Показать статистику
    /// @}

    /// @name Сеттеры статистики (для загрузки)
    /// @{
    void setGamesPlayed(int value) { gamesPlayed_ = value; updateWinRate(); }
    void setGamesWon(int value) { gamesWon_ = value; updateWinRate(); }
    void setGamesLost(int value) { gamesLost_ = value; updateWinRate(); }
    void setGamesPushed(int value) { gamesPushed_ = value; updateWinRate(); }
    void setGamesScore(int value) { maxScore_ = value; }
    /// @}

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
    static void setPlayerColor() { setColor(11); } ///< Голубой для игрока
    static void setCardColor() { setColor(14); }   ///< Желтый для карт
    static void setScoreColor() { setColor(10); }  ///< Зеленый для счета
    static void setActionColor() { setColor(15); } ///< Ярко-белый для действий

private:
    std::string name_;                           ///< Имя игрока
    std::vector<Card> hand_;                     ///< Карты в руке

    // Статистика игрока
    int gamesPlayed_ = 0;                        ///< Сыграно игр
    int gamesWon_ = 0;                           ///< Выиграно игр
    int gamesLost_ = 0;                          ///< Проиграно игр
    int gamesPushed_ = 0;                        ///< Ничьих
    int maxScore_ = 0;                           ///< Максимальный счет
    double winRate_ = 0.0;                       ///< Процент побед

    /**
     * @brief Обновить процент побед
     */
    void updateWinRate() {
        winRate_ = (gamesPlayed_ > 0) ?
            (static_cast<double>(gamesWon_) / gamesPlayed_ * 100) : 0.0;
    }
};