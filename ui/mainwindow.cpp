#include <ui/mainwindow.hpp>
#include <ui/GridDrawer/griddrawer.hpp>

MainWindow::MainWindow( QWidget* parent ) :
    QMainWindow( parent )
{
    GridDrawer* gd = new GridDrawer( QSize( 100, 100 ) );
    this->setCentralWidget( gd );
}

MainWindow::~MainWindow()
{
}
