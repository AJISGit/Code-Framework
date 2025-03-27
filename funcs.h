#pragma once

#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"

#include<raylib.h>

void openLib(lua_State* L);

int window_Init(lua_State* L);
int window_Close(lua_State* L);
int window_SetFps(lua_State* L);
int window_Wait(lua_State* L);

int draw_setColor(lua_State* L);
int draw_setBGColor(lua_State* L);
int draw_clearWindow(lua_State* L);
int draw_circle(lua_State* L);

int input_isKeyDown(lua_State* L);