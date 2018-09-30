#ifndef HEBBIANNEURALNETWORKWIDGET_HPP
#define HEBBIANNEURALNETWORKWIDGET_HPP

#include <QWidget>
#include <QVector>

#include <ui/GridDrawer/markeddrawer.hpp>
#include <ui/GridDrawer/griddrawer.hpp>

class HebbianNeuralNetworkWidget : public QWidget
{
    Q_OBJECT
public:
    explicit HebbianNeuralNetworkWidget( QWidget* parent = Q_NULLPTR );

signals:

public slots:

private:
    constexpr static int N_SAMPLES = 4;
    constexpr static QSize SAMPLE_SIZE = QSize( 4, 5 );
    constexpr static int LEARNING_LAYOUT_WIDTH = 2;

    QVector< MarkedDrawer* > learningDrawers;
};

#endif /// HEBBIANNEURALNETWORKWIDGET_HPP
