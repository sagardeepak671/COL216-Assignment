# COL216 Assignment 2
# Pipeline Simulator: RISC-V Edition (Forwarding vs. No-Forwarding)

## Non-Forwarding Pipeline vs Forwarding Pipeline
This project simulates a 5-stage RISC V pipeline with two implementations:
- **Noforwarding.cpp:** A classic 5-stage pipeline that uses stalls to handle data hazards.
- **Forwarding.cpp:** An optimized version that employs data forwarding to cut down on stalls and boost performance.

---

## 📚 Table of Contents

- [Project Overview]
- [Design & Implementation]
  - [Instruction Decoding](#instruction-decoding)
  - [Pipeline Stages](#pipeline-stages)
  - [Hazard Busters](#hazard-busters)
- [Directory Layout](#directory-layout)
- [How to Run the Simulator](#how-to-run-the-simulator)
- [Known Issues & Limitations](#known-issues--limitations)
- [Contributing](#contributing)
- [License](#license)

---

## Project Overview

This project simulates a **5-stage RISC-V pipeline** where we go deep into:
- **Instruction Fetch (IF)**
- **Instruction Decode (ID)**
- **Execute (EX)**
- **Memory Access (MEM)**
- **Write Back (WB)**

We built two versions:
- **No-Forwarding:** Uses stalls to deal with data hazards.
- **Forwarding:** Leverages data forwarding to reduce stalls and speed things up.
---

## Design & Implementation

### Instruction Decoding 🔍

We break down each instruction with a neat `instruction` struct:
- **type & opcode:** Define what kind of operation it is.
- **rd, rs1, rs2:** Manage our registers.
- **imm, funct3, funct7:** Deal with immediates and function codes.
- **result, rs1_value, rs2_value:** Track the outcome and operand values.

This struct helps us decode raw machine code into easy-to-use instructions for our pipeline.

### Pipeline Stages ⏩

Our simulator uses a `pipeline_stage` class to handle every stage of the pipeline:
- **IF:** Snags instructions from our test files.
- **ID:** Decodes the fetched instruction.
- **EX:** Executes ALU operations.
- **MEM:** Manages memory reads/writes.
- **WB:** Updates the register file.

Each stage carries its own metadata (instruction numbers, PC address, etc.), so you can follow every step in our cycle-by-cycle simulation.

### Hazard Busters🚧 

#### Data Hazards:
- **No-Forwarding:**  
  Uses an `in_use` flag for registers to stall when dependencies show up.
  
- **Forwarding:**  
  Smooths things out by forwarding data between the EX/MEM and MEM/WB stages. We even added an `in_use2` flag to cover tricky edge cases (like branch instructions using a register immediately after it's been updated).

#### Control Hazards:
- Branch decisions are sorted out in the **ID stage**. If a branch is taken, our pipeline flushes outdated instructions and jumps to the target address. No more wasted cycles!

### Code Highlights

- **`push_next_stage`:**  
  Transitions instructions between stages, handling stalls and jumps effortlessly.
  
- **`get_rs_values`:**  
  Fetches register values and checks for hazards in real-time.
  
- **`compute`:**  
  Manages all stage computations per cycle, updating registers, memory, and more.
  
- **`proccessor`:**  
  The mastermind function that runs the whole simulation over a set number of cycles.

---

## Directory Layout 📁
.
├── src
│   ├── forwarding.cpp        # Pipeline with forwarding
│   ├── non_forwarding.cpp    # Pipeline with non-forwarding
│   ├── instructions.hpp      # Instruction struct and helpers
|   ├── instructions.cpp      
│   ├── main.cpp              # Main simulation logic
│   ├── main.hpp              # Main header file
│   ├── utils.hpp             
|   └── utils.cpp             # Utility functions
├── input                     # Test case files
├── output                    # Logs & simulation outputs
├── Makefile                  # Quick build and run script
├──instruction_types.txt      # help with types of instructions
└── README.md                 # This rad documentation file


## HOW TO RUN THE SIMULATOR
Step 1: Clone the repository
Step 2: Build the Project using command "make" (without quotes in terminal)
Step 3: For No-Forwarding:
        run command "./noforwarding <input_file>"
        For Forwarding:
        run command "./forwarding <input_file>"
        where <input_file> is the path of input file
Step 4: Check the output in output folder

