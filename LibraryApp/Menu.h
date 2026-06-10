#ifndef MENU_H
#define MENU_H

#include "Library.h"
#include "FileIO.h"
#include <iostream>
#include <limits>
#include <string>

class Menu {
public:
    Menu(Library& lib) : library(lib) {}

    void run() {
        int choice;
        do {
            showMainMenu();
            std::cin >> choice;
            if (std::cin.fail()) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Неверный ввод. Пожалуйста, введите число.\n";
                continue;
            }
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

            switch (choice) {
            case 1: handleAddBook(); break;
            case 2: handleEditBook(); break;
            case 3: handleRemoveBook(); break;
            case 4: handleSearch(); break;
            case 5: handleFilter(); break;
            case 6: handleSort(); break;
            case 7: handleAnalytics(); break;
            case 8: handleListAll(); break;
            case 9: handleSave(); break;
            case 0: std::cout << "До свидания!\n"; break;
            default: std::cout << "Неверный пункт меню.\n"; break;
            }
            if (choice != 0) {
                std::cout << "\nНажмите Enter для продолжения...";
                std::cin.get();
            }
        } while (choice != 0);
    }

private:
    Library& library;

    void showMainMenu() {
        std::cout << "\n========== БИБЛИОТЕЧНЫЙ КАТАЛОГ ==========\n"
            << "1. Добавить книгу\n"
            << "2. Редактировать книгу\n"
            << "3. Удалить книгу\n"
            << "4. Поиск книг\n"
            << "5. Фильтрация каталога\n"
            << "6. Сортировка\n"
            << "7. Аналитика\n"
            << "8. Показать все книги\n"
            << "9. Сохранить данные в файл\n"
            << "0. Выход\n"
            << "Ваш выбор: ";
    }

    std::string inputString(const std::string& prompt, bool allowEmpty = false) {
        std::string input;
        while (true) {
            std::cout << prompt;
            std::getline(std::cin, input);
            size_t start = input.find_first_not_of(" \t");
            if (start == std::string::npos) {
                if (allowEmpty) return "";
                std::cout << "Поле не может быть пустым. Попробуйте снова.\n";
                continue;
            }
            size_t end = input.find_last_not_of(" \t");
            input = input.substr(start, end - start + 1);
            if (!allowEmpty && input.empty()) {
                std::cout << "Поле не может быть пустым. Попробуйте снова.\n";
                continue;
            }
            return input;
        }
    }

    int inputInt(const std::string& prompt, int minValue = 1, int maxValue = INT_MAX) {
        int value;
        while (true) {
            std::cout << prompt;
            std::cin >> value;
            if (std::cin.fail() || value < minValue || value > maxValue) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Ошибка: введите целое число от " << minValue << " до " << maxValue << ".\n";
            }
            else {
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                return value;
            }
        }
    }

    double inputDouble(const std::string& prompt, double minValue = 0.0) {
        double value;
        while (true) {
            std::cout << prompt;
            std::cin >> value;
            if (std::cin.fail() || value < minValue) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Ошибка: введите число не менее " << minValue << ".\n";
            }
            else {
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                return value;
            }
        }
    }

    void displayBooks(const std::vector<Book>& books, const std::string& title) {
        std::cout << "\n=== " << title << " ===\n";
        if (books.empty()) {
            std::cout << "Нет книг, соответствующих критериям.\n";
            return;
        }
        for (size_t i = 0; i < books.size(); ++i) {
            std::cout << "\n--- Книга " << (i + 1) << " ---\n";
            books[i].display();
        }
    }

    void handleAddBook() {
        std::cout << "\n--- Добавление новой книги ---\n";
        std::string id = inputString("Введите уникальный ID (например, ISBN): ");
        if (library.findBookById(id) != nullptr) {
            std::cout << "Книга с таким ID уже существует.\n";
            return;
        }
        std::string title = inputString("Название: ");
        std::string author = inputString("Автор: ");
        std::string genre = inputString("Жанр: ");
        int year = inputInt("Год издания (>0): ", 1);
        int copies = inputInt("Количество экземпляров (>=0): ", 0);
        double price = inputDouble("Цена (>=0): ", 0.0);

        Book newBook(id, title, author, genre, year, copies, price);
        if (newBook.getId().empty() || newBook.getTitle().empty() || newBook.getAuthor().empty() ||
            newBook.getGenre().empty() || newBook.getYear() <= 0 || newBook.getCopies() < 0 || newBook.getPrice() < 0) {
            std::cout << "Ошибка: некорректные данные. Книга не добавлена.\n";
            return;
        }
        if (library.addBook(newBook))
            std::cout << "Книга успешно добавлена.\n";
    }

    void handleEditBook() {
        std::cout << "\n--- Редактирование книги ---\n";
        std::string id = inputString("Введите ID книги для редактирования: ");
        Book* book = library.findBookById(id);
        if (!book) {
            std::cout << "Книга не найдена.\n";
            return;
        }
        std::cout << "Текущие данные:\n";
        book->display();
        std::cout << "\nВведите новые данные (оставьте пустым, чтобы не менять):\n";

        std::string newId = inputString("Новый ID (уникальный): ", true);
        std::string newTitle = inputString("Новое название: ", true);
        std::string newAuthor = inputString("Новый автор: ", true);
        std::string newGenre = inputString("Новый жанр: ", true);
        std::string yearStr = inputString("Новый год (>0): ", true);
        std::string copiesStr = inputString("Новое количество (>=0): ", true);
        std::string priceStr = inputString("Новая цена (>=0): ", true);

        Book updated = *book;
        if (!newId.empty()) {
            if (newId != id && library.findBookById(newId) != nullptr) {
                std::cout << "ID " << newId << " уже занят. Редактирование отменено.\n";
                return;
            }
            updated.setId(newId);
        }
        if (!newTitle.empty()) updated.setTitle(newTitle);
        if (!newAuthor.empty()) updated.setAuthor(newAuthor);
        if (!newGenre.empty()) updated.setGenre(newGenre);
        if (!yearStr.empty()) {
            int y = std::stoi(yearStr);
            if (y > 0) updated.setYear(y);
            else std::cout << "Неверный год. Изменение пропущено.\n";
        }
        if (!copiesStr.empty()) {
            int c = std::stoi(copiesStr);
            if (c >= 0) updated.setCopies(c);
            else std::cout << "Неверное количество. Изменение пропущено.\n";
        }
        if (!priceStr.empty()) {
            double p = std::stod(priceStr);
            if (p >= 0) updated.setPrice(p);
            else std::cout << "Неверная цена. Изменение пропущено.\n";
        }

        if (library.editBook(id, updated))
            std::cout << "Книга успешно отредактирована.\n";
    }

    void handleRemoveBook() {
        std::cout << "\n--- Удаление книги ---\n";
        std::cout << "1. Удалить по ID\n2. Удалить по названию\nВыбор: ";
        int choice;
        std::cin >> choice;
        std::cin.ignore();
        if (choice == 1) {
            std::string id = inputString("Введите ID: ");
            if (library.removeBookById(id))
                std::cout << "Книга удалена.\n";
        }
        else if (choice == 2) {
            std::string title = inputString("Введите название: ");
            if (library.removeBookByTitle(title))
                std::cout << "Книга удалена.\n";
        }
        else {
            std::cout << "Неверный выбор.\n";
        }
    }

    void handleSearch() {
        std::cout << "\n--- Поиск книг ---\n";
        std::cout << "1. По названию\n2. По автору\n3. По жанру\nВыбор: ";
        int choice;
        std::cin >> choice;
        std::cin.ignore();
        std::string query;
        std::vector<Book> result;
        switch (choice) {
        case 1:
            query = inputString("Введите часть названия: ");
            result = library.searchByTitle(query);
            displayBooks(result, "Результаты поиска по названию");
            break;
        case 2:
            query = inputString("Введите часть имени автора: ");
            result = library.searchByAuthor(query);
            displayBooks(result, "Результаты поиска по автору");
            break;
        case 3:
            query = inputString("Введите часть жанра: ");
            result = library.searchByGenre(query);
            displayBooks(result, "Результаты поиска по жанру");
            break;
        default: std::cout << "Неверный выбор.\n";
        }
    }

    void handleFilter() {
        std::cout << "\n--- Фильтрация ---\n";
        std::cout << "1. По диапазону года издания\n2. По диапазону цены\nВыбор: ";
        int choice;
        std::cin >> choice;
        std::cin.ignore();
        if (choice == 1) {
            int from = inputInt("Год от: ", 1);
            int to = inputInt("Год до: ", from);
            auto result = library.filterByYearRange(from, to);
            displayBooks(result, "Фильтр по году");
        }
        else if (choice == 2) {
            double from = inputDouble("Цена от: ", 0.0);
            double to = inputDouble("Цена до: ", from);
            auto result = library.filterByPriceRange(from, to);
            displayBooks(result, "Фильтр по цене");
        }
        else {
            std::cout << "Неверный выбор.\n";
        }
    }

    void handleSort() {
        std::cout << "\n--- Сортировка ---\n";
        std::cout << "1. По году издания\n2. По количеству экземпляров\n3. По названию (алфавит)\n4. По автору (алфавит)\nВыбор: ";
        int choice;
        std::cin >> choice;
        std::cin.ignore();
        int dirChoice;
        std::cout << "Направление: 1 - по возрастанию, 2 - по убыванию: ";
        std::cin >> dirChoice;
        std::cin.ignore();
        bool ascending = (dirChoice == 1);

        switch (choice) {
        case 1: library.sortByYear(ascending); break;
        case 2: library.sortByCopies(ascending); break;
        case 3: library.sortByTitle(ascending); break;
        case 4: library.sortByAuthor(ascending); break;
        default: std::cout << "Неверный выбор.\n"; return;
        }
        std::cout << "Сортировка выполнена.\n";
    }

    void handleAnalytics() {
        std::cout << "\n--- Аналитика ---\n";
        double total = library.getTotalValue();
        std::cout << "Общая стоимость фонда: " << total << " руб.\n";

        auto needRestock = library.getBooksNeedingRestock(2);
        std::cout << "\nКниги, требующие пополнения (остаток <= 2):\n";
        if (needRestock.empty()) std::cout << "Нет таких книг.\n";
        else for (const auto& b : needRestock) {
            std::cout << b.getTitle() << " (остаток: " << b.getCopies() << ")\n";
        }

        auto top3 = library.getTop3MostExpensive();
        std::cout << "\nТоп-3 самых дорогих книг:\n";
        if (top3.empty()) std::cout << "Нет книг.\n";
        else for (size_t i = 0; i < top3.size(); ++i) {
            std::cout << i + 1 << ". " << top3[i].getTitle() << " - " << top3[i].getPrice() << " руб.\n";
        }
    }

    void handleListAll() {
        displayBooks(library.getAllBooks(), "Все книги в каталоге");
    }

    void handleSave() {
        if (FileIO::saveToFile("data/catalog.txt", library))
            std::cout << "Сохранение успешно выполнено.\n";
        else
            std::cout << "Ошибка сохранения.\n";
    }
};

#endif