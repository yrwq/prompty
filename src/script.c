#include "prompty.h"
#include <lualib.h>
#include <lauxlib.h>
#include <stdarg.h>

lua_State * L;

/* Initialize lua vm */
void script_init(void){
    L = luaL_newstate();
    luaL_openlibs(L);
}

/* Default config file location */
char * config_location(void) {
    char * xdg_conf = getenv("XDG_CONFIG_HOME");

    return xdg_conf
        ? strcat(xdg_conf, "/prompty/init.lua")
        : strcat(getenv("HOME"), "/.config/prompty/init.lua");
}


/* Run prompty.init() from config */
void script_run(char * filename){

    /* Load config */
    (luaL_loadfile(L, filename) || lua_pcall(L, 0, LUA_MULTRET, 0));

    /* Look for a table called 'prompty' */
    lua_getfield(L, LUA_GLOBALSINDEX, "prompty");
    if(!lua_istable(L, -1)){
        fprintf(stderr, "Global prompty is not a table");
        return;
    }

    /* Look for a function called 'init' in table 'prompty' */
    lua_getfield(L, -1, "init");
    if(!lua_isfunction(L, -1)){
        fprintf(stderr, "Global prompty.init is not a function");
        return;
    }

    /* Run the function */
    lua_pcall(L, 0, 0, 0);

    lua_pop(L, 1);
}

/* Close lua vm */
void script_destroy(void){
    lua_close(L);
}
