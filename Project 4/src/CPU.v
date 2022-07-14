`timescale 1ns / 1ps 
`include "CPU_para.v"

// unit:1ns precision:1ps

module CPU(clock,i_datain,d_datain,d_dataout,d_addr,i_addr);

input clock;
input [31:0] i_datain;
input [31:0] d_datain;

output [31:0] d_dataout;
output [31:0] d_addr; 
output [31:0] i_addr; 

// Register definition------------------------------------------

// ***********************************
//Step 1: IF: instruction fetching
// ***********************************

// input: i_datain, d_datain, pcSrcD, pcBranchD, pcPlus4F, 
// input: jumpD, jumpAddressD

// output: InstrF, pcPlus4F, dataF

reg [31:0] InstrF; 
reg [31:0] pcPlus4F;
reg [31:0] dataF;

reg [31:0] pc = 32'h0;  // initial value
reg [31:0] pcF;

reg [31:0] pcAddressD;

// ***********************************
//Step 2: ID: instruction decoding
// ***********************************

// input: InstrD, pcPlus4D, dataD

// output: RegWriteD, MemtoRegD, MemWriteD, BranchD, ShiftD
// output: ALUControlD, ALUSrcD, RegDstD, jumpD
// output: RD1, RD2, RtD, RdD, ShamtD, SignImmD, ZeroImmD 
// output: pcSrcD, pcBranchD, jumpAddressD, dataD


reg [31:0] InstrD;
reg [31:0] pcPlus4D;

// control unit registers with default values 
reg RegWriteD; 
reg MemtoRegD;
reg MemWriteD;
reg BranchD;
reg ShiftD;
reg [3:0] ALUControlD;
reg [1:0] ALUSrcD;
reg RegDstD;
reg jumpD;
reg pcSrcD;


reg [31:0] RD1;
reg [31:0] RD2;
reg [4:0] RtD;
reg [4:0] RdD;
reg [4:0] ShamtD;
reg [31:0] SignImmD;
reg [31:0] ZeroImmD;
reg [31:0] pcBranchD;
reg [31:0] jumpAddressD;
reg [31:0] dataD;

reg [5:0] opcode;
reg [5:0] func;
reg [4:0] A1;
reg [4:0] A2;
reg [4:0] A3;  
reg [31:0] WD3;
reg signed [15:0] imm;
reg WE3; // write enable

reg b_flag = 0; // 0: beq  1:bne
reg jr = 0; // 1: jr

// 32 general registers:
reg [31:0] gr[31:0]; 


// ***********************************
//Step 3: EX: execute operation
// ***********************************

//  input: RegWriteE, MemtoRegE, MemWriteE, ShiftE
//  input: ALUControlE, ALUSrcE, RegDstE
//  input: SrcRD1, SrcRD2, RtE, RdE, ShamtE
//  input: SignImmE, ZeroImmE, dataE

//  output: RegWriteE, MemtoRegE, MemWriteE
//  output: ALUOutE, WriteDataE, WriteRegE, dataE

reg RegWriteE;
reg MemtoRegE;
reg MemWriteE;
reg ShiftE;
reg [3:0] ALUControlE;
reg [1:0] ALUSrcE;
reg RegDstE;

reg [31:0] SrcRD1;
reg [31:0] SrcRD2;
reg [4:0] RtE;
reg [4:0] RdE;
reg [4:0] ShamtE;
reg [31:0] SignImmE;
reg [31:0] ZeroImmE;
reg [31:0] dataE;

reg [31:0] ALUOutE;
reg [31:0] WriteDataE;
reg [4:0] WriteRegE;

reg [31:0] SrcAE;
reg [31:0] SrcBE;
reg of;
reg zf;
reg nf;



// ***********************************
//Step 4: MEM: assess data memory
// ***********************************

//  input: RegWriteM, MemtoRegM, MemWriteM
//  input: ALUOutM, WriteDataM, WriteRegM, dataM

