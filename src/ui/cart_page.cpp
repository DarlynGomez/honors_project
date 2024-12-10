#include "ui/cart_page.h"
#include <QSpinBox>
#include <QGraphicsDropShadowEffect>
#include <QMessageBox>

CartPage::CartPage(DatabaseManager* db, const QString& userEmail, QWidget* parent)
    : QWidget(parent), dbManager(db), currentUserEmail(userEmail), cartTotal(0.0)
{
    setupUI();
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

    // Title
    QLabel* title = new QLabel("Shopping Cart");
    title->setStyleSheet(
        "font-size: 24px;"
        "font-weight: bold;"
        "color: " + darkBlue + ";"
    );
    cardLayout->addWidget(title);

    // Scrollable cart items
    QScrollArea* scrollArea = new QScrollArea;
    scrollArea->setWidgetResizable(true);
    scrollArea->setFrameShape(QFrame::NoFrame);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollArea->setStyleSheet(
        "QScrollArea { border: none; }"
        "QScrollBar:vertical {"
        "    border: none;"
        "    background: #F0F0F0;"
        "    width: 8px;"
        "    margin: 0px;"
        "}"
    );

    QWidget* scrollContent = new QWidget;
    cartItemsLayout = new QVBoxLayout(scrollContent);
    cartItemsLayout->setSpacing(15);
    scrollArea->setWidget(scrollContent);
    cardLayout->addWidget(scrollArea);

    // Total and checkout section
    QWidget* bottomSection = new QWidget;
    QVBoxLayout* bottomLayout = new QVBoxLayout(bottomSection);
    
    totalLabel = new QLabel;
    totalLabel->setStyleSheet(
        "font-size: 18px;"
        "font-weight: bold;"
        "color: " + darkBlue + ";"
    );
    
    QPushButton* checkoutButton = new QPushButton("Proceed to Checkout");
    checkoutButton->setStyleSheet(
        "QPushButton {"
        "    background-color: " + sageGreen + ";"
        "    color: white;"
        "    padding: 15px 30px;"
        "    border-radius: 25px;"
        "    font-size: 16px;"
        "    font-weight: bold;"
        "}"
        "QPushButton:hover {"
        "    background-color: " + darkBlue + ";"
        "}"
    );
    
    bottomLayout->addWidget(totalLabel);
    bottomLayout->addWidget(checkoutButton);
    cardLayout->addWidget(bottomSection);

    mainLayout->addWidget(cardWidget);
    
    connect(checkoutButton, &QPushButton::clicked, this, &CartPage::handleCheckout);
}

QWidget* CartPage::createCartItem(const Textbook& book, int quantity) {
    QWidget* itemWidget = new QWidget;
    itemWidget->setStyleSheet(
        "QWidget {"
        "    background-color: white;"
        "    border: 1px solid #E0E0E0;"
        "    border-radius: 10px;"
        "    padding: 15px;"
        "}"
    );

    QHBoxLayout* layout = new QHBoxLayout(itemWidget);
    layout->setSpacing(20);

    // Image
    QLabel* imageLabel = new QLabel;
    QPixmap image(book.getImagePath());
    imageLabel->setPixmap(image.scaled(80, 100, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    imageLabel->setFixedSize(80, 100);

    // Info section
    QVBoxLayout* infoLayout = new QVBoxLayout;
    QLabel* titleLabel = new QLabel(book.title);
    titleLabel->setStyleSheet("font-weight: bold; font-size: 16px;");
    QLabel* detailsLabel = new QLabel(
        QString("%1 - %2 %3")
            .arg(book.department)
            .arg(book.courseCategory)
            .arg(book.courseCode)
    );
    infoLayout->addWidget(titleLabel);
    infoLayout->addWidget(detailsLabel);

    // Price and quantity section
    QVBoxLayout* priceLayout = new QVBoxLayout;
    QLabel* priceLabel = new QLabel(QString("$%1").arg(book.price, 0, 'f', 2));
    priceLabel->setStyleSheet("font-weight: bold; color: " + sageGreen + ";");
    
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

    QPushButton* removeButton = new QPushButton("Remove");
    removeButton->setStyleSheet(
        "QPushButton {"
        "    color: #FF5252;"
        "    border: none;"
        "    background: none;"
        "}"
        "QPushButton:hover {"
        "    text-decoration: underline;"
        "}"
    );

    priceLayout->addWidget(priceLabel);
    priceLayout->addWidget(quantityBox);
    priceLayout->addWidget(removeButton);

    layout->addWidget(imageLabel);
    layout->addLayout(infoLayout, 1);
    layout->addLayout(priceLayout);

    // Connect signals
    connect(quantityBox, &QSpinBox::valueChanged, [=](int value) {
        handleQuantityChange(book.productId, value);
    });
    connect(removeButton, &QPushButton::clicked, [=]() {
        handleRemoveItem(book.productId);
    });

    return itemWidget;
}

void CartPage::refreshCart() {
    // Clear existing items
    QLayoutItem* item;
    while ((item = cartItemsLayout->takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }

    // Load cart items
    auto cartItems = dbManager->getCart(currentUserEmail);
    
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
        }
    }

    updateTotal();
}

void CartPage::updateTotal() {
    auto cartItems = dbManager->getCart(currentUserEmail);
    cartTotal = 0.0;
    
    for (const auto& pair : cartItems) {
        cartTotal += pair.first.price * pair.second;
    }
    
    totalLabel->setText(QString("Total: $%1").arg(cartTotal, 0, 'f', 2));
}

void CartPage::handleQuantityChange(const QString& productId, int quantity) {
    dbManager->updateCartQuantity(currentUserEmail, productId, quantity);
    updateTotal();
}

void CartPage::handleRemoveItem(const QString& productId) {
    dbManager->removeFromCart(currentUserEmail, productId);
    refreshCart();
}

void CartPage::handleCheckout() {
    // Implement checkout functionality
    QMessageBox::information(this, "Checkout", "Proceeding to checkout...");
}

