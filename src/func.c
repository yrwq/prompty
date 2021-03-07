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



LFUNC(pwd){

    const char * pwd = getenv("PWD");

    if(pwd) {
        lua_pushstring(L, pwd);
    } else lua_pushstring(L, "unknown");

    return 1;
}

LFUNC(prompt){

    lua_getfield(L, LUA_GLOBALSINDEX, "prompty");
    lua_getfield(L, -1, "prompt_symbol");
    const char * symbol = lua_tostring(L, -1);
    lua_pop(L, 0);

    /* Get arguments from lua function */
    const char * fg = lua_tostring(L, 1);
    const char * bg = lua_tostring(L, 2);

    printf("\n%s%s",
            symbol, reset);
    return 1;
}

/* prompty.format() */
/* Formats given strings. */
/* Helpers function to output information */
LFUNC(bubble){

    lua_getfield(L, LUA_GLOBALSINDEX, "prompty");
    lua_getfield(L, -1, "left_symbol");
    const char * left_symbol = lua_tostring(L, -1);

    lua_getfield(L, LUA_GLOBALSINDEX, "prompty");
    lua_getfield(L, -1, "right_symbol");
    const char * right_symbol = lua_tostring(L, -1);
    lua_pop(L, 0);

    /* Get arguments from lua function */
    const char * prompt_fg = lua_tostring(L, 1);
    const char * fg = lua_tostring(L, 2);
    const char * bg = lua_tostring(L, 3);
    const char * info = lua_tostring(L, 4);

    printf("%s%s%s%s%s%s%s%s%s%s",
            prompt_fg, left_symbol, reset, bg, fg, info, reset, prompt_fg, right_symbol, reset);
    return 1;
}

/* Register functions in lua */
void func_reg(void){
#define REG(N) lua_pushcfunction(L, lua_##N); lua_setfield(L, -2, #N);
    lua_newtable(L);
    REG(user)
    REG(hostname)
    REG(bubble)
    REG(prompt)
    REG(pwd)
    lua_setfield(L, LUA_GLOBALSINDEX, "prompty");

    luaL_newmetatable(L, "prompty");
    lua_newtable(L);
    lua_setfield(L, -2, "__index");
    lua_pop(L, 1);
#undef REG
}