//  output: RegWriteM, MemtoRegM
//  output: ALUOutM,ReadDataM, WriteRegM
 
reg RegWriteM;
reg MemtoRegM;
reg MemWriteM;
reg [31:0] ALUOutM;
reg [31:0] WriteDataM;
reg [4:0] WriteRegM;
reg [31:0] dataM;

reg [31:0] ReadDataM;

reg WE; // write enable
reg [31:0] A;
reg [31:0] WD;



// ***********************************
//Step 5: WB: write back
// ***********************************

//  input: RegWriteW, MemtoRegW
//  input: ALUOutW, ReadDataW, WriteRegW

//  output: RegWriteW, ResultW, WriteRegW

reg RegWriteW;
reg MemtoRegW;
reg [31:0] ALUOutW;
reg [31:0] ReadDataW;
reg [4:0] WriteRegW;

reg [31:0] ResultW; 

// Task definition------------------------------------------

// 1.Decide control unit variables
task control_unit;

input [5:0] opcode,func;

output reg RegWriteD; 
output reg MemtoRegD;
output reg MemWriteD;
output reg BranchD;
output reg ShiftD;
output reg [3:0] ALUControlD;
output reg [1:0] ALUSrcD;
output reg RegDstD;
output reg jumpD;


begin
// set default value
RegWriteD = 1; 
MemtoRegD = 0;
MemWriteD = 0;
BranchD = 0;
pcSrcD = 0;
ShiftD = 0; // 0: use RD1E  1: use ShamtE
ALUSrcD = 2'h0; // 0: use RD2E 1: use SignImmE  2: use ZeroImmE
RegDstD = 1; // 0: use Rt  1: use Rd
jumpD = 0; 

// define parameters


case (opcode)

`op_R:begin // R instructions
    case(func)
    
    `func_add:begin
        ALUControlD = `ALU_add;
    end

    `func_sub:begin
        ALUControlD = `ALU_sub;
    end

    `func_addu:begin
        ALUControlD = `ALU_add;
    end

    `func_subu:begin
        ALUControlD = `ALU_sub;
    end

    `func_and:begin
        ALUControlD = `ALU_and;
    end

    `func_or:begin
        ALUControlD = `ALU_or;
    end    

    `func_nor:begin
        ALUControlD = `ALU_nor;
    end

    `func_xor:begin
        ALUControlD = `ALU_xor;
    end

    `func_sll:begin
        ALUControlD = `ALU_sll;
        ShiftD = 1;
    end

    `func_srl:begin
        ALUControlD = `ALU_srl;
        ShiftD = 1;
    end

    `func_sra:begin
        ALUControlD = `ALU_sra;
        ShiftD = 1;
    end

    `func_sllv:begin
        ALUControlD = `ALU_sll;
    end    

    `func_srlv:begin
        ALUControlD = `ALU_srl;
    end

    `func_srav:begin
        ALUControlD = `ALU_sra;
    end

    `func_slt:begin
        ALUControlD = `ALU_slt;
    end    

    `func_jr:begin
        ALUControlD = `ALU_NOP;
        RegWriteD = 0;
        jumpD = 1;
        jr = 1;
    end




    endcase
end // end of op_R

`op_lw:begin
    ALUControlD = `ALU_add;
    MemtoRegD = 1;
    ALUSrcD = 2'h1;
    RegDstD = 0;
end

`op_sw:begin
    ALUControlD = `ALU_add;
    RegWriteD = 0;
    MemWriteD = 1;
    ALUSrcD = 2'h1;
end

`op_addi:begin
    ALUControlD = `ALU_add;
    ALUSrcD = 2'h1;
    RegDstD = 0;
end

`op_addiu:begin 
    ALUControlD = `ALU_add;
    ALUSrcD = 2'h1; // signed extension
    RegDstD = 0;
end

`op_andi:begin
    ALUControlD = `ALU_and;
    ALUSrcD = 2'h2;
    RegDstD = 0;
end

`op_ori:begin
    ALUControlD = `ALU_or;
    ALUSrcD = 2'h2;
    RegDstD = 0;
end

`op_beq:begin
    ALUControlD = `ALU_NOP;
    RegWriteD = 0;
    BranchD = 1;
end

`op_bne:begin
    ALUControlD = `ALU_NOP;
    RegWriteD = 0;
    BranchD = 1;
    b_flag = 1;
end

`op_j:begin 
    ALUControlD = `ALU_NOP;
    RegWriteD = 0;
    jumpD = 1;
end

`op_jal:begin 
    ALUControlD = `ALU_NOP;
    RegWriteD = 0;
    jumpD = 1;
    gr[31] = pcPlus4D; // write to $ra
end

endcase

end
endtask

// 2. ALU operations
task ALU;
input [31:0] SrcAE, SrcBE;
input [3:0] ALUControlE;

output reg [31:0] ALUOutE;
output reg of;
output reg zf;
output reg nf;


begin
case(ALUControlE)

`ALU_and:begin
    ALUOutE = SrcAE & SrcBE;
