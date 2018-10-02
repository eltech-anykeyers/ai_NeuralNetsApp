#include "data_converters.hpp"

QVector< qreal > DataConverters::ConvertPixelDataIntoBinary(
    const QVector< QColor >& data )
{
    QVector< qreal > result;
    for( const auto& color : data )
    {
        result.push_back( color == Qt::GlobalColor::white ? 0.0 : 1.0 );
    }
    return result;
}

QVector< qreal > DataConverters::ConvertPixelDataIntoBipolar(
    const QVector< QColor >& data )
{
    QVector< qreal > result;
    for( const auto& color : data )
    {
        result.push_back( color == Qt::GlobalColor::white ? -1.0 : 1.0 );
    }
    return result;
}
