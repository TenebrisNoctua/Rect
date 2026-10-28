# Rect
A Rect value library for Luwow-Project.

This repository is not stand-alone. You must build it externally from within another
repository or library.

## Project Overview

This project contains the sources for Luwow Rect library.

A Rect is an immutable rectangle in whole pixels. Positions and sizes are Luau vectors, only their x and y are used.

The implementation lives in `RectCore`, a small static library other libraries link to create and read rectangles from C++.

## Library Documentation

```luau
local Rect = require("@Luwow/rect")
```

### `Rect.new(position: vector, size: vector) -> Rect`
### `Rect.new(x: number?, y: number?, width: number?, height: number?) -> Rect`

Creates a rectangle. Values are rounded to whole pixels, and the size can't be negative.

### `rect.X`, `rect.Y`, `rect.Width`, `rect.Height: number`

The fields, read only.

### `rect.Position`, `rect.Size`, `rect.Min`, `rect.Max: vector`

`Min` is the top left corner, the same as `Position`, and `Max` the bottom right.

### `rect:Contains(point: vector) -> boolean`

Whether the point is inside the rectangle.

### `rect:Intersect(other: Rect) -> Rect`

The area both cover, empty when they don't overlap.

## C++

```cpp
#include "RectCore.h"

using Luwow::Rects::Rect;

Luwow::Rects::registerRect(L);                  // once, in the module's initialize
Luwow::Rects::pushRect(L, Rect{ 0, 0, 100, 50 });

Rect rect = Luwow::Rects::checkRect(L, 1);      // raises for anything else
bool inside = rect.contains(10, 10);
```

Rect uses the userdata tag 123.
