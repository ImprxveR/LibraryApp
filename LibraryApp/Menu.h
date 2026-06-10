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
                std::cout << "Invalid input. Please enter a number.\n";
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
            case 0: std::cout << "Goodbye!\n"; break;
            default: std::cout << "Invalid choice.\n"; break;
            }
            if (choice != 0) {
                std::cout << "\nPress Enter to continue...";
                std::cin.get();
            }
        } while (choice != 0);
    }

private:
    Library& library;

    void showMainMenu() {
        std::cout << "\n========== LIBRARY CATALOG ==========\n"
            << "1. Add book\n"
            << "2. Edit book\n"
            << "3. Remove book\n"
            << "4. Search books\n"
            << "5. Filter catalog\n"
            << "6. Sort\n"
            << "7. Analytics\n"
            << "8. Show all books\n"
            << "9. Save data to file\n"
            << "0. Exit\n"
            << "Your choice: ";
    }

    std::string inputString(const std::string& prompt, bool allowEmpty = false) {
        std::string input;
        while (true) {
            std::cout << prompt;
            std::getline(std::cin, input);
            size_t start = input.find_first_not_of(" \t");
            if (start == std::string::npos) {
                if (allowEmpty) return "";
                std::cout << "Field cannot be empty. Try again.\n";
                continue;
            }
            size_t end = input.find_last_not_of(" \t");
            input = input.substr(start, end - start + 1);
            if (!allowEmpty && input.empty()) {
                std::cout << "Field cannot be empty. Try again.\n";
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
                std::cout << "Error: enter an integer from " << minValue << " to " << maxValue << ".\n";
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
                std::cout << "Error: enter a number not less than " << minValue << ".\n";
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
            std::cout << "No books matching criteria.\n";
            return;
        }
        for (size_t i = 0; i < books.size(); ++i) {
            std::cout << "\n--- Book " << (i + 1) << " ---\n";
            books[i].display();
        }
    }

    void handleAddBook() {
        std::cout << "\n--- Add new book ---\n";
        std::string id = inputString("Enter unique ID (e.g., ISBN): ");
        if (library.findBookById(id) != nullptr) {
            std::cout << "Book with this ID already exists.\n";
            return;
        }
        std::string title = inputString("Title: ");
        std::string author = inputString("Author: ");
        std::string genre = inputString("Genre: ");
        int year = inputInt("Year (>0): ", 1);
        int copies = inputInt("Number of copies (>=0): ", 0);
        double price = inputDouble("Price (>=0): ", 0.0);

        Book newBook(id, title, author, genre, year, copies, price);
        if (newBook.getId().empty() || newBook.getTitle().empty() || newBook.getAuthor().empty() ||
            newBook.getGenre().empty() || newBook.getYear() <= 0 || newBook.getCopies() < 0 || newBook.getPrice() < 0) {
            std::cout << "Error: invalid data. Book not added.\n";
            return;
        }
        if (library.addBook(newBook))
            std::cout << "Book added successfully.\n";
    }

    void handleEditBook() {
        std::cout << "\n--- Edit book ---\n";
        std::string id = inputString("Enter book ID to edit: ");
        Book* book = library.findBookById(id);
        if (!book) {
            std::cout << "Book not found.\n";
            return;
        }
        std::cout << "Current data:\n";
        book->display();
        std::cout << "\nEnter new data (leave empty to keep unchanged):\n";

        std::string newId = inputString("New ID (unique): ", true);
        std::string newTitle = inputString("New title: ", true);
        std::string newAuthor = inputString("New author: ", true);
        std::string newGenre = inputString("New genre: ", true);
        std::string yearStr = inputString("New year (>0): ", true);
        std::string copiesStr = inputString("New copies (>=0): ", true);
        std::string priceStr = inputString("New price (>=0): ", true);

        Book updated = *book;
        if (!newId.empty()) {
            if (newId != id && library.findBookById(newId) != nullptr) {
                std::cout << "ID " << newId << " already taken. Edit cancelled.\n";
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
            else std::cout << "Invalid year. Change skipped.\n";
        }
        if (!copiesStr.empty()) {
            int c = std::stoi(copiesStr);
            if (c >= 0) updated.setCopies(c);
            else std::cout << "Invalid copies. Change skipped.\n";
        }
        if (!priceStr.empty()) {
            double p = std::stod(priceStr);
            if (p >= 0) updated.setPrice(p);
            else std::cout << "Invalid price. Change skipped.\n";
        }

        if (library.editBook(id, updated))
            std::cout << "Book edited successfully.\n";
    }

    void handleRemoveBook() {
        std::cout << "\n--- Remove book ---\n";
        std::cout << "1. Remove by ID\n2. Remove by title\nChoice: ";
        int choice;
        std::cin >> choice;
        std::cin.ignore();
        if (choice == 1) {
            std::string id = inputString("Enter ID: ");
            if (library.removeBookById(id))
                std::cout << "Book removed.\n";
        }
        else if (choice == 2) {
            std::string title = inputString("Enter title: ");
            if (library.removeBookByTitle(title))
                std::cout << "Book removed.\n";
        }
        else {
            std::cout << "Invalid choice.\n";
        }
    }

    void handleSearch() {
        std::cout << "\n--- Search books ---\n";
        std::cout << "1. By title\n2. By author\n3. By genre\nChoice: ";
        int choice;
        std::cin >> choice;
        std::cin.ignore();
        std::string query;
        std::vector<Book> result;
        switch (choice) {
        case 1:
            query = inputString("Enter part of title: ");
            result = library.searchByTitle(query);
            displayBooks(result, "Search results by title");
            break;
        case 2:
            query = inputString("Enter part of author name: ");
            result = library.searchByAuthor(query);
            displayBooks(result, "Search results by author");
            break;
        case 3:
            query = inputString("Enter part of genre: ");
            result = library.searchByGenre(query);
            displayBooks(result, "Search results by genre");
            break;
        default: std::cout << "Invalid choice.\n";
        }
    }

    void handleFilter() {
        std::cout << "\n--- Filter ---\n";
        std::cout << "1. By year range\n2. By price range\nChoice: ";
        int choice;
        std::cin >> choice;
        std::cin.ignore();
        if (choice == 1) {
            int from = inputInt("Year from: ", 1);
            int to = inputInt("Year to: ", from);
            auto result = library.filterByYearRange(from, to);
            displayBooks(result, "Filter by year");
        }
        else if (choice == 2) {
            double from = inputDouble("Price from: ", 0.0);
            double to = inputDouble("Price to: ", from);
            auto result = library.filterByPriceRange(from, to);
            displayBooks(result, "Filter by price");
        }
        else {
            std::cout << "Invalid choice.\n";
        }
    }

    void handleSort() {
        std::cout << "\n--- Sort ---\n";
        std::cout << "1. By year\n2. By copies\n3. By title (alphabetical)\n4. By author (alphabetical)\nChoice: ";
        int choice;
        std::cin >> choice;
        std::cin.ignore();
        int dirChoice;
        std::cout << "Order: 1 - ascending, 2 - descending: ";
        std::cin >> dirChoice;
        std::cin.ignore();
        bool ascending = (dirChoice == 1);

        switch (choice) {
        case 1: library.sortByYear(ascending); break;
        case 2: library.sortByCopies(ascending); break;
        case 3: library.sortByTitle(ascending); break;
        case 4: library.sortByAuthor(ascending); break;
        default: std::cout << "Invalid choice.\n"; return;
        }
        std::cout << "Sorting completed.\n";
    }

    void handleAnalytics() {
        std::cout << "\n--- Analytics ---\n";
        double total = library.getTotalValue();
        std::cout << "Total collection value: " << total << " rub.\n";

        auto needRestock = library.getBooksNeedingRestock(2);
        std::cout << "\nBooks needing restock (copies <= 2):\n";
        if (needRestock.empty()) std::cout << "No such books.\n";
        else for (const auto& b : needRestock) {
            std::cout << b.getTitle() << " (remaining: " << b.getCopies() << ")\n";
        }

        auto top3 = library.getTop3MostExpensive();
        std::cout << "\nTop 3 most expensive books:\n";
        if (top3.empty()) std::cout << "No books.\n";
        else for (size_t i = 0; i < top3.size(); ++i) {
            std::cout << i + 1 << ". " << top3[i].getTitle() << " - " << top3[i].getPrice() << " rub.\n";
        }
    }

    void handleListAll() {
        displayBooks(library.getAllBooks(), "All books in catalog");
    }

    void handleSave() {
        if (FileIO::saveToFile("data/catalog.txt", library))
            std::cout << "Save successful.\n";
        else
            std::cout << "Save error.\n";
    }
};

#endif