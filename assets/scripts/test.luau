print("Hello I am a script!")

-- local input = require("input")

local vector = Vector2.new(20, 50)
print(vector)
print("This is my X value" .. tostring(vector.x))
print("This is my Y value" .. tostring(vector.y))

local vector2 = Vector2.new(50, 50)
print(vector2)
print("This is my second X value" .. tostring(vector2.x))
print("This is my second Y value" .. tostring(vector2.y))

local newvector = vector + vector2
print(newvector)
print("This is my added X value" .. tostring(newvector.x))
print("This is my added Y value" .. tostring(newvector.y))

newvector:normalize()
print(newvector)
print("This is my normalized X value" .. tostring(newvector.x))
print("This is my normalized Y value" .. tostring(newvector.y))

newvector = newvector * 2
print(newvector)
print("This is my 2x X value" .. tostring(newvector.x))
print("This is my 2x Y value" .. tostring(newvector.y))

function void.tick(self, dt)
	-- print(self)
	-- print(dt)
	-- print("Tick")

	-- print(self:has("Transform2D"))
	-- print(self:has("fun"))
	--
	-- print("width: " .. self.Transform2D.width)
	-- print("height: " .. self.Transform2D.height)
	-- print("x: " .. self.Transform2D.pos.x)
	-- print("y: " .. self.Transform2D.pos.y)

	local c = self.Color

	local dir = Vector2.new(0, 0)

	if input.is_pressed("w") then
		if c then
			if c.r >= 255 then
				c.r = 0
			else
				c.r += 1
			end
		end
		dir.y -= 1
	end
	if input.is_pressed("s") then
		dir.y += 1
		-- print("S is being held!")
	end
	if input.is_pressed("d") then
		if c then
			if c.b >= 255 then
				c.b = 0
			else
				c.b += 1
			end
		end
		dir.x += 1
		-- print("D is being held!")
	end
	if input.is_pressed("a") then
		if c then
			if c.g >= 255 then
				c.g = 0
			else
				c.g += 1
			end
		end
		dir.x -= 1
		-- print("A is being held!")
	end

	dir = dir:normalize()

	local t = self.Transform2D

	if t then
		t.pos += dir * 200 * dt
	end
end
