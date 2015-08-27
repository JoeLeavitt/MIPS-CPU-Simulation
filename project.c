// Joseph Leavitt
// CDA 3103 Mips simulation project

#include "spimcore.h"

/* ALU */
void ALU(unsigned A,unsigned B,char ALUControl,unsigned *ALUresult,char *Zero)
{
    switch(ALUControl)
    {
        // 000: Add
        case 0:
            *ALUresult = (signed)A +(signed) B;
            break;
        // 001: Subtract
        case 1:
            *ALUresult = (signed)A - (signed)B;
            break;
        // 010: Set Less Than
        case 2:
            if ((signed)A < (signed)B)
                *ALUresult = 1;
            else
                *ALUresult = 0;
            break;
        // 011: Set Less Than Immediate
        case 3:
            if (A < B)
                *ALUresult = 1;
            else
                *ALUresult = 0;
            break;
        // 100: AND
        case 4:
            *ALUresult = A && B;
            break;
        // 101: OR
        case 5:
            *ALUresult = A || B;
            break;
        // 110: Shift Logical Left
        case 6:
            *ALUresult = B << 16;
            break;
       // 111: NOT
        case 7: // 111 not
            *ALUresult = !A;
            break;
    }
    
    if (*ALUresult == 0)
        *Zero = 1;
    else
        *Zero = 0;

}

/* instruction fetch */
int instruction_fetch(unsigned PC,unsigned *Mem,unsigned *instruction)
{
    // Check if PC is word aligned
    if (PC % 4 != 0)
        return 1;
    
    // Shift PC right by 2 to get proper index
    *instruction = Mem[PC >> 2];
    return 0;
}


/* instruction partition */
void instruction_partition(unsigned instruction, unsigned *op, unsigned *r1,unsigned *r2, unsigned *r3, unsigned *funct, unsigned *offset, unsigned *jsec)
{
    // Shift right to place the bits at the end of the string
    *op     = (instruction & 0xfc000000) >> 26;
    *r1     = (instruction & 0x03e00000) >> 21;
    *r2     = (instruction & 0x001f0000) >> 16;
    *r3     = (instruction & 0x0000f800) >> 11;
    // Place bits at the end of the string
    *funct  =  instruction & 0x0000003f;
    *offset =  instruction & 0x0000ffff;
    *jsec   =  instruction & 0x03ffffff;
}

