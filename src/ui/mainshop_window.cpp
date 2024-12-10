#include "../include/ui/mainshop_window.h"
#include "ui/textbook_page.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDir>
#include <QToolBar>
#include <QLabel>
#include <QEvent>
#include <QFrame>
#include <QScrollArea>
#include <QPropertyAnimation>
#include <QGraphicsDropShadowEffect>
#include <QGridLayout>
#include <QCoreApplication>
#include <QDebug>
#include <QMouseEvent>


MainShopWindow::MainShopWindow(Authenticator* auth, DatabaseManager* db, const QString& userEmail, QWidget *parent)
    : QMainWindow(parent)
    , authenticator(auth)
    , dbManager(db)
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
    , profilePage(nullptr)
{
    setupUI();
    handleFeaturedTabChange(0);
}

// Update UI elements that display the email
void MainShopWindow::setUserEmail(const QString& email) {
    currentUserEmail = email;
    if (profilePage) {
        delete profilePage;
        profilePage = nullptr;
    }
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

    // Setup nav and category bars first
    setupNavBar();
    setupCategoryBar();

    // Create stacked widgets for homepage and category content
    homepageStack = new QStackedWidget(this);
    contentStack = new QStackedWidget(this);

    // Create and setup homepage
    QWidget* homePage = new QWidget;
    QVBoxLayout* homeLayout = new QVBoxLayout(homePage);
    homeLayout->setSpacing(0);
    homeLayout->setContentsMargins(0, 0, 0, 0);
    
    // Add hero and featured sections to homeLayout instead of central widget
    QWidget* heroSection = new QWidget;
    QVBoxLayout* heroLayout = new QVBoxLayout(heroSection);
    setupHeroSection(heroLayout);
    homeLayout->addWidget(heroSection);

    QWidget* featuredSection = new QWidget;
    QVBoxLayout* featuredLayout = new QVBoxLayout(featuredSection);
    setupFeaturedSection(featuredLayout);
    homeLayout->addWidget(featuredSection);
    
    homepageStack->addWidget(homePage);

    // Setup category pages
    setupContentArea();

    // Add both stacks to main layout
    mainLayout->addWidget(homepageStack);
    mainLayout->addWidget(contentStack);

    // Initially show homepage and hide category content
    homepageStack->show();
    contentStack->hide();
}

