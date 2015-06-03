local lltxplatform = require "lltxplatform"

local fonts = lltxplatform.get_installed_fonts()

for k, v in pairs(fonts) do
    print(k, v.name, v.path)
end

local fonts = lltxplatform.get_inactive_fonts()

for k, v in pairs(fonts) do
    print(k, v)
end
