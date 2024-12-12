#include "ui/profile_page.h"
#include "ui/mainshop_window.h"
#include <QScrollArea>
#include <QGridLayout>
#include <QFrame>
#include <QGraphicsDropShadowEffect>
#include <QFileDialog>
#include <QRegularExpression>
#include <QRegularExpressionValidator>
#include <QAbstractItemView>
#include <QListWidget>

ProfilePage::ProfilePage(Authenticator* auth, DatabaseManager* db, const QString& email, QWidget* parent)
    : QWidget(parent), authenticator(auth), dbManager(db), userEmail(email)
{
    extractNameFromEmail();
    setupUI();
    loadUserProfile();
}

void ProfilePage::setupUI() {
    // Main layout
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setAlignment(Qt::AlignCenter);
    
    // Create central card widget
    QWidget* cardWidget = new QWidget;
    cardWidget->setFixedWidth(1000);
    cardWidget->setStyleSheet(
        "QWidget {"
        "    background-color: white;"
        "    border-radius: 20px;"
        "    padding: 20px;"
        "}"
    );
    
    // Add shadow effect
    QGraphicsDropShadowEffect* shadow = new QGraphicsDropShadowEffect;
    shadow->setBlurRadius(20);
    shadow->setColor(QColor(0, 0, 0, 50));
    shadow->setOffset(0, 5);
    cardWidget->setGraphicsEffect(shadow);
    
    // Card layout
    QVBoxLayout* cardLayout = new QVBoxLayout(cardWidget);
    cardLayout->setSpacing(30);
    cardLayout->setContentsMargins(40, 40, 40, 40);

    // Header section
    QWidget* headerWidget = new QWidget;
    QVBoxLayout* headerLayout = new QVBoxLayout(headerWidget);
    headerLayout->setSpacing(10);

    nameLabel = new QLabel(firstName + " " + lastName);
    nameLabel->setStyleSheet(
        "font-size: 32px;"
        "font-weight: bold;"
        "color: " + darkBlue + ";"
    );

    emailLabel = new QLabel(userEmail);
    emailLabel->setStyleSheet(
        "font-size: 16px;"
        "color: #666;"
    );

    headerLayout->addWidget(nameLabel, 0, Qt::AlignCenter);
    headerLayout->addWidget(emailLabel, 0, Qt::AlignCenter);
    cardLayout->addWidget(headerWidget);

    // Navigation tabs
    QWidget* navWidget = new QWidget;
    QHBoxLayout* navLayout = new QHBoxLayout(navWidget);
    navLayout->setSpacing(20);
    navLayout->setAlignment(Qt::AlignCenter);

    QStringList sections = {"Edit Profile", "Your Listings", "Order History"};
    for (int i = 0; i < sections.size(); ++i) {
        QPushButton* btn = new QPushButton(sections[i]);
        btn->setStyleSheet(
            "QPushButton {"
            "    background-color: white;"
            "    color: " + darkBlue + ";"
            "    border: 2px solid " + sageGreen + ";"
            "    padding: 12px 30px;"
            "    border-radius: 25px;"
            "    font-size: 14px;"
            "    min-width: 150px;"
            "    font-weight: bold;"
            "}"
            "QPushButton:hover {"
            "    background-color: " + sageGreen + ";"
            "    color: white;"
            "}"
        );
        connect(btn, &QPushButton::clicked, this, [this, i]() { switchSection(i); });
        navLayout->addWidget(btn);
    }
    cardLayout->addWidget(navWidget);

    // Content stack
    contentStack = new QStackedWidget;
    contentStack->addWidget(createProfileSection());
    contentStack->addWidget(createListingsSection());
    contentStack->addWidget(createOrderHistorySection());
    cardLayout->addWidget(contentStack);

    mainLayout->addWidget(cardWidget);
}

QScrollArea* ProfilePage::createStyledScrollArea() {
    QScrollArea* scrollArea = new QScrollArea;
    scrollArea->setWidgetResizable(true);
    scrollArea->setFrameShape(QFrame::NoFrame);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    scrollArea->setStyleSheet(
        "QScrollArea {"
        "    border: none;"
        "}"
        "QScrollBar:vertical {"
        "    border: none;"
        "    background: #F0F0F0;"
        "    width: 8px;"
        "    margin: 0px;"
        "}"
        "QScrollBar::handle:vertical {"
        "    background: " + sageGreen + ";"
        "    border-radius: 4px;"
        "    min-height: 20px;"
        "}"
        "QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {"
        "    border: none;"
        "    background: none;"
        "}"
    );
    return scrollArea;
}

