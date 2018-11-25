#ifndef NEURAL_NETWORK_WIDGET_HPP
#define NEURAL_NETWORK_WIDGET_HPP

#include <QWidget>
#include <QVector>

#include <memory>
#include <vector>

#include <neural_nets/hebbian_neural_network.hpp>
#include <neural_nets/hamming_neural_network.hpp>
#include <ui/GridDrawer/marked_drawer.hpp>
#include <ui/GridDrawer/grid_drawer.hpp>
#include <ui/ImageListViewWidget/image_list_view_widget.hpp>
#include <utils/NeuralNetworkSerializer/neural_network_data.hpp>

#include <QLabel>

enum class NeuralNetType : quint32
{
    HEBBIAN,
    HAMMING
};

class NeuralNetworkWidget : public QWidget
{
    Q_OBJECT
public:
    explicit NeuralNetworkWidget( QWidget* parent = Q_NULLPTR );
    explicit NeuralNetworkWidget( const NeuralNetType type,
                                  const quint32 N_NEURONS, const QSize& SAMPLE_SIZE,
                                  QWidget* parent = Q_NULLPTR );

    NeuralNetworkData getNeuralNetworkData() const;
    void setNeuralNetworkData( const NeuralNetworkData& data );

    QImage getSampleImage() const;
    void setSampleImage( const QImage& image );

    const QSize& getSampleImageSize() const;
    quint32 getNumberOfNeurons() const;
    NeuralNetType getNeuralNetworkType() const;

    static QByteArray createMeta( const NeuralNetType type, const QSize& size );
    static QPair< NeuralNetType, QSize > readMeta( const QByteArray& meta );

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

    static const char* META_HEADER;

    QVector< Sample > samples;

    MarkedDrawer* sampleDrawer;

    ImageListViewWidget* imageListWidget;

    QLabel* resultLabel;

    std::shared_ptr< INeuralNetwork > neuralNetwork;

    const quint32 N_NEURONS;
    const QSize SAMPLE_SIZE;
    const NeuralNetType NEURAL_NETWORK_TYPE;
};

#endif /// NEURAL_NETWORK_WIDGET_HPP
