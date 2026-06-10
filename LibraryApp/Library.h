#ifndef LIBRARY_H
#define LIBRARY_H

#include "Book.h"
#include <vector>
#include <string>
#include <algorithm>
#include <cctype>
#include <iostream>

class Library {
public:
    bool addBook(const Book& book) {
        if (!isIdUnique(book.getId())) {
            std::cout << "Ошибка: книга с ID " << book.getId() << " уже существует.\n";
            return false;
        }
        books.push_back(book);
        return true;
    }

    bool editBook(const std::string& id, const Book& newData) {
        for (auto& book : books) {
            if (book.getId() == id) {
                if (newData.getId() != id && !isIdUnique(newData.getId())) {
                    std::cout << "Ошибка: ID " << newData.getId() << " уже используется.\n";
                    return false;
                }
                book = newData;
                return true;
            }
        }
        std::cout << "Книга с ID " << id << " не найдена.\n";
        return false;
    }

    bool removeBookById(const std::string& id) {
        auto it = std::find_if(books.begin(), books.end(),
            [&id](const Book& b) { return b.getId() == id; });
        if (it != books.end()) {
            books.erase(it);
            return true;
        }
        std::cout << "Книга с ID " << id << " не найдена.\n";
        return false;
    }

    bool removeBookByTitle(const std::string& title) {
        auto it = std::find_if(books.begin(), books.end(),
            [&title](const Book& b) { return toLower(b.getTitle()) == toLower(title); });
        if (it != books.end()) {
            books.erase(it);
            return true;
        }
        std::cout << "Книга с названием \"" << title << "\" не найдена.\n";
        return false;
    }

    std::vector<Book> searchByTitle(const std::string& query) const {
        std::vector<Book> result;
        std::copy_if(books.begin(), books.end(), std::back_inserter(result),
            [&query](const Book& b) { return containsIgnoreCase(b.getTitle(), query); });
        return result;
    }

    std::vector<Book> searchByAuthor(const std::string& query) const {
        std::vector<Book> result;
        std::copy_if(books.begin(), books.end(), std::back_inserter(result),
            [&query](const Book& b) { return containsIgnoreCase(b.getAuthor(), query); });
        return result;
    }

    std::vector<Book> searchByGenre(const std::string& query) const {
        std::vector<Book> result;
        std::copy_if(books.begin(), books.end(), std::back_inserter(result),
            [&query](const Book& b) { return containsIgnoreCase(b.getGenre(), query); });
        return result;
    }

    std::vector<Book> filterByYearRange(int from, int to) const {
        std::vector<Book> result;
        std::copy_if(books.begin(), books.end(), std::back_inserter(result),
            [from, to](const Book& b) { return b.getYear() >= from && b.getYear() <= to; });
        return result;
    }

    std::vector<Book> filterByPriceRange(double from, double to) const {
        std::vector<Book> result;
        std::copy_if(books.begin(), books.end(), std::back_inserter(result),
            [from, to](const Book& b) { return b.getPrice() >= from && b.getPrice() <= to; });
        return result;
    }

    void sortByYear(bool ascending = true) {
        std::sort(books.begin(), books.end(),
            [ascending](const Book& a, const Book& b) {
                return ascending ? a.getYear() < b.getYear() : a.getYear() > b.getYear();
            });
    }

    void sortByCopies(bool ascending = true) {
        std::sort(books.begin(), books.end(),
            [ascending](const Book& a, const Book& b) {
                return ascending ? a.getCopies() < b.getCopies() : a.getCopies() > b.getCopies();
            });
    }

    void sortByTitle(bool ascending = true) {
        std::sort(books.begin(), books.end(),
            [ascending](const Book& a, const Book& b) {
                return ascending ? toLower(a.getTitle()) < toLower(b.getTitle())
                    : toLower(a.getTitle()) > toLower(b.getTitle());
            });
    }

    void sortByAuthor(bool ascending = true) {
        std::sort(books.begin(), books.end(),
            [ascending](const Book& a, const Book& b) {
                return ascending ? toLower(a.getAuthor()) < toLower(b.getAuthor())
                    : toLower(a.getAuthor()) > toLower(b.getAuthor());
            });
    }

    double getTotalValue() const {
        double total = 0.0;
        for (const auto& book : books) {
            total += book.getPrice() * book.getCopies();
        }
        return total;
    }

    std::vector<Book> getBooksNeedingRestock(int threshold = 2) const {
        std::vector<Book> result;
        std::copy_if(books.begin(), books.end(), std::back_inserter(result),
            [threshold](const Book& b) { return b.getCopies() <= threshold; });
        return result;
    }

    std::vector<Book> getTop3MostExpensive() const {
        std::vector<Book> sorted = books;
        std::sort(sorted.begin(), sorted.end(),
            [](const Book& a, const Book& b) { return a.getPrice() > b.getPrice(); });
        if (sorted.size() > 3) sorted.resize(3);
        return sorted;
    }

    const std::vector<Book>& getAllBooks() const { return books; }
    void clear() { books.clear(); }
    bool isEmpty() const { return books.empty(); }
    Book* findBookById(const std::string& id) {
        auto it = std::find_if(books.begin(), books.end(),
            [&id](const Book& b) { return b.getId() == id; });
        return (it != books.end()) ? &(*it) : nullptr;
    }

private:
    std::vector<Book> books;

    bool isIdUnique(const std::string& id) const {
        return std::none_of(books.begin(), books.end(),
            [&id](const Book& b) { return b.getId() == id; });
    }

    static std::string toLower(const std::string& str) {
        std::string result = str;
        std::transform(result.begin(), result.end(), result.begin(),
            [](unsigned char c) { return std::tolower(c); });
        return result;
    }

    static bool containsIgnoreCase(const std::string& str, const std::string& query) {
        if (query.empty()) return true;
        std::string lowerStr = toLower(str);
        std::string lowerQuery = toLower(query);
        return lowerStr.find(lowerQuery) != std::string::npos;
    }
};

#endif