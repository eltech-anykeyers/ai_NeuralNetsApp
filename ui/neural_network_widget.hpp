#ifndef NEURAL_NETWORK_WIDGET_HPP
#define NEURAL_NETWORK_WIDGET_HPP

#include <QWidget>
#include <QVector>

#include <memory>

#include <ui/GridDrawer/marked_drawer.hpp>
#include <ui/GridDrawer/grid_drawer.hpp>
#include <ui/ImageListViewWidget/image_list_view_widget.hpp>
#include <neural_nets/hebbian_neural_network.hpp>
#include <utils/NeuralNetworkSerializer/neural_network_data.hpp>

#include <QLabel>

class NeuralNetworkWidget : public QWidget
{
    Q_OBJECT
public:
    explicit NeuralNetworkWidget( QWidget* parent = Q_NULLPTR );
    explicit NeuralNetworkWidget( quint32 nNeurons, QSize sampleSize,
                                         QWidget* parent = Q_NULLPTR );

    NeuralNetworkData getNeuralNetworkData() const;
    void setNeuralNetworkData( const NeuralNetworkData& data );

    QImage getSampleImage() const;
    void setSampleImage( const QImage& image );

    QSize getSampleImageSize() const;
    void setSampleImageSize( const QSize& size );

public slots:
    void learn();
    void test();
    void clear();

private:
    struct Sample
    {
        std::shared_ptr< QImage > image;
        QVector< qreal > target;
        QString mark;
    };

    QVector< Sample > samples;

    MarkedDrawer* sampleDrawer;

    ImageListViewWidget* imageListWidget;

    QLabel* resultLabel;

    std::shared_ptr< INeuralNetwork > neuralNetwork;

    const quint32 N_NEURONS;
    const QSize SAMPLE_SIZE;
};

#endif /// NEURAL_NETWORK_WIDGET_HPP
