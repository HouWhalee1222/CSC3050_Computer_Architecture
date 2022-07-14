`timescale 1ns/1ps
`include "CPU_para.v"

module CPU_test;

reg clock;
reg [31:0] i_datain;
reg [31:0] d_datain;

output [31:0] d_dataout;
output [31:0] d_addr; 
output [31:0] i_addr; 

CPU uut(clock,i_datain,d_datain,d_dataout,d_addr,i_addr);

initial
begin
    clock = 0;
    i_datain = 32'h0;
    d_datain = 32'h0;
    uut.gr[0] = 0;

    $display("pc:instruct:  SrcA  :  SrcB  : ALUOUT : Result : Datain :gr0:gr1:gr2:gr3:gr4:gr5:  gr6   :  gr7   :ra");
    $monitor("%h:%h:%h:%h:%h:%h:%h: %h : %h : %h : %h : %h : %h :%h:%h:%h", 
        uut.pcF[7:0], uut.InstrF, uut.SrcAE, uut.SrcBE, uut.ALUOutM,
        uut.ResultW, d_datain, uut.gr[0][3:0], uut.gr[1][3:0], 
        uut.gr[2][3:0], uut.gr[3][3:0], uut.gr[4][3:0], uut.gr[5][3:0],
        uut.gr[6], uut.gr[7],uut.gr[31][7:0]);

    #period
    d_datain <= 32'h1;
    i_datain <= {`op_lw,`gr0,`gr1,16'h0};
    $display("  :Load data '1' into gr1");

    #period
    d_datain <= 32'h2;
    i_datain <= {`op_lw,`gr0,`gr2,16'h4};
    $display("  :Load data '2' into gr2");

    #period
    d_datain <= 32'h3;
    i_datain <= {`op_lw,`gr0,`gr3,16'h8};
    $display("  :Load data '3' into gr3");

    #period
    d_datain <= 32'h4;
    i_datain <= {`op_lw,`gr0,`gr4,16'hc};
    $display("  :Load data '4' into gr4");    

    #period
    d_datain <= 32'h5;
    i_datain <= {`op_lw,`gr0,`gr5,16'h10};
    $display("  :Load data '5' into gr5");   

    #period
    d_datain <= -32'h2;
    i_datain <= {`op_lw,`gr0,`gr6,16'h14};
    $display("  :Load data '-2' into gr6");  

    #period
    d_datain <= 32'hdd;  
    i_datain <= {`op_lw,`gr0,`gr31,16'h18};
    $display("  :Load data 'hdd into $ra");  

    #period
    i_datain <= {`op_beq,`gr1,`gr2,16'h4};
    $display("  :beq $gr1, $gr2, label");

    #period
    i_datain <= {`op_R,`gr1,`gr2,`gr7,5'h0,`func_add};
    $display("  :add $gr7, $gr1, $gr2");

    #period
    i_datain <= {`op_R,`gr1,`gr2,`gr7,5'h0,`func_sub};
    $display("  :sub $gr7, $gr1, $gr2");

    #period
    i_datain <= {`op_R,`gr3,`gr4,`gr7,5'h0,`func_addu};
    $display("  :addu $gr7, $gr3, $gr4");

    #period
    i_datain <= {`op_R,`gr4,`gr3,`gr7,5'h0,`func_subu};
    $display("  :subu $gr7, $gr4, $gr3");

    #period
    i_datain <= {`op_addi,`gr1,`gr7,-16'h10};
    $display("  :addi $gr7, $gr1, -10");

    #period
    i_datain <= {`op_addiu,`gr2,`gr7,16'h10};
    $display("  :addiu $gr7, $gr2, 10");

    #period
    i_datain <= {`op_bne,`gr1,`gr2,16'h4};
    $display("  :bne $gr1, $gr2, label (offset = 4) ~ pc + 10");

    #period
    i_datain <= {`op_R,`gr3,`gr5,`gr7,5'h0,`func_and};
    $display("  :and $gr7, $gr3, $gr5");

    #period
    i_datain <= {`op_R,`gr3,`gr5,`gr7,5'h0,`func_or};
    $display("  :or $gr7, $gr3, $gr5");

    #period
    i_datain <= {`op_R,`gr3,`gr5,`gr7,5'h0,`func_nor};
    $display("  :nor $gr7, $gr3, $gr5");

    #period
    i_datain <= {`op_R,`gr3,`gr5,`gr7,5'h0,`func_xor};
    $display("  :xor $gr7, $gr3, $gr5");

    #period
    i_datain <= {`op_andi,`gr3,`gr7,16'h9};
    $display("  :andi $gr7, $gr3, 9");

    #period
    i_datain <= {`op_ori,`gr3,`gr7,16'h9};
    $display("  :ori $gr7, $gr3, 9");

    #period
    $display("  :jal target - jump to pc=80, save next pc in $ra");
    i_datain <= {`op_jal,26'h20};

    #period
    i_datain <= {`op_R,5'h0,`gr1,`gr7,5'h1,`func_sll};
    $display("  :sll $gr7, $gr1, 1");

    #period
    i_datain <= {`op_R,5'h0,`gr2,`gr7,5'h1,`func_srl};
    $display("  :srl $gr7, $gr2, 1");

    #period
    i_datain <= {`op_R,5'h0,`gr6,`gr7,5'h1,`func_sra};
    $display("  :sra $gr7, $gr6, 1");

    #period
    i_datain <= {`op_R,`gr2,`gr1,`gr7,5'h0,`func_sllv};
    $display("  :sllv $gr7, $gr1, $gr2");
    // sllv rd, rt, rs

    #period 
    i_datain <= {`op_R,`gr1,`gr4,`gr7,5'h0,`func_srlv};
    $display("  :srlv $gr7, $gr4, $gr1");

    #period
    i_datain <= {`op_R,`gr1,`gr6,`gr7,5'h0,`func_srav};
    $display("  :srav $gr7, $gr6, $gr1");
    
    #period
    i_datain <= {`op_R,`gr1,`gr2,`gr7,5'h0,`func_slt};
    $display("  :slt $gr7, $gr1, $gr2");

    #period
    i_datain <= {`op_R,`gr1,`gr2,`gr7,5'h0,`func_add};
    $display("  :add $gr7, $gr1, $gr2");

    #period
    i_datain <= {`op_j,26'h33};
    $display("  :j target - jump to pc=cc");

    #period
    i_datain <= {`op_R,`gr1,`gr2,`gr7,5'h0,`func_addu};
    $display("  :addu $gr7, $gr1, $gr2");

    #period
    $display("  :add $gr7, $gr1, $gr2"); 
    i_datain <= {`op_R,`gr1,`gr2,`gr7,5'h0,`func_add}; 

    #period
    i_datain <= {`op_sw,`gr0,`gr1,16'h104};
    $display("  :Store data in $gr1 to memory");

    #period
    i_datain <= {`op_sw,`gr0,`gr2,16'h108};
    $display("  :Store data in $gr2 to memory");
    
    #period
    i_datain <= {`op_sw,`gr0,`gr3,16'h10c};
    $display("  :Store data in $gr3 to memory");

    #period
    i_datain <= {`op_sw,`gr0,`gr4,16'h110};
    $display("  :Store data in $gr4 to memory");

    #period
    i_datain <= {`op_sw,`gr0,`gr5,16'h114};
    $display("  :Store data in $gr5 to memory");

    #period
    i_datain <= {`op_sw,`gr0,`gr6,16'h118};
    $display("  :Store data in $gr6 to memory");
    
    #period
    i_datain <= {`op_sw,`gr0,`gr7,16'h11c};
    $display("  :Store data in $gr7 to memory");    

    #period
    $display("  :jr $ra");
    i_datain <= {`op_R,`gr31,15'h0,`func_jr};
    
    #period
    i_datain <= 32'h0;
    $display("  :NOP");

    #period
    i_datain <= 32'h0;
    $display("  :NOP");

    #period
    i_datain <= 32'h0;
    $display("  :NOP");

    #period
    i_datain <= 32'h0;
    $display("  :NOP");

    #period $finish;
    
end

parameter period = 10;
always #5 clock = ~clock;


endmodule