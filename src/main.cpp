#include <QApplication>
#include <QStackedWidget>
#include <QGraphicsEffect>
#include <QPropertyAnimation>
#include <QEasingCurve>
#include "ui/login_window.h"         
#include "ui/registration_window.h"  
#include "auth/authenticator.h"


int main(int argc, char *argv[]) {
   QApplication app(argc, argv);


   // Add global styles
   app.setStyleSheet(R"(
       * {
           outline: none;  /* Removes focus rectangles */
       }
       QWidget {
           outline: none;
       }
       QPushButton {
           outline: none;
       }
       QLabel {
           outline: none;
           background: none;  /* Removes any background */
       }
       QLineEdit {
           outline: none;
       }
   )");
  
   // Create main container
   QStackedWidget mainWindow;
   mainWindow.setWindowTitle("BMCC E-Store");
   mainWindow.setStyleSheet(
       "QStackedWidget {"
       "   background-color: white;"
       "   outline: none;"
       "}"
   );
  
   // Create authenticator
   Authenticator* authenticator = new Authenticator();
  
   // Create login and registration windows
   LoginWindow* loginWindow = new LoginWindow(authenticator);
   RegistrationWindow* registrationWindow = new RegistrationWindow(authenticator);
  
   // Add windows to stack
   mainWindow.addWidget(loginWindow);
   mainWindow.addWidget(registrationWindow);
  
   // Connect signals for switching between windows
   QObject::connect(loginWindow, &LoginWindow::switchToRegister, [&]() {
       // Disable both windows' effects during transition
       loginWindow->setEnabled(false);
       registrationWindow->setEnabled(false);
      
       QPropertyAnimation *fadeOut = new QPropertyAnimation(loginWindow, "windowOpacity");
       fadeOut->setDuration(200);
       fadeOut->setStartValue(1.0);
       fadeOut->setEndValue(0.0);
      
       QObject::connect(fadeOut, &QPropertyAnimation::finished, [&]() {
           mainWindow.setCurrentWidget(registrationWindow);
           registrationWindow->setWindowOpacity(0.0);
          
           QPropertyAnimation *fadeIn = new QPropertyAnimation(registrationWindow, "windowOpacity");
           fadeIn->setDuration(200);
           fadeIn->setStartValue(0.0);
           fadeIn->setEndValue(1.0);
          
           QObject::connect(fadeIn, &QPropertyAnimation::finished, [&]() {
               loginWindow->setEnabled(true);
               registrationWindow->setEnabled(true);
           });
          
           fadeIn->start(QAbstractAnimation::DeleteWhenStopped);
       });
      
       fadeOut->start(QAbstractAnimation::DeleteWhenStopped);
   });


   // Do the same for the registration to login transition
   QObject::connect(registrationWindow, &RegistrationWindow::switchToLoginWindow, [&]() {
       registrationWindow->setEnabled(false);
       loginWindow->setEnabled(false);
      
       QPropertyAnimation *fadeOut = new QPropertyAnimation(registrationWindow, "windowOpacity");
       fadeOut->setDuration(200);
       fadeOut->setStartValue(1.0);
       fadeOut->setEndValue(0.0);
      
       QObject::connect(fadeOut, &QPropertyAnimation::finished, [&]() {
           mainWindow.setCurrentWidget(loginWindow);
           loginWindow->setWindowOpacity(0.0);
          
           QPropertyAnimation *fadeIn = new QPropertyAnimation(loginWindow, "windowOpacity");
           fadeIn->setDuration(200);
           fadeIn->setStartValue(0.0);
           fadeIn->setEndValue(1.0);
          
           QObject::connect(fadeIn, &QPropertyAnimation::finished, [&]() {
               loginWindow->setEnabled(true);
               registrationWindow->setEnabled(true);
           });
          
           fadeIn->start(QAbstractAnimation::DeleteWhenStopped);
       });
      
       fadeOut->start(QAbstractAnimation::DeleteWhenStopped);
   });
  
   QObject::connect(registrationWindow, &RegistrationWindow::registrationSuccessful, [&]() {
       mainWindow.setCurrentWidget(loginWindow);
   });
  
   mainWindow.setMinimumSize(1200, 800);  // Minimum size
   mainWindow.resize(1400, 900);          // Initial size
   mainWindow.show();
  
   return app.exec();
}

