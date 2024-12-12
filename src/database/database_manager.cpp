#include "database/database_manager.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QSql>
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
    
    // Create textbooks table
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

    // Create wishlist table
    query.exec(
        "CREATE TABLE IF NOT EXISTS wishlist ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "user_email TEXT,"
        "product_id TEXT,"
        "FOREIGN KEY(product_id) REFERENCES textbooks(product_id),"
        "UNIQUE(user_email, product_id))"
    );

    // Create cart table
    query.exec(
        "CREATE TABLE IF NOT EXISTS cart ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "user_email TEXT,"
        "product_id TEXT,"
        "quantity INTEGER,"
        "FOREIGN KEY(product_id) REFERENCES textbooks(product_id))"
    );

    // Create student profiles table
    query.exec(
        "CREATE TABLE IF NOT EXISTS student_profiles ("
        "email TEXT PRIMARY KEY,"
        "major TEXT,"
        "semester_level TEXT,"
        "UNIQUE(email))"
    );

    // Create semester requirements table
    query.exec(
        "CREATE TABLE IF NOT EXISTS semester_requirements ("
        "requirement_id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "major TEXT,"
        "semester_level TEXT,"
        "course_category TEXT,"
        "course_code TEXT)"
    );
}

// Adds item into cart database after add to cart is clciked
bool DatabaseManager::addToCart(const QString& userEmail, const QString& productId, int quantity) {
    try {
        // Create a QSqlQuery object to use SQL queries
        QSqlQuery query;

        // Prepares my SQL query object to add data in cart table
        query.prepare(
            "INSERT INTO cart (user_email, product_id, quantity) "
            "VALUES (?, ?, ?)"
        );

        // Adds input paramters to queries
        query.addBindValue(userEmail);  // User's email address.
        query.addBindValue(productId);  // Product ID being added to the cart.
        query.addBindValue(quantity);   // Quantity of the product being added.

        // Execute the query, if fails, throw an exception
        if (!query.exec()) {
            // Throws an exception with the error message
            throw std::runtime_error("Error executing query: " + query.lastError().text().toStdString());
        }

        // If no errors, return true indicating success
        qDebug() << "Succesfully added to cart";
        return true;
    } catch (const std::exception& e) {
        // Catch any exceptions and log the error message
        qDebug() << "Exception in addToCart:" << e.what();
        return false;  // Return false to show failure
    }
}

bool DatabaseManager::addToWishlist(const QString& userEmail, const QString& productId) {
    QSqlQuery query;
    query.prepare(
        "INSERT OR IGNORE INTO wishlist (user_email, product_id) "
        "VALUES (?, ?)"
    );
    query.addBindValue(userEmail);
    query.addBindValue(productId);
    return query.exec();
}

bool DatabaseManager::removeFromWishlist(const QString& userEmail, const QString& productId) {
    QSqlQuery query;
    query.prepare(
        "DELETE FROM wishlist WHERE user_email = ? AND product_id = ?"
    );
    query.addBindValue(userEmail);
    query.addBindValue(productId);
    return query.exec();
}

QVector<Textbook> DatabaseManager::getWishlist(const QString& userEmail) {
    QVector<Textbook> wishlistItems;
    QSqlQuery query;
    query.prepare(
        "SELECT t.* FROM wishlist w "
        "JOIN textbooks t ON w.product_id = t.product_id "
        "WHERE w.user_email = ?"
    );
    query.addBindValue(userEmail);
    
    if (query.exec()) {
        while (query.next()) {
            wishlistItems.append(Textbook(
                query.value("department").toString(),
                query.value("lec").toString(),
                query.value("course_category").toString(),
                query.value("course_code").toString(),
                query.value("title").toString(),
                query.value("author").toString(),
                query.value("product_id").toString(),
                query.value("price").toDouble(),
                query.value("image_path").toString()
            ));
        }
    }
    
    return wishlistItems;
}


