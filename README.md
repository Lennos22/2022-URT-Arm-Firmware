# Arm Firmware

The robotic arm firmware for controlling the movement of the arm. **(This sentence is incredibly stupid and redundant. Who the hell wrote this...?)**

- [Dependencies](#dependencies)
- [Installation](#installation)
	- [Prerequisites](#prerequisites)
	- [Windows](#windows)
	- [Linux](#linux)
- [ROS 2 Packages](#ros-2-packages)
	- [Adding Custom Packages](#adding-custom-packages)
- [Rebuilding micro-ROS](#rebuilding-micro-ros)

## Dependencies

- Windows 10 (If installing on Windows)
- Ubuntu 20.04 (If installing on Linux)
- [ROS 2 (Foxy)](https://docs.ros.org/en/foxy/index.html)
- [micro-ROS](https://micro.ros.org/)
	- [micro_ros_stm32cubemx_utils](https://github.com/micro-ROS/micro_ros_stm32cubemx_utils)

## Installation

### Prerequisites

- [STM32CubeIDE (version 1.11.2)](https://www.st.com/en/development-tools/stm32cubeide.html)
- [Docker](https://www.docker.com)

### Windows

Install [STM32CubeIDE](https://www.st.com/en/development-tools/stm32cubeide.html) and run it
**Optional:** Install EGit in the STM32CubeIDE Eclipse Marketplace (`Help -> Eclipse Marketplace` and search for EGit)
Either clone the repo (don't forget to `init` and `update` the [`micro_ros_stm32cubemx_utils`](https://github.com/micro-ROS/micro_ros_stm32cubemx_utils) submodule) and import it as an STM32CubeIDE project, **or** you can import the project using EGit via `File -> Import` and select `Git -> Projects from Git` (ensure you have "Clone submodules" ticked)  

(Note that [`micro_ros_stm32cubemx_utils`](https://github.com/micro-ROS/micro_ros_stm32cubemx_utils) is a submodule of the repo, and for some reason submodules have their HEADs on the non-`Main` branches detached when they're cloned. You may want to ensure that you `git checkout` the `foxy` branch before you move on to the next step. Though, from my testing, this doesn't seem completely necessary. Still, it doesn't hurt...)  

**Important:** Replace all instances of `${workspace_loc:/${ProjName}}` in `Project -> Properties` with the with the `<ABSOLUTE_PATH>` to the project. E.g: `C:/Users/<USERNAME>/STM32CubeIDE/arm-firmware` (note the use of **forward slashes**):  
- Go to `Project -> Properties -> C/C++ Build -> Settings` and select the `Build Steps` tab. In the `Pre-build steps` you will see on the `Command` line:
```
docker pull microros/micro_ros_static_library_builder:foxy && docker run --rm -v ${workspace_loc:/${ProjName}}:/project --env MICROROS_LIBRARY_FOLDER=micro_ros_stm32cubemx_utils/microros_static_library_ide microros/micro_ros_static_library_builder:foxy
```
- Go to `Project -> Properties -> C/C++ Build -> Settings` and on the `Tool Settings` tab, select `MCU GCC Linker -> Libraries`. You will see `${workspace_loc:/${ProjName}}/micro_ros_stm32cubemx_utils/microros_static_library_ide/libmicroros` in `Library search path (-L)`
[Install Docker](https://www.docker.com/) on your machine and ensure that it is running (Docker **must be running** every time you build the project) 
**Important:** Add the packages listed in [ROS 2 Packages](#ros-2-packages) to the [`micro_ros_stm32cubemx_utils`](https://github.com/micro-ROS/micro_ros_stm32cubemx_utils) build system (see the [Adding Custom Packages](#adding-custom-packages) section)
> May have to tweak the above step depending on where the packages are gonna be stored...
Build and run the project!

### Linux

Install [STM32CubeIDE](https://www.st.com/en/development-tools/stm32cubeide.html) and run it
**Optional:** Install EGit in the STM32CubeIDE Eclipse Marketplace (`Help -> Eclipse Marketplace` and search for EGit)
Either clone the repo (don't forget to `init` and `update` the [`micro_ros_stm32cubemx_utils`](https://github.com/micro-ROS/micro_ros_stm32cubemx_utils) submodule) and import it as an STM32CubeIDE project, **or** you can import the project using EGit via `File -> Import` and select `Git -> Projects from Git` (ensure you have "Clone submodules" ticked)  

(Note that [`micro_ros_stm32cubemx_utils`](https://github.com/micro-ROS/micro_ros_stm32cubemx_utils) is a submodule of the repo, and for some reason submodules have their HEADs on the non-`Main` branches detached when they're cloned. You may want to ensure that you `git checkout` the `foxy` branch before you move on to the next step. Though, from my testing, this doesn't seem completely necessary. Still, it doesn't hurt...)  

Ensure [Docker is installed](https://docs.docker.com/engine/install/ubuntu/) on your machine and that [non-privileged users can run Docker](https://docs.docker.com/engine/install/linux-postinstall/)
**Important:** Add the packages listed in [ROS 2 Packages](#ros-2-packages) to the [`micro_ros_stm32cubemx_utils`](https://github.com/micro-ROS/micro_ros_stm32cubemx_utils) build system (see the [Adding Custom Packages](#adding-custom-packages) section)
> May have to tweak above step depending on where the packages are gonna be stored...
Build and run the project!

## ROS 2 Packages

No packages to add, currently :D

> TODO... Link The appropriate package repo(s) or smth, idk...

### Adding Custom Packages

Add your packages to `microros_static_library_ide/library_generation/extra-packages` folder or, if you have your packages stored in an online repository, the folder also contains a file called `extra_packages.repos` where you can add an entry containing your package repo.

More information can be found [here](https://github.com/micro-ROS/micro_ros_stm32cubemx_utils#adding-custom-packages).

## Rebuilding micro-ROS

You will generally want to do this when [`micro_ros_stm32cubemx_utils`](https://github.com/micro-ROS/micro_ros_stm32cubemx_utils) is updated; a change in the `micro_ros_stm32cubemex_utils` distro is required; or when a new [ROS 2 package dependency](#ros-2-packages) has been added.

To rebuild micro-ROS, follow these steps:
1. Go to `micro_ros_stm32cubemx_utils/microros_static_library_ide/libmicroros` and delete `libmicroros.a` (or, you can rename/move it if you want a backup for some reason). This will "trick" the build script into thinking that micro-ROS is no longer built into the project.
2. Double check that [`micro_ros_stm32cubemx_utils`](https://github.com/micro-ROS/micro_ros_stm32cubemx_utils) is checked out to the latest version on your desired branch, and/or you have your [custom packages](#adding-custom-packages) correctly added into the build system
3. Build and run the project!

Zac Sux
