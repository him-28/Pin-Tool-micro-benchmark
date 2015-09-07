#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <stddef.h>
#include "pin.H"
#include "portability.H"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

using namespace std;

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

KNOB<string> KnobOutputFile(KNOB_MODE_WRITEONCE, "pintool", "o", "buffer.out", "output file");
//KNOB<string> KnobOutputFile1(KNOB_MODE_WRITEONCE, "pintool", "o", "buffer1.out", "output file");

BUFFER_ID bufId;

/*
 * Thread specific data
 */
TLS_KEY mlog_key;

/*
 * Number of OS pages for the buffer
 */
#define NUM_BUF_PAGES 1024


/*
 * Record of memory references.  Rather than having two separate
 * buffers for reads and writes, we just use one struct that includes a
 * flag for type.
 */
struct MEMREF
{
    ADDRINT     pc;
    ADDRINT     ea;
    UINT32      size;
    BOOL        read;
};


/*
 * MLOG - thread specific data that is not handled by the buffering API.
 */
class MLOG
{
  public:
    MLOG(THREADID tid);
    ~MLOG();

    VOID DumpBufferToFile( struct MEMREF * reference, UINT64 numElements, THREADID tid );
private:
    ofstream _ofile;
  	
};


MLOG::MLOG(THREADID tid)
{
	//OutFile == _ofile;
	cout<<"Im going through this"<<endl;
    string filename = KnobOutputFile.Value() + "." + decstr(getpid_portable()) + "." + decstr(tid);
    
    _ofile.open(filename.c_str());
    
    if ( !_ofile )
    {
        cerr << "Error: could not open output file 1." << endl;
        exit(1);
    }
    
    //_ofile << hex;
}


MLOG::~MLOG()
{
  _ofile.close();
}