end

`ALU_or:begin
    ALUOutE = SrcAE | SrcBE;
end

`ALU_add:begin
    ALUOutE = SrcAE + SrcBE;
end

`ALU_xor:begin
    ALUOutE = SrcAE ^ SrcBE;
end

`ALU_sub:begin
    SrcBE = -$signed(SrcBE);
    ALUOutE = SrcAE + SrcBE;
end

`ALU_slt:begin
    ALUOutE = ($signed(SrcAE)<$signed(SrcBE)) ? 1:0;
end

`ALU_sll:begin
    ALUOutE = SrcBE << SrcAE;
end

`ALU_srl:begin
    ALUOutE = SrcBE >> SrcAE;
end

`ALU_sra:begin
    ALUOutE = $signed(SrcBE) >>> SrcAE;
end

`ALU_nor:begin
    ALUOutE = SrcBE ~| SrcAE;
end

`ALU_NOP:begin
    ALUOutE = 0;// clear the result
end


endcase

of = (~SrcAE[31]*~SrcBE[31]*ALUOutE[31])|(SrcAE[31]*SrcBE[31]*~ALUOutE[31]);
zf = (ALUOutE==0) ? 1:0;
nf = (ALUOutE[31]==1) ? 1:0;

end
endtask




// DataPath Starts!!! -------------------------------------------

// ***********************************
//Step 1: IF: instruction fetching
// ***********************************

always @(posedge clock)
begin
    // $display("1");
    pcF <= pc;
    InstrF <= i_datain;
    dataF <= d_datain;
end

always @(pcF,InstrF,dataF)
begin
    // $display("IF");
    pcPlus4F = pcF + 4;

    if (pcSrcD==1) pcAddressD = pcBranchD;
    else pcAddressD = pcPlus4F;

    if (jumpD==1) pc = jumpAddressD;
    else pc = pcAddressD;

end


// ***********************************
//Step 2: ID: instruction decoding
// ***********************************

always @(posedge clock)
begin
    // $display("2");
    InstrD <= InstrF;
    pcPlus4D <= pcPlus4F;
    dataD <= dataF;
end

