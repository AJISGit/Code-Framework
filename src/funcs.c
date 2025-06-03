/*********************************************

    funcs.c:
    Contains definition for the API functions.

**********************************************/

#include<stdio.h>
#include<string.h>

#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

#include "funcs.h"

Color drawColor = WHITE;
Color backgroundColor = BLACK;
bool isShaderLoaded = false;
bool isFontLoaded = false;
Shader* currentShader;
Font* currentFont;

// internal functions

bool cdf_internal_isShaderLoaded() {

    return isShaderLoaded;

}

Shader* cdf_internal_getCurrentShader() {

    return currentShader;

}

bool cdf_internal_isFontLoaded() {

    return isFontLoaded;

}

// lua functions

void cdf_openLib(lua_State* L) {

    lua_pushcfunction(L, cdf_window_Init);
    lua_setglobal(L, "Init");
    lua_pushcfunction(L, cdf_window_SetFps);
    lua_setglobal(L, "SetMaxFps");
    lua_pushcfunction(L, cdf_window_Close);
    lua_setglobal(L, "Close");

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
    lua_pushcfunction(L, cdf_input_getMousePosition);
    lua_setglobal(L, "GetMousePosition");
    lua_pushcfunction(L, cdf_input_isMouseButtonDown);
    lua_setglobal(L, "IsMouseButtonDown");
    lua_pushcfunction(L, cdf_input_isMouseButtonPressed);
    lua_setglobal(L, "IsMouseButtonPressed");

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
    lua_pushcfunction(L, cdf_shader_reset);
    lua_setglobal(L, "ResetShader");

    lua_pushcfunction(L, cdf_font_load);
    lua_setglobal(L, "LoadFont");
    lua_pushcfunction(L, cdf_font_set);
    lua_setglobal(L, "SetFont");
    lua_pushcfunction(L, cdf_font_reset);
    lua_setglobal(L, "ResetFont");

    lua_pushcfunction(L, cdf_unload_sprite);
    lua_setglobal(L, "UnloadSprite");
    lua_pushcfunction(L, cdf_unload_sound);
    lua_setglobal(L, "UnloadSound");
    lua_pushcfunction(L, cdf_unload_shader);
    lua_setglobal(L, "UnloadShader");
    lua_pushcfunction(L, cdf_unload_font);
    lua_setglobal(L, "UnloadFont");

    lua_pushcfunction(L, cdf_misc_Wait);
    lua_setglobal(L, "Wait");

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
    float posX = (float) lua_tonumber(L, 2);
    float posY = (float) lua_tonumber(L, 3);
    float size = (float) lua_tonumber(L, 4);

    int defaultFontSize = 10;
    int spacing = size / defaultFontSize;

    Vector2 textPos = (Vector2) {posX, posY};

    if (cdf_internal_isFontLoaded()) {

        DrawTextEx(*currentFont, text, textPos, size, (float) spacing, drawColor);

    } else {

        DrawTextEx(GetFontDefault(), text, textPos, size, (float) spacing, drawColor);

    }

    return 0;

}


int cdf_input_isKeyDown(lua_State* L) {
    const char *key = lua_tostring(L, 1);
    
    if (strcmp(key, "w") == 0) {
        if (IsKeyDown(KEY_W)) {
            lua_pushboolean(L, 1);
        } else {
            lua_pushboolean(L, 0);
        }
    } else if (strcmp(key, "a") == 0) {
        if (IsKeyDown(KEY_A)) {
            lua_pushboolean(L, 1);
        } else {
            lua_pushboolean(L, 0);
        }
    } else if (strcmp(key, "s") == 0) {
        if (IsKeyDown(KEY_S)) {
            lua_pushboolean(L, 1);
        } else {
            lua_pushboolean(L, 0);
        }
    } else if (strcmp(key, "d") == 0) {
        if (IsKeyDown(KEY_D)) {
            lua_pushboolean(L, 1);
        } else {
            lua_pushboolean(L, 0);
        }
    } else if (strcmp(key, "space") == 0) {

        if (IsKeyDown(KEY_SPACE)) {
            lua_pushboolean(L, 1);
        } else {
            lua_pushboolean(L, 0);
        }

    } else if (strcmp(key, "enter") == 0) {

        if (IsKeyDown(KEY_ENTER)) {
            lua_pushboolean(L, 1);
        } else {
            lua_pushboolean(L, 0);
        }

    } else {
        fprintf(stderr, "Invalid Key\n");
        lua_pushboolean(L, 0);
    }

    return 1;
}

int cdf_input_getMousePosition(lua_State* L) {

    Vector2 mousePos = GetMousePosition();

    lua_pushnumber(L, mousePos.x);
    lua_pushnumber(L, mousePos.y);

    return 2;

}

int cdf_input_isMouseButtonDown(lua_State* L) {

    const char* button = lua_tostring(L, 1);
    int buttonNumber;

    if (strcmp(button, "left") == 0) {

        buttonNumber = MOUSE_BUTTON_LEFT;

    } else if (strcmp(button, "right") == 0) {

        buttonNumber = MOUSE_BUTTON_RIGHT;

    } else {

        luaL_error(L, "Mouse button \"%s\" is not a valid mouse button.", button);
        return 0;

    }

    lua_pushboolean(L, (int) IsMouseButtonDown(buttonNumber));

    return 1;

}

int cdf_input_isMouseButtonPressed(lua_State* L) {

    const char* button = lua_tostring(L, 1);
    int buttonNumber;

    if (strcmp(button, "left") == 0) {

        buttonNumber = MOUSE_BUTTON_LEFT;

    } else if (strcmp(button, "right") == 0) {

        buttonNumber = MOUSE_BUTTON_RIGHT;

    } else {

        luaL_error(L, "Mouse button \"%s\" is not a valid mouse button.", button);
        return 0;

    }

    lua_pushboolean(L, (int) IsMouseButtonPressed(buttonNumber));

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

int cdf_shader_reset(lua_State* L) {

    isShaderLoaded = false;
    return 0;

}


int cdf_font_load(lua_State* L) {

    cdf_font_reset(L);

    const char* filename = lua_tostring(L, 1);
    Font newFont = LoadFont(filename);

    Font* pFont = (Font*) lua_newuserdata(L, sizeof(newFont));

    pFont->baseSize = newFont.baseSize;
    pFont->glyphCount = newFont.glyphCount;
    pFont->glyphPadding = newFont.glyphPadding;
    pFont->glyphs = newFont.glyphs;
    pFont->recs = newFont.recs;
    pFont->texture = newFont.texture;

    return 1;

}

int cdf_font_set(lua_State* L) {

    Font* font = (Font*) lua_touserdata(L, 1);
    currentFont = font;

    isFontLoaded = true;

    return 0;

}

int cdf_font_reset(lua_State* L) {

    isFontLoaded = false;
    return 0;

}


int cdf_unload_sprite(lua_State* L) {

    Texture2D* sprite = (Texture2D*) lua_touserdata(L, 1);
    UnloadTexture(*sprite);
    return 0;

}

int cdf_unload_sound(lua_State* L) {

    Sound* sound = (Sound*) lua_touserdata(L, 1);
    UnloadSound(*sound);
    return 0;

}

int cdf_unload_shader(lua_State* L) {

    Shader* shader = (Shader*) lua_touserdata(L, 1);
    UnloadShader(*shader);
    return 0;

}

int cdf_unload_font(lua_State* L) {

    isFontLoaded = false;
    return 0;

}


int cdf_misc_Wait(lua_State* L) {

    WaitTime(lua_tonumber(L, 1));
    return 0;

}
