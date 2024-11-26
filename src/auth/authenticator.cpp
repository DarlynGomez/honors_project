#include "../include/auth/authenticator.h"
#include <QCryptographicHash>
// QT Class often used to securely has passwords using crypto hashes
// I'm storing my passwords in non plain text
#include <QUuid> // Generates unqiue identifiers
// Using this to create my unqiue session tokens

// Define functions from authenticator class

// Default Constructor
Authenticator::Authenticator() {}

// Registers user
bool Authenticator::registerUser(const QString &email,
                                const QString &password, 
                                QString &errorMsg) {
    
    // Error handling -- Validating email
    if(!emailValidator.isValidEmail(email)) {
        errorMsg = QString::fromUtf8("Invalid email. Must be a @stu.bmcc.cuny.edu address");
        return false;
    }

    // Check Users existance
    // contains() checks if key exists in map
    if(userCredentials.contains(email)) {
        errorMsg = QString::fromUtf8("Email already registered");
        return false;
    }

    // Error handling -- Validating password
    if(!validatePassword(password, errorMsg)) {
        return false;
    }

    // Registers user if passes validation
    // Adds key-value pair with a HASHED password that is cryptohashed
    userCredentials.insert(email, hashPassword(password));
    return true;
}

// Logins in User
bool Authenticator::login(const QString &email,
                        const QString &password, 
                        QString &errorMsg) {

    // If no key exists with email in hashmap, false
    if (!userCredentials.contains(email)) {
        errorMsg = QString::fromUtf8("Email not found");
        return false;
    }

    // Verifys password
    // If key to value pair don't match, Wrong password
    if (userCredentials[email] != hashPassword(password)) {
        errorMsg = QString::fromUtf8("Incorrect password");
        return false;
    }

    // If validation is passed, create new session

    // Generates unique session token
    QString token = generateSessionToken();

    // Delete existing session if any
    if (activeSessions.contains(email)) {
        delete activeSessions[email];
    }

    activeSessions[email] = new UserSession(email, token);
    
    return true;
}

// Logs out of account and removes active session token from activeSessions has map
void Authenticator::logout(const QString &email) {
    if (activeSessions.contains(email)) {
        delete activeSessions[email];
        activeSessions.remove(email);
    }
}

// Changes password with proper email and old password
bool Authenticator::changePassword(const QString &email,
                    const QString &oldPassword, 
                    const QString &newPassword, 
                    QString &errorMsg) {
    // Verify the old password
    // If login doesn't go through, something is incorrect or user doesn't exist
    if(!login(email, oldPassword, errorMsg)) {
        return false;
    }

    // Validate new password
    if (!validatePassword(newPassword, errorMsg)) {
        return false;
    }

    // If validation is passed, update password
    // Updates key to value pair's value associated with key (email)
    userCredentials[email] = hashPassword(newPassword);
    return true;
}

// Checks if user is logged in using session object associated with key (email)
bool Authenticator::isUserLoggedIn(const QString &email) const {
    // If there is no active sessions associated with key, return false
    if (!activeSessions.contains(email)) {
        return false;
    }
    
    // Creates pointer that points to a user session object
    // Pointer points to session associated with key (email)
    UserSession* session = activeSessions[email];
    // Returns whether or not the session is valid
    return session->isValid();
}

// Gets current session
// Returns pointer pointing to key to value pair associated with email
// pointing to a user session object
UserSession* Authenticator::getCurrentSession(const QString& email) const {
    return activeSessions.value(email, nullptr);
}

// Validates password according to length and charcters, returns error messages
bool Authenticator::validatePassword(const QString &password, QString &errorMsg) const {
    // Length Validator
    if (password.length() < MIN_PASSWORD_LENGTH) {
        errorMsg = QString("Password must be at least %1 characters long").arg(MIN_PASSWORD_LENGTH);
        return false;
    }
    
    if (password.length() > MAX_PASSWORD_LENGTH) {
        errorMsg = QString("Password must not exceed %1 characters").arg(MAX_PASSWORD_LENGTH);
        return false;
    }
    
    // Check for at least one uppercase letter in password QString
    if (!password.contains(QRegularExpression("[A-Z]"))) {
        errorMsg = "Password must contain at least one uppercase letter";
        return false;
    }
    
    // Check for at least one number in password QString
    if (!password.contains(QRegularExpression("[0-9]"))) {
        errorMsg = "Password must contain at least one number";
        return false;
    }
    
    return true;
}

// Creates a crypto hash code / index code for password
// Can be converted back to original format, this is purely for security
QString Authenticator::hashPassword(const QString& password) const {
    QByteArray hash = QCryptographicHash::hash(
        password.toUtf8(), 
        QCryptographicHash::Sha256 // Uses the SHA-256 has algorithm
    );
    return QString(hash.toHex());
    // Returns hex version of hashed password
}

// Creates unique intricate session codes use <QUuid> in QStrings
QString Authenticator::generateSessionToken() const {
    return QUuid::createUuid().toString();
}

