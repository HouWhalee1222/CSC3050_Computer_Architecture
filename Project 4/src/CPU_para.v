// func definition
`define func_add 6'h20
`define func_addu 6'h21
`define func_sub 6'h22
`define func_and 6'h24
`define func_nor 6'h27
`define func_or 6'h25
`define func_sll 6'h0
`define func_sllv 6'h4
`define func_sra 6'h3
`define func_srav 6'h7
`define func_srl 6'h2
`define func_srlv 6'h6
`define func_subu 6'h23
`define func_xor 6'h26
`define func_slt 6'h2a
`define func_jr 6'h8





// opcode definition
`define op_R 6'h0
`define op_addi 6'h8
`define op_addiu 6'h9
`define op_andi 6'hc
`define op_ori 6'hd
`define op_beq 6'h4
`define op_bne 6'h5
`define op_j 6'h2
`define op_jal 6'h3
`define op_lw 6'h23
`define op_sw 6'h2b





// ALUCtr definition
`define ALU_and 4'b0000
`define ALU_or 4'b0001
`define ALU_add 4'b0010
`define ALU_xor 4'b0101
`define ALU_sub 4'b0110
`define ALU_slt 4'b0111
`define ALU_sll 4'b1000
`define ALU_srl 4'b1001
`define ALU_sra 4'b1010
`define ALU_nor 4'b1100
`define ALU_NOP 4'b1111  // do nothing


// general register in register file
`define gr0  	5'b00000
`define gr1  	5'b00001
`define gr2  	5'b00010
`define gr3 	5'b00011
`define gr4  	5'b00100
`define gr5  	5'b00101
`define gr6  	5'b00110
`define gr7  	5'b00111
`define gr8     5'b01000
`define gr9     5'b01001
`define gr10    5'b01010
`define gr11    5'b01011
`define gr12    5'b01100
`define gr13    5'b01101
`define gr14    5'b01110
`define gr15    5'b01111
`define gr16    5'b10000
`define gr17    5'b10001
`define gr18    5'b10010
`define gr19    5'b10011
`define gr20    5'b10100
`define gr21    5'b10101
`define gr22    5'b10110
`define gr23    5'b10111
`define gr24    5'b11000
`define gr25    5'b11001
`define gr26    5'b11010
`define gr27    5'b11011
`define gr28    5'b11100
`define gr29    5'b11101
`define gr30    5'b11110
`define gr31    5'b11111








