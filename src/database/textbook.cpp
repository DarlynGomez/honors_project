#include "database/textbook.h"

Textbook::Textbook(
    const QString& dept,
    const QString& lecCode,
    const QString& category,
    const QString& code,
    const QString& bookTitle,
    const QString& bookAuthor,
    const QString& id,
    double bookPrice,
    const QString& image
) : department(dept),
    lec(lecCode),
    courseCategory(category),
    courseCode(code),
    title(bookTitle),
    author(bookAuthor),
    productId(id),
    price(bookPrice),
    imagePath(image)
{}
