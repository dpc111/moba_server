package.path = "../tools/?.lua"
require "common"

local maxNumLen = 15
--------------------------------------------------------------------------
-- type： 该行的类型： 0，注释   1，定义  -1，空行
--------------------------------------------------------------------------
function loadConstsCsv(csvfile)
	local data = {}
	-- 找到上面的第一个有效定义行， 即上一行为注释时需要继续往上找
	local function findForwardLine(curLine)
		if data[curLine-1].type == 1 then
			return curLine - 1
		else
			return findForwardLine(curLine - 1)
		end
	end
	local csv = io.open("csv\\" .. csvfile, "r")
	if not csv then
		print("file open error", csvfile)
		return false
	end
	local row = 0
	for csvLine in csv:lines() do
		row = row + 1
		data[row] = {}
		local tbLine = splitString(csvLine, ",")
		local id = tbLine[1] or "\"\""
		if id ~= "" then
			local _, y = string.find(id, "//")
			if y ~= nil then
				data[row].type = 0
				data[row].content = tbLine[1]
			else
				local value = tbLine[2]
				local comment = tbLine[3]
				if value == "\"\"" or value == "" then
					value = data[findForwardLine(row)].content[2] + 1
				elseif  string.byte(value) > string.byte("9") then
					for _, v in pairs(data) do							-- 从已经加载的table里寻找
						if v.type == 1 and v.content[1] == value then
							value = v.content[2] 
							break
						end
					end
					if type(value) == "string" then
						error("can not find" .. ",csvfile:" .. csvfile .. ",row:"..row .. ",value:" .. value)
					end
				else
					value = tonumber(value)
				end
				data[row].type = 1
				data[row].content = {id, value, comment}
			end
		else
			data[row].type = -1
		end
	end
	local _, count = string.gsub(csvfile, "%.", "%.")					-- "."出现的次数
	local x = 1															-- 解析csv文件名，确定生成文件的名称
	if count > 1 then
		for i = 1, count-1  do
			x = string.find(csvfile, "%.", x)+1
		end
	end
	local y = string.find(csvfile, "%.", x)
	local line = row
	local maxLen = 1													-- 计算名称最大长度，排版用
	for k, v in pairs(data) do
		if v.type == 1 then
			if maxLen < #v.content[1] then
				maxLen = #v.content[1]
			end
		end
	end
	local headFileName = string.format("cpp/%sh", string.sub(csvfile, x, y))
	createHeadFile(data, line, headFileName, csvfile, maxLen)
	local path = string.gsub(string.sub(csvfile, 1, y-1), "%.", "/")
	local luaFielName = string.format("lua/%s.lua", path)
	createLuaFile(data, line, luaFielName, csvfile, maxLen, string.sub(csvfile, 1, y-1))

	local className = string.upper(string.sub(path, 1, 1)) .. string.sub(path, 2, -1)
	local csharpFielName = string.format("csharp/%s.cs", className)
	createCsharpFile(data, line, csharpFielName, csvfile, maxLen, className)
	return true
end

--------------------------------------------------------------------------
-- 生成 .lua文件
--------------------------------------------------------------------------
function createLuaFile(data, line, fileName, csvFile, maxLen, absFileName)
	local fp = io.open(fileName, "w")
    if not fp then
        error("open file error:" .. fileName)
    end
    fp:write("-----------------------------------------------------\n")
    fp:write("-- do not modify the file, gen by const/gen.bat\n")
    fp:write("-----------------------------------------------------\n")
	local curTable = ""
	local commentBuff= ""
	local tables = {}
	for row = 1, line do
		if data[row].type == 0 then
			fp:write(string.format("%s\n", string.gsub(data[row].content, "/", "-")))
		elseif data[row].type == -1 then
			fp:write("\n")
		else
			local id = data[row].content[1]
			local _,y = string.find(id, "%.")
			local _, count = string.gsub(id, "%.", "%.")						-- 得到ID中“.”的个数
			if count > 0 then
				local oldX = 1
				for k = 0, count-1 do
					local _, x = string.find(string.sub(id, oldX, #id), "%.")	-- 从上次找到“.”的地方继续往后找
					if x ~= nil then
						x = x + oldX
						if tables[string.sub(id, 0, x-2)] == nil then			-- 新的table都会保存在tables中，每次解析ID时，都会去tables中去检索，没有找到则创建新的table
							tables[string.sub(id, 0, x-2)] = true
							fp:write(string.format("%s = {}\n", string.sub(id, 0, x-2)))
						end
						oldX = x
					end
				end

			end
			fp:write(string.format("%s= %s-- %s\n", 
				align(id, maxLen + 4), 
				align(tostring(data[row].content[2]), maxNumLen),
				data[row].content[3] or""))
		end
	end
	fp:close()
end

function createHeadFile(data, line, fileName, csvfile, maxLen)
    local fp = io.open(fileName, "w")
    if not fp then
        error("open file error:" .. fileName)
    end
    fp:write("/////////////////////////////////////////////////////\n")
    fp:write("// do not modify the file, gen by const/gen.bat\n")
    fp:write("/////////////////////////////////////////////////////\n")
	for row = 1, line do
		if data[row].type == 0 then
			fp:write(string.format("%s\n", data[row].content))
		elseif data[row].type == -1 then
			fp:write("\n")
		else
			fp:write(string.format("#define %s%s// %s\n", 
				align(string.gsub(data[row].content[1], "%.", "_"), maxLen + 4), 
				align(data[row].content[2], maxNumLen), 
				data[row].content[3] or ""))
		end
	end
	fp:close()
end

function createCsharpFile(data, line, fileName, csvfile, maxLen, className)
	local fp = io.open(fileName, "w")
    if not fp then
        error("open file error:" .. fileName)
    end
    fp:write("/////////////////////////////////////////////////////\n")
    fp:write("// do not modify the file, gen by const/gen.bat\n")
    fp:write("/////////////////////////////////////////////////////\n")

    fp:write([[
using System.Collections;
using System.Collections.Generic;
using System.IO;
using System.Text;

public class ]])
    fp:write(className)
    fp:write(" {\n")
    for row = 1, line do
		if data[row].type == 0 then
			fp:write(string.format("	%s\n", data[row].content))
		elseif data[row].type == -1 then
			fp:write("\n")
		else
			fp:write(string.format("	public const int %s= %s// %s\n", 
				align(string.gsub(data[row].content[1], "%.", "_"), maxLen + 4), 
				align(data[row].content[2] .. ";", maxNumLen), 
				data[row].content[3] or ""))
		end
	end
    fp:write("}")
	fp:close()
end

function genConst()
    print("csv to const files---------------------------begin")
	local files = getpathes("csv\\", files)
	for _, fname in pairs(files or {}) do
		print(fname)
		loadConstsCsv(fname)
	end
    print("csv to const files---------------------------end")
end

genConst()
