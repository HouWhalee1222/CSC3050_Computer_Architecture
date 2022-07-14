`timescale 1ns/1ps

module alu_test;

reg[31:0] i_datain,gr1;

wire[31:0] c;
//wire zero;
//wire overflow;
//wire neg;

alu testalu(i_datain,gr1,c);

initial
begin

$display("instruction:op:func:  gr1   :   c    : reg_A  : reg_B  : reg_C");
$monitor("   %h:%h: %h :%h:%h:%h:%h:%h",
i_datain, testalu.opcode, testalu.func, gr1 , c, testalu.reg_A, testalu.reg_B, testalu.reg_C);

//// logical left shift

#10 i_datain<=32'b0000_0000_0000_0001_0001_0000_0100_0000;
gr1<=32'b1101_1101_1101_1101_1101_1101_1101_1101;

#10 i_datain<=32'b0000_0000_0000_0001_0001_0000_1000_0000;
gr1<=32'b1101_1101_1101_1101_1101_1101_1101_1101;

#10 i_datain<=32'b0000_0000_0000_0001_0001_0000_0100_0000;
gr1<=32'b0100_0000_0100_0000_0100_0000_0100_0000;

#10 i_datain<=32'b0000_0000_0000_0001_0001_0001_0000_0000;
gr1<=32'b0100_0000_0100_0000_0100_0000_0100_0000;

#10 $finish;
end
endmodule