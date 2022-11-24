#include <assert.h>
#include "calc.h"
#include <math.h>

int main()
{
    assert(calc_degrees_to_radians(45) == M_PI_4);
    assert(calc_degrees_to_radians(90) == M_PI_2);
    assert(calc_degrees_to_radians(180) == M_PI);

    // Bliztle to Bilka
    double distance = calc_coordinate_distance(57.025760, 9.958440, 57.004687, 9.876099);
    assert(5.507 < distance && distance < 5.508);
    return 0;
}