#include "prompty.h"
#include <lauxlib.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#define LFUNC(N) int lua_##N(lua_State * L)

/* prompty.user() */
/* Returns 'USER' environment variable */
LFUNC(user){

    char * username = getenv("USER");
    if(username) {
        lua_pushstring(L, username);
    } else lua_pushstring(L, "unknown");

    return 1;
}

/* prompty.hostname() */
/* Returns hostname */
LFUNC(hostname){
    char hostname[1024];
    hostname[1023] = '\0';
    gethostname(hostname, 1023);
    lua_pushstring(L, hostname);
    return 1;
}

/* prompty.format() */
/* Formats given strings. */
/* Helpers function to output information */
LFUNC(format){

    lua_getfield(L, LUA_GLOBALSINDEX, "prompty");

    lua_getfield(L, -1, "sep");
    const char * sep = lua_tostring(L, -1);

    lua_getfield(L, LUA_GLOBALSINDEX, "prompty");

    lua_getfield(L, -1, "sep_color");
    const char * sep_color = lua_tostring(L, -1);
    lua_pop(L, 0);

    /* Get arguments from lua function */
    /* Icon */
    const char * col_icon = lua_tostring(L, 1);
    const char * icon = lua_tostring(L, 2);

    /* Info */
    const char * col_info = lua_tostring(L, 3);
    const char * info = lua_tostring(L, 4);

    printf("%7s%s%s%s%s%s%s%s%s\n",
            col_icon, icon, reset, sep_color, sep, reset, col_info, info, reset);
    return 1;
}

/* Register functions in lua */
void func_reg(void){
#define REG(N) lua_pushcfunction(L, lua_##N); lua_setfield(L, -2, #N);
    lua_newtable(L);
    REG(user)
    REG(hostname)
    REG(format)
    lua_setfield(L, LUA_GLOBALSINDEX, "prompty");

    luaL_newmetatable(L, "prompty");
    lua_newtable(L);
    lua_setfield(L, -2, "__index");
    lua_pop(L, 1);
#undef REG
}
