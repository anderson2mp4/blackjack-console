#include "card.h"

/**
 * @brief Конструктор карты
 * @param suit Масть карты
 * @param rank Достоинство карты
 */
Card::Card(Suit suit, Rank rank)
    : suit_(suit), rank_(rank) {
}

// ==================== ГЕТТЕРЫ ====================

Suit Card::getSuit() const {
    return suit_;
}

Rank Card::getRank() const {
    return rank_;
}

// ==================== ИГРОВАЯ ЛОГИКА ====================

int Card::getValue() const {
    // Картинки (J, Q, K) дают 10 очков
    if (rank_ == Rank::Jack || rank_ == Rank::Queen || rank_ == Rank::King) {
        return 10;
    }
    // Туз дает 1 очко (гибкость обработки в классе Player)
    else if (rank_ == Rank::Ace) {
        return 1;
    }
    // Числовые карты дают свое значение
    else {
        return static_cast<int>(rank_);
    }
}

bool Card::isAce() const {
    return rank_ == Rank::Ace;
}

// ==================== МЕТОДЫ ОТОБРАЖЕНИЯ ====================

std::ostream& operator<<(std::ostream& os, const Card& card) {
    // Вывод достоинства
    switch (card.rank_) {
    case Rank::Jack:  os << "J"; break;
    case Rank::Queen: os << "Q"; break;
    case Rank::King:  os << "K"; break;
    case Rank::Ace:   os << "A"; break;
    default:          os << static_cast<int>(card.rank_); break;
    }

    // Вывод масти
    switch (card.suit_) {
    case Suit::Hearts:   os << "H"; break;
    case Suit::Diamonds: os << "D"; break;
    case Suit::Clubs:    os << "C"; break;
    case Suit::Spades:   os << "S"; break;
    }

    return os;
}

std::string Card::toString() const {
    std::string result;

    // Преобразование достоинства в символ
    switch (rank_) {
    case Rank::Two:   result += "2"; break;
    case Rank::Three: result += "3"; break;
    case Rank::Four:  result += "4"; break;
    case Rank::Five:  result += "5"; break;
    case Rank::Six:   result += "6"; break;
    case Rank::Seven: result += "7"; break;
    case Rank::Eight: result += "8"; break;
    case Rank::Nine:  result += "9"; break;
    case Rank::Ten:   result += "10"; break;
    case Rank::Jack:  result += "J"; break;
    case Rank::Queen: result += "Q"; break;
    case Rank::King:  result += "K"; break;
    case Rank::Ace:   result += "A"; break;
    }

    // Преобразование масти в символ
    switch (suit_) {
    case Suit::Hearts:   result += "H"; break;
    case Suit::Diamonds: result += "D"; break;
    case Suit::Clubs:    result += "C"; break;
    case Suit::Spades:   result += "S"; break;
    }

    return result;
}

std::vector<std::string> Card::getAsASCII() const {
    std::string rank = getRankSymbol();

    // Выравнивание для двузначного числа (10)
    std::string topRank = (rank == "10") ? "10" : rank + " ";
    std::string bottomRank = (rank == "10") ? "10" : " " + rank;

    // ASCII-представление карты с отступами для центрирования
    return {
        "           +-----+",
        "           |" + topRank + "   |",
        "           |  " + getSuitSymbol() + "  |",
        "           |   " + bottomRank + "|",
        "           +-----+"
    };
}

// ==================== ПРИВАТНЫЕ ВСПОМОГАТЕЛЬНЫЕ МЕТОДЫ ====================

std::string Card::getRankSymbol() const {
    switch (rank_) {
    case Rank::Ace:   return "A";
    case Rank::King:  return "K";
    case Rank::Queen: return "Q";
    case Rank::Jack:  return "J";
    case Rank::Ten:   return "10";
    case Rank::Nine:  return "9";
    case Rank::Eight: return "8";
    case Rank::Seven: return "7";
    case Rank::Six:   return "6";
    case Rank::Five:  return "5";
    case Rank::Four:  return "4";
    case Rank::Three: return "3";
    case Rank::Two:   return "2";
    default:          return "?";
    }
}

std::string Card::getSuitSymbol() const {
    switch (suit_) {
    case Suit::Hearts:   return "H";
    case Suit::Diamonds: return "D";
    case Suit::Clubs:    return "C";
    case Suit::Spades:   return "S";
    default:             return "?";
    }
}

