package.path = "../tools/?.lua"
require "common"

-- 仅一个下划线情况  默认配置不用下划线
function rename()
    local files = getpathes("csv\\", files)
    for _, fname in pairs(files or {}) do
        local strs = splitString(fname, "_")
        local fname1 = fname
        if #(strs or {}) > 1 then
            fname1 = strs[1] .. ".csv"
        end
        print(fname, fname1)
        fname = "csv\\" .. fname
        fname1 = "csv\\" .. fname1
        os.rename(fname, fname1)
    end
    local init
end

rename()