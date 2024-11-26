#include <QApplication> // My MAIN QT Application class
#include <QMainWindow>  // This gives my application window
#include <QLabel>  // This displays QT text/labels

int main(int argc, char *argv[]) {
    QApplication app(argc, argv); // Creates the QT application!
    
    QMainWindow window;  // Creates main window
    window.setWindowTitle("E-Store Project - Darlyn Gomez"); // My window title
    
    // Creates a text label and centers it in the window
    QLabel *label = new QLabel("Welcome to Honors Project E-Store", &window);
    label->setAlignment(Qt::AlignCenter);
    window.setCentralWidget(label);
    
    window.resize(800, 600); // Sets window size
    window.show(); // Shows the window
    
    return app.exec(); // Starts the application event loop
}