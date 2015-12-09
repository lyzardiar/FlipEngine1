package.path = package.path..";script/?.lua"
local vk = require("virtualkeycode")

local function check_render()
	print("-----------------------------------------")
	print(renderSys)
	print(render)
	for k,v in pairs(render) do
		print(k,v)
	end
end 

local function check_class(newfunc)
	print("-------------", newfunc, "----------------")
	if not render[newfunc] then
		print("render ", newfunc, "not find")
		return
	end

	local obj = render[newfunc]()
	local meta = getmetatable(obj)
	for k, v in pairs(meta) do
		print(k,v)
	end
end

local animodel
local function main()
	check_render()
	-- check_class("newsprite")
	-- check_class("newmodel")
	-- check_class("newcamera")
	-- check_class("newanimodel")

	-- camera = render.newcamera()
	camera:setup3DCamera(800, 600)

	-- local sp = render.newsprite()
	-- sp:setTexture("../Media/flare1.jpg")
	-- sp:setPosition(300, 300, 0)
	-- render.addsprite(sp)

	-- local sp = render.newsprite()
	-- sp:setTexture("../Media/flare1.jpg")
	-- sp:setPosition(0, 0, 0)
	-- sp:setViewProj(camera:getViewProj())
	-- render.addsprite(sp)

	animodel = render.newanimodel()
	animodel:setFile("../Media/ninja.b3d")
	animodel:setPosition(0, 0, -10)
	animodel:setViewProj(camera:getViewProj())
	render.addanimodel(animodel)

	-- animodel:update(2)

	-- animodel:update(1)
	local model = render.newmodel()
	model:setFile("../Media/Cube.3ds")
	model:setPosition(0, 0, -10)
	model:setViewProj(camera:getViewProj())
	render.addmodel(model)
end

xpcall(main,
function( ... )
	print(debug.traceback())
end)

function frameUpdate()
	-- print("frameUpdate")
	animodel:update(0.01)
end

function onKey(key)
	if not camera then
		return
	end

	if key == vk.w or key == vk.W then
		camera:walk(1)
	elseif key == vk.s or key == vk.S then
		camera:walk(-1)
	elseif key == vk.a or key == vk.A then
		-- camera:rotateByAxis(0, 1, 0, -2)
		camera:yaw(5)
	elseif key == vk.d or key == vk.D then
		camera:yaw(-5)
	end
end

-- for k,v in pairs(_G) do
-- 	print(k,v)
-- end