QPushButton* MainShopWindow::createPreNavButton(const QString& iconPath, const QString& style) {
   QPushButton* button = new QPushButton;
   button->setIcon(QIcon(iconPath));
   button->setIconSize(QSize(20, 20));
   button->setStyleSheet(style);
   return button;
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
        "font-size: 24px;"
        "font-weight: bold;"
        "color: " + darkBlue + ";"
        "font-family: -apple-system, BlinkMacSystemFont;"
        "margin-right: 20px;"
        "background: transparent;"
    );
    logo->setCursor(Qt::PointingHandCursor);
    logo->installEventFilter(this);
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

    // Create container for icons
    QWidget* iconContainer = new QWidget;
    QHBoxLayout* iconLayout = new QHBoxLayout(iconContainer);
    iconLayout->setSpacing(20);  // Even spacing between icons
    iconLayout->setContentsMargins(0, 0, 20, 0);  // Right margin before user section

    // Cart Button
    cartButton = new QPushButton(this);
    QString cartIconPath = QCoreApplication::applicationDirPath() + "/../assets/images/nav/cartIcon.png";
    cartButton->setIcon(QIcon(cartIconPath));
    cartButton->setIconSize(QSize(24, 24));
    cartButton->setStyleSheet(
        "QPushButton {"
        "    border: none;"
        "    padding: 8px;"
        "    border-radius: 20px;"
        "    background-color: transparent;"
        "}"
        "QPushButton:hover {"
        "    background-color: " + lightSage + ";"
        "}"
    );

    // Wishlist Button
    wishlistButton = new QPushButton(this);
    QString wishlistIconPath = QCoreApplication::applicationDirPath() + "/../assets/images/nav/wishlistIcon.png";
    wishlistButton->setIcon(QIcon(wishlistIconPath));
    wishlistButton->setIconSize(QSize(24, 24));
    wishlistButton->setStyleSheet(
        "QPushButton {"
        "    border: none;"
        "    padding: 8px;"
        "    border-radius: 20px;"
        "    background-color: transparent;"
        "}"
        "QPushButton:hover {"
        "    background-color: " + lightSage + ";"
        "}"
    );

    // Profile Button
    profileButton = new QPushButton(this);
    QString profileIconPath = QCoreApplication::applicationDirPath() + "/../assets/images/nav/profileIcon.png";
    profileButton->setIcon(QIcon(profileIconPath));
    profileButton->setIconSize(QSize(24, 24));
    profileButton->setStyleSheet(
        "QPushButton {"
        "    border: none;"
        "    padding: 8px;"
        "    border-radius: 20px;"
        "    background-color: transparent;"
        "}"
        "QPushButton:hover {"
        "    background-color: " + lightSage + ";"
        "}"
    );

    // Add buttons to icon container
    iconLayout->addWidget(cartButton);
    iconLayout->addWidget(wishlistButton);
    iconLayout->addWidget(profileButton);

    // Add the icon container to navbar
    navBar->addWidget(iconContainer);

    // User email label
    QLabel* userLabel = new QLabel(currentUserEmail);
    userLabel->setStyleSheet("color: " + darkBlue + "; margin-right: 10px;");
    navBar->addWidget(userLabel);

    addToolBar(navBar);

    // Setup profile menu
    setupProfileMenu();

    // Connect signals
    connect(profileButton, &QPushButton::clicked, this, &MainShopWindow::toggleProfileMenu);
    connect(searchBar, &QLineEdit::returnPressed, this, &MainShopWindow::handleSearch);
}

// Implement the showProfile method
void MainShopWindow::showProfile() {
    // If profile page doesn't exist, create it
    if (!profilePage) {
        profilePage = new ProfilePage(authenticator, dbManager, currentUserEmail, this);
        contentStack->addWidget(profilePage);
    }

    // Hide profile menu
    profileMenu->hide();

    // Hide homepage stack and show content stack
    homepageStack->hide();
    contentStack->show();

    // Switch to profile page with fade transition
    QWidget* currentWidget = contentStack->currentWidget();
    profilePage->setWindowOpacity(0.0);
    contentStack->setCurrentWidget(profilePage);

    // Fade out current widget
    QPropertyAnimation* fadeOut = new QPropertyAnimation(currentWidget, "windowOpacity");
    fadeOut->setDuration(200);
    fadeOut->setStartValue(1.0);
    fadeOut->setEndValue(0.0);
    
    // Fade in profile page
    QPropertyAnimation* fadeIn = new QPropertyAnimation(profilePage, "windowOpacity");
    fadeIn->setDuration(200);
    fadeIn->setStartValue(0.0);
    fadeIn->setEndValue(1.0);

    fadeOut->start(QAbstractAnimation::DeleteWhenStopped);
    fadeIn->start(QAbstractAnimation::DeleteWhenStopped);
}


// Sets up profile drop down/ pop-up menu
void MainShopWindow::setupProfileMenu() {
    profileMenu = new ProfileMenu(this);
    profileMenu->hide();
    
    // Connect signals
    connect(profileMenu, &ProfileMenu::profileRequested, this, &MainShopWindow::showProfile);
    connect(profileMenu, &ProfileMenu::logoutRequested, 
            this, &MainShopWindow::handleLogout);
    connect(profileMenu, &ProfileMenu::profileRequested, 
            this, &MainShopWindow::showProfile); // We'll implement this later
}

