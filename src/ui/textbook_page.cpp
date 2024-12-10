#include "ui/textbook_page.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGraphicsDropShadowEffect>
#include <QMessageBox>

TextbookPage::TextbookPage(DatabaseManager* db, QWidget *parent)
    : QWidget(parent)
    , dbManager(db)
    , currentPage(1)
    , booksGrid(nullptr)
    , recommendedLayout(nullptr)
    , filterPanel(nullptr)
{
    setupUI();
    loadDepartments();
    loadCategories();
}

void TextbookPage::setupUI() {
    // Main layout
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    
    // Create tab widget
    mainTabWidget = new QTabWidget(this);
    mainTabWidget->setStyleSheet(
        "QTabWidget::pane {"
        "    border: none;"
        "}"
        "QTabBar::tab {"
        "    padding: 12px 20px;"
        "    margin-right: 5px;"
        "    background-color: white;"
        "    border: 2px solid " + sageGreen + ";"
        "    border-radius: 8px;"
        "    color: " + darkBlue + ";"
        "    font-weight: bold;"
        "}"
        "QTabBar::tab:selected {"
        "    background-color: " + sageGreen + ";"
        "    color: white;"
        "}"
        "QTabBar::tab:hover {"
        "    background-color: " + lightSage + ";"
        "}"
    );

    // Initialize all grids and layouts first
    booksGrid = new QGridLayout;
    recommendedLayout = new QVBoxLayout;

    // Create and add tabs
    QWidget* allBooksWidget = new QWidget;
    QVBoxLayout* allBooksLayout = new QVBoxLayout(allBooksWidget);
    setupFilterPanel();
    allBooksLayout->addWidget(filterPanel);
    
    QScrollArea* gridScrollArea = new QScrollArea;
    QWidget* gridContainer = new QWidget;
    booksGrid->setSpacing(20);
    gridContainer->setLayout(booksGrid);
    gridScrollArea->setWidget(gridContainer);
    gridScrollArea->setWidgetResizable(true);
    gridScrollArea->setFrameShape(QFrame::NoFrame);
    allBooksLayout->addWidget(gridScrollArea);
    
    QWidget* recommendedWidget = createRecommendedTab();
    
    mainTabWidget->addTab(allBooksWidget, "All Books");
    mainTabWidget->addTab(recommendedWidget, "Recommended");
    
    mainLayout->addWidget(mainTabWidget);
    
    // Load initial books
    handleFilter();
}

QWidget* TextbookPage::createAllBooksTab() {
    QWidget* tab = new QWidget;
    QVBoxLayout* layout = new QVBoxLayout(tab);
    
    // Title
    QLabel* title = new QLabel("Textbooks", tab);
    title->setStyleSheet(
        "font-size: 24px;"
        "font-weight: bold;"
        "color: " + darkBlue + ";"
        "margin-bottom: 20px;"
    );
    layout->addWidget(title);
    
    // Add filter panel
    setupFilterPanel();
    layout->addWidget(filterPanel);
    
    // Books grid container
    QScrollArea* scrollArea = new QScrollArea(tab);
    QWidget* scrollContent = new QWidget;
    booksGrid = new QGridLayout(scrollContent);
    booksGrid->setSpacing(20);
    scrollArea->setWidget(scrollContent);
    scrollArea->setWidgetResizable(true);
    layout->addWidget(scrollArea);
    
    // Pagination
    QHBoxLayout* paginationLayout = new QHBoxLayout;
    prevButton = new QPushButton("Previous", tab);
    nextButton = new QPushButton("Next", tab);
    
    QString buttonStyle = 
        "QPushButton {"
        "    background-color: " + sageGreen + ";"
        "    color: white;"
        "    padding: 8px 15px;"
        "    border-radius: 4px;"
        "    border: none;"
        "}"
        "QPushButton:hover {"
        "    background-color: " + darkBlue + ";"
        "}"
        "QPushButton:disabled {"
        "    background-color: #CCCCCC;"
        "}";
        
    prevButton->setStyleSheet(buttonStyle);
    nextButton->setStyleSheet(buttonStyle);
    
    paginationLayout->addStretch();
    paginationLayout->addWidget(prevButton);
    paginationLayout->addWidget(nextButton);
    paginationLayout->addStretch();
    
    layout->addLayout(paginationLayout);
    
    return tab;
}

