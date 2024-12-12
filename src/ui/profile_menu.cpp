#include "ui/profile_menu.h"
#include <QVBoxLayout>
#include <QDebug>

ProfileMenu::ProfileMenu(QWidget *parent) : QWidget(parent) {
    setFixedWidth(200);
    
    setStyleSheet(
        "QWidget {"
        "    background-color: white;"
        "    border: 1px solid #E0E0E0;"
        "    border-radius: 8px;"
        "}"
    );

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setSpacing(0);
    layout->setContentsMargins(0, 0, 0, 0);

    profileButton = new QPushButton("Profile", this);
    logoutButton = new QPushButton("Logout", this);

    QString buttonStyle = 
        "QPushButton {"
        "    border: none;"
        "    padding: 15px 20px;"
        "    text-align: left;"
        "    font-size: 14px;"
        "    color: #2C3E50;"
        "}"
        "QPushButton:hover {"
        "    background-color: #E8F0E3;"
        "}";

    profileButton->setStyleSheet(buttonStyle);
    logoutButton->setStyleSheet(buttonStyle);

    layout->addWidget(profileButton);
    layout->addWidget(logoutButton);

    connect(profileButton, &QPushButton::clicked, this, &ProfileMenu::profileRequested);
    connect(logoutButton, &QPushButton::clicked, this, &ProfileMenu::logoutRequested);
}