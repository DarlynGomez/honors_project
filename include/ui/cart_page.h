#ifndef CART_PAGE_H
#define CART_PAGE_H

#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QScrollArea>
#include <QSpinBox>
#include "database/database_manager.h"

class CartPage : public QWidget {
    Q_OBJECT

public:
    explicit CartPage(DatabaseManager* db, const QString& userEmail, QWidget *parent = nullptr);
    void refreshCart();
    void setUserEmail(const QString& email);

signals:
    void checkoutCompleted();

private slots:
    void handleCheckout();
    void handleQuantityChange(const QString& productId, int quantity);
    void handleRemoveItem(const QString& productId);
    void handleContinueShopping();

private:
    DatabaseManager* dbManager;
    QString currentUserEmail;
    QVBoxLayout* cartItemsLayout;
    QLabel* totalLabel;
    QLabel* itemCountLabel;
    double cartTotal;
    int itemCount;

    void setupUI();
    QWidget* createCartItem(const Textbook& book, int quantity);
    void updateTotal();
    void updateItemCount();
    QScrollArea* createStyledScrollArea();
    QPushButton* createStyledButton(const QString& text, bool isPrimary = false);
    
    // Style constants
    const QString sageGreen = "#9CAF88";    
    const QString darkBlue = "#2C3E50";
    const QString lightSage = "#E8F0E3";
};

#endif