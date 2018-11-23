#include <ui/neural_network_widget.hpp>
#include <utils/data_converters.hpp>
#include <utils/NeuralNetworkSerializer/neural_network_learning_sample.hpp>
#include <QFile>

#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QString>
#include <QListWidget>

NeuralNetworkWidget::NeuralNetworkWidget( QWidget* parent )
    : NeuralNetworkWidget( 4, QSize( 4, 5 ), parent )
{}

NeuralNetworkWidget::NeuralNetworkWidget(
        quint32 nNeurons, QSize sampleSize, QWidget* parent )
    : QWidget( parent )
    , N_NEURONS( nNeurons )
    , SAMPLE_SIZE( sampleSize )
{
    /// Create neural network.
    neuralNetwork = std::make_shared< HebbianNeuralNetwork >(
                SAMPLE_SIZE.height() * SAMPLE_SIZE.width(), N_NEURONS );

    /// Create layout with sampleDrawer and control buttons.
    QVBoxLayout* toolsLayout = new QVBoxLayout();
    sampleDrawer = new MarkedDrawer( SAMPLE_SIZE );
    toolsLayout->addWidget( sampleDrawer );

    /// Create result label.
    resultLabel = new QLabel();
    toolsLayout->addWidget( resultLabel );

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
    QHBoxLayout* mainLayout = new QHBoxLayout();
    mainLayout->addLayout( toolsLayout );
    mainLayout->addWidget( imageListWidget );
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
    auto input = DataConverters::convertImage( sampleDrawer->getImage(),
                                               converters::colorToBinary );


    neuralNetwork->addSampleToLearningDataSet(
        input.toStdVector(), target.toStdVector() );
    neuralNetwork->adjustConnectionsWeights();

    samples.append( std::move( sample ) );

    sampleDrawer->refresh();
}

void NeuralNetworkWidget::test()
{
    ///TODO: Reimplement.
    auto input = DataConverters::convertImage( sampleDrawer->getImage(),
                                               converters::colorToBinary );
    QVector< qreal > result = QVector< qreal >::fromStdVector(
        neuralNetwork->recognizeSample( input.toStdVector() ) );

    if( quint32( result.size() ) != N_NEURONS ) throw std::runtime_error( "bad result" );

    for( const auto& sample : samples )
    {
        if( std::equal( sample.target.begin(), sample.target.end(),
                        result.begin() ) )
        {
            resultLabel->setText(
                QString("Match found: %1").arg( sample.mark ) );
            return;
        }
    }

    resultLabel->setText( QString("Bad result") );
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
    NeuralNetworkData data( N_NEURONS, SAMPLE_SIZE );

    auto weights = neuralNetwork->getWeights();
    for( quint32 i = 0; i < N_NEURONS; i++ )
    {
        for( quint32 j = 0; j < data.getInputSize() + 1; j++ )
        {
            data.setRelationshipWeight( i, j, weights[ i ][ j ] );
        }
        delete[] weights[ i ];
    }
    delete[] weights;

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

    return data;
}

void NeuralNetworkWidget::setNeuralNetworkData( const NeuralNetworkData& data )
{
    if( N_NEURONS != data.getNumberOfNeurons() ) throw std::invalid_argument( "invalid number of neurons" );
    if( SAMPLE_SIZE != data.getImageSize() ) throw std::invalid_argument( "invalid image size" );

    auto weights = new qreal*[ N_NEURONS ];
    for( quint32 i = 0; i < N_NEURONS; i++ )
    {
        weights[ i ] = new qreal[ data.getInputSize() ];
        for( quint32 j = 0; j < data.getInputSize(); j++ )
        {
            weights[ i ][ j ] = data.getRelationshipWeight( i, j );
        }
    }

    neuralNetwork = std::make_shared< HebbianNeuralNetwork >(
                data.getNumberOfNeurons(), data.getInputSize() + 1,
                weights );

    for( const auto& sample : data.getLearningData() )
    {
        auto image = QImage( data.getImageSize(), QImage::Format::Format_Mono );
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

QSize NeuralNetworkWidget::getSampleImageSize() const
{
    return sampleDrawer->getSize();
}

void NeuralNetworkWidget::setSampleImageSize( const QSize& size )
{
    sampleDrawer->setSize( size );
}
