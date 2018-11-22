#include <ui/main_window.hpp>

#include <QMenuBar>
#include <QStatusBar>
#include <QFileDialog>
#include <QMessageBox>

#include <QStandardPaths>
#include <QFile>

MainWindow::MainWindow( QWidget* parent ) :
    QMainWindow( parent )
{   
    /// Menu.
    QMenuBar* menuBar = new QMenuBar();
    this->setMenuBar( menuBar );
    QMenu* fileMenu = new QMenu( "File" );
    menuBar->addMenu( fileMenu );
    auto openAction = new QAction( "Open" );
    auto saveAction = new QAction( "Save" );
    auto openImageAction = new QAction( "Open image" );
    auto saveImageAction = new QAction( "Save image" );
    connect( openAction, &QAction::triggered,
             this, &MainWindow::openNeuralNetwork );
    connect( saveAction, &QAction::triggered,
             this, &MainWindow::saveNeuralNetwork );
    connect( openImageAction, &QAction::triggered,
             this, &MainWindow::openImage );
    connect( saveImageAction, &QAction::triggered,
             this, &MainWindow::saveImage );
    fileMenu->addAction( openAction );
    fileMenu->addAction( saveAction );
    fileMenu->addAction( openImageAction );
    fileMenu->addAction( saveImageAction );

    QStatusBar* statusBar = new QStatusBar();
    this->setStatusBar( statusBar );

    serializer = new NeuralNetworkSerializer();

    hebbian = new HebbianNeuralNetworkWidget();
    this->setCentralWidget( hebbian );

    this->setWindowTitle( "AI" );
}

MainWindow::~MainWindow()
{
}

void MainWindow::replaceNeuralNetworkWidget(
        HebbianNeuralNetworkWidget* newNeuralNetworkWidget )
{
    this->setCentralWidget( newNeuralNetworkWidget );
    delete hebbian;
    hebbian = newNeuralNetworkWidget;
}

void MainWindow::openNeuralNetwork()
{
    auto fileName = QFileDialog::getOpenFileName(
            this, "Open File",
            QStandardPaths::standardLocations( QStandardPaths::HomeLocation ).last(),
            "NeuralNetworkData (*.nnd )" );
    if( fileName.isNull() ) return;

    serializer->setDevice( std::make_unique< QFile >( fileName ) );
    auto data = serializer->deserialize();

    replaceNeuralNetworkWidget(
                new HebbianNeuralNetworkWidget( data.getNumberOfNeurons(),
                                                data.getImageSize() ) );
    hebbian->setNeuralNetworkData( data );
}

void MainWindow::saveNeuralNetwork()
{
    auto fileName = QFileDialog::getSaveFileName(
            this, "Save File",
            QStandardPaths::standardLocations( QStandardPaths::HomeLocation ).last(),
            "NeuralNetworkData (*.nnd )" );
    if( fileName.isNull() ) return;

    serializer->setDevice( std::make_unique< QFile >( fileName ) );
    serializer->serialize( hebbian->getNeuralNetworkData() );
}

void MainWindow::openImage()
{
    auto fileName = QFileDialog::getOpenFileName(
            this, "Open File",
            QStandardPaths::standardLocations( QStandardPaths::HomeLocation ).last(),
            "Images (*.png *.xpm *.jpg)" );
    if( fileName.isNull() ) return;

    QImage image( fileName );

    if( image.size() != hebbian->getSampleImageSize() )
    {
        auto result = QMessageBox::question(
                    this, "Wrong size",
                    "Given image has incorrect size. Resize it?",
                    QMessageBox::Yes | QMessageBox::No );

        if( result != QMessageBox::Yes ) return;
        image = image.scaled( hebbian->getSampleImageSize() );
    }

    hebbian->setSampleImage( image.convertToFormat( QImage::Format_Mono ) );
}

void MainWindow::saveImage()
{
    auto fileName = QFileDialog::getSaveFileName(
            this, "Save File",
            QStandardPaths::standardLocations( QStandardPaths::HomeLocation ).last(),
            "Images (*.png *.xpm *.jpg)" );
    if( fileName.isNull() ) return;

    auto image = hebbian->getSampleImage();
    image.save( fileName );
}
