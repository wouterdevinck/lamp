module protocol #(
  parameter c_ledboards = 30,
  parameter c_bpc = 12,
  parameter c_max_time = 1024,
  parameter c_max_type = 64,
  parameter c_channels = c_ledboards * 32,
  parameter c_addr_w = $clog2(c_channels),
  parameter c_time_w = $clog2(c_max_time),
  parameter c_type_w = $clog2(c_max_type)
)(
  input i_clk,
  input i_dck,
  input i_cs,
  input i_mosi,
  output o_wen,
  output [c_addr_w-1:0] o_addr,
  output [c_bpc-1:0] o_data,
  output [c_time_w-1:0] o_time,
  output [c_type_w-1:0] o_type,
  output o_ready // TODO: TEMP? note: ready pulse comes at same time as last write, not after!
);

  reg r_prev_dck = 0;

  localparam c_command_bits = 5;
  localparam c_length_bits = 11;
  localparam c_kf_type_bits = 6;
  localparam c_kf_duration_bits = 10;

  localparam c_command_bit_w = $clog2(c_command_bits);
  localparam c_length_bit_w = $clog2(c_length_bits);
  localparam c_kf_type_bit_w = $clog2(c_kf_type_bits);
  localparam c_kf_duration_bit_w = $clog2(c_kf_duration_bits);

  reg [c_command_bits-1:0] r_command = 0;
  reg [c_length_bits-1:0] r_length = 0;
  reg [c_kf_type_bits-1:0] r_kf_type = 0;
  reg [c_kf_duration_bits-1:0] r_kf_duration = 0;

  reg [c_command_bit_w-1:0] r_command_bit = 0;
  reg [c_length_bit_w-1:0] r_length_bit = 0;
  reg [c_kf_type_bit_w-1:0] r_kf_type_bit = 0;
  reg [c_kf_duration_bit_w-1:0] r_kf_duration_bit = 0;
  reg r_kf_flag = 0;

  reg [2:0] r_bitcount = 0;

  reg r_wen = 0;
  reg [c_addr_w-1:0] r_addr = 0;
  reg [c_bpc-1:0] r_data = 0;

  reg [3:0] r_databit = 0;
  
  localparam s_global_wait = 3'd0;
  localparam s_global_command = 3'd1;
  localparam s_global_length = 3'd2;
  localparam s_global_execute = 3'd3;
  localparam s_global_ready = 3'd4;
  reg [2:0] r_global_state = s_global_wait;
  
  localparam s_keyframe_wait = 2'd0;
  localparam s_keyframe_type = 2'd1;
  localparam s_keyframe_duration = 2'd2;
  localparam s_keyframe_data = 2'd3;
  reg [1:0] r_keyframe_state = s_keyframe_wait;

  localparam c_command_keyframe = 5'd0;

  always @(posedge i_clk) begin
    if (i_cs == 0) begin
      if (i_dck != r_prev_dck) begin
        r_prev_dck = i_dck;
        if(i_dck == 1) begin
          receiveBit(i_mosi);
        end 
      end 
    end else begin
      // TODO to be checked in HW if this is okay, depending on how fast CS goes up again
      r_prev_dck = 0;
      r_global_state = s_global_wait;
      r_keyframe_state = s_keyframe_wait;
      r_command_bit = 0;
      r_length_bit = 0;
      r_kf_type_bit = 0;
      r_kf_duration_bit = 0;
      r_kf_flag = 0;
      r_bitcount = 0;
      r_databit = 0;
      r_wen = 0;
      r_addr = 0;
      r_data = 0;
    end
  end

  task receiveBit;
    input i_bit;
    begin
      case (r_global_state)
        s_global_wait: begin
          r_global_state = s_global_command;
          r_command[c_command_bits - 1] = i_bit;
          r_command_bit = 1;
          r_keyframe_state = s_keyframe_wait;
        end
        s_global_command: begin
          if (r_command_bit == c_command_bits - 1) begin
            r_global_state = s_global_length;
          end
          r_command[c_command_bits - 1 - r_command_bit] = i_bit;
          r_command_bit = r_command_bit + 1;
        end
        s_global_length: begin
          if (r_length_bit == c_length_bits - 1) begin
            r_global_state = s_global_execute;
          end
          r_length[c_length_bits - 1 - r_length_bit] = i_bit;
          r_length_bit = r_length_bit + 1;
        end
        s_global_execute: begin
          if (r_length == 1 && r_bitcount == 7) begin
            r_global_state = s_global_ready;
          end
          if (r_bitcount == 7) begin
            r_length = r_length - 1; 
            r_bitcount = 0;
          end else begin
            r_bitcount = r_bitcount + 1;
          end
          messagePump(i_bit);
        end
        s_global_ready: begin
          r_global_state = s_global_wait;
        end
        default: begin
        end
      endcase
    end
  endtask

  task messagePump;
    input i_bit;
    case (r_command)
      c_command_keyframe: begin
        receiveKeyframe(i_bit);
      end
      default: begin
      end
    endcase
  endtask

  task receiveKeyframe;
    input i_bit;
    begin
      case (r_keyframe_state)
        s_keyframe_wait: begin
          r_keyframe_state = s_keyframe_type;
          r_kf_type[c_kf_type_bits - 1] = i_bit;
          r_kf_type_bit = 1;
        end
        s_keyframe_type: begin
          if (r_kf_type_bit == c_kf_type_bits - 1) begin
            r_keyframe_state = s_keyframe_duration;
          end
          r_kf_type[c_kf_type_bits - 1 - r_kf_type_bit] = i_bit;
          r_kf_type_bit = r_kf_type_bit + 1;
        end
        s_keyframe_duration: begin
          if (r_kf_duration_bit == c_kf_duration_bits - 1) begin
            r_keyframe_state = s_keyframe_data;
          end
          r_kf_duration[c_kf_duration_bits - 1 - r_kf_duration_bit] = i_bit;
          r_kf_duration_bit = r_kf_duration_bit + 1;
        end
        s_keyframe_data: begin
          if (r_databit == 0 && r_kf_flag == 1) begin
            r_addr = r_addr + 1;
          end
          if (r_databit == c_bpc - 2) begin
            r_wen = 1;
          end
          if (r_databit == c_bpc - 1) begin
            r_databit = 0; 
            r_wen = 0;
          end else begin
            if (r_kf_flag == 1) begin
              r_databit = r_databit + 1;
            end
          end
          r_data[c_bpc - 1 - r_databit] = i_bit; 
          r_kf_flag = 1;
        end
        default: begin
        end
      endcase
    end
  endtask

  assign o_wen = r_wen;
  assign o_addr = r_addr;
  assign o_data = r_data;
  assign o_time = r_kf_duration;
  assign o_type = r_kf_type;
  assign o_ready = (r_global_state == s_global_ready);

endmodule