VOID MLOG::DumpBufferToFile( struct MEMREF * reference, UINT64 numElements, THREADID tid )
{
    for(UINT64 i=0; i<numElements; i++, reference++)
    {
        if (reference->ea != 0)
            _ofile << reference->pc << "   " << reference->ea << endl;
    }
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

VOID docount(INS ins) { 


icount++; 


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




/*if(fixed_pim_flag){ FixPimCount(); cout<<"\n";}
else if(gen_pim_flag){  for (UINT64 memOp = 0; memOp < memOperands; memOp++)
    {
        
	if (INS_MemoryOperandIsRead(ins, memOp) || INS_MemoryOperandIsWritten(ins, memOp))
        {
            INS_InsertPredicatedCall(
                ins, IPOINT_BEFORE, (AFUNPTR)GenPimMemCount,
                IARG_INST_PTR,
                IARG_MEMORYOP_EA, memOp,
                IARG_END);
		GenPimMemCount();cout<<"\n";
	// INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)GenPimMemCount, IARG_END);
        }
        
        else {
		GenPimCoRef();cout<<"\n";
	    //INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)GenPimCoRef, IARG_END);
}
    } }
else { 
for (UINT64 memOp = 0; memOp < memOperands; memOp++)
    {
       if (INS_MemoryOperandIsRead(ins, memOp) || INS_MemoryOperandIsWritten(ins, memOp))
        {
            INS_InsertPredicatedCall(
                ins, IPOINT_BEFORE, (AFUNPTR)RegMemRef,
                IARG_INST_PTR,
                IARG_MEMORYOP_EA, memOp,
                IARG_END);
		RegMemRef();cout<<"\n";
	 //INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)RegMemRef, IARG_END);
        }
    else 
        {        RegCoRef();cout<<"\n";
		// INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)RegCoRef, IARG_END); 
}
    } }  
*/

VOID Trace(TRACE trace, VOID *v)
{
    for(BBL bbl = TRACE_BblHead(trace); BBL_Valid(bbl); bbl=BBL_Next(bbl))
    {
        for(INS ins = BBL_InsHead(bbl); INS_Valid(ins); ins=INS_Next(ins))
        {
		if (!INS_IsStandardMemop(ins) && !INS_HasMemoryVector(ins))
            {
                // We don't know how to treat these instructions
                continue;
            }
	     if(fixed_pim_flag){
                                  FixPimCount(); 
				}
              else if(gen_pim_flag){ 
               UINT32 memoryOperands = INS_MemoryOperandCount(ins);

                 for (UINT32 memOp = 0; memOp < memoryOperands; memOp++)
                 {
                UINT32 refSize = INS_MemoryOperandSize(ins, memOp);

                // Note that if the operand is both read and written we log it once
                // for each.
                if (INS_MemoryOperandIsRead(ins, memOp))
                {
                     INS_InsertFillBuffer(ins, IPOINT_BEFORE, bufId,
                                         IARG_INST_PTR, offsetof(struct MEMREF, pc),
                                         IARG_MEMORYOP_EA, memOp, offsetof(struct MEMREF, ea),
                                         IARG_UINT32, refSize, offsetof(struct MEMREF, size), 
                                         IARG_BOOL, TRUE, offsetof(struct MEMREF, read),
                                         IARG_END); 
		 GenPimMemCount();
                }

                if (INS_MemoryOperandIsWritten(ins, memOp))
                {
                    INS_InsertFillBuffer(ins, IPOINT_BEFORE, bufId,
                                         IARG_INST_PTR, offsetof(struct MEMREF, pc),
                                         IARG_MEMORYOP_EA, memOp, offsetof(struct MEMREF, ea),
                                         IARG_UINT32, refSize, offsetof(struct MEMREF, size), 
                                         IARG_BOOL, FALSE, offsetof(struct MEMREF, read),
                                         IARG_END);
			 GenPimMemCount();
                }
		
		 else {
		GenPimCoRef();cout<<"\n";
	    //INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)GenPimCoRef, IARG_END);
                      } 
                 }
		}

	else { 
               UINT32 memoryOperands = INS_MemoryOperandCount(ins);

            for (UINT32 memOp = 0; memOp < memoryOperands; memOp++)
            {
                //UINT32 refSize = INS_MemoryOperandSize(ins, memOp);

                // Note that if the operand is both read and written we log it once
                // for each.
                if (INS_MemoryOperandIsRead(ins, memOp))
                {
                    /*INS_InsertFillBuffer(ins, IPOINT_BEFORE, bufId,
                                         IARG_INST_PTR, offsetof(struct MEMREF, pc),
                                         IARG_MEMORYOP_EA, memOp, offsetof(struct MEMREF, ea),
                                         IARG_UINT32, refSize, offsetof(struct MEMREF, size), 
                                         IARG_BOOL, TRUE, offsetof(struct MEMREF, read),
                                         IARG_END);*/
		 RegMemRef();
                }

                if (INS_MemoryOperandIsWritten(ins, memOp))
                {
                    /*INS_InsertFillBuffer(ins, IPOINT_BEFORE, bufId,
                                         IARG_INST_PTR, offsetof(struct MEMREF, pc),
                                         IARG_MEMORYOP_EA, memOp, offsetof(struct MEMREF, ea),
                                         IARG_UINT32, refSize, offsetof(struct MEMREF, size), 
                                         IARG_BOOL, FALSE, offsetof(struct MEMREF, read),
                                         IARG_END);*/
			 RegMemRef();
                }
		
		 else {
		RegCoRef();cout<<"\n";
	    //INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)GenPimCoRef, IARG_END);
                        } 
			}
				
			 }


            



        }
    }
}


/**************************************************************************
 *
 *  Callback Routines
 *
 **************************************************************************/

VOID * BufferFull(BUFFER_ID id, THREADID tid, const CONTEXT *ctxt, VOID *buf,
                  UINT64 numElements, VOID *v)
{
    struct MEMREF * reference=(struct MEMREF*)buf;

    MLOG * mlog = static_cast<MLOG*>( PIN_GetThreadData( mlog_key, tid ) );

    mlog->DumpBufferToFile( reference, numElements, tid );
    
    return buf;
}


/*
 * Note that opening a file in a callback is only supported on Linux systems.
 * See buffer-win.cpp for how to work around this issue on Windows.
 */
VOID ThreadStart(THREADID tid, CONTEXT *ctxt, INT32 flags, VOID *v)
{
    // There is a new MLOG for every thread.  Opens the output file.
    MLOG * mlog = new MLOG(tid);

    // A thread will need to look up its MLOG, so save pointer in TLS
    PIN_SetThreadData(mlog_key, mlog, tid);

}


VOID ThreadFini(THREADID tid, const CONTEXT *ctxt, INT32 code, VOID *v)
{
    MLOG * mlog = static_cast<MLOG*>(PIN_GetThreadData(mlog_key, tid));

    delete mlog;

    PIN_SetThreadData(mlog_key, 0, tid);
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
    else if ( RTN_IsDynamic(rtn)&& gen_pim_flag)
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
	 //INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)GenPimMemCount, IARG_END);
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
	
        }
    else 
        {         reg_comp_ref++;
		
}
    }
}
	INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)docount, IARG_END);
	INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)FixPimCount, IARG_END);
	INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)GenPimMemCount, IARG_END);
	INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)GenPimCoRef, IARG_END);
	INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)RegMemRef, IARG_END);
	INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)RegCoRef, IARG_END);
}





// This function is called when the application exits
VOID Fini(INT32 code, VOID *v)
{
    // Write to a file since cout and cerr maybe closed by the application
	
    
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
	 PIN_InitSymbols();
    if( PIN_Init(argc,argv) )
    {
        return Usage();
    }
	

   
bufId = PIN_DefineTraceBuffer(sizeof(struct MEMREF), NUM_BUF_PAGES,
                                  BufferFull, 0);

    if(bufId == BUFFER_ID_INVALID)
    {
        cerr << "Error: could not allocate initial buffer" << endl;
        return 1;
    }

    // Initialize thread-specific data not handled by buffering api.
    mlog_key = PIN_CreateThreadDataKey(0);
   
    OutFile.open(KnobOutputFile.Value().c_str());
	OutFile.setf(ios::showbase);
    // add an instrumentation function
   
    
	 TRACE_AddInstrumentFunction(Trace, 0);
     PIN_AddThreadStartFunction(ThreadStart, 0);
     PIN_AddThreadFiniFunction(ThreadFini, 0);

    // Register ImageLoad to be called to instrument instructions
    IMG_AddInstrumentFunction(Image, 0);
    // Register Instruction to be called to instrument instructions
    INS_AddInstrumentFunction(Instruction, 0);

    // Register Fini to be called when the application exits
    PIN_AddFiniFunction(Fini, 0);
    
    // Start the program, never returns
    PIN_StartProgram();
    
    return 0;
}



