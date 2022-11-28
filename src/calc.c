#include "api/parse.h"
#include "calc.h"
#include <math.h>

/**
 * Calculates the distance in meters between two coordinates
 *
 * @author Bliztle <asbjoern.r.e@gmail.com>
 *
 * @param lat1 latitude of the first coordinate
 * @param lon1 longitude of the first coordinate
 * @param lat2 latitude of the second coordinate
 * @param lon2 longitude of the second coordinate
 *
 * @returns the distance in meters
 */
double calc_coordinate_distance(coord_lat_t lat1, coord_lon_t lon1, coord_lat_t lat2, coord_lon_t lon2)
{
    double delta_lat = calc_degrees_to_radians(lat2 - lat1);
    double delta_lon = calc_degrees_to_radians(lon2 - lon1);
    double a = pow(sin(delta_lat / 2), 2) +
               cos(calc_degrees_to_radians(lat1)) * cos(calc_degrees_to_radians(lat2)) *
                   pow(sin(delta_lon), 2);
    double c = 2 * atan2(sqrt(a), sqrt(1 - a));
    return EARTH_RADIUS_KILOMETERS * c;
}

/**
 * Converts degrees to radians.
 *
 * @author Bliztle <asbjoern.r.e@gmail.com>
 *
 * @param degrees to change to radians
 *
 * @returns radian equavilent to degrees
 */
double calc_degrees_to_radians(double degrees)
{
    return degrees * M_PI / 180;
}