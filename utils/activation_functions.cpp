#include "activation_functions.hpp"

namespace activation {

double binary( double signal )
{
    return signal > 0.0 ? 1.0 : 0.0;
}

double bipolar( double signal )
{
    return signal > 0.0 ? 1.0 : -1.0;
}

} /// namespace activation
