module framemanager #(
  parameter c_ledboards = 30,
  parameter c_max_time = 1024,
  parameter c_channels = c_ledboards * 32,
  parameter c_addr_w = $clog2(c_channels),
  parameter c_time_w = $clog2(c_max_time)
)(
  input i_clk,
  input i_drq,
  input [c_time_w-1:0] i_target_time,
  output [c_addr_w-1:0] o_addr,
  output [c_time_w-1:0] o_start_time,
  output o_wen,
  output o_drq
);

  reg [c_addr_w-1:0] r_addr = 0;
  reg [c_time_w-1:0] r_time = 0;
  reg r_drq = 0;

  localparam c_channels_1 = c_channels - 1;

  localparam s_wait =  2'd0;
  localparam s_copy =  2'd1;
  localparam s_flush =  2'd2;
  reg [1:0] r_state = s_wait;

  always @(negedge i_clk) begin
    case (r_state)
      s_wait: begin
        if (i_drq == 1) begin
          r_time <= i_target_time;
          r_state <= s_copy;
        end
        r_addr <= 0;
      end
      s_copy: begin
        if (r_addr == c_channels_1[c_addr_w-1:0]) begin
          r_state <= s_flush;
          r_drq <= 1;
        end else begin
          r_addr <= r_addr + 1;
        end
      end
      s_flush: begin
        r_drq <= 0;
        r_state <= s_wait;
      end
      default: begin
      end
    endcase
  end

  assign o_addr = r_addr;
  assign o_start_time = r_time;
  assign o_wen = (r_state != s_wait);
  assign o_drq = r_drq;

endmodule