#include <QScrollArea>
#include <QGridLayout>
#include <QFrame>
#include <QGraphicsDropShadowEffect>
#include "ui/profile_page.h"

ProfilePage::ProfilePage(Authenticator* auth, DatabaseManager* db, const QString& email, QWidget* parent)
    : QWidget(parent), authenticator(auth), dbManager(db), userEmail(email)
{
    extractNameFromEmail();
    setupUI();
    loadUserProfile();
}

void ProfilePage::setupUI() {
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(40, 40, 40, 40);
    mainLayout->setSpacing(30);

    // Header section with user info
    QWidget* headerWidget = new QWidget;
    QVBoxLayout* headerLayout = new QVBoxLayout(headerWidget);
    headerLayout->setSpacing(10);

    nameLabel = new QLabel(firstName + " " + lastName);
    nameLabel->setStyleSheet(
        "font-size: 32px;"
        "font-weight: bold;"
        "color: " + darkBlue + ";"
    );

    emailLabel = new QLabel(userEmail);
    emailLabel->setStyleSheet(
        "font-size: 16px;"
        "color: #666;"
    );

    headerLayout->addWidget(nameLabel);
    headerLayout->addWidget(emailLabel);
    mainLayout->addWidget(headerWidget);

    // Section navigation buttons
    QWidget* navWidget = new QWidget;
    QHBoxLayout* navLayout = new QHBoxLayout(navWidget);
    navLayout->setSpacing(20);

    QStringList sections = {"Edit Profile", "Your Listings", "Order History"};
    for (int i = 0; i < sections.size(); ++i) {
        QPushButton* btn = new QPushButton(sections[i]);
        btn->setStyleSheet(
            "QPushButton {"
            "    background-color: white;"
            "    color: " + darkBlue + ";"
            "    border: 2px solid " + sageGreen + ";"
            "    padding: 10px 20px;"
            "    border-radius: 20px;"
            "    font-size: 14px;"
            "    min-width: 150px;"
            "}"
            "QPushButton:hover {"
            "    background-color: " + sageGreen + ";"
            "    color: white;"
            "}"
        );
        connect(btn, &QPushButton::clicked, this, [this, i]() { switchSection(i); });
        navLayout->addWidget(btn);
    }
    mainLayout->addWidget(navWidget);

    // Content stack
    contentStack = new QStackedWidget;
    contentStack->addWidget(createProfileSection());
    contentStack->addWidget(createListingsSection());
    contentStack->addWidget(createOrderHistorySection());
    mainLayout->addWidget(contentStack);
}

QWidget* ProfilePage::createProfileSection() {
    QWidget* widget = new QWidget;
    QVBoxLayout* layout = new QVBoxLayout(widget);
    layout->setSpacing(20);

    // Create section title
    QLabel* title = new QLabel("Edit Profile Information");
    title->setStyleSheet(
        "font-size: 24px;"
        "font-weight: bold;"
        "color: " + darkBlue + ";"
        "margin-bottom: 20px;"
    );
    layout->addWidget(title);

    // Major selection
    QLabel* majorLabel = new QLabel("Major:");
    majorLabel->setStyleSheet("font-weight: bold; color: " + darkBlue + ";");
    majorCombo = new QComboBox;
    majorCombo->addItems({"Computer Science", "Computer Information Systems", "Liberal Arts", "Business Administration"});
    majorCombo->setStyleSheet(
        "QComboBox {"
        "    padding: 8px;"
        "    border: 2px solid #E0E0E0;"
        "    border-radius: 8px;"
        "    background: white;"
        "}"
        "QComboBox:hover {"
        "    border-color: " + sageGreen + ";"
        "}"
    );
    layout->addWidget(majorLabel);
    layout->addWidget(majorCombo);

    // Semester selection
    QLabel* semesterLabel = new QLabel("Semester Status:");
    semesterLabel->setStyleSheet("font-weight: bold; color: " + darkBlue + ";");
    semesterCombo = new QComboBox;
    semesterCombo->addItems({"Lower Freshman", "Upper Freshman", "Lower Sophomore", "Upper Sophomore"});
    semesterCombo->setStyleSheet(majorCombo->styleSheet());
    layout->addWidget(semesterLabel);
    layout->addWidget(semesterCombo);

    // Save button
    QPushButton* saveButton = new QPushButton("Save Changes");
    saveButton->setStyleSheet(
        "QPushButton {"
        "    background-color: " + sageGreen + ";"
        "    color: white;"
        "    padding: 12px;"
        "    border-radius: 8px;"
        "    font-size: 14px;"
        "    min-width: 120px;"
        "}"
        "QPushButton:hover {"
        "    background-color: " + darkBlue + ";"
        "}"
    );
    connect(saveButton, &QPushButton::clicked, this, &ProfilePage::saveMajorAndSemester);
    layout->addWidget(saveButton, 0, Qt::AlignLeft);

    layout->addStretch();
    return widget;
}

