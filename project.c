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

//#define ALU_ADD = 0;
//#define ALU_MINUS = 1;
//#define ALU_NOT = 7;
//#define ALU_SHIFT_LEFT_SIXTEEN = 6;
//#define ALU_OR  = 5;
//#define ALU_AND  = 4;

int c =0;

/* ALU */
/* 10 Points */

// 1. Implement the operations on input parameters A and B according to ALUControl.
// 2. Output the result (Z) to ALUresult.
// 3. Assign Zero to 1 if the result is zero; otherwise, assign 0.
// 4. The following table shows the operations of the ALU.
void ALU(unsigned A,unsigned B,char ALUControl,unsigned *ALUresult,char *Zero)
{

}


/* instruction fetch */
// HEX 21080001
// 001000 01000010000000000000000001

/* 10 Points */
// 1. Fetch the instruction addressed by PC from Mem and write it to instruction.
// 2. Return 1 if a halt condition occurs; otherwise, return 0.
int instruction_fetch(unsigned PC,unsigned *Mem,unsigned *instruction) {



//FIGURE OUT A WAY TO DETECT BAD INSTRUCTIONS!!!
//    if (Mem[ (PC >> 2) ] % 2 != 0) {
//        printf("Bad instruction");
//        return 1;//WORD NOT ALIGNED
//    }

//    printf("FIRST PC: %d\n", PC);
//    //16384 is 0x4000
//    int arrayIndex = PC >
//    printf("arrayIndex: %d\n", arrayIndex);
//> 2; //Get index
//    printf("Decimal memory value: %d\n", Mem[ (PC >> 2) ]);
    *instruction = Mem[ (PC >> 2) ]; //Load instructions

    int t = 3735928495%4;
    //100001
    return 0; //DONT HALT


}


/* instruction partition */
/* 10 Points */
// Partition instruction into several parts (op, r1, r2, r3, funct, offset, jsec).
void instruction_partition(unsigned instruction, unsigned *op, unsigned *r1,unsigned *r2, unsigned *r3, unsigned *funct, unsigned *offset, unsigned *jsec)
{
//    printf("INSTRUCTION: %d\n", instruction);
    //111111 = 63
    int decOpCode  = (0xFC000000 & instruction ) >> 26;
    *op = decOpCode;

    /*
     * RS: Source Register
RT: Target Register
RD: Destination Register
shamt:  Shift amount
func: Function code
     */
    if (decOpCode== 0) {
        //R-Type Since OP CODE is 0 {
        int rs =   (0x3E00000 & instruction ) >> 21; //Performing bit masking and shifting 21 bits
        int rt =   (0x1F0000 & instruction ) >> 16; //Performing bit masking and shifting 16 bits
        //             OP CODE RS     RT   RD   SHAMT  FUNCT
        //BASE MASKING 000000 00000 00000 00000 00000 000000
        //             000000 00000 00000 00000 00000 111111

        //add $10, $8, $9		r10 = r8 + r9 = 4
        //SHIFT BY 11111
        int rd = (0xF800 & instruction) >> 11;
        int shamt = (0x7C0  & instruction) >> 6;
        int func = (0x3F  & instruction);
//0x7C0 SHMT
        *r1 = rs;
        *r2 = rt;
        *r3 = rd;
        *funct = func;
        *offset = shamt;//TODO: CHANGE?


    } else if (decOpCode == 3 || decOpCode==2){
        //J-Type
        *jsec = decOpCode<<6;//UNSURE IF THIS WORKS



    } else {
        //REMAINING I-Type


        //00000011111000000000000000000000


        //CREDIT: https://www.rapidtables.com/convert/number/hex-to-binary.html
        int rs =   (0x3E00000 & instruction ) >> 21; //Performing bit masking and shifting 21 bits
        int rt =   (0x1F0000&instruction ) >> 16; //Performing bit masking and shifting 16 bits
        int immediate =   (0xFFFF&instruction ); //Performing bit masking s

        *r1 = rs;
        *r2 = rt;
        *offset = immediate;

//        printf("RS: %d\n", rs);
//        printf("RT: %d\n", rt);
//        printf("Immediate: %d\n", immediate);
//
//        if (decOpCode==35||decOpCode==43||decOpCode==4||decOpCode==5) {
//            //OFFSET
//            *offset = immediate;
//        } else {
//            // immediate
//        }
    }




}


