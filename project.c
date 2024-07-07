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



    if (PC % 2 != 0) {
        return 1;//WORD NOT ALIGNED
    }

//    printf("FIRST PC: %d\n", PC);
//    //16384 is 0x4000
//    printf("arrayIndex: %d\n", arrayIndex);
//
//    int arrayIndex = PC >> 2; //Get index
//    printf("Decimal memory value: %d\n", Mem[ (PC >> 2) ]);
    *instruction = Mem[ (PC >> 2) ]; //Load instructions

    //100001
    return 0; //DONT HALT


}


/* instruction partition */
/* 10 Points */
void instruction_partition(unsigned instruction, unsigned *op, unsigned *r1,unsigned *r2, unsigned *r3, unsigned *funct, unsigned *offset, unsigned *jsec)
{
//    printf("INSTRUCTION: %d\n", instruction);
    //111111 = 63
    int decOpCode  = (0xFC000000&instruction ) >> 26;
    *op = decOpCode;

    if (decOpCode== 0) {
        //R-Type Since OP CODE is 0 {
        int rs =   (0x3E00000&instruction ) >> 21; //Performing bit masking and shifting 21 bits
        int rt =   (0x1F0000&instruction ) >> 16; //Performing bit masking and shifting 16 bits

//        printf

    } else if (decOpCode == 3 || decOpCode==2){
        //J-Type
        *jsec = decOpCode<<6;//UNSURE IF THIS WORKS



    } else {
        //REMAINING I-Type

        //CREDIT: https://www.rapidtables.com/convert/number/hex-to-binary.html
        int rs =   (0x3E00000&instruction ) >> 21; //Performing bit masking and shifting 21 bits
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

int instruction_decode(unsigned op,struct_controls *controls) {
    controls->ALUOp = op;
    controls->Jump = 0;
    controls->Branch = 0;
    controls->MemRead = 0;
    controls->MemWrite = 0;
    controls->MemtoReg = 0;
    controls->RegDst = 0;
    controls->RegWrite = 0;


    controls->ALUSrc = 0;

    if (op == ADDI || op == ANDI || op == ORI || op==LW || op == SW) {
        controls->ALUSrc = 1;

    }

    if (op==0) //TYPE R
        controls->RegDst = 1;

    if (op ==BEQ || op == BNE ) {
        controls->Branch=1;
    }
    if (op == J||op==JAL) {
         controls->Jump = 1;
    }

    if (op == LW) {
        controls->MemRead = 1;
        controls->MemtoReg = 1;
    }

    if (op==SW) {
        controls->MemWrite = 1;
    }





//    if (op== 0) {
//        //Type R
//        controls->RegDst = 1;//Type R
//        controls->RegWrite = 1;//Type R
//
//
//    } else if (op == 3 || op==2) {
//        //J-Type
//    } else {
//        controls->ALUOp = op;
//        controls->RegDst = 1;//Type R
//        //TYPE-I
//
//
//    }
    return 1;


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

