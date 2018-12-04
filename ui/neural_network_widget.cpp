#include <ui/neural_network_widget.hpp>
#include <utils/data_converters.hpp>
#include <utils/NeuralNetworkSerializer/neural_network_learning_sample.hpp>

#include <QtMath>

#include <QFile>
#include <QHash>

#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QString>
#include <QListWidget>

const char* NeuralNetworkWidget::META_HEADER = "NNW_0.0.1";

uint qHash( const NeuralNetType& val )
{
    return qHash( static_cast< quint32 >( val ) );
}

qreal distance( const QVector< qreal >& v1, const QVector< qreal >& v2 )
{
    qreal result = 0.0;
    for( qint32 i = 0; i < v1.size(); ++i )
    {
        result += qPow( v2[ i ] - v1[ i ], 2.0 );
    }
    return qSqrt( result );
}

NeuralNetworkWidget::NeuralNetworkWidget( QWidget* parent )
    : NeuralNetworkWidget( NeuralNetType::HEBBIAN, 4, QSize( 4, 5 ), parent )
{}

NeuralNetworkWidget::NeuralNetworkWidget(
        const NeuralNetType type,
        const quint32 nNeurons, const QSize& sampleSize,
        QWidget* parent )
    : QWidget( parent )
    , N_NEURONS( nNeurons )
    , SAMPLE_SIZE( sampleSize )
    , NEURAL_NETWORK_TYPE( type )
{    
    /// Create neural network.
    switch( NEURAL_NETWORK_TYPE )
    {
        case NeuralNetType::HEBBIAN :
        {
            neuralNetwork = std::make_shared< HebbianNeuralNetwork >(
                        sampleSize.height() * sampleSize.width(), nNeurons );
            break;
        }
        case NeuralNetType::HAMMING :
        {
            neuralNetwork = std::make_shared< HammingNeuralNetwork >(
                        sampleSize.height() * sampleSize.width(), nNeurons );
            break;
        }
    }

    /// Create layout with sampleDrawer and control buttons.
    QVBoxLayout* toolsLayout = new QVBoxLayout();
    sampleDrawer = new MarkedDrawer( sampleSize );
    toolsLayout->addWidget( sampleDrawer );

    /// Create info layout.
    QHBoxLayout* infoLayout = new QHBoxLayout();

    static QHash< NeuralNetType, QString > typesNames
        { { NeuralNetType::HEBBIAN, "Hebbian Neural Network" },
          { NeuralNetType::HAMMING, "Hamming Neural Network" } };


    /// Create result label.
    auto infoLabel = new QLabel(
                QString( "Info:\n"
                         "Type: %1\n"
                         "Number of neurons: %2\n"
                         "Image size: %3x%4" )
                .arg( typesNames[ NEURAL_NETWORK_TYPE ] )
                .arg( N_NEURONS )
                .arg( SAMPLE_SIZE.width() )
                .arg( SAMPLE_SIZE.height() ) );
    infoLayout->addWidget( infoLabel );

    /// Create result label.
    resultLabel = new QLabel();
    infoLayout->addWidget( resultLabel );

    /// Create buttons.
    QPushButton* learnPushButton = new QPushButton( "Learn" );
    toolsLayout->addWidget( learnPushButton );
    connect( learnPushButton, &QPushButton::clicked,
             this, &NeuralNetworkWidget::learn );
    QPushButton* testPushButton = new QPushButton( "Test" );
    connect( testPushButton, &QPushButton::clicked,
             this, &NeuralNetworkWidget::test );
    toolsLayout->addWidget( testPushButton );
    QPushButton* clearPushButton = new QPushButton( "Clear" );
    connect( clearPushButton, &QPushButton::clicked,
             this, &NeuralNetworkWidget::clear );
    toolsLayout->addWidget( clearPushButton );
    QPushButton* generatePushButton = new QPushButton( "Generate" );
    toolsLayout->addWidget( generatePushButton );
    connect( generatePushButton, &QPushButton::clicked,
             sampleDrawer, &MarkedDrawer::generate );

    /// ImageListView
    imageListWidget = new ImageListViewWidget();

    /// Create main layout.
    QHBoxLayout* neuralNetsLayout = new QHBoxLayout();
    neuralNetsLayout->addLayout( toolsLayout );
    neuralNetsLayout->addWidget( imageListWidget );

    QVBoxLayout* mainLayout = new QVBoxLayout();
    mainLayout->addLayout( infoLayout );
    mainLayout->addLayout( neuralNetsLayout );
    this->setLayout( mainLayout );
}

