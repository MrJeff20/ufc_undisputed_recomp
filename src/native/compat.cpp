#include <cmath>

extern "C" float roundevenf(float value)
{
    if (!std::isfinite(value))
    {
        return value;
    }

    const float lower = std::floor(value);
    const float fraction = value - lower;
    if (fraction < 0.5F)
    {
        return lower;
    }
    if (fraction > 0.5F)
    {
        return lower + 1.0F;
    }
    return std::fmod(lower, 2.0F) == 0.0F ? lower : lower + 1.0F;
}