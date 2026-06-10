#ifndef BOOK_H
#define BOOK_H

#include <string>
#include <iostream>
#include <algorithm>
#include <cctype>

class Book {
public:
    Book() = default;
    Book(const std::string& id, const std::string& title, const std::string& author,
        const std::string& genre, int year, int copies, double price) {
        setId(id);
        setTitle(title);
        setAuthor(author);
        setGenre(genre);
        setYear(year);
        setCopies(copies);
        setPrice(price);
    }

    std::string getId() const { return id; }
    std::string getTitle() const { return title; }
    std::string getAuthor() const { return author; }
    std::string getGenre() const { return genre; }
    int getYear() const { return year; }
    int getCopies() const { return copies; }
    double getPrice() const { return price; }

    bool setId(const std::string& newId) {
        if (isNotEmpty(newId)) {
            id = newId;
            return true;
        }
        return false;
    }

    bool setTitle(const std::string& newTitle) {
        if (isNotEmpty(newTitle)) {
            title = newTitle;
            return true;
        }
        return false;
    }

    bool setAuthor(const std::string& newAuthor) {
        if (isNotEmpty(newAuthor)) {
            author = newAuthor;
            return true;
        }
        return false;
    }

    bool setGenre(const std::string& newGenre) {
        if (isNotEmpty(newGenre)) {
            genre = newGenre;
            return true;
        }
        return false;
    }

    bool setYear(int newYear) {
        if (newYear > 0) {
            year = newYear;
            return true;
        }
        return false;
    }

    bool setCopies(int newCopies) {
        if (newCopies >= 0) {
            copies = newCopies;
            return true;
        }
        return false;
    }

    bool setPrice(double newPrice) {
        if (newPrice >= 0.0) {
            price = newPrice;
            return true;
        }
        return false;
    }

    void display() const {
        std::cout << "ID: " << id << "\n"
            << "Title: " << title << "\n"
            << "Author: " << author << "\n"
            << "Genre: " << genre << "\n"
            << "Year: " << year << "\n"
            << "Copies: " << copies << "\n"
            << "Price: " << price << "\n";
    }

private:
    std::string id;
    std::string title;
    std::string author;
    std::string genre;
    int year = 0;
    int copies = 0;
    double price = 0.0;

    static bool isNotEmpty(const std::string& str) {
        return !str.empty() && std::any_of(str.begin(), str.end(),
            [](char c) { return !std::isspace(static_cast<unsigned char>(c)); });
    }
};

#endif