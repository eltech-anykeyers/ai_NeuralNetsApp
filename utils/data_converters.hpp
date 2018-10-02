#ifndef DATA_CONVERTERS_HPP
#define DATA_CONVERTERS_HPP

#include <QVector>
#include <QColor>

class DataConverters
{
public:
    static QVector< qreal > ConvertPixelDataIntoBinary( const QVector< QColor >& data );
    static QVector< qreal > ConvertPixelDataIntoBipolar( const QVector< QColor >& data );
};

#endif /// DATA_CONVERTERS_HPP
