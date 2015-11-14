-- print("=====================================")
-- print("helloworld")

-- local meta = getmetatable(renderSys)

-- for k,v in pairs(meta) do
-- 	print(k,v)
-- end

-- renderSys:DrawString("===")
-- -- print(render)


-- -- print(sprite)
-- print(renderSys)

-- for k,v in pairs(meta) do
-- 	for kk,vv in pairs(v) do
-- 		print(kk,vv)
-- 	end
-- end
-- print(_VERSION)

-- for k,v in pairs(_G) do
-- 	print(k,v)
-- end
local function main()
	-- local a = newbuffer()
	-- print(a)
	-- a.print()
	-- local meta = getmetatable(a)
	-- for k,v in pairs(meta) do
	-- 	print(k,v)
	-- end

	print("RenderSystem")
	print(RS)
	local sprite = RS.CreateSprite()
	local meta = getmetatable(sprite)
	for k,v in pairs(meta) do
		print(k,v)
	end
	print("sprite", sprite )
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