// Adds textbook Listing onto DB
bool DatabaseManager::createTextbookListing(
    const QString& department,
    const QString& lec,
    const QString& courseCategory,
    const QStringList& courseCodes,
    const QString& title,
    const QString& author,
    double price,
    const QString& imagePath
) {
    // Generate unique product ID using timestamp
    QString productId = QString::number(QDateTime::currentSecsSinceEpoch());
    
    QSqlQuery query;
    query.prepare(
        "INSERT INTO textbooks "
        "(product_id, department, lec, course_category, course_code, title, author, price, image_path) "
        "VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?)"
    );
    
    query.addBindValue(productId);
    query.addBindValue(department);
    query.addBindValue(lec);
    query.addBindValue(courseCategory);
    query.addBindValue(courseCodes.join(","));  // Store multiple codes as comma-separated
    query.addBindValue(title);
    query.addBindValue(author);
    query.addBindValue(price);
    query.addBindValue(imagePath);
    
    bool success = query.exec();
    if (!success) {
        qDebug() << "Failed to create listing:" << query.lastError().text();
    }
    
    return success;
}


// Change number of items of a particular item
bool DatabaseManager::updateCartQuantity(const QString& userEmail, const QString& productId, int quantity) {
    QSqlQuery query;
    query.prepare(
        "UPDATE cart SET quantity = ? "
        "WHERE user_email = ? AND product_id = ?"
    );
    query.addBindValue(quantity);
    query.addBindValue(userEmail);
    query.addBindValue(productId);
    return query.exec();
}

// Removes item from cart database
bool DatabaseManager::removeFromCart(const QString& userEmail, const QString& productId) {
    QSqlQuery query;
    query.prepare(
        "DELETE FROM cart WHERE user_email = ? AND product_id = ?"
    );
    query.addBindValue(userEmail);
    query.addBindValue(productId);
    return query.exec();
}

// Gets cart to display it in cart listing
QVector<QPair<Textbook, int>> DatabaseManager::getCart(const QString& userEmail) {
    QVector<QPair<Textbook, int>> cartItems;
    QSqlQuery query;
    query.prepare(
        "SELECT t.*, c.quantity FROM cart c "
        "JOIN textbooks t ON c.product_id = t.product_id "
        "WHERE c.user_email = ?"
    );
    query.addBindValue(userEmail);
    
    if (query.exec()) {
        while (query.next()) {
            Textbook book(
                query.value("department").toString(),
                query.value("lec").toString(),
                query.value("course_category").toString(),
                query.value("course_code").toString(),
                query.value("title").toString(),
                query.value("author").toString(),
                query.value("product_id").toString(),
                query.value("price").toDouble(),
                query.value("image_path").toString()
            );
            cartItems.append({book, query.value("quantity").toInt()});
        }
    }
    
    return cartItems;
}




// Profile Data base


void DatabaseManager::createRecommendationTables() {
    QSqlQuery query;
    
    // Create semester requirements table
    query.exec(
        "CREATE TABLE IF NOT EXISTS semester_requirements ("
        "requirement_id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "major TEXT,"
        "semester_level TEXT,"
        "course_category TEXT,"
        "course_code TEXT)"
    );

    // Create student profiles table
    query.exec(
        "CREATE TABLE IF NOT EXISTS student_profiles ("
        "email TEXT PRIMARY KEY,"
        "major TEXT,"
        "semester_level TEXT)"
    );
}

void DatabaseManager::populateRecommendationData() {
    QSqlQuery query;
    query.exec("SELECT COUNT(*) FROM semester_requirements");
    query.next();
    
    // Always repopulate for testing
    query.exec("DELETE FROM semester_requirements");
    
    qDebug() << "Populating recommendation data...";
    
    // Computer Science - Lower Freshman Requirements
    struct CourseRequirement {
        QString category;
        QString code;
    };
    
    QVector<CourseRequirement> csLowerFreshmanCourses = {
        {"CSC", "101"},  // Core CS course
        {"ENG", "121"}   // English requirement
    };

    // Add requirements
    for (const auto& course : csLowerFreshmanCourses) {
        QSqlQuery insertQuery;
        insertQuery.prepare(
            "INSERT INTO semester_requirements (major, semester_level, course_category, course_code) "
            "VALUES (?, ?, ?, ?)"
        );
        insertQuery.addBindValue("Computer Science");
        insertQuery.addBindValue("Lower Freshman");
        insertQuery.addBindValue(course.category);
        insertQuery.addBindValue(course.code);
        
        if (!insertQuery.exec()) {
            qDebug() << "Failed to insert requirement:" << insertQuery.lastError().text();
        } else {
            qDebug() << "Added requirement:" << course.category << course.code;
        }
    }
}


