/*
Name: Kevin Libdan
Student Number: 821-420-189
Course: CENG 356
Professor: Maryam DavoudPour
*/

#include <stdio.h>
#include <stdbool.h>

#define CPU_MEM 0x4000 //(16384) memory  

#define MEMORY_MAX 0x3E80 //Defines maximum memory size (16000)
#define MAX32 0xFFFFFFFF //(4,294,967,295)32-bit register
#define TOTAL_REGS 0x10 //16 registers

//regs[13]-SP(Stack Pointer), regs[14]-LR(Link Register), regs[15]-PC(Program Counter)
long int registers[TOTAL_REGS];
int signFlag = 0;
int zeroFlag = 0;
int carryFlag = 0;
long int MBR = 0; //Memory Buffer Register (32-bit)
long int MAR = 0; //Memory Address Register (32-bit)
short int IR0 = 0; //Instruction Register (16-bit)
short int IR1 = 0; //Instruction Register (16-bit)
int stopFlag = 0;

int main(void)
{
	char choice;
	bool cont = true; 
	unsigned int bytesRead;
	unsigned int length, offset, ModAddress;
	unsigned char memory[CPU_MEM];

	printf("\n\n\t\t\tName: Kevin Libdan\n\n");
	printf("\t\td - dump memory\n");
	printf("\t\tg - go - run the entire program\n");
	printf("\t\tl - load a file into memory\n");
	printf("\t\tm - memory modify\n");
	printf("\t\tq - quit\n");
	printf("\t\tr - display registers\n");
	printf("\t\tt - trace - execute one instruction\n");
	printf("\t\tw - write file\n");
	printf("\t\tz - reset all registers to zero\n");
	printf("\t\t?, h - display list of commands\n");

	while(cont == true){

	printf("\nEnter an option: ");
	scanf("%c", &choice);
	
		switch(choice)
		{
			case 'l':
			case 'L':
				bytesRead = LoadFile(memory, CPU_MEM);
			break;

			case 'w':
			case 'W':
				WriteFile(memory);
			break;

			case 'q':
			case 'Q':
				printf("Quitting\n");
				cont = false;
			break;

			case 'd':
			case 'D':
				printf("Enter the length of bytes to be displayed in HEX:");
				scanf("%x", &length);

				printf("Enter the offset which specifies the part to be displayed in HEX:");
				scanf("%x", &offset);

				MemDump(memory, offset, length);
			break;

			case 'm':
			case 'M':
				printf("Enter an offset to start modifying in HEX: ");
				scanf("%x", &ModAddress);

				MemModify(memory, ModAddress);
			break;

			case 'g':
			case 'G':
				go(memory);
			break;

			case 'r':
			case 'R':
				displayRegisters();
			break;

			case 'z':
			case 'Z':
				zeroRegisters();
				printf("Registers have been reset\n");
			break;

			case 'h':
			case 'H':
			case '?':
				printf("\t\t\t****Help Menu****\n");
				printf("\t\tEnter 'l' or 'L' to load the file into memory\n");
				printf("\t\tEnter 'w' or 'W' to write file\n");
				printf("\t\tEnter 'd' or 'D' to perform a memory dump\n");
				printf("\t\tEnter 'm' or 'M' to modify memory\n");
				printf("\t\tEnter 'g' or 'G' to run the entire program\n");
				printf("\t\tEnter 'r' or 'R' to display registers\n");
				printf("\t\tEnter 'z' or 'Z' to reset registers\n");
				printf("\t\tEnter 'q' or 'Q' to quit\n");
			break;
			
			default:
				printf("'%c' is not a valid option\n", choice);
			break;	
		}
		while(choice != '\n')
		{
			scanf("%c", &choice);
		}
	}
	return 0;
}

/*Memory Dump function*/
void MemDump(void *memptr, unsigned offset, unsigned length)
{
	char * p = memptr;
	int rows;
	int i, y, z;

	unsigned int OFFSET;
	unsigned int LENGTH;
	int remainder;

	OFFSET = ("%d", offset);
	LENGTH = ("%d", length);

	//Checks whether the length and offset is greater than the memory 
	if((LENGTH + OFFSET) > *p)
	{
		printf("\nThe length and offset is out of bounds");
	}
	else{
		//Checks how many rows to display
		//takes the length entered by the user and modulus the length by 16	
		remainder = LENGTH % 16;
		if(remainder == 0)//If the remainder is equal to zero divide the length by 16
		{
			rows = LENGTH / 16;
		}
		else
		{
			rows = (LENGTH / 16) + 1;//If the remainder does not equal to 0 
		}

		for(i = 0; i < rows; i++)//loops through each row
		{
			//print the offset at the start of the row
			printf("%04x\t", OFFSET + (i * 16));

			//print all 16 ascii columns for row 
			for(y = 0; y < 16; y++)
			{
				if((y + (i * 16)) < LENGTH)
				{
					//isprint checks if the character is printable
					if(isprint(((char*)p)[OFFSET + y + (i * 16)]))
					{
						printf("%c\t", p[OFFSET + y + (i * 16)]);
					}
					else
					{
						printf(".\t");//displays "." if the character is not printable
					}

				}
			}
			printf("\n\t");

			//print all 16 hex columns for row
			for( z = 0; z < 16; z++)
			{
				if((z + (i * 16)) < LENGTH)
				{
					if((z + (i * 16)) < LENGTH)
					{
						printf("%02x\t", 0xFF & ((char*)p)[OFFSET + z + (i * 16)]);
					}
				}
			}
			printf("\n\n");
		}
	}	 
}

