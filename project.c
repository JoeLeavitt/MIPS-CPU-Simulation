#include "spimcore.h"


/* ALU */
/* 10 Points */
void ALU(unsigned A,unsigned B,char ALUControl,unsigned *ALUresult,char *Zero)
{
	if(ALUControl == '0')
	{
		*ALUresult = A + B;
	}
	else if(ALUControl == '1')
	{
		*ALUresult = A - B;
	}
	else if(ALUControl == '2')
	{
		if(A < B)
			*ALUresult = '1';
		else
			*ALUresult = '0';
	}
	else if(ALUControl == '3')
	{
		if((int)A < (int)B)
			*ALUresult = '1';
		else
			*ALUresult = '0';
	}
	else if(ALUControl == '4')
	{
		*ALUresult = A & B;
	}
	else if(ALUControl == '5')
	{
		*ALUresult = A | B;
	}
	else if(ALUControl == '6')
	{
		*ALUresult = B << 16;
	}
	
	if(*ALUresult == 0)
		*Zero = '1';
	else
		*Zero = '0';
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
	// Set data1 and data2 equal to their respective register values
	*data1 = Reg[r1];
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
	if(ALUSrc == 1)
		data2 = extended_value;
	
	if(ALUOp == 7)
	{
		if(funct == 32)
			ALUOp = 0;
		else if(funct == 34)
			ALUOp = 1;
		else if(funct == 42)
			ALUOp = 2;
		else if(funct == 43)
			ALUOp = 3;
		else if(funct == 36)
			ALUOp = 4;
		else if(funct == 37)
			ALUOp = 5;
		else if(funct == 6)
			ALUOp = 6
		else if(funct == 39)
			ALUOp = 7;
		else
			return 1;
		
		ALU(data1, data2, ALUOp, ALUresult, Zero);
	}
	else
	{
		ALU(data1, data2, ALUOp, ALUresult, Zero);
	}
	
	return 0;
}

/* Read / Write Memory */
/* 10 Points */
int rw_memory(unsigned ALUresult,unsigned data2,char MemWrite,char MemRead,unsigned *memdata,unsigned *Mem)
{
	if((ALUresult % 4) != 0 && (MemWrite == '1' || MemRead == '1'))
		return 1;
	if(ALUresult > 65536 && (MemWrite == '1' ||MemRead == '1'))
		return 1;
	
	ALUresult = ALUresult >> 2;
	
	if(MemWrite == '1')
		Mem[ALUresult] = data1;
	if(MemRead == '1')
		*memdata = Mem[ALUresult];
	
	return 0;		
}


/* Write Register */
/* 10 Points */
void write_register(unsigned r2,unsigned r3,unsigned memdata,unsigned ALUresult,char RegWrite,char RegDst,char MemtoReg,unsigned *Reg)
{
	if(RegWrite == '1')
	{
		if(MemtoReg == '0')
		{
			if(RegDst == '0')
				reg[r2] = ALUresult;
			else if(RegDst == '1')
				Reg[r3] = ALUresult;
		}
		else if(MemtoReg == '1')
		{
			if(RegDst == '0')
				Reg[r2] = memdata;
			else if(RegDst == '1')
				reg[r3] = memdata;
		}
	}
}

/* PC update */
/* 10 Points */
void PC_update(unsigned jsec,unsigned extended_value,char Branch,char Jump,char Zero,unsigned *PC)
{
	*PC += 4;
	
	if(Zero == 1 && Branch == 1)
		*PC = *PC + (extended_value << 2);
	
	if(jump == 1)
		*PC = (*PC & 0xf0000000) | (jsec << 2);
}

