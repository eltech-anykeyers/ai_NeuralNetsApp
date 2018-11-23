#-------------------------------------------------
#
# Project created by QtCreator 2018-09-29T15:26:33
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = 7sem_ai_lab8
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS

CONFIG += c++1z
QMAKE_CXXFLAGS += -std=c++1z

SOURCES += \
        main.cpp \
        neural_nets/hebbian_neural_network.cpp \
        neural_nets/hamming_neural_network.cpp \
        ui/neural_nets_app.cpp \
        ui/neural_network_widget.cpp \
        ui/GridDrawer/grid_drawer.cpp \
        ui/GridDrawer/marked_drawer.cpp \
        ui/ImageListViewWidget/image_list_model.cpp \
        ui/ImageListViewWidget/image_list_item_delegate.cpp \
        ui/ImageListViewWidget/image_list_view_widget.cpp \
        utils/data_converters.cpp \
        utils/activation_functions.cpp \
        utils/NeuralNetworkSerializer/neural_network_serializer.cpp \
        utils/NeuralNetworkSerializer/neural_network_data.cpp \
        utils/NeuralNetworkSerializer/neural_network_learning_sample.cpp

HEADERS += \
        neural_nets/hebbian_neural_network.hpp \
        neural_nets/hamming_neural_network.hpp \
        ui/neural_nets_app.hpp \
        ui/neural_network_widget.hpp \
        ui/GridDrawer/grid_drawer.hpp \
        ui/GridDrawer/marked_drawer.hpp \
        ui/ImageListViewWidget/image_list_model.hpp \
        ui/ImageListViewWidget/image_list_item_delegate.hpp \
        ui/ImageListViewWidget/image_list_view_widget.hpp \
        utils/data_converters.hpp \
        utils/activation_functions.hpp \
        utils/NeuralNetworkSerializer/neural_network_serializer.hpp \
        utils/NeuralNetworkSerializer/neural_network_data.hpp \
        utils/NeuralNetworkSerializer/neural_network_learning_sample.hpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
