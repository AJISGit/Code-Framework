#include<stdio.h>

#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"

#include "funcs.h"

Color drawColor = WHITE;
Color backgroundColor = BLACK;
bool isShaderLoaded = false;
Shader* currentShader;

// internal functions

bool cdf_internal_isShaderLoaded() {

    return isShaderLoaded;

}

Shader* cdf_internal_getCurrentShader() {
    return currentShader;
}

// lua functions

void cdf_openLib(lua_State* L) {
    lua_pushcfunction(L, cdf_window_Init);
    lua_setglobal(L, "Init");
    lua_pushcfunction(L, cdf_window_SetFps);
    lua_setglobal(L, "SetMaxFps");
    lua_pushcfunction(L, cdf_window_Close);
    lua_setglobal(L, "Close");
    lua_pushcfunction(L, cdf_window_Wait);
    lua_setglobal(L, "Wait");

    lua_pushcfunction(L, cdf_draw_setColor);
    lua_setglobal(L, "SetDrawColor");
    lua_pushcfunction(L, cdf_draw_setBGColor);
    lua_setglobal(L, "SetBackgroundColor");
    lua_pushcfunction(L, cdf_draw_clearWindow);
    lua_setglobal(L, "ClearWindow");
    lua_pushcfunction(L, cdf_draw_circle);
    lua_setglobal(L, "DrawCircle");
    lua_pushcfunction(L, cdf_draw_rectangle);
    lua_setglobal(L, "DrawRectangle");
    lua_pushcfunction(L, cdf_draw_sprite);
    lua_setglobal(L, "DrawSprite");
    lua_pushcfunction(L, cdf_draw_text);
    lua_setglobal(L, "DrawText");

    lua_pushcfunction(L, cdf_input_isKeyDown);
    lua_setglobal(L, "IsKeyDown");

    lua_pushcfunction(L, cdf_load_sprite);
    lua_setglobal(L, "LoadSprite");
    lua_pushcfunction(L, cdf_load_sound);
    lua_setglobal(L, "LoadSound");

    lua_pushcfunction(L, cdf_sound_play);
    lua_setglobal(L, "PlaySound");

    lua_pushcfunction(L, cdf_shader_load);
    lua_setglobal(L, "LoadShader");
    lua_pushcfunction(L, cdf_shader_set);
    lua_setglobal(L, "SetShader");

}

int cdf_window_Init(lua_State* L) {
    SetTraceLogLevel(LOG_ERROR);
    //printf("working\n");
    
    int width = (int) lua_tointeger(L, 1);
    int height = (int) lua_tointeger(L, 2);
    const char* title = (char*) lua_tostring(L, 3);

    //printf("width: %d\nheight: %d\nname: %s\n", width, height, title);

    InitWindow(width, height, title);
    InitAudioDevice();

    return 0;

}

int cdf_window_SetFps(lua_State* L) {
    int fps = (int) lua_tointeger(L, -1);
    SetTargetFPS(fps);
    return 0;
}

int cdf_window_Close(lua_State* L) {
    CloseWindow();
    return 0;
}

int cdf_window_Wait(lua_State* L) {
    WaitTime(lua_tonumber(L, 1));
    return 0;
}



int cdf_draw_setColor(lua_State* L) {
    int red = (int) lua_tointeger(L, 1);
    int green = (int) lua_tointeger(L, 2);
    int blue = (int) lua_tointeger(L, 3);
    int alpha = (int) lua_tointeger(L, 4);

    drawColor = (Color){red, green, blue, alpha};

    return 0;
}

int cdf_draw_setBGColor(lua_State* L) {
    int red = (int) lua_tointeger(L, 1);
    int green = (int) lua_tointeger(L, 2);
    int blue = (int) lua_tointeger(L, 3);
    backgroundColor = (Color){red, green, blue, 255};
}

int cdf_draw_clearWindow(lua_State* L) {
    ClearBackground(backgroundColor);
}

int cdf_draw_circle(lua_State* L) {
    float posX = (float) lua_tonumber(L, 1);
    float posY = (float) lua_tonumber(L, 2);
    float radius = (float) lua_tonumber(L, 3);

    DrawCircleV((Vector2){posX, posY}, radius, drawColor);

    return 0;

}

int cdf_draw_rectangle(lua_State* L) {
    float posX = (float) lua_tonumber(L, 1);
    float posY = (float) lua_tonumber(L, 2);
    float width = (float) lua_tonumber(L, 3);
    float height = (float) lua_tonumber(L, 4);
    DrawRectangleV((Vector2){posX, posY}, (Vector2){width, height}, drawColor);
    return 0;
}

int cdf_draw_sprite(lua_State* L) {
    
    Texture2D* sprite = (Texture2D*) lua_touserdata(L, 1);
    float posX = (float) lua_tonumber(L, 2);
    float posY = (float) lua_tonumber(L, 3);

    DrawTextureV(*sprite, (Vector2){posX, posY}, drawColor);

}

int cdf_draw_text(lua_State* L) {
    const char* text = lua_tostring(L, 1);
    int posX = (int) lua_tointeger(L, 2);
    int posY = (int) lua_tointeger(L, 3);
    int size = (int) lua_tointeger(L, 4);

    DrawText(text, posX, posY, size, drawColor);

    return 0;

}


int cdf_input_isKeyDown(lua_State* L) {
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

int cdf_load_sprite(lua_State* L) {

    const char* filename = lua_tostring(L, 1);
    Texture2D newTexture = LoadTexture(filename);
    Texture2D* pTexture = (Texture2D*) lua_newuserdata(L, sizeof(newTexture));
    pTexture->format = newTexture.format;
    pTexture->height = newTexture.height;
    pTexture->id = newTexture.id;
    pTexture->mipmaps = newTexture.mipmaps;
    pTexture->width = newTexture.width;

    return 1;

}

int cdf_load_sound(lua_State* L) {

    const char* filename = lua_tostring(L, 1);

    Sound newSound = LoadSound(filename);
    Sound* pSound = (Sound*) lua_newuserdata(L, sizeof(newSound));

    pSound->frameCount = newSound.frameCount;
    pSound->stream = newSound.stream;

    return 1;

}


int cdf_sound_play(lua_State* L) {

    Sound* sound = (Sound*) lua_touserdata(L, 1);
    PlaySound(*sound);
    return 0;

}


int cdf_shader_load(lua_State* L) {

    const char* ffilename = lua_tostring(L, 1);
    const char* vfilename = lua_tostring(L, 2);

    Shader newShader = LoadShader(vfilename, ffilename);
    Shader* pShader = (Shader*) lua_newuserdata(L, sizeof(newShader));

    pShader->id = newShader.id;
    pShader->locs = newShader.locs;

    return 1;

}

int cdf_shader_set(lua_State* L) {

    Shader* shader = (Shader*) lua_touserdata(L, 1);
    currentShader = shader;
    isShaderLoaded = true;
    return 0;

}
