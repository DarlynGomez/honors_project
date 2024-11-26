#include "../include/ui/mainshop_window.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QToolBar>
#include <QLabel>
#include <QFrame>
#include <QScrollArea>
#include <QPropertyAnimation>
#include <QGraphicsDropShadowEffect>
#include <QGridLayout>

MainShopWindow::MainShopWindow(Authenticator* auth, const QString& userEmail, QWidget *parent)
    : QMainWindow(parent)
    , authenticator(auth)
    , currentUserEmail(userEmail)
    , contentStack(nullptr)
    , navBar(nullptr)
    , searchBar(nullptr)
    , cartButton(nullptr)
    , wishlistButton(nullptr)
    , textbooksButton(nullptr)
    , furnitureButton(nullptr)
    , electronicsButton(nullptr)
    , suppliesButton(nullptr)
    , clothingButton(nullptr)
{
    setupUI();
}

void MainShopWindow::setUserEmail(const QString& email) {
    currentUserEmail = email;
    // Update UI elements that display the email
}

void MainShopWindow::setupUI() {
    // Set window properties
    setMinimumSize(1200, 800);
    setStyleSheet("QMainWindow { background-color: " + white + "; }");

    // Create central widget with proper initialization
    QWidget* centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    
    QVBoxLayout* mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(0, 0, 0, 0);

    // Add a container for all content
    QWidget* contentContainer = new QWidget;
    QVBoxLayout* contentLayout = new QVBoxLayout(contentContainer);
    contentLayout->setSpacing(0);
    contentLayout->setContentsMargins(0, 0, 0, 0);

    // Setup components
    setupNavBar();
    setupCategoryBar();
    setupContentArea();

    // Add the content container to the main layout
    mainLayout->addWidget(contentContainer);
}

void MainShopWindow::setupNavBar() {
    navBar = new QToolBar(this);
    navBar->setMovable(false);
    navBar->setStyleSheet(
        "QToolBar {"
        "    background-color: " + white + ";"
        "    border-bottom: 1px solid #E0E0E0;"
        "    padding: 10px;"
        "}"
    );

    // BMCC Logo
    QLabel* logo = new QLabel("BMCC E-Store", this);
    logo->setStyleSheet(
        "font-size: 24px; font-weight: bold; color: " + darkBlue + ";"
        "font-family: -apple-system, BlinkMacSystemFont;"
        "margin-right: 20px;"
    );
    navBar->addWidget(logo);

    // Search Bar
    searchBar = new QLineEdit(this);
    searchBar->setPlaceholderText("Search products...");
    searchBar->setMinimumWidth(400);
    searchBar->setStyleSheet(
        "QLineEdit {"
        "    padding: 8px 15px;"
        "    border: 2px solid #E0E0E0;"
        "    border-radius: 20px;"
        "    font-size: 14px;"
        "    background-color: white;"
        "}"
        "QLineEdit:focus {"
        "    border-color: " + sageGreen + ";"
        "}"
    );
    navBar->addWidget(searchBar);

    // Spacer
    QWidget* spacer = new QWidget();
    spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    navBar->addWidget(spacer);

    // Cart and Wishlist Buttons
    cartButton = createNavButton("", "Cart");
    wishlistButton = createNavButton("", "Wishlist");
    
    // User section
    QLabel* userLabel = new QLabel(currentUserEmail);
    userLabel->setStyleSheet("color: " + darkBlue + "; margin-right: 10px;");
    
    QPushButton* logoutBtn = new QPushButton("Logout");
    logoutBtn->setStyleSheet(
        "QPushButton {"
        "    color: " + darkBlue + ";"
        "    border: none;"
        "    padding: 5px 10px;"
        "    font-size: 14px;"
        "}"
        "QPushButton:hover { color: " + sageGreen + "; }"
    );

    navBar->addWidget(cartButton);
    navBar->addWidget(wishlistButton);
    navBar->addWidget(userLabel);
    navBar->addWidget(logoutBtn);

    addToolBar(navBar);

    // Connect signals
    connect(logoutBtn, &QPushButton::clicked, this, &MainShopWindow::handleLogout);
    connect(searchBar, &QLineEdit::returnPressed, this, &MainShopWindow::handleSearch);
}

void MainShopWindow::setupCategoryBar() {
    QWidget* categoryBar = new QWidget(this);
    QHBoxLayout* categoryLayout = new QHBoxLayout(categoryBar);
    categoryLayout->setSpacing(20);
    categoryLayout->setContentsMargins(20, 10, 20, 10);

    // Create buttons
    textbooksButton = createCategoryButton("Textbooks");
    furnitureButton = createCategoryButton("Furniture");
    electronicsButton = createCategoryButton("Electronics");
    suppliesButton = createCategoryButton("School Supplies");
    clothingButton = createCategoryButton("Clothing");

    // Add buttons to layout
    categoryLayout->addWidget(textbooksButton);
    categoryLayout->addWidget(furnitureButton);
    categoryLayout->addWidget(electronicsButton);
    categoryLayout->addWidget(suppliesButton);
    categoryLayout->addWidget(clothingButton);

    // Add to central widget's layout
    centralWidget()->layout()->addWidget(categoryBar);

    // Connect signals
    connect(textbooksButton, &QPushButton::clicked, this, &MainShopWindow::showTextbooks);
    connect(furnitureButton, &QPushButton::clicked, this, &MainShopWindow::showFurniture);
    connect(electronicsButton, &QPushButton::clicked, this, &MainShopWindow::showElectronics);
    connect(suppliesButton, &QPushButton::clicked, this, &MainShopWindow::showSchoolSupplies);
    connect(clothingButton, &QPushButton::clicked, this, &MainShopWindow::showClothing);
}

