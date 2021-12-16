#include<iostream>
#include<string>
#include<vector>
#include<bitset>
#include<fstream>
using namespace std;
#define MemSize 1000 // memory size, in reality, the memory size should be 2^32, but for this lab, for the space resaon, we keep it as this large number, but the memory is still 32-bit addressable.

struct IFStruct {
    bitset<32>  PC;
    bool        nop;  
};

struct IDStruct {
    bitset<32>  Instr;
    bool        nop;  
};

struct EXStruct {
    bitset<32>  Read_data1;
    bitset<32>  Read_data2;
    bitset<16>  Imm;
    bitset<5>   Rs;
    bitset<5>   Rt;
    bitset<5>   Wrt_reg_addr;
    bool        is_I_type;
    bool        rd_mem;
    bool        wrt_mem; 
    bool        alu_op;     //1 for addu, lw, sw, 0 for subu 
    bool        wrt_enable;
    bool        nop;  
};

struct MEMStruct {
    bitset<32>  ALUresult;
    bitset<32>  Store_data;
    bitset<5>   Rs;
    bitset<5>   Rt;    
    bitset<5>   Wrt_reg_addr;
    bool        rd_mem;
    bool        wrt_mem; 
    bool        wrt_enable;    
    bool        nop;    
};

struct WBStruct {
    bitset<32>  Wrt_data;
    bitset<5>   Rs;
    bitset<5>   Rt;     
    bitset<5>   Wrt_reg_addr;
    bool        wrt_enable;
    bool        nop;     
};

struct stateStruct {
    IFStruct    IF;
    IDStruct    ID;
    EXStruct    EX;
    MEMStruct   MEM;
    WBStruct    WB;
};

class RF
{
    public: 
        bitset<32> Reg_data;
     	RF()
    	{ 
			Registers.resize(32);  
			Registers[0] = bitset<32> (0);  
        }
	
        bitset<32> readRF(bitset<5> Reg_addr)
        {   
            Reg_data = Registers[Reg_addr.to_ulong()];
            return Reg_data;
        }
    
        void writeRF(bitset<5> Reg_addr, bitset<32> Wrt_reg_data)
        {
            Registers[Reg_addr.to_ulong()] = Wrt_reg_data;
        }
		 
		void outputRF()
		{
			ofstream rfout;
			rfout.open("RFresult.txt",std::ios_base::app);
			if (rfout.is_open())
			{
				rfout<<"State of RF:\t"<<endl;
				for (int j = 0; j<32; j++)
				{        
					rfout << Registers[j]<<endl;
				}
			}
			else cout<<"Unable to open file";
			rfout.close();               
		} 
			
	private:
		vector<bitset<32> >Registers;	
};

class INSMem
{
	public:
        bitset<32> Instruction;
        INSMem()
        {       
			IMem.resize(MemSize); 
            ifstream imem;
			string line;
			int i=0;
			imem.open("imem.txt");
			if (imem.is_open())
			{
				while (getline(imem,line))
				{      
					IMem[i] = bitset<8>(line);
					i++;
				}                    
			}
            else cout<<"Unable to open file";
			imem.close();                     
		}
                  
		bitset<32> readInstr(bitset<32> ReadAddress) 
		{    
			string insmem;
			insmem.append(IMem[ReadAddress.to_ulong()].to_string());
			insmem.append(IMem[ReadAddress.to_ulong()+1].to_string());
			insmem.append(IMem[ReadAddress.to_ulong()+2].to_string());
			insmem.append(IMem[ReadAddress.to_ulong()+3].to_string());
			Instruction = bitset<32>(insmem);		//read instruction memory
			return Instruction;     
		}     
      
    private:
        vector<bitset<8> > IMem;     
};
      