/* instruction decode */
/* 15 Points */
// 1. Decode the instruction using the opcode (op).
// 2. Assign the values of the control signals to the variables in the structure controls

/*	char RegDst;
	char Jump;
	char Branch;
	char MemRead;
	char MemtoReg;
	char ALUOp;
	char MemWrite;
	char ALUSrc;
	char RegWrite;
 */

void printALU(   struct_controls *controls) {
    printf("ALUOp: %d\n", controls->ALUOp);
    printf("Jump: %d\n",controls->Jump);
    printf("Branch: %d\n",controls->Branch);
    printf("MemRead: %d\n",controls->MemRead);
    printf("MemWrite: %d\n",controls->MemWrite);
    printf("MemtoReg: %d\n",controls->MemtoReg);
    printf("RegDst: %d\n",controls->RegDst);
    printf("RegWrite: %d\n",controls->RegWrite);
    printf("ALUSrc: %d\n",controls->ALUSrc);
}
int instruction_decode(unsigned op,struct_controls *controls) {

    switch(op) {

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

    case J: //Op-code: 000 010
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

    /*
     * ALU OP FIRST
     */
    if (op==0) {
        controls->ALUOp = 2;

        used  = 1;
    }


    if (op == BEQ||op == BNE ) {
        controls->ALUOp = 1;
        used  = 1;
    }
    if (op == ANDI||op ==ORI || op == LUI) {
        controls->ALUOp = 3;
        used  = 1;
    }

    /*
     * Reg Dest
     */
    if (op==0) { // type r
        controls->RegDst= 1;
        used  = 1;
    }

    /*
     * ALU SOURCE
     */

    //R TYPE OR ANDI OR ORI OR LUI OR LW OR SW
    if (op==ANDI ||op==ORI||op==ADDI||op==LUI||op==LW||op==SW){
        controls->ALUSrc = 1;
        used  = 1;
    }

    /*
     * MEM TO REG
     */
    if (op == LW) {
        controls->MemtoReg=1;
        used  = 1;
    }

    /*
     * Reg write
     */
    if (op==0) {//TYPE R

        controls->RegWrite= 1;
        used  = 1;
    }
    if (op==ANDI ||op==ORI||op==ADDI||op==LUI||op==LW) {
        controls->RegWrite= 1;
        used  = 1;
    }



    /*
     * MEM READ
     */
    if (op == LW) {
        controls->MemRead=1;
        used  = 1;
    }
    /*
     * MEM Write
     */
    if (op == SW) {
        used  = 1;
        controls->MemWrite=1;
    }
    /*
     * Branches
     */
    if (op == BEQ||op == BNE ) {
        controls->Branch = 1;
        used  = 1;
    }


//    printALU(controls);
    return !used;


}

/* Read Register */
/* 5 Points */
// Read the registers addressed by r1 and r2 from Reg, and write the read values to data1 and data2 respectively
void read_register(unsigned r1,unsigned r2,unsigned *Reg,unsigned *data1,unsigned *data2)
{

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
void sign_extend(unsigned offset,unsigned *extended_value)
{


}

/* ALU operations */
/* 10 Points */
int ALU_operations(unsigned data1,unsigned data2,unsigned extended_value,unsigned funct,char ALUOp,char ALUSrc,unsigned *ALUresult,char *Zero)
{
    return 0;

}

/* Read / Write Memory */
/* 10 Points */
int rw_memory(unsigned ALUresult,unsigned data2,char MemWrite,char MemRead,unsigned *memdata,unsigned *Mem)
{
    return 0;

}


/* Write Register */
/* 10 Points */
void write_register(unsigned r2,unsigned r3,unsigned memdata,unsigned ALUresult,char RegWrite,char RegDst,char MemtoReg,unsigned *Reg)
{

}

/* PC update */
/* 10 Points */
void PC_update(unsigned jsec,unsigned extended_value,char Branch,char Jump,char Zero,unsigned *PC)
{

}