QWidget* TextbookPage::createRecommendedTab() {
    QWidget* tab = new QWidget;
    QVBoxLayout* layout = new QVBoxLayout(tab);
    layout->setSpacing(20);
    layout->setContentsMargins(40, 40, 40, 40);
    
    // Header section
    QLabel* title = new QLabel("Recommended Books", tab);
    title->setStyleSheet(
        "font-size: 24px;"
        "font-weight: bold;"
        "color: " + darkBlue + ";"
    );
    
    QLabel* subtitle = new QLabel(
        "Based on your major and semester level", tab);
    subtitle->setStyleSheet(
        "font-size: 16px;"
        "color: #666;"
        "margin-bottom: 20px;"
    );
    
    layout->addWidget(title);
    layout->addWidget(subtitle);
    
    // Scrollable area for recommended books
    QScrollArea* scrollArea = new QScrollArea;
    scrollArea->setWidgetResizable(true);
    scrollArea->setFrameShape(QFrame::NoFrame);
    scrollArea->setStyleSheet(
        "QScrollArea {"
        "    border: none;"
        "}"
        "QScrollBar:vertical {"
        "    border: none;"
        "    background: #F0F0F0;"
        "    width: 8px;"
        "    margin: 0px;"
        "}"
        "QScrollBar::handle:vertical {"
        "    background: " + sageGreen + ";"
        "    border-radius: 4px;"
        "}"
    );
    
    // Container for book list
    QWidget* listContainer = new QWidget;
    recommendedLayout = new QVBoxLayout(listContainer);
    recommendedLayout->setSpacing(15);
    recommendedLayout->setContentsMargins(0, 0, 0, 0);
    
    scrollArea->setWidget(listContainer);
    layout->addWidget(scrollArea);
    
    return tab;
}

