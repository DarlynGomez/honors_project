#include "ui/login_window.h"
#include <QMessageBox>
#include <QVBoxLayout>
#include <QGraphicsEffect>
#include <QPropertyAnimation>
#include <QGraphicsDropShadowEffect>




// Define functions from LoginWindow class


// Login Window INtentional constructor, connections and UI is set up
LoginWindow::LoginWindow(Authenticator* auth, QWidget *parent)
   : QWidget(parent), authenticator(auth)
{
   setupUI();
   setupConnections();
}


// Set up UI structure and widgets
void LoginWindow::setupUI() {
   auto mainLayout = new QVBoxLayout(this);
  
   // Define color scheme
   QString sageGreen = "#9CAF88";  // Sage green
   QString darkBlue = "#2C3E50";   // Dark blue
   QString lightSage = "#E8F0E3";  // Light sage for hover effects
  
   // Create a container widget for the login form
   auto formContainer = new QWidget(this);
   auto formLayout = new QVBoxLayout(formContainer);
   formContainer->setFixedWidth(450);
   formContainer->setStyleSheet(
       "QWidget {"
       "   background-color: white;"
       "   border-radius: 15px;"
       "   border: 1px solid #E0E0E0;"
       "   outline: none;"
       "}"
       "QLabel {"
       "   background: transparent;"
       "   border: none;"
       "}"
       "QPushButton {"
       "   outline: none;"
       "}"
   );
  
   // Add drop shadow effect to form container
   QGraphicsDropShadowEffect* shadowEffect = new QGraphicsDropShadowEffect;
   shadowEffect->setBlurRadius(20);
   shadowEffect->setColor(QColor(0, 0, 0, 50));
   shadowEffect->setOffset(0, 5);
   formContainer->setGraphicsEffect(shadowEffect);


   formLayout->setAlignment(Qt::AlignCenter);
   formLayout->setSpacing(15);
   formLayout->setContentsMargins(30, 30, 30, 30);
  
   // Title
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
   subtitleLabel->setAlignment(Qt::AlignCenter);
  
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
  
   // Email input
   auto emailLabel = new QLabel("Email Address", this);
   emailLabel->setStyleSheet("font-weight: bold; color: " + darkBlue + ";");
   emailInput = new QLineEdit(this);
   emailInput->setPlaceholderText("your.email@stu.bmcc.cuny.edu");
   emailInput->setMinimumWidth(350);
   emailInput->setStyleSheet(inputStyle);
  
   // Password input
   auto passwordLabel = new QLabel("Password", this);
   passwordLabel->setStyleSheet("font-weight: bold; color: " + darkBlue + ";");
   passwordInput = new QLineEdit(this);
   passwordInput->setPlaceholderText("Enter your password");
   passwordInput->setEchoMode(QLineEdit::Password);
   passwordInput->setStyleSheet(inputStyle);
  
   // Login button with animation effect
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
  
   // Register link button
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
   registerButton->setCursor(Qt::PointingHandCursor);
  
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
   formLayout->addWidget(titleLabel, 0, Qt::AlignCenter);
   formLayout->addWidget(subtitleLabel, 0, Qt::AlignCenter);
   formLayout->addSpacing(20);
  
   formLayout->addWidget(emailLabel);
   formLayout->addWidget(emailInput);
   formLayout->addSpacing(10);
  
   formLayout->addWidget(passwordLabel);
   formLayout->addWidget(passwordInput);
   formLayout->addSpacing(20);
  
   formLayout->addWidget(loginButton, 0, Qt::AlignCenter);
   formLayout->addSpacing(10);
   formLayout->addWidget(statusLabel);
   formLayout->addWidget(registerButton, 0, Qt::AlignCenter);
  
   // Center the form in the main layout
   mainLayout->addWidget(formContainer, 0, Qt::AlignCenter);
   setLayout(mainLayout);
  
   // Add fade-in animation for the form
   QGraphicsOpacityEffect* opacity = new QGraphicsOpacityEffect(formContainer);
   formContainer->setGraphicsEffect(opacity);
   QPropertyAnimation* fadeIn = new QPropertyAnimation(opacity, "opacity");
   fadeIn->setDuration(500);
   fadeIn->setStartValue(0);
   fadeIn->setEndValue(1);
   fadeIn->start(QAbstractAnimation::DeleteWhenStopped);
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







