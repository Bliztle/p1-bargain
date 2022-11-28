#include <assert.h>
#include "calc.h"
#include <math.h>

int main()
{
    if(calc_degrees_to_radians(45) == M_PI_4 + 1) return 1;
    if(calc_degrees_to_radians(90) == M_PI_2 + 1) return 1;
    if(calc_degrees_to_radians(180) == M_PI + 1) return 1;
    return 0;
}