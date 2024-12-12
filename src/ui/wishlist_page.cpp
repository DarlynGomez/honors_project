#include "ui/wishlist_page.h"
#include <QScrollArea>
#include <QGraphicsDropShadowEffect>
#include <QMessageBox>

WishlistPage::WishlistPage(DatabaseManager* db, const QString& userEmail, QWidget *parent)
    : QWidget(parent), dbManager(db), currentUserEmail(userEmail), itemCount(0)
{
    setupUI();
    refreshWishlist();
}

void WishlistPage::setUserEmail(const QString& email) {
    currentUserEmail = email;
    refreshWishlist();
}

void WishlistPage::setupUI() {
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
    cardLayout->setSpacing(20);
    cardLayout->setContentsMargins(40, 40, 40, 40);

    // Header
    QWidget* headerWidget = new QWidget;
    QHBoxLayout* headerLayout = new QHBoxLayout(headerWidget);
    
    QLabel* title = new QLabel("Your Wishlist");
    title->setStyleSheet(
        "font-size: 28px;"
        "font-weight: bold;"
        "color: " + darkBlue + ";"
    );
    
    itemCountLabel = new QLabel;
    itemCountLabel->setStyleSheet(
        "font-size: 16px;"
        "color: #666;"
    );
    
    headerLayout->addWidget(title);
    headerLayout->addStretch();
    headerLayout->addWidget(itemCountLabel);
    
    cardLayout->addWidget(headerWidget);

    // Wishlist items
    QScrollArea* scrollArea = createStyledScrollArea();
    QWidget* scrollContent = new QWidget;
    wishlistItemsLayout = new QVBoxLayout(scrollContent);
    wishlistItemsLayout->setSpacing(15);
    scrollArea->setWidget(scrollContent);
    cardLayout->addWidget(scrollArea);

    // Button section
    QHBoxLayout* buttonLayout = new QHBoxLayout;
    buttonLayout->setSpacing(20);
    
    QPushButton* continueButton = createStyledButton("Continue Shopping", false);
    
    buttonLayout->addWidget(continueButton);
    cardLayout->addLayout(buttonLayout);
    
    mainLayout->addWidget(cardWidget);
    
    connect(continueButton, &QPushButton::clicked, this, &WishlistPage::handleContinueShopping);
}

QScrollArea* WishlistPage::createStyledScrollArea() {
    QScrollArea* scrollArea = new QScrollArea;
    scrollArea->setWidgetResizable(true);
    scrollArea->setFrameShape(QFrame::NoFrame);
    scrollArea->setStyleSheet(
        "QScrollArea { border: none; }"
        "QScrollBar:vertical {"
        "    border: none;"
        "    background: #F0F0F0;"
        "    width: 8px;"
        "    margin: 0px;"
        "}"
        "QScrollBar::handle:vertical {"
        "    background: " + sageGreen + ";"
        "    border-radius: 4px;"
        "}"
        "QScrollBar::up-button:vertical, QScrollBar::down-button:vertical {"
        "    border: none;"
        "    background: none;"
        "}"
    );
    return scrollArea;
}

QPushButton* WishlistPage::createStyledButton(const QString& text, bool isPrimary) {
    QPushButton* button = new QPushButton(text);
    button->setStyleSheet(
        QString("QPushButton {"
        "    background-color: %1;"
        "    color: %2;"
        "    padding: 15px 30px;"
        "    border-radius: 25px;"
        "    font-size: 16px;"
        "    font-weight: bold;"
        "    min-width: 200px;"
        "}"
        "QPushButton:hover {"
        "    background-color: " + darkBlue + ";"
        "    color: white;"
        "}").arg(isPrimary ? sageGreen : "white", isPrimary ? "white" : darkBlue)
    );
    return button;
}

