#include "Library.h"
#include "RectCore.h"
#include "lualib.h"

#include <cmath>

namespace Luwow::Rects {
    ILuauModule* Library::initialize(ILuauHost* host) {
        registerRect(host->getMainState());
        return new Library();
    }

    Luwow::Engine::RunMode Library::getRunMode() const {
        return LUWOW_MODULE_RUN_MODE;
    }

    static int round(float value) {
        return static_cast<int>(std::lround(value));
    }

    // Rect.new(position, size) with vectors, or Rect.new(x, y, width, height). Values are rounded to whole pixels.
    static int rect_new(lua_State* L) {
        Rect rect;
        if (lua_isvector(L, 1)) {
            const float* position = luaL_checkvector(L, 1);
            const float* size = luaL_checkvector(L, 2);
            rect = Rect{ round(position[0]), round(position[1]), round(size[0]), round(size[1]) };
        } else {
            rect = Rect{
                round(static_cast<float>(luaL_optnumber(L, 1, 0))),
                round(static_cast<float>(luaL_optnumber(L, 2, 0))),
                round(static_cast<float>(luaL_optnumber(L, 3, 0))),
                round(static_cast<float>(luaL_optnumber(L, 4, 0))),
            };
        }

        if (rect.width < 0 || rect.height < 0) luaL_error(L, "a Rect's size can't be negative");
        pushRect(L, rect);
        return 1;
    }

    const char* Library::getModuleName() const {
        return "rect";
    }

    const char* Library::getModuleAlias() const {
        return "Luwow";
    }

    static LuauExport exports[] = {
        { "new", rect_new },
        { nullptr, nullptr }
    };

    const LuauExport* Library::getExports() const {
        return exports;
    }
} // namespace Luwow::Rects

LUWOW_REGISTER_MODULE(Luwow::Rects::Library)
