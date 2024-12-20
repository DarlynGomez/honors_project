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
#include <QRegularExpression>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QMessageBox>
#include "../auth/authenticator.h"
#include "../database/database_manager.h"

// Define Order structure
class Order {
public:
    Order(const QString& orderId, const QString& listingId, const QString& status, const QDateTime& orderDate)
        : orderId(orderId), listingId(listingId), status(status), orderDate(orderDate) {}

    QString orderId;
    QString listingId;
    QString status; // e.g., "Completed", "Pending", etc.
    QDateTime orderDate;
    
    // Add additional methods if needed for displaying or updating the order.
};

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
    void handleOrderPlacement();  // Slot to handle placing orders
    void viewOrderHistory();  // Slot to view the user's order history

private:
    QGridLayout* listingsGrid;
    // Core components
    Authenticator* authenticator;
    DatabaseManager* dbManager;
    QString userEmail;
    QString firstName;
    QString lastName;
    QList<Order> orderHistory;  // List to store user orders
    
    // UI Elements
    QLabel* nameLabel;
    QLabel* emailLabel;
    QComboBox* majorCombo;
    QComboBox* semesterCombo;
    QStackedWidget* contentStack;
    
    // Layout elements
    QWidget* createProfileSection();
    QWidget* createListingsSection();
    QWidget* createOrderHistorySection();  // Add new section for orders
    QWidget* createOrderItem(const QString& title, double price, const QString& status, const QString& imagePath);
    QScrollArea* createStyledScrollArea();
    QWidget* createListingCard(const QString& title = "Sample Listing", 
                             double price = 99.99,
                             const QString& status = "Active",
                             const QString& imagePath = "");
    QPushButton* createStyledButton(const QString& text, bool isPrimary = false);
    
    void setupUI();
    void refreshListings();
    void extractNameFromEmail();
    void showCreateListingDialog();
    QString handleImageUpload();
    bool validateListingForm(const QString& title, const QString& price, const QString& description);
    void loadOrderHistory();  // Load order history into the UI
    
    // Style constants
    const QString sageGreen = "#9CAF88";    
    const QString darkBlue = "#2C3E50";
    const QString lightSage = "#E8F0E3";
};

#endif
