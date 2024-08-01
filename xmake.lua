if (os.exists("local_config.lua")) then
    includes("local_config.lua")
    load_configs()
end

add_rules("mode.debug", "mode.release")

add_requires("glfw", "glad")
add_requires("imgui docking", {configs = {glfw = true, opengl3 = true}})
add_requires("boost")
add_requires("fmt")

add_languages("c++17")

target("main", function()
    add_packages("glfw", "glad", "imgui")
    add_packages("boost")
    add_packages("fmt")

    if (is_mode("debug")) then
        add_defines("BILIXUI_DEBUG")
    end

    add_defines("BILIXUI_ASSET_PATH=\"$(projectdir)/asset\"")
    add_defines("BILIXUI_DEFAULT_FONT=\"SourceHanSansCN-Normal.otf\"")

    add_files("src/*.cpp")
end)
