#include "../include/ui/registration_window.h"
#include <QVBoxLayout>
#include <QMessageBox>


RegistrationWindow::RegistrationWindow(Authenticator* auth, QWidget *parent)
   : QWidget(parent), authenticator(auth)
{
   setupUI();
   setupConnections();
}

void RegistrationWindow::setupUI() {
   auto mainLayout = new QVBoxLayout(this);
  
   // Define color scheme
   QString sageGreen = "#9CAF88";  // Sage green
   QString darkBlue = "#2C3E50";   // Dark blue
   QString lightSage = "#E8F0E3";  // Light sage for hover effects
  
   // Create a container widget for the registration form
   auto formContainer = new QWidget(this);
   auto formLayout = new QVBoxLayout(formContainer);
   formContainer->setFixedWidth(450);  // Slightly wider for registration
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
   formLayout->setContentsMargins(30, 30, 30, 30);  // Padding inside container
  
   // Title with custom font and styling
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
   auto passwordLabel = new QLabel("Create Password", this);
   passwordLabel->setStyleSheet("font-weight: bold; color: " + darkBlue + ";");
   passwordInput = new QLineEdit(this);
   passwordInput->setPlaceholderText("Enter your password");
   passwordInput->setEchoMode(QLineEdit::Password);
   passwordInput->setStyleSheet(inputStyle);
  
   // Confirm password input
   auto confirmPasswordLabel = new QLabel("Confirm Password", this);
   confirmPasswordLabel->setStyleSheet("font-weight: bold; color: " + darkBlue + ";");
   confirmPasswordInput = new QLineEdit(this);
   confirmPasswordInput->setPlaceholderText("Confirm your password");
   confirmPasswordInput->setEchoMode(QLineEdit::Password);
   confirmPasswordInput->setStyleSheet(inputStyle);
  
   // Password requirements
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
  
   // Register button with animation effect
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
  
   // Login link button
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
   loginButton->setCursor(Qt::PointingHandCursor);
  
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
   formLayout->addSpacing(10);
  
   formLayout->addWidget(confirmPasswordLabel);
   formLayout->addWidget(confirmPasswordInput);
   formLayout->addSpacing(5);
  
   formLayout->addWidget(requirementsLabel);
   formLayout->addSpacing(20);
  
   formLayout->addWidget(registerButton, 0, Qt::AlignCenter);
   formLayout->addSpacing(10);
   formLayout->addWidget(statusLabel);
   formLayout->addWidget(loginButton, 0, Qt::AlignCenter);
  
   // Center the form in the main layout with margins
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


void RegistrationWindow::setupConnections() {
   connect(registerButton, &QPushButton::clicked,
           this, &RegistrationWindow::handleRegistration);
   connect(loginButton, &QPushButton::clicked,
           this, &RegistrationWindow::switchToLogin);
   connect(confirmPasswordInput, &QLineEdit::textChanged,
           this, &RegistrationWindow::validatePasswordMatch);
}


void RegistrationWindow::handleRegistration() {
   QString email = emailInput->text();
   QString password = passwordInput->text();
   QString confirmPassword = confirmPasswordInput->text();
  
   if (password != confirmPassword) {
       statusLabel->setText("Passwords do not match");
       return;
   }
  
   QString errorMsg;
   if (authenticator->registerUser(email, password, errorMsg)) {
       QMessageBox::information(this, "Success",
           "Registration successful! Please login with your credentials.");
       emit registrationSuccessful();
   } else {
       statusLabel->setText(errorMsg);
   }
}


void RegistrationWindow::switchToLogin() {
   emit switchToLoginWindow();
}


void RegistrationWindow::validatePasswordMatch() {
   if (passwordInput->text() == confirmPasswordInput->text()) {
       statusLabel->clear();
   } else {
       statusLabel->setText("Passwords do not match");
   }
}

