#pragma once

#include <algorithm>

struct lua_State;

// Rect values, usable from Luau and C++ alike. A Rect is a rectangle in whole pixels.
namespace Luwow::Rects {
    // Userdata tag reserved for Rect, below Luau's limit of 128
    inline constexpr int RectTag = 123;

    struct Rect {
        int x = 0;
        int y = 0;
        int width = 0;
        int height = 0;

        int right() const { return x + width; }
        int bottom() const { return y + height; }

        bool contains(int px, int py) const {
            return px >= x && py >= y && px < right() && py < bottom();
        }

        // The area both cover, empty when they don't overlap
        Rect intersect(const Rect& other) const {
            int left = std::max(x, other.x);
            int top = std::max(y, other.y);
            int r = std::min(right(), other.right());
            int b = std::min(bottom(), other.bottom());
            return Rect{ left, top, std::max(0, r - left), std::max(0, b - top) };
        }

        bool operator==(const Rect& other) const = default;
    };

    // Registers the Rect metatable once per Luau state, calling it again does nothing
    void registerRect(lua_State* L);

    // Pushes a new Rect. registerRect must have been called on the state.
    void pushRect(lua_State* L, const Rect& rect);

    // Whether the value at index is a Rect
    bool isRect(lua_State* L, int index);

    // The Rect at index, raising a type error for anything else
    Rect checkRect(lua_State* L, int index);
}
