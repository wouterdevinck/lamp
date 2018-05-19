module driver #( 
  parameter c_ledboards = 30,
  parameter c_bpc = 12,
  parameter c_frame_period = 16666,
  parameter c_channels = c_ledboards * 32,
  parameter c_addr_w = $clog2(c_channels)
)(
  input i_clk,
  input [c_bpc-1:0] i_data,
  output [c_addr_w-1:0] o_addr,
  output o_clk,
  output o_dai,
  output o_lat
);

  localparam c_frame_period_1 = c_frame_period - 1;
  localparam c_channels_1 = c_channels - 1;
  localparam c_bpc_1 = c_bpc - 1;

  localparam c_count_width = $clog2(c_frame_period);
  localparam c_bit_count_width = $clog2(c_bpc);

  reg [c_count_width-1:0] r_count = 0;
  reg [c_addr_w-1:0] r_addr = 0;
  reg [c_bit_count_width-1:0] r_bitcount = 0;

  localparam s_wait = 3'd0;
  localparam s_load = 3'd1;
  localparam s_prep = 3'd2;
  localparam s_send = 3'd3;
  localparam s_latch = 3'd4;
  reg [2:0] r_state = s_wait; 

  reg r_dai = 0;
  reg r_lat = 0;

  always @(posedge i_clk) begin
    if (r_count == c_frame_period_1[c_count_width-1:0]) begin
      r_count <= 0;
    end else begin
      r_count <= r_count + 1;
    end
  end

  always @(negedge i_clk) begin
    if (r_state == s_send) begin
      r_bitcount <= r_bitcount + 1;
    end else begin
      r_bitcount <= 0;
    end
  end

  always @(posedge i_clk) begin
    case (r_state)
      s_wait: begin
        if (r_count == 0) begin
          r_addr <= 0;
          r_state <= s_load;
        end
      end
      s_load: begin
        r_state <= s_prep;
      end
      s_prep: begin
        r_state <= s_send;
        r_dai <= i_data[c_bpc - 1];
      end
      s_send: begin
        if (r_bitcount == c_bpc[c_bit_count_width-1:0]) begin
          if (r_addr == c_channels_1[c_addr_w-1:0]) begin
            r_state <= s_latch;
          end else begin
            r_addr <= r_addr + 1;
            r_dai <= 0;
            r_state <= s_load;
          end
        end else begin
          r_dai <= i_data[c_bpc - r_bitcount - 1];
        end
      end
      s_latch: begin
        if (r_lat == 1) begin
          r_lat <= 0;
          r_state <= s_wait;
        end else begin
          r_lat <= 1;
        end
      end
      default: begin
      end
    endcase
  end

  assign o_addr = ((r_addr >> 4) << 4) + (15 - (r_addr % 16));

  assign o_clk = ~i_clk & (r_state == s_send);
  assign o_dai = r_dai;
  assign o_lat = r_lat;

endmodule