/*********************************************

    funcs.h:
    Contains declarations for the API functions.

    All function names follow this format: "cdf_catagory_name"

**********************************************/

#pragma once

#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"

#include<raylib.h>

// internal functions

bool cdf_internal_isShaderLoaded();
Shader* cdf_internal_getCurrentShader();

// lua functions

void cdf_openLib(lua_State* L);

int cdf_window_Init(lua_State* L);
int cdf_window_Close(lua_State* L);
int cdf_window_SetFps(lua_State* L);
int cdf_window_Wait(lua_State* L);

int cdf_draw_setColor(lua_State* L);
int cdf_draw_setBGColor(lua_State* L);
int cdf_draw_clearWindow(lua_State* L);
int cdf_draw_circle(lua_State* L);
int cdf_draw_rectangle(lua_State* L);
int cdf_draw_sprite(lua_State* L);
int cdf_draw_text(lua_State* L);

int cdf_input_isKeyDown(lua_State* L);

int cdf_load_sprite(lua_State* L);
int cdf_load_sound(lua_State* L);

int cdf_sound_play(lua_State* L);

int cdf_shader_load(lua_State* L);
int cdf_shader_set(lua_State* L);
int cdf_shader_reset(lua_State* L);

int cdf_unload_sprite(lua_State* L);
int cdf_unload_sound(lua_State* L);
int cdf_unload_shader(lua_State* L);
