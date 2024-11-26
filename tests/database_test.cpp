
#include <QCoreApplication>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QDir>

bool initializeDatabase() {
    qDebug() << "Current directory:" << QDir::currentPath();
    
    // Create and open database
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("estore.db");
    
    if (!db.open()) {
        qDebug() << "Error: connection with database failed";
        qDebug() << "Error details:" << db.lastError().text();
        return false;
    }
    
    qDebug() << "Connected to database successfully";
    
    // Create tables
    QSqlQuery query;
    
    // Users table
    if (!query.exec("CREATE TABLE IF NOT EXISTS users ("
                   "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                   "email TEXT UNIQUE NOT NULL,"
                   "password_hash TEXT NOT NULL,"
                   "created_at DATETIME DEFAULT CURRENT_TIMESTAMP)")) {
        qDebug() << "Error creating users table:" << query.lastError().text();
        return false;
    }
    
    qDebug() << "Users table created successfully";
    
    // Products table
    if (!query.exec("CREATE TABLE IF NOT EXISTS products ("
                   "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                   "name TEXT NOT NULL,"
                   "description TEXT,"
                   "price DECIMAL(10,2) NOT NULL,"
                   "category TEXT NOT NULL,"
                   "seller_id INTEGER,"
                   "created_at DATETIME DEFAULT CURRENT_TIMESTAMP,"
                   "FOREIGN KEY(seller_id) REFERENCES users(id))")) {
        qDebug() << "Error creating products table:" << query.lastError().text();
        return false;
    }
    
    qDebug() << "Products table created successfully";
    
    // Test insert
    if (!query.exec("INSERT OR IGNORE INTO users (email, password_hash) "
                   "VALUES ('test@stu.bmcc.cuny.edu', 'testhash')")) {
        qDebug() << "Error inserting test user:" << query.lastError().text();
        return false;
    }
    
    qDebug() << "Test user inserted successfully";
    
    // Test select
    if (!query.exec("SELECT * FROM users")) {
        qDebug() << "Error querying users:" << query.lastError().text();
        return false;
    }
    
    while (query.next()) {
        qDebug() << "Found user:" << query.value("email").toString();
    }
    
    qDebug() << "Database initialized successfully!";
    return true;
}

int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);
    
    qDebug() << "Starting database test...";
    
    if (initializeDatabase()) {
        qDebug() << "Database test completed successfully";
    } else {
        qDebug() << "Database test failed";
        return 1;
    }
    
    return 0;
}