class DataMem    
{
    public:
        bitset<32> ReadData;  
        DataMem()
        {
            DMem.resize(MemSize); 
            ifstream dmem;
            string line;
            int i=0;
            dmem.open("dmem.txt");
            if (dmem.is_open())
            {
                while (getline(dmem,line))
                {      
                    DMem[i] = bitset<8>(line);
                    i++;
                }
            }
            else cout<<"Unable to open file";
                dmem.close();          
        }
		
        bitset<32> readDataMem(bitset<32> Address)
        {	
			string datamem;
            datamem.append(DMem[Address.to_ulong()].to_string());
            datamem.append(DMem[Address.to_ulong()+1].to_string());
            datamem.append(DMem[Address.to_ulong()+2].to_string());
            datamem.append(DMem[Address.to_ulong()+3].to_string());
            ReadData = bitset<32>(datamem);		//read data memory
            return ReadData;               
		}
            
        void writeDataMem(bitset<32> Address, bitset<32> WriteData)            
        {
            DMem[Address.to_ulong()] = bitset<8>(WriteData.to_string().substr(0,8));
            DMem[Address.to_ulong()+1] = bitset<8>(WriteData.to_string().substr(8,8));
            DMem[Address.to_ulong()+2] = bitset<8>(WriteData.to_string().substr(16,8));
            DMem[Address.to_ulong()+3] = bitset<8>(WriteData.to_string().substr(24,8));  
        }   
                     
        void outputDataMem()
        {
            ofstream dmemout;
            dmemout.open("dmemresult.txt");
            if (dmemout.is_open())
            {
                for (int j = 0; j< 1000; j++)
                {     
                    dmemout << DMem[j]<<endl;
                }
                     
            }
            else cout<<"Unable to open file";
            dmemout.close();               
        }             
      
    private:
		vector<bitset<8> > DMem;      
};  

void printState(stateStruct state, int cycle)
{
    ofstream printstate;
    printstate.open("stateresult.txt", std::ios_base::app);
    if (printstate.is_open())
    {
        printstate<<"State after executing cycle:\t"<<cycle<<endl; 
        
        printstate<<"IF.PC:\t"<<state.IF.PC.to_ulong()<<endl;        
        printstate<<"IF.nop:\t"<<state.IF.nop<<endl; 
        
        printstate<<"ID.Instr:\t"<<state.ID.Instr<<endl; 
        printstate<<"ID.nop:\t"<<state.ID.nop<<endl;
        
        printstate<<"EX.Read_data1:\t"<<state.EX.Read_data1<<endl;
        printstate<<"EX.Read_data2:\t"<<state.EX.Read_data2<<endl;
        printstate<<"EX.Imm:\t"<<state.EX.Imm<<endl; 
        printstate<<"EX.Rs:\t"<<state.EX.Rs<<endl;
        printstate<<"EX.Rt:\t"<<state.EX.Rt<<endl;
        printstate<<"EX.Wrt_reg_addr:\t"<<state.EX.Wrt_reg_addr<<endl;
        printstate<<"EX.is_I_type:\t"<<state.EX.is_I_type<<endl; 
        printstate<<"EX.rd_mem:\t"<<state.EX.rd_mem<<endl;
        printstate<<"EX.wrt_mem:\t"<<state.EX.wrt_mem<<endl;        
        printstate<<"EX.alu_op:\t"<<state.EX.alu_op<<endl;
        printstate<<"EX.wrt_enable:\t"<<state.EX.wrt_enable<<endl;
        printstate<<"EX.nop:\t"<<state.EX.nop<<endl;        

        printstate<<"MEM.ALUresult:\t"<<state.MEM.ALUresult<<endl;
        printstate<<"MEM.Store_data:\t"<<state.MEM.Store_data<<endl; 
        printstate<<"MEM.Rs:\t"<<state.MEM.Rs<<endl;
        printstate<<"MEM.Rt:\t"<<state.MEM.Rt<<endl;   
        printstate<<"MEM.Wrt_reg_addr:\t"<<state.MEM.Wrt_reg_addr<<endl;              
        printstate<<"MEM.rd_mem:\t"<<state.MEM.rd_mem<<endl;
        printstate<<"MEM.wrt_mem:\t"<<state.MEM.wrt_mem<<endl; 
        printstate<<"MEM.wrt_enable:\t"<<state.MEM.wrt_enable<<endl;         
        printstate<<"MEM.nop:\t"<<state.MEM.nop<<endl;        

        printstate<<"WB.Wrt_data:\t"<<state.WB.Wrt_data<<endl;
        printstate<<"WB.Rs:\t"<<state.WB.Rs<<endl;
        printstate<<"WB.Rt:\t"<<state.WB.Rt<<endl;        
        printstate<<"WB.Wrt_reg_addr:\t"<<state.WB.Wrt_reg_addr<<endl;
        printstate<<"WB.wrt_enable:\t"<<state.WB.wrt_enable<<endl;        
        printstate<<"WB.nop:\t"<<state.WB.nop<<endl; 
    }
    else cout<<"Unable to open file";
    printstate.close();
}
 

