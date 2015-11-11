-- print("=====================================")
-- print("helloworld")
-- print(renderSys)

-- print(render)


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