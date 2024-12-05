#ifndef TEXTBOOK_H
#define TEXTBOOK_H

#include <QString>

class Textbook {
public:
    QString department;
    QString lec;
    QString courseCategory;
    QString courseCode;
    QString title;
    QString author;
    QString productId;
    QString imagePath;
    double price;
    
    Textbook(
        const QString& dept,
        const QString& lecCode,
        const QString& category,
        const QString& code,
        const QString& bookTitle,
        const QString& bookAuthor,
        const QString& id,
        double bookPrice,
        const QString& image
    );
    
    QString getImagePath() const { return imagePath; }
};

#endif