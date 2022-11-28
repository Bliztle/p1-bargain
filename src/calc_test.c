#include <assert.h>
#include "calc.h"
#include <math.h>

int main()
{
    if(calc_degrees_to_radians(45) != M_PI_4) {
        printf("calc_degrees_to_radians(45) != M_PI_4 - Test failed\n");
        exit(1);
    }
    if(calc_degrees_to_radians(90) != M_PI_2) {
        printf("calc_degrees_to_radians(90) != M_PI_2 - Test failed\n");
        exit(1);
    }
    if(calc_degrees_to_radians(180) == M_PI) {
        printf("calc_degrees_to_radians(180) == M_PI - Test failed\n");
        exit(1);
    }
    return 0;
}