void NeuralNetworkWidget::learn()
{
    if( quint32( imageListWidget->model()->size() ) >= N_NEURONS )
    {
        return;
    }

    /// Get target.
    /// TODO: Create hashing function.
    auto target = QVector< qreal >( qint32( N_NEURONS ), 0.0 );
    auto nSamples = imageListWidget->model()->size();
    if( nSamples >= 0 )
    {
        target[ nSamples ] = 1.0;
    }

    auto sample = Sample{ std::make_shared< QImage >( sampleDrawer->getImage() ),
                          target, sampleDrawer->getMark() };

    imageListWidget->model()->addImage( sample.image, sample.mark );

    /// Get input.
    QVector< qreal > input;

    switch( NEURAL_NETWORK_TYPE )
    {
        case NeuralNetType::HAMMING :
        {
            input = DataConverters::convertImage( sampleDrawer->getImage(),
                                                  converters::colorToBipolar );
        } break;
        case NeuralNetType::HEBBIAN :
        {
            input = DataConverters::convertImage( sampleDrawer->getImage(),
                                                  converters::colorToBinary );
        } break;
    }

    neuralNetwork->addSampleToLearningDataSet(
        input.toStdVector(), target.toStdVector() );
    neuralNetwork->adjustConnectionsWeights();

    samples.append( std::move( sample ) );

    sampleDrawer->refresh();
}

void NeuralNetworkWidget::test()
{
    QVector< qreal > input;

    switch( NEURAL_NETWORK_TYPE )
    {
        case NeuralNetType::HAMMING :
        {
            input = DataConverters::convertImage( sampleDrawer->getImage(),
                                                  converters::colorToBipolar );
        } break;
        case NeuralNetType::HEBBIAN :
        {
            input = DataConverters::convertImage( sampleDrawer->getImage(),
                                                  converters::colorToBinary );
        } break;
    }

    QVector< qreal > result = QVector< qreal >::fromStdVector(
        neuralNetwork->recognizeSample( input.toStdVector() ) );

    if( quint32( result.size() ) != N_NEURONS ) throw std::runtime_error( "bad result" );

    QVector< QPair< QString, qreal > > results;
    for( const auto& sample : samples )
    {
        results.append( qMakePair( sample.mark,
                                   distance( sample.target, result ) ) );
    }
    std::sort( results.begin(), results.end(),
               []( QPair< QString, qreal > a, QPair< QString, qreal > b ) -> bool
               { return a.second < b.second; } );

    QString resultsText = results.front().second == 0.0 ?
                          QString("Match found: %1.\n").arg( results.front().first ) :
                          QString("Match not found.\n");
    for( const auto& result : results )
    {
        resultsText += QString( "%1 : %2\n" )
                       .arg( result.first )
                       .arg( result.second );
    }
    resultLabel->setText( resultsText );
}

void NeuralNetworkWidget::clear()
{
    neuralNetwork->clear();
    sampleDrawer->refresh();
    imageListWidget->model()->clear();
    samples.clear();
}

NeuralNetworkData NeuralNetworkWidget::getNeuralNetworkData() const
{
    NeuralNetworkData data;

    auto weightsMatrices = neuralNetwork->getWeightsMatrices();
    for( const auto& weightsMatrix : weightsMatrices )
    {
        NeuralNetworkWeightsMatrix layer( QSize( qint32( weightsMatrix.width ),
                                                 qint32( weightsMatrix.height ) ) );

        for( quint32 i = 0; i < weightsMatrix.width; ++i )
        {
            for( quint32 j = 0; j < weightsMatrix.height + 1; ++j )
            {
                layer.setRelationshipWeight( i, j, weightsMatrix.matrix[ i ][ j ] );
            }
            delete[] weightsMatrix.matrix[ i ];
        }
        delete[] weightsMatrix.matrix;

        data.addNeuralNetworkLayer( layer );
    }

    for( const auto& sourceSample : samples )
    {
        NeuralNetworkLearningSample sample;

        /// Target.
        auto targetSize = quint32( sourceSample.target.size() );
        auto target = new qreal[ targetSize ];
        std::copy( sourceSample.target.begin(), sourceSample.target.end(), target );
        sample.setTargetVector( reinterpret_cast< uchar* >( target ),
                                sizeof(qreal) * targetSize );

        /// Input.
        auto inputSize = quint32( sourceSample.image->sizeInBytes() );
        auto input = new uchar[ inputSize ];
        std::copy( sourceSample.image->bits(),
                   sourceSample.image->bits() + inputSize,
                   input );
        sample.setInputVector( input, inputSize );

        /// Mark.
        sample.setMark( sourceSample.mark );

        data.addLearningData( sample );
    }

    data.setMetaInformation( createMeta( NEURAL_NETWORK_TYPE, SAMPLE_SIZE ) );

    return data;
}

