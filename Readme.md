
# MyCPU project
It is my experiment of designing CPU and its' iSA. \
This project allows you to add new instructions to the processor and assemble it using a custom ASM.

## Folders structure
- `CPU` - cpu description.
- `Tools` - binary translation tools and etc.
- `Tools/Tests` - tests and examples.

## Requirements
- [Python3](https://www.python.org/downloads/)
- [CMake 3.10+](https://cmake.org/download/)

## How to build
### ASM
#### Windows/Linux:
Use `CMake` from folder `Tools` or use scripts from folder `Tools/Devops`.

### CPU
#### Linux:
- run `CPU/Scripts/InstallRequirements.sh` for first time
- use `make` from folder `CPU` or use scripts from folder `CPU/Scripts`
- to upload to the FPGA, use script `CPU/Scripts/LoadToFPGA.sh` or `sudo make load`
#### Windows:
Not supported, but you can try to use `GoWin`. \
When using `VirtualBox` you need to add your FPGA into the Settings/USB.

