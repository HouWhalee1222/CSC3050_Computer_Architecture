`timescale 1ns/1ps

module alu_test;

// Input
reg[31:0] i_datain,gr1,gr2;

// Output
wire[31:0] c;
wire zero;
wire overflow;
wire neg;

alu testalu(i_datain,gr1,gr2,c,zero,overflow,neg);

initial
begin

$display("instruction:op:func:ALUctr:ALUSrc:  gr1   :  gr2   :   c    :zero:overflow:neg:zf:of:nf: reg_A  : reg_B  : reg_C");
$monitor("  %h :%h: %h : %b :  %h   :%h:%h:%h:  %h :    %h   : %h : %h: %h: %h:%h:%h:%h",
i_datain, testalu.opcode, testalu.func,testalu.ALUctr,testalu.ALUSrc, gr1 ,gr2, c,zero,overflow,neg,testalu.zf, testalu.of, testalu.nf, testalu.reg_A, testalu.reg_B, testalu.reg_C);

// // add
// #10 i_datain<=32'b0000_0001_1000_1011_0101_0000_0010_0000;
// gr1<=32'b0100_0000_0000_0000_0000_0000_0000_0001;
// gr2<=32'b0100_0000_0000_0000_0000_0000_0000_0001;
// $display("instruction:op:func:ALUctr:ALUSrc:  gr1   :  gr2   :   c    :zero:overflow:neg:zf:of:nf: reg_A  : reg_B  : reg_C");

// // addi
// #10 i_datain<=32'b0010_0001_1000_1011_1000_0000_0000_0001;
// gr1<=32'b1000_0000_0000_0000_0000_0000_0000_0001;
// gr2<=32'h0;
// $display("instruction:op:func:ALUctr:ALUSrc:  gr1   :  gr2   :   c    :zero:overflow:neg:zf:of:nf: reg_A  : reg_B  : reg_C");

// // addu
// #10 i_datain<=32'b0000_0001_1000_1011_0101_0000_0010_0001;
// gr1<=32'b1000_0000_0000_0000_0000_0000_0000_1000;
// gr2<=32'b1000_0000_0000_0000_0000_0000_0000_1111;
// $display("instruction:op:func:ALUctr:ALUSrc:  gr1   :  gr2   :   c    :zero:overflow:neg:zf:of:nf: reg_A  : reg_B  : reg_C");

// // addiu
// #10 i_datain<=32'b0010_0101_1000_1011_1000_0000_0000_0000;
// gr1<=32'b0000_0000_0000_0000_0000_0000_0000_0001;
// gr2<=32'h0;
// $display("instruction:op:func:ALUctr:ALUSrc:  gr1   :  gr2   :   c    :zero:overflow:neg:zf:of:nf: reg_A  : reg_B  : reg_C");

// // sub
// #10 i_datain<=32'b0000_0001_1000_1011_0101_0000_0010_0010;
// gr1<=32'b0000_0000_0000_0000_0000_0000_0000_0001;
// gr2<=32'b0000_0000_0000_0000_0000_0000_0001_0000;
// $display("instruction:op:func:ALUctr:ALUSrc:  gr1   :  gr2   :   c    :zero:overflow:neg:zf:of:nf: reg_A  : reg_B  : reg_C");

// // subu
// #10 i_datain<=32'b0000_0001_1000_1011_0101_0000_0010_0011;
// gr1<=32'b1000_0000_0000_0000_0000_0000_0000_0010;
// gr2<=32'b0000_0000_0000_0000_0000_0000_0000_0001;
// $display("instruction:op:func:ALUctr:ALUSrc:  gr1   :  gr2   :   c    :zero:overflow:neg:zf:of:nf: reg_A  : reg_B  : reg_C");

// // mult
// #10 i_datain<=32'b0000_0001_1000_1011_0101_0000_0001_1000;
// gr1<=32'b0000_0000_0000_0000_0000_0000_0000_0010;
// gr2<=32'b1111_1111_1111_1111_1111_1111_1111_1101;
// #10 $display("   HI   :   LO   ");
// #10 $display("%h:%h",testalu.hi,testalu.lo);

// $display("instruction:op:func:ALUctr:ALUSrc:  gr1   :  gr2   :   c    :zero:overflow:neg:zf:of:nf: reg_A  : reg_B  : reg_C");

// // multu
// #10 i_datain<=32'b0000_0001_1000_1011_0101_0000_0001_1001;
// gr1<=32'b0000_0000_0000_0000_0000_0000_0000_0010;
// gr2<=32'b1111_1111_1111_1111_1111_1111_1111_1101;
// #10 $display("   HI   :   LO   ");
// #10 $display("%h:%h",testalu.hi,testalu.lo);

// $display("instruction:op:func:ALUctr:ALUSrc:  gr1   :  gr2   :   c    :zero:overflow:neg:zf:of:nf: reg_A  : reg_B  : reg_C");

// // // div
// #10 i_datain<=32'b0000_0001_1000_1011_0101_0000_0001_1010;
// gr1<=32'b0000_0000_0000_0000_0000_0000_1111_0000;
// gr2<=32'b1111_1111_1111_1111_1111_1111_1111_1110;
// #10 $display("   HI   :   LO   ");
// #10 $display("%h:%h",testalu.hi,testalu.lo);

// $display("instruction:op:func:ALUctr:ALUSrc:  gr1   :  gr2   :   c    :zero:overflow:neg:zf:of:nf: reg_A  : reg_B  : reg_C");

// // divu
// #10 i_datain<=32'b0000_0001_1000_1011_0101_0000_0001_1011;
// gr1<=32'b1100_0000_0000_0000_0000_0000_0001_0000;
// gr2<=32'b0000_0000_0000_0110_0000_0000_0010_0011;
// #10 $display("   HI   :   LO   ");
// #10 $display("%h:%h",testalu.hi,testalu.lo);
// $display("instruction:op:func:ALUctr:ALUSrc:  gr1   :  gr2   :   c    :zero:overflow:neg:zf:of:nf: reg_A  : reg_B  : reg_C");


// // and
// #10 i_datain<=32'b0000_0001_1000_1011_0101_0000_0010_0100;
// gr1<=32'b0000_0000_0000_0000_0000_0000_0001_0101;
// gr2<=32'b0000_0000_0000_0000_0000_0000_0101_1011;
// $display("instruction:op:func:ALUctr:ALUSrc:  gr1   :  gr2   :   c    :zero:overflow:neg:zf:of:nf: reg_A  : reg_B  : reg_C");

// // andi
// #10 i_datain<=32'b0011_0001_1000_1011_0000_0000_0000_0111;
// gr1<=32'b0000_0000_1111_0000_1000_0000_0000_1100;
// gr2<=32'h0;
// $display("instruction:op:func:ALUctr:ALUSrc:  gr1   :  gr2   :   c    :zero:overflow:neg:zf:of:nf: reg_A  : reg_B  : reg_C");

// // nor
// #10 i_datain<=32'b0000_0001_1000_1011_0101_0000_0010_0111;
// gr1<=32'b1111_1111_1111_1111_1111_0101_1111_0000;
// gr2<=32'b1111_0101_1111_0000_1111_1111_1111_0001;
// $display("instruction:op:func:ALUctr:ALUSrc:  gr1   :  gr2   :   c    :zero:overflow:neg:zf:of:nf: reg_A  : reg_B  : reg_C");

// // or
// #10 i_datain<=32'b0000_0001_1000_1011_0101_0000_0010_0101;
// gr1<=32'b0000_0000_0000_0000_0000_0000_0001_0101;
// gr2<=32'b0000_0000_0000_0000_0000_0000_0001_1001;
// $display("instruction:op:func:ALUctr:ALUSrc:  gr1   :  gr2   :   c    :zero:overflow:neg:zf:of:nf: reg_A  : reg_B  : reg_C");

// // ori
// #10 i_datain<=32'b0011_0101_1000_1011_0000_0000_0000_0000;
// gr1<=32'b0000_0000_0000_1111_0000_0000_0000_0001;
// gr2<=32'h0;
// $display("instruction:op:func:ALUctr:ALUSrc:  gr1   :  gr2   :   c    :zero:overflow:neg:zf:of:nf: reg_A  : reg_B  : reg_C");

// // xor
// #10 i_datain<=32'b0000_0001_1000_1011_0101_0000_0010_0110;
// gr1<=32'b0000_0000_0000_0000_0000_0000_0000_0101;
// gr2<=32'b0000_0000_0000_0000_0000_0000_0000_0010;
// $display("instruction:op:func:ALUctr:ALUSrc:  gr1   :  gr2   :   c    :zero:overflow:neg:zf:of:nf: reg_A  : reg_B  : reg_C");

// // xori
// #10 i_datain<=32'b0011_1001_1000_1011_0000_0000_1111_0100;
// gr1<=32'b0000_0000_0000_0000_0000_0000_1111_0011;
// gr2<=32'h0;
// $display("instruction:op:func:ALUctr:ALUSrc:  gr1   :  gr2   :   c    :zero:overflow:neg:zf:of:nf: reg_A  : reg_B  : reg_C");

// // beq
// #10 i_datain<=32'b0001_0001_1000_1011_0101_0000_0010_0010;
// gr1<=32'b0000_0000_0000_0000_0000_0000_0000_0001;
// gr2<=32'b0000_0000_0000_0000_0000_0000_0000_0001;
// $display("instruction:op:func:ALUctr:ALUSrc:  gr1   :  gr2   :   c    :zero:overflow:neg:zf:of:nf: reg_A  : reg_B  : reg_C");

// // bne 
// #10 i_datain<=32'b0001_0101_1000_1011_0101_0000_0010_0010;
// gr1<=32'b0000_0000_0000_0000_0000_0000_0000_0010;
// gr2<=32'b0000_0000_0000_0000_0000_0000_0000_0001;
// $display("instruction:op:func:ALUctr:ALUSrc:  gr1   :  gr2   :   c    :zero:overflow:neg:zf:of:nf: reg_A  : reg_B  : reg_C");

// // slt
// #10 i_datain<=32'b0000_0001_1000_1011_0101_0000_0010_1010;
// gr1<=32'b1000_0000_0000_0000_0000_0000_0000_0001;
// gr2<=32'b0000_0000_0000_0000_0000_0000_0000_0010;
// $display("instruction:op:func:ALUctr:ALUSrc:  gr1   :  gr2   :   c    :zero:overflow:neg:zf:of:nf: reg_A  : reg_B  : reg_C");

// // slti
// #10 i_datain<=32'b0010_1001_1000_1011_0000_0000_0000_0010;
// gr1<=32'b1000_0000_0000_0000_0000_0000_0000_0001;
// gr2<=32'h0;
// $display("instruction:op:func:ALUctr:ALUSrc:  gr1   :  gr2   :   c    :zero:overflow:neg:zf:of:nf: reg_A  : reg_B  : reg_C");

// // sltiu
// #10 i_datain<=32'b0010_1101_1000_1011_0000_0000_0000_0010;
// gr1<=32'b1000_0000_0000_0000_0000_0000_0000_0001;
// gr2<=32'h0;
// $display("instruction:op:func:ALUctr:ALUSrc:  gr1   :  gr2   :   c    :zero:overflow:neg:zf:of:nf: reg_A  : reg_B  : reg_C");

// // sltu
// #10 i_datain<=32'b0000_0001_1000_1011_0101_0000_0010_1011;
// gr1<=32'b1000_0000_0000_0000_0000_0000_0000_0001;
// gr2<=32'b0000_0000_0000_0000_0000_0000_0000_0010;
// $display("instruction:op:func:ALUctr:ALUSrc:  gr1   :  gr2   :   c    :zero:overflow:neg:zf:of:nf: reg_A  : reg_B  : reg_C");

// // lw
// #10 i_datain<=32'b1000_1101_1000_1011_0000_0000_0000_0100;
// gr1<=32'b0000_0000_1111_0000_1000_0000_0000_1100;
// gr2<=32'h0;
// $display("instruction:op:func:ALUctr:ALUSrc:  gr1   :  gr2   :   c    :zero:overflow:neg:zf:of:nf: reg_A  : reg_B  : reg_C");

// // sw
// #10 i_datain<=32'b1010_1101_1000_1011_0000_0000_0000_1000;
// gr1<=32'b0000_0000_1111_0000_1000_0000_0000_1100;
// gr2<=32'h0;
// $display("instruction:op:func:ALUctr:ALUSrc:  gr1   :  gr2   :   c    :zero:overflow:neg:zf:of:nf: reg_A  : reg_B  : reg_C");

// // sll
// #10 i_datain<=32'b0000_0000_0000_0001_0001_0000_1000_0000;
// gr1<=32'h0;
// gr2<=32'b1101_1101_1101_1101_1101_1101_1101_1101;
// $display("instruction:op:func:ALUctr:ALUSrc:  gr1   :  gr2   :   c    :zero:overflow:neg:zf:of:nf: reg_A  : reg_B  : reg_C");

// // srl
// #10 i_datain<=32'b0000_0000_0000_0001_0001_0000_0100_0010;
// gr1<=32'h0;
// gr2<=32'b1101_1101_1101_1101_1101_1101_1101_1101;
// $display("instruction:op:func:ALUctr:ALUSrc:  gr1   :  gr2   :   c    :zero:overflow:neg:zf:of:nf: reg_A  : reg_B  : reg_C");

// // sra
// #10 i_datain<=32'b0000_0000_0000_0001_0001_0000_0100_0011;
// gr1<=32'h0;
// gr2<=32'b1101_1101_1101_1101_1101_1101_1101_1101;
// $display("instruction:op:func:ALUctr:ALUSrc:  gr1   :  gr2   :   c    :zero:overflow:neg:zf:of:nf: reg_A  : reg_B  : reg_C");

// // sllv
// #10 i_datain<=32'b0000_0000_0000_0001_0001_0000_0000_0100;
// gr1<=32'b0000_0000_0000_0000_0000_0000_0000_0100;
// gr2<=32'b1101_1101_1101_1101_1101_1101_1101_1101;
// $display("instruction:op:func:ALUctr:ALUSrc:  gr1   :  gr2   :   c    :zero:overflow:neg:zf:of:nf: reg_A  : reg_B  : reg_C");

// // srav
// #10 i_datain<=32'b0000_0000_0000_0001_0001_0000_0000_0111;
// gr1<=32'b0000_0000_0000_0000_0000_0000_0000_0100;
// gr2<=32'b1101_1101_1101_1101_1101_1101_1101_1101;
// $display("instruction:op:func:ALUctr:ALUSrc:  gr1   :  gr2   :   c    :zero:overflow:neg:zf:of:nf: reg_A  : reg_B  : reg_C");

// // srlv
// #10 i_datain<=32'b0000_0000_0000_0001_0001_0000_0000_0110;
// gr1<=32'b0000_0000_0000_0000_0000_0000_0000_0100;
// gr2<=32'b1101_1101_1101_1101_1101_1101_1101_1101;
// $display("instruction:op:func:ALUctr:ALUSrc:  gr1   :  gr2   :   c    :zero:overflow:neg:zf:of:nf: reg_A  : reg_B  : reg_C");

// // sub-overflow
// #10 i_datain<=32'b0000_0001_1000_1011_0101_0000_0010_0010;
// gr1<=32'b0100_0000_0000_0000_0000_0000_0000_0000;
// gr2<=32'b1000_0000_0000_0000_0000_0000_0000_0001;
// $display("instruction:op:func:ALUctr:ALUSrc:  gr1   :  gr2   :   c    :zero:overflow:neg:zf:of:nf: reg_A  : reg_B  : reg_C");

// // sub-zero 
// #10 i_datain<=32'b0000_0001_1000_1011_0101_0000_0010_0010;
// gr1<=32'b0000_0000_0000_0000_0000_0000_0000_0001;
// gr2<=32'b0000_0000_0000_0000_0000_0000_0000_0001;
// $display("instruction:op:func:ALUctr:ALUSrc:  gr1   :  gr2   :   c    :zero:overflow:neg:zf:of:nf: reg_A  : reg_B  : reg_C");

// // subu-positive
// #10 i_datain<=32'b0000_0001_1000_1011_0101_0000_0010_0011;
// gr1<=32'b1000_0000_0000_0000_0000_0000_0000_1000;
// gr2<=32'b0000_0000_0000_0000_0000_0000_0000_0001;
// $display("instruction:op:func:ALUctr:ALUSrc:  gr1   :  gr2   :   c    :zero:overflow:neg:zf:of:nf: reg_A  : reg_B  : reg_C");

// div-negative
#10 i_datain<=32'b0000_0001_1000_1011_0101_0000_0001_1010;
gr1<=32'b0000_0000_0000_0000_0000_0000_1111_1000;
gr2<=32'b1111_1111_1111_1111_1111_1111_1111_0110;
#10 $display("   HI   :   LO   ");
#10 $display("%h:%h",testalu.hi,testalu.lo);
$display(gr1);$display($signed(gr2));$display(testalu.hi);$display($signed(testalu.lo));
// // subu-negative
// #10 i_datain<=32'b0000_0001_1000_1011_0101_0000_0010_0011;
// gr1<=32'b1000_0000_0000_0000_0000_0000_0000_0000;
// gr2<=32'b1000_0000_0000_0000_0000_0000_0000_0001;
// $display("instruction:op:func:ALUctr:ALUSrc:  gr1   :  gr2   :   c    :zero:overflow:neg:zf:of:nf: reg_A  : reg_B  : reg_C");

#10 $finish;


end
endmodule