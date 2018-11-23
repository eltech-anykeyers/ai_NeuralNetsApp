#include <ui/neural_nets_app.hpp>
#include <QApplication>

int main( int argc, char *argv[] )
{
    QApplication a( argc, argv );
    NeuralNetsApp w;
    w.show();

    return a.exec();
}
