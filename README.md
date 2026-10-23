# VTK VR CAD Viewer

A desktop 3D CAD viewer written in **C++** with **Qt** and **VTK**. You can load STL models of a Formula Student car, organise the parts in a tree view, edit how they look, and view the whole assembly in **virtual reality** through OpenVR/SteamVR.

## Features

- **STL loading**: open a single STL file, or a whole folder of parts at once
- **Tree view of model parts**: a custom `QAbstractItemModel` (`ModelPartList` / `ModelPart`) holds the assembly hierarchy
- **Part editing**: an options dialog to rename a part, change its colour and toggle its visibility, plus removing parts from the scene
- **VTK filters**: shrink and clip filters you can toggle on parts
- **Scene lighting** using `vtkLight`
- **VR mode**: start and stop a VR session from the GUI. The scene is mirrored into an OpenVR renderer running on its own thread (`VRRenderThread`), so the Qt interface stays responsive, and parts can be rotated in VR
- **VR controller bindings** for HTC Vive, Valve Index (Knuckles), Oculus Touch and HP Motion Controllers
- **Doxygen documentation** for all classes, [hosted on GitHub Pages](https://onafuwadav.github.io/vtk-vr-cad-viewer/)

## Tech stack

| Area | Tools |
|------|-------|
| Language | C++17 |
| GUI | Qt 6 (Widgets, Qt Creator `.ui` files, signals & slots) |
| 3D rendering | VTK (STL reader, mappers, actors, filters, lighting) |
| VR | VTK OpenVR module + SteamVR |
| Build | CMake 3.16+, Visual Studio |
| Docs | Doxygen |

## Project structure

```
src/
├── main.cpp                 # Application entry point
├── mainwindow.*             # Main window: render view, tree view, toolbar actions
├── optiondialog.*           # Dialog for editing a part's name, colour and visibility
├── ModelPart.*              # One node in the tree: an STL part and its VTK actor
├── ModelPartList.*          # Tree model (QAbstractItemModel) of all parts
├── VRRenderThread.*         # Runs the OpenVR render loop on a separate thread
├── vtk_openvr_*.json        # VR controller action manifest and bindings
├── icons.qrc, Icons/        # Toolbar icons
└── CMakeLists.txt
docs/                        # Doxygen-generated HTML documentation
```

## Building

**Requirements**

- Windows with Visual Studio 2022 (64-bit)
- Qt 6
- VTK **built from source** with Qt and OpenVR support switched on (`VTK_GROUP_ENABLE_Qt`, `VTK_MODULE_ENABLE_VTK_RenderingOpenVR`)
- CMake 3.16 or newer
- SteamVR and a supported headset (only needed for VR mode)

**Steps**

```bash
git clone https://github.com/Onafuwadav/vtk-vr-cad-viewer.git
cd vtk-vr-cad-viewer/src
cmake -S . -B build -DCMAKE_PREFIX_PATH="<path-to-Qt>;<path-to-VTK>"
cmake --build build --config Release
```

You can also open the generated Visual Studio solution in `build/` and run it from there. If you run the executable outside Visual Studio, the Qt and VTK DLLs need to be on your `PATH` or next to the `.exe`.

## Usage

1. Open an STL file or a folder of STL parts from the toolbar or the File menu.
2. Select a part in the tree, then open **Item Options** to change its name, colour or visibility.
3. Toggle the **Shrink** or **Clip** filter to change how parts are displayed.
4. Click **Start VR** to view the model in your headset, and **Stop VR** to end the session.

## Documentation

The Doxygen class and file reference is hosted at **[onafuwadav.github.io/vtk-vr-cad-viewer](https://onafuwadav.github.io/vtk-vr-cad-viewer/)**. The generated HTML source is in the [`docs/`](docs/) folder.

## Acknowledgements

- Template code for `ModelPart`, `ModelPartList` and `VRRenderThread` came from the module materials by P. Evans, University of Nottingham ([Template code](https://github.com/plevans/EEEE2076)).
- Built by Group 13 as part of group assessment.