/*Memory Modify function*/
void MemModify(void *memptr, unsigned MemAddrLocation)
{
	char *memoryPTR = memptr;
	char buffer[256];
	int newHexVal;
	unsigned char exit = '.';

	while(1)
	{
		printf("\nAddress location 0x%02x contains the HEX value: 0x%x", MemAddrLocation, memoryPTR[MemAddrLocation]);
		printf("\nEnter a new HEX value to replace the value at address 0x%02x: ");
		scanf("%x", &newHexVal);

		//gets the input entered by the user and stores it into buffer
		fgets(buffer, sizeof(buffer), stdin);
			
		//if the value of the buffer is equal to '.' exit the loop
		if(buffer[0] == exit)
			break;

		//replace the value at the memory location with the new value entered by the user
		(((char*)memoryPTR)[MemAddrLocation]) = newHexVal; 
		
		//Increment the address location
		++MemAddrLocation;
	}
}

//LoadFile Function
int LoadFile(void * buffer, unsigned int max)
{
	FILE *fp;
	char filename[24];
	int count, fileSize;

	printf("Enter the filename you want to load: ");
	scanf("%s", &filename);

	fp = fopen(filename, "r");//opens the given filename for reading

	if(fp != NULL)
	{
		fseek(fp, 0, SEEK_END);
		fileSize = ftell(fp);
		fseek(fp, 0, SEEK_SET);		

		count = fread(buffer, 1, max, fp);
		printf("\nThe number of bytes read from the file '%s' is %d bytes (0x%.4x in hex)\n", filename, fileSize, fileSize);
		fclose(fp);
	}
	else
	{
		printf("\nThe file '%s' does not exist", filename);
		return(-1);
	}
	return count;
}

//WriteFile function
void WriteFile(void * memory)
{
	char filename[24];
	unsigned int fileSize;
	int i;

	printf("Enter a filename to write: ");
	scanf("%s", &filename);

	printf("Enter the file size: ");
	scanf("%d", &fileSize);

	FILE *fp;
	//creates a file of the given filename for reading and writing
	fp = fopen(filename, "w+");

	//memory is the pointer to the array of elements to be written
	//1 - is the size in bytes of each element to be written
	//filesize - is the number of elements
	//fp - specifies the output stream
	fwrite(memory, 1, fileSize, fp);
	fclose(fp);
}

/*
iscarry() - determine if a carry is generated by addition: op1+op2+C
op1 - receives register 1
op2 - receives register 2
C can only have a value or 1 or 0
returns 1 or 0 depending if the carry is set
*/
int iscarry(unsigned long op1, unsigned long op2, unsigned C)
{
	if((op2 == MAX32) && (C == 1))
		return (1);
	return((op1 > (MAX32 - op2 - C))?1:0);	
}

