#include "player.h"
#include <iostream>
#include <limits>
#include <iomanip>

// Константы для числовых представлений действий
constexpr int ACTION_HIT = 1;
constexpr int ACTION_STAND = 2;
constexpr int ACTION_DOUBLE_DOWN = 3;
constexpr int ACTION_SPLIT = 4;

/**
 * @brief Конструктор игрока
 * @param playerName Имя игрока
 */
Player::Player(const std::string& playerName)
    : name_(playerName) {
}

// ==================== ОСНОВНЫЕ ИГРОВЫЕ МЕТОДЫ ====================

/**
 * @brief Взять карту из колоды
 * @param deck Колода из которой берется карта
 */
void Player::takeCard(Deck& deck) {
    Card newCard = deck.drawCard();
    hand_.push_back(newCard);
}

/**
 * @brief Рассчитать текущий счет руки с учетом тузов
 * @return Счет руки (тузы считаются как 1 или 11)
 */
int Player::calculateScore() const {
    int score = 0;
    int aceCount = 0;

    // Первый проход: считаем все тузы как 1
    for (const auto& card : hand_) {
        if (card.isAce()) {
            aceCount++;
            score += 1;  // Изначально туз = 1
        }
        else {
            score += card.getValue();
        }
    }

    // Второй проход: пытаемся считать тузы как 11 если это не вызывает перебор
    for (int i = 0; i < aceCount; ++i) {
        if (score + 10 <= 21) {
            score += 10;  // Тузы становятся 11 (добавляем 10 к изначальной 1)
        }
    }

    return score;
}

/**
 * @brief Показать карты игрока в ASCII-формате
 */
void Player::showHand() const {
    setPlayerColor();
    std::cout << name_ << "'s hand:\n";
    resetColor();

    if (hand_.empty()) {
        std::cout << "  Empty hand\n";
        return;
    }

    // Получаем ASCII-арт всех карт в руке
    std::vector<std::vector<std::string>> cardsArt;
    for (const auto& card : hand_) {
        cardsArt.push_back(card.getAsASCII());
    }

    // Выводим карты построчно
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

    setScoreColor();
    std::cout << "Score: " << calculateScore() << std::endl;
    resetColor();
}

std::string Player::getName() const {
    return name_;
}

/**
 * @brief Показать доступные действия с цветовым оформлением
 */
void Player::showAvailableActions() const {
    setActionColor();
    std::cout << "Available actions:\n";

    int option = 1;
    setColor(11); // Голубой для Hit
    std::cout << option++ << " - Hit\n";
    setColor(15); // Белый для Stand
    std::cout << option++ << " - Stand\n";

    if (canDoubleDown()) {
        setColor(10); // Зеленый для Double Down
        std::cout << option++ << " - Double Down\n";
    }

    if (canSplit()) {
        setColor(13); // Фиолетовый для Split
        std::cout << option++ << " - Split\n";
    }

    resetColor();
}

/**
 * @brief Получить числовой выбор игрока
 * @return Выбор игрока (1-4)
 */
int Player::getPlayerChoice() const {
    int choice;
    std::cout << "Your choice (" << ACTION_HIT << "-" << ACTION_SPLIT << "): ";
    std::cin >> choice;
    std::cin.ignore(10000, '\n');
    return choice;
}

/**
 * @brief Конвертировать числовой выбор в действие
 * @param choice Числовой выбор игрока
 * @return Соответствующее действие PlayerAction
 */
PlayerAction Player::convertChoiceToAction(int choice) const {
    switch (choice) {
    case ACTION_HIT:
        return PlayerAction::Hit;
    case ACTION_STAND:
        return PlayerAction::Stand;
    case ACTION_DOUBLE_DOWN:
        return canDoubleDown() ? PlayerAction::DoubleDown : PlayerAction::Hit;
    case ACTION_SPLIT:
        return canSplit() ? PlayerAction::Split : PlayerAction::Hit;
    default:
        std::cout << "Invalid choice, defaulting to Hit.\n";
        return PlayerAction::Hit;
    }
}

/**
 * @brief Получить действие игрока (интерактивный ввод)
 * @return Выбранное действие
 */
PlayerAction Player::getPlayerAction() const {
    if (isBusted()) {
        std::cout << name_ << " has bust! Automatic Stand.\n";
        return PlayerAction::Stand;
    }

    std::cout << "\n" << name_ << ", your move:\n";
    std::cout << "Cards: ";
    for (const auto& card : hand_) {
        std::cout << card << " ";
    }
    std::cout << "(score: " << calculateScore() << ")\n";

    showAvailableActions();
    int choice = getPlayerChoice();
    return convertChoiceToAction(choice);
}

bool Player::isBusted() const {
    return calculateScore() > 21;
}