QWidget* ProfilePage::createListingsSection() {
    QWidget* widget = new QWidget;
    QVBoxLayout* layout = new QVBoxLayout(widget);
    layout->setSpacing(20);

    // Header with title and create button
    QHBoxLayout* headerLayout = new QHBoxLayout;
    QLabel* title = new QLabel("Your Listings");
    title->setStyleSheet(
        "font-size: 24px;"
        "font-weight: bold;"
        "color: " + darkBlue + ";"
    );
    headerLayout->addWidget(title);

    QPushButton* createButton = new QPushButton("Create New Listing");
    createButton->setStyleSheet(
        "QPushButton {"
        "    background-color: " + sageGreen + ";"
        "    color: white;"
        "    padding: 12px 20px;"
        "    border-radius: 8px;"
        "    font-size: 14px;"
        "}"
        "QPushButton:hover {"
        "    background-color: " + darkBlue + ";"
        "}"
    );
    connect(createButton, &QPushButton::clicked, this, &ProfilePage::handleCreateListing);
    headerLayout->addWidget(createButton);
    layout->addLayout(headerLayout);

    // Grid for listings
    QScrollArea* scrollArea = new QScrollArea;
    QWidget* scrollContent = new QWidget;
    QGridLayout* grid = new QGridLayout(scrollContent);
    grid->setSpacing(20);

    // This will be populated with actual listings later
    // For now, just show a placeholder
    QLabel* placeholder = new QLabel("No listings yet");
    placeholder->setStyleSheet(
        "color: #666;"
        "font-size: 16px;"
        "padding: 40px;"
    );
    placeholder->setAlignment(Qt::AlignCenter);
    grid->addWidget(placeholder, 0, 0);

    scrollArea->setWidget(scrollContent);
    scrollArea->setWidgetResizable(true);
    scrollArea->setFrameShape(QFrame::NoFrame);
    layout->addWidget(scrollArea);

    return widget;
}

QWidget* ProfilePage::createOrderHistorySection() {
    QWidget* widget = new QWidget;
    QVBoxLayout* layout = new QVBoxLayout(widget);
    layout->setSpacing(20);

    QLabel* title = new QLabel("Order History");
    title->setStyleSheet(
        "font-size: 24px;"
        "font-weight: bold;"
        "color: " + darkBlue + ";"
        "margin-bottom: 20px;"
    );
    layout->addWidget(title);

    QScrollArea* scrollArea = new QScrollArea;
    QWidget* scrollContent = new QWidget;
    QVBoxLayout* ordersLayout = new QVBoxLayout(scrollContent);
    ordersLayout->setSpacing(15);

    // This will be populated with actual orders later
    QLabel* placeholder = new QLabel("No orders yet");
    placeholder->setStyleSheet(
        "color: #666;"
        "font-size: 16px;"
        "padding: 40px;"
    );
    placeholder->setAlignment(Qt::AlignCenter);
    ordersLayout->addWidget(placeholder);

    scrollArea->setWidget(scrollContent);
    scrollArea->setWidgetResizable(true);
    scrollArea->setFrameShape(QFrame::NoFrame);
    layout->addWidget(scrollArea);

    return widget;
}

void ProfilePage::extractNameFromEmail() {
    // Extract first.last from first.last##@stu.bmcc.cuny.edu
    QString namePart = userEmail.split('@').first();
    QStringList parts = namePart.split('.');
    
    if (parts.size() >= 2) {
        // Remove any numbers from the last name
        QString lastName = parts[1];
        lastName.remove(QRegularExpression("\\d+"));
        
        // Capitalize first letters
        this->firstName = parts[0].at(0).toUpper() + parts[0].mid(1);
        this->lastName = lastName.at(0).toUpper() + lastName.mid(1);
    }
}

void ProfilePage::loadUserProfile() {
    // This will be implemented when we add database support
}

void ProfilePage::saveMajorAndSemester() {
    // This will be implemented when we add database support
}

void ProfilePage::handleCreateListing() {
    // This will be implemented later
}

void ProfilePage::handleRemoveListing() {
    // This will be implemented later
}

void ProfilePage::switchSection(int index) {
    contentStack->setCurrentIndex(index);
}