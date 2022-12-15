#include "bargain.h"

char *CURRENT_DIRECTORY;

int main(int argc, char **args)
{
    CURRENT_DIRECTORY = args[0];
    conf_setup();
    bargain_run_bargain();

    return 0;
}