bool Player::canSplit() const {
    // Может разделить если ровно 2 карты одинакового достоинства
    return (hand_.size() == 2) &&
        (hand_[0].getValue() == hand_[1].getValue());
}

bool Player::canDoubleDown() const {
    // Может удвоить если ровно 2 карты
    return hand_.size() == 2;
}

// ==================== МЕТОДЫ ДЛЯ РАБОТЫ С РУКОЙ ====================

const std::vector<Card>& Player::getHand() const {
    return hand_;
}

void Player::clearHand() {
    hand_.clear();
}

/**
 * @brief Разделить руку на две
 * @param deck Колода для взятия дополнительных карт
 * @return Вторая рука после разделения
 */
std::vector<Card> Player::splitHand(Deck& deck) {
    std::vector<Card> secondHand;
    if (canSplit()) {
        // Берем вторую карту для новой руки
        secondHand.push_back(hand_.back());
        hand_.pop_back(); // Убираем ее из текущей руки

        // Добавляем новые карты в обе руки
        takeCard(deck); // для текущей руки
    }
    return secondHand;
}

void Player::setHand(const std::vector<Card>& newHand) {
    hand_ = newHand;
}

// ==================== СТАТИСТИКА И РЕЗУЛЬТАТЫ ====================

void Player::recordWin() {
    gamesPlayed_++;
    gamesWon_++;
    updateWinRate();
}

void Player::recordLoss() {
    gamesPlayed_++;
    gamesLost_++;
    updateWinRate();
}

void Player::recordPush() {
    gamesPlayed_++;
    gamesPushed_++;
    updateWinRate();
}

void Player::updateMaxScore(int score) {
    if (score > maxScore_) {
        maxScore_ = score;
    }
}

void Player::showStats() const {
    std::cout << "\n--- Statistics " << name_ << " ---\n";
    std::cout << "Games played: " << gamesPlayed_ << "\n";
    std::cout << "Wins: " << gamesWon_ << "\n";
    std::cout << "Losses: " << gamesLost_ << "\n";
    std::cout << "Pushes: " << gamesPushed_ << "\n";
    std::cout << "Win Rate: " << std::fixed << std::setprecision(1) << winRate_ << "%\n";
    std::cout << "Max Score: " << maxScore_ << "\n";
}

// ==================== СЕТЕВЫЕ И ВСПОМОГАТЕЛЬНЫЕ МЕТОДЫ ====================

std::string Player::getHandAsString() const {
    if (hand_.empty()) {
        return "Empty hand";
    }

    std::string result;
    for (size_t i = 0; i < hand_.size(); ++i) {
        result += hand_[i].toString();
        if (i < hand_.size() - 1) {
            result += ", ";
        }
    }
    result += " | Score: " + std::to_string(calculateScore());
    return result;
}

std::string Player::getStatsAsString() const {
    return name_ + " - Games: " + std::to_string(gamesPlayed_) +
        " | Wins: " + std::to_string(gamesWon_) +
        " | Win Rate: " + std::to_string(static_cast<int>(winRate_)) + "%" +
        " | Best Score: " + std::to_string(maxScore_);
}

std::vector<PlayerAction> Player::getAvailableActions() const {
    std::vector<PlayerAction> actions;

    // Всегда доступные действия
    actions.push_back(PlayerAction::Hit);
    actions.push_back(PlayerAction::Stand);

    // Условно доступные действия
    if (canDoubleDown()) {
        actions.push_back(PlayerAction::DoubleDown);
    }
    if (canSplit()) {
        actions.push_back(PlayerAction::Split);
    }

    return actions;
}

std::string Player::getActionsAsString() const {
    auto actions = getAvailableActions();
    std::string result = "Available actions: ";

    for (size_t i = 0; i < actions.size(); ++i) {
        switch (actions[i]) {
        case PlayerAction::Hit:
            result += std::to_string(i + 1) + ".Hit ";
            break;
        case PlayerAction::Stand:
            result += std::to_string(i + 1) + ".Stand ";
            break;
        case PlayerAction::DoubleDown:
            result += std::to_string(i + 1) + ".DoubleDown ";
            break;
        case PlayerAction::Split:
            result += std::to_string(i + 1) + ".Split ";
            break;
        }
    }
    return result;
}

PlayerAction Player::convertNetworkChoice(int networkChoice) const {
    if (isBusted()) {
        return PlayerAction::Stand;
    }

    auto availableActions = getAvailableActions();
    if (networkChoice >= 1 && networkChoice <= static_cast<int>(availableActions.size())) {
        return availableActions[networkChoice - 1];
    }

    // Fallback на Stand при некорректном вводе
    return PlayerAction::Stand;
}