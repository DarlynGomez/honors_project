#include "ui/textbook_page.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QScrollArea>
#include <QGraphicsDropShadowEffect>

TextbookPage::TextbookPage(DatabaseManager* db, QWidget *parent)
    : QWidget(parent), dbManager(db), currentPage(1)
{
    setupUI();
    loadDepartments();
    loadCategories();
    handleFilter(); // Load initial books
}

void TextbookPage::setupUI() {
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    
    // Title
    QLabel* title = new QLabel("Textbooks", this);
    title->setStyleSheet(
        "font-size: 24px;"
        "font-weight: bold;"
        "color: #2C3E50;"
        "margin-bottom: 20px;"
    );
    mainLayout->addWidget(title);
    
    setupFilterPanel();
    
    // Books grid
    QScrollArea* scrollArea = new QScrollArea(this);
    QWidget* scrollContent = new QWidget;
    booksGrid = new QGridLayout(scrollContent);
    booksGrid->setSpacing(20);
    scrollArea->setWidget(scrollContent);
    scrollArea->setWidgetResizable(true);
    mainLayout->addWidget(scrollArea);
    
    // Pagination
    QHBoxLayout* paginationLayout = new QHBoxLayout;
    prevButton = new QPushButton("Previous", this);
    nextButton = new QPushButton("Next", this);
    
    QString buttonStyle = 
        "QPushButton {"
        "    background-color: #9CAF88;"
        "    color: white;"
        "    padding: 8px 15px;"
        "    border-radius: 4px;"
        "    border: none;"
        "}"
        "QPushButton:hover {"
        "    background-color: #2C3E50;"
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
    
    mainLayout->addLayout(paginationLayout);
    
    connect(prevButton, &QPushButton::clicked, this, &TextbookPage::handlePrevPage);
    connect(nextButton, &QPushButton::clicked, this, &TextbookPage::handleNextPage);
}

void TextbookPage::setupFilterPanel() {
    QWidget* filterPanel = new QWidget(this);
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
    
    lecInput->setPlaceholderText("LEC Code");
    codeInput->setPlaceholderText("Course Code");
    searchInput->setPlaceholderText("Search by Title");
    
    QPushButton* filterButton = new QPushButton("Apply Filter", this);
    filterButton->setStyleSheet(
        "QPushButton {"
        "    background-color: #9CAF88;"
        "    color: white;"
        "    padding: 8px 15px;"
        "    border-radius: 4px;"
        "    border: none;"
        "}"
        "QPushButton:hover {"
        "    background-color: #2C3E50;"
        "}"
    );
    
    filterLayout->addWidget(departmentCombo);
    filterLayout->addWidget(lecInput);
    filterLayout->addWidget(categoryCombo);
    filterLayout->addWidget(codeInput);
    filterLayout->addWidget(searchInput);
    filterLayout->addWidget(filterButton);
    
    layout()->addWidget(filterPanel);
    
    connect(filterButton, &QPushButton::clicked, this, &TextbookPage::handleFilter);
}

QWidget* TextbookPage::createBookCard(const Textbook& book) {
   QWidget* card = new QWidget;
   card->setFixedSize(500, 400);
   card->setStyleSheet(
            "QWidget {"
            "background-color: white;"
            "border: 1px solid #E0E0E0;"
            "border-radius: 8px;"
            "outline: none;"
       "}"
   );
   
   QVBoxLayout* cardLayout = new QVBoxLayout(card);
   cardLayout->setSpacing(10);
   cardLayout->setContentsMargins(15, 15, 15, 15);
   
   // Book image
   QLabel* imageLabel = new QLabel(card);
   QPixmap bookImage(book.getImagePath());
   if (bookImage.isNull()) {
       bookImage = QPixmap(":/images/textbooks/default_book.jpg");
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
   
   // Add to Cart button
   QPushButton* addToCartBtn = new QPushButton("Add to Cart");
   addToCartBtn->setStyleSheet(
       "QPushButton {"
       "    background-color: #9CAF88;"
       "    color: white;"
       "    padding: 8px;"
       "    border-radius: 4px;"
       "    border: none;"
       "}"
       "QPushButton:hover {"
       "    background-color: #2C3E50;"
       "}"
   );
   
   // Add widgets to layout
   cardLayout->addWidget(imageLabel);
   cardLayout->addWidget(titleLabel);
   cardLayout->addWidget(courseLabel);
   cardLayout->addWidget(priceLabel);
   cardLayout->addWidget(addToCartBtn);
   cardLayout->addStretch();
   
   // Add shadow effect
   QGraphicsDropShadowEffect* shadow = new QGraphicsDropShadowEffect;
   shadow->setBlurRadius(15);
   shadow->setColor(QColor(0, 0, 0, 50));
   shadow->setOffset(0, 2);
   card->setGraphicsEffect(shadow);
   
   return card;
}

void TextbookPage::displayBooks(const QVector<Textbook>& books) {
    // Clear existing items
    QLayoutItem* item;
    while ((item = booksGrid->takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }
    
    // Add new books
    int row = 0, col = 0;
    for (const auto& book : books) {
        booksGrid->addWidget(createBookCard(book), row, col);
        col++;
        if (col >= 3) {  // 3 books per row
            col = 0;
            row++;
        }
    }
    
    // Update pagination buttons
    prevButton->setEnabled(currentPage > 1);
    nextButton->setEnabled(books.size() == 9);  // Disable if less than full page
}

void TextbookPage::handleFilter() {
    currentPage = 1;  // Reset to first page when filtering
    
    QVector<Textbook> books = dbManager->getTextbooks(
        departmentCombo->currentText(),
        lecInput->text(),
        categoryCombo->currentText(),
        codeInput->text(),
        searchInput->text(),
        currentPage,
        9  // Items per page
    );
    
    displayBooks(books);
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