void MainShopWindow::setupContentArea() {
    contentStack = new QStackedWidget(this);
    
    // Add category pages
    contentStack->addWidget(createCategoryWidget("Textbooks"));
    contentStack->addWidget(createCategoryWidget("Furniture"));
    contentStack->addWidget(createCategoryWidget("Electronics"));
    contentStack->addWidget(createCategoryWidget("School Supplies"));
    contentStack->addWidget(createCategoryWidget("Clothing"));

    // Add to central widget's layout
    centralWidget()->layout()->addWidget(contentStack);
}

void MainShopWindow::setupStyles() {
    // Additional styles can be added here
}

QPushButton* MainShopWindow::createNavButton(const QString& iconPath, const QString& text) {
    QPushButton* button = new QPushButton(text);
    button->setStyleSheet(
        "QPushButton {"
        "    color: " + darkBlue + ";"
        "    border: none;"
        "    padding: 8px 15px;"
        "    font-size: 14px;"
        "    border-radius: 15px;"
        "}"
        "QPushButton:hover {"
        "    background-color: " + lightSage + ";"
        "}"
    );
    return button;
}

QPushButton* MainShopWindow::createCategoryButton(const QString& text) {
    QPushButton* button = new QPushButton(text);
    button->setStyleSheet(
        "QPushButton {"
        "    color: " + darkBlue + ";"
        "    background-color: white;"
        "    border: 2px solid " + sageGreen + ";"
        "    padding: 10px 20px;"
        "    font-size: 14px;"
        "    border-radius: 20px;"
        "    min-width: 120px;"
        "}"
        "QPushButton:hover {"
        "    background-color: " + sageGreen + ";"
        "    color: white;"
        "}"
        "QPushButton:pressed {"
        "    background-color: " + darkBlue + ";"
        "    border-color: " + darkBlue + ";"
        "}"
    );
    return button;
}

QWidget* MainShopWindow::createCategoryWidget(const QString& category) {
    QWidget* widget = new QWidget;
    QVBoxLayout* layout = new QVBoxLayout(widget);
    
    QLabel* title = new QLabel(category);
    title->setStyleSheet(
        "font-size: 24px;"
        "font-weight: bold;"
        "color: " + darkBlue + ";"
        "margin-bottom: 20px;"
    );
    layout->addWidget(title);

    QGridLayout* grid = new QGridLayout;
    grid->setSpacing(20);

    // Add sample product cards
    for (int i = 0; i < 12; i++) {
        QWidget* card = new QWidget;
        card->setMinimumSize(250, 300);
        card->setStyleSheet(
            "QWidget {"
            "    background-color: white;"
            "    border: 1px solid #E0E0E0;"
            "    border-radius: 10px;"
            "}"
        );
        
        QGraphicsDropShadowEffect* shadow = new QGraphicsDropShadowEffect;
        shadow->setBlurRadius(10);
        shadow->setColor(QColor(0, 0, 0, 30));
        shadow->setOffset(0, 2);
        card->setGraphicsEffect(shadow);

        grid->addWidget(card, i / 4, i % 4);
    }

    QWidget* gridWidget = new QWidget;
    gridWidget->setLayout(grid);
    
    QScrollArea* scrollArea = new QScrollArea;
    scrollArea->setWidget(gridWidget);
    scrollArea->setWidgetResizable(true);
    scrollArea->setFrameShape(QFrame::NoFrame);
    
    layout->addWidget(scrollArea);
    return widget;
}

// Implement slot methods
void MainShopWindow::handleSearch() {
    // Implement search functionality
}

void MainShopWindow::showTextbooks() {
    contentStack->setCurrentIndex(0);
}

void MainShopWindow::showFurniture() {
    contentStack->setCurrentIndex(1);
}

void MainShopWindow::showElectronics() {
    contentStack->setCurrentIndex(2);
}

void MainShopWindow::showSchoolSupplies() {
    contentStack->setCurrentIndex(3);
}

void MainShopWindow::showClothing() {
    contentStack->setCurrentIndex(4);
}

void MainShopWindow::showCart() {
    // Implement cart view
}

void MainShopWindow::showWishlist() {
    // Implement wishlist view
}

void MainShopWindow::handleLogout() {
    authenticator->logout(currentUserEmail);
    emit logoutRequested();
}