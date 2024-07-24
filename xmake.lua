add_rules("mode.debug", "mode.release")

add_requires("glfw", "glad")
add_requires("imgui docking", {configs = {glfw = true, opengl3 = true}})
add_requires("boost")

add_languages("c++17")
add_ldflags("-lstdc++fs")

target("main", function()
    add_packages("glfw", "glad", "imgui")
    add_packages("boost")

    add_defines("BILIXUI_ASSET_PATH=\"$(projectdir)/asset\"")
    add_defines("BILIXUI_DEFAULT_FONT=\"SourceHanSansCN-Normal.otf\"")

    add_files("src/*.cpp")
end)
