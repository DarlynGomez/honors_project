#include <QApplication> // Initializes and handles my GUI app with widgets
#include <QStackedWidget> // Used to implement my multi-pages and switching between them
#include <QGraphicsEffect>  // For modifying my widgets appearence like CSS
#include <QPropertyAnimation>  // Smooth time based transtions for objects
#include <QEasingCurve>  // Controls the rate of change of animations
#include "ui/login_window.h"         
#include "ui/registration_window.h"  
#include "auth/authenticator.h"
#include "ui/mainshop_window.h"

int main(int argc, char *argv[]) {
   QApplication app(argc, argv);    // Initializing QT App


   // Add global styles
   app.setStyleSheet(R"(
       * {
           outline: none;  /* Removes ugly focused rectangles from ALL objects */
       }
       QWidget {
           outline: none;
       }
       QPushButton {
           outline: none;
       }
       QLabel {
           outline: none;
           background: none;  /* Removes any background or focused rectangle */
       }
       QLineEdit {
           outline: none;
       }
   )");
  
   // Create main container
   QStackedWidget mainWindow;  // Manages stacked multiple windows
   mainWindow.setWindowTitle("BMCC E-Store");  // Title of my main window
   mainWindow.setStyleSheet(  // Style of my main window
       "QStackedWidget {"
       "   background-color: white;"
       "   outline: none;"
       "}"
   );
  
   // Create authenticator object to handle my login&registration
   Authenticator* authenticator = new Authenticator();
  
   // Create login and registration windows
   LoginWindow* loginWindow = new LoginWindow(authenticator);  // My Login INterface
   RegistrationWindow* registrationWindow = new RegistrationWindow(authenticator); // My registration interface
   MainShopWindow* shopWindow = new MainShopWindow(authenticator, "");  // My main shop interface
  
   // Add windows to stack -> the mainWindow container
   mainWindow.addWidget(loginWindow);   // Adds login window to the stack
   mainWindow.addWidget(registrationWindow);    // Adds registration window to the stack
   mainWindow.addWidget(shopWindow);    // Adds main shop window to the stack
  
   // Connect signals for switching between windows
   // This connection hands transition from login to register
   QObject::connect(loginWindow, &LoginWindow::switchToRegister, [&]() {
       // Disable both windows user interactions during transition
       // Makes it not look like a chunky animation
       loginWindow->setEnabled(false);
       registrationWindow->setEnabled(false);
      
       // Fade out animation for login window
       QPropertyAnimation *fadeOut = new QPropertyAnimation(loginWindow, "windowOpacity");
       fadeOut->setDuration(200);
       fadeOut->setStartValue(1.0);
       fadeOut->setEndValue(0.0);
      
       // Switch to registration with animation
       QObject::connect(fadeOut, &QPropertyAnimation::finished, [&]() {
           mainWindow.setCurrentWidget(registrationWindow);
           registrationWindow->setWindowOpacity(0.0);
           // Fade in animation for registration window
           QPropertyAnimation *fadeIn = new QPropertyAnimation(registrationWindow, "windowOpacity");
           fadeIn->setDuration(200);
           fadeIn->setStartValue(0.0);
           fadeIn->setEndValue(1.0);
           // Allows interaction again after animation is completed
           QObject::connect(fadeIn, &QPropertyAnimation::finished, [&]() {
               loginWindow->setEnabled(true);
               registrationWindow->setEnabled(true);
           });
           // Start my fade in
           fadeIn->start(QAbstractAnimation::DeleteWhenStopped);
       });
       // Start my fade out
       fadeOut->start(QAbstractAnimation::DeleteWhenStopped);
   });


   // Transition from registration to login
   QObject::connect(registrationWindow, &RegistrationWindow::switchToLoginWindow, [&]() {
       // Disable both windows user interactions during transition
       // Makes it not look like a chunky animation
       registrationWindow->setEnabled(false);
       loginWindow->setEnabled(false);

       // Fade out animation for registration window
       QPropertyAnimation *fadeOut = new QPropertyAnimation(registrationWindow, "windowOpacity");
       fadeOut->setDuration(200);
       fadeOut->setStartValue(1.0);
       fadeOut->setEndValue(0.0);
      
       // Switch to login with animation
       QObject::connect(fadeOut, &QPropertyAnimation::finished, [&]() {
           mainWindow.setCurrentWidget(loginWindow);
           loginWindow->setWindowOpacity(0.0);
           // Fade in animation for login window
           QPropertyAnimation *fadeIn = new QPropertyAnimation(loginWindow, "windowOpacity");
           fadeIn->setDuration(200);
           fadeIn->setStartValue(0.0);
           fadeIn->setEndValue(1.0);
           // Allows interaction again after animation is completed
           QObject::connect(fadeIn, &QPropertyAnimation::finished, [&]() {
               loginWindow->setEnabled(true);
               registrationWindow->setEnabled(true);
           });
           // Start my fade in
           fadeIn->start(QAbstractAnimation::DeleteWhenStopped);
       });
       // Start my fade out
       fadeOut->start(QAbstractAnimation::DeleteWhenStopped);
   });
  
   // If user successfully registers, switches to login
   QObject::connect(registrationWindow, &RegistrationWindow::registrationSuccessful, [&]() {
       mainWindow.setCurrentWidget(loginWindow);
   });

   // Add connection for successful login
    QObject::connect(loginWindow, &LoginWindow::loginSuccessful, 
        [&](const QString& email) {
            // Disable windows during transition
            loginWindow->setEnabled(false);
            shopWindow->setEnabled(false);
            // Fade out animation for login window
            QPropertyAnimation *fadeOut = new QPropertyAnimation(loginWindow, "windowOpacity");
            fadeOut->setDuration(200);
            fadeOut->setStartValue(1.0);
            fadeOut->setEndValue(0.0);

            // Switch to main shop window with animation
            QObject::connect(fadeOut, &QPropertyAnimation::finished, [&, email]() {
                shopWindow->setUserEmail(email);  // Set user email before showing window
                mainWindow.setCurrentWidget(shopWindow);
                shopWindow->setWindowOpacity(0.0);
                // Fade in animation for main shop window
                QPropertyAnimation *fadeIn = new QPropertyAnimation(shopWindow, "windowOpacity");
                fadeIn->setDuration(200);
                fadeIn->setStartValue(0.0);
                fadeIn->setEndValue(1.0);
                // Allows interaction again after animation is completed
                QObject::connect(fadeIn, &QPropertyAnimation::finished, [&]() {
                    loginWindow->setEnabled(true);
                    shopWindow->setEnabled(true);
                });
                // Start my fade in
                fadeIn->start(QAbstractAnimation::DeleteWhenStopped);
            });
            // Start my fade out
            fadeOut->start(QAbstractAnimation::DeleteWhenStopped);
        });

    // Add connection for logout with same animation style, switches from shop window to login
    QObject::connect(shopWindow, &MainShopWindow::logoutRequested, [&]() {
        // Disable both windows user interactions during transition
        // Makes it not look like a chunky animation
        shopWindow->setEnabled(false);
        loginWindow->setEnabled(false);
        // Fade out animation for shop window
        QPropertyAnimation *fadeOut = new QPropertyAnimation(shopWindow, "windowOpacity");
        fadeOut->setDuration(200);
        fadeOut->setStartValue(1.0);
        fadeOut->setEndValue(0.0);
        
        // Switch to login window with animation
        QObject::connect(fadeOut, &QPropertyAnimation::finished, [&]() {
            mainWindow.setCurrentWidget(loginWindow);
            loginWindow->setWindowOpacity(0.0);
            // Fade in animation for login window
            QPropertyAnimation *fadeIn = new QPropertyAnimation(loginWindow, "windowOpacity");
            fadeIn->setDuration(200);
            fadeIn->setStartValue(0.0);
            fadeIn->setEndValue(1.0);
            // Allows interaction again after animation is completed
            QObject::connect(fadeIn, &QPropertyAnimation::finished, [&]() {
                shopWindow->setEnabled(true);
                loginWindow->setEnabled(true);
            });
            // Start my fade in
            fadeIn->start(QAbstractAnimation::DeleteWhenStopped);
        });
        // Start my fade out
        fadeOut->start(QAbstractAnimation::DeleteWhenStopped);
    });
  
   mainWindow.setMinimumSize(1200, 800);  // Minimum size of main window
   mainWindow.resize(1400, 900);          // Initial size of mainwindow
   mainWindow.show(); // Display main window
  
   return app.exec(); // Entering my main event loop
}

