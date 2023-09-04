#ifndef COLOR_H
#define COLOR_H

#include "interval.hpp"
#include "vec3.hpp"

#include <iostream>

inline double linear_to_gamma(double linear_component) {
    return sqrt(linear_component);
}

void write_color(std::ostream &out, color pixel_color, int samples_per_pixel) {
    auto scale = 1.0 / samples_per_pixel;
    auto r = pixel_color.x() * scale;
    auto g = pixel_color.y() * scale;
    auto b = pixel_color.z() * scale;

    r = linear_to_gamma(r);
    g = linear_to_gamma(g);
    b = linear_to_gamma(b);

    static const interval intensity(0.000, 0.999);

    out << static_cast<int>(256 * intensity.clamp(r)) << ' '
        << static_cast<int>(256 * intensity.clamp(g)) << ' '
        << static_cast<int>(256 * intensity.clamp(b)) << '\n';
}

#endif
