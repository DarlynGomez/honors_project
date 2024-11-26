#include "../include/auth/user_session.h"


// Define functions from user session class

// Defines default onstructor
UserSession::UserSession() {
    userId = "";
    sessionToken = "";
    createdAt = QDateTime::currentDateTime();
    // Retrieves the current local date and time.
    expiresAt = createdAt.addSecs(SESSION_DURATION_HOURS * 3600);
    // Sets expiration date of session 24hrs * 3600s after creation date
}

// Defines overloaded onstructor
UserSession::UserSession(const QString &id, const QString &token) {
    userId = id;
    sessionToken = token;
    createdAt = QDateTime::currentDateTime();
    // Retrieves the current local date and time.
    expiresAt = createdAt.addSecs(SESSION_DURATION_HOURS * 3600);
    // Sets expiration date of session 24hrs * 3600s after creation date
}

bool UserSession::isValid() const {
    // If my user id and session token is not equal to "" or QString()
    // And my session hasn't expired, the token is valid
    return !userId.isEmpty() && !sessionToken.isEmpty() && !hasExpired();
}

bool UserSession::hasExpired() const {
    // Returns true if current Date and Time surpasses previously
    // set token expiration date. Returns false otherwise
    return QDateTime::currentDateTime() > expiresAt;
}

// Refreshes token expiration to be 24hrs after current date and time
void UserSession::refresh() {
    expiresAt = QDateTime::currentDateTime().addSecs(SESSION_DURATION_HOURS * 3600);
}

// Define my getters

QString UserSession::getUserId() const {
    return userId;
}

QString UserSession::getSessionToken() const {
    return sessionToken;
}

