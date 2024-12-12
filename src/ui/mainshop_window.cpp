#include "../include/ui/mainshop_window.h"
#include "ui/wishlist_page.h"
#include "ui/cart_page.h"
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
    , profileMenu(nullptr)
    , wishlistPage(nullptr)
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
    // Create immovable toolbar for navigation
    navBar = new QToolBar(this);
    navBar->setMovable(false);
    navBar->setStyleSheet(
        "QToolBar {"
        "    background-color: " + white + ";"
        "    border-bottom: 1px solid #E0E0E0;"
        "    padding: 5px 20px;"
        "}"
    );

    // Create container for logo and search
    QWidget* leftContainer = new QWidget;
    QHBoxLayout* leftLayout = new QHBoxLayout(leftContainer);
    leftLayout->setSpacing(20);
    leftLayout->setContentsMargins(0, 0, 0, 0);

    // Logo
    logoLabel = new QLabel("BMCC E-Store", this);
    logoLabel->setStyleSheet(
        "font-size: 22px;"
        "font-weight: bold;"
        "color: " + darkBlue + ";"
        "padding: 0 20px;"
    );
    logoLabel->setCursor(Qt::PointingHandCursor);
    logoLabel->installEventFilter(this);
    leftLayout->addWidget(logoLabel);

    // Search bar
    searchBar = new QLineEdit(this);
    searchBar->setPlaceholderText("Search products...");
    searchBar->setFixedWidth(300);
    searchBar->setStyleSheet(
        "QLineEdit {"
        "    padding: 8px 15px;"
        "    border: 1px solid #E0E0E0;"
        "    border-radius: 20px;"
        "    font-size: 14px;"
        "    background-color: " + white + ";"
        "}"
        "QLineEdit:focus {"
        "    border-color: " + sageGreen + ";"
        "}"
    );
    leftLayout->addWidget(searchBar);

    navBar->addWidget(leftContainer);

    // Add expanding spacer
    QWidget* spacer = new QWidget();
    spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    navBar->addWidget(spacer);

    // Container for right-aligned navigation buttons
    QWidget* iconContainer = new QWidget;
    QHBoxLayout* iconLayout = new QHBoxLayout(iconContainer);
    iconLayout->setSpacing(20);
    iconLayout->setContentsMargins(0, 0, 20, 0);

    // Create and style buttons
    cartButton = new QPushButton(this);
    wishlistButton = new QPushButton(this);
    profileButton = new QPushButton(this);

    // Common button style
    const QString buttonStyle = 
        "QPushButton {"
        "    border: none;"
        "    padding: 8px;"
        "    border-radius: 20px;"
        "    background-color: transparent;"
        "}"
        "QPushButton:hover {"
        "    background-color: " + lightSage + ";"
        "}"
        "QPushButton:pressed {"
        "    background-color: " + sageGreen + ";"
        "}";

    // Set icons and styles
    cartButton->setIcon(QIcon(QCoreApplication::applicationDirPath() + "/../assets/images/nav/cartIcon.png"));
    wishlistButton->setIcon(QIcon(QCoreApplication::applicationDirPath() + "/../assets/images/nav/wishlistIcon.png"));
    profileButton->setIcon(QIcon(QCoreApplication::applicationDirPath() + "/../assets/images/nav/profileIcon.png"));

    // Set icon sizes
    cartButton->setIconSize(QSize(24, 24));
    wishlistButton->setIconSize(QSize(24, 24));
    profileButton->setIconSize(QSize(24, 24));

    // Apply styles
    cartButton->setStyleSheet(buttonStyle);
    wishlistButton->setStyleSheet(buttonStyle);
    profileButton->setStyleSheet(buttonStyle);

    // Add hover effect animations
    for (QPushButton* button : {cartButton, wishlistButton, profileButton}) {
        QGraphicsOpacityEffect* opacity = new QGraphicsOpacityEffect(button);
        button->setGraphicsEffect(opacity);
        
        // Store the animation as a property of the button
        QPropertyAnimation* hoverAnimation = new QPropertyAnimation(opacity, "opacity");
        hoverAnimation->setDuration(200);
        button->setProperty("hoverAnimation", QVariant::fromValue(hoverAnimation));
        
        // Install event filter for hover effects
        button->installEventFilter(this);
    }

    // Add buttons to layout
    iconLayout->addWidget(cartButton);
    iconLayout->addWidget(wishlistButton);
    iconLayout->addWidget(profileButton);

    // Add the icon container to navbar
    navBar->addWidget(iconContainer);

    // Initialize profile menu
    setupProfileMenu();

    // Connect button signals
    connect(cartButton, &QPushButton::clicked, this, [this]() {
        QGraphicsOpacityEffect* effect = new QGraphicsOpacityEffect(cartButton);
        cartButton->setGraphicsEffect(effect);

        QPropertyAnimation* clickAnim = new QPropertyAnimation(effect, "opacity");
        clickAnim->setDuration(100);
        clickAnim->setStartValue(1.0);
        clickAnim->setEndValue(0.5);
        clickAnim->setEasingCurve(QEasingCurve::OutQuad);

        QPropertyAnimation* revertAnim = new QPropertyAnimation(effect, "opacity");
        revertAnim->setDuration(100);
        revertAnim->setStartValue(0.5);
        revertAnim->setEndValue(1.0);
        revertAnim->setEasingCurve(QEasingCurve::InQuad);

        connect(clickAnim, &QPropertyAnimation::finished, [=]() {
            revertAnim->start(QAbstractAnimation::DeleteWhenStopped);
        });

        connect(revertAnim, &QPropertyAnimation::finished, [=]() {
            showCart();
        });

        clickAnim->start(QAbstractAnimation::DeleteWhenStopped);
    });

    connect(wishlistButton, &QPushButton::clicked, this, [this]() {
        QGraphicsOpacityEffect* effect = new QGraphicsOpacityEffect(wishlistButton);
        wishlistButton->setGraphicsEffect(effect);

        QPropertyAnimation* clickAnim = new QPropertyAnimation(effect, "opacity");
        clickAnim->setDuration(100);
        clickAnim->setStartValue(1.0);
        clickAnim->setEndValue(0.5);
        clickAnim->setEasingCurve(QEasingCurve::OutQuad);

        QPropertyAnimation* revertAnim = new QPropertyAnimation(effect, "opacity");
        revertAnim->setDuration(100);
        revertAnim->setStartValue(0.5);
        revertAnim->setEndValue(1.0);
        revertAnim->setEasingCurve(QEasingCurve::InQuad);

        connect(clickAnim, &QPropertyAnimation::finished, [=]() {
            revertAnim->start(QAbstractAnimation::DeleteWhenStopped);
        });

        connect(revertAnim, &QPropertyAnimation::finished, [=]() {
            showWishlist();
        });

        clickAnim->start(QAbstractAnimation::DeleteWhenStopped);
    });

    connect(profileButton, &QPushButton::clicked, this, [this]() {
        QGraphicsOpacityEffect* effect = new QGraphicsOpacityEffect(profileButton);
        profileButton->setGraphicsEffect(effect);

        QPropertyAnimation* clickAnim = new QPropertyAnimation(effect, "opacity");
        clickAnim->setDuration(100);
        clickAnim->setStartValue(1.0);
        clickAnim->setEndValue(0.5);
        clickAnim->setEasingCurve(QEasingCurve::OutQuad);

        QPropertyAnimation* revertAnim = new QPropertyAnimation(effect, "opacity");
        revertAnim->setDuration(100);
        revertAnim->setStartValue(0.5);
        revertAnim->setEndValue(1.0);
        revertAnim->setEasingCurve(QEasingCurve::InQuad);

        connect(clickAnim, &QPropertyAnimation::finished, [=]() {
            revertAnim->start(QAbstractAnimation::DeleteWhenStopped);
        });

        connect(revertAnim, &QPropertyAnimation::finished, [=]() {
            toggleProfileMenu();
        });

        clickAnim->start(QAbstractAnimation::DeleteWhenStopped);
    });

    connect(wishlistButton, &QPushButton::clicked, this, [this]() {
        QGraphicsOpacityEffect* effect = new QGraphicsOpacityEffect(wishlistButton);
        wishlistButton->setGraphicsEffect(effect);

        QPropertyAnimation* clickAnim = new QPropertyAnimation(effect, "opacity");
        clickAnim->setDuration(100);
        clickAnim->setStartValue(1.0);
        clickAnim->setEndValue(0.5);
        clickAnim->setEasingCurve(QEasingCurve::OutQuad);

        QPropertyAnimation* revertAnim = new QPropertyAnimation(effect, "opacity");
        revertAnim->setDuration(100);
        revertAnim->setStartValue(0.5);
        revertAnim->setEndValue(1.0);
        revertAnim->setEasingCurve(QEasingCurve::InQuad);

        connect(clickAnim, &QPropertyAnimation::finished, [=]() {
            revertAnim->start(QAbstractAnimation::DeleteWhenStopped);
        });

        connect(revertAnim, &QPropertyAnimation::finished, [=]() {
            showWishlist();
        });

        clickAnim->start(QAbstractAnimation::DeleteWhenStopped);
    });

    // Add navbar to main window
    addToolBar(navBar);
}

