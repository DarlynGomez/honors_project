#include "ui/checkout_window.h"
#include <QRegularExpression>

CheckoutWindow::CheckoutWindow(DatabaseManager* db, const QString& userEmail, double total, QWidget *parent)
    : QWidget(parent), dbManager(db), currentUserEmail(userEmail), orderTotal(total)
{
    setupUI();
    updateProgressBar();
}

void CheckoutWindow::setupUI() {
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    setStyleSheet("background-color: white;");
    
    // Progress bar at top
    QWidget* progressBar = createProgressBar();
    mainLayout->addWidget(progressBar);
    
    // Form stack
    formStack = new QStackedWidget;
    formStack->addWidget(createShippingForm());
    formStack->addWidget(createPaymentForm());
    formStack->addWidget(createOrderSummary());
    mainLayout->addWidget(formStack);
    
    // Navigation buttons
    QHBoxLayout* buttonLayout = new QHBoxLayout;
    
    backButton = new QPushButton("Back");
    nextButton = new QPushButton("Next");
    
    QString buttonStyle = 
        "QPushButton {"
        "    padding: 10px 20px;"
        "    border-radius: 20px;"
        "    font-weight: bold;"
        "    font-size: 14px;"
        "}";
        
    backButton->setStyleSheet(buttonStyle + 
        "QPushButton {"
        "    background-color: " + lightGrey + ";"
        "    color: " + darkBlue + ";"
        "}"
        "QPushButton:hover {"
        "    background-color: " + darkGrey + ";"
        "    color: white;"
        "}"
    );
    
    nextButton->setStyleSheet(buttonStyle + 
        "QPushButton {"
        "    background-color: " + sageGreen + ";"
        "    color: white;"
        "}"
        "QPushButton:hover {"
        "    background-color: " + darkBlue + ";"
        "}"
    );
    
    buttonLayout->addWidget(backButton);
    buttonLayout->addWidget(nextButton);
    mainLayout->addLayout(buttonLayout);
    
    connect(backButton, &QPushButton::clicked, this, &CheckoutWindow::previousStep);
    connect(nextButton, &QPushButton::clicked, this, &CheckoutWindow::nextStep);
}

QWidget* CheckoutWindow::createProgressBar() {
    QWidget* container = new QWidget;
    QHBoxLayout* layout = new QHBoxLayout(container);
    
    QStringList steps = {"Shipping", "Payment", "Review"};
    
    for (int i = 0; i < steps.size(); ++i) {
        QLabel* step = new QLabel(steps[i]);
        stepLabels.append(step);
        layout->addWidget(step);
        
        if (i < steps.size() - 1) {
            QFrame* line = new QFrame;
            line->setFrameShape(QFrame::HLine);
            line->setFixedWidth(50);
            stepLines.append(line);
            layout->addWidget(line);
        }
    }
    
    return container;
}

QWidget* CheckoutWindow::createShippingForm() {
    QWidget* container = new QWidget;
    QVBoxLayout* layout = new QVBoxLayout(container);
    
    QLabel* title = new QLabel("Shipping Information");
    title->setStyleSheet(
        "font-size: 24px;"
        "font-weight: bold;"
        "color: " + darkBlue + ";"
        "margin-bottom: 20px;"
    );
    layout->addWidget(title);
    
    QString inputStyle = 
        "QLineEdit {"
        "    padding: 12px;"
        "    border: 2px solid " + lightGrey + ";"
        "    border-radius: 8px;"
        "    font-size: 14px;"
        "}"
        "QLineEdit:focus {"
        "    border-color: " + sageGreen + ";"
        "}";
    
    nameInput = new QLineEdit;
    nameInput->setPlaceholderText("Full Name");
    nameInput->setStyleSheet(inputStyle);
    
    addressInput = new QLineEdit;
    addressInput->setPlaceholderText("Street Address");
    addressInput->setStyleSheet(inputStyle);
    
    cityInput = new QLineEdit;
    cityInput->setPlaceholderText("City");
    cityInput->setStyleSheet(inputStyle);
    
    stateInput = new QLineEdit;
    stateInput->setPlaceholderText("State");
    stateInput->setStyleSheet(inputStyle);
    
    zipInput = new QLineEdit;
    zipInput->setPlaceholderText("ZIP Code");
    zipInput->setStyleSheet(inputStyle);
    
    phoneInput = new QLineEdit;
    phoneInput->setPlaceholderText("Phone Number");
    phoneInput->setStyleSheet(inputStyle);
    
    layout->addWidget(nameInput);
    layout->addWidget(addressInput);
    
    QHBoxLayout* cityStateLayout = new QHBoxLayout;
    cityStateLayout->addWidget(cityInput);
    cityStateLayout->addWidget(stateInput);
    cityStateLayout->addWidget(zipInput);
    layout->addLayout(cityStateLayout);
    
    layout->addWidget(phoneInput);
    layout->addStretch();
    
    return container;
}

