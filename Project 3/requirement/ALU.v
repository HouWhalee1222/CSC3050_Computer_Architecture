module alu(i_datain,gr1,c);

output signed[31:0] c;
//output zero;
//output overflow;
//output neg;

input signed[31:0] i_datain,gr1;

reg[5:0] opcode, func;

reg[31:0] reg_C;
//reg zf;
//reg nf;
reg[31:0] reg_A, reg_B;

parameter gr0 = 32'h0000_0000;


always @(i_datain,gr1)
begin

opcode = i_datain[31:26];
func = i_datain[5:0];

case(opcode)
6'b000000:
begin

case(func)
6'b000000:
begin
reg_A = gr1;
reg_B = i_datain[10:6];
reg_C = reg_A << reg_B;
end
endcase

end
endcase

end

assign c = reg_C[31:0];

endmodule