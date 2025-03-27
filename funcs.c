#include<stdio.h>

#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"

#include "funcs.h"

Color drawColor = WHITE;
Color backgroundColor = BLACK;

void openLib(lua_State* L) {
    lua_pushcfunction(L, window_Init);
    lua_setglobal(L, "Init");
    lua_pushcfunction(L, window_SetFps);
    lua_setglobal(L, "SetMaxFps");
    lua_pushcfunction(L, window_Close);
    lua_setglobal(L, "Close");
    lua_pushcfunction(L, window_Wait);
    lua_setglobal(L, "Wait");

    lua_pushcfunction(L, draw_setColor);
    lua_setglobal(L, "SetDrawColor");
    lua_pushcfunction(L, draw_setBGColor);
    lua_setglobal(L, "SetBackgroundColor");
    lua_pushcfunction(L, draw_clearWindow);
    lua_setglobal(L, "ClearWindow");
    lua_pushcfunction(L, draw_circle);
    lua_setglobal(L, "DrawCircle");

    lua_pushcfunction(L, input_isKeyDown);
    lua_setglobal(L, "IsKeyDown");
}

int window_Init(lua_State* L) {
    SetTraceLogLevel(LOG_ERROR);
    //printf("working\n");
    
    int width = (int) lua_tointeger(L, 1);
    int height = (int) lua_tointeger(L, 2);
    const char* title = (char*) lua_tostring(L, 3);

    //printf("width: %d\nheight: %d\nname: %s\n", width, height, title);

    InitWindow(width, height, title);

    return 0;

}

int window_SetFps(lua_State* L) {
    int fps = (int) lua_tointeger(L, -1);
    SetTargetFPS(fps);
    return 0;
}

int window_Close(lua_State* L) {
    CloseWindow();
    return 0;
}

int window_Wait(lua_State* L) {
    WaitTime(lua_tonumber(L, 1));
    return 0;
}



int draw_setColor(lua_State* L) {
    int red = (int) lua_tointeger(L, 1);
    int green = (int) lua_tointeger(L, 2);
    int blue = (int) lua_tointeger(L, 3);
    int alpha = (int) lua_tointeger(L, 4);

    drawColor = (Color){red, green, blue, alpha};

    return 0;
}

int draw_setBGColor(lua_State* L) {
    int red = (int) lua_tointeger(L, 1);
    int green = (int) lua_tointeger(L, 2);
    int blue = (int) lua_tointeger(L, 3);
    backgroundColor = (Color){red, green, blue, 255};
}

int draw_clearWindow(lua_State* L) {
    ClearBackground(backgroundColor);
}

int draw_circle(lua_State* L) {
    int posX = (int) lua_tointeger(L, 1);
    int posY = (int) lua_tointeger(L, 2);
    float radius = (float) lua_tonumber(L, 3);

    DrawCircle(posX, posY, radius, drawColor);

    return 0;

}



int input_isKeyDown(lua_State* L) {
    const char *key = luaL_checkstring(L, 1);
    
    if (key[0] == 'w') {
        if (IsKeyDown(KEY_W)) {
            lua_pushboolean(L, 1);
        } else {
            lua_pushboolean(L, 0);
        }
    } else if (key[0] == 'a') {
        if (IsKeyDown(KEY_A)) {
            lua_pushboolean(L, 1);
        } else {
            lua_pushboolean(L, 0);
        }
    } else if (key[0] == 's') {
        if (IsKeyDown(KEY_S)) {
            lua_pushboolean(L, 1);
        } else {
            lua_pushboolean(L, 0);
        }
    } else if (key[0] == 'd') {
        if (IsKeyDown(KEY_D)) {
            lua_pushboolean(L, 1);
        } else {
            lua_pushboolean(L, 0);
        }
    }  else {
        fprintf(stderr, "Invalid Key\n");
        lua_pushboolean(L, 0);
    }

    return 1;
}