void MainShopWindow::toggleProfileMenu() {
    if (profileMenu->isHidden()) {
        // Calculate position relative to the profile button
        QPoint buttonPos = profileButton->mapToGlobal(QPoint(0, 0));
        QPoint menuPos = buttonPos + QPoint(
            profileButton->width() - profileMenu->width(),  // Align right edge
            profileButton->height() + 5  // Small gap below button
        );
        
        // Move menu to calculated position
        profileMenu->move(mapFromGlobal(menuPos));
        
        // Show menu with fade effect
        QGraphicsOpacityEffect* opacity = new QGraphicsOpacityEffect(profileMenu);
        profileMenu->setGraphicsEffect(opacity);
        
        QPropertyAnimation* fadeIn = new QPropertyAnimation(opacity, "opacity");
        fadeIn->setDuration(150);
        fadeIn->setStartValue(0.0);
        fadeIn->setEndValue(1.0);
        fadeIn->start(QAbstractAnimation::DeleteWhenStopped);
        
        profileMenu->show();
        
        // Install event filter to handle clicking outside
        qApp->installEventFilter(this);
    } else {
        profileMenu->hide();
        qApp->removeEventFilter(this);
    }
}





bool MainShopWindow::eventFilter(QObject* watched, QEvent* event) {
    if (event->type() == QEvent::MouseButtonPress) {
        if (profileMenu && !profileMenu->isHidden()) {
            QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
            QPoint globalPos = mouseEvent->globalPosition().toPoint();  // Use globalPosition()
            if (!profileMenu->geometry().contains(mapFromGlobal(globalPos)) &&
                !profileButton->geometry().contains(mapFromGlobal(globalPos))) {
                profileMenu->hide();
                qApp->removeEventFilter(this);
                return true;
            }
        }
    }
    return QMainWindow::eventFilter(watched, event);
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
        "   color: " + darkBlue + ";"
        "   border: none;"
        "   padding: 8px 15px;"
        "   font-size: 14px;"
        "   border-radius: 15px;"
        "   background-image: " + iconPath + ";"
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
    if (category == "Textbooks") {
        return new TextbookPage(dbManager, this);
    }
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



void MainShopWindow::setupHeroSection(QVBoxLayout* parentLayout) {
    QWidget* heroWidget = new QWidget;
    QVBoxLayout* heroLayout = new QVBoxLayout(heroWidget);
    heroLayout->setContentsMargins(0, 0, 0, 0);  // Remove all margins
    heroLayout->setSpacing(0);  // Remove spacing

    QLabel* heroImage = new QLabel;
    QString imagePath = QCoreApplication::applicationDirPath() + "/../assets/images/home/school.jpg";
    QPixmap image(imagePath);
    
    if (!image.isNull()) {
        // Fill the entire width while maintaining aspect ratio
        heroImage->setPixmap(image.scaled(1400, 400, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        heroImage->setAlignment(Qt::AlignCenter);
    } else {
        qDebug() << "Image not found at:" << imagePath;
        heroImage->setText("Image not found");
    }

    heroLayout->addWidget(heroImage);
    parentLayout->addWidget(heroWidget);
}

void MainShopWindow::setupFeaturedSection(QVBoxLayout* parentLayout) {
    QWidget* featuredWidget = new QWidget;
    QVBoxLayout* featuredLayout = new QVBoxLayout(featuredWidget);
    featuredLayout->setSpacing(0);
    featuredLayout->setContentsMargins(60, 60, 60, 60);

    // Title
    QLabel* title = new QLabel("Featured Products");
    title->setStyleSheet(
        "font-size: 32px;"
        "font-weight: bold;"
        "color: " + darkBlue + ";"
        "margin-bottom: 30px;"
    );
    title->setAlignment(Qt::AlignCenter);
    featuredLayout->addWidget(title);

    // Tabs container
    QWidget* tabsWidget = new QWidget;
    QHBoxLayout* tabsLayout = new QHBoxLayout(tabsWidget);
    tabsLayout->setContentsMargins(0, 0, 0, 0);
    tabsLayout->setAlignment(Qt::AlignCenter);

    // Create tab buttons with fixed spacing
    QStringList categories = {"Tech", "Clothing", "Supplies"};
    for (int i = 0; i < categories.size(); ++i) {
        QPushButton* tab = new QPushButton(categories[i]);
        tab->setFixedWidth(500);  // Fixed width for each tab
        tab->setStyleSheet(
            "QPushButton {"
            "    border: none;"
            "    padding: 15px;"
            "    font-size: 16px;"
            "    color: " + darkBlue + ";"
            "    background: none;"
            "    text-align: center;"
            "}"
            "QPushButton:hover {"
            "    color: " + sageGreen + ";"
            "}"
        );
        connect(tab, &QPushButton::clicked, this, [this, i]() { handleFeaturedTabChange(i); });
        featureTabButtons.append(tab);
        tabsLayout->addWidget(tab);

        if (i < categories.size() - 1) {
            // Add fixed spacing between buttons
            QSpacerItem* spacer = new QSpacerItem(50, 0, QSizePolicy::Fixed, QSizePolicy::Minimum);
            tabsLayout->addItem(spacer);
        }
    }

    // Create indicator container
    QWidget* indicatorContainer = new QWidget;
    QHBoxLayout* indicatorLayout = new QHBoxLayout(indicatorContainer);
    indicatorLayout->setSpacing(0);  // No spacing between indicators
    indicatorLayout->setContentsMargins(0, 0, 0, 0);
    indicatorLayout->setAlignment(Qt::AlignCenter);

    // Create indicators with fixed width
    for (int i = 0; i < 3; ++i) {
        QFrame* line = new QFrame;
        line->setFixedHeight(3);
        line->setFixedWidth(500);  // Match tab width
        line->setStyleSheet("background-color: " + lightGrey + ";");
        featureIndicators.append(line);
        indicatorLayout->addWidget(line);
    }

    QVBoxLayout* tabContainerLayout = new QVBoxLayout;
    tabContainerLayout->addWidget(tabsWidget);
    tabContainerLayout->addWidget(indicatorContainer);
    tabContainerLayout->setSpacing(0);
    tabContainerLayout->setContentsMargins(0, 0, 0, 0);
    featuredLayout->addLayout(tabContainerLayout);

    parentLayout->addWidget(featuredWidget);
}

void MainShopWindow::handleFeaturedTabChange(int index) {
    // Change the tab line color
    for (int i = 0; i < featureIndicators.size(); ++i) {
        featureIndicators[i]->setStyleSheet(
            "background-color: " + (i == index ? darkGrey : lightGrey) + ";"
        );
    }

    // Update tab text label color
    for (int i = 0; i < featureTabButtons.size(); ++i) {
        featureTabButtons[i]->setStyleSheet(
            "QPushButton {"
            "    border: none;"
            "    padding: 15px;"
            "    font-size: 16px;"
            "    color: " + (i == index ? sageGreen : darkBlue) + ";"
            "}"
            "QPushButton:hover {"
            "    color: " + sageGreen + ";"
            "}"
        );
    }
}




// Implement slot methods
void MainShopWindow::handleSearch() {
    // Implement search functionality
}

void MainShopWindow::showTextbooks() {
    homepageStack->hide();
    contentStack->show();
    contentStack->setCurrentIndex(0);
}

void MainShopWindow::showFurniture() {
    homepageStack->hide();
    contentStack->show();
    contentStack->setCurrentIndex(1);
}

void MainShopWindow::showElectronics() {
    homepageStack->hide();
    contentStack->show();
    contentStack->setCurrentIndex(2);
}

void MainShopWindow::showSchoolSupplies() {
    homepageStack->hide();
    contentStack->show();
    contentStack->setCurrentIndex(3);
}

void MainShopWindow::showClothing() {
    homepageStack->hide();
    contentStack->show();
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

void MainShopWindow::showHomepage() {
    contentStack->hide();
    homepageStack->show();
}