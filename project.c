#include "spimcore.h"

// J-Type Instructions

#define JUMP 2        // Jump

// I-Type Instructions

#define BEQ 4      // Branch if equal

#define ADDI 8     // Add immediate

#define SLTI 10    // Set on less than immediate

#define SLTIU 11   // Set on less than immediate unsigned

#define LUI 15     // OR immediate

#define LW 35      // Load word

#define SW 43      // Store word

//Funct Op-Codes - R-Type Instructions

#define ALU_ADD 32 //Addition

#define  ALU_SUB 34 //Subtraction

#define  ALU_AND 36 //And &&

#define  ALU_OR 37 //Or ||

#define  ALU_SLL 6 //Shift Left Extended

#define  ALU_SLT 42 //Set Less Than Signed
#define  ALU_SLTU 43 //Set Less Than Signed



/* ALU */
int isValidOpCode(int input) {
    switch (input) {
        case 0: // R-Type
        case ADDI:
        case SLTI:
        case SLTIU:
        case LUI:
        case JUMP:
        case BEQ:
        case LW:
        case SW:
            return 1;
    }
    return 0;
}

/* 10 Points */


// 1. Implement the operations on input parameters A and B according to ALUControl.

// 2. Output the result (Z) to ALUresult.

// 3. Assign Zero to 1 if the result is zero; otherwise, assign 0.

// 4. The following table shows the operations of the ALU.

void ALU(unsigned A, unsigned B, char ALUControl, unsigned *ALUresult, char *Zero) {

    *Zero = 0; // Based on instructions;

    switch (ALUControl) {
        case 0: //Addition
            *ALUresult = A + B;
            break;

        case 1: //Subtraction
            *ALUresult = A - B;
            break;

        case 2: // A < B, Z = 1
            if ((signed)A < (signed)B)    *ALUresult = 1;
            else *ALUresult = 0;
            break;

        case 3: // A < B, Z=; otherwise z = 0
            if (A < B)    *ALUresult = 1;
            else *ALUresult = 0;
            break;

        case 4: // Z = A AND B
            *ALUresult = A & B;
            break;

        case 5: //Z = A OR B
            *ALUresult = A | B;
            break;

        case 6: //Z = Shift B Left By 16 Bits
            *ALUresult = B << 16;
            break;

        case 7: //Z = NOT A
            *ALUresult = ~A;
            break;

    }
    if (*ALUresult <  1) {
        *Zero = 1;
    }
}



/* instruction fetch */

/* 10 Points */

// 1. Fetch the instruction addressed by PC from Mem and write it to instruction.

// 2. Return 1 if a halt condition occurs; otherwise, return 0.

int instruction_fetch(unsigned PC, unsigned *Mem, unsigned *instruction) {
    //FIGURE OUT A WAY TO DETECT BAD INSTRUCTIONS!!!

    int var = Mem[(PC >> 2)];

    int opCode = (0xFC000000 & var) >> 26;

    if (!isValidOpCode(opCode)) { //Checks for invalid instruction
        return 1;
    }
    *instruction = Mem[(PC >> 2)]; //Load instructions
    return 0; //DONT HALT
}

//CREDIT for bitwise operations: https://www.rapidtables.com/convert/number/hex-to-binary.html
/* instruction partition */
/* 10 Points */
// Partition instruction into several parts (op, r1, r2, r3, funct, offset, jsec).
void instruction_partition(unsigned instruction, unsigned *op, unsigned *r1, unsigned *r2, unsigned *r3,
                           unsigned *funct, unsigned *offset, unsigned *jsec) {
    //    printf("INSTRUCTION: %d\n", instruction);
    //111111 = 63
    int decOpCode = (0xFC000000 & instruction) >> 26;
    *op = decOpCode;

    /*
     * RS: Source Register
RT: Target Register
RD: Destination Register
shamt:  Shift amount
func: Function code
     */
    if (decOpCode == 0) {
        //R-Type Since OP CODE is 0 {
        int rs = (0x3E00000 & instruction) >> 21; //Performing bit masking and shifting 21 bits
        int rt = (0x1F0000 & instruction) >> 16; //Performing bit masking and shifting 16 bits`
        //             OP CODE RS     RT   RD   SHAMT  FUNCT
        //BASE MASKING 000000 00000 00000 00000 00000 010000
        //             000000 00000 00000 00000 00000 111111

        //add $10, $8, $9		r10 = r8 + r9 = 4
        //SHIFT BY 11111
        int rd = (0xF800 & instruction) >> 11;
        int shamt = (0x7C0 & instruction) >> 6;
        int func = (0x3F & instruction);
        //0x7C0 SHMT
        *r1 = rs;
        *r2 = rt;
        *r3 = rd;
        *funct = func;
        *offset = shamt; //TODO: CHANGE?
    } else if (decOpCode == 3 || decOpCode == 2) {
        //J-Type
        printf("JSEC TEST %d\n", decOpCode << 6);
 //10000000 JUMPING TO
        //000100 01010 01011 11111 11111111101 LABEL IT SHOULD GO TO
        *jsec = decOpCode << 6; //UNSURE IF THIS WORKS
    } else {
        //REMAINING I-Type

        int rs = (0x3E00000 & instruction) >> 21; //Performing bit masking and shifting 21 bits
        int rt = (0x1F0000 & instruction) >> 16; //Performing bit masking and shifting 16 bits
        int immediate = (0xFFFF & instruction); //Performing bit masking s

        *r1 = rs;
        *r2 = rt;
        *offset = immediate;
    }
}


