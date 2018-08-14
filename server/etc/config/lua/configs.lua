require("bullet")
require("entity")

g_cfgs = {
   ["bullet"] = cfg_bullet,
   ["entity"] = cfg_entity,
}

function cfg_get(name)
    return g_cfgs[name]
end

function cfg_get_line(name, index)
    return g_cfgs[name][index]
end

function cfg_get_value(name, index, key)
    return g_cfgs[name][index][key]
end
