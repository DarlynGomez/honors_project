#ifndef CHECKOUT_WINDOW_H
#define CHECKOUT_WINDOW_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QComboBox>
#include <QProgressDialog>
#include <QMessageBox>
#include <QTimer>
#include <QFrame>
#include <QStackedWidget>
#include "database/database_manager.h"

class CheckoutWindow : public QWidget {
    Q_OBJECT

public:
    explicit CheckoutWindow(DatabaseManager* db, const QString& userEmail, double total, QWidget *parent = nullptr);

signals:
    void orderComplete();
    void backToCart();

private slots:
    void nextStep();
    void previousStep();
    void processOrder();
    bool validateShippingForm(); 
    bool validatePaymentForm();

private:
    void setupUI();
    QWidget* createProgressBar();
    QWidget* createShippingForm();
    QWidget* createPaymentForm();
    QWidget* createOrderSummary();
    void updateProgressBar();
    bool validateCardNumber(const QString& number);
    bool validateExpiryDate(const QString& date);
    
    DatabaseManager* dbManager;
    QString currentUserEmail;
    double orderTotal;
    int currentStep = 0;
    
    // UI Elements
    QStackedWidget* formStack;
    QVector<QLabel*> stepLabels;
    QVector<QFrame*> stepLines;
    
    // Form Widgets
    QLineEdit* nameInput;
    QLineEdit* addressInput;
    QLineEdit* cityInput;
    QLineEdit* stateInput;
    QLineEdit* zipInput;
    QLineEdit* phoneInput;
    
    QLineEdit* cardNumberInput;
    QLineEdit* cardNameInput;
    QLineEdit* expiryInput;
    QLineEdit* cvvInput;
    
    QPushButton* nextButton;
    QPushButton* backButton;
    
    // Style Constants
    const QString sageGreen = "#9CAF88";
    const QString darkBlue = "#2C3E50";
    const QString lightSage = "#E8F0E3";
    const QString darkGrey = "#666666";
    const QString lightGrey = "#D3D3D3";
};

#endif