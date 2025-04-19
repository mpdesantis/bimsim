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
# Date: 2025/03/23
###############################################################################

set -e

SCRIPT=$(readlink -f "$0")
SCRIPT_DIR=$(dirname ${SCRIPT})

echo -e "EXECUTING $SCRIPT...\n"

# Binary
BIN_DIR=${PWD}/bin
SIMULATION_BINARY=${BIN_DIR}/map_generation

# Config
CONFIG_DIR=${PWD}/config

EX01_CONFIG_DIR=${PWD}/config/ex01
EX01_V01_CONFIG=${EX01_CONFIG_DIR}/ex01_v01_mapgen_config.json
EX01_V02_CONFIG=${EX01_CONFIG_DIR}/ex01_v02_mapgen_config.json
EX01_V03_CONFIG=${EX01_CONFIG_DIR}/ex01_v03_mapgen_config.json

EX02_CONFIG_DIR=${PWD}/config/ex02
EX02_V01_CONFIG=${EX02_CONFIG_DIR}/ex02_v01_mapgen_config.json
EX02_V02_CONFIG=${EX02_CONFIG_DIR}/ex02_v02_mapgen_config.json
EX02_V03_CONFIG=${EX02_CONFIG_DIR}/ex02_v03_mapgen_config.json

EX03_CONFIG_DIR=${PWD}/config/ex03
EX03_V01_CONFIG=${EX03_CONFIG_DIR}/ex03_v01_mapgen_config.json
EX03_V02_CONFIG=${EX03_CONFIG_DIR}/ex03_v02_mapgen_config.json
EX03_V03_CONFIG=${EX03_CONFIG_DIR}/ex03_v03_mapgen_config.json

# Output
OUTPUT_DIR=${PWD}/output

EX01_V01_OUTPUT=${OUTPUT_DIR}/ex01_v01_mapgen_grid_log.csv
EX01_V02_OUTPUT=${OUTPUT_DIR}/ex01_v02_mapgen_grid_log.csv
EX01_V03_OUTPUT=${OUTPUT_DIR}/ex01_v03_mapgen_grid_log.csv

EX02_V01_OUTPUT=${OUTPUT_DIR}/ex02_v01_mapgen_grid_log.csv
EX02_V02_OUTPUT=${OUTPUT_DIR}/ex02_v02_mapgen_grid_log.csv
EX02_V03_OUTPUT=${OUTPUT_DIR}/ex02_v03_mapgen_grid_log.csv

EX03_V01_OUTPUT=${OUTPUT_DIR}/ex03_v01_mapgen_grid_log.csv
EX03_V02_OUTPUT=${OUTPUT_DIR}/ex03_v02_mapgen_grid_log.csv
EX03_V03_OUTPUT=${OUTPUT_DIR}/ex03_v03_mapgen_grid_log.csv


###############################################################################
# Run all simulation examples
echo -e "Executing all simulation examples...\n"

# Ex01
echo -e "Executing simulation example 01...\n"
${SIMULATION_BINARY} -c ${EX01_V01_CONFIG} -o ${EX01_V01_OUTPUT}
${SIMULATION_BINARY} -c ${EX01_V02_CONFIG} -o ${EX01_V02_OUTPUT}
${SIMULATION_BINARY} -c ${EX01_V03_CONFIG} -o ${EX01_V03_OUTPUT}

# Ex02
echo -e "Executing simulation example 02...\n"
${SIMULATION_BINARY} -c ${EX02_V01_CONFIG} -o ${EX02_V01_OUTPUT}
${SIMULATION_BINARY} -c ${EX02_V02_CONFIG} -o ${EX02_V02_OUTPUT}
${SIMULATION_BINARY} -c ${EX02_V03_CONFIG} -o ${EX02_V03_OUTPUT}

# Ex03
echo -e "Executing simulation example 03...\n"
${SIMULATION_BINARY} -c ${EX03_V01_CONFIG} -o ${EX03_V01_OUTPUT}
${SIMULATION_BINARY} -c ${EX03_V02_CONFIG} -o ${EX03_V02_OUTPUT}
${SIMULATION_BINARY} -c ${EX03_V03_CONFIG} -o ${EX03_V03_OUTPUT}

echo -e "\n--- YOU EXECUTED ---"

