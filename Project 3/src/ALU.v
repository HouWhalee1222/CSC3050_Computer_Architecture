module alu(i_datain,gr1,gr2,c,zero,overflow,neg);


// Wire type for input & output

// input: 32-bit instruction, two register values
input signed[31:0] i_datain,gr1,gr2;// gr1 = rs ; gr2 = rt ;

// output: 32-bit result, three flags
output signed[31:0] c;
output zero;
output overflow;
output neg;

// Reg type for datapath

// For instruction decoding
reg[5:0] opcode, func;
reg signed [15:0] imm;

// For control unit
reg[3:0] ALUctr;
reg[1:0] ALUSrc; // 0:select gr2  1:select signImm 2:select zeroImm
reg branch; // 0: no branch  1: branch instruction
reg Overflowctr; // 0: no overflow  1:maybe overflow
reg Negctr; // 0: non-negative  1:maybe negative
reg subuctr; // specially for judging negative flag of subu. 0:subu 1:not subu
reg mdctr; // specially for differentiate mult(u) and div(u) // 0:signed 1:unsigned

// For ALU 
reg[31:0] reg_A, reg_B; // ALU input
reg[31:0] reg_C; // ALU output     
reg zf; // zero flag
reg nf; // negative flag
reg of; // overflow flag 

// Special reg in ALU for mult(u) and div(u)
reg[63:0] mult,mult_A,mult_B;
reg[31:0] hi,lo;

// For sign/zero extension
reg signed [31:0] signImm;
reg [31:0] zeroImm;


//  ============ Execution begins  ============ //

always @(i_datain,gr1,gr2) // As long as either of the three inputs changes
// no wire should be included
begin

 // Instruction decoding
opcode = i_datain[31:26];
func = i_datain[5:0];
imm = i_datain[15:0];

//sign extension
signImm = $signed(i_datain[15:0]);
zeroImm = $unsigned(i_datain[15:0]);

// default value for control unit;
ALUSrc = 0;
branch = 0; 
Overflowctr = 0;
Negctr = 0;
subuctr = 1;

// Control Part decoding
case(opcode)  
6'h0: // R instructions
begin
    case(func) 
    6'h20: // add
    begin
        ALUctr = 4'b0010;
        Overflowctr = 1;
        Negctr = 1;
    end

    6'h21: // addu
    begin
        ALUctr = 4'b0010;
    end

    6'h22: // sub
    begin
        ALUctr = 4'b0110;
        Overflowctr = 1;
        Negctr = 1;
    end

    6'h23: // subu
    begin
        ALUctr = 4'b0110;
        Negctr = 1; // special
        subuctr = 0; // special
    end

    6'h18: // mult
    begin
        ALUctr = 4'b0100;
        Negctr = 1;
        mdctr = 0;
    end

    6'h19: // multu
    begin
        ALUctr = 4'b0100;
        mdctr = 1;
    end

    6'h1a: // div
    begin
        ALUctr = 4'b0011;
        Negctr = 1;
        mdctr = 0;
    end

    6'h1b: // divu
    begin
        ALUctr = 4'b0011;
        mdctr = 1;
    end

    6'h24: // and
    begin
        ALUctr = 4'b0000;
        Negctr = 1;
    end

    6'h27: // nor
    begin
        ALUctr = 4'b1100;
        Negctr = 1;
    end

    6'h25: // or
    begin
        ALUctr = 4'b0001;
        Negctr = 1;
    end
    
    6'h26: // xor
    begin
        ALUctr = 4'b0101;
        Negctr = 1;
    end
    
    6'h2a: // slt
    begin
        ALUctr = 4'b0111;
        Negctr = 1;
    end
    
    6'h2b: // sltu
    begin
        ALUctr = 4'b1111;
    end

    6'h0: // sll
    begin
        ALUctr = 4'b1000;
        Negctr = 1;         
    end

    6'h4: // sllv
    begin
        ALUctr = 4'b1101;
        Negctr = 1;           
    end

    6'h2: // srl
    begin
        ALUctr = 4'b1001;
    end

    6'h6: // srlv
    begin
        ALUctr = 4'b1110;
    end
    6'h3: // sra
    begin
        ALUctr = 4'b1010;
        Negctr = 1; // arithmetic-signed         
    end

    6'h7: // srav
    begin
        ALUctr = 4'b1011;
        Negctr = 1;   
    end

    endcase // end of case(funct) 
end  // end of R instrcutions cases

6'h8: // addi
begin
    ALUctr = 4'b0010;
    ALUSrc = 1;
    Overflowctr = 1;
    Negctr = 1;
end

6'h9: // addiu
begin
    ALUctr = 4'b0010;
    ALUSrc = 1;
end

6'hc: // andi
begin
    ALUctr = 4'b0000;
    ALUSrc = 2; // zero extend