always @(InstrD,pcPlus4D,dataD)
begin
    // $display("ID");

    opcode = InstrD[31:26];
    func = InstrD[5:0];
    A1 = InstrD[25:21];
    A2 = InstrD[20:16];
    RtD = InstrD[20:16];
    RdD = InstrD[15:11];
    ShamtD = InstrD[10:6];
    imm = InstrD[15:0];

    control_unit(opcode,func,RegWriteD,MemtoRegD,MemWriteD,
    BranchD,ShiftD,ALUControlD,ALUSrcD,RegDstD,jumpD);

    RD1 = gr[A1]; 
    RD2 = gr[A2]; 

    jumpAddressD = {pcPlus4D[31:28],(InstrD[25:0] << 2)};    
    if (jr==1) jumpAddressD = gr[A1]; // jr

    pcSrcD = ((RD1==RD2) ^ b_flag) && BranchD;  
    
    SignImmD = $signed(imm);
    ZeroImmD = $unsigned(imm);

    pcBranchD = (SignImmD << 2) + pcPlus4D;

    if (pcSrcD==1) pcAddressD = pcBranchD;
    else pcAddressD = pcPlus4F;

    if (jumpD==1) pc = jumpAddressD;
    else pc = pcAddressD;    

end


// ***********************************
//Step 3: EX: execute operation
// ***********************************

always @(posedge clock)
begin
    // $display("3");
    RegWriteE <= RegWriteD;
    MemtoRegE <= MemtoRegD;
    MemWriteE <= MemWriteD;
    ShiftE <= ShiftD;    
    ALUControlE <= ALUControlD;
    ALUSrcE <= ALUSrcD;
    RegDstE <= RegDstD;
    SrcRD1 <= RD1;
    SrcRD2 <= RD2;
    RtE <= RtD;
    RdE <= RdD;
    ShamtE <= ShamtD;
    SignImmE <= SignImmD;
    ZeroImmE <= ZeroImmD;
    dataE <= dataD;
end

always @(RegWriteE,MemtoRegE,MemWriteE,ShiftE,ALUControlE,ALUSrcE,
        RegDstE,SrcRD1,SrcRD2,RtE,RdE,ShamtE,SignImmE,ZeroImmE,dataE)
begin
    // $display("EX");

    if (ShiftE==1) SrcAE = ShamtE;
    else SrcAE = SrcRD1; // 0

    if (ALUSrcE==1) SrcBE = SignImmE;
    else if (ALUSrcE==2) SrcBE = ZeroImmE;
    else SrcBE = SrcRD2; // 0

    if (RegDstE==1) WriteRegE = RdE;
    else WriteRegE = RtE; // 0

    WriteDataE = SrcRD2;

    ALU(SrcAE,SrcBE,ALUControlE,ALUOutE,of,zf,nf);

end



// ***********************************
//Step 4: MEM: assess data memory
// ***********************************

always @(posedge clock)
begin
    // $display("4");
    RegWriteM <= RegWriteE;
    MemtoRegM <= MemtoRegE;
    MemWriteM <= MemWriteE;
    ALUOutM <= ALUOutE;
    WriteDataM <= WriteDataE;
    WriteRegM <= WriteRegE;    
    dataM <= dataE;
end

always @(RegWriteM,MemtoRegM,MemWriteM,ALUOutM,WriteDataM,WriteRegM,dataM)
begin
    // $display("MEM");

    WE = MemWriteM;
    A = ALUOutM;
    WD = WriteDataM;
    if (WE==1) $display("  :Data Written in Memory    :%b", WD);

    ReadDataM = dataM;

end


// ***********************************
//Step 5: WB: write back
// ***********************************

always @(posedge clock)
begin
    // $display("5");
    RegWriteW <= RegWriteM;
    MemtoRegW <= MemtoRegM;
    ALUOutW <= ALUOutM;
    ReadDataW <= ReadDataM;
    WriteRegW <= WriteRegM;
end

always @(RegWriteW,MemtoRegW,ALUOutW,ReadDataW,WriteRegW)
begin
    // $display("WB");

    if (MemtoRegW==1) ResultW = ReadDataW;
    else ResultW = ALUOutW; // 0

    A3 = WriteRegW;
    WD3 = ResultW;
    WE3 = RegWriteW;
    
    if (WE3==1) gr[A3] = WD3;  
end


assign d_dataout = WD;
assign d_addr = A;
assign i_addr = pcF;

endmodule