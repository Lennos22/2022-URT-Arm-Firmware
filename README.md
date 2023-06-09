# Arm Firmware

The robotic arm firmware for controlling the movement of the arm. **(This sentence is incredibly stupid and redundant. Who the hell wrote this...?)**

Firmware for the Robotic Arm. Developed using the STM32CubeIDE, which uses the GCC toolchain. It uses micro-ROS to interface with ROS 2 (Foxy) via TTL UART.

## Table of Contents

- [Table of Contents](#table-of-contents)
- [Dependencies](#dependencies)
- [Prerequisites](#prerequisites)
- [Setup Procedure](#setup-procedure)
	- [Import repository to STM32CubeIDE](#import-repository-to-stm32cubeide)
	- [Create the "libmicroros" folder](#create-the-libmicroros-folder)
	- [Get packages listed in ROS 2 packages](#get-packages-listed-in-ros-2-packages)
	- [Build and run the project!](#build-and-run-the-project)
- [Interfacing with ROS 2](#interfacing-with-ros-2)
- [ROS 2 Packages](#ros-2-packages)
	- [Adding Custom Packages](#adding-custom-packages)
- [Rebuilding micro-ROS](#rebuilding-micro-ros)

## Dependencies

- Windows 10 (If installing on Windows)
- Ubuntu 20.04 (If installing on Linux)
- [ROS 2 (Foxy)](https://docs.ros.org/en/foxy/index.html)
- [micro-ROS](https://micro.ros.org/)
	- [micro_ros_stm32cubemx_utils](https://github.com/micro-ROS/micro_ros_stm32cubemx_utils)

## Prerequisites

- [STM32CubeIDE (version 1.11.2)](https://www.st.com/en/development-tools/stm32cubeide.html)
- [Docker](https://www.docker.com)
	- You can install [Docker Desktop](https://www.docker.com/)

		OR
	- You can install [Docker Engine](https://docs.docker.com/engine/install/ubuntu/) using `apt` repository on Ubuntu

## Setup Procedure

There's some minor, yet important, differences when building on either Operating System.

### Import repository to STM32CubeIDE

There are two ways you can do this:
- **Option A:** Install EGit using the STM32CubeIDE Eclipse Marketplace (`Help -> Eclipse Marketplace` and search for "EGit") and import the project in STM32CubeIDE via `File -> Import` and select `Git -> Projects from Git`
	- You need to select the `stm32` branch
	- Ensure "Clone submodules" is ticked
- **Option B:** Clone the repository and import the project in STM32CubeIDE
	- Checkout the `stm32` branch
	- MAKE SURE you **fetch the submodules**

(**Note** that [`micro_ros_stm32cubemx_utils`](https://github.com/micro-ROS/micro_ros_stm32cubemx_utils) is a submodule of the repo, and for some reason submodules have their HEADs on the non-`Main` branches detached when they're cloned. You may want to ensure that you `git checkout` the `foxy` branch before you move on to the next step. Though, from my testing, this doesn't seem completely necessary. Still, it doesn't hurt...)  

### Create the "libmicroros" folder

Create a new folder called `libmicroros` in `<project_dir>/micro_ros_stm32cubemx_utils/microros_static_library_ide/`. You can do this in your File Explorer or you can do it in the STM32CubeIDE Project Explorer

### Get packages listed in [ROS 2 packages](#get-ros-2-packages)

See the [Adding Custom Packages](#adding-custom-packages) section for more information

> **Note** I don't quite know where to put the packages, yet...

### Build and run the project!

The build script for `micro_ros_stm32cubemx_utils` requires [Docker](https://docs.docker.com/engine/install/ubuntu/):
- Windows users: Ensure that **Docker is running** everytime you build
- Linux users: Ensure that that [non-privileged users can run Docker](https://docs.docker.com/engine/install/linux-postinstall/)

## Interfacing with ROS 2

> TODO... For now, go to [First micro-ROS Application on FreeRTOS](https://micro.ros.org/docs/tutorials/core/first_application_rtos/freertos/) for more information 

## ROS 2 Packages

No packages to add, currently :D

> TODO... Link The appropriate package repo(s) or smth, idk...

### Adding Custom Packages

Add your packages to `microros_static_library_ide/library_generation/extra-packages` folder or, if you have your packages stored in an online repository, the folder also contains a file called `extra_packages.repos` where you can add an entry containing your package repo.

More information can be found [here](https://github.com/micro-ROS/micro_ros_stm32cubemx_utils#adding-custom-packages).

## Rebuilding micro-ROS

You will generally want to do this when `micro_ros_stm32cubemx_utils` is updated; a change in the `micro_ros_stm32cubemex_utils` distro is required; or when a new [ROS 2 package dependency](#ros-2-packages) has been added.

To rebuild micro-ROS, follow these steps:
1. Go to `micro_ros_stm32cubemx_utils/microros_static_library_ide/libmicroros` and delete `libmicroros.a` (or, you can rename/move it if you want a backup for some reason). This will "trick" the build script into thinking that micro-ROS is no longer built into the project.
2. Double check that [`micro_ros_stm32cubemx_utils`](https://github.com/micro-ROS/micro_ros_stm32cubemx_utils) is checked out to the latest version on your desired branch, and/or you have your [custom packages](#adding-custom-packages) correctly added into the build system
3. Build and run the project!

Zac Sux :P