#include "data_converters.hpp"

namespace converters {

qreal colorToBinary( const QColor& color )
{
    return color == Qt::GlobalColor::white ? 0.0 : 1.0;
}

qreal colorToBipolar( const QColor& color )
{
    return color == Qt::GlobalColor::white ? -1.0 : 1.0;
}

} /// namespace converters


QVector< qreal > DataConverters::convertImage(
        const QImage& img, std::function< qreal(const QColor&) > converter )
{
    if( img.isNull() ) return QVector< qreal >();

    QVector< qreal > result;
    for( int i = 0; i < img.width(); i++ )
    {
        for( int j = 0; j < img.height(); j++ )
        {
            result.append( converter( img.pixelColor( i, j ) ) );
        }
    }
    return result;
}
