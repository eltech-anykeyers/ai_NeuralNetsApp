#ifndef HEBBIANNEURALNETWORKWIDGET_HPP
#define HEBBIANNEURALNETWORKWIDGET_HPP

#include <QWidget>
#include <QVector>

#include <memory>

#include <ui/GridDrawer/marked_drawer.hpp>
#include <ui/GridDrawer/grid_drawer.hpp>
#include <neural_nets/hebbian_neural_network.hpp>

#include <QLabel>

class HebbianNeuralNetworkWidget : public QWidget
{
    Q_OBJECT
public:
    explicit HebbianNeuralNetworkWidget( QWidget* parent = Q_NULLPTR );

public slots:
    void learn();
    void test();
    void clear();

private:
    constexpr static int N_SAMPLES = 4;
    constexpr static QSize SAMPLE_SIZE = QSize( 4, 5 );
    constexpr static int LEARNING_LAYOUT_WIDTH = 2;

    QVector< MarkedDrawer* > learningDrawers;
    GridDrawer* testingDrawer;

    QLabel* resultLabel;

    std::shared_ptr< HebbianNeuralNetwork > hebbianNet;
};

#endif /// HEBBIANNEURALNETWORKWIDGET_HPP
