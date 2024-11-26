#include "../include/auth/email_validator.h"
#include <QtCore/QRegularExpression>
// This header allows you to allows you to define 
// patterns to search for in strings.

// Define functions from email validator class

// Define my constructor
EmailValidator::EmailValidator() {}

// Define email validator
bool EmailValidator::isValidEmail(const QString &email) const {
    // Using a typical email check format to compare email to regex
    QRegularExpression emailRegex("^[A-Za-z0-9._%+-]+@[A-Za-z0-9.-]+\\.[A-Za-z]{2,}$");
    if (!emailRegex.match(email).hasMatch()) {
        return false;
    }
    // .match(email)   Matches the inputed email to the regex
    // .hasMatch()   Return whether the email matches the pattern

    // Check for domain match (stu.bmcc.cuny.edu)
    QString domain = extractDomain(email);  // Takes email domain from inputted email
    return domain == ALLOWED_DOMAIN;
    // Checks if domain is == to private ALLOWED_DOMAIN
}

// Define email extractor
QString EmailValidator::extractDomain(const QString &email) const {
    int atIndex = email.indexOf('@');  // Goes to character @
    // indexOf() finds position of a substring or regular expression within stirng
    // and returns the index of its first occurence

    // If sub string is not found it returns -1
    if(atIndex == -1) return QString(); 
    // Calls QString Class Construtor which creates an emptry string ""

    // If @ is found, the domain will be extracted by returning the substring starting at @
    return email.mid(atIndex);

}
