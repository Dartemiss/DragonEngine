<h1 align="center" style="border-bottom: none;">Dragon Engine</h1>

## What is Dragon Engine?

![](https://github.com/Dartemiss/DragonEngine/blob/master/Utils/DragonEngineGif.gif)

- Game Engine developed with C++.
- Libraries used: [SDL 2.0](https://www.libsdl.org/), [glew 2.1.0](http://glew.sourceforge.net/), [MathGeoLib 3.0.0](https://github.com/juj/MathGeoLib/), [Dear ImGui 1.73](https://github.com/ocornut/imgui/), [DevIL 1.8](http://openil.sourceforge.net/), [assimp 5.0](https://github.com/assimp/assimp/), [OpenGL](https://www.opengl.org/), [Brofiler](https://github.com/bombomby/optick/), [Font Awersome 5.12.0](https://github.com/juliettef/IconFontCppHeaders/), [ImGuizmo](https://github.com/CedricGuillemet/ImGuizmo/), [PCG Minimal C 0.9](http://www.pcg-random.org/) and [Rapidjson 1.1.0](https://github.com/Tencent/rapidjson/).

- Project developed for [UPC programming AAA videogames master's degree](https://www.talent.upc.edu/ing/estudis/formacio/curs/201200/master-degree-advanced-programming-aaa-videogames/).

- Author: [Enrique Alexandre González Sequeira](https://dartemiss.github.io) and [Jordi Romagosa Mellado](https://github.com/JordiRomagosa)

- Repository: https://github.com/Dartemiss/DragonEngine

- Licence: This project is licensed under the MIT License - see the [LICENSE](https://github.com/Dartemiss/DragonEngine/blob/master/LICENSE) file for details.

## Installation
- Download the release that you want from [Release section](https://github.com/Dartemiss/DragonEngine/releases)
- Unzip "DragonEngine.zip"
- Execute "DragonEngine.exe"

## How to use it
- Unity-like controls.

- `RightClick` + `W` `A` `S` `D` `Q` &rarr; For moving arround the scene.

- `RightClick` + Mouse movement &rarr; FPS camera

- `Alt` + `Left Click` &rarr; Orbit arround the model.

- `MouseWheel` &rarr; Zoom

- Dragging and Dropping an fbx to the scene will load the model with their textures.

- Click over an object to selected or click its name on hierarchy.

- Modify transform from inspector or using Guizmos.

- Save and Load Scene from GUI tab Scene.

- Press the play button to active game time and see the main camera perspective

- Pressing `ESC` will quit the app.

- Static objects are inserted on a quadtree and can't be transformed. Dynamic objects can be transformed and are inserted on an aabbTree

## GUI
- For knowing more about the project click on the menu bar on `Help` and then `About`.

- For showing the console click on the menu bar on `Debug` and then on `Console`.

- For showing information about variables of the window, hardware and Input options click on the menu bar `View` and then on Window.

- For showing the camera position, change the vertical fov and enable and disable the boundig box drawing or show quadtree,aabbTree,Skybox,grid or frustum
click on the menu bar on `View` and then on `Camera`.

- For showing model information and more accurate texture information of the current loaded textures,
click on the menu bar on `View` and then on `Properties`.

- Change view of the UI Scene and Game seeing both at the same time going to `Window` and check: `See Scene and Game together`

## Work Division
### Enrique González Sequeira
- Original version of the model viewer.
- Framerate,timers, game and real time management.
- Frustum culling.
- Scene Management.
- Quadtree and aabbtree.
- Profiling.
- Mouse picking.
- Guizmos
- Aabbtree for dynamic objects and Quadtree for static objects.
- Prepare scene.
- Play, stop, pause game
- Skybox
- Debug Draw
- Shapes
- UI

### Jordi Romagosa
- Shaders and Lighting.
- Binary file importing and loading.
- Filesystem.
- Scene serialization.
- Materials.
- Realtime lighting.
- Help with scene preparation, importing ZomBunny after detecting error with .fbx.

Enjoy! :)
