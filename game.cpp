#include "game.h"
#include <iostream>
#include <fstream>
#include <iomanip>

/**
 * @brief Конструктор игры
 *
 * Инициализирует игру и настраивает игроков
 */
Game::Game() {
    setupPlayers();
}

// ==================== МЕТОДЫ ОТОБРАЖЕНИЯ ====================

/**
 * @brief Отрисовка полного игрового стола со всеми картами
 *
 * Используется в конце раунда когда все карты дилера видны
 */
void Game::drawGameTable() const {
    system("cls");

    std::cout << "\n";
    std::cout << "    ============================\n";
    std::cout << "    |      BLACKJACK TABLE     |\n";
    std::cout << "    ============================\n\n";

    // Дилер (верх стола)
    std::cout << "           DEALER'S HAND\n";
    std::cout << "           ";
    dealer_.showHand();
    std::cout << "\n";

    // Разделитель
    std::cout << "    ----------------------------\n\n";

    // Игроки (низ стола)  
    for (const auto& player : players_) {
        std::cout << "           " << player.getName() << "'s HAND\n";
        std::cout << "           ";
        player.showHand();
        std::cout << "\n";
    }

    // Нижняя часть стола
    std::cout << "    ============================\n";
}

/**
 * @brief Отрисовка стола в начале раунда (скрытые карты дилера)
 *
 * Используется во время ходов игроков когда видна только первая карта дилера
 */
void Game::drawGameTableFirstDeal() const {
    system("cls");

    std::cout << "\n";
    std::cout << "    ============================\n";
    std::cout << "    |      BLACKJACK TABLE     |\n";
    std::cout << "    ============================\n\n";

    // Дилер (только первая карта видна)
    std::cout << "           DEALER'S HAND\n";
    std::cout << "           ";
    dealer_.showFirstCard();
    std::cout << "\n";

    // Разделитель
    std::cout << "    ----------------------------\n\n";

    // Игроки  
    for (const auto& player : players_) {
        std::cout << "           " << player.getName() << "'s HAND\n";
        std::cout << "           ";
        player.showHand();
        std::cout << "\n";
    }

    std::cout << "    ============================\n";
}

// ==================== НАСТРОЙКА ИГРОКОВ ====================

/**
 * @brief Настройка игроков перед началом игры
 *
 * Запрашивает количество игроков и их имена
 * Валидирует ввод и устанавливает значения по умолчанию
 */
void Game::setupPlayers() {
    int playerCount = 0;

    while (playerCount < 1 || playerCount > 4) {
        std::cout << "How many players? (1-4): ";
        std::cin >> playerCount;

        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            setErrorColor();
            std::cout << "Error! Enter a number from 1 to 4.\n";
            resetColor();
            playerCount = 0;
        }
        else if (playerCount < 1 || playerCount > 4) {
            setErrorColor();
            std::cout << "Error! Enter a number from 1 to 4.\n";
            resetColor();
        }

        std::cin.ignore(10000, '\n');
    }

    // Создание игроков
    for (int i = 1; i <= playerCount; ++i) {
        std::string playerName;
        std::cout << "Enter name for player " << i << ": ";
        std::getline(std::cin, playerName);

        if (playerName.empty()) {
            playerName = "Player " + std::to_string(i);
        }

        players_.emplace_back(playerName);
    }

    // Подтверждение состава стола
    std::cout << "\nAt the table: ";
    for (const auto& player : players_) {
        std::cout << player.getName() << " ";
    }
    std::cout << "\n";
}

// ==================== ОСНОВНОЙ ИГРОВОЙ ЦИКЛ ====================

/**
 * @brief Запуск основной игровой сессии
 *
 * Управляет всем игровым процессом от приветствия до завершения
 * Включает заставку, выбор стратегии дилера и основной игровой цикл
 */