/* instruction decode */
int instruction_decode(unsigned op,struct_controls *controls)
{
	/* Couldn't get this methodology to work */ /*
    // struct_controls->(RegDst, Jump, Branch, MemRead, MemtoReg, ALUop, MemWrite, ALUSrc, RegWrite)

	switch (op)
    {
        // R-type
        case 0x0:
            *controls = (struct_controls) {1, 0, 0, 0, 0, 7, 0, 0, 1};
            break;
        // Jump
        case 0x2:
            *controls = (struct_controls) {0, 1, 0, 0, 0, 0, 0, 0, 0};
            break;
        // Branch Equal
        case 0x4:
            *controls = (struct_controls) {2, 0, 1, 0, 2, 1, 0, 0, 0};
            break;
        // Add Immediate
        case 0x8:
            *controls = (struct_controls) {0, 0, 0, 0, 0, 0, 0, 1, 1};
            break;
        // Set Less Than Immediate
        case 0xA:
            *controls = (struct_controls) {0, 0, 0, 0, 0, 2, 0, 1, 1};
            break;
        // Set Less Than Unsigned Immediate
        case 0xB:
            *controls = (struct_controls) {0, 0, 0, 0, 0, 3, 0, 1, 1};
            break;
        // And Immediate
        case 0xC:
            *controls = (struct_controls) {0, 0, 0, 0, 0, 4, 0, 1, 1};
            break;
        // Or Immediate
        case 0xD:
            *controls = (struct_controls) {0, 0, 0, 0, 0, 5, 0, 1, 1};
            break;
        // Upper Immediate
        case 0xF:
            *controls = (struct_controls) {0, 0, 0, 0, 0, 6, 0, 1, 1};
            break;
        // Load Word
        case 0x23:
            *controls = (struct_controls) {0, 0, 0, 1, 1, 0, 0, 1, 1};
            break;
        // Store Word
        case 0x2B:
            *controls = (struct_controls) {2, 0, 0, 0, 2, 0, 1, 1, 0};
        // Halt
        default:
            return 1;
    }
                                                 
    return 0; */
    
    switch(op)
    {
        // R-Type
        case 0x0:
            controls->RegDst = 1;
            controls->Jump = 0;
            controls->Branch = 0;
            controls->MemRead = 0;
            controls->MemtoReg = 0;
            controls->ALUOp = 7;
            controls->MemWrite = 0;
            controls->ALUSrc = 0;
            controls->RegWrite = 1;
            break;
        // Jump
        case 0x2:
            controls->RegDst = 0;
            controls->Jump = 1;
            controls->Branch = 0;
            controls->MemRead = 0;
            controls->MemtoReg = 0;
            controls->ALUOp = 0;
            controls->MemWrite = 0;
            controls->ALUSrc = 0;
            controls->RegWrite = 0;
            break;
        // Branch Equal
        case 0x4:
            controls->RegDst = 2;
            controls->Jump = 0;
            controls->Branch = 1;
            controls->MemRead = 0;
            controls->MemtoReg = 2;
            controls->ALUOp = 1;
            controls->MemWrite = 0;
            controls->ALUSrc = 0;
            controls->RegWrite = 0;
            break;
        // Add Immediate
        case 0x8:
            controls->RegDst = 0;
            controls->Jump = 0;
            controls->Branch = 0;
            controls->MemRead = 0;
            controls->MemtoReg = 0;
            controls->ALUOp = 0;
            controls->MemWrite = 0;
            controls->ALUSrc = 1;
            controls->RegWrite = 1;
            break;
        // Set ess Than Immediate
        case 0xA:
            controls->RegDst = 0;
            controls->Jump = 0;
            controls->Branch = 0;
            controls->MemRead = 0;
            controls->MemtoReg = 0;
            controls->ALUOp = 2;
            controls->MemWrite = 0;
            controls->ALUSrc = 1;
            controls->RegWrite = 1;
            break;
        // Set Less Than Unsigned Immediate
        case 0xB:
            controls->RegDst = 0;
            controls->Jump = 0;
            controls->Branch = 0;
            controls->MemRead = 0;
            controls->MemtoReg = 0;
            controls->ALUOp = 3;
            controls->MemWrite = 0;
            controls->ALUSrc = 1;
            controls->RegWrite = 1;
            break;
        // And Immediate
        case 0xC:
            controls->RegDst = 0;
            controls->Jump = 0;
            controls->Branch = 0;
            controls->MemRead = 0;
            controls->MemtoReg = 0;
            controls->ALUOp = 4;
            controls->MemWrite = 0;
            controls->ALUSrc = 1;
            controls->RegWrite = 1;
            break;
        // Or Immediate
        case 0xD:
            controls->RegDst = 0;
            controls->Jump = 0;
            controls->Branch = 0;
            controls->MemRead = 0;
            controls->MemtoReg = 0;
            controls->ALUOp = 5;
            controls->MemWrite = 0;
            controls->ALUSrc = 1;
            controls->RegWrite = 1;
            break;
        // Load Upper Immediate
        case 0xF:
            controls->RegDst = 0;
            controls->Jump = 0;
            controls->Branch = 0;
            controls->MemRead = 0;
            controls->MemtoReg = 0;
            controls->ALUOp = 6;
            controls->MemWrite = 0;
            controls->ALUSrc = 1;
            controls->RegWrite = 1;
            break;
        // Load Word
        case 0x23:
            controls->RegDst = 0;
            controls->Jump = 0;
            controls->Branch = 0;
            controls->MemRead = 1;
            controls->MemtoReg = 1;
            controls->ALUOp = 0;
            controls->MemWrite = 0;
            controls->ALUSrc = 1;
            controls->RegWrite = 1;
            break;
        // Store Word
        case 0x2B:
            controls->RegDst = 2;
            controls->Jump = 0;
            controls->Branch = 0;
            controls->MemRead = 0;
            controls->MemtoReg = 2;
            controls->ALUOp = 0;
            controls->MemWrite = 1;
            controls->ALUSrc = 1;
            controls->RegWrite = 0;
            break;
        // Halt
        default:
            return 1;
    }
    
    return 0;
}

