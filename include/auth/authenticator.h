#ifndef AUTHENTICATOR_H
#define AUTHENTICATOR_H


#include <QString> // More powerful version of <string>
// Allows seamless use as soneone building a QT-based application
#include <QHash> // Implements hash maps for key value pairs
#include "../include/auth/email_validator.h"
#include "../include/auth/user_session.h"


// Puts together user session tracking and logging athentication
class Authenticator {
   private:
       // BMCC password requirments/limits
       const int MIN_PASSWORD_LENGTH = 8;
       const int MAX_PASSWORD_LENGTH = 16;


       // Creates hash mapping for email to password key to value pairs
       QHash<QString, QString> userCredentials;
       // Creates hash mapping for email to UserSession class objects key to value pairs
       QHash<QString, UserSession*> activeSessions;
       // Create email validator object
       // Basically a helper object to access validating email methods
       EmailValidator emailValidator;


       // Function validates password inputted and sends error if PW is invalid
       bool validatePassword(const QString &password, QString &errorMsg) const;


       // My hash function! Turns key into hash code/index for secure storage
       QString hashPassword(const QString &password) const;


       // Creates unique session token to track and identify user
       QString generateSessionToken() const;


   public:
       Authenticator();    // Default Constructor


       // Registration
       // User gets registered and mapped onto userCredentials hash map
       bool registerUser(const QString &email,
                       const QString &password,
                       QString &errorMsg);


       // Login/Logout
       // Logins user if valid and adds them into activeSessions hash map,
       // creates session object
       bool login(const QString &email,
               const QString &password,
               QString &errorMsg);
       // Logs user out and ends session
       void logout(const QString &email);


       // Password manager
       // Changes password unless old password is invalid, sends error message
       bool changePassword(const QString &email,
                           const QString &oldPassword,
                           const QString &newPassword,
                           QString &errorMsg);
       // Resets password byt sending verification to email unless email doesnt exist
       bool resetPassword(const QString& email, QString& errorMsg);


       // Session manager
       // Checks if user is logged in
       bool isUserLoggedIn(const QString &email) const;
       // Returns pointer to userSession object to get current user session
       UserSession* getCurrentSession(const QString &email) const;
};




#endif