void NeuralNetworkWidget::setNeuralNetworkData( const NeuralNetworkData& data )
{
    auto decodedMeta = readMeta( data.getMetaInformation() );

    if( NEURAL_NETWORK_TYPE != decodedMeta.first ) throw std::invalid_argument( "invalid network type" );
    if( SAMPLE_SIZE != decodedMeta.second ) throw std::invalid_argument( "invalid image size" );

    switch( NEURAL_NETWORK_TYPE )
    {
        case NeuralNetType::HEBBIAN :
        {
            const auto& layer = data.getNeuralNetworkLayers().front();
            auto weights = new qreal*[ layer.getMatrixWidth() ];
            for( quint32 i = 0; i < layer.getMatrixWidth(); ++i )
            {
                weights[ i ] = new qreal[ layer.getMatrixHeight() ];
                for( quint32 j = 0; j < layer.getMatrixHeight(); ++j )
                {
                    weights[ i ][ j ] = layer.getRelationshipWeight( i, j );
                }
            }

            neuralNetwork = std::make_shared< HebbianNeuralNetwork >(
                        layer.getMatrixWidth(), layer.getMatrixHeight(),
                        weights );
            break;
        }
        case NeuralNetType::HAMMING :
        {
            const auto& weightsLayer = data.getNeuralNetworkLayers()[ 0 ];
            std::vector< std::vector< double > > weights( weightsLayer.getMatrixWidth() );
            for( quint32 i = 0; i < weightsLayer.getMatrixWidth(); ++i )
            {
                weights[ i ] = std::vector< double >( weightsLayer.getMatrixHeight() );
                for( quint32 j = 0; j < weightsLayer.getMatrixHeight(); ++j )
                {
                    weights[ i ][ j ] = weightsLayer.getRelationshipWeight( i, j );
                }
            }
            const auto& feedbackLayer = data.getNeuralNetworkLayers()[ 1 ];
            std::vector< std::vector< double > > feedback( weightsLayer.getMatrixWidth() );
            for( quint32 i = 0; i < feedbackLayer.getMatrixWidth(); ++i )
            {
                feedback[ i ] = std::vector< double >( feedbackLayer.getMatrixHeight() );
                for( quint32 j = 0; j < feedbackLayer.getMatrixHeight(); ++j )
                {
                    feedback[ i ][ j ] = feedbackLayer.getRelationshipWeight( i, j );
                }
            }
            neuralNetwork = std::make_shared< HammingNeuralNetwork >(
                        weightsLayer.getMatrixWidth(), weightsLayer.getMatrixHeight(),
                        weights, feedback );
            break;
        }
    }

    for( const auto& sample : data.getLearningData() )
    {
        auto image = QImage( decodedMeta.second, QImage::Format::Format_Mono );
        std::copy( sample.getInputVector(),
                   sample.getInputVector() + qint32( sample.getInputVectorSize() ),
                   image.bits() );

        QVector< qreal > target;
        auto targetData = reinterpret_cast< qreal* >( sample.getTargetVector() );
        std::copy( targetData,
                   targetData + sample.getTargetVectorSize() / sizeof( qreal ),
                   std::back_inserter( target ) );

        auto result = Sample{ std::make_shared< QImage >( image ),
                              target, sample.getMark() };

        imageListWidget->model()->addImage( result.image, result.mark );

        samples.append( std::move( result ) );
    }
}

QImage NeuralNetworkWidget::getSampleImage() const
{
    return sampleDrawer->getImage();
}

void NeuralNetworkWidget::setSampleImage( const QImage& image )
{
    sampleDrawer->setImage( image );
}

const QSize& NeuralNetworkWidget::getSampleImageSize() const
{
    return SAMPLE_SIZE;
}

quint32 NeuralNetworkWidget::getNumberOfNeurons() const
{
    return N_NEURONS;
}

NeuralNetType NeuralNetworkWidget::getNeuralNetworkType() const
{
    return NEURAL_NETWORK_TYPE;
}

QByteArray NeuralNetworkWidget::createMeta(
        const NeuralNetType type, const QSize& size )
{
    QByteArray result;
    QDataStream stream( &result, QIODevice::WriteOnly );

    stream << META_HEADER;
    stream << quint32( type );
    stream << quint32( size.width() ) << quint32( size.height() );

    return result;
}

QPair< NeuralNetType, QSize > NeuralNetworkWidget::readMeta( const QByteArray& meta )
{
    QDataStream stream( meta );

    char* header;
    stream >> header;
    auto validated = strcmp( header, META_HEADER ) == 0;
    delete[] header;

    if( !validated ) throw std::invalid_argument( "headers doesn't match" );

    quint32 type, w, h;
    stream >> type >> w >> h;

    return qMakePair( NeuralNetType( type ),
                      QSize( qint32( w ), qint32( h ) ) );
}
