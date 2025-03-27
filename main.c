#include<stdio.h>

#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"

#include "funcs.h"

// -O1 -std=c++17 -Wno-missing-braces -I ./include/ -L ./lib/ -lraylib -lopengl32 -lgdi32 -lwinmm
// lzio.c lvm.c lutf8lib.c lundump.c lua.c ltm.c ltablib.c ltable.c lstrlib.c lstring.c lstate.c lparser.c loslib.c lopcodes.c lobject.c loadlib.c lmem.c lmathlib.c llex.c liolib.c linit.c lgc.c lfunc.c ldump.c ldo.c ldebug.c ldblib.c lctype.c lcorolib.c lcode.c lbaselib.c lauxlib.c lapi.c

int main(int argc, char* argv[]) {

    if (argc < 2) {
        printf("\nRaylua: version is like alpha 0.001\nNo copyright at all!\nsome sort of license would go here.\nThis is a crappy lua framework made using raylib and C.\n");
        printf("\n---------------------------------\n");
        fprintf(stderr, "You must have forgotten to enter a file, please do that\n\n");
        return 0;
    }

    lua_State* L = luaL_newstate();
    luaL_openlibs(L);
    openLib(L);
    
    int error = luaL_dofile(L, argv[1]);
    if (error) {
        fprintf(stderr, "%s\n", lua_tostring(L, -1));
        lua_close(L);
        return 1;
    }

    int isUpdate = lua_getglobal(L, "onUpdate");

    if (isUpdate) {
        SetTargetFPS(30);
        while (!WindowShouldClose()) {

            lua_getglobal(L, "onUpdate");

            int updateError = lua_pcall(L, 0, 0, 0);

            if (updateError) {
                fprintf(stderr, "%s\n", lua_tostring(L, -1));
                lua_pop(L, -1);
                return 1;
            }

            lua_getglobal(L, "onDraw");

            BeginDrawing();
            int drawError = lua_pcall(L, 0, 0, 0);
            if (drawError) {
                fprintf(stderr, "%s\n", lua_tostring(L, -1));
                lua_pop(L, -1);
                return 1;
            }
            EndDrawing();

            lua_settop(L, 0);
        }
    }


    int isClose = lua_getglobal(L, "onClose");
    
    if (isClose) {
        int closeError = lua_pcall(L, 0, 0, 0);
        if (closeError) {
            fprintf(stderr, "%s\n", lua_tostring(L, -1));
            return 1;
        }
    }

    lua_close(L);
}