/* instruction decode */
/* 15 Points */
// 1. Decode the instruction using the opcode (op).
// 2. Assign the values of the control signals to the variables in the structure controls

void printALU(struct_controls *controls) {
    printf("ALUOp: %d\n", controls->ALUOp);
    printf("Jump: %d\n", controls->Jump);
    printf("Branch: %d\n", controls->Branch);
    printf("MemRead: %d\n", controls->MemRead);
    printf("MemWrite: %d\n", controls->MemWrite);
    printf("MemtoReg: %d\n", controls->MemtoReg);
    printf("RegDst: %d\n", controls->RegDst);
    printf("RegWrite: %d\n", controls->RegWrite);
    printf("ALUSrc: %d\n", controls->ALUSrc);
}

int instruction_decode(unsigned op, struct_controls *controls) {
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

/* Read Register */
/* 5 Points */
// Read the registers addressed by r1 and r2 from Reg, and write the read values to data1 and data2 respectively
void read_register(unsigned r1, unsigned r2, unsigned *Reg, unsigned *data1, unsigned *data2) {
    *data1 = Reg[r1];
    *data2 = Reg[r2];
}

/*
 * Just read registers if we access reg index 5 we access reg 5, store data in first reg in data 1

second reg in data 2
 */


/* Sign Extend */
/* 10 Points */
// Assign the sign-extended value of offset to extended_value.
void sign_extend(unsigned offset, unsigned *extended_value) {
    //Check for negative offset value
    unsigned negative_check = offset >> 15; //collect negative bit

    //Handling negative
    if (negative_check == 1)
        *extended_value = offset | 0xFFFF0000; //Extending with all 1s

    else
        *extended_value = offset & 0x0000FFFF; //Extending with all 0s

    /*
     *Tested.
     */
}

/* ALU operations */
/* 10 Points */
int ALU_operations(unsigned data1, unsigned data2, unsigned extended_value, unsigned funct, char ALUOp, char ALUSrc,
                   unsigned *ALUresult, char *Zero) {
    //Handling extended value for src
    if (ALUSrc == 1)
        data2 = extended_value;

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

    //Send data and instructions to ALU for processing
    ALU(data1, data2, ALUOp, ALUresult, Zero);

    //Final Return
    return 0;
}

/* Read / Write Memory */
/* 10 Points */
int rw_memory(unsigned ALUresult, unsigned data2, char MemWrite, char MemRead, unsigned *memdata, unsigned *Mem) {

    //Check if Reading Memory
    if (MemRead == 1) {
        *memdata = Mem[ALUresult >> 2];
    }

    //Check if Writing Memory
    if (MemWrite == 1) {
        Mem[ALUresult >> 2] = data2;
    }

    return 0;
}


/* Write Register */
/* 10 Points */
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
/* PC update */
/* 10 Points */
void PC_update(unsigned jsec, unsigned extended_value, char Branch, char Jump, char Zero, unsigned *PC) {

    //Incrementing by 4
    *PC += 4;

    //Jump Statement Handling
    if (Jump == 1) {
        // printf("JUMP"); /
        //0000000000000000 0000001000000000
        *PC = ((jsec << 2) | (*PC & 0xF0000000))  ;
        // *PC = ((jsec << 2) | (*PC & 0xF0000000)) << 28;
    }
    if (Branch == 1 && Zero == 1) {

        *PC += extended_value << 2;
        printf("BRANCH");
    }
}
