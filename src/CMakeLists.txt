add_library(Luwow.RectCore STATIC
    RectCore.h
    RectCore.cpp
)
target_include_directories(Luwow.RectCore
    PUBLIC ${RECT_ROOT}/src
    PRIVATE $<TARGET_PROPERTY:Luau.VM,INTERFACE_INCLUDE_DIRECTORIES>
)
target_compile_definitions(Luwow.RectCore PRIVATE $<TARGET_PROPERTY:Luau.VM,INTERFACE_COMPILE_DEFINITIONS>)

luwow_add_library(Luwow.RectLibrary
    NAME rect
    MODES SERIAL
    SOURCES
        Library.h
        Library.cpp
)
target_link_libraries(Luwow.RectLibrary PRIVATE Luwow.RectCore)
