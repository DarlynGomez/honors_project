#include "../include/ui/registration_window.h"
#include <QVBoxLayout>  // Arranges widgets in a vertical column
#include <QMessageBox>  // Creates simple dialog boxes for displaying messages


// Define functions from RegistrationWindow class

// Register Window INtentional constructor, connections and UI is set up
RegistrationWindow::RegistrationWindow(Authenticator* auth, QWidget *parent)
   : QWidget(parent), authenticator(auth)
{
   setupUI();  // Set up the user interface
   setupConnections();  // Create my signal-slot connections
}

// Set up UI structure and widgets
void RegistrationWindow::setupUI() {
    // Main layout for the my login window
    auto mainLayout = new QVBoxLayout(this);
    
    // Define color scheme
    QString sageGreen = "#9CAF88";  // Sage green
    QString darkBlue = "#2C3E50";   // Dark blue
    QString lightSage = "#E8F0E3";  // Light sage for hover effects
    
    // Create a container widget for the registration form
    // Uses shadow effect
    auto formContainer = new QWidget(this);
    auto formLayout = new QVBoxLayout(formContainer);  // Layout for my widgets inside the container
    formContainer->setFixedWidth(450); // Fixes width of container to 450 pixels
    formContainer->setStyleSheet(
        "QWidget {"
        "   background-color: white;"  // Background is white
        "   border-radius: 15px;"   // Rounded container corners
        "   border: 1px solid #E0E0E0;"  // Solid gray border
        "   outline: none;"  // Removes ugly outline
        "}"
        "QLabel {"
        "   background: transparent;"  // Transparent background for my text
        "   border: none;"  // removed borders
        "}"
        "QPushButton {"
        "   outline: none;"  // Removes ugly outline from buttons
        "}"
    );
  
    // Adds a drop shadow effect to the form container
    QGraphicsDropShadowEffect* shadowEffect = new QGraphicsDropShadowEffect;
    shadowEffect->setBlurRadius(20);  // Makes shadow not harsh
    shadowEffect->setColor(QColor(0, 0, 0, 50));  // Black Shadow color, 50% opacity
    shadowEffect->setOffset(0, 5);  // Offset for shadow
    formContainer->setGraphicsEffect(shadowEffect);  // Applies shadow to container

    // Sets widget allignment and spacing
    formLayout->setAlignment(Qt::AlignCenter);
    formLayout->setSpacing(15);  // Vertical margin
    formLayout->setContentsMargins(30, 30, 30, 30);  // Padding inside container
    
    // Title with font and styling
    auto titleLabel = new QLabel("Create Your Account", this);
    titleLabel->setStyleSheet(
        "font-size: 32px;"
        "font-weight: bold;"
        "color: " + darkBlue + ";"
        "margin: 20px;"
        "font-family: -apple-system, BlinkMacSystemFont;"
    );
    titleLabel->setAlignment(Qt::AlignCenter);
    
    // Subtitle
    auto subtitleLabel = new QLabel("Join BMCC E-Store Community", this);
    subtitleLabel->setStyleSheet(
        "font-size: 16px;"
        "color: #666;"
        "margin-bottom: 20px;"
    );
    subtitleLabel->setAlignment(Qt::AlignCenter);  // Center aligned
    
    // Input fields styling
    QString inputStyle =
        "QLineEdit {"
        "   padding: 12px;"
        "   font-size: 14px;"
        "   border: 2px solid #E0E0E0;"
        "   border-radius: 8px;"
        "   background-color: white;"
        "   outline: none;"
        "}"
        "QLineEdit:focus {"
        "   border: 2px solid " + sageGreen + ";"
        "   background-color: " + lightSage + ";"
        "   outline: none;"
        "}";
    
    // Email input field
    auto emailLabel = new QLabel("Email Address", this);
    emailLabel->setStyleSheet("font-weight: bold; color: " + darkBlue + ";");
    emailInput = new QLineEdit(this);
    emailInput->setPlaceholderText("your.email@stu.bmcc.cuny.edu");
    emailInput->setMinimumWidth(350);
    emailInput->setStyleSheet(inputStyle);  // Apply my defined input style
    
    // Password input field
    auto passwordLabel = new QLabel("Create Password", this);
    passwordLabel->setStyleSheet("font-weight: bold; color: " + darkBlue + ";");
    passwordInput = new QLineEdit(this);
    passwordInput->setPlaceholderText("Enter your password");
    passwordInput->setEchoMode(QLineEdit::Password);
    passwordInput->setStyleSheet(inputStyle);  // Apply my defined input style
    
    // Confirm password input field
    auto confirmPasswordLabel = new QLabel("Confirm Password", this);
    confirmPasswordLabel->setStyleSheet("font-weight: bold; color: " + darkBlue + ";");
    confirmPasswordInput = new QLineEdit(this);
    confirmPasswordInput->setPlaceholderText("Confirm your password");
    confirmPasswordInput->setEchoMode(QLineEdit::Password);
    confirmPasswordInput->setStyleSheet(inputStyle);
    
    // Password requirements text box
    auto requirementsLabel = new QLabel(
        "Password must contain:\n"
        "• At least 8 characters\n"
        "• One uppercase letter\n"
        "• One number",
        this
    );
    requirementsLabel->setStyleSheet(
        "color: #666;"
        "font-size: 12px;"
        "padding: 10px;"
        "background-color: " + lightSage + ";"
        "border-radius: 5px;"
    );
    
    // Register button with animation effect and hover
    registerButton = new QPushButton("Create Account", this);
    registerButton->setStyleSheet(
        "QPushButton {"
        "   background-color: " + sageGreen + ";"
        "   color: white;"
        "   padding: 12px;"
        "   font-size: 16px;"
        "   font-weight: bold;"
        "   border-radius: 8px;"
        "   min-width: 200px;"
        "   border: none;"
        "   transition: all 0.3s ease-in-out;"
        "}"
        "QPushButton:hover {"
        "   background-color: " + darkBlue + ";"
        "   transform: scale(1.05);"
        "}"
        "QPushButton:pressed {"
        "   background-color: " + sageGreen + ";"
        "   transform: scale(0.95);"
        "}"
    );
    
   // Login button with hover
    loginButton = new QPushButton("Already have an account? Sign in", this);
    loginButton->setStyleSheet(
        "QPushButton {"
        "   color: " + darkBlue + ";"
        "   text-decoration: none;"
        "   font-size: 14px;"
        "   border: none;"
        "   background: transparent;"
        "   padding: 5px;"
        "}"
        "QPushButton:hover {"
        "   color: " + sageGreen + ";"
        "   text-decoration: underline;"
        "}"
    );
    loginButton->setCursor(Qt::PointingHandCursor);   // Adds hand cursor onhover
    
    // Status label for error messages
    statusLabel = new QLabel(this);
    statusLabel->setStyleSheet(
        "color: #FF5252;"
        "font-size: 14px;"
        "padding: 10px;"
        "border-radius: 5px;"
    );
    statusLabel->setAlignment(Qt::AlignCenter);
    
    // Add widgets to form layout with proper spacing
    formLayout->addWidget(titleLabel, 0, Qt::AlignCenter);  // Adds my title
    formLayout->addWidget(subtitleLabel, 0, Qt::AlignCenter);  // Adds my subtitle
    formLayout->addSpacing(20);  // Adds spacing
    
    formLayout->addWidget(emailLabel);  // Adds my email text label
    formLayout->addWidget(emailInput);  // Adds email input box
    formLayout->addSpacing(10);  // Adds spacing
    
    formLayout->addWidget(passwordLabel);  // Adds my password text label
    formLayout->addWidget(passwordInput);  // Adds my password input
    formLayout->addSpacing(10);  // Adds spacing
    
    formLayout->addWidget(confirmPasswordLabel);  // Adds my confirm password text label
    formLayout->addWidget(confirmPasswordInput);  // Adds my confirm password input
    formLayout->addSpacing(5);  // Adds spacing
    
    formLayout->addWidget(requirementsLabel);  // Adds my password requirements text label
    formLayout->addSpacing(20);  // Adds spacing
    
    formLayout->addWidget(registerButton, 0, Qt::AlignCenter);   // Adds register button
    formLayout->addSpacing(10);  // Adds spacing
    formLayout->addWidget(statusLabel);  // Adds status label for errors/success
    formLayout->addWidget(loginButton, 0, Qt::AlignCenter);   // Adds login button
    
    // Center the form in the main layout with margins
    mainLayout->addWidget(formContainer, 0, Qt::AlignCenter);   // Adds form container to main layout
    setLayout(mainLayout);  // Sets the main layout
    
    // Add fade-in animation for the form
    QGraphicsOpacityEffect* opacity = new QGraphicsOpacityEffect(formContainer);
    formContainer->setGraphicsEffect(opacity);  // Set my opacity effect
    QPropertyAnimation* fadeIn = new QPropertyAnimation(opacity, "opacity");   // Creates fade in animation
    fadeIn->setDuration(500);  // Duration of animation fade in
    fadeIn->setStartValue(0);  // Initial opacity
    fadeIn->setEndValue(1);   // End Opacity 
    fadeIn->start(QAbstractAnimation::DeleteWhenStopped);  // Delete animation after it ends
}

