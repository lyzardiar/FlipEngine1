local function checkRS()
	print("=========================================")
	print("RS, ", RS)
	for k, v in pairs(RS) do
		print(k,v)
	end
	
	print("renderSys", renderSys)
	-- assert(RS, "RS is null")
	local meta = getmetatable(renderSys)
	for k,v in pairs(meta) do
		print(k,v)
	end
	print("=========================================")
end 

local function checkSprite()
	local sprite = RS.newSprite()
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
	checkRS()
	checkSprite()
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