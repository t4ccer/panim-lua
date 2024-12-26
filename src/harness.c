#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
#include <raylib.h>
#include <raymath.h>
#include "env.h"
#include "plug.h"

#define PLUG(name, ret, ...) ret name(__VA_ARGS__);
LIST_OF_PLUGS
#undef PLUG

typedef struct {
    size_t size;
    lua_State *L;
} Plug;

static Plug *p;

int raylib_draw_rectangle(lua_State* L)
{
  lua_Number x = lua_tonumber(L, -5);
  lua_Number y = lua_tonumber(L, -4);
  lua_Number w = lua_tonumber(L, -3);
  lua_Number h = lua_tonumber(L, -2);
  Color *color = (Color*)lua_touserdata(L, -1);
  DrawRectangle(x, y, w, h, *color);
  return 0;
}

static Color raylib_red = RED;
static Color raylib_blue = BLUE;

static void load_assets(void)
{
    lua_pushcfunction(p->L, raylib_draw_rectangle);
    lua_setglobal(p->L, "raylib_draw_rectangle");

    lua_pushlightuserdata(p->L, &raylib_red);
    lua_setglobal(p->L, "raylib_red");

    lua_pushlightuserdata(p->L, &raylib_blue);
    lua_setglobal(p->L, "raylib_blue");

    (void)luaL_dofile(p->L, "../src/plug.lua");
}

void plug_reset(void)
{
}

void plug_init(void)
{
    p = malloc(sizeof(*p));
    assert(p != NULL);
    memset(p, 0, sizeof(*p));
    p->size = sizeof(*p);
    p->L = luaL_newstate();
    luaL_openlibs(p->L);

    load_assets();
    plug_reset();
}

void *plug_pre_reload(void)
{
    return p;
}

void plug_post_reload(void *state)
{
    p = state;
    if (p->size < sizeof(*p)) {
        TraceLog(LOG_INFO, "Migrating plug state schema %zu bytes -> %zu bytes", p->size, sizeof(*p));
        p = realloc(p, sizeof(*p));
        p->size = sizeof(*p);
    }

    load_assets();
}

void plug_update(Env env)
{
    Color background_color = ColorFromHSV(0, 0, 0.05);
    ClearBackground(background_color);

    Camera2D camera = {
        .zoom = 1.0,
        .offset = {env.screen_width/2, env.screen_height/2},
    };
    BeginMode2D(camera);
    (void)luaL_dostring(p->L, "lua_plug_update()");
    EndMode2D();
}

bool plug_finished(void)
{
    return true;
}
