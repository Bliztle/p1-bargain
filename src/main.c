#include <stdio.h>
#include "basket.h"

char *CURRENT_DIRECTORY;

int main(int argc, char **args)
{
    menu_basket_edit();
    CURRENT_DIRECTORY = args[0];
    printf("Hello, World!\n");
    return 0;
}
