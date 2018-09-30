#include <ui/mainwindow.hpp>
#include <ui/hebbianneuralnetworkwidget.hpp>

#include <QMenuBar>
#include <QStatusBar>

MainWindow::MainWindow( QWidget* parent ) :
    QMainWindow( parent )
{
    QMenuBar* menuBar = new QMenuBar();
    this->setMenuBar( menuBar );

    QStatusBar* statusBar = new QStatusBar();
    this->setStatusBar( statusBar );

    HebbianNeuralNetworkWidget* hebbian = new HebbianNeuralNetworkWidget();
    this->setCentralWidget( hebbian );

    this->setWindowTitle( "AI" );
}

MainWindow::~MainWindow()
{
}
