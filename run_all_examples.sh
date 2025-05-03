#!/bin/bash
###############################################################################
# run_all_examples.sh
# -------------------
# Run all simulation examples according to defined configurations.
#
# Usage:
# ------
# $ ./run_all_examples.sh
#
# Positional Arguments:
#   NA
#
# Optional Arguments:
#   NA
# 
# Notes:
# ------
# Ensure you have built the binaries with the `build_sim.sh` script before use.
# Ensure this script has executable permission before use.
#
###############################################################################
# Author: M. De Santis
# Date: 2025/04/26
###############################################################################

set -e

SCRIPT=$(readlink -f "$0")
SCRIPT_DIR=$(dirname ${SCRIPT})

echo -e "EXECUTING $SCRIPT...\n"

# Binary
BIN_DIR=${PWD}/bin
SIMULATION_BINARY=${BIN_DIR}/bimsim

# Duration
DEFAULT_SIM_TIME=96

# Examples
EXAMPLES_DIR=${PWD}/examples

# Example 01
EX01_DIR=${EXAMPLES_DIR}/ex01
## Configuration 01
EX01_C01_DIR=${EX01_DIR}/c01
### Variation 01
EX01_C01_V01_DIR=${EX01_C01_DIR}/v01
EX01_C01_V01_CONFIG=${EX01_C01_V01_DIR}/ex01_c01_v01_bimsim_config.json
EX01_C01_V01_OUTPUT=${EX01_C01_V01_DIR}/ex01_c01_v01_bimsim_grid_log.csv
### Variation 02
EX01_C01_V02_DIR=${EX01_C01_DIR}/v02
EX01_C01_V02_CONFIG=${EX01_C01_V02_DIR}/ex01_c01_v02_bimsim_config.json
EX01_C01_V02_OUTPUT=${EX01_C01_V02_DIR}/ex01_c01_v02_bimsim_grid_log.csv
## Configuration 02
EX01_C02_DIR=${EX01_DIR}/c02
### Variation 01
EX01_C02_V01_DIR=${EX01_C02_DIR}/v01
EX01_C02_V01_CONFIG=${EX01_C02_V01_DIR}/ex01_c02_v01_bimsim_config.json
EX01_C02_V01_OUTPUT=${EX01_C02_V01_DIR}/ex01_c02_v01_bimsim_grid_log.csv
### Variation 02
EX01_C02_V02_DIR=${EX01_C02_DIR}/v02
EX01_C02_V02_CONFIG=${EX01_C02_V02_DIR}/ex01_c02_v02_bimsim_config.json
EX01_C02_V02_OUTPUT=${EX01_C02_V02_DIR}/ex01_c02_v02_bimsim_grid_log.csv

# Example 02
EX02_DIR=${EXAMPLES_DIR}/ex02
## Configuration 01
EX02_C01_DIR=${EX02_DIR}/c01
### Variation 01
EX02_C01_V01_DIR=${EX02_C01_DIR}/v01
EX02_C01_V01_CONFIG=${EX02_C01_V01_DIR}/ex02_c01_v01_bimsim_config.json
EX02_C01_V01_OUTPUT=${EX02_C01_V01_DIR}/ex02_c01_v01_bimsim_grid_log.csv
### Variation 02
EX02_C01_V02_DIR=${EX02_C01_DIR}/v02
EX02_C01_V02_CONFIG=${EX02_C01_V02_DIR}/ex02_c01_v02_bimsim_config.json
EX02_C01_V02_OUTPUT=${EX02_C01_V02_DIR}/ex02_c01_v02_bimsim_grid_log.csv
## Configuration 02
EX02_C02_DIR=${EX02_DIR}/c02
### Variation 01
EX02_C02_V01_DIR=${EX02_C02_DIR}/v01
EX02_C02_V01_CONFIG=${EX02_C02_V01_DIR}/ex02_c02_v01_bimsim_config.json
EX02_C02_V01_OUTPUT=${EX02_C02_V01_DIR}/ex02_c02_v01_bimsim_grid_log.csv
### Variation 02
EX02_C02_V02_DIR=${EX02_C02_DIR}/v02
EX02_C02_V02_CONFIG=${EX02_C02_V02_DIR}/ex02_c02_v02_bimsim_config.json
EX02_C02_V02_OUTPUT=${EX02_C02_V02_DIR}/ex02_c02_v02_bimsim_grid_log.csv

