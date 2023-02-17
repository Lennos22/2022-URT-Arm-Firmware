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

## Installation

### Prerequisites

- [STM32CubeIDE (version 1.11.2)](https://www.st.com/en/development-tools/stm32cubeide.html)
- [Docker](https://www.docker.com)

### Windows

```
TODO...
```

### Linux

```
TODO...
```

## ROS 2 Packages

```
TODO... Link The appropriate package repo or smth, idk...
```

### Adding Custom Packages

## Rebuilding micro-ROS

Must be performed when `micro_ros_stm32cubemx_utils` is updated, a change in the `micro_ros_stm32cubemex_utils` distro is required, or when a new [ROS 2 package dependency](#ros-2-packages) has been added.

To rebuild, follow these steps:
1. Go to `micro_ros_stm32cubemx_utils/microros_static_library_ide/libmicroros` and delete `libmicroros.a` (or, you can rename/move it if you want a backup for some reason). This will "trick" the build script into thinking that micro-ROS is no longer built into the project.
2. Double check that `micro_ros_stm32cubemx_utils` is checked out to the latest version on your desired branch, and/or you have your [custom packages](#adding-custom-packages) correctly added into the build system
3. Build and run the project.

Zac Sux
