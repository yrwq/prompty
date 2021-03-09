local user = "  " .. prompty.user()
local host = "  " .. prompty.hostname()
local sep = " | "
prompty.prompt_symbol = " "

-- remove /home/$USER from pwd
function pwd()
    local dir = prompty.pwd()
    local home = os.getenv("HOME")
    if string.match(dir, home) then
        return dir:gsub(home, "  ~")
    else
        return "  " .. dir
    end
end

-- function to show git branch if we are in a
-- git repository
function git()
    if prompty.git_branch() then
        return "   " .. prompty.git_branch()
    else
        return ""
    end
end

function prompty.init()

    -- prompty.bubble(host)
    -- prompty.bubble(user)
    -- prompty.bubble(pwd())
    -- prompty.bubble(git())

    prompty.format(pwd() .. git())
    prompty.prompt()


    -- if you don't want that fancy stuff
    -- print(pwd() .. " " .. prompty.prompt_symbol)

end
