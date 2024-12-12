#ifndef PROFILE_MENU_H
#define PROFILE_MENU_H

#include <QWidget>
#include <QPushButton>

class ProfileMenu : public QWidget {
    Q_OBJECT

public:
    explicit ProfileMenu(QWidget *parent = nullptr);

signals:
    void profileRequested();
    void logoutRequested();

private:
    QPushButton* profileButton;
    QPushButton* logoutButton;
};

#endif