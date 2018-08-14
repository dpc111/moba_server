package.path = "../tools/?.lua"
require "common"

function csvToJson(preName)
    local csvFileName = "csv\\" .. preName .. ".csv"
    local jsonFileName = "json\\" .. preName .. ".json"
    local csvFile = io.open(csvFileName, "r")
    local jsonFile = io.open(jsonFileName, "w")
    local index = 0
    local names = {}
    local types = {}
    
    jsonFile:write("[" .. lineBreak())
    local has_data_1 = false
    for csvFileLine in csvFile:lines() do
        index = index + 1
        if index > 1 then
            if index == 3 then
                names = splitString(csvFileLine, ",")
            elseif index == 2 then
                types = splitString(csvFileLine, ",")
            else
                local elementData = splitString(csvFileLine, ",")
                if has_data_1 then
                    jsonFile:write("," .. lineBreak())
                end
                jsonFile:write(tab(1) .. "{" .. lineBreak())
                local has_data_2 = false
                for k, v in pairs(elementData) do 
                    if has_data_2 then
                        jsonFile:write("," .. lineBreak()) 
                    end  
                    if types[k] == "index" then
                        if v == "\"\"" then
                            print("key wrong")
                            break
                        end
                        jsonFile:write(tab(2) .. "\"" .. names[k] .. "\"" .. " : " .. v)
                    elseif types[k] == "int" then
                        if v == "" then
                            v = 0
                        end
                        jsonFile:write(tab(2) .. "\"" .. names[k] .. "\"" .. " : " .. v)
                    elseif types[k] == "float" then
                        if v == "" then
                            v = 0.0
                        end
                        jsonFile:write(tab(2) .. "\"" .. names[k] .. "\"" .. " : " .. v)
                    elseif types[k] == "string" then
                        if v == "\"\"" then
                            jsonFile:write(tab(2) .. "\"" .. names[k] .. "\"" .. " : " ..  v)
                        else
                            jsonFile:write(tab(2) .. "\"" .. names[k] .. "\"" .. " : \"" ..  v .. "\"")
                        end
                    elseif types[k] == "list" then
                        jsonFile:write(tab(2) .. "\"" .. names[k] .. "\" : [")
                        local v1 = string.gsub(v, "|", ",")
                        jsonFile:write(v1)
                        jsonFile:write("]")
                    else
                        print("type wrong")    
                    end
                    has_data_2 = true
                end
                jsonFile:write(lineBreak() .. tab(1) .. "}")
                has_data_1 = true
            end
        end
    end
    jsonFile:write(lineBreak() .. "]")
    
    csvFile:close()
    jsonFile:close()
end

function GenJson()
    print("csv to json file---------------------------begin")
    local files = {}
    getpathes("csv\\", files)
    for _, fname in pairs(files or {}) do
        local tb = splitString(fname, ".")
        assert(#tb > 0)
        local name = tb[1]
        print(name)
        csvToJson(name)
    end
    print("csv to json file---------------------------end")
end

GenJson()
