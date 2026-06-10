#ifndef FILEIO_H
#define FILEIO_H

#include "Library.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <sys/stat.h>

#ifdef _WIN32
#include <direct.h>
#define mkdir _mkdir
#else
#include <sys/stat.h>
#endif

class FileIO {
public:
    static bool loadFromFile(const std::string& filename, Library& library) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cout << "File " << filename << " not found. New one will be created on save.\n";
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
                std::cout << "Warning: line " << lineNum << " has invalid format, skipped.\n";
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
                std::cout << "Warning: line " << lineNum << " contains non-numeric data, skipped.\n";
                continue;
            }

            Book book(id, title, author, genre, year, copies, price);
            if (book.getId().empty() || book.getTitle().empty() || book.getAuthor().empty() ||
                book.getGenre().empty() || book.getYear() <= 0 || book.getCopies() < 0 || book.getPrice() < 0) {
                std::cout << "Warning: line " << lineNum << " contains invalid data, skipped.\n";
                continue;
            }
            library.addBook(book);
        }
        file.close();
        return true;
    }

    static bool saveToFile(const std::string& filename, const Library& library) {
        // Создаём директорию, если её нет
        std::string dir = filename;
        size_t lastSlash = dir.find_last_of("/\\");
        if (lastSlash != std::string::npos) {
            dir = dir.substr(0, lastSlash);
#ifdef _WIN32
            mkdir(dir.c_str());
#else
            mkdir(dir.c_str(), 0777);
#endif
        }

        std::ofstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Error: could not open file " << filename << " for writing.\n";
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
        std::cout << "Data saved to " << filename << "\n";
        return true;
    }
};

#endif // FILEIO_H