#include <iostream>
#include <fstream>
#include "pin.H"

#if defined(TARGET_MAC)
#define MALLOC "_malloc"
#define FREE "_free"
#else
#define MALLOC "malloc"
#define FREE "free"
#endif
#define DUMMY_FIXED_PIM_B "_dummy_fixed_pim_begin"
#define DUMMY_FIXED_PIM_E "_dummy_fixed_pim_end"
#define DUMMY_GEN_PIM_B "_dummy_gen_pim_begin"
#define DUMMY_GEN_PIM_E "_dummy_gen_pim_end"

ofstream OutFile;

// The running count of instructions is kept here
// make it static to help the compiler optimize docount
static UINT64 icount = 0;

bool fixed_pim_flag = false;
bool gen_pim_flag = false;
int reg_mem_ref = 0;
int reg_comp_ref = 0;
int fixed_pim_count = 0;
int gen_pim_mem_ref = 0;
int gen_pim_comp_ref = 0;

// This function is called before every instruction is executed
VOID docount() { 

icount++; 
  if(fixed_pim_flag)
  {
	fixed_pim_count++;
  }
  else if(gen_pim_flag)	
  {
	if (gen_pim_mem_ref > 0) 
          gen_pim_mem_ref++;
	else
	    gen_pim_comp_ref++;
  }
  else
  {
	if (reg_mem_ref>0)
        reg_mem_ref++;
	else 
		reg_comp_ref++;
  }
}
    

VOID fixed_pim_flag_set1(bool fixed_pim_flag)
{
    fixed_pim_flag = true;
	std::cout<<"destn1"<<endl;
}

VOID fixed_pim_flag_set2(bool fixed_pim_flag)
{
    fixed_pim_flag = false;
	std::cout<<"destn2"<<endl;
}

VOID gen_pim_flag_set1(bool gen_pim_flag)
{
    gen_pim_flag = true;
	std::cout<<"destn3"<<endl;
}

VOID gen_pim_flag_set2(bool gen_pim_flag)
{
    gen_pim_flag = false;
	std::cout<<"destn4"<<endl;
}

VOID Arg1Before(CHAR * name, ADDRINT size)
{
    OutFile << name << "(" << size << ")" << endl;
}

VOID MallocAfter(ADDRINT ret)
{
    OutFile << "  returns " << ret << endl;
}

VOID Image(IMG img, VOID *v)
{
    // Instrument the malloc() and free() functions.  Print the input argument
    // of each malloc() or free(), and the return value of malloc().
    //
    //  Find the malloc() function.
    RTN fixed_pim_b = RTN_FindByName(img, DUMMY_FIXED_PIM_B);
    if (RTN_Valid(fixed_pim_b))
    {
        RTN_Open(fixed_pim_b);

        // Instrument malloc() to print the input argument value and the return value.
        RTN_InsertCall(fixed_pim_b, IPOINT_BEFORE, (AFUNPTR)Arg1Before,
                       IARG_ADDRINT, fixed_pim_flag_set1,
                       IARG_FUNCARG_ENTRYPOINT_VALUE, 0,
                       IARG_END);
        
        RTN_Close(fixed_pim_b);
    }

 RTN fixed_pim_e = RTN_FindByName(img, DUMMY_FIXED_PIM_E);
    if (RTN_Valid(fixed_pim_e))
    {
        RTN_Open(fixed_pim_e);

        // Instrument malloc() to print the input argument value and the return value.
        RTN_InsertCall(fixed_pim_e, IPOINT_BEFORE, (AFUNPTR)Arg1Before,
                       IARG_ADDRINT, fixed_pim_flag_set2,
                       IARG_FUNCARG_ENTRYPOINT_VALUE, 0,
                       IARG_END);
        
        RTN_Close(fixed_pim_b);
    }



 RTN gen_pim_b = RTN_FindByName(img, DUMMY_GEN_PIM_B);
    if (RTN_Valid(gen_pim_b))
    {
        RTN_Open(gen_pim_b);

        // Instrument malloc() to print the input argument value and the return value.
        RTN_InsertCall(gen_pim_b , IPOINT_BEFORE, (AFUNPTR)Arg1Before,
                       IARG_ADDRINT, gen_pim_flag_set1,
                       IARG_FUNCARG_ENTRYPOINT_VALUE, 0,
                       IARG_END);
        
        RTN_Close(gen_pim_b);
    }

 RTN gen_pim_e = RTN_FindByName(img, DUMMY_GEN_PIM_E);
    if (RTN_Valid(gen_pim_e))
    {
        RTN_Open(gen_pim_e);

        // Instrument malloc() to print the input argument value and the return value.
        RTN_InsertCall(gen_pim_e, IPOINT_BEFORE, (AFUNPTR)Arg1Before,
                       IARG_ADDRINT, gen_pim_flag_set2,
                       IARG_FUNCARG_ENTRYPOINT_VALUE, 0,
                       IARG_END);
        
        RTN_Close(gen_pim_b);
    }
RTN freeRtn = RTN_FindByName(img, FREE);
    if (RTN_Valid(freeRtn))
    {
        RTN_Open(freeRtn);
        // Instrument free() to print the input argument value.
        RTN_InsertCall(freeRtn, IPOINT_BEFORE, (AFUNPTR)Arg1Before,
                       IARG_ADDRINT, FREE,
                       IARG_FUNCARG_ENTRYPOINT_VALUE, 0,
                       IARG_END);
        RTN_Close(freeRtn);
    }



}

// Pin calls this function every time a new instruction is encountered
VOID Instruction(INS ins, VOID *v)
{
    // Insert a call to docount before every instruction, no arguments are passed
    INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)docount, IARG_END);
}

KNOB<string> KnobOutputFile(KNOB_MODE_WRITEONCE, "pintool",
    "o", "final.out", "specify output file name");

// This function is called when the application exits
VOID Fini(INT32 code, VOID *v)
{
    // Write to a file since cout and cerr maybe closed by the application
    OutFile.setf(ios::showbase);
    OutFile << " Count " << icount<<" reg_mem_ref= " << reg_mem_ref <<" reg_comp_ref= "<<reg_comp_ref<< " fixed_pim_count= "<<fixed_pim_count<<" gen_pim_mem_ref= "<<gen_pim_mem_ref<<" gen_pim_comp_ref= "<<gen_pim_comp_ref<<endl;
	OutFile.close();
} 

/* ===================================================================== */
/* Print Help Message                                                    */
/* ===================================================================== */

INT32 Usage()
{
    cerr << "This tool counts the number of dynamic instructions executed" << endl;
    cerr << endl << KNOB_BASE::StringKnobSummary() << endl;
    return -1;
}

/* ===================================================================== */
/* Main                                                                  */
/* ===================================================================== */
/*   argc, argv are the entire command line: pin -t <toolname> -- ...    */
/* ===================================================================== */

int main(int argc, char * argv[])
{
    // Initialize pin
    if (PIN_Init(argc, argv)) return Usage();

    OutFile.open(KnobOutputFile.Value().c_str());

    // Register Instruction to be called to instrument instructions
    INS_AddInstrumentFunction(Instruction, 0);

    // Register Fini to be called when the application exits
    PIN_AddFiniFunction(Fini, 0);
    
    // Start the program, never returns
    PIN_StartProgram();
    
    return 0;
}
