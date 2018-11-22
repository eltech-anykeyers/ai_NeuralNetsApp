#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <ui/hebbian_neural_network_widget.hpp>
#include <utils/NeuralNetworkSerializer/neural_network_serializer.hpp>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow( QWidget* parent = Q_NULLPTR );
    ~MainWindow();

public slots:
    void openNeuralNetwork();
    void saveNeuralNetwork();
    void openImage();
    void saveImage();

protected:
    void replaceNeuralNetworkWidget(
            HebbianNeuralNetworkWidget* newNeuralNetworkWidget );

private:
    HebbianNeuralNetworkWidget* hebbian;
    NeuralNetworkSerializer* serializer;
};

#endif /// MAINWINDOW_H
