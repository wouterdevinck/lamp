module animator #( 
  parameter c_ledboards = 30,
  parameter c_bpc = 12,
  parameter c_max_time = 1024,
  parameter c_max_type = 64,
  parameter c_channels = c_ledboards * 32,
  parameter c_addr_w = $clog2(c_channels),
  parameter c_time_w = $clog2(c_max_time),
  parameter c_type_w = $clog2(c_max_type)
)(
  input i_clk, i_drq,
  input [c_bpc-1:0] i_target_data, i_current_data,
  input [c_type_w-1:0] i_type,
  input [c_time_w-1:0] i_target_time, i_start_time,
  output o_wen,
  output [c_addr_w-1:0] o_addr,
  output [c_bpc-1:0] o_data,
  output o_drq
);

  localparam c_channels_1 = c_channels - 1;
  localparam c_max_time_1 = c_max_time - 1;

  reg [c_addr_w-1:0] r_addr = 0;
  reg [c_bpc-1:0] r_data = 0;
  reg r_wen = 0;
  reg r_drq = 0;

  reg [c_time_w-1:0] r_count = 0;

  localparam s_wait =  3'd0;
  localparam s_read =  3'd1;
  localparam s_anim =  3'd2;
  localparam s_write = 3'd3;
  localparam s_end =   3'd4;
  reg [2:0] r_state = s_wait;

  localparam c_anim_linear =  1'd1; 

  /* verilator lint_off WIDTH */
  task calculate;

    input [c_type_w-1:0] i_anim_type;
    input [c_bpc-1:0] i_current_data;
    input [c_bpc-1:0] i_target_data; 
    input [c_time_w-1:0] i_start_time;
    input [c_time_w-1:0] i_current_time;
    input [c_time_w-1:0] i_target_time;
    output [c_bpc-1:0] o_data;

    begin
      case (i_anim_type) 
        // TODO BUG division by zero possible
        c_anim_linear: begin
          if (i_target_time < i_current_time) begin
            o_data = i_current_data + (i_target_data - i_current_data) / 
                     (c_max_time - i_current_time + i_target_time);
          end else begin
            o_data = i_current_data + (i_target_data - i_current_data) / 
                     (i_target_time - i_current_time);
          end
        end
        default: begin
        end
      endcase
    end

  endtask
  /* verilator lint_on WIDTH */

  always @(posedge i_clk) begin
    case (r_state)
      s_wait: begin
        if (i_drq == 1'b1) begin
          if (r_count == c_max_time_1[c_time_w-1:0]) begin
            r_count <= 0;
          end else begin
            r_count <= r_count + 1;
          end
          r_addr <= 0;
          r_state <= s_read;
        end
        r_drq <= 0;
      end
      s_read: begin
        r_state <= s_anim;
      end
      s_anim: begin
        calculate(
          i_type,
          i_current_data, i_target_data, 
          i_start_time, r_count, i_target_time, 
          r_data
        );
        r_wen <= 1;
        r_state <= s_write;
      end
      s_write: begin
        r_wen <= 0;
        if (r_addr == c_channels_1[c_addr_w-1:0]) begin
          r_state <= s_end;
        end else begin
          r_addr <= r_addr + 1;
          r_state <= s_read;
        end
      end
      s_end: begin
        if (r_count == i_target_time) begin
          r_drq <= 1'b1;
        end
        r_state <= s_wait;
      end
      default: begin
      end
    endcase
  end

  assign o_addr = r_addr;
  assign o_data = r_data;
  assign o_wen = r_wen;
  assign o_drq = r_drq;

endmodule