QWidget* TextbookPage::createRecommendedBookItem(const Textbook& book) {
    QWidget* item = new QWidget;
    item->setStyleSheet(
        "QWidget {"
        "    background-color: white;"
        "    border: 1px solid #E0E0E0;"
        "    border-radius: 10px;"
        "    padding: 15px;"
        "}"
        "QWidget:hover {"
        "    background-color: " + lightSage + ";"
        "}"
    );
    
    QHBoxLayout* layout = new QHBoxLayout(item);
    layout->setSpacing(20);
    
    // Book image
    QLabel* imageLabel = new QLabel;
    QPixmap image(book.getImagePath());
    if (!image.isNull()) {
        imageLabel->setPixmap(image.scaled(80, 100, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    } else {
        imageLabel->setText("No Image");
        imageLabel->setStyleSheet("background: " + lightSage + "; padding: 10px; border-radius: 5px;");
    }
    imageLabel->setFixedSize(80, 100);
    
    // Book information
    QWidget* infoWidget = new QWidget;
    QVBoxLayout* infoLayout = new QVBoxLayout(infoWidget);
    infoLayout->setSpacing(5);
    
    QLabel* titleLabel = new QLabel(book.title);
    titleLabel->setStyleSheet(
        "font-weight: bold;"
        "font-size: 16px;"
        "color: " + darkBlue + ";"
    );
    titleLabel->setWordWrap(true);
    
    QLabel* courseLabel = new QLabel(
        QString("%1 %2 (LEC: %3)")
            .arg(book.courseCategory)
            .arg(book.courseCode)
            .arg(book.lec)
    );
    courseLabel->setStyleSheet("color: #666;");
    
    QLabel* priceLabel = new QLabel(QString("$%1").arg(book.price, 0, 'f', 2));
    priceLabel->setStyleSheet(
        "color: " + sageGreen + ";"
        "font-size: 16px;"
        "font-weight: bold;"
    );
    
    infoLayout->addWidget(titleLabel);
    infoLayout->addWidget(courseLabel);
    infoLayout->addWidget(priceLabel);
    infoLayout->addStretch();
    
    // Add to cart button
    QPushButton* cartButton = new QPushButton("Add to Cart");
    cartButton->setStyleSheet(
        "QPushButton {"
        "    background-color: " + sageGreen + ";"
        "    color: white;"
        "    padding: 8px 15px;"
        "    border-radius: 15px;"
        "    border: none;"
        "}"
        "QPushButton:hover {"
        "    background-color: " + darkBlue + ";"
        "}"
    );
    cartButton->setFixedWidth(120);
    
    connect(cartButton, &QPushButton::clicked, [=]() {
        dbManager->addToCart(currentUserEmail, book.productId, 1);
        QMessageBox::information(this, "Success", "Added to cart!");
    });
    
    // Layout assembly
    layout->addWidget(imageLabel);
    layout->addWidget(infoWidget, 1);
    layout->addWidget(cartButton);
    
    // Add shadow effect
    QGraphicsDropShadowEffect* shadow = new QGraphicsDropShadowEffect;
    shadow->setBlurRadius(10);
    shadow->setColor(QColor(0, 0, 0, 30));
    shadow->setOffset(0, 2);
    item->setGraphicsEffect(shadow);
    
    return item;
}

void TextbookPage::handleTabChange(int index) {
    if (index == 1) { // Recommended tab
        updateRecommendedBooks();
    }
}


void TextbookPage::updateRecommendedBooks() {
    qDebug() << "Updating recommendations for user:" << currentUserEmail;
    
    // Clear existing items
    QLayoutItem* item;
    while ((item = recommendedLayout->takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }
    
    // Get recommended books
    QVector<Textbook> recommendations = dbManager->getRecommendedBooks(currentUserEmail);
    
    qDebug() << "Received" << recommendations.size() << "recommendations";
    
    if (recommendations.isEmpty()) {
        QLabel* placeholder = new QLabel(
            "No recommendations available.\nPlease update your major and semester level in your profile.");
        placeholder->setStyleSheet(
            "color: #666;"
            "font-size: 16px;"
            "padding: 40px;"
        );
        placeholder->setAlignment(Qt::AlignCenter);
        recommendedLayout->addWidget(placeholder);
        qDebug() << "Added placeholder for empty recommendations";
        return;
    }
    
    // Add each recommended book as a list item
    for (const auto& book : recommendations) {
        recommendedLayout->addWidget(createRecommendedBookItem(book));
        qDebug() << "Added recommendation:" << book.title;
    }
    
    recommendedLayout->addStretch();
}


void TextbookPage::refreshRecommendations() {
    if (!recommendedLayout) {
        return;  // Guard against null layout
    }
    updateRecommendedBooks();
}

void TextbookPage::setupFilterPanel() {
    filterPanel = new QWidget(this);
    QHBoxLayout* filterLayout = new QHBoxLayout(filterPanel);
    
    QString inputStyle = 
        "QLineEdit, QComboBox {"
        "    padding: 5px;"
        "    border: 1px solid #E0E0E0;"
        "    border-radius: 4px;"
        "    min-width: 150px;"
        "}"
        "QComboBox::drop-down {"
        "    border: none;"
        "}"
        "QComboBox::down-arrow {"
        "    image: url(:/images/down-arrow.png);"
        "}";
    
    departmentCombo = new QComboBox(this);
    lecInput = new QLineEdit(this);
    categoryCombo = new QComboBox(this);
    codeInput = new QLineEdit(this);
    searchInput = new QLineEdit(this);
    
    departmentCombo->setStyleSheet(inputStyle);
    lecInput->setStyleSheet(inputStyle);
    categoryCombo->setStyleSheet(inputStyle);
    codeInput->setStyleSheet(inputStyle);
    searchInput->setStyleSheet(inputStyle);
    
    departmentCombo->setPlaceholderText("Department");
    categoryCombo->setPlaceholderText("Course Section");
    lecInput->setPlaceholderText("LEC Code");
    codeInput->setPlaceholderText("Course Code");
    searchInput->setPlaceholderText("Search by Title");
    
    QPushButton* filterButton = new QPushButton("Apply Filter", this);
    filterButton->setStyleSheet(
        "QPushButton {"
        "    background-color: " + sageGreen + ";"
        "    color: white;"
        "    padding: 8px 15px;"
        "    border-radius: 4px;"
        "    border: none;"
        "}"
        "QPushButton:hover {"
        "    background-color: " + darkBlue + ";"
        "}"
    );
    
    filterLayout->addWidget(departmentCombo);
    filterLayout->addWidget(lecInput);
    filterLayout->addWidget(categoryCombo);
    filterLayout->addWidget(codeInput);
    filterLayout->addWidget(searchInput);
    filterLayout->addWidget(filterButton);
    
    connect(filterButton, &QPushButton::clicked, this, &TextbookPage::handleFilter);
}

QWidget* TextbookPage::createBookCard(const Textbook& book) {
    QWidget* card = new QWidget;
    card->setFixedSize(400, 400);
    card->setStyleSheet(
        "QWidget {"
        "   background-color: white;"
        "   border: 1px solid #E0E0E0;"
        "   border-radius: 8px;"
        "   outline: none;"
        "}"
    );
   
    QVBoxLayout* cardLayout = new QVBoxLayout(card);
    cardLayout->setSpacing(10);
    cardLayout->setContentsMargins(15, 15, 15, 15);
   
    // Book image with fallback to default
    QLabel* imageLabel = new QLabel(card);
    QPixmap bookImage(book.getImagePath());
    if (bookImage.isNull()) {
        // Use default image from assets
        QString defaultPath = QCoreApplication::applicationDirPath() + "/../assets/images/textbooks/default_book.jpg";
        bookImage = QPixmap(defaultPath);
        if (bookImage.isNull()) {
            qDebug() << "Failed to load image from:" << book.getImagePath();
            qDebug() << "And failed to load default image from:" << defaultPath;
        }
    }
    imageLabel->setPixmap(bookImage.scaled(200, 250, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    imageLabel->setAlignment(Qt::AlignCenter);

   // Title
   QLabel* titleLabel = new QLabel(book.title);
   titleLabel->setWordWrap(true);
   titleLabel->setStyleSheet(
       "font-weight: bold;"
       "font-size: 14px;"
       "color: #2C3E50;"
   );
   titleLabel->setAlignment(Qt::AlignCenter);
   
   // Course info
   QLabel* courseLabel = new QLabel(
       book.courseCategory + " " + book.courseCode + 
       " (LEC: " + book.lec + ")"
   );
   courseLabel->setStyleSheet("color: #666;");
   courseLabel->setAlignment(Qt::AlignCenter);
   
   // Price
   QLabel* priceLabel = new QLabel(
       QString("$%1").arg(book.price, 0, 'f', 2)
   );
   priceLabel->setStyleSheet(
       "color: #9CAF88;"
       "font-size: 18px;"
       "font-weight: bold;"
   );
   priceLabel->setAlignment(Qt::AlignCenter);

    QHBoxLayout* buttonLayout = new QHBoxLayout;

    QPushButton* cartButton = new QPushButton;
    cartButton->setIcon(QIcon(QCoreApplication::applicationDirPath() + "/../assets/images/nav/cartIcon.png"));
    cartButton->setIconSize(QSize(24, 24));
    cartButton->setStyleSheet(
        "QPushButton {"
        "    background-color: " + sageGreen + ";"
        "    border-radius: 20px;"
        "    padding: 8px;"
        "}"
        "QPushButton:hover {"
        "    background-color: " + darkBlue + ";"
        "}"
    );

    QPushButton* wishlistButton = new QPushButton;
    wishlistButton->setIcon(QIcon(QCoreApplication::applicationDirPath() + "/../assets/images/nav/wishlistIcon.png"));
    wishlistButton->setIconSize(QSize(24, 24));
    wishlistButton->setStyleSheet(cartButton->styleSheet());

    buttonLayout->addWidget(cartButton);
    buttonLayout->addWidget(wishlistButton);
    buttonLayout->setAlignment(Qt::AlignRight);


    connect(cartButton, &QPushButton::clicked, [=]() {
        dbManager->addToCart(currentUserEmail, book.productId, 1);
        QMessageBox::information(this, "Success", "Added to cart!");
    });
   
   // Add widgets to layout
   cardLayout->addWidget(imageLabel);
   cardLayout->addWidget(titleLabel);
   cardLayout->addWidget(courseLabel);
   cardLayout->addWidget(priceLabel);
   cardLayout->addLayout(buttonLayout);
   cardLayout->addStretch();
   
   // Add shadow effect
   QGraphicsDropShadowEffect* shadow = new QGraphicsDropShadowEffect;
   shadow->setBlurRadius(15);
   shadow->setColor(QColor(0, 0, 0, 50));
   shadow->setOffset(0, 2);
   card->setGraphicsEffect(shadow);
   
   return card;
}

void TextbookPage::setUserEmail(const QString& email) {
    currentUserEmail = email;
    refreshRecommendations();
}

void TextbookPage::displayBooks(const QVector<Textbook>& books, QGridLayout* targetGrid) {
    // Clear existing items
    QLayoutItem* item;
    while ((item = targetGrid->takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }
    
    if (books.isEmpty()) {
        QLabel* placeholder = new QLabel("No books found matching your criteria.");
        placeholder->setStyleSheet(
            "color: #666;"
            "font-size: 16px;"
            "padding: 40px;"
        );
        placeholder->setAlignment(Qt::AlignCenter);
        targetGrid->addWidget(placeholder, 0, 0);
        return;
    }
    
    // Add new books
    int row = 0, col = 0;
    for (const auto& book : books) {
        targetGrid->addWidget(createBookCard(book), row, col);
        col++;
        if (col >= 3) {  // 3 books per row
            col = 0;
            row++;
        }
    }
    
    // Update pagination buttons
    if (targetGrid == booksGrid) {
        prevButton->setEnabled(currentPage > 1);
        nextButton->setEnabled(books.size() == 9);  // Disable if less than full page
    }
}

void TextbookPage::handleFilter() {
    if (!booksGrid) {
        return;  // Guard against null grid
    }

    currentPage = 1;
    
    QVector<Textbook> books = dbManager->getTextbooks(
        departmentCombo->currentText(),
        lecInput->text(),
        categoryCombo->currentText(),
        codeInput->text(),
        searchInput->text(),
        currentPage,
        9
    );
    
    // Clear existing items
    QLayoutItem* item;
    while ((item = booksGrid->takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }
    
    // Add books to grid
    int row = 0, col = 0;
    for (const auto& book : books) {
        booksGrid->addWidget(createBookCard(book), row, col);
        col++;
        if (col >= 3) {
            col = 0;
            row++;
        }
    }
}

void TextbookPage::handleNextPage() {
    currentPage++;
    handleFilter();
}

void TextbookPage::handlePrevPage() {
    if (currentPage > 1) {
        currentPage--;
        handleFilter();
    }
}

void TextbookPage::loadDepartments() {
    departmentCombo->addItem("");  // Empty option for no filter
    departmentCombo->addItem("Computer Science");
    departmentCombo->addItem("Social Sciences, Human Services & Criminal Justice");
    departmentCombo->addItem("English");
    departmentCombo->addItem("Science");
    departmentCombo->addItem("Mathematics");
}

void TextbookPage::loadCategories() {
    categoryCombo->addItem("");  // Empty option for no filter
    categoryCombo->addItem("CSC");
    categoryCombo->addItem("ECO");
    categoryCombo->addItem("ENG");
    categoryCombo->addItem("PHY");
    categoryCombo->addItem("MAT");
}