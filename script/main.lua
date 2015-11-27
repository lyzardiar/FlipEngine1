local function check_render()
	print("=========================================")
	print(renderSys)
	print(render)
	for k,v in pairs(render) do
		print(k,v)
	end
	print("=========================================")
end 

local function check_sprite()
	local sprite = render.newsprite()
	local meta = getmetatable(sprite)
	for k,v in pairs(meta) do
		print(k,v)
	end

	local pos = sprite:getPosition()
	for k,v in pairs(pos) do
		print(k,v)
	end
end

local function main()
	check_render()
	check_sprite()

	local sp = render.newsprite()
	sp:setTexture("../Media/engineflare1.jpg")
	render.addsprite(sp)
end

xpcall(main,
function( ... )
	print(debug.traceback())
end)

function frameUpdate()
	-- print("frameUpdate")
end

-- for k,v in pairs(_G) do
-- 	print(k,v)
-- end