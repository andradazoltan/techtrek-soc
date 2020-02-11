// Copyright (C) 2017  Intel Corporation. All rights reserved.
// Your use of Intel Corporation's design tools, logic functions 
// and other software and tools, and its AMPP partner logic 
// functions, and any output files from any of the foregoing 
// (including device programming or simulation files), and any 
// associated documentation or information are expressly subject 
// to the terms and conditions of the Intel Program License 
// Subscription Agreement, the Intel Quartus Prime License Agreement,
// the Intel FPGA IP License Agreement, or other applicable license
// agreement, including, without limitation, that your use is for
// the sole purpose of programming logic devices manufactured by
// Intel and sold by Intel or its authorized distributors.  Please
// refer to the applicable agreement for further details.

// PROGRAM		"Quartus Prime"
// VERSION		"Version 17.1.0 Build 590 10/25/2017 SJ Lite Edition"
// CREATED		"Fri Jan 24 23:12:54 2020"

module OnChipSerialIO(
	Reset_L,
	Clock_50Mhz,
	RS232_RxData,
	WE_L,
	GPS_RxData,
	BlueTooth_RxData,
	IOSelect_H,
	ByteSelect_L,
	TouchScreen_RxData,
	Address,
	DataIn,
	RS232_TxData,
	GPS_TxData,
	BlueTooth_TxData,
	TouchScreen_TxData,
	IRQ_H,
	DataOut
);


input wire	Reset_L;
input wire	Clock_50Mhz;
input wire	RS232_RxData;
input wire	WE_L;
input wire	GPS_RxData;
input wire	BlueTooth_RxData;
input wire	IOSelect_H;
input wire	ByteSelect_L;
input wire	TouchScreen_RxData;
input wire	[15:0] Address;
input wire	[7:0] DataIn;
output wire	RS232_TxData;
output wire	GPS_TxData;
output wire	BlueTooth_TxData;
output wire	TouchScreen_TxData;
output wire	IRQ_H;
output wire	[7:0] DataOut;

wire	SYNTHESIZED_WIRE_36;
wire	SYNTHESIZED_WIRE_37;
wire	SYNTHESIZED_WIRE_38;
wire	SYNTHESIZED_WIRE_39;
wire	SYNTHESIZED_WIRE_40;
wire	SYNTHESIZED_WIRE_41;
wire	SYNTHESIZED_WIRE_42;
wire	SYNTHESIZED_WIRE_43;
wire	SYNTHESIZED_WIRE_24;
wire	SYNTHESIZED_WIRE_25;
wire	SYNTHESIZED_WIRE_26;
wire	SYNTHESIZED_WIRE_27;
wire	SYNTHESIZED_WIRE_44;
wire	SYNTHESIZED_WIRE_45;

assign	SYNTHESIZED_WIRE_39 = 0;
assign	SYNTHESIZED_WIRE_41 = 0;
assign	SYNTHESIZED_WIRE_43 = 0;
assign	SYNTHESIZED_WIRE_45 = 0;




SerialIODecoder	b2v_inst(
	.IOSelect_H(IOSelect_H),
	.ByteSelect_L(ByteSelect_L),
	.Address(Address),
	.RS232_Port_Enable(SYNTHESIZED_WIRE_42),
	.GPS_Port_Enable(SYNTHESIZED_WIRE_44),
	.Bluetooth_Port_Enable(SYNTHESIZED_WIRE_37),
	.TouchScreen_Port_Enable(SYNTHESIZED_WIRE_40));


