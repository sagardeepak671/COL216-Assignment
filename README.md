# COL216 Assignment 2
# Pipeline Simulator: RISCV - Forwarding and Non - Forwarding

To run the simulator,type
```
make
```
to run the a particular file, put the file in the inputfiles folder and type
```
./forwarding ./inputfiles/<filename>
```
for the processor with forwarding , and type
```
./non_forwarding ./inputfiles/<filename>
```
the output will be available in the outputfiles folder in forwarding / non_forwarding folder with the same file name as the input.

alternatively you can use `make run` to run all the files in the inputfiles folder . 

## Project Overview

This project simulates the 5 stages of **RISC-V pipeline** :
- **Instruction Fetch (IF)**
- **Instruction Decode (ID)**
- **Execute (EX)**
- **Memory Access (MEM)**
- **Write Back (WB)**

We built two versions:
- **No-Forwarding:** We use stalls whenever we get a data hazard.
- **Forwarding:** We make use of forwarding to reduce the number of stalls.
---

## Design & Implementation

### Memory :
- We modelled the memory as an `uint_8t` array of size 10000 , we access this in the MEM stage and do the store and load operation accordingly.

### Registers :
- We defined an `int` array to store the values of the 32 registers.

The pipeline_stage class stores the instruction and the the instruction number ( i.e the line number of that instruction in the input file) 
```
class pipeline_stage{
public:
    int pc_address=0;
    int IF_instruction_number=-1;
    string instruction_fetched;//used in ID stage
    int ID_instruction_number=-1;
    instruction instruction_decoded;//used in EX stage
    int EX_instruction_number=-1;
    instruction instruction_executed;//used in MEM stage
    int MEM_instruction_number=-1;
    instruction instruction_memory_accessed;//used in WB stage
    int WB_instruction_number=-1;
};
```

### 1.Input in IF Stage
- We take the machine code as the input from the file whenever that particular instruction is fetched in the IF stage as per the PC counter
- We store the machine code as a string in `instruction_fetched`

### 2.Instruction Decode in ID stage
- We then decode the machine code and store the relevant information of the instruction in a struct which is defined as follows :-
```
  struct instruction{
    char type;
    string opcode; 
    uint8_t rd = 32;
    uint8_t rs1 = 32;
    uint8_t rs2 = 32;
    int imm; 
    uint8_t funct3;
    uint8_t funct7;
    int result;
    int rs1_value;
    int rs2_value;
};
```
- In case there is some error in the given machine code, the program stops execution and shows `wrong input`.

- We also do jump of the jump statement at this stage.
- We resolve branches in this stage.
- Due to the jump in case the jump goes out of bounds the program stops fetching instructions.
- At this stage, we check whether the source register in this instruction is equal to the destination register of the instruction `instruction_executed` or the destination register of instruction `instruction_memory_accessed` .
  - In case of non_forwarding processor we stall at the ID stage till the previous instruction due to which stall is hapenning completes the WB stage.
  - In case of forwarding processor we forward the value of a the result if possible and stall if it is not possible to do so.
- Here we store the value in the register ( we get this value either directl from register or through forwarding) in the instruction struct in `rs1_value` and `rs2_value`

### 3. Execution in EX Stage.
- Here we do ALU operations like adding, subtracting ,etc of values of registers and store in the `result` of instruction struct

### 4. Memory Acess in MEM Stage.
-  Here we access the memory array and store and retreive the values according to the instruction.

### 5. Writeback in WB Stage.
- Here we update the value of the resistors in the resistor array.

### Data Hazards :
- As already mentioned above we handle datahazards by comparing the rs value in ID stage with the rs registers in EX and MEM stage.
- for branch instruction, we need the value of rs before the ID stage, so we stall it in the ID stage if we cant forward the data from the previous instructions in the stages ahead of it.
- In case of load instructions we can forward the data of the rd only after the MEM stage, so this might cause stalls in the instruction following it.
- We simpl forward the value by updating the `rs1_value` or the `rs2_value` of the instruction which we need to forward data to.

### Control Hazards :
- We check the branch statements in this stage and do the jump accordingly.
- By default we assume that branch is not taken and fetch the next statement, in case the branch is taken we flush the statement and jump to the branch.
- We update the pc and fetch the next instruction according to result of the branch instruction.


### Limitations/Issues :
- Didnt Implement Branch prediction,by default we assume branch not taken and after resolution of branch if branch is taken , we just flush the previously fetched instruction . This can be made better by using better branch prediction algorithms.
- Instruction fetching is not very efficient. Every time an instruction is to be fetched , we iterate through every line from the first line to the the line number of our instruction and then take that string as input.

### Sources Consulted :
- We used chatgpt to get the machine codes of various instructions and to know the general syntax of c++ of things like taking input from a file, writing into a file,etc.