end


6'hd: // ori
begin
    ALUctr = 4'b0001;
    ALUSrc = 2;
end

6'he: //xori
begin
    ALUctr = 4'b0101;
    ALUSrc = 2;
end

6'h4,6'h5://beq,bne(bug here...)
begin
    ALUctr = 4'b0110;
    ALUSrc = 0;
    branch = 1; 
end

6'ha: //slti
begin
    ALUctr = 4'b0111;
    ALUSrc = 1;
end

6'hb: //sltiu
begin
    ALUctr = 4'b1111;
    ALUSrc = 1;
end

6'h23: // lw
begin
    ALUctr = 4'b0010;
    ALUSrc = 1;
end

6'h2b: // sw
begin
    ALUctr = 4'b0010;
    ALUSrc = 1;
end // there will be difference (lw & sw) in project 4

endcase // end of case(opcode)


// =============== ALU =============== //

// Value of reg in ALU;
// reg_A
reg_A = gr1;

// reg_B, select ALUSrc
case(ALUSrc)
2'h0:
begin
    reg_B = gr2;
end
2'h1:
begin
    reg_B = signImm;
end
2'h2:
begin
    reg_B = zeroImm;
end
endcase

// ALU control selection
case(ALUctr)

4'b0000: // and
begin
    reg_C = reg_A & reg_B;
end

4'b0001: // or
begin
    reg_C = reg_A | reg_B;
end

4'b0010: // add
begin
    reg_C = reg_A + reg_B;
end

4'b0011: // div
begin
    // mdctr: 0-signed 1-unsigned
    lo = (~mdctr) ? $signed($signed(reg_A) / $signed(reg_B)) : (reg_A / reg_B);
    hi = (~mdctr) ? $signed($signed(reg_A) % $signed(reg_B)) : (reg_A % reg_B);
    reg_C = 0;
    reg_C[31] = (~mdctr) ? lo[31] : 0; // judge negative
end

4'b0100: // mult
begin
    // mdctr: 0-signed 1-unsigned
    mult_A = (~mdctr) ? $signed(reg_A):reg_A ; mult_B = (~mdctr) ? $signed(reg_B):reg_B;
    mult = (~mdctr) ? $signed($signed(mult_A) * $signed(mult_B)) : (mult_A * mult_B);
    lo = mult[31:0];
    hi = mult[63:32];
    reg_C = 0;
    reg_C[31] = (~mdctr) ? hi[31] : 0; // judge negative
end

4'b0101: // xor
begin
    reg_C = reg_A ^ reg_B;
end

4'b0110: // sub
begin
    if (~subuctr)
    begin
        subuctr = ($unsigned(reg_A) < $unsigned(reg_B)); 
        // 1:can be negative 0:not negative
    end

    reg_B = -$signed(reg_B); //order...
    reg_C = reg_A + reg_B;

end

4'b0111: // set-on-less-than
begin
    reg_C = ($signed(reg_A)<$signed(reg_B)) ? 1:0;
end

4'b1000: // sll
begin
    reg_A = i_datain[10:6]; // shamt
    reg_C = reg_B << reg_A;
end

4'b1001: // srl
begin
    reg_A = i_datain[10:6]; // shamt
    reg_C = reg_B >> reg_A;
end

4'b1010: // sra
begin
    reg_A = i_datain[10:6];
    reg_C = $signed(reg_B) >>> reg_A;
end

4'b1011: // srav
begin
    reg_C = $signed(reg_B) >>> reg_A;
end

4'b1100: // nor
begin
    reg_C = reg_A ~| reg_B;
end

4'b1101: // sllv
begin
    reg_C = reg_B << reg_A;
end

4'b1110: // srlv
begin
    reg_C = reg_B >> reg_A;
end

4'b1111: // set-on-less-than unsigned
begin
    reg_C = ((reg_A)<(reg_B)) ? 1:0;
end

endcase // end of ALUctr

// This part should not be included in the ALU in project 4
// Control flag result 
zf = branch & ((reg_C==0) ? 1:0);
nf = (Negctr & ((reg_C[31]==1) ? 1:0)) & subuctr; // subuctr=0:not negative
of = Overflowctr & ((~reg_A[31]*~reg_B[31]*reg_C[31])|(reg_A[31]*reg_B[31]*~reg_C[31])); // bug:reg_A

end // end of always

assign c = reg_C[31:0];
assign overflow = (~reg_A[31]*~reg_B[31]*reg_C[31])|(reg_A[31]*reg_B[31]*~reg_C[31]);
assign zero = (reg_C==0) ? 1:0;
assign neg = (reg_C[31]==1) ? 1:0;


endmodule // end of ALU module

// iverilog -o ALU ALU.v test_ALU.v
// vvp ALU

