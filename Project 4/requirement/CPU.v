`timescale 1ns / 1ps

//This is an example for a single cycle cpu.
//You should:
//1. Extend the combinational logic part to construct a single cycle processor.(85%)
//2. Add more sequential logic to construct a pipeline processor.(100%)

module CPU(
    input wire clock,
    input wire start,
    input [31:0] i_datain,
    input wire [31:0] d_datain,
    output wire [31:0] d_dataout
    );

    reg [31:0]gr[7:0];//[31:0]for 32bit MIPS processor
    reg [15:0]pc = 32'h00000000;

    reg [31:0]instr;
    reg [31:0]reg_A;
    reg [31:0]reg_B;
    reg [31:0]reg_C;
    reg [31:0]reg_C1;

    reg [5:0]opcode;
    reg [5:0]functcode;

always @(start)
    begin
        gr[0] = 32'h0000_0000;
    end

//sequential logic
always @(posedge clock)
	begin
        //pc
        pc <= pc + 32'h00000004;
        //Use if sentences to implement the multiplexier
    end

//combinational logic
always @(pc)
    begin
        instr = i_datain;
        //About Project 3
        //control unit
        opcode = instr[31:26];
        functcode = instr[5:0];
        //Please set the ALUControl with your own code.

        //reg_A and reg_B
        //lw
        if (opcode == 6'b100011)
        begin
            reg_A = gr[instr[25:21]];
            reg_B = instr[15:0];//sign extension of imm number
        end

        //R-type instruction (add etc.)
        else if (opcode == 6'b000000)
        begin
            reg_A = gr[instr[25:21]];
            reg_B = gr[instr[20:16]];
        end
        //You should do extension of this part of code with the ALU you have finished in project 3.

        //reg_C
        //lw, add, etc.
        if (opcode == 6'b100011 || opcode == 6'b000000)//Use ALUControl here after changing the control unit's code.
        reg_C = reg_A + reg_B;

        //reg_C1(Result)
        //lw, add, etc.
        if (opcode == 6'b100011)
        begin
            reg_C1 = d_datain[31:0];
        end
        //add, etc.
        else if (opcode == 6'b000000)
            reg_C1 = reg_C;
        
        //write back to general registers
        if (opcode == 6'b100011)
            gr[instr[20:16]] = reg_C1;
        else if (opcode == 6'b000000)
            gr[instr[15:11]] = reg_C1;
        //Use the RegWrite(Output of Ctrl Unit) to select.

    end


endmodule
                
