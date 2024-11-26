#ifndef MAINSHOP_WINDOW_H
#define MAINSHOP_WINDOW_H

#include <QMainWindow>
#include <QToolBar>
#include <QLineEdit>
#include <QPushButton>
#include <QStackedWidget>
#include <QLabel>
#include "../auth/authenticator.h"

#include <QGraphicsEffect>
#include <QPropertyAnimation>
#include <QGraphicsDropShadowEffect>

class QHBoxLayout;
class QVBoxLayout;

class MainShopWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainShopWindow(Authenticator* auth, const QString& userEmail, QWidget *parent = nullptr);
    void setUserEmail(const QString& email);

private slots:
    void handleSearch();
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
    Authenticator* authenticator;
    QString currentUserEmail;
    QStackedWidget* contentStack;

    // Navigation components
    QToolBar* navBar;
    QLineEdit* searchBar;
    QPushButton* cartButton;
    QPushButton* wishlistButton;
    
    // Category buttons
    QPushButton* textbooksButton;
    QPushButton* furnitureButton;
    QPushButton* electronicsButton;
    QPushButton* suppliesButton;
    QPushButton* clothingButton;

    // Setup methods
    void setupUI();
    void setupNavBar();
    void setupCategoryBar();
    void setupContentArea();
    void setupStyles();
    
    // Helper methods
    QPushButton* createNavButton(const QString& iconPath, const QString& text);
    QPushButton* createCategoryButton(const QString& text);
    QWidget* createCategoryWidget(const QString& category);

    // Style constants
    const QString sageGreen = "#9CAF88";
    const QString darkBlue = "#2C3E50";
    const QString lightSage = "#E8F0E3";
    const QString white = "#FFFFFF";
    const QString black = "#000000";

signals:
    void logoutRequested();
};

#endif