#include <ui/hebbian_neural_network_widget.hpp>
#include <utils/data_converters.hpp>
#include <ui/ImageViewWidget/image_view_widget.hpp>

#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QString>
#include <QListWidget>
#include <QScrollArea>

HebbianNeuralNetworkWidget::HebbianNeuralNetworkWidget( QWidget *parent )
    : QWidget( parent )
{
    /// Create neural network.
    hebbianNet = std::make_shared< HebbianNeuralNetwork >(
                SAMPLE_SIZE.height() * SAMPLE_SIZE.width(), N_SAMPLES );

    /// Crate layout with learning drawers.
    QGridLayout* learningLayout = new QGridLayout();
    for( int i = 0; i < N_SAMPLES; i++ )
    {
        auto newDrawer = new MarkedDrawer( SAMPLE_SIZE );
        learningDrawers.append( newDrawer );
        learningLayout->addWidget(
            newDrawer,
            i / LEARNING_LAYOUT_WIDTH, i % LEARNING_LAYOUT_WIDTH );
    }
    learningLayout->setSpacing( 10 );

    /// Create layout with testingDrawer and control buttons.
    QVBoxLayout* toolsLayout = new QVBoxLayout();
    testingDrawer = new GridDrawer( SAMPLE_SIZE );
    toolsLayout->addWidget( testingDrawer );

    /// Create result label.
    resultLabel = new QLabel();
    toolsLayout->addWidget( resultLabel );

    /// Create buttons.
    QPushButton* learnPushButton = new QPushButton( "Learn" );
    toolsLayout->addWidget( learnPushButton );
    connect( learnPushButton, &QPushButton::clicked,
             this, &HebbianNeuralNetworkWidget::learn );
    QPushButton* testPushButton = new QPushButton( "Test" );
    connect( testPushButton, &QPushButton::clicked,
             this, &HebbianNeuralNetworkWidget::test );
    toolsLayout->addWidget( testPushButton );
    QPushButton* clearPushButton = new QPushButton( "Clear" );
    connect( clearPushButton, &QPushButton::clicked,
             this, &HebbianNeuralNetworkWidget::clear );
    toolsLayout->addWidget( clearPushButton );

    /// Create main layout.
    QHBoxLayout* mainLayout = new QHBoxLayout();
    mainLayout->addLayout( learningLayout );
    mainLayout->addLayout( toolsLayout );
    this->setLayout( mainLayout );
}

void HebbianNeuralNetworkWidget::learn()
{
    QVector< qreal > target( N_SAMPLES, 0.0 );
    for( int i = 0; i < N_SAMPLES; i++ )
    {
        target[ i ] = 1.0;
        auto input =
            DataConverters::ConvertPixelDataIntoBinary(
                learningDrawers[ i ]->getPixelData() );
        hebbianNet->addLearningDataSet(
            input.toStdVector(), target.toStdVector() );
        target[ i ] = 0.0;
    }
    hebbianNet->learn();
}

void HebbianNeuralNetworkWidget::test()
{
    auto input =
        DataConverters::ConvertPixelDataIntoBinary(
            testingDrawer->getPixelData() );
    QVector< qreal > result = QVector< qreal >::fromStdVector(
        hebbianNet->test( input.toStdVector() ) );

    int index = -1;
    for( int i = 0; i < result.size(); i++ )
    {
        if( result[ i ] > 0.0 )
        {
            if( index == -1 ) index = i;
            else
            {
                resultLabel->setText( "Bad result" );
                return;
            }
        }
    }
    if( index == -1 )
    {
        resultLabel->setText( "Bad result" );
        return;
    }

    resultLabel->setText( QString("Match found: %1")
                            .arg( learningDrawers[ index ]->getMark() ) );
}

void HebbianNeuralNetworkWidget::clear()
{
    hebbianNet->clear();
    testingDrawer->refresh();
    for( auto& drawer : learningDrawers )
    {
        drawer->refresh();
    }
}
