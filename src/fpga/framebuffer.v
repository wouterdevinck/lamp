module framebuffer #( 
  parameter c_ledboards = 30,
  parameter c_bpc = 12,
  parameter c_max_time = 1024,
  parameter c_max_type = 64,
  parameter c_channels = c_ledboards * 32,
  parameter c_addr_w = $clog2(c_channels),
  parameter c_time_w = $clog2(c_max_time),
  parameter c_type_w = $clog2(c_max_type)
)(
  input i_clk, i_wen,
  input [c_addr_w-1:0] i_waddr, i_raddr,
  input [c_bpc-1:0] i_wdata,
  input [c_time_w-1:0] i_time, 
  input [c_type_w-1:0] i_type, 
  output reg [c_bpc-1:0] o_rdata,
  output reg [c_time_w-1:0] o_time,
  output reg [c_type_w-1:0] o_type 
);

  reg [c_bpc-1:0] r_mem [0:c_channels-1];
  reg [c_time_w-1:0] r_time = 0;
  reg [c_type_w-1:0] r_type = 0;

  integer i;

  always @(posedge i_clk) begin
    if (i_wen) begin
      r_mem[i_waddr] <= i_wdata;
      r_time <= i_time;
      r_type <= i_type;
    end
    o_rdata <= r_mem[i_raddr];
    o_time <= r_time;
    o_type <= r_type;
  end

  initial begin 
    for (i = 0; i < c_channels; i = i + 1) r_mem[i] = 0;
  end

endmodule