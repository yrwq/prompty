#include "prompty.h"

int main(int argc, char ** argv){

    /* Create a lua vm */
    script_init();

    /* Register lua functions */
    func_reg();

    /* Load configuration file */
    if(argc != 2) {
        char * config_file = config_location();

        if( access( config_file, F_OK ) != -1 ) {
            script_run(config_file);
        } else {
            printf("\n%sprompty%s: %s doesn't exist!\n\n", red, reset, config_file);
        }
    } else script_run(argv[1]);

    /* Destroy lua vm */
    script_destroy();

    return 0;
}


