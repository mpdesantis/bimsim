# README
* Author: Michael De Santis
* CUID: 101213450
* Date: 2025/03/23

## Description
Modeling and Simulating Post-Pandemic Work Practices with Building Information Modeling (BIM) and Cell Discrete Event System Specifications (Cell-DEVS). For additional information, please see the `doc/` directory.

## Contents
This repository's contents.

* `bin/`
    - Generated directory containing built executables for this repository.
* `build/`
    - Generated directory containing build files for this repository.
* `build_sim.sh`
    - Shell script defining build rules for host-architecture target.
* `cadmium_v2/`
    - Cadmium V2 repository `git` submodule.
* `CMakeLists.txt`
    - CMake build instructions for this repository.
* `doc/`
    - Directory containing documentation for this repository.
        1. Report: `REPORT.pdf`
        2. docs
* `env.sh`
    - Shell script defining additional environment variables for this repository.
* `example-output/`
    - Directory containing sample output logs from model simulation, accompanied by the corresponding visualization files (images and videos).
* `img/`
    - Directory containing images for this repository, as included inline in the project report.
* `main/`
    - C++ source and header files for this repository's main code module, `map_generation`.
* `output/`
    - Generated directory containing output logs from model simulation.
* `run_all_examples.sh`
    - Shell script to run all available simulation examples from pre-defined configuration.
    
## Usage
Usage instructions for this project and repository.

### Installing and Configuring Cadmium V2
In order to compile this repository's source code, your system must have [Cadmium V2](https://github.com/Sasisekhar/cadmium_v2) available on your system, and accessible via the environment variable `$CADMIUM`. This library, and its own dependencies, are included within this repository as a `git` submodule. To ensure that your system's submodules are up-to-date prior to compilation, issue the following command:
```sh
$ git submodule update --init --recursive
```
Once the `cadmium_v2` sudmodule directory is available on your file system, enter that directory and follow the instructions provided therein for installation and configuration.  Once complete, you should be able to verify that your configuration is correct with the output of the following command:
```sh
$ echo $CADMIUM
<path-to>/Map_Generation/cadmium_v2/include
```
If the above environment variable is discrepant on your system, you can overwrite it by sourcing `env.sh` in this directory with the following command:
```sh
$ . env.sh
```

### Build
To build this project, issue the following command:
```sh
$ . build_sim.sh
```
Find the resultant binary at `bin/map_generation`.

### Configure an Experiment for Simulation
To configure a sample experiment for simulation, you may copy and modify the template configuration files provided in `config/templates/`. For examples, see `config/ex01/`, `config/ex02/`, and `config/ex03/`.

### Execute Custom Simulation
Tp see a list of runtime options and defaults for this project's binary, issue the following command:
```sh
$ ./bin/map_generation [-h | --help]
```
For example, an invocation with all three parameters receiving arguments may look as follows:
```sh
$ ./bin/map_generation \
  --config config/ex01/ex01_v01_mapgen_config.json \    
  --output output/ex01_v01_mapgen_grid_log.csv \ 
  --duration 20                           
```

### Execute Examples
To execute this project's example simulations, issue the following command:
```sh
$ ./run_all_examples.sh
```
Upon successful execution, corresponding output for each simulation binary will be found in the `output/` directory.
Results from a single execution, with corresponding visuals (images and video) may be found in the `example-output/` directory. These examples are referenced in the project report.

### Visualizing the Results
Carleton's DEVSsim [Cell-DEVS Viewer](https://devssim.carleton.ca/cell-devs-viewer://devssim.carleton.ca/cell-devs-viewer/) may be used to visualize the simulations executed by this project's binary. To ensure your files are recognized by the visualizer, constrain your file names as follows:
1. The JSON simulation configuration file:
    * `<name>_config.json`
2. The JSON simulation visualization configuration file:
    * `<name>Visualization_config.json`
3. The CSV output grid log file:
    * `<name>_grid_log.csv`

## Notes
* With permission and by instruction, this project uses the [blank project template](https://github.com/Sasisekhar/blank_project_rt) provided in the Cadmium V2 manual.
* With permission and by instruction, this project is informed by the [sample project](https://github.com/Sasisekhar/cell-devs-manual-example) provided in the Cadmium V2 manual.
* With permission, this project is informed by and reuses parts of the author's original work for the [LEO User Link](https://github.com/mpdesantis/LEO_User_Link).
* Thanks Professor Wainer!! 
