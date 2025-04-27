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

# Ex01 
EX01_DIR=${EXAMPLES_DIR}/ex01
EX01_V01_DIR=${EX01_DIR}/v01
EX01_V01_CONFIG=${EX01_V01_DIR}/ex01_v01_bimsim_config.json
EX01_V01_OUTPUT=${EX01_V01_DIR}/ex01_v01_bimsim_grid_log.csv
EX01_V02_DIR=${EX01_DIR}/v02
EX01_V02_CONFIG=${EX01_V02_DIR}/ex01_v02_bimsim_config.json
EX01_V02_OUTPUT=${EX01_V02_DIR}/ex01_v02_bimsim_grid_log.csv

# Ex02
EX02_DIR=${EXAMPLES_DIR}/ex02
EX02_V01_DIR=${EX02_DIR}/v01
EX02_V01_CONFIG=${EX02_V01_DIR}/ex02_v01_bimsim_config.json
EX02_V01_OUTPUT=${EX02_V01_DIR}/ex02_v01_bimsim_grid_log.csv
EX02_V02_DIR=${EX02_DIR}/v02
EX02_V02_CONFIG=${EX02_V02_DIR}/ex02_v02_bimsim_config.json
EX02_V02_OUTPUT=${EX02_V02_DIR}/ex02_v02_bimsim_grid_log.csv

# Ex03
EX03_DIR=${EXAMPLES_DIR}/ex03
EX03_V01_DIR=${EX03_DIR}/v01
EX03_V01_CONFIG=${EX03_V01_DIR}/ex03_v01_bimsim_config.json
EX03_V01_OUTPUT=${EX03_V01_DIR}/ex03_v01_bimsim_grid_log.csv
EX03_V02_DIR=${EX03_DIR}/v02
EX03_V02_CONFIG=${EX03_V02_DIR}/ex03_v02_bimsim_config.json
EX03_V02_OUTPUT=${EX03_V02_DIR}/ex03_v02_bimsim_grid_log.csv


###############################################################################
# Run all simulation examples
echo -e "Executing all simulation examples...\n"

# Ex01
#echo -e "Executing simulation example 01...\n"
${SIMULATION_BINARY} -c ${EX01_V01_CONFIG} -o ${EX01_V01_OUTPUT} -d ${DEFAULT_SIM_TIME} 
${SIMULATION_BINARY} -c ${EX01_V02_CONFIG} -o ${EX01_V02_OUTPUT} -d ${DEFAULT_SIM_TIME}

# Ex02
#echo -e "Executing simulation example 02...\n"
${SIMULATION_BINARY} -c ${EX02_V01_CONFIG} -o ${EX02_V01_OUTPUT} -d ${DEFAULT_SIM_TIME}
${SIMULATION_BINARY} -c ${EX02_V02_CONFIG} -o ${EX02_V02_OUTPUT} -d ${DEFAULT_SIM_TIME}

# Ex03
#echo -e "Executing simulation example 03...\n"
${SIMULATION_BINARY} -c ${EX03_V01_CONFIG} -o ${EX03_V01_OUTPUT} -d ${DEFAULT_SIM_TIME}
${SIMULATION_BINARY} -c ${EX03_V02_CONFIG} -o ${EX03_V02_OUTPUT} -d ${DEFAULT_SIM_TIME}

echo -e "\n--- YOU EXECUTED ---"
