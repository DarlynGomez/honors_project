
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

bool insertTestData() {
    QSqlQuery query;
    
    // Insert test users
    QStringList users = {
        "student1@stu.bmcc.cuny.edu",
        "student2@stu.bmcc.cuny.edu",
        "student3@stu.bmcc.cuny.edu"
    };
    
    for (const QString& email : users) {
        query.prepare("INSERT OR IGNORE INTO users (email, password_hash) VALUES (?, ?)");
        query.addBindValue(email);
        query.addBindValue("testhash123");  // In real app, this would be properly hashed
        
        if (!query.exec()) {
            qDebug() << "Error inserting user" << email << ":" << query.lastError().text();
            return false;
        }
    }
    
    // Insert test products
    struct TestProduct {
        QString name;
        QString description;
        double price;
        QString category;
    };
    
    QList<TestProduct> products = {
        {"Calculus Textbook", "Early Transcendentals, 8th Edition", 89.99, "Textbooks"},
        {"Scientific Calculator", "TI-84 Plus", 119.99, "Electronics"},
        {"Study Desk", "Compact Folding Desk", 79.99, "Furniture"},
        {"BMCC Hoodie", "Blue, Size L", 39.99, "Clothing"},
        {"Notebook Pack", "5-Subject Notebooks", 15.99, "School Supplies"}
    };
    
    for (const TestProduct& product : products) {
        query.prepare("INSERT INTO products (name, description, price, category, seller_id) "
                     "VALUES (?, ?, ?, ?, (SELECT id FROM users LIMIT 1))");
        query.addBindValue(product.name);
        query.addBindValue(product.description);
        query.addBindValue(product.price);
        query.addBindValue(product.category);
        
        if (!query.exec()) {
            qDebug() << "Error inserting product" << product.name << ":" << query.lastError().text();
            return false;
        }
    }
    
    // Verify data
    qDebug() << "\nUsers in database:";
    query.exec("SELECT email FROM users");
    while (query.next()) {
        qDebug() << "- " << query.value("email").toString();
    }
    
    qDebug() << "\nProducts in database:";
    query.exec("SELECT name, price, category FROM products");
    while (query.next()) {
        qDebug() << "- " << query.value("name").toString()
                << "($" << query.value("price").toDouble() << ")"
                << "[" << query.value("category").toString() << "]";
    }
    
    return true;
}

int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);
    
    qDebug() << "Starting database test...";
    
    if (initializeDatabase() && insertTestData()) {
        qDebug() << "Database test completed successfully";
    } else {
        qDebug() << "Database test failed";
        return 1;
    }
    
    return 0;
}




