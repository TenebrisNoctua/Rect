#include "RectCore.h"
#include "lua.h"
#include "lualib.h"

#include <cmath>
#include <cstring>
#include <new>

namespace Luwow::Rects {
    static constexpr const char* LockedMessage = "The metatable is locked";

    bool isRect(lua_State* L, int index) {
        return lua_touserdatatagged(L, index, RectTag) != nullptr;
    }

    Rect checkRect(lua_State* L, int index) {
        auto* rect = static_cast<Rect*>(lua_touserdatatagged(L, index, RectTag));
        if (!rect) luaL_typeerror(L, index, "Rect");
        return *rect;
    }

    void pushRect(lua_State* L, const Rect& rect) {
        void* data = lua_newuserdatataggedwithmetatable(L, sizeof(Rect), RectTag);
        new (data) Rect(rect);
    }

    static void pushVector2(lua_State* L, int x, int y) {
        lua_pushvector(L, static_cast<float>(x), static_cast<float>(y), 0.0f);
    }

    // rect:Contains(point: vector) -> boolean
    static int rect_contains(lua_State* L) {
        Rect rect = checkRect(L, 1);
        const float* point = luaL_checkvector(L, 2);
        lua_pushboolean(L, rect.contains(static_cast<int>(std::floor(point[0])), static_cast<int>(std::floor(point[1]))));
        return 1;
    }

    // rect:Intersect(other: Rect) -> Rect
    static int rect_intersect(lua_State* L) {
        pushRect(L, checkRect(L, 1).intersect(checkRect(L, 2)));
        return 1;
    }

    static lua_CFunction findMethod(const char* name) {
        if (std::strcmp(name, "Contains") == 0) return rect_contains;
        if (std::strcmp(name, "Intersect") == 0) return rect_intersect;
        return nullptr;
    }

    static int rect_index(lua_State* L) {
        Rect rect = checkRect(L, 1);
        const char* key = luaL_checkstring(L, 2);

        if (std::strcmp(key, "X") == 0) lua_pushinteger(L, rect.x);
        else if (std::strcmp(key, "Y") == 0) lua_pushinteger(L, rect.y);
        else if (std::strcmp(key, "Width") == 0) lua_pushinteger(L, rect.width);
        else if (std::strcmp(key, "Height") == 0) lua_pushinteger(L, rect.height);
        else if (std::strcmp(key, "Position") == 0 || std::strcmp(key, "Min") == 0) pushVector2(L, rect.x, rect.y);
        else if (std::strcmp(key, "Size") == 0) pushVector2(L, rect.width, rect.height);
        else if (std::strcmp(key, "Max") == 0) pushVector2(L, rect.right(), rect.bottom());
        else if (lua_CFunction method = findMethod(key)) lua_pushcfunction(L, method, key);
        else luaL_error(L, "%s is not a valid member of Rect", key);
        return 1;
    }

    static int rect_namecall(lua_State* L) {
        const char* name = lua_namecallatom(L, nullptr);
        lua_CFunction method = name ? findMethod(name) : nullptr;
        if (!method) luaL_error(L, "%s is not a valid member of Rect", name ? name : "?");
        return method(L);
    }

    static int rect_newindex(lua_State* L) {
        luaL_error(L, "%s cannot be assigned to, Rect is immutable", luaL_checkstring(L, 2));
        return 0;
    }

    static int rect_eq(lua_State* L) {
        lua_pushboolean(L, checkRect(L, 1) == checkRect(L, 2));
        return 1;
    }

    static int rect_tostring(lua_State* L) {
        Rect rect = checkRect(L, 1);
        lua_pushfstring(L, "%d, %d, %d, %d", rect.x, rect.y, rect.width, rect.height);
        return 1;
    }

    void registerRect(lua_State* L) {
        lua_getuserdatametatable(L, RectTag);
        bool registered = !lua_isnil(L, -1);
        lua_pop(L, 1);
        if (registered) return;

        // luaL_newmetatable keeps the table in the registry
        luaL_newmetatable(L, "Luwow.Rect");
        lua_pushcfunction(L, rect_index, "Rect.__index");
        lua_setfield(L, -2, "__index");
        lua_pushcfunction(L, rect_namecall, "Rect.__namecall");
        lua_setfield(L, -2, "__namecall");
        lua_pushcfunction(L, rect_newindex, "Rect.__newindex");
        lua_setfield(L, -2, "__newindex");
        lua_pushcfunction(L, rect_eq, "Rect.__eq");
        lua_setfield(L, -2, "__eq");
        lua_pushcfunction(L, rect_tostring, "Rect.__tostring");
        lua_setfield(L, -2, "__tostring");
        lua_pushstring(L, "Rect");
        lua_setfield(L, -2, "__type");
        lua_pushstring(L, LockedMessage);
        lua_setfield(L, -2, "__metatable");
        lua_setreadonly(L, -1, true);
        lua_setuserdatametatable(L, RectTag);
    }
}
