#include "prompty.h"
#include <lauxlib.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <git2.h>

#define LFUNC(N) int lua_##N(lua_State * L)

#define PATH_LENGTH 4096
#define TOKEN_LENGTH 64

/* prompty.user() */
/* Returns 'USER' environment variable */
LFUNC(user){

    char * username = getenv("USER");
    if(username) {
        lua_pushstring(L, username);
    } else lua_pushstring(L, "unknown");

    return 1;
}

LFUNC(git_branch){
    /* Initizalite git */
    git_libgit2_init();

    /* Try to open current directory as git */
    git_repository *repo;
    int error = git_repository_open(&repo, getenv("PWD"));

    /* Get current branch name */
    git_reference *head = NULL;
    const char *branch = NULL;

    /* Only continue if current directory is a git repo */
    if(!error) {
        int err = git_repository_head(&head, repo);
        if (err) {
            git_libgit2_shutdown();
        } else {
            branch = git_reference_shorthand(head);
        }
    }

    lua_pushstring(L, branch);
    /* Cleanup */
    git_repository_free(repo);
    git_libgit2_shutdown();
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

/* prompty.pwd() */
/* Returns current directory */
LFUNC(pwd){

    const char * pwd = getenv("PWD");

    if(pwd) {
        lua_pushstring(L, pwd);
    } else lua_pushstring(L, "unknown");

    return 1;
}

/* prompty.prompt() */
/* Creates the prompt with a symbol */
LFUNC(prompt){
    lua_getfield(L, LUA_GLOBALSINDEX, "prompty");
    lua_getfield(L, -1, "prompt_symbol");
    const char * symbol = lua_tostring(L, -1);
    lua_pop(L, 0);

    printf("\n%s%s",
            symbol, reset);
    return 1;
}

/* prompty.format() */
/* Formats the given information(s) to a line */
LFUNC(format){
    const char * info = lua_tostring(L, 1);
    printf("%s%s",
            info, reset);
    return 1;
}

/* prompty.format() */
/* Bubbles go brrr */
LFUNC(bubble){

    /* Get arguments from lua function */
    const char * info = lua_tostring(L, 1);

    printf("%s%s%s%s%s%s%s%s%s%s",
        bubble_fg,
        bubble_left,
        reset,
        bg,
        fg,
        info,
        reset,
        bubble_fg,
        bubble_right,
        reset);
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
    REG(format)
    REG(pwd)
    REG(git_branch)
    lua_setfield(L, LUA_GLOBALSINDEX, "prompty");

    luaL_newmetatable(L, "prompty");
    lua_newtable(L);
    lua_setfield(L, -2, "__index");
    lua_pop(L, 1);
#undef REG
}