QWidget* ProfilePage::createProfileSection() {
    QScrollArea* scrollArea = createStyledScrollArea();
    QWidget* contentWidget = new QWidget;
    QVBoxLayout* layout = new QVBoxLayout(contentWidget);
    layout->setSpacing(20);
    layout->setContentsMargins(0, 0, 20, 0);

    QLabel* title = new QLabel("Edit Profile Information");
    title->setStyleSheet(
        "font-size: 24px;"
        "font-weight: bold;"
        "color: " + darkBlue + ";"
        "margin-bottom: 20px;"
    );
    layout->addWidget(title);

    // Major selection
    QLabel* majorLabel = new QLabel("Major:");
    majorLabel->setStyleSheet("font-weight: bold; color: " + darkBlue + ";");
    majorCombo = new QComboBox;
    majorCombo->addItems({
        "Computer Science", 
        "Computer Information Systems", 
        "Liberal Arts", 
        "Business Administration"
    });
    majorCombo->setStyleSheet(
        "QComboBox {"
        "    padding: 8px;"
        "    border: 2px solid #E0E0E0;"
        "    border-radius: 8px;"
        "    background: white;"
        "}"
        "QComboBox:hover {"
        "    border-color: " + sageGreen + ";"
        "}"
    );
    layout->addWidget(majorLabel);
    layout->addWidget(majorCombo);

    // Semester selection
    QLabel* semesterLabel = new QLabel("Semester Status:");
    semesterLabel->setStyleSheet("font-weight: bold; color: " + darkBlue + ";");
    semesterCombo = new QComboBox;
    semesterCombo->addItems({
        "Lower Freshman", 
        "Upper Freshman", 
        "Lower Sophomore", 
        "Upper Sophomore"
    });
    semesterCombo->setStyleSheet(majorCombo->styleSheet());
    layout->addWidget(semesterLabel);
    layout->addWidget(semesterCombo);

    QPushButton* saveButton = createStyledButton("Save Changes", true);
    connect(saveButton, &QPushButton::clicked, this, &ProfilePage::saveMajorAndSemester);
    layout->addWidget(saveButton, 0, Qt::AlignLeft);

    layout->addStretch();
    scrollArea->setWidget(contentWidget);
    return scrollArea;
}

QWidget* ProfilePage::createListingsSection() {
    QScrollArea* scrollArea = createStyledScrollArea();
    QWidget* contentWidget = new QWidget;
    QVBoxLayout* layout = new QVBoxLayout(contentWidget);
    layout->setSpacing(30);
    layout->setContentsMargins(0, 0, 20, 0);

    // Header with title and create button
    QWidget* headerWidget = new QWidget;
    QHBoxLayout* headerLayout = new QHBoxLayout(headerWidget);
    
    QLabel* title = new QLabel("Your Listings");
    title->setStyleSheet(
        "font-size: 24px;"
        "font-weight: bold;"
        "color: " + darkBlue + ";"
    );
    
    QPushButton* createButton = createStyledButton("+ Create New Listing", true);
    connect(createButton, &QPushButton::clicked, this, &ProfilePage::handleCreateListing);
    
    headerLayout->addWidget(title);
    headerLayout->addStretch();
    headerLayout->addWidget(createButton);
    layout->addWidget(headerWidget);


    // Grid for listings
    QWidget* gridContainer = new QWidget;
    listingsGrid = new QGridLayout(gridContainer);  // Use the member variable
    listingsGrid->setSpacing(20);
    listingsGrid->setContentsMargins(0, 0, 0, 0);

    // Sample listing cards
    for (int i = 0; i < 6; i++) {
        QWidget* card = createListingCard();
        listingsGrid->addWidget(card, i / 2, i % 2);  // Use the member variable
    }



    layout->addWidget(gridContainer);
    layout->addStretch();
    
    scrollArea->setWidget(contentWidget);
    return scrollArea;
}



