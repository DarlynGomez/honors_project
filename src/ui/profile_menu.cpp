// src/ui/profile_menu.cpp
#include "ui/profile_menu.h"
#include <QVBoxLayout>

ProfileMenu::ProfileMenu(QWidget *parent) : QWidget(parent) {
    setupUI();
}

void ProfileMenu::setupUI() {
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

    // Profile button
    profileButton = new QPushButton("Profile");
    profileButton->setStyleSheet(
        "QPushButton {"
        "    color: #2C3E50;"
        "    background: none;"
        "    border: none;"
        "    padding: 10px 20px;"
        "    text-align: left;"
        "    font-size: 14px;"
        "}"
        "QPushButton:hover {"
        "    background-color: #E8F0E3;"
        "}"
    );

    // Logout button
    logoutButton = new QPushButton("Logout");
    logoutButton->setStyleSheet(
        "QPushButton {"
        "    color: #2C3E50;"
        "    background: none;"
        "    border: none;"
        "    padding: 10px 20px;"
        "    text-align: left;"
        "    font-size: 14px;"
        "}"
        "QPushButton:hover {"
        "    background-color: #E8F0E3;"
        "}"
    );

    layout->addWidget(profileButton);
    layout->addWidget(logoutButton);

    // Connect signals
    connect(profileButton, &QPushButton::clicked, this, &ProfileMenu::profileRequested);
    connect(logoutButton, &QPushButton::clicked, this, &ProfileMenu::logoutRequested);
}