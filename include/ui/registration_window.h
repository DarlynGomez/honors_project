#ifndef REGISTRATION_WINDOW_H
#define REGISTRATION_WINDOW_H


#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include "../auth/authenticator.h"


#include <QGraphicsEffect>
#include <QPropertyAnimation>
#include <QGraphicsDropShadowEffect>


class RegistrationWindow : public QWidget {
   Q_OBJECT


public:
   explicit RegistrationWindow(Authenticator* auth, QWidget *parent = nullptr);


private slots:
   void handleRegistration();
   void switchToLogin();
   void validatePasswordMatch();


signals:
   void registrationSuccessful();
   void switchToLoginWindow();


private:
   QLineEdit* emailInput;
   QLineEdit* passwordInput;
   QLineEdit* confirmPasswordInput;
   QPushButton* registerButton;
   QPushButton* loginButton;
   QLabel* statusLabel;
  
   Authenticator* authenticator;
  
   void setupUI();
   void setupConnections();
};


#endif

