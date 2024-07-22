#include "spimcore.h"

/*
    CDA3103C - Summer 24 - MySPIM Project 

    Developed by: Benedetto Falin, Matthew Eisenberg, Kayla Pham, and Zachary Watford.
*/

//Main Op-Code & Instruction Comparison Definitons:
    // J-Type Instructions
    #define JUMP 2          // Jump

    // I-Type Instructions
    #define BEQ 4           // Branch if equal
    #define ADDI 8          // Add immediate
    #define SLTI 10         // Set on less than immediate
    #define SLTIU 11        // Set on less than immediate unsigned
    #define LUI 15          // OR immediate
    #define LW 35           // Load word
    #define SW 43           // Store word

    //Funct Op-Codes - R-Type Instructions
    #define ALU_ADD 32      //Addition
    #define  ALU_SUB 34     //Subtraction
    #define  ALU_AND 36     //And &&
    #define  ALU_OR 37      //Or ||
    #define  ALU_SLL 6      //Shift Left Extended
    #define  ALU_SLT 42     //Set Less Than Signed
    #define  ALU_SLTU 43    //Set Less Than Signed

// ALU 
// 10 Points    -------------------------------------------------------------------------

// 1. Implement the operations on input parameters A and B according to ALUControl.
// 2. Output the result (Z) to ALUresult.
// 3. Assign Zero to 1 if the result is zero; otherwise, assign 0.
// 4. The following table shows the operations of the ALU.
void ALU(unsigned A, unsigned B, char ALUControl, unsigned *ALUresult, char *Zero) {

    switch (ALUControl) {
        case 0: //Addition 000
            *ALUresult = A + B;
            break;

        case 1: //Subtraction 001
            *ALUresult = A - B;
            break;

        case 2: //Signed A < B, Z = 1 Otherwise Z = 0 010
            if ((signed)A < (signed)B)   
                *ALUresult = 1;
            else 
                *ALUresult = 0;
            break;

        case 3: //Unsigned A < B, Z = 1; Otherwise Z = 0 011
            if (A < B)
                *ALUresult = 1;
            else 
                *ALUresult = 0;
            break;

        case 4: //Z = A AND B 100
            *ALUresult = A & B;
            break;

        case 5: //Z = A OR B 101
            *ALUresult = A | B;
            break;

        case 6: //Z = Shift B (extended value) Left By 16 Bits 110
            *ALUresult = B << 16;
            break;

        case 7: //Z = NOT A 111
            *ALUresult = ~A;
            break;

    }

    //Final result is zero
    if (*ALUresult == 0) 
        *Zero = 1;
    
    //Final result is not zero
    else
        *Zero = 0; // Based on instructions;
}



//Instruction Fetch
// 10 Points    -------------------------------------------------------------------------

// 1. Fetch the instruction addressed by PC from Mem and write it to instruction.
// 2. Return 1 if a halt condition occurs; otherwise, return 0.
int instruction_fetch(unsigned PC, unsigned *Mem, unsigned *instruction) {
    //FIGURE OUT A WAY TO DETECT BAD INSTRUCTIONS!!!

    
    //Check if instruction/word has proper allignment (halt condition)
    if (PC % 4 == 0) {
        //Properly alligned - write / do not halt
        *instruction = Mem[(PC >> 2)];

        return 0;
    }

    else
        //Not properly alligned - halt
        return 1;
}

//CREDIT for binary to hex conversion: https://www.rapidtables.com/convert/number/hex-to-binary.html
// Instruction Partition
// 10 Points    -------------------------------------------------------------------------

// 1. Partition instruction into several parts (op, r1, r2, r3, funct, offset, jsec).
// 2. Read line 41 to 47 of spimcore.c for more information.
void instruction_partition(unsigned instruction, unsigned *op, unsigned *r1, unsigned *r2, unsigned *r3, unsigned *funct, unsigned *offset, unsigned *jsec) {

    //Assigning Op-code
    *op = (0xFC000000 & instruction) >> 26; //0xFC000000 =  11111100000000000000000000000000 (bits 31-26)

    //Assigning r1
    *r1 = (0x3E00000 & instruction) >> 21;  //0x3E00000 =   00000011111000000000000000000000 (bits 25-21)

    //Assigning r2
    *r2 = (0x1F0000 & instruction) >> 16;   //0x1F0000 =    00000000000111110000000000000000 (bits 20-16)

    //Assigning r3
    *r3 = (0xF800 & instruction) >> 11;     //0xF800 =      00000000000000001111100000000000 (bits 15-11)

    //Assigning funct
    *funct = (0x3F & instruction);          //0x3F =        00000000000000000000000000111111 (bits 5-0)

    //Assigning offset
    *offset = (0xFFFF & instruction);       //0x7C0 =       00000000000000001111111111111111 (bits 15 - 0)

    //Assigning jsec
    *jsec = (0x03ffffff & instruction);     //0x03ffffff =  00000011111111111111111111111111 (bits 25-0)
                                                        
}