// Function to set up connections between signals and slots
void RegistrationWindow::setupConnections() {
    connect(registerButton, &QPushButton::clicked,
            this, &RegistrationWindow::handleRegistration);
    connect(loginButton, &QPushButton::clicked,
            this, &RegistrationWindow::switchToLogin);
    connect(confirmPasswordInput, &QLineEdit::textChanged,
            this, &RegistrationWindow::validatePasswordMatch);
}

// Handles register button clicks
void RegistrationWindow::handleRegistration() {
    QString email = emailInput->text();    // User-inputted email
    QString password = passwordInput->text();    // User-inputted password
    QString confirmPassword = confirmPasswordInput->text();   // User inputter confierm pass
    
    if (password != confirmPassword) {
        statusLabel->setText("Passwords do not match");
        return;
    }
    
    QString errorMsg;
    if (authenticator->registerUser(email, password, errorMsg)) {
        QMessageBox::information(this, "Success",
            "Registration successful! Please login with your credentials.");
        emit registrationSuccessful();     // Emits the registration successful signal .
    } else {
        statusLabel->setText(errorMsg);
    }
}

// Tells application to switch to login view
void RegistrationWindow::switchToLogin() {
    emit switchToLoginWindow();
}

// Validate that inputted passwords match
void RegistrationWindow::validatePasswordMatch() {
   if (passwordInput->text() == confirmPasswordInput->text()) {
       statusLabel->clear();
   } else {
       statusLabel->setText("Passwords do not match");
   }
}

