#ifndef DATABASE_MANAGER_H
#define DATABASE_MANAGER_H

#include <QtCore/QVector>
#include <QtSql/QSqlDatabase>
#include <QDateTime>
#include <QCoreApplication>
#include "textbook.h"

class DatabaseManager {
public:
    DatabaseManager();
    ~DatabaseManager();

    // Add Listing To DataBase Functionality
    bool createTextbookListing(
        const QString& department,
        const QString& lec,
        const QString& courseCategory,
        const QStringList& courseCodes,
        const QString& title,
        const QString& author,
        double price,
        const QString& imagePath
    );
    // Wishlist Functionality
    bool addToWishlist(const QString& userEmail, const QString& productId);
    bool removeFromWishlist(const QString& userEmail, const QString& productId);
    QVector<Textbook> getWishlist(const QString& userEmail);
    
    // Cart Database Functionality
    bool addToCart(const QString& userEmail, const QString& productId, int quantity);
    bool updateCartQuantity(const QString& userEmail, const QString& productId, int quantity);
    bool removeFromCart(const QString& userEmail, const QString& productId);
    QVector<QPair<Textbook, int>> getCart(const QString& userEmail);
    bool initializeDatabase();
    bool addTextbook(const Textbook& textbook);
    QVector<Textbook> getTextbooks(
        const QString& department = "",
        const QString& lec = "",
        const QString& category = "",
        const QString& code = "",
        const QString& title = "",
        int page = 1,
        int itemsPerPage = 9
    );

    // For student profiles and recommendations
    bool updateStudentProfile(const QString& email, const QString& major, const QString& semesterLevel);
    QString getStudentMajor(const QString& email);
    QString getStudentSemesterLevel(const QString& email);
    QVector<Textbook> getRecommendedBooks(const QString& email);
    
private:
    QSqlDatabase db;
    void createTables();
    void populateInitialData();
    void createRecommendationTables();
    void populateRecommendationData();
};

#endif