QWidget* WishlistPage::createWishlistItem(const Textbook& book) {
    QWidget* itemWidget = new QWidget;
    itemWidget->setStyleSheet(
        "QWidget {"
        "    background-color: white;"
        "    border: 1px solid #E0E0E0;"
        "    border-radius: 15px;"
        "    padding: 20px;"
        "}"
    );

    QHBoxLayout* layout = new QHBoxLayout(itemWidget);
    layout->setSpacing(20);

    // Image
    QLabel* imageLabel = new QLabel;
    QPixmap image(book.getImagePath());
    if (!image.isNull()) {
        imageLabel->setPixmap(image.scaled(100, 120, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }
    imageLabel->setFixedSize(100, 120);
    imageLabel->setStyleSheet(
        "border: 1px solid #E0E0E0;"
        "border-radius: 10px;"
        "padding: 5px;"
    );

    // Info section
    QVBoxLayout* infoLayout = new QVBoxLayout;
    
    QLabel* titleLabel = new QLabel(book.title);
    titleLabel->setStyleSheet(
        "font-weight: bold;"
        "font-size: 16px;"
        "color: " + darkBlue + ";"
    );
    titleLabel->setWordWrap(true);
    
    QLabel* detailsLabel = new QLabel(
        QString("%1 - %2 %3 (LEC: %4)")
            .arg(book.department)
            .arg(book.courseCategory)
            .arg(book.courseCode)
            .arg(book.lec)
    );
    detailsLabel->setStyleSheet("color: #666;");
    
    QLabel* priceLabel = new QLabel(QString("$%1").arg(book.price, 0, 'f', 2));
    priceLabel->setStyleSheet(
        "font-weight: bold;"
        "font-size: 18px;"
        "color: " + sageGreen + ";"
    );
    
    infoLayout->addWidget(titleLabel);
    infoLayout->addWidget(detailsLabel);
    infoLayout->addWidget(priceLabel);
    infoLayout->addStretch();

    // Buttons section
    QVBoxLayout* buttonsLayout = new QVBoxLayout;
    buttonsLayout->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    
    QPushButton* moveToCartButton = new QPushButton("Move to Cart");
    moveToCartButton->setStyleSheet(
        "QPushButton {"
        "    background-color: " + sageGreen + ";"
        "    color: white;"
        "    padding: 8px 15px;"
        "    border-radius: 15px;"
        "    font-weight: bold;"
        "}"
        "QPushButton:hover {"
        "    background-color: " + darkBlue + ";"
        "}"
    );

    QPushButton* removeButton = new QPushButton("Remove");
    removeButton->setStyleSheet(
        "QPushButton {"
        "    color: #FF5252;"
        "    border: none;"
        "    background: none;"
        "    padding: 5px;"
        "}"
        "QPushButton:hover {"
        "    text-decoration: underline;"
        "}"
    );

    buttonsLayout->addWidget(moveToCartButton);
    buttonsLayout->addWidget(removeButton);
    buttonsLayout->addStretch();

    layout->addWidget(imageLabel);
    layout->addLayout(infoLayout, 1);
    layout->addLayout(buttonsLayout);

    // Add shadow effect
    QGraphicsDropShadowEffect* shadow = new QGraphicsDropShadowEffect;
    shadow->setBlurRadius(10);
    shadow->setColor(QColor(0, 0, 0, 30));
    shadow->setOffset(0, 2);
    itemWidget->setGraphicsEffect(shadow);

    // Connect buttons
    connect(moveToCartButton, &QPushButton::clicked, 
            [this, book]() { handleMoveToCart(book.productId); });
    connect(removeButton, &QPushButton::clicked, 
            [this, book]() { handleRemoveItem(book.productId); });

    return itemWidget;
}

void WishlistPage::refreshWishlist() {
    // Clear existing items
    QLayoutItem* item;
    while ((item = wishlistItemsLayout->takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }

    auto wishlistItems = dbManager->getWishlist(currentUserEmail);
    itemCount = wishlistItems.size();
    
    if (wishlistItems.isEmpty()) {
        QLabel* emptyLabel = new QLabel("Your wishlist is empty");
        emptyLabel->setStyleSheet(
            "color: #666;"
            "font-size: 16px;"
            "padding: 40px;"
        );
        emptyLabel->setAlignment(Qt::AlignCenter);
        wishlistItemsLayout->addWidget(emptyLabel);
    } else {
        for (const auto& book : wishlistItems) {
            wishlistItemsLayout->addWidget(createWishlistItem(book));
        }
    }

    updateItemCount();
}

void WishlistPage::updateItemCount() {
    itemCountLabel->setText(QString("%1 item%2")
        .arg(itemCount)
        .arg(itemCount == 1 ? "" : "s"));
}

void WishlistPage::handleMoveToCart(const QString& productId) {
    if (dbManager->addToCart(currentUserEmail, productId, 1)) {
        dbManager->removeFromWishlist(currentUserEmail, productId);
        refreshWishlist();
        QMessageBox::information(this, "Success", "Item moved to cart!");
    }
}

void WishlistPage::handleRemoveItem(const QString& productId) {
    if (dbManager->removeFromWishlist(currentUserEmail, productId)) {
        refreshWishlist();
    }
}

void WishlistPage::handleContinueShopping() {
    emit continueShoppingClicked();
}