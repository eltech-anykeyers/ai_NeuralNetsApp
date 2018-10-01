#include <ui/hebbian_neural_network_widget.hpp>

#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>

HebbianNeuralNetworkWidget::HebbianNeuralNetworkWidget( QWidget *parent )
    : QWidget( parent )
{
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
    GridDrawer* testingDrawer = new GridDrawer( SAMPLE_SIZE );
    toolsLayout->addWidget( testingDrawer );
    toolsLayout->addStretch();

    /// Create buttons.
    QPushButton* learnPushButton = new QPushButton( "Learn" );
    toolsLayout->addWidget( learnPushButton );
    QPushButton* testPushButton = new QPushButton( "Test" );
    toolsLayout->addWidget( testPushButton );

    /// Create main layout.
    QHBoxLayout* mainLayout = new QHBoxLayout();
    mainLayout->addLayout( learningLayout );
    mainLayout->addLayout( toolsLayout );

    this->setLayout( mainLayout );
}
