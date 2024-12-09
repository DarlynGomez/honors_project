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

private slots:
    void handleFilter();
    void handleNextPage();
    void handlePrevPage();

private:
    DatabaseManager* dbManager;
    QComboBox* departmentCombo;
    QLineEdit* lecInput;
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
};

#endif