// instruction decode
// 15 Points    -------------------------------------------------------------------------

// 1. Decode the instruction using the opcode (op).
// 2. Assign the values of the control signals to the variables in the structure controls
int instruction_decode(unsigned op, struct_controls *controls) {
    
    //Checking all possible op-codes for implemented operations
    switch (op) {
        case 0: //Op-code: 000 000
            //Always R-Type Instruction

            controls->RegDst = 1;
            controls->Jump = 0;
            controls->Branch = 0;
            controls->MemRead = 0;
            controls->MemWrite = 0;
            controls->MemtoReg = 0;
            controls->RegWrite = 1;
            controls->ALUSrc = 0;
            controls->ALUOp = 7;
            break;

        case JUMP: //Op-code: 000 010
            //J-Type

            controls->RegDst = 0;
            controls->Jump = 1;
            controls->Branch = 0;
            controls->MemRead = 0;
            controls->MemWrite = 0;
            controls->MemtoReg = 0;
            controls->RegWrite = 1;
            controls->ALUSrc = 0;
            controls->ALUOp = 0;
            break;

        case BEQ: //Op-code: 000 100
            //Branch on Equal

            controls->RegDst = 2;
            controls->Jump = 0;
            controls->Branch = 1;
            controls->MemRead = 0;
            controls->MemWrite = 0;
            controls->MemtoReg = 2;
            controls->RegWrite = 0;
            controls->ALUSrc = 0;
            controls->ALUOp = 1;
            break;

        case ADDI: //Op-code: 001 000
            //Add on immediate

            controls->RegDst = 0;
            controls->Jump = 0;
            controls->Branch = 0;
            controls->MemRead = 0;
            controls->MemWrite = 0;
            controls->MemtoReg = 0;
            controls->RegWrite = 1;
            controls->ALUSrc = 1;
            controls->ALUOp = 0;
            break;

        case SLTI: //Op-code: 001 010
            //Set on less than immediate

            controls->RegDst = 1;
            controls->Jump = 0;
            controls->Branch = 0;
            controls->MemRead = 0;
            controls->MemWrite = 0;
            controls->MemtoReg = 0;
            controls->RegWrite = 1;
            controls->ALUSrc = 1;
            controls->ALUOp = 2;
            break;

        case SLTIU: //Op-code: 001 011
            //Set on less than immediate unsigned

            controls->RegDst = 1;
            controls->Jump = 0;
            controls->Branch = 0;
            controls->MemRead = 0;
            controls->MemWrite = 0;
            controls->MemtoReg = 0;
            controls->RegWrite = 1;
            controls->ALUSrc = 0;
            controls->ALUOp = 3;
            break;

        case LUI: //Op-code: 001 111
            //Load upper immediate

            controls->RegDst = 0;
            controls->Jump = 0;
            controls->Branch = 0;
            controls->MemRead = 0;
            controls->MemWrite = 0;
            controls->MemtoReg = 0;
            controls->RegWrite = 1;
            controls->ALUSrc = 1;
            controls->ALUOp = 6;
            break;

        case LW: //Op-code: 100 011
            //Load word

            controls->RegDst = 0;
            controls->Jump = 0;
            controls->Branch = 0;
            controls->MemRead = 1;
            controls->MemWrite = 0;
            controls->MemtoReg = 1;
            controls->RegWrite = 1;
            controls->ALUSrc = 1;
            controls->ALUOp = 0;
            break;

        case SW: //Op-code: 101 011
            //Store word
            controls->RegDst = 2;
            controls->Jump = 0;
            controls->Branch = 0;
            controls->MemRead = 0;
            controls->MemWrite = 1;
            controls->MemtoReg = 2;
            controls->RegWrite = 0;
            controls->ALUSrc = 1;
            controls->ALUOp = 0;
            break;

        default: //Returning 1 to Halt CPU - Invalid Operation
            return 1;
    }

    return 0;
}

// Read Register
// 5 Points     -------------------------------------------------------------------------

// 1. Read the registers addressed by r1 and r2 from Reg, and write the read values to data1 and data2 respectively
void read_register(unsigned r1, unsigned r2, unsigned *Reg, unsigned *data1, unsigned *data2) {

    //Data at address r1 in Reg written to data1
    *data1 = Reg[r1];

    //Data at address r2 in Reg written to data2
    *data2 = Reg[r2];
}

// Sign Extend
// 10 Points    -------------------------------------------------------------------------

// 1. Assign the sign-extended value of offset to extended_value.
void sign_extend(unsigned offset, unsigned *extended_value) {
    
    //Check for negative offset value
    unsigned negative_check = offset >> 15; //collect negative bit

    //Handling negative
    if (negative_check == 1)
        *extended_value = offset | 0xFFFF0000; //Extending with all 1s 11111111111111110000000000000000

    else
        *extended_value = offset & 0x0000FFFF; //Extending with all 0s 00000000000000001111111111111111

}

