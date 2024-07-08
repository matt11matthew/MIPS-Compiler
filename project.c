#include "spimcore.h"
// J-Type Instructions
#define J 2        // Jump
#define JAL 3      // Jump and link

// I-Type Instructions
#define SW 43      // Store word
#define LW 35      // Load word
#define BNE 5      // Branch if not equal
#define BEQ 4      // Branch if equal
#define ANDI 12    // AND immediate
#define ADDI 8     // Add immediate

#define SLTI 10    // Set on less than immediate
#define ORI 13     // OR immediate
#define LUI 15     // OR immediate

//#define ALU_ADD = 0;
//#define ALU_MINUS = 1;
//#define ALU_NOT = 7;
//#define ALU_SHIFT_LEFT_SIXTEEN = 6;
//#define ALU_OR  = 5;
//#define ALU_AND  = 4;



/* ALU */
/* 10 Points */
void ALU(unsigned A,unsigned B,char ALUControl,unsigned *ALUresult,char *Zero)
{

}

/* instruction fetch */
// HEX 21080001
// 001000 01000010000000000000000001


int c =0;

/* 10 Points */


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
void instruction_partition(unsigned instruction, unsigned *op, unsigned *r1,unsigned *r2, unsigned *r3, unsigned *funct, unsigned *offset, unsigned *jsec)
{
//    printf("INSTRUCTION: %d\n", instruction);
    //111111 = 63
    int decOpCode  = (0xFC000000 & instruction ) >> 26;
    *op = decOpCode;

    if (decOpCode== 0) {
        //R-Type Since OP CODE is 0 {
        int rs =   (0x3E00000 & instruction ) >> 21; //Performing bit masking and shifting 21 bits
        int rt =   (0x1F0000 & instruction ) >> 16; //Performing bit masking and shifting 16 bits
        //             OP CODE RS     RT   RD   SHAMT  FUNCT
        //BASE MASKING 000000 00000 00000 00000 00000
        // 000000 00000 00000 00000 11111000000

        //SHIFT BY 11111
        int rd = (0x7C0 & instruction) >> 11;
        printf("R TYPE ");
        printf("RS: %d\n", rs);
        printf("RT: %d\n", rt);
        printf("RD: %d\n", rd);
//        int rd = (MASK_VALUE & instruction) >> 11;
        //


//        printf

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
    controls->ALUOp = 0;
    controls->Jump = 0;
    controls->Branch = 0;
    controls->MemRead = 0;
    controls->MemWrite = 0;
    controls->MemtoReg = 0;
    controls->RegDst = 0;
    controls->RegWrite = 0;
    controls->ALUSrc = 0;

    int used = 0;

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
void read_register(unsigned r1,unsigned r2,unsigned *Reg,unsigned *data1,unsigned *data2)
{

}


/* Sign Extend */
/* 10 Points */
void sign_extend(unsigned offset,unsigned *extended_value)
{

}

/* ALU operations */
/* 10 Points */
int ALU_operations(unsigned data1,unsigned data2,unsigned extended_value,unsigned funct,char ALUOp,char ALUSrc,unsigned *ALUresult,char *Zero)
{

}

/* Read / Write Memory */
/* 10 Points */
int rw_memory(unsigned ALUresult,unsigned data2,char MemWrite,char MemRead,unsigned *memdata,unsigned *Mem)
{

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
