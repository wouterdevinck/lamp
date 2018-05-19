module clkdiv #( 
  parameter c_div = 4 // needs to be even and >= 4
)(
  input i_clk,
  output o_clk
);

  localparam c_div_2 = c_div / 2;
  localparam c_div_2_1 = c_div_2 - 1;
  localparam c_width = $clog2(c_div_2);
  reg [c_width-1:0] r_count = 0;
  reg r_clk = 0;

  always @(posedge i_clk) begin
    if (r_count == c_div_2_1[c_width-1:0]) begin
      r_count <= 0;
      r_clk <= ~r_clk;
    end else begin
      r_count <= r_count + 1;
    end
  end

  assign o_clk = r_clk;

endmodule