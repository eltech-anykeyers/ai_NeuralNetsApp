#include <ui/neural_nets_app.hpp>

#include <QMenuBar>
#include <QStatusBar>
#include <QFileDialog>
#include <QMessageBox>

#include <QStandardPaths>
#include <QFile>

NeuralNetsApp::NeuralNetsApp( QWidget* parent ) :
    QMainWindow( parent )
{   
    /// Menu.
    QMenuBar* menuBar = new QMenuBar();
    this->setMenuBar( menuBar );

    /// File menu.
    QMenu* fileMenu = new QMenu( "File" );
    menuBar->addMenu( fileMenu );
    auto openAction = new QAction( "Open neural network" );
    auto saveAction = new QAction( "Save neural network" );
    auto openImageAction = new QAction( "Open image" );
    auto saveImageAction = new QAction( "Save image" );
    connect( openAction, &QAction::triggered,
             this, &NeuralNetsApp::openNeuralNetwork );
    connect( saveAction, &QAction::triggered,
             this, &NeuralNetsApp::saveNeuralNetwork );
    connect( openImageAction, &QAction::triggered,
             this, &NeuralNetsApp::openImage );
    connect( saveImageAction, &QAction::triggered,
             this, &NeuralNetsApp::saveImage );
    fileMenu->addAction( openAction );
    fileMenu->addAction( saveAction );
    fileMenu->addAction( openImageAction );
    fileMenu->addAction( saveImageAction );

    /// Edit menu.
    QMenu* editMenu = new QMenu( "Edit" );
    menuBar->addMenu( editMenu );
    auto editTypeAction = new QAction( "Change neural network type" );
    auto editSizeAction = new QAction( "Resize image" );
    auto editNNeuronsAction = new QAction( "Change number of neurons" );
    connect( editTypeAction, &QAction::triggered,
             this, &NeuralNetsApp::editNeuralNetworkType );
    connect( editSizeAction, &QAction::triggered,
             this, &NeuralNetsApp::editImageSize );
    connect( editNNeuronsAction, &QAction::triggered,
             this, &NeuralNetsApp::editNumberOfNeurons );
    editMenu->addAction( editTypeAction );
    editMenu->addAction( editSizeAction );
    editMenu->addAction( editNNeuronsAction );

    /// Status bar.
    QStatusBar* statusBar = new QStatusBar();
    this->setStatusBar( statusBar );

    /// Serializer.
    serializer = new NeuralNetworkSerializer();

    /// Main widget.
    nnWidget = new NeuralNetworkWidget();
    this->setCentralWidget( nnWidget );

    /// Params.
    this->setWindowTitle( "AI" );
}

NeuralNetsApp::~NeuralNetsApp()
{
}

void NeuralNetsApp::replaceNeuralNetworkWidget(
        NeuralNetworkWidget* newNeuralNetworkWidget )
{
    this->setCentralWidget( newNeuralNetworkWidget );
    delete nnWidget;
    nnWidget = newNeuralNetworkWidget;
}

void NeuralNetsApp::openNeuralNetwork()
{
    auto fileName = QFileDialog::getOpenFileName(
            this, "Open File",
            QStandardPaths::standardLocations( QStandardPaths::HomeLocation ).last(),
            "NeuralNetworkData (*.nnd )" );
    if( fileName.isNull() ) return;

    serializer->setDevice( std::make_unique< QFile >( fileName ) );
    auto data = serializer->deserialize();

    auto decodedMeta = NeuralNetworkWidget::readMeta( data.getMetaInformation() );
    replaceNeuralNetworkWidget(
            new NeuralNetworkWidget( decodedMeta.first,
                                     data.getNeuralNetworkLayers().front().getMatrixWidth(),
                                     decodedMeta.second ) );
    nnWidget->setNeuralNetworkData( data );
}

void NeuralNetsApp::saveNeuralNetwork()
{
    auto fileName = QFileDialog::getSaveFileName(
            this, "Save File",
            QStandardPaths::standardLocations( QStandardPaths::HomeLocation ).last(),
            "NeuralNetworkData (*.nnd )" );
    if( fileName.isNull() ) return;

    serializer->setDevice( std::make_unique< QFile >( fileName ) );
    serializer->serialize( nnWidget->getNeuralNetworkData() );
}

void NeuralNetsApp::openImage()
{
    auto fileName = QFileDialog::getOpenFileName(
            this, "Open File",
            QStandardPaths::standardLocations( QStandardPaths::HomeLocation ).last(),
            "Images (*.png *.xpm *.jpg)" );
    if( fileName.isNull() ) return;

    QImage image( fileName );

    if( image.size() != nnWidget->getSampleImageSize() )
    {
        auto result = QMessageBox::question(
                    this, "Wrong size",
                    "Given image has incorrect size. Resize it?",
                    QMessageBox::Yes | QMessageBox::No );

        if( result != QMessageBox::Yes ) return;
        image = image.scaled( nnWidget->getSampleImageSize() );
    }

    nnWidget->setSampleImage( image.convertToFormat( QImage::Format_Mono ) );
}

void NeuralNetsApp::saveImage()
{
    auto fileName = QFileDialog::getSaveFileName(
            this, "Save File",
            QStandardPaths::standardLocations( QStandardPaths::HomeLocation ).last(),
            "Images (*.png *.xpm *.jpg)" );
    if( fileName.isNull() ) return;

    auto image = nnWidget->getSampleImage();
    image.save( fileName );
}

void NeuralNetsApp::editNeuralNetworkType()
{

}

void NeuralNetsApp::editImageSize()
{

}

void NeuralNetsApp::editNumberOfNeurons()
{

}
