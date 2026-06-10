#ifndef FILEIO_H
#define FILEIO_H

#include "Library.h"
#include <fstream>
#include <sstream>
#include <iostream>

class FileIO {
public:
    static bool loadFromFile(const std::string& filename, Library& library) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cout << "Файл " << filename << " не найден. Будет создан новый при сохранении.\n";
            return false;
        }

        std::string line;
        int lineNum = 0;
        while (std::getline(file, line)) {
            lineNum++;
            if (line.empty()) continue;

            std::stringstream ss(line);
            std::string id, title, author, genre, yearStr, copiesStr, priceStr;

            if (!std::getline(ss, id, ',') ||
                !std::getline(ss, title, ',') ||
                !std::getline(ss, author, ',') ||
                !std::getline(ss, genre, ',') ||
                !std::getline(ss, yearStr, ',') ||
                !std::getline(ss, copiesStr, ',') ||
                !std::getline(ss, priceStr)) {
                std::cout << "Предупреждение: строка " << lineNum << " имеет неверный формат, пропущена.\n";
                continue;
            }

            int year = 0, copies = 0;
            double price = 0.0;
            try {
                year = std::stoi(yearStr);
                copies = std::stoi(copiesStr);
                price = std::stod(priceStr);
            }
            catch (...) {
                std::cout << "Предупреждение: строка " << lineNum << " содержит нечисловые данные, пропущена.\n";
                continue;
            }

            Book book(id, title, author, genre, year, copies, price);
            if (book.getId().empty() || book.getTitle().empty() || book.getAuthor().empty() ||
                book.getGenre().empty() || book.getYear() <= 0 || book.getCopies() < 0 || book.getPrice() < 0) {
                std::cout << "Предупреждение: строка " << lineNum << " содержит некорректные данные, пропущена.\n";
                continue;
            }
            library.addBook(book);
        }
        file.close();
        return true;
    }

    static bool saveToFile(const std::string& filename, const Library& library) {
        std::ofstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Ошибка: не удалось открыть файл " << filename << " для записи.\n";
            return false;
        }

        for (const auto& book : library.getAllBooks()) {
            file << book.getId() << ","
                << book.getTitle() << ","
                << book.getAuthor() << ","
                << book.getGenre() << ","
                << book.getYear() << ","
                << book.getCopies() << ","
                << book.getPrice() << "\n";
        }
        file.close();
        std::cout << "Данные сохранены в " << filename << "\n";
        return true;
    }
};

#endif