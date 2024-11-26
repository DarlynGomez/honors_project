#ifndef REGISTRATION_WINDOW_H
#define REGISTRATION_WINDOW_H


#include <QWidget> // Qt Clas to create UI objects
#include <QLineEdit> // Creates single line input box
#include <QPushButton> // Gives clickable button
#include <QLabel> // Displays static, dynamic text, or images
#include "../auth/authenticator.h"

#include <QGraphicsEffect>  // For modifying my widgets appearence like CSS
#include <QPropertyAnimation>  // Smooth time based transtions for objects
#include <QGraphicsDropShadowEffect>  // Enables drop shadow effects for widgets


class RegistrationWindow : public QWidget {
   Q_OBJECT // QT Signal Slot mechanism


   public:
      // Class constructor that is only called intentionally
      explicit RegistrationWindow(Authenticator* auth, QWidget *parent = nullptr);

   // My signal responders
   private slots:
      void handleRegistration();   // registration button clicked event
      void switchToLogin();    // Switch to login button clicked event
      void validatePasswordMatch();    // Checks that passwords match 

   // My Signals - Emitted after specifc events occurs
   signals:
      void registrationSuccessful();  // Signal emitted after succesful registration
      void switchToLoginWindow();   // Signal emitted after 'register user' button is clicked


   private:
      // UI Elements
      QLineEdit* emailInput;  // email input field
      QLineEdit* passwordInput;   // password input field
      QLineEdit* confirmPasswordInput;   // Confirm password input field
      QPushButton* registerButton;   // Register Button
      QPushButton* loginButton;   // Login Button
      QLabel* statusLabel;    // Status Update for register success/failure
   
      // Pointer to Authentication object to handle authentication login process
      Authenticator* authenticator;
   
      void setupUI(); // Initializes and arranges my widgets
      void setupConnections();   // Establishes connectionbetween signals and slots
};


#endif

