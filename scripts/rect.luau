-- This is the type system for the Rect API as it exists in C++ code.

--[=[
	A rectangle in whole pixels, immutable. Min is its top left corner and Max its bottom right.
	Positions and sizes are vectors, only their x and y are used.
]=]
export type Rect = {
	read X: number,
	read Y: number,
	read Width: number,
	read Height: number,
	read Position: vector,
	read Size: vector,
	read Min: vector,
	read Max: vector,

	--[=[
		Whether the point is inside the rectangle.
	]=]
	Contains: (self: Rect, point: vector) -> boolean,

	--[=[
		The area both rectangles cover, empty when they don't overlap.
	]=]
	Intersect: (self: Rect, other: Rect) -> Rect,
}

export type Library = {
	--[=[
		Creates a rectangle from a position and a size vector, or from x, y, width and height.
		Values are rounded to whole pixels, and the size can't be negative.
	]=]
	new: ((position: vector, size: vector) -> Rect) & ((x: number?, y: number?, width: number?, height: number?) -> Rect),
}

return table.freeze({} :: Library)
