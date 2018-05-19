`timescale 1us/1ns
`include "lamp.v"

module lamp_tb();

  reg r_clk;

  reg r_dck;
  reg r_cs;
  reg r_mosi;

  wire w_clk;
  wire w_dai;
  wire w_lat;
    
  integer i;

  lamp #(
    .c_freq (20000000) // 20 Mhz
  ) lamp (
    .i_clk (r_clk),
    .i_dck (r_dck),
    .i_cs (r_cs),
    .i_mosi (r_mosi),
    .o_clk (w_clk),
    .o_dai (w_dai),
    .o_lat (w_lat)
  );

  initial begin
    r_clk = 0;
    r_dck = 0;
    r_cs = 1;
    r_mosi = 0;
  end 
 
  always begin
    #0.025 r_clk = !r_clk; // 20 MHz
  end

  always begin
    #100000 $display($time, " 100 ms have passed");
  end

  initial begin
    //#100;
    r_cs = 0;
    sendData(128'h000e0078001001001001800800800800);
    r_cs = 1;
  end

  initial begin
    $dumpfile("build/fpga/lamp.vcd");
    $dumpvars(0, lamp_tb);
    #100000 $finish;
  end

  task sendData;
    input [127:0] i_data;
    begin
      r_cs = 0;
      #10;
      for(i=0; i<128; i=i+1) begin
        #5;
        r_dck = 0;
        r_mosi = i_data[127 - i];
        #5;
        r_dck = 1; // 100 kHz
      end
      #5;
      r_dck = 0;
      r_mosi = 0;
      #10;
      r_cs = 1;
    end
  endtask
   
endmodule