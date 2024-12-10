#ifndef CART_PAGE_H
#define CART_PAGE_H

#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QScrollArea>
#include "database/database_manager.h"
#include "auth/authenticator.h"

class CartPage : public QWidget {
    Q_OBJECT

public:
    explicit CartPage(DatabaseManager* db, const QString& userEmail, QWidget* parent = nullptr);
    void refreshCart();

private slots:
    void handleCheckout();
    void handleQuantityChange(const QString& productId, int quantity);
    void handleRemoveItem(const QString& productId);

private:
    DatabaseManager* dbManager;
    QString currentUserEmail;
    QVBoxLayout* cartItemsLayout;
    QLabel* totalLabel;
    double cartTotal;

    void setupUI();
    QWidget* createCartItem(const Textbook& book, int quantity);
    void updateTotal();
    
    // Style constants
    const QString sageGreen = "#9CAF88";    
    const QString darkBlue = "#2C3E50";
    const QString lightSage = "#E8F0E3";
};

#endif