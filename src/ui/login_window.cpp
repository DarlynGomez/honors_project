#include "ui/login_window.h"
#include <QMessageBox>  // Creates simple dialog boxes for displaying messages
#include <QVBoxLayout>  // Arranges widgets in a vertical column
#include <QGraphicsEffect>  // For modifying my widgets appearence like CSS
#include <QPropertyAnimation>  // Smooth time based transtions for objects
#include <QGraphicsDropShadowEffect>  // Enables drop shadow effects for widgets

// Define functions from LoginWindow class


// Login Window INtentional constructor, connections and UI is set up
LoginWindow::LoginWindow(Authenticator* auth, QWidget *parent)
   : QWidget(parent), authenticator(auth)
{
   setupUI();  // Set up the user interface
   setupConnections();  // Create my signal-slot connections
}


// Set up UI structure and widgets
void LoginWindow::setupUI() {
    // Main layout for the my login window
    auto mainLayout = new QVBoxLayout(this);

    // Define color scheme
    QString sageGreen = "#9CAF88";  // Sage green
    QString darkBlue = "#2C3E50";   // Dark blue
    QString lightSage = "#E8F0E3";  // Light sage for hover effects

    // Creates a container widget for the login form
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
    formLayout->setContentsMargins(30, 30, 30, 30);  // My padding
  
   // Title for form
   auto titleLabel = new QLabel("Welcome Back", this);
   titleLabel->setStyleSheet(
       "font-size: 32px;"
       "font-weight: bold;"
       "color: " + darkBlue + ";"
       "margin: 20px;"
       "font-family: -apple-system, BlinkMacSystemFont;"
   );
   titleLabel->setAlignment(Qt::AlignCenter);
  
   // Subtitle
   auto subtitleLabel = new QLabel("Sign in to BMCC E-Store", this);
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
   auto passwordLabel = new QLabel("Password", this);
   passwordLabel->setStyleSheet("font-weight: bold; color: " + darkBlue + ";");
   passwordInput = new QLineEdit(this);
   passwordInput->setPlaceholderText("Enter your password");
   passwordInput->setEchoMode(QLineEdit::Password);
   passwordInput->setStyleSheet(inputStyle);  // Apply my defined input style
  
   // Login button with animation effect and hover
   loginButton = new QPushButton("Sign In", this);
   loginButton->setStyleSheet(
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
  
   // Register button with animation effect and hover
   registerButton = new QPushButton("New to BMCC E-Store? Create an account", this);
   registerButton->setStyleSheet(
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
   registerButton->setCursor(Qt::PointingHandCursor);  // Adds hand cursor onhover
  
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
   formLayout->addWidget(subtitleLabel, 0, Qt::AlignCenter); // Adds my subtitle
   formLayout->addSpacing(20);  // Adds spacing
  
   formLayout->addWidget(emailLabel);  // Adds my email text label
   formLayout->addWidget(emailInput);  // Adds email input box
   formLayout->addSpacing(10);  // Adds spacing
  
   formLayout->addWidget(passwordLabel);  // Adds my password text label
   formLayout->addWidget(passwordInput);  // Adds my password input
   formLayout->addSpacing(20);   // Adds spacing
  
   formLayout->addWidget(loginButton, 0, Qt::AlignCenter);  // Adds login button
   formLayout->addSpacing(10);  // Adds spacing
   formLayout->addWidget(statusLabel);  // Adds status label for errors/success
   formLayout->addWidget(registerButton, 0, Qt::AlignCenter);  // Adds register button
  
   // Center the form in the main layout
   mainLayout->addWidget(formContainer, 0, Qt::AlignCenter);   // Adds form container to main layout
   setLayout(mainLayout);  // Sets the main layout
  
   // Add fade-in animation for the form's entry
   QGraphicsOpacityEffect* opacity = new QGraphicsOpacityEffect(formContainer);
   formContainer->setGraphicsEffect(opacity);  // Set my opacity effect
   QPropertyAnimation* fadeIn = new QPropertyAnimation(opacity, "opacity");  // Creates fade in animation
   fadeIn->setDuration(500);  // Duration of animation fade in
   fadeIn->setStartValue(0);  // Initial opacity
   fadeIn->setEndValue(1);  // End opacity
   fadeIn->start(QAbstractAnimation::DeleteWhenStopped);  // Delete animation after it ends
}


// Function to set up connections between signals and slots
void LoginWindow::setupConnections() {
   connect(loginButton, &QPushButton::clicked, this, &LoginWindow::handleLoginButton);
   connect(registerButton, &QPushButton::clicked, this, &LoginWindow::switchToRegistration);
  
   // Allow login on Enter key
   connect(emailInput, &QLineEdit::returnPressed, loginButton, &QPushButton::click);
   connect(passwordInput, &QLineEdit::returnPressed, loginButton, &QPushButton::click);
}


// Handles login button clicks
void LoginWindow::handleLoginButton() {
   QString email = emailInput->text();     // User-inputted email
   QString password = passwordInput->text();     // User-inputted password
   QString errorMsg;   // Error messages
  
   if (authenticator->login(email, password, errorMsg)) {
       statusLabel->clear();
       emit loginSuccessful(email);    // Emits the loginSuccessful signal with the email.
   } else {
       statusLabel->setText(errorMsg);
       statusLabel->setStyleSheet("color: red;");
   }
}


// Tells application to switch to registration view
void LoginWindow::switchToRegistration() {
   emit switchToRegister();
}