// Implement the showProfile method
void MainShopWindow::showProfile() {
    qDebug() << "showProfile called in MainShopWindow";
    if (!profilePage) {
        qDebug() << "Creating new profile page";
        profilePage = new ProfilePage(authenticator, dbManager, currentUserEmail, this);
        contentStack->addWidget(profilePage);
    }
    
    homepageStack->hide();
    contentStack->show();
    contentStack->setCurrentWidget(profilePage);
    profileMenu->hide();
    qDebug() << "Profile page should be visible now";
}


// Sets up profile drop down/ pop-up menu
void MainShopWindow::setupProfileMenu() {
    if (!profileMenu) {
        qDebug() << "Creating profile menu";
        profileMenu = new ProfileMenu(this);
        profileMenu->hide();
        
        // Use direct connection syntax
        connect(profileMenu, SIGNAL(profileRequested()), this, SLOT(showProfile()));
        connect(profileMenu, SIGNAL(logoutRequested()), this, SLOT(handleLogout()));
    }
}

void MainShopWindow::toggleProfileMenu() {
    if (profileMenu->isHidden()) {
        QPoint pos = profileButton->mapToGlobal(QPoint(0, profileButton->height()));
        pos.setX(pos.x() + profileButton->width() - profileMenu->width());
        profileMenu->move(pos);
        profileMenu->show();
        qApp->installEventFilter(this);
    } else {
        profileMenu->hide();
        qApp->removeEventFilter(this);
    }
}





