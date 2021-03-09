#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <lua.h>

extern lua_State * L;

#define reset "\x1B[0m"
#define red "\x1B[31m"

#define bubble_fg "\x1b[37m"
#define fg "\x1b[30m"
#define bg "\x1b[47m"
#define bubble_left ""
#define bubble_right ""

void script_init(void);
void script_run(char * filename);
void script_destroy(void);
void func_reg(void);
char * config_location(void);
