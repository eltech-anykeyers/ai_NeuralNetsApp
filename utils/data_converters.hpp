#ifndef DATA_CONVERTERS_HPP
#define DATA_CONVERTERS_HPP

#include <QVector>
#include <QColor>
#include <QImage>

#include <functional>

/// Convertation functions.
namespace converters {

qreal colorToBinary( const QColor& color );
qreal colorToBipolar( const QColor& color );

} /// namespace converters

class DataConverters
{
public:
    static QVector< qreal > convertImage(
            const QImage& img, std::function< qreal(const QColor&) > converter );
};

#endif /// DATA_CONVERTERS_HPP
