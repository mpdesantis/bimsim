// Standard
#include <chrono>
#include <fstream>
#include <string>
#include <iomanip>
#include <iostream>

// Other
#include "nlohmann/json.hpp"

// Cadmium V2
#include <cadmium/modeling/celldevs/grid/coupled.hpp>
#include <cadmium/simulation/logger/csv.hpp>
#include <cadmium/simulation/root_coordinator.hpp>

// Models
#include "include/bimsimCell.hpp"

using namespace cadmium::celldevs;
using namespace cadmium;

// Print help menu
void printHelp() {
    std::cout << "\n\t*** Cell-DEVS BIM Simulator Utility ***\n";
    std::cout << "\nUsage: bimsim [OPTIONS]";
    std::cout << "\n\nOptions:";
    std::cout << "\n  --config, -c     FILE   Take configuration from FILE.";
    std::cout << "\n                            Default: config/bimsim_config.json";
    std::cout << "\n  --duration, -d   NUM    Execute simulation for NUM time units.";
    std::cout << "\n                            Default: 20";
    std::cout << "\n  --output, -o     FILE   Log CSV output to FILE.";
    std::cout << "\n                            Default: output/bimsim_grid_log.csv";
    std::cout << "\n  --help, -h              Print this menu and exit.";
    std::cout << std::endl;
}

// Model
std::shared_ptr<GridCell<BimSimState, double>> addGridCell(const coordinates & cellId, const std::shared_ptr<const GridCellConfig<BimSimState, double>>& cellConfig) {
    auto cellModel = cellConfig->cellModel;

    if (cellModel == "bimsim") {
        return std::make_shared<bimsim>(cellId, cellConfig);
    } else {
        throw std::bad_typeid();
    }
}

// Simulation
int main(int argc, char ** argv) {

    /* Constants */
    const std::string DEFAULT_CONFIG_FILE_PATH = "config/bimsim_config.json";
    constexpr int DEFAULT_SIM_TIME = 96;
    const std::string DEFAULT_OUTPUT_FILE_PATH = "output/bimsim_grid_log.csv";

    /* Defaults */
    std::string configFilePath = DEFAULT_CONFIG_FILE_PATH;
    double simTime = DEFAULT_SIM_TIME;
    std::string outputFilePath = DEFAULT_OUTPUT_FILE_PATH;

    /* Argument Parsing */
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        // 1. Configuration File (JSON)
        if (arg == "--config" || arg == "-c") {
            if (i + 1 < argc) {
                configFilePath = argv[++i];
            }
            else {
                std::cout << "Missing argument for switch: [--config | -c]" <<std::endl;
                return 1;
            }
        }
        // 2. Simulation Duration (double)
        else if (arg == "--duration" || arg == "-d") {
            if (i + 1 < argc) {
                simTime = std::stod(argv[++i]);
            }
            else {
                std::cout << "Missing argument for switch: [--duration | -d]" <<std::endl;
                return 1;
            }
        }
        // 3. Output File (JSON)
        else if (arg == "--output" || arg == "-o") {
            if (i + 1 < argc) {
                outputFilePath = argv[++i];
            }
            else {
                std::cout << "Missing argument for switch: [--output | -o]" <<std::endl;
                return 1;
            }
        }
        // 4. Help Menu
        else if (arg == "--help" || arg == "-h") {
            printHelp();
            return 0;
        }
        // Invalid Argument
        else {
            std::cout << "Invalid argument: " << arg <<std::endl;
            printHelp();
            return 1;
        }

    }

    // Print simulation parameters to console
    std::cout << "\nAttempting simulation with the following parameters:";
    std::cout << "\n\tConfig File: " << configFilePath;
    std::cout << "\n\tSimulation Duration: " << simTime;
    std::cout << "\n\tOutput File: " << outputFilePath;
    std::cout << std::endl;

    /* Models */
    auto model = std::make_shared<GridCellDEVSCoupled<BimSimState, double>>("bimsim", addGridCell, configFilePath);
    model->buildModel();
    
    /* Logs */
    auto rootCoordinator = RootCoordinator(model);
    rootCoordinator.setLogger<CSVLogger>(outputFilePath, ";");
    
    /* Simulation */

    // Run simulation
    rootCoordinator.start();
    rootCoordinator.simulate(simTime);
    rootCoordinator.stop();

    // Report results and return
    std::cout << "\nSimulation complete.\n";
    return 0;
}
