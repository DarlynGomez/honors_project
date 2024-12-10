#include "ui/profile_page.h"
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
    QGridLayout* grid = new QGridLayout(gridContainer);
    grid->setSpacing(20);
    grid->setContentsMargins(0, 0, 0, 0);

    // Sample listing cards
    for (int i = 0; i < 6; i++) {
        QWidget* card = createListingCard();
        grid->addWidget(card, i / 2, i % 2);
    }

    layout->addWidget(gridContainer);
    layout->addStretch();
    
    scrollArea->setWidget(contentWidget);
    return scrollArea;
}

QWidget* ProfilePage::createOrderHistorySection() {
    QScrollArea* scrollArea = createStyledScrollArea();
    QWidget* contentWidget = new QWidget;
    QVBoxLayout* layout = new QVBoxLayout(contentWidget);
    layout->setSpacing(20);
    layout->setContentsMargins(0, 0, 20, 0);

    QLabel* title = new QLabel("Order History");
    title->setStyleSheet(
        "font-size: 24px;"
        "font-weight: bold;"
        "color: " + darkBlue + ";"
    );
    layout->addWidget(title);

    QLabel* placeholder = new QLabel("No orders yet");
    placeholder->setStyleSheet(
        "color: #666;"
        "font-size: 16px;"
        "padding: 40px;"
    );
    placeholder->setAlignment(Qt::AlignCenter);
    layout->addWidget(placeholder);

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
    dialog->setMinimumWidth(500);
    dialog->setStyleSheet("QDialog { background-color: white; }");

    QVBoxLayout* layout = new QVBoxLayout(dialog);
    layout->setSpacing(20);
    layout->setContentsMargins(30, 30, 30, 30);

    // Category selection
    QLabel* categoryLabel = new QLabel("Category:", dialog);
    QComboBox* categoryCombo = new QComboBox(dialog);
    categoryCombo->addItems({"Textbooks", "Furniture", "Electronics", "School Supplies", "Clothing"});
    
    // Stack for category-specific inputs
    QStackedWidget* inputStack = new QStackedWidget;
    
    // Create textbook form
    QWidget* textbookForm = new QWidget;
    QVBoxLayout* textbookLayout = new QVBoxLayout(textbookForm);
    
    // Textbook name
    QLabel* nameLabel = new QLabel("Textbook Name:", dialog);
    QLineEdit* nameInput = new QLineEdit(dialog);
    
    // Department
    QLabel* deptLabel = new QLabel("Department:", dialog);
    QLineEdit* deptInput = new QLineEdit(dialog);
    
    // Course Section
    QLabel* sectionLabel = new QLabel("Course Section:", dialog);
    QComboBox* sectionCombo = new QComboBox;
    sectionCombo->addItem("CSC");
    // Add other sections as needed
    
    // Course Codes
    QLabel* codesLabel = new QLabel("Course Codes (Multiple Selection):", dialog);
    QListWidget* codesWidget = new QListWidget;
    codesWidget->setSelectionMode(QAbstractItemView::MultiSelection);
    QStringList codes = {"111", "110", "211"};
    for(const QString& code : codes) {
        codesWidget->addItem(code);
    }
    codesWidget->setMaximumHeight(100);
    
    // LEC Code
    QLabel* lecLabel = new QLabel("LEC Code (Optional):", dialog);
    QLineEdit* lecInput = new QLineEdit;
    
    // Price
    QLabel* priceLabel = new QLabel("Price ($):", dialog);
    QLineEdit* priceInput = new QLineEdit;
    priceInput->setValidator(new QDoubleValidator(0, 99999.99, 2));

    // Add widgets to textbook layout
    textbookLayout->addWidget(nameLabel);
    textbookLayout->addWidget(nameInput);
    textbookLayout->addWidget(deptLabel);
    textbookLayout->addWidget(deptInput);
    textbookLayout->addWidget(sectionLabel);
    textbookLayout->addWidget(sectionCombo);
    textbookLayout->addWidget(codesLabel);
    textbookLayout->addWidget(codesWidget);
    textbookLayout->addWidget(lecLabel);
    textbookLayout->addWidget(lecInput);
    textbookLayout->addWidget(priceLabel);
    textbookLayout->addWidget(priceInput);

    // Add textbook form to stack
    inputStack->addWidget(textbookForm);
    
    // Add other category forms as needed...
    
    // Connect category combo to show appropriate form
    connect(categoryCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            inputStack, &QStackedWidget::setCurrentIndex);
    
    // Add to main layout
    layout->addWidget(categoryLabel);
    layout->addWidget(categoryCombo);
    layout->addWidget(inputStack);
    
    // Add buttons
    QHBoxLayout* buttonLayout = new QHBoxLayout;
    QPushButton* cancelButton = createStyledButton("Cancel");
    QPushButton* createButton = createStyledButton("Create Listing", true);
    
    buttonLayout->addWidget(cancelButton);
    buttonLayout->addWidget(createButton);
    layout->addLayout(buttonLayout);

    // Connect buttons
    connect(cancelButton, &QPushButton::clicked, dialog, &QDialog::reject);
    connect(createButton, &QPushButton::clicked, [=]() {
        if (categoryCombo->currentText() == "Textbooks") {
            // Validate textbook form
            if (nameInput->text().isEmpty() || deptInput->text().isEmpty() || 
                priceInput->text().isEmpty()) {
                QMessageBox::warning(dialog, "Validation Error", 
                    "Please fill in all required fields.");
                return;
            }
            
            // Get selected course codes
            QStringList selectedCodes;
            for (auto item : codesWidget->selectedItems()) {
                selectedCodes.append(item->text());
            }
            
            // Create textbook listing in database
            // You'll need to implement this in DatabaseManager
            // dbManager->createTextbookListing(...);
            
            dialog->accept();
        }
    });
    
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
    // Will be implemented when we add database support
}

void ProfilePage::saveMajorAndSemester() {
    // Will be implemented when we add database support
}

void ProfilePage::handleCreateListing() {
    showCreateListingDialog();
}

void ProfilePage::handleRemoveListing() {
    // Will be implemented when we add database support
}

void ProfilePage::switchSection(int index) {
    contentStack->setCurrentIndex(index);
}

