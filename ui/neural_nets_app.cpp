#include <ui/neural_nets_app.hpp>

#include <QMenuBar>
#include <QStatusBar>
#include <QFileDialog>
#include <QMessageBox>
#include <QInputDialog>

#include <QStandardPaths>
#include <QFile>
#include <QStringList>

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
    static QHash< QString, NeuralNetType > types
        { { "Hebbian Neural Network", NeuralNetType::HEBBIAN },
          { "Hamming Neural Network", NeuralNetType::HAMMING } };

    QStringList typesList;
    for( const auto& typesItem : types.keys() )
    {
        typesList << typesItem;
    }

    bool ok;
    auto typeKey = QInputDialog::getItem(
                this, "Nural network type",
                "Choose type of neural network",
                typesList, 0, false, &ok );

    if( !ok || typeKey.isNull() ) return;

    replaceNeuralNetworkWidget(
                new NeuralNetworkWidget( types[ typeKey ],
                                         nnWidget->getNumberOfNeurons(),
                                         nnWidget->getSampleImageSize() ) );
}

void NeuralNetsApp::editImageSize()
{
    bool ok;
    auto width = QInputDialog::getInt(
                this, "Width",
                "Enter image width",
                nnWidget->getSampleImageSize().width(),
                2, 10, 1, &ok );
    if( !ok ) return;

    auto height = QInputDialog::getInt(
                this, "Height",
                "Enter image height",
                nnWidget->getSampleImageSize().height(),
                2, 10, 1, &ok );
    if( !ok ) return;

    replaceNeuralNetworkWidget(
                new NeuralNetworkWidget( nnWidget->getNeuralNetworkType(),
                                         nnWidget->getNumberOfNeurons(),
                                         QSize( width, height ) ) );
}

void NeuralNetsApp::editNumberOfNeurons()
{
    bool ok;
    auto nNeurons = QInputDialog::getInt(
                this, "Number of neurons",
                "Enter number of neurons",
                qint32( nnWidget->getNumberOfNeurons() ),
                2, 10, 1, &ok );
    if( !ok ) return;

    replaceNeuralNetworkWidget(
                new NeuralNetworkWidget( nnWidget->getNeuralNetworkType(),
                                         nNeurons,
                                         nnWidget->getSampleImageSize() ) );
}