QWidget* CheckoutWindow::createPaymentForm() {
    QWidget* container = new QWidget;
    QVBoxLayout* layout = new QVBoxLayout(container);
    
    QLabel* title = new QLabel("Payment Information");
    title->setStyleSheet(
        "font-size: 24px;"
        "font-weight: bold;"
        "color: " + darkBlue + ";"
        "margin-bottom: 20px;"
    );
    layout->addWidget(title);
    
    QString inputStyle = 
        "QLineEdit {"
        "    padding: 12px;"
        "    border: 2px solid " + lightGrey + ";"
        "    border-radius: 8px;"
        "    font-size: 14px;"
        "}"
        "QLineEdit:focus {"
        "    border-color: " + sageGreen + ";"
        "}";
    
    cardNumberInput = new QLineEdit;
    cardNumberInput->setPlaceholderText("Card Number");
    cardNumberInput->setStyleSheet(inputStyle);
    
    cardNameInput = new QLineEdit;
    cardNameInput->setPlaceholderText("Name on Card");
    cardNameInput->setStyleSheet(inputStyle);
    
    expiryInput = new QLineEdit;
    expiryInput->setPlaceholderText("MM/YY");
    expiryInput->setStyleSheet(inputStyle);
    
    cvvInput = new QLineEdit;
    cvvInput->setPlaceholderText("CVV");
    cvvInput->setStyleSheet(inputStyle);
    cvvInput->setMaxLength(4);
    cvvInput->setEchoMode(QLineEdit::Password);
    
    layout->addWidget(cardNumberInput);
    layout->addWidget(cardNameInput);
    
    QHBoxLayout* cardDetailsLayout = new QHBoxLayout;
    cardDetailsLayout->addWidget(expiryInput);
    cardDetailsLayout->addWidget(cvvInput);
    layout->addLayout(cardDetailsLayout);
    
    layout->addStretch();
    
    return container;
}

QWidget* CheckoutWindow::createOrderSummary() {
    QWidget* container = new QWidget;
    QVBoxLayout* layout = new QVBoxLayout(container);
    
    QLabel* title = new QLabel("Order Summary");
    title->setStyleSheet(
        "font-size: 24px;"
        "font-weight: bold;"
        "color: " + darkBlue + ";"
        "margin-bottom: 20px;"
    );
    layout->addWidget(title);
    
    // Display cart items
    QVector<QPair<Textbook, int>> cartItems = dbManager->getCart(currentUserEmail);
    for (const auto& item : cartItems) {
        QWidget* itemWidget = new QWidget;
        QHBoxLayout* itemLayout = new QHBoxLayout(itemWidget);
        
        QLabel* titleLabel = new QLabel(item.first.title);
        QLabel* quantityLabel = new QLabel(QString("x%1").arg(item.second));
        QLabel* priceLabel = new QLabel(
            QString("$%1").arg(item.first.price * item.second, 0, 'f', 2)
        );
        
        itemLayout->addWidget(titleLabel);
        itemLayout->addWidget(quantityLabel);
        itemLayout->addWidget(priceLabel);
        
        layout->addWidget(itemWidget);
    }
    
    QFrame* line = new QFrame;
    line->setFrameShape(QFrame::HLine);
    line->setStyleSheet("background-color: " + lightGrey + ";");
    layout->addWidget(line);
    
    QLabel* totalLabel = new QLabel(QString("Total: $%1").arg(orderTotal, 0, 'f', 2));
    totalLabel->setStyleSheet(
        "font-size: 18px;"
        "font-weight: bold;"
        "color: " + darkBlue + ";"
    );
    totalLabel->setAlignment(Qt::AlignRight);
    layout->addWidget(totalLabel);
    
    // Change next button text on summary page
    nextButton->setText("Place Order");
    
    return container;
}

