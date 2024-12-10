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
    
private:
    QSqlDatabase db;
    void createTables();
    void populateInitialData();
};

#endif