bool DatabaseManager::updateStudentProfile(const QString& email, const QString& major, const QString& semesterLevel) {
    qDebug() << "Updating profile for:" << email 
             << "Major:" << major 
             << "Semester:" << semesterLevel;
             
    QSqlQuery query;
    query.prepare(
        "INSERT OR REPLACE INTO student_profiles (email, major, semester_level) "
        "VALUES (?, ?, ?)"
    );
    query.addBindValue(email);
    query.addBindValue(major);
    query.addBindValue(semesterLevel);
    
    bool success = query.exec();
    if (!success) {
        qDebug() << "Profile update failed:" << query.lastError().text();
    } else {
        qDebug() << "Profile updated successfully";
    }
    
    return success;
}

QString DatabaseManager::getStudentMajor(const QString& email) {
    QSqlQuery query;
    query.prepare("SELECT major FROM student_profiles WHERE email = ?");
    query.addBindValue(email);
    
    if (query.exec() && query.next()) {
        return query.value(0).toString();
    }
    
    return QString();
}

QString DatabaseManager::getStudentSemesterLevel(const QString& email) {
    QSqlQuery query;
    query.prepare("SELECT semester_level FROM student_profiles WHERE email = ?");
    query.addBindValue(email);
    
    if (query.exec() && query.next()) {
        return query.value(0).toString();
    }
    
    return QString();
}

QVector<Textbook> DatabaseManager::getRecommendedBooks(const QString& email) {
    QVector<Textbook> recommendations;
    
    qDebug() << "Getting recommendations for:" << email;
    
    // Define the asset path (adjust as needed)
    QString assetPath = QCoreApplication::applicationDirPath() + "/../assets/images/textbooks/";

    // Get student's major and semester level
    QString major = getStudentMajor(email);
    QString semester = getStudentSemesterLevel(email);
    
    qDebug() << "Student profile - Major:" << major << "Semester:" << semester;
    
    if (major.isEmpty() || semester.isEmpty()) {
        qDebug() << "Missing profile information";
        return recommendations;
    }

    QSqlQuery query;
    query.prepare(
        "SELECT DISTINCT t.* FROM textbooks t "
        "JOIN semester_requirements r ON "
        "t.course_category = r.course_category AND t.course_code = r.course_code "
        "WHERE r.major = ? AND r.semester_level = ?"
    );
    query.addBindValue(major);
    query.addBindValue(semester);
    
    if (!query.exec()) {
        qDebug() << "Failed to execute recommendations query:" << query.lastError().text();
        return recommendations;
    }
    
    // Debug print all textbooks that match
    while (query.next()) {
        qDebug() << "Found matching book:" 
                 << query.value("title").toString()
                 << "for course" 
                 << query.value("course_category").toString()
                 << query.value("course_code").toString();
                 
        recommendations.append(Textbook(
            query.value("department").toString(),
            query.value("lec").toString(),
            query.value("course_category").toString(),
            query.value("course_code").toString(),
            query.value("title").toString(),
            query.value("author").toString(),
            query.value("product_id").toString(),
            query.value("price").toDouble(),
            query.value("image_path").toString()
        ));
    }
    
    qDebug() << "Total recommendations found:" << recommendations.size();
    // If no books are found, add default books
    if (recommendations.isEmpty()) {
        // Adding default recommended books when recommendations are empty
        recommendations.append(Textbook(
            "CSC",                          // dept (Department)
            "101",                          // lecCode (Lecture Code)
            "CSC",                          // courseCategory (Course Category)
            "101",                          // courseCode (Course Code)
            "Starting Out with C++ from Control Structures to Objects", // title
            "Tony Gaddis",                  // author
            "1",                            // productId
            110.70,                         // price
            assetPath + "cpp_book.jpg"      // imagePath
        ));

        recommendations.append(Textbook(
            "ENG",                          // dept (Department)
            "121",                          // lecCode (Lecture Code)
            "ENG",                          // courseCategory (Course Category)
            "121",                          // courseCode (Course Code)
            "The Epic of Gilgamesh",         // title
            "Anonymous",                    // author
            "1",                            // productId
            5.99,                          // price
            assetPath + "gilgamesh.jpg" // imagePath
        ));

        recommendations.append(Textbook(
            "ENG",                          // dept (Department)
            "121",                          // lecCode (Lecture Code)
            "ENG",                          // courseCategory (Course Category)
            "121",                          // courseCode (Course Code)
            "Frankenstein",                  // title
            "Mary Shelley",                  // author
            "2",                             // productId
            9.50,                           // price
            assetPath + "frankenstein.jpg" // imagePath
        ));

        recommendations.append(Textbook(
            "ENG",                          // dept (Department)
            "121",                          // lecCode (Lecture Code)
            "ENG",                          // courseCategory (Course Category)
            "121",                          // courseCode (Course Code)
            "Enverlet Me Go",               // title
            "Jane Doe",                     // author
            "3",                             // productId
            7.75,                           // price
            assetPath + "never_let_me_go.jpg" // imagePath
        ));
    }
    return recommendations;
}







