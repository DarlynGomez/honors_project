#ifndef DATABASE_MANAGER_H
#define DATABASE_MANAGER_H

#include <QtCore/QVector>
#include <QtSql/QSqlDatabase>
#include "textbook.h"

class DatabaseManager {
public:
    DatabaseManager();
    ~DatabaseManager();
    
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