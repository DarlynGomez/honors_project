#ifndef PROFILE_PAGE_H
#define PROFILE_PAGE_H

#include <QWidget>
#include <QLabel>
#include <QComboBox>
#include <QPushButton>
#include <QStackedWidget>
#include <QScrollArea>
#include <QLineEdit>
#include <QTextEdit>
#include <QDialog>
#include <QFileDialog>
#include <QMessageBox>
#include "../auth/authenticator.h"
#include "../database/database_manager.h"

class ProfilePage : public QWidget {
    Q_OBJECT

public:
    explicit ProfilePage(Authenticator* auth, DatabaseManager* db, const QString& email, QWidget* parent = nullptr);
    void loadUserProfile();

private slots:
    void saveMajorAndSemester();
    void handleCreateListing();
    void handleRemoveListing();
    void switchSection(int index);

private:
    // Core components
    Authenticator* authenticator;
    DatabaseManager* dbManager;
    QString userEmail;
    QString firstName;
    QString lastName;
    
    // UI Elements
    QLabel* nameLabel;
    QLabel* emailLabel;
    QComboBox* majorCombo;
    QComboBox* semesterCombo;
    QStackedWidget* contentStack;
    
    // Layout elements
    QWidget* createProfileSection();
    QWidget* createListingsSection();
    QWidget* createOrderHistorySection();
    QScrollArea* createStyledScrollArea();
    QWidget* createListingCard(const QString& title = "Sample Listing", 
                             double price = 99.99,
                             const QString& status = "Active",
                             const QString& imagePath = "");
    QPushButton* createStyledButton(const QString& text, bool isPrimary = false);
    
    void setupUI();
    void extractNameFromEmail();
    void showCreateListingDialog();
    QString handleImageUpload();
    bool validateListingForm(const QString& title, const QString& price, const QString& description);
    
    // Style constants
    const QString sageGreen = "#9CAF88";    
    const QString darkBlue = "#2C3E50";
    const QString lightSage = "#E8F0E3";
};

#endif