gh_uart_16550_wb_wrapper	b2v_inst10(
	.wb_clk_i(Clock_50Mhz),
	.wb_rst_i(SYNTHESIZED_WIRE_36),
	.wb_stb_i(SYNTHESIZED_WIRE_37),
	.wb_cyc_i(SYNTHESIZED_WIRE_37),
	.wb_we_i(SYNTHESIZED_WIRE_38),
	.BR_clk(Clock_50Mhz),
	.sRX(BlueTooth_RxData),
	.CTSn(SYNTHESIZED_WIRE_39),
	.DSRn(SYNTHESIZED_WIRE_39),
	.RIn(SYNTHESIZED_WIRE_39),
	.DCDn(SYNTHESIZED_WIRE_39),
	.wb_adr_i(Address[3:1]),
	.wb_dat_i(DataIn),
	
	.sTX(BlueTooth_TxData),
	
	
	
	
	
	
	.IRQ(SYNTHESIZED_WIRE_25),
	
	.wb_dat_o(DataOut));


gh_uart_16550_wb_wrapper	b2v_inst11(
	.wb_clk_i(Clock_50Mhz),
	.wb_rst_i(SYNTHESIZED_WIRE_36),
	.wb_stb_i(SYNTHESIZED_WIRE_40),
	.wb_cyc_i(SYNTHESIZED_WIRE_40),
	.wb_we_i(SYNTHESIZED_WIRE_38),
	.BR_clk(Clock_50Mhz),
	.sRX(TouchScreen_RxData),
	.CTSn(SYNTHESIZED_WIRE_41),
	.DSRn(SYNTHESIZED_WIRE_41),
	.RIn(SYNTHESIZED_WIRE_41),
	.DCDn(SYNTHESIZED_WIRE_41),
	.wb_adr_i(Address[3:1]),
	.wb_dat_i(DataIn),
	
	.sTX(TouchScreen_TxData),
	
	
	
	
	
	
	.IRQ(SYNTHESIZED_WIRE_26),
	
	.wb_dat_o(DataOut));





gh_uart_16550_wb_wrapper	b2v_inst3(
	.wb_clk_i(Clock_50Mhz),
	.wb_rst_i(SYNTHESIZED_WIRE_36),
	.wb_stb_i(SYNTHESIZED_WIRE_42),
	.wb_cyc_i(SYNTHESIZED_WIRE_42),
	.wb_we_i(SYNTHESIZED_WIRE_38),
	.BR_clk(Clock_50Mhz),
	.sRX(RS232_RxData),
	.CTSn(SYNTHESIZED_WIRE_43),
	.DSRn(SYNTHESIZED_WIRE_43),
	.RIn(SYNTHESIZED_WIRE_43),
	.DCDn(SYNTHESIZED_WIRE_43),
	.wb_adr_i(Address[3:1]),
	.wb_dat_i(DataIn),
	
	.sTX(RS232_TxData),
	
	
	
	
	
	
	.IRQ(SYNTHESIZED_WIRE_24),
	
	.wb_dat_o(DataOut));

assign	SYNTHESIZED_WIRE_38 =  ~WE_L;

assign	SYNTHESIZED_WIRE_36 =  ~Reset_L;


assign	IRQ_H = SYNTHESIZED_WIRE_24 | SYNTHESIZED_WIRE_25 | SYNTHESIZED_WIRE_26 | SYNTHESIZED_WIRE_27;


gh_uart_16550_wb_wrapper	b2v_inst9(
	.wb_clk_i(Clock_50Mhz),
	.wb_rst_i(SYNTHESIZED_WIRE_36),
	.wb_stb_i(SYNTHESIZED_WIRE_44),
	.wb_cyc_i(SYNTHESIZED_WIRE_44),
	.wb_we_i(SYNTHESIZED_WIRE_38),
	.BR_clk(Clock_50Mhz),
	.sRX(GPS_RxData),
	.CTSn(SYNTHESIZED_WIRE_45),
	.DSRn(SYNTHESIZED_WIRE_45),
	.RIn(SYNTHESIZED_WIRE_45),
	.DCDn(SYNTHESIZED_WIRE_45),
	.wb_adr_i(Address[3:1]),
	.wb_dat_i(DataIn),
	
	.sTX(GPS_TxData),
	
	
	
	
	
	
	.IRQ(SYNTHESIZED_WIRE_27),
	
	.wb_dat_o(DataOut));


endmodule