# Example 03
EX03_DIR=${EXAMPLES_DIR}/ex03
## Configuration 01
EX03_C01_DIR=${EX03_DIR}/c01
### Variation 01
EX03_C01_V01_DIR=${EX03_C01_DIR}/v01
EX03_C01_V01_CONFIG=${EX03_C01_V01_DIR}/ex03_c01_v01_bimsim_config.json
EX03_C01_V01_OUTPUT=${EX03_C01_V01_DIR}/ex03_c01_v01_bimsim_grid_log.csv
### Variation 02
EX03_C01_V02_DIR=${EX03_C01_DIR}/v02
EX03_C01_V02_CONFIG=${EX03_C01_V02_DIR}/ex03_c01_v02_bimsim_config.json
EX03_C01_V02_OUTPUT=${EX03_C01_V02_DIR}/ex03_c01_v02_bimsim_grid_log.csv
## Configuration 02
EX03_C02_DIR=${EX03_DIR}/c02
### Variation 01
EX03_C02_V01_DIR=${EX03_C02_DIR}/v01
EX03_C02_V01_CONFIG=${EX03_C02_V01_DIR}/ex03_c02_v01_bimsim_config.json
EX03_C02_V01_OUTPUT=${EX03_C02_V01_DIR}/ex03_c02_v01_bimsim_grid_log.csv
### Variation 02
EX03_C02_V02_DIR=${EX03_C02_DIR}/v02
EX03_C02_V02_CONFIG=${EX03_C02_V02_DIR}/ex03_c02_v02_bimsim_config.json
EX03_C02_V02_OUTPUT=${EX03_C02_V02_DIR}/ex03_c02_v02_bimsim_grid_log.csv


###############################################################################
# Run all simulation examples
echo -e "\nExecuting all simulation examples...\n"

# Ex01: High Occupancy
echo -e "\n********************************************************************"
echo -e "Executing all simulations for Example 01: High Occupancy Work Day..."
${SIMULATION_BINARY} -c ${EX01_C01_V01_CONFIG} -o ${EX01_C01_V01_OUTPUT} -d ${DEFAULT_SIM_TIME} 
${SIMULATION_BINARY} -c ${EX01_C01_V02_CONFIG} -o ${EX01_C01_V02_OUTPUT} -d ${DEFAULT_SIM_TIME}
${SIMULATION_BINARY} -c ${EX01_C02_V01_CONFIG} -o ${EX01_C02_V01_OUTPUT} -d ${DEFAULT_SIM_TIME} 
${SIMULATION_BINARY} -c ${EX01_C02_V02_CONFIG} -o ${EX01_C02_V02_OUTPUT} -d ${DEFAULT_SIM_TIME}

# Ex02: Half Occupancy
echo -e "\n********************************************************************"
echo -e "Executing all simulations for Example 02: Half Occupancy Work Day..."
${SIMULATION_BINARY} -c ${EX02_C01_V01_CONFIG} -o ${EX02_C01_V01_OUTPUT} -d ${DEFAULT_SIM_TIME} 
${SIMULATION_BINARY} -c ${EX02_C01_V02_CONFIG} -o ${EX02_C01_V02_OUTPUT} -d ${DEFAULT_SIM_TIME}
${SIMULATION_BINARY} -c ${EX02_C02_V01_CONFIG} -o ${EX02_C02_V01_OUTPUT} -d ${DEFAULT_SIM_TIME} 
${SIMULATION_BINARY} -c ${EX02_C02_V02_CONFIG} -o ${EX02_C02_V02_OUTPUT} -d ${DEFAULT_SIM_TIME}

# Ex03: Low Occupancy
echo -e "\n********************************************************************"
echo -e "Executing all simulations for Example 03: Low Occupancy Work Day..."
${SIMULATION_BINARY} -c ${EX03_C01_V01_CONFIG} -o ${EX03_C01_V01_OUTPUT} -d ${DEFAULT_SIM_TIME} 
${SIMULATION_BINARY} -c ${EX03_C01_V02_CONFIG} -o ${EX03_C01_V02_OUTPUT} -d ${DEFAULT_SIM_TIME}
${SIMULATION_BINARY} -c ${EX03_C02_V01_CONFIG} -o ${EX03_C02_V01_OUTPUT} -d ${DEFAULT_SIM_TIME} 
${SIMULATION_BINARY} -c ${EX03_C02_V02_CONFIG} -o ${EX03_C02_V02_OUTPUT} -d ${DEFAULT_SIM_TIME}

# All simulations complete
echo -e "\n********************************************************************"
echo -e "All simulations executed successfully."
echo -e "\n--- YOU EXECUTED ---"
