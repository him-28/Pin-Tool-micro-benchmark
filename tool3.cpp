#include <iostream>
#include <fstream>
#include "pin.H"
#include <fstream>
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

static UINT64 icount = 0;

bool fixed_pim_flag = false;
bool gen_pim_flag = false;
UINT64 reg_mem_ref = 0;
UINT64 reg_comp_ref = 0;
UINT64 fixed_pim_count = 0;
UINT64 gen_pim_mem_ref = 0;
UINT64 gen_pim_comp_ref = 0;



VOID docount() { 

icount++; 

    
}
VOID FixPimCount()
{
    fixed_pim_count++;
}

VOID GenPimMemCount()
{
    gen_pim_mem_ref++;
}

VOID GenPimCoRef()
{
    gen_pim_comp_ref++;
}

VOID RegMemRef()
{
    reg_mem_ref++;
}

VOID RegCoRef()
{
    reg_comp_ref++;
}

VOID fixed_pim_flag_set1(bool fixed_pim_flag)
{
    fixed_pim_flag = true;
	
}

VOID fixed_pim_flag_set2(bool fixed_pim_flag)
{
    fixed_pim_flag = false;
	
}

VOID gen_pim_flag_set1(bool gen_pim_flag)
{
    gen_pim_flag = true;
	
}

VOID gen_pim_flag_set2(bool gen_pim_flag)
{
    gen_pim_flag = false;
	
}


VOID Image(IMG img, VOID *v)
{
    
    RTN fixed_pim_b = RTN_FindByName(img, DUMMY_FIXED_PIM_B);
    if (RTN_Valid(fixed_pim_b))
    {
        RTN_Open(fixed_pim_b);

        // Instrument malloc() to print the input argument value and the return value.
        RTN_InsertCall(fixed_pim_b, IPOINT_BEFORE, (AFUNPTR)FixPimCount,
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
        RTN_InsertCall(fixed_pim_e, IPOINT_BEFORE, (AFUNPTR) RegCoRef,
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
        RTN_InsertCall(gen_pim_b , IPOINT_BEFORE, (AFUNPTR)GenPimMemCount,
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
        RTN_InsertCall(gen_pim_e, IPOINT_BEFORE, (AFUNPTR)GenPimCoRef,
                       IARG_ADDRINT, gen_pim_flag_set2,
                       IARG_FUNCARG_ENTRYPOINT_VALUE, 0,
                       IARG_END);
        
        RTN_Close(gen_pim_b);
    }



} 




VOID Instruction(INS ins, VOID *v)
{

	 UINT32 memOperands = INS_MemoryOperandCount(ins);

      
   RTN rtn = INS_Rtn(ins);
    if ( !RTN_Valid(rtn) &&  fixed_pim_flag) 
    {
	  fixed_pim_count++; 
	//INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)FixPimCount, IARG_END);
       // icount++;//++insNoRtnDiscoveredCount;
        //INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)InsNativeCount, IARG_END);
    }
    else if (RTN_IsDynamic(rtn)&& gen_pim_flag)
    {
          for (UINT32 memOp = 0; memOp < memOperands; memOp++)
    {
        if ((INS_MemoryOperandIsRead(ins, memOp) || INS_MemoryOperandIsWritten(ins, memOp)) )
        {
           /* INS_InsertPredicatedCall(
                ins, IPOINT_BEFORE, (AFUNPTR)GenPimMemCount,
                IARG_INST_PTR,
                IARG_MEMORYOP_EA, memOp,
                IARG_END);*/
		 gen_pim_mem_ref++;
	 INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)GenPimMemCount, IARG_END);
        }
        
        else {
		gen_pim_comp_ref++;
	    //INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)GenPimCoRef, IARG_END);
}
    }
        
    }
    else
    {
        for (UINT32 memOp = 0; memOp < memOperands; memOp++)
    {
       if (INS_MemoryOperandIsRead(ins, memOp) || INS_MemoryOperandIsWritten(ins, memOp))
        {
            /*INS_InsertPredicatedCall(
                ins, IPOINT_BEFORE, (AFUNPTR)RegMemRef,
                IARG_INST_PTR,
                IARG_MEMORYOP_EA, memOp,
                IARG_END);*/
		reg_mem_ref++;
	 INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)RegMemRef, IARG_END);
        }
    else 
        {        reg_comp_ref++;
		// INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)RegCoRef, IARG_END); 
}
    }
}
	INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)docount, IARG_END);
	INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)FixPimCount, IARG_END);
	//INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)GenPimMemCount, IARG_END);
	INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)GenPimCoRef, IARG_END);
	//INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)RegMemRef, IARG_END);
	INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)RegCoRef, IARG_END);
}


// Pin calls this function every time a new instruction is encountered
/*VOID Instruction(INS ins, VOID *v)
{
    // Insert a call to docount before every instruction, no arguments are passed
    INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)docount, IARG_END);
}*/

KNOB<string> KnobOutputFile(KNOB_MODE_WRITEONCE, "pintool",
    "o", "f4.out", "specify output file name");

// This function is called when the application exits
VOID Fini(INT32 code, VOID *v)
{
    // Write to a file since cout and cerr maybe closed by the application
    OutFile.setf(ios::showbase);
   OutFile <<  "===============================================" << endl;
   OutFile <<  "reg_mem_ref= " << reg_mem_ref << endl;
   OutFile <<  "reg_comp_ref= "<<reg_comp_ref << endl;
   OutFile <<  "fixed_pim_count= "<<fixed_pim_count << endl;
   // *OutFile <<  "===============================================" << endl;
   OutFile <<  "gen_pim_mem_ref= "<<gen_pim_mem_ref << endl;
   OutFile <<  "gen_pim_comp_ref= "<<gen_pim_comp_ref << endl;
   OutFile <<  "Count: " << icount << endl;
   OutFile <<  "===============================================" << endl;	
   //OutFile << " Count " << icount<<" reg_mem_ref= " << reg_mem_ref <<" reg_comp_ref= "<<reg_comp_ref<< " fixed_pim_count= "<<fixed_pim_count<<" gen_pim_mem_ref= "<<gen_pim_mem_ref<<" gen_pim_comp_ref= "<<gen_pim_comp_ref<<endl;
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



