#include <iostream>
#include "game.h"

/**
 * @brief Точка входа в приложение Blackjack
 *
 * Создает и запускает игровой экземпляр, управляет жизненным циклом приложения
 *
 * @return Код завершения программы (0 - успешное завершение)
 */
int main() {
    // Настройка локализации для корректного отображения символов
    setlocale(LC_ALL, "Russian");

    std::cout << "=== BLACKJACK GAME ===\n";
    std::cout << "Initializing game...\n\n";

    try {
        // Создание и запуск игрового экземпляра
        Game blackjackGame;
        blackjackGame.startGame();

        std::cout << "\nGame session completed successfully.\n";
    }
    catch (const std::exception& e) {
        // Обработка критических ошибок
        std::cerr << "\n*** CRITICAL ERROR ***\n";
        std::cerr << "Game terminated unexpectedly: " << e.what() << "\n";
        std::cerr << "Please restart the application.\n";

        std::cout << "\nPress Enter to exit...";
        std::cin.get();
        return 1; // Возврат кода ошибки
    }
    catch (...) {
        // Обработка непредвиденных ошибок
        std::cerr << "\n*** UNKNOWN ERROR ***\n";
        std::cerr << "Game terminated due to unexpected error.\n";
        std::cerr << "Please restart the application.\n";

        std::cout << "\nPress Enter to exit...";
        std::cin.get();
        return 1; // Возврат кода ошибки
    }

    // Корректное завершение программы
    std::cout << "\nPress Enter to exit...";
    std::cin.get();
    return 0;
}