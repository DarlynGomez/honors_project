#ifndef USER_SESSION_H
#define USER_SESSION_H


#include <QtCore/QString> // More powerful version of <string>
#include <QtCore/QDateTime> // QT class that provides methods for data and time!


// Create UserSession class which will track the session token of user
// logged in and expire/refresh token or move on depending on sessions length


class UserSession {
   private:
       QString userId;     // ID of session user
       QString sessionToken;       // Holds the unqiue session token
       QDateTime createdAt;        // Holds time session was created
       QDateTime expiresAt;        // Holds time session expires
       // Sessions of my app will expire after 24 hours
       const int SESSION_DURATION_HOURS = 24;


   public:
       UserSession(); // Default Constructor
       // Constructor that will track users session
       UserSession(const QString &userId, const QString &sessionToken);


       // Checks is sesssion token is still valid
       bool isValid() const;
       // Checks is sesssion token is expired
       bool hasExpired() const;
       // Refreshes token
       void refresh();


       // Getters
       QString getUserId() const;
       QString getSessionToken() const;


};






#endif

