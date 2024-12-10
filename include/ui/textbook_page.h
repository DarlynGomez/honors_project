#ifndef TEXTBOOK_PAGE_H
#define TEXTBOOK_PAGE_H

#include <QWidget>
#include <QComboBox>
#include <QLineEdit>
#include <QPushButton>
#include <QCoreApplication>
#include <QGridLayout>
#include "database/database_manager.h"

class TextbookPage : public QWidget {
    Q_OBJECT

public:
    explicit TextbookPage(DatabaseManager* db, QWidget *parent = nullptr);
    void setUserEmail(const QString& email); // Get User email in session

private slots:
    void handleFilter();
    void handleNextPage();
    void handlePrevPage();

private:
    DatabaseManager* dbManager;
    QComboBox* departmentCombo;
    QLineEdit* lecInput;
    QString currentUserEmail; // Store current user email
    QComboBox* categoryCombo;
    QLineEdit* codeInput;
    QLineEdit* searchInput;
    QGridLayout* booksGrid;
    QPushButton* prevButton;
    QPushButton* nextButton;
    int currentPage;
    
    void setupUI();
    void setupFilterPanel();
    void displayBooks(const QVector<Textbook>& books);
    QWidget* createBookCard(const Textbook& book);
    void loadDepartments();
    void loadCategories();

    // My Style Colors
    const QString sageGreen = "#9CAF88";    
    const QString darkBlue = "#2C3E50";
    const QString lightSage = "#E8F0E3";
};

#endif