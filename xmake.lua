add_rules("mode.debug", "mode.release")

add_requires("glfw", "glad")
add_requires("imgui docking", {configs = {glfw = true, opengl3 = true}})

target("main", function()
    add_packages("glfw", "glad", "imgui")
    add_files("src/*.cpp")
end)