int main()
{
    
    RF myRF;
    INSMem myInsMem;
    DataMem myDataMem;
    struct stateStruct state{0};
    state.IF.nop = false;
    state.ID.nop = true;
    state.EX.nop = true;
    state.MEM.nop = true;
    state.WB.nop = true;
    state.EX.alu_op = true;
    int cycle = 0;
			
             
    while (1) {


        /* --------------------- WB stage --------------------- */
        if (!state.WB.nop)
        {
            if (state.WB.wrt_enable)
            {
                myRF.writeRF(state.WB.Wrt_reg_addr, state.WB.Wrt_data);
                
            }
        }






        /* --------------------- MEM stage --------------------- */
        
         if (state.MEM.nop){
            newState.WB.nop = true;
        }
        else{
            newState.WB.nop = false;
            newState.WB.Wrt_data = state.MEM.ALUresult;
            auto writeData = state.MEM.Store_data;
            if (state.MEM.rd_mem){
                newState.WB.Wrt_data = myDataMem.readDataMem(state.MEM.ALUresult);
            }
            else if (state.MEM.wrt_mem){
                if (state.WB.wrt_enable && state.WB.Wrt_reg_addr == state.MEM.Rt) // mem-mem forwarding
                {//forwarding
                    writeData = state.WB.Wrt_data;
                }
                myDataMem.writeDataMem(state.MEM.ALUresult, writeData);
            }
            newState.WB.Rs = state.MEM.Rs;
            newState.WB.Rt = state.MEM.Rt;
            newState.WB.wrt_enable = state.MEM.wrt_enable;
            newState.WB.Wrt_reg_addr = state.MEM.Wrt_reg_addr;
        }

        /* --------------------- EX stage --------------------- */
        if (state.EX.nop){
            newState.MEM.nop = true;
        }
        else
        {
            newState.MEM.nop = false;
            newState.MEM.Store_data = state.EX.Read_data2;
            auto ALUin1 = state.EX.Read_data1;
          
            
            if (state.EX.is_I_type){
            	auto ALUin2 = signextend(state.EX.Imm);
            }
            else{
            	auto ALUin2 = state.EX.Read_data2;
            }
            
            

            if (state.MEM.wrt_enable) { // forwarding   type 1             
                if (state.EX.Rs == state.MEM.Wrt_reg_addr)
                    ALUin1 = state.MEM.ALUresult;
                if (state.EX.Rt == state.MEM.Wrt_reg_addr && !state.EX.rd_mem && !state.EX.wrt_mem)
                    ALUin2 = state.MEM.ALUresult;
            }
            if (state.WB.wrt_enable) //forwarding   type 2
            {
                if (state.EX.Rs == state.WB.Wrt_reg_addr)
                    ALUin1 = state.WB.Wrt_data;
                if (!state.EX.is_I_type && state.EX.Rt == state.WB.Wrt_reg_addr)
                    ALUin2 = state.WB.Wrt_data;
            }
            
            if ( state.EX.alu_op){
            	auto ALUout = ALUin1.to_ulong() + ALUin2.to_ulong();	
            }
            else{
            	 auto ALUout = ALUin1.to_ulong() - ALUin2.to_ulong();
            }
            

            
            if (state.EX.wrt_mem && state.EX.Rt == state.WB.Wrt_reg_addr){// mem forwarding
                newState.MEM.Store_data = state.WB.Wrt_data;
            }

            newState.MEM.ALUresult = ALUout;
            newState.MEM.rd_mem = state.EX.rd_mem;
            newState.MEM.Rs = state.EX.Rs;
            newState.MEM.Rt = state.EX.Rt;
            newState.MEM.wrt_enable = state.EX.wrt_enable;
            newState.MEM.wrt_mem = state.EX.wrt_mem;
            newState.MEM.Wrt_reg_addr = state.EX.Wrt_reg_addr;
        }
        




        /* --------------------- ID stage --------------------- */
         if (state.ID.nop){
            newState.EX.nop = true;
        }
        else
        {
            newState.EX.nop = false;
            auto opcode = bitset<6>((state.ID.Instr >> 26).to_ulong());
            auto funct = bitset<6>((state.ID.Instr).to_ulong() & 0x3f);
            newState.EX.is_I_type = opcode != 0 && opcode != 2;
            newState.EX.Rs = bitset<5>((state.ID.Instr >> 21).to_ulong() & 0x1f);
            newState.EX.Rt = bitset<5>((state.ID.Instr >> 16).to_ulong() & 0x1f);
			newState.EX.rd_mem = opcode == 0x23;  
			newState.EX.wrt_mem = opcode == 0x2B; 
            newState.EX.Read_data1 = myRF.readRF(newState.EX.Rs);
            newState.EX.Read_data2 = myRF.readRF(newState.EX.Rt);
            newState.EX.alu_op = 1;
            newState.EX.wrt_enable = false;
            if (newState.EX.is_I_type)
            {
                newState.EX.Wrt_reg_addr = newState.EX.Rt; 
                newState.EX.Imm = bitset<16>((state.ID.Instr).to_ulong() & 0xffff);
                if (opcode == 0x23)                        
                    newState.EX.wrt_enable = true;
                if (opcode == 0x04) 
                    if (newState.EX.Read_data2 != newState.EX.Read_data1){
                        isBranch = true;
                        auto signext = signextend(newState.EX.Imm);
                        branchAddress = bitset<32>(
                            state.IF.PC.to_ulong() + (bitset<32>((bitset<30>(shiftbits(signext, 0))).to_string<char, std::string::traits_type, std::string::allocator_type>() + "00")).to_ulong());
                    }
            }
            else
            {
                newState.EX.alu_op = funct == 0x23 ? 0 : 1;
                newState.EX.Wrt_reg_addr = bitset<5>((state.ID.Instr >> 11).to_ulong() & 0x1f); 
                newState.EX.wrt_enable = true;
            }
            if (state.EX.wrt_enable) {
                if (!state.EX.nop && ( (!newState.EX.is_I_type && state.EX.Wrt_reg_addr == newState.EX.Rt) || state.EX.Wrt_reg_addr == newState.EX.Rs ))
                    isStall = true;
            }
        }






        /* --------------------- IF stage --------------------- */





        /* --------------------- Stall unit--------------------- */







        if (state.IF.nop && state.ID.nop && state.EX.nop && state.MEM.nop && state.WB.nop)
            break;

        printState(newState, cycle); //print states after executing cycle 0, cycle 1, cycle 2 ... 

        cycle += 1;
        state = newState; /*The end of the cycle and updates the current state with the values calculated in this cycle */

    }

    myRF.outputRF(); // dump RF;	
    myDataMem.outputDataMem(); // dump data mem 

    return 0;
}