#ifndef TEXTBOOK_PAGE_H
#define TEXTBOOK_PAGE_H

#include <QWidget>
#include <QComboBox>
#include <QLineEdit>
#include <QPushButton>
#include <QTabWidget>
#include <QLabel>
#include <QScrollArea>
#include <QVBoxLayout>
#include "database/database_manager.h"

class TextbookPage : public QWidget {
    Q_OBJECT

public:
    explicit TextbookPage(DatabaseManager* db, QWidget *parent = nullptr);
    void setUserEmail(const QString& email);
    void refreshRecommendations();

private slots:
    void handleFilter();
    void handleNextPage();
    void handlePrevPage();
    void handleTabChange(int index);

private:
    DatabaseManager* dbManager;
    QString currentUserEmail;
    QTabWidget* mainTabWidget;
    QWidget* createAllBooksTab();
    QWidget* createRecommendedTab();
    QWidget* allBooksTab;
    QWidget* recommendedTab;
    QVBoxLayout* recommendedLayout;
    QWidget* createRecommendedBookItem(const Textbook& book);
    QWidget* filterPanel;
    
    // Filter panel widgets
    QComboBox* departmentCombo;
    QLineEdit* lecInput;
    QComboBox* categoryCombo;
    QLineEdit* codeInput;
    QLineEdit* searchInput;
    QGridLayout* booksGrid;
    QGridLayout* recommendedGrid;
    QPushButton* prevButton;
    QPushButton* nextButton;
    int currentPage;
    
    void setupUI();
    void setupFilterPanel();
    void displayBooks(const QVector<Textbook>& books, QGridLayout* targetGrid);
    QWidget* createBookCard(const Textbook& book);
    void loadDepartments();
    void loadCategories();
    void updateRecommendedBooks();

    // Style constants
    const QString sageGreen = "#9CAF88";    
    const QString darkBlue = "#2C3E50";
    const QString lightSage = "#E8F0E3";
};

#endif