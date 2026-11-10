local dir = 1

function void.tick(self, dt)
	local t = self.Transform2D

	if t then
		if t.pos.x >= 200 then
			dir = -1
		elseif t.pos.x <= -200 then
			dir = 1
		end

		t.pos += Vector2.new(dir, 0)
	end
end
