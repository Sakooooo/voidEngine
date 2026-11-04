print("Hello I am a script!")

-- local input = require("input")

function void.tick(self, dt)
	-- print(self)
	-- print(dt)
    -- print("Tick")

    -- print(self:has("Transform2D"))
    -- print(self:has("fun"))

  -- print(input.is_pressed("w"))
  if input.is_pressed("w") then
    print("W is being held!")
  end
if input.is_pressed("s") then
    print("S is being held!")
  end
if input.is_pressed("d") then
    print("D is being held!")
  end
if input.is_pressed("a") then
    print("A is being held!")
  end
end