void CheckoutWindow::updateProgressBar() {
    for (int i = 0; i < stepLabels.size(); ++i) {
        stepLabels[i]->setStyleSheet(
            "color: " + (i <= currentStep ? sageGreen : darkGrey) + ";"
            "font-weight: " + (i == currentStep ? "bold" : "normal") + ";"
        );
    }
    
    for (int i = 0; i < stepLines.size(); ++i) {
        stepLines[i]->setStyleSheet(
            "background-color: " + (i < currentStep ? sageGreen : lightGrey) + ";"
        );
    }
    
    backButton->setEnabled(currentStep > 0);
    nextButton->setText(currentStep == 2 ? "Place Order" : "Next");
}

void CheckoutWindow::nextStep() {
    if (currentStep == 0 && !validateShippingForm()) {
        return;
    }
    if (currentStep == 1 && !validatePaymentForm()) {
        return;
    }
    if (currentStep == 2) {
        processOrder();
        return;
    }
    
    currentStep++;
    formStack->setCurrentIndex(currentStep);
    updateProgressBar();
}

void CheckoutWindow::previousStep() {
    if (currentStep > 0) {
        currentStep--;
        formStack->setCurrentIndex(currentStep);
        updateProgressBar();
    } else {
        emit backToCart();
    }
}

bool CheckoutWindow::validateShippingForm() {
    if (nameInput->text().isEmpty() || 
        addressInput->text().isEmpty() ||
        cityInput->text().isEmpty() ||
        stateInput->text().isEmpty() ||
        zipInput->text().isEmpty() ||
        phoneInput->text().isEmpty()) {
        
        QMessageBox::warning(this, "Validation Error", 
            "Please fill in all required fields.");
        return false;
    }
    return true;
}

bool CheckoutWindow::validatePaymentForm() {
    if (!validateCardNumber(cardNumberInput->text())) {
        QMessageBox::warning(this, "Validation Error", 
            "Please enter a valid card number.");
        return false;
    }
    
    if (cardNameInput->text().isEmpty()) {
        QMessageBox::warning(this, "Validation Error", 
            "Please enter the name on the card.");
        return false;
    }
    
    if (!validateExpiryDate(expiryInput->text())) {
        QMessageBox::warning(this, "Validation Error", 
            "Please enter a valid expiry date (MM/YY).");
        return false;
    }
    
    if (cvvInput->text().length() < 3) {
        QMessageBox::warning(this, "Validation Error", 
            "Please enter a valid CVV code.");
        return false;
    }
    
    return true;
}

bool CheckoutWindow::validateCardNumber(const QString& number) {
    // Create a regular expression pattern to match non-digits
    QRegularExpression regex("\\D");
    
    // Remove non-digits from the card number
    QString cleanNumber = number;
    cleanNumber.remove(regex);
    
    if (cleanNumber.length() < 13 || cleanNumber.length() > 19) {
        return false;
    }
    
    // Rest of the Luhn algorithm remains the same...
    int sum = 0;
    bool alternate = false;
    for (int i = cleanNumber.length() - 1; i >= 0; i--) {
        int n = cleanNumber[i].digitValue();
        if (alternate) {
            n *= 2;
            if (n > 9) {
                n = (n % 10) + 1;
            }
        }
        sum += n;
        alternate = !alternate;
    }
    
    return (sum % 10 == 0);
}

bool CheckoutWindow::validateExpiryDate(const QString& date) {
    QRegularExpression re("^(0[1-9]|1[0-2])/([0-9]{2})$");
    QRegularExpressionMatch match = re.match(date);
    
    if (!match.hasMatch()) {
        return false;
    }
    
    int month = match.captured(1).toInt();
    int year = match.captured(2).toInt() + 2000;
    
    QDate expiryDate(year, month, 1);
    QDate currentDate = QDate::currentDate();
    
    return expiryDate > currentDate;
}

void CheckoutWindow::processOrder() {
    // Show processing animation
    QProgressDialog progress("Processing payment...", QString(), 0, 0, this);
    progress.setWindowModality(Qt::WindowModal);
    progress.setMinimumDuration(0);
    progress.show();

    // Simulate payment processing
    QTimer::singleShot(2000, this, [this, &progress]() {
        progress.close();
        
        // Clear cart and show success message
        QMessageBox::information(this, "Order Confirmed", 
            "Thank you for your order!\n\n"
            "A confirmation email has been sent to your address.");
            
        emit orderComplete();
    });
}