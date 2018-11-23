#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <ui/neural_network_widget.hpp>
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
            NeuralNetworkWidget* newNeuralNetworkWidget );

private:
    NeuralNetworkWidget* nnWidget;
    NeuralNetworkSerializer* serializer;
};

#endif /// MAINWINDOW_H
