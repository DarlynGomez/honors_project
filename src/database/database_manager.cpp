#include "database/database_manager.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

DatabaseManager::DatabaseManager() {
    initializeDatabase();
}

DatabaseManager::~DatabaseManager() {
    if (db.isOpen()) {
        db.close();
    }
}

bool DatabaseManager::initializeDatabase() {
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("bmcc_store.db");
    
    if (!db.open()) {
        qDebug() << "Error opening database:" << db.lastError().text();
        return false;
    }
    
    createTables();
    populateInitialData();
    return true;
}

void DatabaseManager::createTables() {
    QSqlQuery query;
    query.exec(
        "CREATE TABLE IF NOT EXISTS textbooks ("
        "product_id TEXT PRIMARY KEY,"
        "department TEXT,"
        "lec TEXT,"
        "course_category TEXT,"
        "course_code TEXT,"
        "title TEXT,"
        "author TEXT,"
        "price REAL,"
        "image_path TEXT)"
    );
}

void DatabaseManager::populateInitialData() {
    QSqlQuery query;
    query.exec("SELECT COUNT(*) FROM textbooks");
    query.next();
    if (query.value(0).toInt() > 0) return;

    // Add the sample data
    QVector<Textbook> initialBooks = {
        Textbook("Computer Science", "1100", "CSC", "101", 
                "Starting Out with C++ from Control Structures to Objects", 
                "", "0001", 110.70, "resources/images/textbooks/cpp_book.jpg"),
                
        Textbook("Computer Science", "1500", "CSC", "211H", 
                "Starting Out with C++ from Control Structures to Objects", 
                "", "0002", 113.70, "resources/images/textbooks/cpp_book.jpg"),
                
        Textbook("Computer Science", "0901", "CSC", "111", 
                "Starting Out with C++ from Control Structures to Objects", 
                "", "0001", 110.70, "resources/images/textbooks/cpp_book.jpg"),
                
        Textbook("Social Sciences, Human Services & Criminal Justice", "A05W", "ECO", "201H",
                "Exploring Macroeconomics", 
                "Robert L. L. Sexton", "0003", 70.25, "resources/images/textbooks/macro_econ.jpg"),
                
        Textbook("Social Sciences, Human Services & Criminal Justice", "A05W", "ECO", "201",
                "Exploring Macroeconomics", 
                "Robert L. L. Sexton", "0004", 70.25, "resources/images/textbooks/macro_econ.jpg"),
                
        Textbook("Social Sciences, Human Services & Criminal Justice", "1700", "ENG", "121",
                "Gilgamesh: A New Translation of the Ancient Epic", 
                "Sophus Helle", "0005", 5.15, "resources/images/textbooks//gilgamesh.jpg"),
                
        Textbook("English", "1700", "ENG", "121", 
                "Frankenstein", 
                "Mary Shelley", "0006", 9.00, "resources/images/textbooks/frankenstein.jpg"),
                
        Textbook("English", "1700", "ENG", "121", 
                "Never Let Me Go", 
                "Kazuo Ishiguro", "0007", 9.00, "resources/images/textbooks/never_let_me_go.jpg"),
                
        Textbook("Science", "1249", "PHY", "215", 
                "Physics for Scientists and Engineers Volume I", 
                "Giancoli Douglas", "0008", 62.00, "resources/images/textbooks/physics.jpg"),
                
        Textbook("Mathematics", "1249", "MAT", "302", 
                "Calculus: Early Transcendental Functions 7th Edition", 
                "Ron Larson", "0009", 182.24, "resources/images/textbooks/calculus.jpg")
    };

    for (const auto& book : initialBooks) {
        addTextbook(book);
    }
}

bool DatabaseManager::addTextbook(const Textbook& textbook) {
    QSqlQuery query;
    query.prepare(
        "INSERT INTO textbooks VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?)"
    );
    query.addBindValue(textbook.productId);
    query.addBindValue(textbook.department);
    query.addBindValue(textbook.lec);
    query.addBindValue(textbook.courseCategory);
    query.addBindValue(textbook.courseCode);
    query.addBindValue(textbook.title);
    query.addBindValue(textbook.author);
    query.addBindValue(textbook.price);
    query.addBindValue(textbook.getImagePath());
    
    return query.exec();
}

QVector<Textbook> DatabaseManager::getTextbooks(
    const QString& department,
    const QString& lec,
    const QString& category,
    const QString& code,
    const QString& title,
    int page,
    int itemsPerPage
) {
    QVector<Textbook> results;
    QSqlQuery query;
    QString queryStr = "SELECT * FROM textbooks WHERE 1=1";
    
    if (!department.isEmpty())
        queryStr += " AND department LIKE '%" + department + "%'";
    if (!lec.isEmpty())
        queryStr += " AND lec = '" + lec + "'";
    if (!category.isEmpty())
        queryStr += " AND course_category = '" + category + "'";
    if (!code.isEmpty())
        queryStr += " AND course_code = '" + code + "'";
    if (!title.isEmpty())
        queryStr += " AND title LIKE '%" + title + "%'";
        
    queryStr += " LIMIT " + QString::number(itemsPerPage);
    queryStr += " OFFSET " + QString::number((page - 1) * itemsPerPage);
    
    query.exec(queryStr);
    
    while (query.next()) {
        results.append(Textbook(
            query.value("department").toString(),
            query.value("lec").toString(),
            query.value("course_category").toString(),
            query.value("course_code").toString(),
            query.value("title").toString(),
            query.value("author").toString(),
            query.value("product_id").toString(),
            query.value("price").toDouble(),
            query.value("image_path").toString()  // Add this line
        ));
    }
    
    return results;
}