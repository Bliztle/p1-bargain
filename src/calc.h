#include "api/parse.h"

#define EARTH_RADIUS_KILOMETERS 6371

double calc_coordinate_distance(coord_lat_t lat1, coord_lon_t lon1, coord_lat_t lat2, coord_lon_t lon2);
double calc_degrees_to_radians(double degrees);