QPushButton* ProfilePage::createStyledButton(const QString& text, bool isPrimary) {
    QPushButton* button = new QPushButton(text);
    button->setStyleSheet(
        QString("QPushButton {"
        "    background-color: %1;"
        "    color: %2;"
        "    padding: 12px 25px;"
        "    border-radius: 20px;"
        "    font-size: 14px;"
        "    font-weight: bold;"
        "    border: none;"
        "}"
        "QPushButton:hover {"
        "    background-color: %3;"
        "    color: white;"
        "}").arg(
            isPrimary ? sageGreen : "white",
            isPrimary ? "white" : darkBlue,
            darkBlue
        )
    );
    return button;
}

QWidget* ProfilePage::createListingCard(const QString& title, double price, 
                                      const QString& status, const QString& imagePath) {
    QWidget* card = new QWidget;
    card->setFixedSize(300, 350);
    card->setStyleSheet(
        "QWidget {"
        "    background-color: white;"
        "    border: 1px solid #E0E0E0;"
        "    border-radius: 15px;"
        "}"
    );

    QVBoxLayout* cardLayout = new QVBoxLayout(card);
    cardLayout->setSpacing(10);
    cardLayout->setContentsMargins(15, 15, 15, 15);

    // Image
    QLabel* imageLabel = new QLabel;
    imageLabel->setFixedSize(250, 200);
    imageLabel->setStyleSheet(
        "background-color: " + lightSage + ";"
        "border-radius: 10px;"
    );
    
    if (!imagePath.isEmpty()) {
        QPixmap pixmap(imagePath);
        imageLabel->setPixmap(pixmap.scaled(250, 200, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    } else {
        imageLabel->setAlignment(Qt::AlignCenter);
        imageLabel->setText("No Image");
    }

    // Title
    QLabel* titleLabel = new QLabel(title);
    titleLabel->setStyleSheet(
        "font-weight: bold;"
        "font-size: 16px;"
        "color: " + darkBlue + ";"
    );

    // Price
    QLabel* priceLabel = new QLabel(QString("$%1").arg(price, 0, 'f', 2));
    priceLabel->setStyleSheet(
        "color: " + sageGreen + ";"
        "font-size: 18px;"
        "font-weight: bold;"
    );

    // Status
    QLabel* statusLabel = new QLabel(status);
    statusLabel->setStyleSheet(
        "color: #4CAF50;"
        "font-size: 14px;"
    );

    cardLayout->addWidget(imageLabel);
    cardLayout->addWidget(titleLabel);
    cardLayout->addWidget(priceLabel);
    cardLayout->addWidget(statusLabel);

    // Add shadow effect
    QGraphicsDropShadowEffect* shadow = new QGraphicsDropShadowEffect;
    shadow->setBlurRadius(15);
    shadow->setColor(QColor(0, 0, 0, 30));
    shadow->setOffset(0, 2);
    card->setGraphicsEffect(shadow);

    return card;
}

void ProfilePage::showCreateListingDialog() {
    QDialog* dialog = new QDialog(this);
    dialog->setWindowTitle("Create New Listing");
    dialog->setFixedSize(600, 750);  // Set a fixed size that fits the screen
    dialog->setStyleSheet(
        "QDialog {"
        "    background-color: white;"
        "    border-radius: 15px;"
        "}"
        "QLabel {"
        "    color: " + darkBlue + ";"
        "    font-weight: bold;"
        "    margin-top: 5px;"  // Reduced from 10px
        "}"
        "QLineEdit, QComboBox {"
        "    padding: 8px;"     // Reduced from 10px
        "    border: 2px solid #E0E0E0;"
        "    border-radius: 8px;"
        "    font-size: 14px;"
        "    background: white;"
        "    min-width: 200px;"
        "}"
        "QLineEdit:focus, QComboBox:focus {"
        "    border-color: " + sageGreen + ";"
        "}"
        "QPushButton {"
        "    padding: 8px 20px;"  // Reduced padding
        "    border-radius: 20px;"
        "    font-size: 14px;"
        "    font-weight: bold;"
        "    min-width: 120px;"
        "}"
    );

    // Make layout more compact
    QVBoxLayout* layout = new QVBoxLayout(dialog);
    layout->setSpacing(10);  // Reduced from 20
    layout->setContentsMargins(20, 20, 20, 20);  // Reduced from 30

    // Department Selection
    QLabel* deptLabel = new QLabel("Department:", dialog);
    QComboBox* deptCombo = new QComboBox(dialog);
    deptCombo->addItems({
        "Academic Literacy and Linguistics",
        "Accounting",
        "Allied Health Sciences",
        "Business Management",
        "Computer Information Systems",
        "Computer Science",
        "English",
        "Ethnic and Race Studies",
        "Health Studies",
        "Library",
        "Mathematics",
        "Media Arts and Technology",
        "Modern Languages",
        "Music and Art",
        "Nursing",
        "Science",
        "Social Sciences, Human Services and Criminal Justice",
        "Speech, Communications and Theatre Arts",
        "Student Life",
        "Teacher Education"
    });

    // Course Section
    QLabel* sectionLabel = new QLabel("Course Section:", dialog);
    QComboBox* sectionCombo = new QComboBox;
    sectionCombo->addItems({
        "ACC", "AFL", "AFN", "ANT", "ARC", "ART", "ASL", "ASN", "BIO", 
        "BTE", "BUS", "CHE", "CHI", "CIS", "CRJ", "CSC", "ECO", "ECE", 
        "ENG", "ESL", "FNB", "FRN", "GEO", "HED", "HIS", "HSC", "ITL", 
        "LIN", "MAT", "MES", "MUS", "NUR", "PHY", "POL", "PSY", "RDG", 
        "SCI", "SOC", "SPN", "SPE", "THE", "VAT"
    });

    // Book Title
    QLabel* titleLabel = new QLabel("Book Title:", dialog);
    QLineEdit* titleInput = new QLineEdit(dialog);
    titleInput->setPlaceholderText("Enter book title");

    // Course Number
    QLabel* courseLabel = new QLabel("Course Number:", dialog);
    QLineEdit* courseInput = new QLineEdit(dialog);
    courseInput->setPlaceholderText("e.g., 101, 201, etc.");

    // LEC Code
    QLabel* lecLabel = new QLabel("LEC Code:", dialog);
    QLineEdit* lecInput = new QLineEdit(dialog);
    lecInput->setPlaceholderText("e.g., 1234");

    // Price
    QLabel* priceLabel = new QLabel("Price ($):", dialog);
    QLineEdit* priceInput = new QLineEdit(dialog);
    priceInput->setPlaceholderText("0.00");
    QDoubleValidator* validator = new QDoubleValidator(0, 9999.99, 2, priceInput);
    priceInput->setValidator(validator);

    // Image Upload
    QLabel* imageLabel = new QLabel("Book Image:", dialog);
    QPushButton* imageButton = new QPushButton("Upload Image", dialog);
    imageButton->setStyleSheet(
        "background-color: " + sageGreen + ";"
        "color: white;"
    );
    QLabel* imagePreview = new QLabel(dialog);
    imagePreview->setFixedSize(80, 100); 
    imagePreview->setStyleSheet(
        "border: 2px dashed #E0E0E0;"
        "border-radius: 10px;"
    );
    imagePreview->setAlignment(Qt::AlignCenter);

    QString imagePath;
    connect(imageButton, &QPushButton::clicked, [&]() {
        QString fileName = QFileDialog::getOpenFileName(dialog,
            "Select Image", "", "Image Files (*.png *.jpg *.jpeg)");
        if (!fileName.isEmpty()) {
            QPixmap pixmap(fileName);
            imagePreview->setPixmap(pixmap.scaled(80, 100, Qt::KeepAspectRatio, Qt::SmoothTransformation));
            imagePath = fileName;
        }
    });

    // Buttons
    QHBoxLayout* buttonLayout = new QHBoxLayout;
    QPushButton* cancelButton = new QPushButton("Cancel", dialog);
    QPushButton* createButton = new QPushButton("Create Listing", dialog);
    
    cancelButton->setStyleSheet(
        "background-color: white;"
        "color: " + darkBlue + ";"
        "border: 2px solid " + sageGreen + ";"
    );
    
    createButton->setStyleSheet(
        "background-color: " + sageGreen + ";"
        "color: white;"
    );

    buttonLayout->addWidget(cancelButton);
    buttonLayout->addWidget(createButton);

    // Add everything to layout
    layout->addWidget(deptLabel);
    layout->addWidget(deptCombo);
    layout->addWidget(sectionLabel);
    layout->addWidget(sectionCombo);
    layout->addWidget(titleLabel);
    layout->addWidget(titleInput);
    layout->addWidget(courseLabel);
    layout->addWidget(courseInput);
    layout->addWidget(lecLabel);
    layout->addWidget(lecInput);
    layout->addWidget(priceLabel);
    layout->addWidget(priceInput);
    layout->addWidget(imageLabel);
    layout->addWidget(imageButton);
    layout->addWidget(imagePreview);
    layout->addLayout(buttonLayout);

    // Connect buttons
    connect(cancelButton, &QPushButton::clicked, dialog, &QDialog::reject);
    connect(createButton, &QPushButton::clicked, [=]() {
        if (titleInput->text().isEmpty() || courseInput->text().isEmpty() || 
            priceInput->text().isEmpty()) {
            QMessageBox::warning(dialog, "Validation Error", 
                "Please fill in all required fields.");
            return;
        }

        QString destPath;
        if (!imagePath.isEmpty()) {
            // Create assets directory if it doesn't exist
            QString assetDir = QCoreApplication::applicationDirPath() + "/../assets/images/textbooks/";
            QDir().mkpath(assetDir);
            
            // Copy image to assets with unique filename to avoid overwriting
            QString uniqueFileName = QString::number(QDateTime::currentSecsSinceEpoch()) + "_" + QFileInfo(imagePath).fileName();
            destPath = assetDir + uniqueFileName;
            
            // Remove existing file if it exists
            if (QFile::exists(destPath)) {
                QFile::remove(destPath);
            }
            
            // Copy the new file
            if (!QFile::copy(imagePath, destPath)) {
                qDebug() << "Failed to copy image file to:" << destPath;
                destPath = QCoreApplication::applicationDirPath() + "/../assets/images/textbooks/default_book.jpg";
            }
        } else {
            // Set default image path if no image was selected
            destPath = QCoreApplication::applicationDirPath() + "/../assets/images/textbooks/default_book.jpg";
        }

        qDebug() << "Image path being saved:" << destPath;  // Debug line

        // Add this part - actually create the listing
        if (dbManager->createTextbookListing(
            deptCombo->currentText(),
            lecInput->text(),
            sectionCombo->currentText(),
            {courseInput->text()},
            titleInput->text(),
            "", // Author can be added later
            priceInput->text().toDouble(),
            destPath
        )) {
            QMessageBox::information(dialog, "Success", 
                "Listing created successfully!");
            refreshListings();  // Refresh after successful creation
            dialog->accept();
        } else {
            QMessageBox::warning(dialog, "Error", 
                "Failed to create listing. Please try again.");
        }
    });

    // Show dialog
    dialog->exec();
}



QString ProfilePage::handleImageUpload() {
    QString fileName = QFileDialog::getOpenFileName(this,
        "Select Image", "", "Image Files (*.png *.jpg *.jpeg)");
    
    if (!fileName.isEmpty()) {
        // Copy file to application's storage
        // This will be implemented when we add file storage support
    }
    
    return fileName;
}

bool ProfilePage::validateListingForm(const QString& title, const QString& price, const QString& description) {
    if (title.isEmpty()) {
        QMessageBox::warning(this, "Validation Error", "Please enter a title for your listing.");
        return false;
    }
    
    if (price.isEmpty() || price.toDouble() <= 0) {
        QMessageBox::warning(this, "Validation Error", "Please enter a valid price.");
        return false;
    }
    
    if (description.isEmpty()) {
        QMessageBox::warning(this, "Validation Error", "Please provide a description for your listing.");
        return false;
    }
    
    return true;
}

void ProfilePage::refreshListings() {
    // Clear existing grid
    QLayoutItem* child;
    while ((child = listingsGrid->takeAt(0)) != nullptr) {
        delete child->widget();
        delete child;
    }

    // Get listings for current user from database
    auto listings = dbManager->getTextbooks("", "", "", "", "", 1, 100); // Get all listings
    
    // Add new listing cards to grid
    int row = 0, col = 0;
    for (const auto& book : listings) {
        QWidget* card = createListingCard(
            book.title, 
            book.price, 
            "Active", // You might want to add status to your Textbook class
            book.getImagePath()
        );
        listingsGrid->addWidget(card, row, col);
        
        col++;
        if (col >= 2) {  // 2 cards per row
            col = 0;
            row++;
        }
    }
}

void ProfilePage::extractNameFromEmail() {
    QString namePart = userEmail.split('@').first();
    QStringList parts = namePart.split('.');
    
    if (parts.size() >= 2) {
        QString lastName = parts[1];
        lastName.remove(QRegularExpression("\\d+"));
        
        firstName = parts[0].at(0).toUpper() + parts[0].mid(1);
        lastName = lastName.at(0).toUpper() + lastName.mid(1);
    }
}



void ProfilePage::loadUserProfile() {
    // Load existing profile data if any
    QString major = dbManager->getStudentMajor(userEmail);
    QString semester = dbManager->getStudentSemesterLevel(userEmail);
    
    if (!major.isEmpty()) {
        int majorIndex = majorCombo->findText(major);
        if (majorIndex >= 0) {
            majorCombo->setCurrentIndex(majorIndex);
        }
    }
    
    if (!semester.isEmpty()) {
        int semesterIndex = semesterCombo->findText(semester);
        if (semesterIndex >= 0) {
            semesterCombo->setCurrentIndex(semesterIndex);
        }
    }
}

QWidget* ProfilePage::createOrderHistorySection() {
    QWidget* orderHistoryWidget = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(orderHistoryWidget);

    // Add order items to the layout
    QWidget* order1 = createOrderItem("CSC 101: Introduction to Computer Science", 59.99, "Shipped", "path_to_image_of_CSC101_book.jpg");
    layout->addWidget(order1);

    QWidget* order2 = createOrderItem("Gilgamesh", 14.99, "Delivered", "path_to_image_of_Gilgamesh.jpg");
    layout->addWidget(order2);

    // You can add more orders as needed

    return orderHistoryWidget;
}

QWidget* ProfilePage::createOrderItem(const QString& title, double price, const QString& status, const QString& imagePath) {
    QWidget* orderWidget = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(orderWidget);

    QLabel* titleLabel = new QLabel(title);
    QLabel* priceLabel = new QLabel(QString("$%1").arg(price));
    QLabel* statusLabel = new QLabel(status);

    // Add image if the path is provided
    QLabel* imageLabel = new QLabel();
    if (!imagePath.isEmpty()) {
        QPixmap pixmap(imagePath);
        imageLabel->setPixmap(pixmap.scaled(100, 100, Qt::KeepAspectRatio));
    }
    
    layout->addWidget(titleLabel);
    layout->addWidget(priceLabel);
    layout->addWidget(statusLabel);
    layout->addWidget(imageLabel);

    return orderWidget;
}

void ProfilePage::viewOrderHistory() {
    // Example implementation
    qDebug() << "Viewing order history...";
    // Code to display order history goes here
}

void ProfilePage::handleOrderPlacement() {
    // Example implementation
    qDebug() << "Handling order placement...";
    // Code to handle placing an order goes here
}




void ProfilePage::saveMajorAndSemester() {
    QString selectedMajor = majorCombo->currentText();
    QString selectedSemester = semesterCombo->currentText();
    
    // Update the database with the new profile information
    if (dbManager->updateStudentProfile(userEmail, selectedMajor, selectedSemester)) {
        // Show success message
        QMessageBox::information(this, "Profile Updated",
            "Your profile has been updated successfully. Your recommended books will be updated accordingly.");
            
        // Get parent window (MainShopWindow)
        QWidget* parent = parentWidget();
        while (parent && !parent->inherits("MainShopWindow")) {
            parent = parent->parentWidget();
        }
        
        // If we found MainShopWindow, update the TextbookPage
        if (parent) {
            if (auto* mainWindow = qobject_cast<MainShopWindow*>(parent)) {
                // This will trigger a refresh of the recommendations
                mainWindow->refreshTextbookPage();
            }
        }
    } else {
        QMessageBox::warning(this, "Error",
            "Failed to update profile. Please try again.");
    }
}

void ProfilePage::handleCreateListing() {
    showCreateListingDialog();
    refreshListings();  // Refresh after successful creation
}

void ProfilePage::handleRemoveListing() {
    // Will be implemented when we add database support
}

void ProfilePage::switchSection(int index) {
    contentStack->setCurrentIndex(index);
}

