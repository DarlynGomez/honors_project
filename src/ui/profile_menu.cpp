#include "ui/profile_menu.h"
#include <QVBoxLayout>

ProfileMenu::ProfileMenu(QWidget *parent) : QWidget(parent) {
    setupUI();
    
    // Connect signals
    connect(profileButton, &QPushButton::clicked, this, &ProfileMenu::profileRequested);
    connect(logoutButton, &QPushButton::clicked, this, &ProfileMenu::logoutRequested);
}

void ProfileMenu::setupUI() {
    setFixedWidth(200);  // Set fixed width for menu
    setStyleSheet(
        "QWidget {"
        "    background-color: white;"
        "    border: 1px solid #E0E0E0;"
        "    border-radius: 8px;"
        "    padding: 5px;"
        "}"
    );

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setSpacing(0);
    layout->setContentsMargins(0, 0, 0, 0);

    // Create buttons
    profileButton = new QPushButton("Profile", this);
    logoutButton = new QPushButton("Logout", this);

    // Update button styles
    QString buttonStyle =
        "QPushButton {"
        "    color: #2C3E50;"
        "    background: none;"
        "    border: none;"
        "    padding: 15px 20px;"
        "    text-align: left;"
        "    font-size: 14px;"
        "    width: 100%;"  // Make buttons fill width
        "}"
        "QPushButton:hover {"
        "    background-color: #E8F0E3;"
        "}";

    profileButton->setStyleSheet(buttonStyle);
    logoutButton->setStyleSheet(buttonStyle);

    // Add buttons to layout
    layout->addWidget(profileButton);
    layout->addWidget(logoutButton);
}