// ALU operations 
// 10 Points    -------------------------------------------------------------------------

// 1. Apply ALU operations on data1, and data2 or extended_value (determined by ALUSrc).
// 2. The operation performed is based on ALUOp and funct.
// 3. Apply the function ALU(...).
// 4. Output the result to ALUresult.
// 5. Return 1 if a halt condition occurs; otherwise, return 0.
int ALU_operations(unsigned data1, unsigned data2, unsigned extended_value, unsigned funct, char ALUOp, char ALUSrc, unsigned *ALUresult, char *Zero) {
    
    //Check R-Types for Funct for ALU operation 
    if (ALUOp == 7) {

        //Always R-Type
        switch (funct) {
            //Case of Addition
            case ALU_ADD:
                ALUOp = 0;
            break;

            //Case of Subtraction
            case ALU_SUB:
                ALUOp = 1;
            break;

            //Case of Set on Less Than
            case ALU_SLT:
                ALUOp = 2;
            break;

            //Case of Set on Less Than Unsigned
            case ALU_SLTU:
                ALUOp = 3;
            break;

            //Case of And
            case ALU_AND:
                ALUOp = 4;
            break;

            //Case of Or
            case ALU_OR:
                ALUOp = 5;
            break;

            //Case of SLL
            case ALU_SLL:
                ALUOp = 6;
            break;

            //HALT - Not Valid Signal
            default:
                return 1;
        }
    }

    //Handling extended value for src for BEQ
    if (ALUSrc == 1)
        data2 = extended_value;

    //Send data and instructions to ALU for processing
    ALU(data1, data2, ALUOp, ALUresult, Zero);

    //Final Return - No Halt
    return 0;
}

// Read / Write Memory 
// 10 Points    -------------------------------------------------------------------------

// 1. Use the value of MemWrite or MemRead to determine if a memory writeoperation or memory read operation or neither is occurring.
// 2. If reading from memory, read the content of the memory location addressed by ALUresult to memdata.
// 3. If writing to memory, write the value of data2 to the memory locationaddressed by ALUresult.
// 4. Return 1 if a halt condition occurs; otherwise, return 0.
int rw_memory(unsigned ALUresult, unsigned data2, char MemWrite, char MemRead, unsigned *memdata, unsigned *Mem) {

    //Check if Reading Memory
    if (MemRead == 1) {

        //Check if ALUresult is properly alligned (halt condition)
        if (ALUresult % 4 != 0) //Improper address - halt
            return 1;

        else //Proper address - do not halt
            *memdata = Mem[ALUresult >> 2];
    }

    //Check if Writing Memory
    if (MemWrite == 1) {

        //Check if ALUresult is properly alligned (halt condition)
        if (ALUresult % 4 != 0) //Improper address - halt
            return 1;

        else //Proper address - do not halt
            Mem[ALUresult >> 2] = data2;
    }

    //Final return - did not halt
    return 0;
}


// Write Register
// 10 Points    -------------------------------------------------------------------------

// 1. Write the data (ALUresult or memdata) to a register (Reg) addressed by r2 or r3.
void write_register(unsigned r2, unsigned r3, unsigned memdata, unsigned ALUresult, char RegWrite, char RegDst, char MemtoReg, unsigned *Reg) {

    //Ensure we are writing to Memory
    if (RegWrite == 1) {

        //Determining which data to send
        switch (MemtoReg) {

            //Case of Writing Result to Register
            case 0:

                //Determining r2 or r3 addressing
                    switch(RegDst) {

                        //Case of r2
                        case 0:
                            Reg[r2] = ALUresult;
                        break;

                        //Case of r3
                        case 1:
                            Reg[r3] = ALUresult;
                        break;
                    }

            break;

            //Case of Writing Memory to Register
            case 1:

                //Determining r2 or r3 addressing
                    switch(RegDst) {

                        //Case of r2
                        case 0:
                            Reg[r2] = memdata;
                        break;

                        //Case of r3
                        case 1:
                            Reg[r3] = memdata;
                        break;
                    }

            break;

        }

    }
}

// PC update
// 10 Points    -------------------------------------------------------------------------

// 1. Update the program counter (PC).
void PC_update(unsigned jsec, unsigned extended_value, char Branch, char Jump, char Zero, unsigned *PC) {

    //Incrementing by 4
    *PC += 4;

    //Jump Statement Handling
    if (Jump == 1)
        *PC = ((jsec << 2) | (*PC & 0xF0000000)); //Grabbing first 4 digits of PC 11110000000000000000000000000000

    //Branch Statement Handling - Adding extended value / 4 for addressing
    if (Branch == 1 && Zero == 1)
        *PC += extended_value << 2;
}