/* Read Register */
void read_register(unsigned r1,unsigned r2,unsigned *Reg,unsigned *data1,unsigned *data2)
{
	// Set data1 and data2 equal to their respective register values
	*data1 = Reg[r1];
	*data2 = Reg[r2];
}

/* Sign Extend */
void sign_extend(unsigned offset,unsigned *extended_value)
{
	if(offset >> 15 == 1)
    {
		// Extend offset such that the value is negative
		*extended_value = offset | 0xffff0000;
    }
	else
    {
		// Extend offset such that the value is positive
		*extended_value = offset & 0x0000ffff;
    }
}

/* ALU operations */
int ALU_operations(unsigned data1,unsigned data2,unsigned extended_value,unsigned funct,char ALUOp,char ALUSrc,unsigned *ALUresult,char *Zero)
{
    // Send data2 or extended value to ALU
    if (ALUSrc == 1)
        data2 = extended_value;
    
    // Temp variable as a place-holder
    unsigned ALUControl;
    
    // If the ALU operation is R type
    if (ALUOp == 7)
    {
        switch(funct)
        {
            // Add
            case 32:
                ALUControl = 0b000;
                break;
            // Subtract
            case 34:
                ALUControl = 0b001;
                break;
            // Set Less Than
            case 42:
                ALUControl = 0b010;
                break;
            // Set Less Than Unsigned
            case 43:
                ALUControl = 0b011;
                break;
            // AND
            case 36:
                ALUControl = 0b100;
                break;
            // OR
            case 37:
                ALUControl = 0b101;
                break;
            // Shift Left Extended Value
            case 38:
                ALUControl = 0b110;
                break;
            // NOT
            case 39:
                ALUControl = 0b111;
                break;
            // Halt
            default:
                return 1;
        }
    }
    else
        ALUControl = ALUOp;
    
    // Send info the ALU
    ALU(data1, data2, ALUControl, ALUresult, Zero);
    
    return 0;
}

/* Read / Write Memory */
int rw_memory(unsigned ALUresult,unsigned data2,char MemWrite,char MemRead,unsigned *memdata,unsigned *Mem)
{
    // Check if ALU result is world aligned
    if((MemWrite || MemRead) && ALUresult % 4 != 0)
        return 1;
    
    // Write then read case
    if(MemWrite == 1)
        Mem[ALUresult >> 2] = data2;
    else if(MemRead == 1)
        *memdata = Mem[ALUresult >> 2];
    
    return 0;
}


/* Write Register */
void write_register(unsigned r2,unsigned r3,unsigned memdata,unsigned ALUresult,char RegWrite,char RegDst,char MemtoReg,unsigned *Reg)
{
    // Set the register to the correct source
    if (RegWrite == 1)
    {
        if (MemtoReg == 1)
        {
            if (RegDst == 1)
                Reg[r3] = memdata;
            else
                Reg[r2] = memdata;
        }
        else
        {
            if (RegDst == 1)
                Reg[r3] = ALUresult;
            else
                Reg[r2] = ALUresult;
        }
    }
}

/* PC update */
void PC_update(unsigned jsec,unsigned extended_value,char Branch,char Jump,char Zero,unsigned *PC)
{
    // Get next instruction
    *PC += 4;
    
    // Add PC to the extended value
    if(Zero == 1 && Branch == 1)
        *PC += extended_value << 2;
    
    // Add PC with jsec
    if(Jump == 1)
        *PC = (*PC & 0xf0000000) | jsec << 2;
}

