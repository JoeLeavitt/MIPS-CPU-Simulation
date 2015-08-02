#include "spimcore.h"


/* ALU */
/* 10 Points */
void ALU(unsigned A,unsigned B,char ALUControl,unsigned *ALUresult,char *Zero)
{

}

/* instruction fetch */
/* 10 Points */
int instruction_fetch(unsigned PC,unsigned *Mem,unsigned *instruction)
{
	// if the address is in the proper format
	if(PC % 4 == 0)
		// Get the instruction 
		*instruction = Mem[PC >> 2];
		return 0;
	else
		return 1;
	
	// For my sanity
	return 0;
}


/* instruction partition */
/* 10 Points */
void instruction_partition(unsigned instruction, unsigned *op, unsigned *r1,unsigned *r2, unsigned *r3, unsigned *funct, unsigned *offset, unsigned *jsec)
{
	*op     = (instruction & 0xfc000000) >> 26; // 11111100 00000000 00000000 00000000  
	*r1     = (instruction & 0x03e00000) >> 21; // 00000011 11100000 00000000 00000000
	*r2     = (instruction & 0x001f0000) >> 16; // 00000000 00011111 00000000 00000000
	*r3     = (instruction & 0x0000f800) >> 11; // 00000000 00000000 11111000 00000000
	*funct  =  instruction & 0x0000003f; 		// 00000000 00000000 00000000 00111111
	*offset =  instruction & 0x0000ffff; 		// 00000000 00000000 11111111 11111111
	*jsec   =  instruction & 0x03ffffff; 		// 00000011 11111111 11111111 11111111
}

/* instruction decode */
/* 15 Points */
int instruction_decode(unsigned op,struct_controls *controls)
{
	// struct_controls->(RegDst, Jump, Branch, MemRead, MemtoReg, MemWrite, ALUSrc, RegWrite)
	
	// R Type
	if(op = 0x0)
	{
		*controls = (struct_controls){1, 0, 0, 0, 0, 7, 0, 0, 1};
		return 0;
	}
	// Add Immediate
	else if(op == 0x8)
	{
		*controls = (struct_controls){0, 0, 0, 0, 0, 0, 0, 1, 1};
		return 0;
	}
	// Load Word
	else if(op == 0x23)
	{
		*controls = (struct_controls){0, 0, 0, 1, 1, 0, 0, 1, 1};
		return 0;
	}
	// Store Word
	else if(op == 0x2b)
	{
		*controls = (struct_controls){0, 0, 0, 0, 0, 0, 1, 1, 0};
		return 0;
	}
	// Load Upper Immediate
	else if(op == 0xf)
	{
		*controls = (struct_controls){0, 0, 0, 0, 0, 6, 0, 1, 1};
		return 0;
	}
	// Branch on equal
	else if(op == 0x4)
	{
		*controls = (struct_controls){2, 0, 1, 0, 2, 1, 0, 2, 0};
		return 0;
	}
	// Set less than immediate
	else if(op == 0xa)
	{
		*controls = (struct_controls){0, 0, 0, 0, 0, 2, 0, 1, 1};
		return 0;
	}
	// Set less than immediate unsigned
	else if(op == 0xb)
	{
		*controls = (struct_controls){0, 0, 0, 0, 0, 3, 0, 1, 1};
		return 0;
	}
	// Jump
	else if(op == 0x2)
	{
		*controls = (struct_controls){2, 1, 2, 0, 2, 0, 0, 2, 0};
		return 0;
	}
	else
	{
		return 1;
	}
	
	return 0;
}

/* Read Register */
/* 5 Points */
void read_register(unsigned r1,unsigned r2,unsigned *Reg,unsigned *data1,unsigned *data2)
{
	*data1 =  Reg[r1];
	*data2 = Reg[r2];
}

/* Sign Extend */
/* 10 Points */
void sign_extend(unsigned offset,unsigned *extended_value)
{
	// Gets the LSB
	unsigned LSB = offset >> 15;
	
	// If LSB is 1 the offset is negative.
	if(LSB == 1)
		// Extend offset such that the value is negative
		*extended_value = offset | 0xffff0000;
	else
		// Extend offset such that the value is positive 
		*extended_value = offset & 0x0000ffff;
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

