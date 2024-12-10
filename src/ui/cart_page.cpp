#include "ui/cart_page.h"
#include <QScrollArea>
#include <QGraphicsDropShadowEffect>
#include <QMessageBox>

CartPage::CartPage(DatabaseManager* db, const QString& userEmail, QWidget *parent)
    : QWidget(parent), dbManager(db), currentUserEmail(userEmail), cartTotal(0.0), itemCount(0)
{
    setupUI();
    refreshCart();
}

void CartPage::setUserEmail(const QString& email) {
    currentUserEmail = email;
    refreshCart();
}

void CartPage::setupUI() {
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
    
    QLabel* title = new QLabel("Shopping Cart");
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

    // Cart items
    QScrollArea* scrollArea = createStyledScrollArea();
    QWidget* scrollContent = new QWidget;
    cartItemsLayout = new QVBoxLayout(scrollContent);
    cartItemsLayout->setSpacing(15);
    scrollArea->setWidget(scrollContent);
    cardLayout->addWidget(scrollArea);

    // Total and checkout section
    QWidget* bottomSection = new QWidget;
    QVBoxLayout* bottomLayout = new QVBoxLayout(bottomSection);
    bottomLayout->setSpacing(20);
    
    // Summary widget
    QWidget* summaryWidget = new QWidget;
    summaryWidget->setStyleSheet(
        "background-color: " + lightSage + ";"
        "border-radius: 10px;"
        "padding: 20px;"
    );
    QVBoxLayout* summaryLayout = new QVBoxLayout(summaryWidget);
    
    totalLabel = new QLabel;
    totalLabel->setStyleSheet(
        "font-size: 24px;"
        "font-weight: bold;"
        "color: " + darkBlue + ";"
    );
    
    summaryLayout->addWidget(totalLabel);
    bottomLayout->addWidget(summaryWidget);
    
    // Button section
    QHBoxLayout* buttonLayout = new QHBoxLayout;
    buttonLayout->setSpacing(20);
    
    QPushButton* continueButton = createStyledButton("Continue Shopping", false);
    QPushButton* checkoutButton = createStyledButton("Proceed to Checkout", true);
    
    buttonLayout->addWidget(continueButton);
    buttonLayout->addWidget(checkoutButton);
    bottomLayout->addLayout(buttonLayout);
    
    cardLayout->addWidget(bottomSection);
    mainLayout->addWidget(cardWidget);
    
    // Connect buttons
    connect(continueButton, &QPushButton::clicked, this, &CartPage::handleContinueShopping);
    connect(checkoutButton, &QPushButton::clicked, this, &CartPage::handleCheckout);
}

QScrollArea* CartPage::createStyledScrollArea() {
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

QPushButton* CartPage::createStyledButton(const QString& text, bool isPrimary) {
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

QWidget* CartPage::createCartItem(const Textbook& book, int quantity) {
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
    
    infoLayout->addWidget(titleLabel);
    infoLayout->addWidget(detailsLabel);
    infoLayout->addStretch();

    // Price and controls section
    QVBoxLayout* controlsLayout = new QVBoxLayout;
    controlsLayout->setAlignment(Qt::AlignRight);
    
    QLabel* priceLabel = new QLabel(QString("$%1").arg(book.price * quantity, 0, 'f', 2));
    priceLabel->setStyleSheet(
        "font-weight: bold;"
        "font-size: 18px;"
        "color: " + sageGreen + ";"
    );
    
    QHBoxLayout* quantityLayout = new QHBoxLayout;
    QLabel* quantityLabel = new QLabel("Quantity:");
    quantityLabel->setStyleSheet("color: #666;");
    
    QSpinBox* quantityBox = new QSpinBox;
    quantityBox->setRange(1, 99);
    quantityBox->setValue(quantity);
    quantityBox->setStyleSheet(
        "QSpinBox {"
        "    border: 1px solid #E0E0E0;"
        "    border-radius: 4px;"
        "    padding: 5px;"
        "}"
    );
    
    quantityLayout->addWidget(quantityLabel);
    quantityLayout->addWidget(quantityBox);

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

    controlsLayout->addWidget(priceLabel);
    controlsLayout->addLayout(quantityLayout);
    controlsLayout->addWidget(removeButton);
    controlsLayout->addStretch();

    layout->addWidget(imageLabel);
    layout->addLayout(infoLayout, 1);
    layout->addLayout(controlsLayout);

    connect(quantityBox, QOverload<int>::of(&QSpinBox::valueChanged), 
            [=](int value) { handleQuantityChange(book.productId, value); });
    connect(removeButton, &QPushButton::clicked, 
            [=]() { handleRemoveItem(book.productId); });

    // Add shadow effect
    QGraphicsDropShadowEffect* shadow = new QGraphicsDropShadowEffect;
    shadow->setBlurRadius(10);
    shadow->setColor(QColor(0, 0, 0, 30));
    shadow->setOffset(0, 2);
    itemWidget->setGraphicsEffect(shadow);

    return itemWidget;
}

void CartPage::refreshCart() {
    // Clear existing items
    QLayoutItem* item;
    while ((item = cartItemsLayout->takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }

    auto cartItems = dbManager->getCart(currentUserEmail);
    itemCount = 0;
    
    if (cartItems.isEmpty()) {
        QLabel* emptyLabel = new QLabel("Your cart is empty");
        emptyLabel->setStyleSheet(
            "color: #666;"
            "font-size: 16px;"
            "padding: 40px;"
        );
        emptyLabel->setAlignment(Qt::AlignCenter);
        cartItemsLayout->addWidget(emptyLabel);
    } else {
        for (const auto& pair : cartItems) {
            cartItemsLayout->addWidget(createCartItem(pair.first, pair.second));
            itemCount += pair.second;
        }
    }

    updateTotal();
    updateItemCount();
}

void CartPage::updateTotal() {
    auto cartItems = dbManager->getCart(currentUserEmail);
    cartTotal = 0.0;
    
    for (const auto& pair : cartItems) {
        cartTotal += pair.first.price * pair.second;
    }
    
    totalLabel->setText(QString("Total: $%1").arg(cartTotal, 0, 'f', 2));
}

void CartPage::updateItemCount() {
    itemCountLabel->setText(QString("%1 item%2")
        .arg(itemCount)
        .arg(itemCount == 1 ? "" : "s"));
}

void CartPage::handleQuantityChange(const QString& productId, int quantity) {
    dbManager->updateCartQuantity(currentUserEmail, productId, quantity);
    refreshCart();
}

void CartPage::handleRemoveItem(const QString& productId) {
    dbManager->removeFromCart(currentUserEmail, productId);
    refreshCart();
}

void CartPage::handleCheckout() {
    if (cartTotal == 0) {
        QMessageBox::information(this, "Cart Empty", 
            "Your cart is empty. Add some items before checking out.");
        return;
    }

    QMessageBox::StandardButton reply = QMessageBox::question(this, 
        "Confirm Checkout", 
        QString("Proceed to checkout? Total amount: $%1").arg(cartTotal, 0, 'f', 2),
        QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        // Clear cart and show success message
        // In a real app, this would process payment
        QMessageBox::information(this, "Success", 
            "Thank you for your purchase! Your order has been confirmed.");
        emit checkoutCompleted();
    }
}

void CartPage::handleContinueShopping() {
    this->hide();
}