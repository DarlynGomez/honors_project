#ifndef EMAIL_VALIDATOR_H
#define EMAIL_VALIDATOR_H

#include <QtCore/QString>  // More powerful version of <string> 
// Allows seamless use as soneone building a QT-based application
#include <QtCore/QRegularExpression>

// Define Email Validator class which will only enable BMCC domains

class EmailValidator {
    private:
        // Only allowed domain to log in
        const QString ALLOWED_DOMAIN = "@stu.bmcc.cuny.edu";

    public:
        EmailValidator();   // Constructor

        // Validates if the email is form @stu.bmcc.cuny.edu: returns T or F
        bool isValidEmail(const QString &email) const;

        // Gets the domain from email to check for validation: returns string
        QString extractDomain(const QString &email) const;
};


#endif