bool MainShopWindow::eventFilter(QObject* watched, QEvent* event) {
    if (event->type() == QEvent::MouseButtonPress) {
        // Add this block to handle logo clicks
        if (watched == logoLabel) {
            showHomepage();
            return true;
        }
        
        // Your existing profile menu code
        if (profileMenu && !profileMenu->isHidden()) {
            QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
            QPoint globalPos = mouseEvent->globalPosition().toPoint();
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
    // Create container for category navigation
    QWidget* categoryBar = new QWidget(this);
    QHBoxLayout* categoryLayout = new QHBoxLayout(categoryBar);
    categoryLayout->setSpacing(5);  // Reduced spacing between categories
    categoryLayout->setContentsMargins(20, 8, 20, 8);  // Comfortable padding

    // Create home button for returning to main page
    homeButton = new QPushButton("Home", this);
    applyButtonStyle(homeButton, true);
    
    // Create category navigation buttons
    textbooksButton = new QPushButton("Textbooks", this);
    furnitureButton = new QPushButton("Furniture", this);
    electronicsButton = new QPushButton("Electronics", this);
    suppliesButton = new QPushButton("Supplies", this);
    clothingButton = new QPushButton("Clothing", this);

    // Collect all buttons for consistent styling
    QVector<QPushButton*> buttons = {
        homeButton, textbooksButton, furnitureButton, 
        electronicsButton, suppliesButton, clothingButton
    };
    
    // Apply consistent styling to all category buttons
    for(auto button : buttons) {
        applyButtonStyle(button, true);
        categoryLayout->addWidget(button);
    }

    // Add subtle dividers between categories for visual separation
    for(int i = 0; i < buttons.size() - 1; i++) {
        QFrame* divider = new QFrame;
        divider->setFrameShape(QFrame::VLine);
        divider->setStyleSheet("background-color: #E0E0E0;");
        divider->setFixedWidth(1);
        categoryLayout->addWidget(divider);
    }

    // Connect category buttons to their respective views
    connect(homeButton, &QPushButton::clicked, this, &MainShopWindow::showHomepage);
    connect(textbooksButton, &QPushButton::clicked, this, &MainShopWindow::showTextbooks);
    connect(furnitureButton, &QPushButton::clicked, this, &MainShopWindow::showFurniture);
    connect(electronicsButton, &QPushButton::clicked, this, &MainShopWindow::showElectronics);
    connect(suppliesButton, &QPushButton::clicked, this, &MainShopWindow::showSchoolSupplies);
    connect(clothingButton, &QPushButton::clicked, this, &MainShopWindow::showClothing);

    // Add category bar to main layout
    centralWidget()->layout()->addWidget(categoryBar);
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

void MainShopWindow::applyButtonStyle(QPushButton* button, bool isCategory) {
    // Apply consistent styling to buttons with size variation based on type
    button->setStyleSheet(
        "QPushButton {"
        "    color: " + darkBlue + ";"
        "    background: none;"
        "    border: none;"
        "    padding: " + (isCategory ? "8px 15px" : "10px 20px") + ";"  // Different padding for categories
        "    font-size: 14px;"
        "    font-weight: " + (isCategory ? "normal" : "bold") + ";"  // Bold for non-category buttons
        "    min-width: " + (isCategory ? "80px" : "100px") + ";"  // Smaller width for category buttons
        "    transition: all 0.3s;"  // Smooth transition for all changes
        "}"
        "QPushButton:hover {"
        "    color: " + sageGreen + ";"  // Change text color on hover
        "    background-color: " + lightSage + ";"  // Subtle background on hover
        "}"
    );
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

void MainShopWindow::refreshTextbookPage() {
    // Find and refresh the TextbookPage
    for (int i = 0; i < contentStack->count(); ++i) {
        TextbookPage* textbookPage = qobject_cast<TextbookPage*>(contentStack->widget(i));
        if (textbookPage) {
            textbookPage->refreshRecommendations();
            break;
        }
    }
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
    TextbookPage* textbookPage = qobject_cast<TextbookPage*>(contentStack->widget(0));
    if (textbookPage) {
        textbookPage->setUserEmail(currentUserEmail);
    }
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
    if (!cartPage) {
        cartPage = new CartPage(dbManager, currentUserEmail, this);
        contentStack->addWidget(cartPage);
    }
    
    cartPage->setUserEmail(currentUserEmail);
    cartPage->refreshCart();
    
    homepageStack->hide();
    contentStack->show();

    // Fade transition to cart page
    QWidget* currentWidget = contentStack->currentWidget();
    cartPage->setWindowOpacity(0.0);
    contentStack->setCurrentWidget(cartPage);

    // Fade out current widget
    QPropertyAnimation* fadeOut = new QPropertyAnimation(currentWidget, "windowOpacity");
    fadeOut->setDuration(200);
    fadeOut->setStartValue(1.0);
    fadeOut->setEndValue(0.0);
    
    // Fade in cart page
    QPropertyAnimation* fadeIn = new QPropertyAnimation(cartPage, "windowOpacity");
    fadeIn->setDuration(200);
    fadeIn->setStartValue(0.0);
    fadeIn->setEndValue(1.0);

    // Connect cart signals
    connect(cartPage, &CartPage::checkoutCompleted, [this]() {
        // Return to homepage after successful checkout
        cartPage->refreshCart();
        showHomepage();
    });

    fadeOut->start(QAbstractAnimation::DeleteWhenStopped);
    fadeIn->start(QAbstractAnimation::DeleteWhenStopped);
}

void MainShopWindow::showWishlist() {
    if (!wishlistPage) {
        wishlistPage = new WishlistPage(dbManager, currentUserEmail, this);
        contentStack->addWidget(wishlistPage);
        
        // Connect continue shopping signal
        connect(wishlistPage, &WishlistPage::continueShoppingClicked, [this]() {
            showHomepage();
        });
    }
    
    wishlistPage->setUserEmail(currentUserEmail);
    wishlistPage->refreshWishlist();
    
    homepageStack->hide();
    contentStack->show();

    // Fade transition to wishlist page
    QWidget* currentWidget = contentStack->currentWidget();
    wishlistPage->setWindowOpacity(0.0);
    contentStack->setCurrentWidget(wishlistPage);

    // Fade out current widget
    QPropertyAnimation* fadeOut = new QPropertyAnimation(currentWidget, "windowOpacity");
    fadeOut->setDuration(200);
    fadeOut->setStartValue(1.0);
    fadeOut->setEndValue(0.0);
    
    // Fade in wishlist page
    QPropertyAnimation* fadeIn = new QPropertyAnimation(wishlistPage, "windowOpacity");
    fadeIn->setDuration(200);
    fadeIn->setStartValue(0.0);
    fadeIn->setEndValue(1.0);

    fadeOut->start(QAbstractAnimation::DeleteWhenStopped);
    fadeIn->start(QAbstractAnimation::DeleteWhenStopped);
}

void MainShopWindow::handleLogout() {
    authenticator->logout(currentUserEmail);
    emit logoutRequested();
}

void MainShopWindow::showHomepage() {
    // Hide category content and show homepage
    contentStack->hide();
    homepageStack->show();
}