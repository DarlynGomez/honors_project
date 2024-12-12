#ifndef WISHLIST_PAGE_H
#define WISHLIST_PAGE_H

#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QScrollArea>
#include "database/database_manager.h"

class WishlistPage : public QWidget {
    Q_OBJECT

public:
    explicit WishlistPage(DatabaseManager* db, const QString& userEmail, QWidget *parent = nullptr);
    void refreshWishlist();
    void setUserEmail(const QString& email);

signals:
    void continueShoppingClicked();

private slots:
    void handleMoveToCart(const QString& productId);
    void handleRemoveItem(const QString& productId);
    void handleContinueShopping();

private:
    DatabaseManager* dbManager;
    QString currentUserEmail;
    QVBoxLayout* wishlistItemsLayout;
    QLabel* itemCountLabel;
    int itemCount;

    void setupUI();
    QWidget* createWishlistItem(const Textbook& book);
    QScrollArea* createStyledScrollArea();
    QPushButton* createStyledButton(const QString& text, bool isPrimary = false);
    void updateItemCount();
    
    // Style constants
    const QString sageGreen = "#9CAF88";    
    const QString darkBlue = "#2C3E50";
    const QString lightSage = "#E8F0E3";
};

#endif