/*
go() function
*/
void go(void *memptr)
{
	
	char input;
	int reg;
	char *memoryPTR = memptr;
	registers[15] = 0;
	int value, x;
	int y = 0;
	char svalue[100];
	stopFlag = 0;	
	int condBranch = 0;

	while(stopFlag == 0)
	{
		input = ((char*)memoryPTR[registers[15]]);

		if(input == 'x' || input == NULL || input == '\0')
		{
			stopFlag = 1;
		}

		//load
		if(input == 'l')
		{
			registers[15] += 2;
			reg=((int)((char*)memoryPTR[registers[15]]))-48; //converts char to int
			registers[15] += 2;

			x = 0;

			for(y = 0; y < 100; y++)
			{
				svalue[y] = '\0'; //clears the array of any values
			}

			//Checking if the next character is not a white space
			//The decimal 32 in ascii represents a white space
			while(((int)((char*)memoryPTR[registers[15]])) != 32)
			{
				svalue[x++] = ((char)((char*)memoryPTR[registers[15]++]));
			}
			value = atoi(svalue);
			loadRegister(value,reg);
		}
		
		/*
		Conditional Branch	
		*/
		condBranch = conditionalFlag();
		
		if((input == 'c') && (condBranch == 1))
		{
			registers[15] += 2;
			
			for(y = 0; y < 100; y++)
			{
				svalue[y] = '\0'; //clears the array of any values
			}
			x = 0;
			
			//Checking if the next character is not a white space
			while(((int)((char*)memoryPTR[registers[15]])) != 32)
			{
				svalue[x++] = ((char)((char*)memoryPTR[registers[15]++]));
			}
			
			value = atoi(svalue);
			registers[15] = value;
			
		}
		
		/*
		Unconditional Branch
		*/
		if(input == 'u')
		{
			registers[15] += 2;
			
			for(y = 0; y < 100; y++)
			{
				svalue[y] = '\0'; //clears the array of any values
			}
			
			x = 0;
			
			//Checking if the next character is not a white space
			while(((int)((char*)memoryPTR[registers[15]])) != 32)
			{
				svalue[x++] = ((char)((char*)memoryPTR[registers[15]++]));
			}
			
			value = atoi(svalue);
			registers[15] = value;
		}
		
		//add instruction
		if(input == 'a') //If input equals to 'a' add registers 0 and 1
		{
			addRegisters();
		}
		
		//sub instruction
		if(input == 's') //If input equals to 's' subtract registers 0 and 1
		{
			subRegisters();
		}
		
		registers[15]++;
	}
	displayRegisters();
}

/*conditionalFlag function
This function will check whether to execute the conditional branch or not
returns 1 when the value at register 1 is 100
*/
int conditionalFlag()
{
	if(registers[1] == 100)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

/*
loadRegister function (Load/store instruction)
value - takes in a value that will be stored at a register
location - the register in which a value is to be stored
*/
void loadRegister(long int value, int location)
{
	registers[location] = value;
}

/*
addRegisters() Function
Adds registers 0 and 1 and will place the result in register 0
If the result of the operation is equal to 0 - set zeroFlag to 1
If the result of the operation is less than 0 - set signFlag to 1
*/
void addRegisters()
{
	registers[0] += registers[1];

	carryFlag = iscarry(registers[0], registers[1], carryFlag);

	if(registers[0] == 0)
	{
		zeroFlag = 1;
	}
	else
	{
		zeroFlag = 0;
	} 
	
	if(registers[0] < 0)
	{
		signFlag = 1;
	}
	else
	{
		signFlag = 0;
	}
}

/*
subRegisters() function
Subtracts registers 0 and 1 and will place the result in register 0
If register 0 is equal to 0 then the zero flag will be set
If register 0 is less than 0 then the sign flag will be set
*/
void subRegisters()
{
	registers[0] -= registers[1];
	
	if(registers[0] == 0)
	{
		zeroFlag = 1;
	}
	else
	{
		zeroFlag = 0;
	}
	if(registers[0] < 0)
	{
		signFlag = 1;
	}
	else
	{
		signFlag = 0;
	}
}

/*
zeroRegisters() function
resets all the registers and flags to 0
*/
void zeroRegisters()
{
	int i;

	for(i = 0; i < sizeof(registers); i++)
	{
		registers[i] = 0; //set all registers(0-15) to 0 
	}
	signFlag = 0;
	zeroFlag = 0;
	carryFlag = 0;
	MBR = 0;
	MAR = 0;
	signFlag = 0;
	zeroFlag = 0;
	carryFlag = 0;
	stopFlag = 0;
	IR0 = 0;
	IR1 = 0;

}

/*
displayRegisters() function
Displays all registers and flags
*/
void displayRegisters()
{
	printf("R0\t\tR1\t\tR2\t\tR3\n");
	printf("%08d\t%08d\t%08d\t%08d\n",registers[0],registers[1],registers[2],registers[3]);

	printf("R4\t\tR5\t\tR6\t\tR7\n");
	printf("%08d\t%08d\t%08d\t%08d\n",registers[4],registers[5],registers[6],registers[7]);

	printf("R8\t\tR9\t\tR10\t\tR11\n");
	printf("%08d\t%08d\t%08d\t%08d\n",registers[8],registers[9],registers[10],registers[11]);

	printf("R12\t\tR13(SP)\t\tR14(LR)\t\tR15(PC)\n");
	printf("%08d\t%08d\t%08d\t%08d\n",registers[12],registers[13],registers[14],registers[15]);
	
	printf("\nMBR:%08d\t\tMAR:%08d", MBR, MAR);
	printf("\nsignFlag:%08d\tzeroFlag:%08d\tcarryFlag:%08d", signFlag, zeroFlag, carryFlag);
	printf("\nIR0:%08d\t\tIR1:%08d\n", IR0, IR1);	

}
