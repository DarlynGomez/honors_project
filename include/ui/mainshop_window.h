#ifndef MAINSHOP_WINDOW_H
#define MAINSHOP_WINDOW_H

#include <QMainWindow> // This holds the MAIN application window
#include <QToolBar> // My nav menu
#include <QLineEdit>  // Singular line of inputeed text
#include <QPushButton> // Gives clickable button
#include <QStackedWidget> // Switchign between widgets or pages
#include <QLabel> // Displays static, dynamic text, or images
#include "../auth/authenticator.h"
#include "database/database_manager.h"

#include <QGraphicsEffect>  // For modifying my widgets appearence like CSS
#include <QPropertyAnimation>  // Smooth time based transtions for objects
#include <QGraphicsDropShadowEffect>  // Enables drop shadow effects for widgets

class QHBoxLayout; 
class QVBoxLayout;
class TextbookPage;

class MainShopWindow : public QMainWindow {
    Q_OBJECT    // How I make my slots and signal connections

public:
    explicit MainShopWindow(Authenticator* auth, DatabaseManager* db, const QString& userEmail, QWidget *parent = nullptr);
    // Accesses my user information
    void setUserEmail(const QString& email);
    // Makes my user email accessible

private slots:
    void handleSearch();    // My slots to respond to signals
    void showTextbooks();
    void showFurniture();
    void showElectronics();
    void showSchoolSupplies();
    void showClothing();
    void showCart();
    void showWishlist();
    void handleLogout();

private:
    // Core components
    Authenticator* authenticator;   // Manages my user authentication
    QString currentUserEmail;   // Stores the email of the user logged in
    QStackedWidget* contentStack;   // Stack for displaying my windows
    DatabaseManager* dbManager; // Stores my database for products

    // Navigation components
    QToolBar* preNavBar;   // My pre-navigation bar
    QToolBar* navBar;   // My navigation bar
    QLineEdit* searchBar;   // My seach bar input box
    QPushButton* cartButton;    // Open cart button
    QPushButton* wishlistButton;    // Wishlist button
    
    // Category buttons
    QPushButton* textbooksButton;   // Textbook category button
    QPushButton* furnitureButton;   // Furniture category button
    QPushButton* electronicsButton;   // Electronic category button
    QPushButton* suppliesButton;   // Supplies category button
    QPushButton* clothingButton;   // Clothing category button

    // Setup methods
    void setupUI(); // Loads in my UI application
    void setUpPreNavBar();
    void setupNavBar(); // Loads in my nav bar
    void setupCategoryBar();  // Loads in my category bar
    void setupContentArea();    // Loads in my content area, under category bar
    void setupStyles();  // Styles for my main shop/ global
    
    // Helper methods
    QPushButton* createNavButton(const QString& iconPath, const QString& text); // Automatically creates new nav bar button
    QPushButton* createPreNavButton(const QString& iconPath, const QString& style);
    QPushButton* createCategoryButton(const QString& text); // Makes new category button
    QWidget* createCategoryWidget(const QString& category); // Makes new category widget to add to stack

    // My Main Style Colors
    const QString sageGreen = "#9CAF88";    
    const QString darkBlue = "#2C3E50";
    const QString lightSage = "#E8F0E3";
    const QString white = "#FFFFFF";
    const QString black = "#000000";

signals:
    void logoutRequested(); // logout request is emiitted on logout button click
};

#endif