void Game::startGame() {
    loadStatistics();

    // Красивая заставка
    system("cls");
    std::cout << R"(
    .------..------..------..------..------.
    |B.--. ||L.--. ||A.--. ||C.--. ||K.--. |
    | :(): || :/\: || (\/) || :/\: || :/\: |
    | ()() || (__) || :\/: || :\/: || :\/: |
    | '--'B|| '--'L|| '--'A|| '--'C|| '--'K|
    `------'`------'`------'`------'`------'
    .------..------..------..------.
    |J.--. ||A.--. ||C.--. ||K.--. |
    | :(): || (\/) || :/\: || :/\: |
    | ()() || :\/: || :\/: || :\/: |
    | '--'J|| '--'A|| '--'C|| '--'K|
    `------'`------'`------'`------'
    )" << std::endl;

    std::cout << "\nPress Enter to start...";
    std::cin.ignore();

    // Меню выбора стратегии дилера
    std::cout << "\n=== WELCOME TO BLACKJACK ===\n";
    std::cout << "\nSelect dealer strategy:\n";
    std::cout << "1 - Standard (stops at 17+)\n";
    std::cout << "2 - Aggressive (stops at 18+)\n";
    std::cout << "3 - Cautious (stops at 16+)\n";
    std::cout << "Your choice (1-3): ";

    int strategyChoice;
    std::cin >> strategyChoice;
    std::cin.ignore(10000, '\n');

    // Установка стратегии дилера
    switch (strategyChoice) {
    case 1:
        dealer_.setStrategy(DealerStrategy::Standard);
        break;
    case 2:
        dealer_.setStrategy(DealerStrategy::Aggressive);
        break;
    case 3:
        dealer_.setStrategy(DealerStrategy::Cautious);
        break;
    default:
        std::cout << "Invalid choice, using standard strategy\n";
        dealer_.setStrategy(DealerStrategy::Standard);
    }

    // Основной игровой цикл
    while (true) {
        playRound();

        // Статистика после раунда
        std::cout << "\n--- CURRENT STATISTICS ---\n";
        for (auto& player : players_) {
            player.showStats();
        }

        // Запрос на продолжение
        std::cout << "\nPlay again? (y/n): ";
        char choice;
        std::cin >> choice;
        std::cin.ignore(10000, '\n');

        if (choice != 'y' && choice != 'Y') {
            break;
        }

        // Сброс состояния для нового раунда
        deck_ = Deck();
        deck_.shuffle();

        for (auto& player : players_) {
            player.clearHand();
        }
        dealer_.clearHand();
    }

    saveStatistics();
    std::cout << "Thanks for playing!\n";
}

/**
 * @brief Выполнение одного игрового раунда
 *
 * Полный цикл раунда: раздача, ходы игроков, ход дилера, определение победителя
 */
void Game::playRound() {
    setTitleColor();
    std::cout << "\n--- NEW ROUND ---\n";
    resetColor();

    deck_.shuffle();
    dealInitialCards();
    playerTurns();
    dealerTurn();
    determineWinner();
}

/**
 * @brief Начальная раздача карт
 *
 * Раздает по 2 карты каждому игроку и дилеру
 * Показывает стол со скрытыми картами дилера
 */
void Game::dealInitialCards() {
    // Раздача карт игрокам
    for (auto& player : players_) {
        player.takeCard(deck_);
        player.takeCard(deck_);
    }

    // Раздача карт дилеру
    dealer_.takeCard(deck_);
    dealer_.takeCard(deck_);

    // Показываем стол после раздачи
    drawGameTableFirstDeal();
}

/**
 * @brief Очередь ходов игроков
 *
 * Каждый игрок по очереди делает свои ходы
 * Обрабатывает Split и создает новых игроков при разделении
 */
void Game::playerTurns() {
    std::vector<Player> newSplitPlayers;

    for (size_t i = 0; i < players_.size(); ++i) {
        auto& player = players_[i];

        while (true) {
            // Показываем актуальное состояние стола перед каждым ходом
            drawGameTableFirstDeal();

            std::cout << "\n" << player.getName() << ", your move:\n";
            PlayerAction action = player.getPlayerAction();

            // Условия выхода из цикла хода игрока
            if (action == PlayerAction::Stand ||
                action == PlayerAction::DoubleDown ||
                player.isBusted()) {
                break;
            }

            // Обработка Split
            if (action == PlayerAction::Split && player.canSplit()) {
                handleSplit(player, newSplitPlayers);
            }

            // Обработка Hit
            if (action == PlayerAction::Hit) {
                player.takeCard(deck_);
                // Обновляем отображение после взятия карты
                drawGameTableFirstDeal();
            }
        }
    }

    // Добавляем split-игроков в конец очереди
    for (auto& splitPlayer : newSplitPlayers) {
        players_.push_back(splitPlayer);
    }
}

/**
 * @brief Ход дилера (автоматический)
 *
 * Дилер играет по установленной стратегии до достижения порогового значения
 */
void Game::dealerTurn() {
    // Показываем полный стол с картами дилера
    drawGameTable();

    setTitleColor();
    std::cout << "\n--- Dealer's Move ---\n";
    resetColor();

    // Автоматическая игра дилера по стратегии
    while (dealer_.mustDrawCard() && !dealer_.isBusted()) {
        setColor(11); // Голубой для действий
        std::cout << "The dealer takes the card...\n";
        resetColor();

        dealer_.takeCard(deck_);
        drawGameTable(); // Обновляем отображение
    }

    // Результат хода дилера
    if (dealer_.isBusted()) {
        setErrorColor();
        std::cout << "Dealer is busted!\n";
        resetColor();
    }
    else {
        setSuccessColor();
        std::cout << "Dealer stands.\n";
        resetColor();
    }
}

/**
 * @brief Определение победителей раунда
 *
 * Сравнивает счета игроков и дилера, объявляет результаты
 * Обновляет статистику игроков
 */
void Game::determineWinner() {
    int dealerScore = dealer_.calculateScore();

    // Показываем финальный стол
    drawGameTable();

    for (auto& player : players_) {
        int playerScore = player.calculateScore();

        setTitleColor();
        std::cout << "\n=== RESULT for " << player.getName() << " ===\n";

        if (player.isBusted()) {
            setErrorColor();
            std::cout << player.getName() << " busted! Dealer wins.\n";
            player.recordLoss();
        }
        else if (dealer_.isBusted()) {
            setSuccessColor();
            std::cout << "Dealer busted! " << player.getName() << " wins!\n";
            player.recordWin();
        }
        else if (playerScore > dealerScore) {
            setSuccessColor();
            std::cout << player.getName() << " wins! " << playerScore << " vs " << dealerScore << "\n";
            player.recordWin();
        }
        else if (playerScore < dealerScore) {
            setErrorColor();
            std::cout << "Dealer wins! " << dealerScore << " vs " << playerScore << "\n";
            player.recordLoss();
        }
        else {
            setColor(14); // Желтый для ничьи
            std::cout << "Push! " << player.getName() << " and dealer tie with " << playerScore << "\n";
            player.recordPush();
        }
        resetColor();
    }
}

// ==================== СИСТЕМА СТАТИСТИКИ ====================

/**
 * @brief Загрузка статистики игроков из файла
 *
 * Читает статистику из файла blackjack_stats.txt
 * Восстанавливает прогресс игроков между сессиями
 */
void Game::loadStatistics() {
    std::ifstream file("blackjack_stats.txt");
    if (!file) {
        std::cout << "No statistics file found. Starting with clean statistics.\n";
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        parsePlayerStats(line);
    }
    std::cout << "Statistics loaded successfully!\n";
}

/**
 * @brief Сохранение статистики игроков в файл
 *
 * Сохраняет статистику в файл blackjack_stats.txt
 * Формат: Имя:Игр:Побед:Поражений:Ничьих:МаксОчков
 */
void Game::saveStatistics() {
    std::ofstream file("blackjack_stats.txt");
    if (!file) {
        std::cout << "Error: Failed to create statistics file!\n";
        return;
    }

    for (const auto& player : players_) {
        file << player.getName() << ":"
            << player.getGamesPlayed() << ":"
            << player.getGamesWon() << ":"
            << player.getGamesLost() << ":"
            << player.getGamesPushed() << ":"
            << player.getMaxScore() << "\n";
    }

    std::cout << "Statistics saved to file!\n";
}

/**
 * @brief Парсинг строки статистики из файла
 * @param line Строка для парсинга в формате "Имя:Игр:Побед:Поражений:Ничьих:МаксОчков"
 */
void Game::parsePlayerStats(const std::string& line) {
    std::istringstream iss(line);
    std::string name;
    int gamesPlayed, gamesWon, gamesLost, gamesPushed, maxScore;
    char delimiter;

    if (std::getline(iss, name, ':') >> gamesPlayed >> delimiter
        >> gamesWon >> delimiter >> gamesLost >> delimiter >> gamesPushed
        >> delimiter >> maxScore) {

        for (auto& player : players_) {
            if (player.getName() == name) {
                player.setGamesPlayed(gamesPlayed);
                player.setGamesWon(gamesWon);
                player.setGamesLost(gamesLost);
                player.setGamesPushed(gamesPushed);
                player.setGamesScore(maxScore);
                break;
            }
        }
    }
}

// ==================== ОБРАБОТКА SPLIT ====================

/**
 * @brief Обработка разделения карт (Split)
 * @param player Игрок выполняющий разделение
 * @param newSplitPlayers Вектор для новых игроков после разделения
 */
void Game::handleSplit(Player& player, std::vector<Player>& newSplitPlayers) {
    if (!player.canSplit()) return;

    std::cout << player.getName() << " split hand!\n";

    // Создание нового игрока для split-руки
    Player splitPlayer(player.getName() + " (Split)");
    auto secondHand = player.splitHand(deck_);
    splitPlayer.setHand(secondHand);

    // Добавление карт в обе руки
    player.takeCard(deck_);
    splitPlayer.takeCard(deck_);

    // Сохранение нового игрока
    newSplitPlayers.push_back(splitPlayer);

    // Показываем обновленный стол после split
    drawGameTableFirstDeal();
}