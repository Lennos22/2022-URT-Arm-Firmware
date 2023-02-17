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

```
TODO...
```

### Linux

1. Install [STM32CubeIDE](https://www.st.com/en/development-tools/stm32cubeide.html) and open it
2. **Optional:** Install EGit in the STM32CubeIDE Eclipse Marketplace (`Help -> Eclipse Marketplace` and search for EGit)
3. Either clone the repo and import it as an STM32CubeIDE project, or you can import the project using EGit (`File -> Import` and select `Git -> Projects from Git`) if you installed it  
(Note that [`micro_ros_stm32cubemx_utils`](https://github.com/micro-ROS/micro_ros_stm32cubemx_utils) is a submodule of the repo, and for some reason submodules have Detached HEADs when they're cloned. You may want to ensure that you `git checkout` the `foxy` branch before you move on to the next step, though, from my testing, this doesn't seem completely necessary. Still, it doesn't hurt...)
4. Ensure [Docker is installed](https://docs.docker.com/engine/install/ubuntu/) on your machine and that [non-privileged users can run Docker](https://docs.docker.com/engine/install/linux-postinstall/)
5. Add the packages listed in [ROS 2 Packages](#ros-2-packages) to the [`micro_ros_stm32cubemx_utils`](https://github.com/micro-ROS/micro_ros_stm32cubemx_utils) build system (see [here](#adding-custom-packages))
```
May have to tweak 5. depending on where the packages are gonna be stored...
```
6. Build and run the project

## ROS 2 Packages

```
TODO... Link The appropriate package repo or smth, idk...
```

### Adding Custom Packages

Add your packages to `microros_static_library_ide/library_generation/extra-packages/` or, if you have your packages stored in an online repository, the folder also contains a file called `extra_packages.repos` where you can add an entry containing your package repo information.

More information can be found [here](https://github.com/micro-ROS/micro_ros_stm32cubemx_utils#adding-custom-packages).

## Rebuilding micro-ROS

You will generally want to do this when [`micro_ros_stm32cubemx_utils`](https://github.com/micro-ROS/micro_ros_stm32cubemx_utils) is updated, a change in the `micro_ros_stm32cubemex_utils` distro is required, or when a new [ROS 2 package dependency](#ros-2-packages) has been added.

To rebuild, follow these steps:
1. Go to `micro_ros_stm32cubemx_utils/microros_static_library_ide/libmicroros` and delete `libmicroros.a` (or, you can rename/move it if you want a backup for some reason). This will "trick" the build script into thinking that micro-ROS is no longer built into the project.
2. Double check that [`micro_ros_stm32cubemx_utils`](https://github.com/micro-ROS/micro_ros_stm32cubemx_utils) is checked out to the latest version on your desired branch, and/or you have your [custom packages](#adding-custom-packages) correctly added into the build system
3. Build and run the project.

Zac Sux
