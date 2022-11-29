#include <assert.h>
#include "calc.h"
#include <math.h>

int main()
{

    int calc_test_one = (calc_degrees_to_radians(45) == M_PI_4);
    assert(calc_test_one);

    int calc_test_two = (calc_degrees_to_radians(90) == M_PI_2);
    assert(calc_test_two);

    int calc_test_three = (calc_degrees_to_radians(180) == M_PI);
    assert(calc_test_three);


    // Bliztle to Bilka
    double distance = calc_coordinate_distance(57.025760, 9.958440, 57.004687, 9.876099);
    assert(5.507 < distance && distance < 5.508);
    return 0;
}