void DatabaseManager::populateInitialData() {
    QSqlQuery query;
    query.exec("SELECT COUNT(*) FROM textbooks");
    query.next();
    if (query.value(0).toInt() > 0) return;

    // Get the application directory path
    QString assetPath = QCoreApplication::applicationDirPath() + "/../assets/images/textbooks/";

    // Required textbooks for Computer Science Lower Freshman
    QVector<Textbook> initialBooks = {
        // CSC 101 Textbook
        Textbook("Computer Science", "1100", "CSC", "101", 
                "Starting Out with C++ from Control Structures to Objects", 
                "Tony Gaddis", "0001", 110.70, assetPath + "cpp_book.jpg"),
                
        // ENG 121 Required Reading
        Textbook("English", "1700", "ENG", "121", 
                "Gilgamesh: A New Translation of the Ancient Epic", 
                "Sophus Helle", "0005", 5.15, assetPath + "gilgamesh.jpg"),
                
        Textbook("English", "1700", "ENG", "121", 
                "Frankenstein", 
                "Mary Shelley", "0006", 9.00, assetPath + "frankenstein.jpg"),
                
        Textbook("English", "1700", "ENG", "121", 
                "Never Let Me Go", 
                "Kazuo Ishiguro", "0007", 9.00, assetPath + "never_let_me_go.jpg"),
                
        Textbook("Computer Science", "1500", "CSC", "211H", 
                "Starting Out with C++ from Control Structures to Objects", 
                "", "0002", 113.70, assetPath + "cpp_book.jpg"),
                
        Textbook("Computer Science", "0901", "CSC", "111", 
                "Starting Out with C++ from Control Structures to Objects", 
                "", "0001", 110.70, assetPath + "cpp_book.jpg"),
                
        Textbook("Social Sciences, Human Services & Criminal Justice", "A05W", "ECO", "201H",
                "Exploring Macroeconomics", 
                "Robert L. L. Sexton", "0003", 70.25, assetPath + "macro_econ.jpg"),
                
        Textbook("Social Sciences, Human Services & Criminal Justice", "A05W", "ECO", "201",
                "Exploring Macroeconomics", 
                "Robert L. L. Sexton", "0004", 70.25, assetPath + "macro_econ.jpg"),
                
        Textbook("Science", "1249", "PHY", "215", 
                "Physics for Scientists and Engineers Volume I", 
                "Giancoli Douglas", "0008", 62.00, assetPath + "physics.jpg"),
                
        Textbook("Mathematics", "1249", "MAT", "302", 
                "Calculus: Early Transcendental Functions 7th Edition", 
                "Ron Larson", "0009", 182.24, assetPath + "calculus.jpg")
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