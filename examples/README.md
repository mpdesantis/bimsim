# README
* Author: Michael De Santis
* CUID: 101213450
* Date: 2025/04/29

## Description
Example configurations. 

## Examples
Examples in this directory are organized as follows:

### Example 1: High Occupany 
* Occupancy: 44/48 workstations occupied
* Configuration 1: Scattered occupancy pattern
    * Variation 1: Occupancy sensors disabled
    * Variation 2: Occupancy sensors enabled
* Configuration 2: Condensed occupancy pattern 
    * Variation 1: Occupancy sensors disabled
    * Variation 2: Occupancy sensors enabled
    
### Example 2: Half Occupany 
* Occupancy: 24/48 workstations occupied
* Configuration 1: Scattered occupancy pattern
    * Variation 1: Occupancy sensors disabled
    * Variation 2: Occupancy sensors enabled
* Configuration 2: Condensed occupancy pattern 
    * Variation 1: Occupancy sensors disabled
    * Variation 2: Occupancy sensors enabled
    
### Example 3: Low Occupany 
* Occupancy: 8/48 workstations occupied
* Configuration 1: Scattered occupancy pattern
    * Variation 1: Occupancy sensors disabled
    * Variation 2: Occupancy sensors enabled
* Configuration 2: Condensed occupancy pattern 
    * Variation 1: Occupancy sensors disabled
    * Variation 2: Occupancy sensors enabled

## File Organization
Configuration files associated with the above example experiments are organized in this directory as follows:
```sh
.
├── ex01
│   ├── c01
│   │   ├── v01
│   │   │   └── ex01_c01_v01_bimsim_config.json
│   │   └── v02
│   │       └── ex01_c01_v02_bimsim_config.json
│   └── c02
│       ├── v01
│       │   └── ex01_c02_v01_bimsim_config.json
│       └── v02
│           └── ex01_c02_v02_bimsim_config.json
├── ex02
│   ├── c01
│   │   ├── v01
│   │   │   └── ex02_c01_v01_bimsim_config.json
│   │   └── v02
│   │       └── ex02_c01_v02_bimsim_config.json
│   └── c02
│       ├── v01
│       │   └── ex02_c02_v01_bimsim_config.json
│       └── v02
│           └── ex02_c02_v02_bimsim_config.json
└── ex03
    ├── c01
    │   ├── v01
    │   │   └── ex03_c01_v01_bimsim_config.json
    │   └── v02
    │       └── ex03_c01_v02_bimsim_config.json
    └── c02
        ├── v01
        │   └── ex03_c02_v01_bimsim_config.json
        └── v02
            └── ex03_c02_v02_bimsim_config.json
```

## Notes
* To execute these examples, run the `run_all_examples.sh` script.
* Sample output of one execution of these examples may be found in `sample-results/`
