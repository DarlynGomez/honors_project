#ifndef LOGIN_WINDOW_H
#define LOGIN_WINDOW_H


#include <QWidget> // Qt Clas to create UI objects
#include <QLineEdit> // Creates single line input bo
#include <QPushButton> // Gives clickable button
#include <QLabel> // Displays static, dynamic text, or images
#include <QVBoxLayout> // Arranges widgets in a vertical column
#include "../auth/authenticator.h"


#include <QGraphicsEffect>
#include <QPropertyAnimation>
#include <QGraphicsDropShadowEffect>


class QGraphicsDropShadowEffect;
class QGraphicsOpacityEffect;
class QPropertyAnimation;


// My login in UI page


// Create login window class that inherits formt QWidget Class
// QWidget is the base class for all QT user interface objects
class LoginWindow : public QWidget {
   Q_OBJECT // QT Signal Slot mechanism


   public:
   // Class constructor that is only called intentionally
   explicit LoginWindow(Authenticator* auth, QWidget *parent = nullptr);


   // My signal responders
   private slots:
       void handleLoginButton();   // login buttin clicked event
       void switchToRegistration();    // Register button clicked event


   // My Signals - Emitted after specifc events occurs
   signals:
       // Signal emitted after succesful login
       void loginSuccessful(const QString& email);
       // Signal emitted after register button is clicked
       void switchToRegister();


   private:
       // UI Elements
       QLineEdit* emailInput;  // email input field
       QLineEdit* passwordInput;   // password input field
       QPushButton* loginButton;   // Login Button
       QPushButton* registerButton;    // Register Button
       QLabel* statusLabel;    // Status Update for login success/failure
      
       // Pointer to Authentication object to handle authentication login process
       Authenticator* authenticator;
      
       void setupUI(); // Initializes and arranges my widgets
       void setupConnections();    // Establishes connectionbetween signals and slots
};


#endif













