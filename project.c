#include "spimcore.h"


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
int instruction_decode(unsigned op,struct_controls *controls)
{

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

