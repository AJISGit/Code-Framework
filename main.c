/*********************************************

    main.c:
    Handles stuff like the callback functions (onDraw or onUpdate).

**********************************************/

#include<stdio.h>
#include<string.h>
#include<time.h>

#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"

#include "funcs.h"

#define GET_CURRENT_TIME (int) clock()

// -O1 -std=c++17 -Wno-missing-braces -I ./include/ -L ./lib/ -lraylib -lopengl32 -lgdi32 -lwinmm
// lzio.c lvm.c lutf8lib.c lundump.c lua.c ltm.c ltablib.c ltable.c lstrlib.c lstring.c lstate.c lparser.c loslib.c lopcodes.c lobject.c loadlib.c lmem.c lmathlib.c llex.c liolib.c linit.c lgc.c lfunc.c ldump.c ldo.c ldebug.c ldblib.c lctype.c lcorolib.c lcode.c lbaselib.c lauxlib.c lapi.c

int main(int argc, char* argv[]) {

    if (argc < 2) {
        printf("\nCodeFramework: version beta 1.2, \"The Unloading Update!\"\nNo copyright at all!\nsome sort of license would go here.\nThis is a crappy lua framework made using raylib and C. Generic name on purpose\n");
        printf("\n---------------------------------\n");
        printf("You must have forgotten to enter a file, please do that\n");
        printf("\nOr you could type \"codeframework --info\" for more info\n\n");
        return 0;
    }

    if (strcmp(argv[1], "--info") == 0) {
        const char* infoText = "\n\
        Codeframework:\n\
        The new Lua framework in town.\n\
        You may know of it as, \"That framework that only supports four keys\",\n\
        Or, \"That framework that doesn't test the metatables of userdata.\"\n\
        \n\
        We, and by we I mean I, don't care about these major flaws. I think of them as features. \n\
        \n\
        You want documentation? I'm working on it! You can see the progress at https://github.com/AJISGit/Code-Framework/wiki.\n\
        \n\
        ";
        printf("%s\n", infoText);
        return 0;
    }

    lua_State* L = luaL_newstate();
    luaL_openlibs(L);
    cdf_openLib(L);
    
    int error = luaL_dofile(L, argv[1]);
    if (error) {
        fprintf(stderr, "%s\n", lua_tostring(L, -1));
        lua_close(L);
        return 1;
    }

    int isUpdate = lua_getglobal(L, "onUpdate");

    if (isUpdate) {
        
        int startTime = GET_CURRENT_TIME;
        while (!WindowShouldClose()) {

            lua_getglobal(L, "onUpdate");
            
            lua_pushnumber(L, (lua_Number) (clock() - startTime));
            startTime = GET_CURRENT_TIME;

            int updateError = lua_pcall(L, 1, 0, 0);

            if (updateError) {
                fprintf(stderr, "%s\n", lua_tostring(L, -1));
                lua_pop(L, -1);
                return 1;
            }

            lua_getglobal(L, "onDraw");

            BeginDrawing();

            if (cdf_internal_isShaderLoaded()) {
                BeginShaderMode(*(cdf_internal_getCurrentShader()));
            }

            int drawError = lua_pcall(L, 0, 0, 0);
            if (drawError) {
                fprintf(stderr, "%s\n", lua_tostring(L, -1));
                lua_pop(L, -1);
                return 1;
            }

            if (cdf_internal_isShaderLoaded()) {
                EndShaderMode();
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

    return 0;

}