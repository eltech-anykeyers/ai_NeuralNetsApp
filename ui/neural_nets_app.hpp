#ifndef NEURAL_NETS_APP_HPP
#define NEURAL_NETS_APP_HPP

#include <QMainWindow>

#include <ui/neural_network_widget.hpp>
#include <utils/NeuralNetworkSerializer/neural_network_serializer.hpp>

class NeuralNetsApp : public QMainWindow
{
    Q_OBJECT

public:
    explicit NeuralNetsApp( QWidget* parent = Q_NULLPTR );
    ~NeuralNetsApp();

public slots:
    void openNeuralNetwork();
    void saveNeuralNetwork();
    void openImage();
    void saveImage();

protected:
    void replaceNeuralNetworkWidget(
            NeuralNetworkWidget* newNeuralNetworkWidget );

private:
    NeuralNetworkWidget* nnWidget;
    NeuralNetworkSerializer* serializer;
};

#endif /// NEURAL_NETS_APP_HPP
