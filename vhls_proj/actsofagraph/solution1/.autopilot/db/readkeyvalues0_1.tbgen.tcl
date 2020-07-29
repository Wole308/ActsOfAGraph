set moduleName readkeyvalues0_1
set isTopModule 0
set isTaskLevelControl 1
set isCombinational 0
set isDatapathOnly 0
set isFreeRunPipelineModule 0
set isPipelined 0
set pipeline_type none
set FunctionProtocol ap_ctrl_hs
set isOneStateSeq 0
set ProfileFlag 0
set StallSigGenFlag 0
set isEnableWaveformDebug 1
set C_modelName {readkeyvalues0.1}
set C_modelType { void 0 }
set C_modelArgList {
	{ enable int 1 regular  }
	{ kvdram_V int 512 regular {axi_master 0}  }
	{ kvdram_V_offset int 26 regular  }
	{ buffer0_V int 512 regular {array 512 { 0 3 } 0 1 }  }
	{ buffer1_V int 512 regular {array 512 { 0 3 } 0 1 }  }
	{ buffer2_V int 512 regular {array 512 { 0 3 } 0 1 }  }
	{ buffer3_V int 512 regular {array 512 { 0 3 } 0 1 }  }
	{ baseaddress int 25 regular  }
	{ offset_kvs int 32 regular  }
	{ travstate_i_kvs int 32 regular  }
	{ travstate_end_kvs int 30 regular  }
}
set C_modelArgMapList {[ 
	{ "Name" : "enable", "interface" : "wire", "bitwidth" : 1, "direction" : "READONLY"} , 
 	{ "Name" : "kvdram_V", "interface" : "axi_master", "bitwidth" : 512, "direction" : "READONLY"} , 
 	{ "Name" : "kvdram_V_offset", "interface" : "wire", "bitwidth" : 26, "direction" : "READONLY"} , 
 	{ "Name" : "buffer0_V", "interface" : "memory", "bitwidth" : 512, "direction" : "WRITEONLY"} , 
 	{ "Name" : "buffer1_V", "interface" : "memory", "bitwidth" : 512, "direction" : "WRITEONLY"} , 
 	{ "Name" : "buffer2_V", "interface" : "memory", "bitwidth" : 512, "direction" : "WRITEONLY"} , 
 	{ "Name" : "buffer3_V", "interface" : "memory", "bitwidth" : 512, "direction" : "WRITEONLY"} , 
 	{ "Name" : "baseaddress", "interface" : "wire", "bitwidth" : 25, "direction" : "READONLY"} , 
 	{ "Name" : "offset_kvs", "interface" : "wire", "bitwidth" : 32, "direction" : "READONLY"} , 
 	{ "Name" : "travstate_i_kvs", "interface" : "wire", "bitwidth" : 32, "direction" : "READONLY"} , 
 	{ "Name" : "travstate_end_kvs", "interface" : "wire", "bitwidth" : 30, "direction" : "READONLY"} ]}
# RTL Port declarations: 
set portNum 73
set portList { 
	{ ap_clk sc_in sc_logic 1 clock -1 } 
	{ ap_rst sc_in sc_logic 1 reset -1 active_high_sync } 
	{ ap_start sc_in sc_logic 1 start -1 } 
	{ ap_done sc_out sc_logic 1 predone -1 } 
	{ ap_idle sc_out sc_logic 1 done -1 } 
	{ ap_ready sc_out sc_logic 1 ready -1 } 
	{ enable sc_in sc_lv 1 signal 0 } 
	{ m_axi_kvdram_V_AWVALID sc_out sc_logic 1 signal 1 } 
	{ m_axi_kvdram_V_AWREADY sc_in sc_logic 1 signal 1 } 
	{ m_axi_kvdram_V_AWADDR sc_out sc_lv 32 signal 1 } 
	{ m_axi_kvdram_V_AWID sc_out sc_lv 1 signal 1 } 
	{ m_axi_kvdram_V_AWLEN sc_out sc_lv 32 signal 1 } 
	{ m_axi_kvdram_V_AWSIZE sc_out sc_lv 3 signal 1 } 
	{ m_axi_kvdram_V_AWBURST sc_out sc_lv 2 signal 1 } 
	{ m_axi_kvdram_V_AWLOCK sc_out sc_lv 2 signal 1 } 
	{ m_axi_kvdram_V_AWCACHE sc_out sc_lv 4 signal 1 } 
	{ m_axi_kvdram_V_AWPROT sc_out sc_lv 3 signal 1 } 
	{ m_axi_kvdram_V_AWQOS sc_out sc_lv 4 signal 1 } 
	{ m_axi_kvdram_V_AWREGION sc_out sc_lv 4 signal 1 } 
	{ m_axi_kvdram_V_AWUSER sc_out sc_lv 1 signal 1 } 
	{ m_axi_kvdram_V_WVALID sc_out sc_logic 1 signal 1 } 
	{ m_axi_kvdram_V_WREADY sc_in sc_logic 1 signal 1 } 
	{ m_axi_kvdram_V_WDATA sc_out sc_lv 512 signal 1 } 
	{ m_axi_kvdram_V_WSTRB sc_out sc_lv 64 signal 1 } 
	{ m_axi_kvdram_V_WLAST sc_out sc_logic 1 signal 1 } 
	{ m_axi_kvdram_V_WID sc_out sc_lv 1 signal 1 } 
	{ m_axi_kvdram_V_WUSER sc_out sc_lv 1 signal 1 } 
	{ m_axi_kvdram_V_ARVALID sc_out sc_logic 1 signal 1 } 
	{ m_axi_kvdram_V_ARREADY sc_in sc_logic 1 signal 1 } 
	{ m_axi_kvdram_V_ARADDR sc_out sc_lv 32 signal 1 } 
	{ m_axi_kvdram_V_ARID sc_out sc_lv 1 signal 1 } 
	{ m_axi_kvdram_V_ARLEN sc_out sc_lv 32 signal 1 } 
	{ m_axi_kvdram_V_ARSIZE sc_out sc_lv 3 signal 1 } 
	{ m_axi_kvdram_V_ARBURST sc_out sc_lv 2 signal 1 } 
	{ m_axi_kvdram_V_ARLOCK sc_out sc_lv 2 signal 1 } 
	{ m_axi_kvdram_V_ARCACHE sc_out sc_lv 4 signal 1 } 
	{ m_axi_kvdram_V_ARPROT sc_out sc_lv 3 signal 1 } 
	{ m_axi_kvdram_V_ARQOS sc_out sc_lv 4 signal 1 } 
	{ m_axi_kvdram_V_ARREGION sc_out sc_lv 4 signal 1 } 
	{ m_axi_kvdram_V_ARUSER sc_out sc_lv 1 signal 1 } 
	{ m_axi_kvdram_V_RVALID sc_in sc_logic 1 signal 1 } 
	{ m_axi_kvdram_V_RREADY sc_out sc_logic 1 signal 1 } 
	{ m_axi_kvdram_V_RDATA sc_in sc_lv 512 signal 1 } 
	{ m_axi_kvdram_V_RLAST sc_in sc_logic 1 signal 1 } 
	{ m_axi_kvdram_V_RID sc_in sc_lv 1 signal 1 } 
	{ m_axi_kvdram_V_RUSER sc_in sc_lv 1 signal 1 } 
	{ m_axi_kvdram_V_RRESP sc_in sc_lv 2 signal 1 } 
	{ m_axi_kvdram_V_BVALID sc_in sc_logic 1 signal 1 } 
	{ m_axi_kvdram_V_BREADY sc_out sc_logic 1 signal 1 } 
	{ m_axi_kvdram_V_BRESP sc_in sc_lv 2 signal 1 } 
	{ m_axi_kvdram_V_BID sc_in sc_lv 1 signal 1 } 
	{ m_axi_kvdram_V_BUSER sc_in sc_lv 1 signal 1 } 
	{ kvdram_V_offset sc_in sc_lv 26 signal 2 } 
	{ buffer0_V_address0 sc_out sc_lv 9 signal 3 } 
	{ buffer0_V_ce0 sc_out sc_logic 1 signal 3 } 
	{ buffer0_V_we0 sc_out sc_logic 1 signal 3 } 
	{ buffer0_V_d0 sc_out sc_lv 512 signal 3 } 
	{ buffer1_V_address0 sc_out sc_lv 9 signal 4 } 
	{ buffer1_V_ce0 sc_out sc_logic 1 signal 4 } 
	{ buffer1_V_we0 sc_out sc_logic 1 signal 4 } 
	{ buffer1_V_d0 sc_out sc_lv 512 signal 4 } 
	{ buffer2_V_address0 sc_out sc_lv 9 signal 5 } 
	{ buffer2_V_ce0 sc_out sc_logic 1 signal 5 } 
	{ buffer2_V_we0 sc_out sc_logic 1 signal 5 } 
	{ buffer2_V_d0 sc_out sc_lv 512 signal 5 } 
	{ buffer3_V_address0 sc_out sc_lv 9 signal 6 } 
	{ buffer3_V_ce0 sc_out sc_logic 1 signal 6 } 
	{ buffer3_V_we0 sc_out sc_logic 1 signal 6 } 
	{ buffer3_V_d0 sc_out sc_lv 512 signal 6 } 
	{ baseaddress sc_in sc_lv 25 signal 7 } 
	{ offset_kvs sc_in sc_lv 32 signal 8 } 
	{ travstate_i_kvs sc_in sc_lv 32 signal 9 } 
	{ travstate_end_kvs sc_in sc_lv 30 signal 10 } 
}
set NewPortList {[ 
	{ "name": "ap_clk", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "clock", "bundle":{"name": "ap_clk", "role": "default" }} , 
 	{ "name": "ap_rst", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "reset", "bundle":{"name": "ap_rst", "role": "default" }} , 
 	{ "name": "ap_start", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "start", "bundle":{"name": "ap_start", "role": "default" }} , 
 	{ "name": "ap_done", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "predone", "bundle":{"name": "ap_done", "role": "default" }} , 
 	{ "name": "ap_idle", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "done", "bundle":{"name": "ap_idle", "role": "default" }} , 
 	{ "name": "ap_ready", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "ready", "bundle":{"name": "ap_ready", "role": "default" }} , 
 	{ "name": "enable", "direction": "in", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "enable", "role": "default" }} , 
 	{ "name": "m_axi_kvdram_V_AWVALID", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "kvdram_V", "role": "AWVALID" }} , 
 	{ "name": "m_axi_kvdram_V_AWREADY", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "kvdram_V", "role": "AWREADY" }} , 
 	{ "name": "m_axi_kvdram_V_AWADDR", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "kvdram_V", "role": "AWADDR" }} , 
 	{ "name": "m_axi_kvdram_V_AWID", "direction": "out", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "kvdram_V", "role": "AWID" }} , 
 	{ "name": "m_axi_kvdram_V_AWLEN", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "kvdram_V", "role": "AWLEN" }} , 
 	{ "name": "m_axi_kvdram_V_AWSIZE", "direction": "out", "datatype": "sc_lv", "bitwidth":3, "type": "signal", "bundle":{"name": "kvdram_V", "role": "AWSIZE" }} , 
 	{ "name": "m_axi_kvdram_V_AWBURST", "direction": "out", "datatype": "sc_lv", "bitwidth":2, "type": "signal", "bundle":{"name": "kvdram_V", "role": "AWBURST" }} , 
 	{ "name": "m_axi_kvdram_V_AWLOCK", "direction": "out", "datatype": "sc_lv", "bitwidth":2, "type": "signal", "bundle":{"name": "kvdram_V", "role": "AWLOCK" }} , 
 	{ "name": "m_axi_kvdram_V_AWCACHE", "direction": "out", "datatype": "sc_lv", "bitwidth":4, "type": "signal", "bundle":{"name": "kvdram_V", "role": "AWCACHE" }} , 
 	{ "name": "m_axi_kvdram_V_AWPROT", "direction": "out", "datatype": "sc_lv", "bitwidth":3, "type": "signal", "bundle":{"name": "kvdram_V", "role": "AWPROT" }} , 
 	{ "name": "m_axi_kvdram_V_AWQOS", "direction": "out", "datatype": "sc_lv", "bitwidth":4, "type": "signal", "bundle":{"name": "kvdram_V", "role": "AWQOS" }} , 
 	{ "name": "m_axi_kvdram_V_AWREGION", "direction": "out", "datatype": "sc_lv", "bitwidth":4, "type": "signal", "bundle":{"name": "kvdram_V", "role": "AWREGION" }} , 
 	{ "name": "m_axi_kvdram_V_AWUSER", "direction": "out", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "kvdram_V", "role": "AWUSER" }} , 
 	{ "name": "m_axi_kvdram_V_WVALID", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "kvdram_V", "role": "WVALID" }} , 
 	{ "name": "m_axi_kvdram_V_WREADY", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "kvdram_V", "role": "WREADY" }} , 
 	{ "name": "m_axi_kvdram_V_WDATA", "direction": "out", "datatype": "sc_lv", "bitwidth":512, "type": "signal", "bundle":{"name": "kvdram_V", "role": "WDATA" }} , 
 	{ "name": "m_axi_kvdram_V_WSTRB", "direction": "out", "datatype": "sc_lv", "bitwidth":64, "type": "signal", "bundle":{"name": "kvdram_V", "role": "WSTRB" }} , 
 	{ "name": "m_axi_kvdram_V_WLAST", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "kvdram_V", "role": "WLAST" }} , 
 	{ "name": "m_axi_kvdram_V_WID", "direction": "out", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "kvdram_V", "role": "WID" }} , 
 	{ "name": "m_axi_kvdram_V_WUSER", "direction": "out", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "kvdram_V", "role": "WUSER" }} , 
 	{ "name": "m_axi_kvdram_V_ARVALID", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "kvdram_V", "role": "ARVALID" }} , 
 	{ "name": "m_axi_kvdram_V_ARREADY", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "kvdram_V", "role": "ARREADY" }} , 
 	{ "name": "m_axi_kvdram_V_ARADDR", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "kvdram_V", "role": "ARADDR" }} , 
 	{ "name": "m_axi_kvdram_V_ARID", "direction": "out", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "kvdram_V", "role": "ARID" }} , 
 	{ "name": "m_axi_kvdram_V_ARLEN", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "kvdram_V", "role": "ARLEN" }} , 
 	{ "name": "m_axi_kvdram_V_ARSIZE", "direction": "out", "datatype": "sc_lv", "bitwidth":3, "type": "signal", "bundle":{"name": "kvdram_V", "role": "ARSIZE" }} , 
 	{ "name": "m_axi_kvdram_V_ARBURST", "direction": "out", "datatype": "sc_lv", "bitwidth":2, "type": "signal", "bundle":{"name": "kvdram_V", "role": "ARBURST" }} , 
 	{ "name": "m_axi_kvdram_V_ARLOCK", "direction": "out", "datatype": "sc_lv", "bitwidth":2, "type": "signal", "bundle":{"name": "kvdram_V", "role": "ARLOCK" }} , 
 	{ "name": "m_axi_kvdram_V_ARCACHE", "direction": "out", "datatype": "sc_lv", "bitwidth":4, "type": "signal", "bundle":{"name": "kvdram_V", "role": "ARCACHE" }} , 
 	{ "name": "m_axi_kvdram_V_ARPROT", "direction": "out", "datatype": "sc_lv", "bitwidth":3, "type": "signal", "bundle":{"name": "kvdram_V", "role": "ARPROT" }} , 
 	{ "name": "m_axi_kvdram_V_ARQOS", "direction": "out", "datatype": "sc_lv", "bitwidth":4, "type": "signal", "bundle":{"name": "kvdram_V", "role": "ARQOS" }} , 
 	{ "name": "m_axi_kvdram_V_ARREGION", "direction": "out", "datatype": "sc_lv", "bitwidth":4, "type": "signal", "bundle":{"name": "kvdram_V", "role": "ARREGION" }} , 
 	{ "name": "m_axi_kvdram_V_ARUSER", "direction": "out", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "kvdram_V", "role": "ARUSER" }} , 
 	{ "name": "m_axi_kvdram_V_RVALID", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "kvdram_V", "role": "RVALID" }} , 
 	{ "name": "m_axi_kvdram_V_RREADY", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "kvdram_V", "role": "RREADY" }} , 
 	{ "name": "m_axi_kvdram_V_RDATA", "direction": "in", "datatype": "sc_lv", "bitwidth":512, "type": "signal", "bundle":{"name": "kvdram_V", "role": "RDATA" }} , 
 	{ "name": "m_axi_kvdram_V_RLAST", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "kvdram_V", "role": "RLAST" }} , 
 	{ "name": "m_axi_kvdram_V_RID", "direction": "in", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "kvdram_V", "role": "RID" }} , 
 	{ "name": "m_axi_kvdram_V_RUSER", "direction": "in", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "kvdram_V", "role": "RUSER" }} , 
 	{ "name": "m_axi_kvdram_V_RRESP", "direction": "in", "datatype": "sc_lv", "bitwidth":2, "type": "signal", "bundle":{"name": "kvdram_V", "role": "RRESP" }} , 
 	{ "name": "m_axi_kvdram_V_BVALID", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "kvdram_V", "role": "BVALID" }} , 
 	{ "name": "m_axi_kvdram_V_BREADY", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "kvdram_V", "role": "BREADY" }} , 
 	{ "name": "m_axi_kvdram_V_BRESP", "direction": "in", "datatype": "sc_lv", "bitwidth":2, "type": "signal", "bundle":{"name": "kvdram_V", "role": "BRESP" }} , 
 	{ "name": "m_axi_kvdram_V_BID", "direction": "in", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "kvdram_V", "role": "BID" }} , 
 	{ "name": "m_axi_kvdram_V_BUSER", "direction": "in", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "kvdram_V", "role": "BUSER" }} , 
 	{ "name": "kvdram_V_offset", "direction": "in", "datatype": "sc_lv", "bitwidth":26, "type": "signal", "bundle":{"name": "kvdram_V_offset", "role": "default" }} , 
 	{ "name": "buffer0_V_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":9, "type": "signal", "bundle":{"name": "buffer0_V", "role": "address0" }} , 
 	{ "name": "buffer0_V_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "buffer0_V", "role": "ce0" }} , 
 	{ "name": "buffer0_V_we0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "buffer0_V", "role": "we0" }} , 
 	{ "name": "buffer0_V_d0", "direction": "out", "datatype": "sc_lv", "bitwidth":512, "type": "signal", "bundle":{"name": "buffer0_V", "role": "d0" }} , 
 	{ "name": "buffer1_V_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":9, "type": "signal", "bundle":{"name": "buffer1_V", "role": "address0" }} , 
 	{ "name": "buffer1_V_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "buffer1_V", "role": "ce0" }} , 
 	{ "name": "buffer1_V_we0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "buffer1_V", "role": "we0" }} , 
 	{ "name": "buffer1_V_d0", "direction": "out", "datatype": "sc_lv", "bitwidth":512, "type": "signal", "bundle":{"name": "buffer1_V", "role": "d0" }} , 
 	{ "name": "buffer2_V_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":9, "type": "signal", "bundle":{"name": "buffer2_V", "role": "address0" }} , 
 	{ "name": "buffer2_V_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "buffer2_V", "role": "ce0" }} , 
 	{ "name": "buffer2_V_we0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "buffer2_V", "role": "we0" }} , 
 	{ "name": "buffer2_V_d0", "direction": "out", "datatype": "sc_lv", "bitwidth":512, "type": "signal", "bundle":{"name": "buffer2_V", "role": "d0" }} , 
 	{ "name": "buffer3_V_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":9, "type": "signal", "bundle":{"name": "buffer3_V", "role": "address0" }} , 
 	{ "name": "buffer3_V_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "buffer3_V", "role": "ce0" }} , 
 	{ "name": "buffer3_V_we0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "buffer3_V", "role": "we0" }} , 
 	{ "name": "buffer3_V_d0", "direction": "out", "datatype": "sc_lv", "bitwidth":512, "type": "signal", "bundle":{"name": "buffer3_V", "role": "d0" }} , 
 	{ "name": "baseaddress", "direction": "in", "datatype": "sc_lv", "bitwidth":25, "type": "signal", "bundle":{"name": "baseaddress", "role": "default" }} , 
 	{ "name": "offset_kvs", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "offset_kvs", "role": "default" }} , 
 	{ "name": "travstate_i_kvs", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "travstate_i_kvs", "role": "default" }} , 
 	{ "name": "travstate_end_kvs", "direction": "in", "datatype": "sc_lv", "bitwidth":30, "type": "signal", "bundle":{"name": "travstate_end_kvs", "role": "default" }}  ]}

set RtlHierarchyInfo {[
	{"ID" : "0", "Level" : "0", "Path" : "`AUTOTB_DUT_INST", "Parent" : "", "Child" : ["1"],
		"CDFG" : "readkeyvalues0_1",
		"Protocol" : "ap_ctrl_hs",
		"ControlExist" : "1", "ap_start" : "1", "ap_ready" : "1", "ap_done" : "1", "ap_continue" : "0", "ap_idle" : "1",
		"Pipeline" : "None", "UnalignedPipeline" : "0", "RewindPipeline" : "0", "ProcessNetwork" : "0",
		"II" : "0",
		"VariableLatency" : "1", "ExactLatency" : "-1", "EstimateLatencyMin" : "1", "EstimateLatencyMax" : "2096",
		"Combinational" : "0",
		"Datapath" : "0",
		"ClockEnable" : "0",
		"HasSubDataflow" : "0",
		"InDataflowNetwork" : "0",
		"HasNonBlockingOperation" : "0",
		"WaitState" : [
			{"State" : "ap_ST_fsm_state3", "FSM" : "ap_CS_fsm", "SubInstance" : "grp_readkeyvalues0_158_fu_94"},
			{"State" : "ap_ST_fsm_state5", "FSM" : "ap_CS_fsm", "SubInstance" : "grp_readkeyvalues0_158_fu_94"},
			{"State" : "ap_ST_fsm_state7", "FSM" : "ap_CS_fsm", "SubInstance" : "grp_readkeyvalues0_158_fu_94"},
			{"State" : "ap_ST_fsm_state9", "FSM" : "ap_CS_fsm", "SubInstance" : "grp_readkeyvalues0_158_fu_94"}],
		"Port" : [
			{"Name" : "enable", "Type" : "None", "Direction" : "I"},
			{"Name" : "kvdram_V", "Type" : "MAXI", "Direction" : "I",
				"SubConnect" : [
					{"ID" : "1", "SubInstance" : "grp_readkeyvalues0_158_fu_94", "Port" : "kvdram_V"}]},
			{"Name" : "kvdram_V_offset", "Type" : "None", "Direction" : "I"},
			{"Name" : "buffer0_V", "Type" : "Memory", "Direction" : "O",
				"SubConnect" : [
					{"ID" : "1", "SubInstance" : "grp_readkeyvalues0_158_fu_94", "Port" : "buffer_V"}]},
			{"Name" : "buffer1_V", "Type" : "Memory", "Direction" : "O",
				"SubConnect" : [
					{"ID" : "1", "SubInstance" : "grp_readkeyvalues0_158_fu_94", "Port" : "buffer_V"}]},
			{"Name" : "buffer2_V", "Type" : "Memory", "Direction" : "O",
				"SubConnect" : [
					{"ID" : "1", "SubInstance" : "grp_readkeyvalues0_158_fu_94", "Port" : "buffer_V"}]},
			{"Name" : "buffer3_V", "Type" : "Memory", "Direction" : "O",
				"SubConnect" : [
					{"ID" : "1", "SubInstance" : "grp_readkeyvalues0_158_fu_94", "Port" : "buffer_V"}]},
			{"Name" : "baseaddress", "Type" : "None", "Direction" : "I"},
			{"Name" : "offset_kvs", "Type" : "None", "Direction" : "I"},
			{"Name" : "travstate_i_kvs", "Type" : "None", "Direction" : "I"},
			{"Name" : "travstate_end_kvs", "Type" : "None", "Direction" : "I"}]},
	{"ID" : "1", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.grp_readkeyvalues0_158_fu_94", "Parent" : "0",
		"CDFG" : "readkeyvalues0_158",
		"Protocol" : "ap_ctrl_hs",
		"ControlExist" : "1", "ap_start" : "1", "ap_ready" : "1", "ap_done" : "1", "ap_continue" : "0", "ap_idle" : "1",
		"Pipeline" : "None", "UnalignedPipeline" : "0", "RewindPipeline" : "0", "ProcessNetwork" : "0",
		"II" : "0",
		"VariableLatency" : "1", "ExactLatency" : "-1", "EstimateLatencyMin" : "9", "EstimateLatencyMax" : "522",
		"Combinational" : "0",
		"Datapath" : "0",
		"ClockEnable" : "0",
		"HasSubDataflow" : "0",
		"InDataflowNetwork" : "0",
		"HasNonBlockingOperation" : "0",
		"Port" : [
			{"Name" : "kvdram_V", "Type" : "MAXI", "Direction" : "I",
				"BlockSignal" : [
					{"Name" : "kvdram_V_blk_n_AR", "Type" : "RtlSignal"},
					{"Name" : "kvdram_V_blk_n_R", "Type" : "RtlSignal"}]},
			{"Name" : "kvdram_V_offset", "Type" : "None", "Direction" : "I"},
			{"Name" : "buffer_V", "Type" : "Memory", "Direction" : "O"},
			{"Name" : "baseaddress", "Type" : "None", "Direction" : "I"},
			{"Name" : "offset_kvs", "Type" : "None", "Direction" : "I"},
			{"Name" : "size_kvs", "Type" : "None", "Direction" : "I"}]}]}


set ArgLastReadFirstWriteLatency {
	readkeyvalues0_1 {
		enable {Type I LastRead 0 FirstWrite -1}
		kvdram_V {Type I LastRead 9 FirstWrite -1}
		kvdram_V_offset {Type I LastRead 0 FirstWrite -1}
		buffer0_V {Type O LastRead -1 FirstWrite 10}
		buffer1_V {Type O LastRead -1 FirstWrite 10}
		buffer2_V {Type O LastRead -1 FirstWrite 10}
		buffer3_V {Type O LastRead -1 FirstWrite 10}
		baseaddress {Type I LastRead 0 FirstWrite -1}
		offset_kvs {Type I LastRead 0 FirstWrite -1}
		travstate_i_kvs {Type I LastRead 0 FirstWrite -1}
		travstate_end_kvs {Type I LastRead 0 FirstWrite -1}}
	readkeyvalues0_158 {
		kvdram_V {Type I LastRead 9 FirstWrite -1}
		kvdram_V_offset {Type I LastRead 0 FirstWrite -1}
		buffer_V {Type O LastRead -1 FirstWrite 10}
		baseaddress {Type I LastRead 0 FirstWrite -1}
		offset_kvs {Type I LastRead 0 FirstWrite -1}
		size_kvs {Type I LastRead 1 FirstWrite -1}}}

set hasDtUnsupportedChannel 0

set PerformanceInfo {[
	{"Name" : "Latency", "Min" : "1", "Max" : "2096"}
	, {"Name" : "Interval", "Min" : "1", "Max" : "2096"}
]}

set PipelineEnableSignalInfo {[
]}

set Spec2ImplPortList { 
	enable { ap_none {  { enable in_data 0 1 } } }
	kvdram_V { m_axi {  { m_axi_kvdram_V_AWVALID VALID 1 1 }  { m_axi_kvdram_V_AWREADY READY 0 1 }  { m_axi_kvdram_V_AWADDR ADDR 1 32 }  { m_axi_kvdram_V_AWID ID 1 1 }  { m_axi_kvdram_V_AWLEN LEN 1 32 }  { m_axi_kvdram_V_AWSIZE SIZE 1 3 }  { m_axi_kvdram_V_AWBURST BURST 1 2 }  { m_axi_kvdram_V_AWLOCK LOCK 1 2 }  { m_axi_kvdram_V_AWCACHE CACHE 1 4 }  { m_axi_kvdram_V_AWPROT PROT 1 3 }  { m_axi_kvdram_V_AWQOS QOS 1 4 }  { m_axi_kvdram_V_AWREGION REGION 1 4 }  { m_axi_kvdram_V_AWUSER USER 1 1 }  { m_axi_kvdram_V_WVALID VALID 1 1 }  { m_axi_kvdram_V_WREADY READY 0 1 }  { m_axi_kvdram_V_WDATA DATA 1 512 }  { m_axi_kvdram_V_WSTRB STRB 1 64 }  { m_axi_kvdram_V_WLAST LAST 1 1 }  { m_axi_kvdram_V_WID ID 1 1 }  { m_axi_kvdram_V_WUSER USER 1 1 }  { m_axi_kvdram_V_ARVALID VALID 1 1 }  { m_axi_kvdram_V_ARREADY READY 0 1 }  { m_axi_kvdram_V_ARADDR ADDR 1 32 }  { m_axi_kvdram_V_ARID ID 1 1 }  { m_axi_kvdram_V_ARLEN LEN 1 32 }  { m_axi_kvdram_V_ARSIZE SIZE 1 3 }  { m_axi_kvdram_V_ARBURST BURST 1 2 }  { m_axi_kvdram_V_ARLOCK LOCK 1 2 }  { m_axi_kvdram_V_ARCACHE CACHE 1 4 }  { m_axi_kvdram_V_ARPROT PROT 1 3 }  { m_axi_kvdram_V_ARQOS QOS 1 4 }  { m_axi_kvdram_V_ARREGION REGION 1 4 }  { m_axi_kvdram_V_ARUSER USER 1 1 }  { m_axi_kvdram_V_RVALID VALID 0 1 }  { m_axi_kvdram_V_RREADY READY 1 1 }  { m_axi_kvdram_V_RDATA DATA 0 512 }  { m_axi_kvdram_V_RLAST LAST 0 1 }  { m_axi_kvdram_V_RID ID 0 1 }  { m_axi_kvdram_V_RUSER USER 0 1 }  { m_axi_kvdram_V_RRESP RESP 0 2 }  { m_axi_kvdram_V_BVALID VALID 0 1 }  { m_axi_kvdram_V_BREADY READY 1 1 }  { m_axi_kvdram_V_BRESP RESP 0 2 }  { m_axi_kvdram_V_BID ID 0 1 }  { m_axi_kvdram_V_BUSER USER 0 1 } } }
	kvdram_V_offset { ap_none {  { kvdram_V_offset in_data 0 26 } } }
	buffer0_V { ap_memory {  { buffer0_V_address0 mem_address 1 9 }  { buffer0_V_ce0 mem_ce 1 1 }  { buffer0_V_we0 mem_we 1 1 }  { buffer0_V_d0 mem_din 1 512 } } }
	buffer1_V { ap_memory {  { buffer1_V_address0 mem_address 1 9 }  { buffer1_V_ce0 mem_ce 1 1 }  { buffer1_V_we0 mem_we 1 1 }  { buffer1_V_d0 mem_din 1 512 } } }
	buffer2_V { ap_memory {  { buffer2_V_address0 mem_address 1 9 }  { buffer2_V_ce0 mem_ce 1 1 }  { buffer2_V_we0 mem_we 1 1 }  { buffer2_V_d0 mem_din 1 512 } } }
	buffer3_V { ap_memory {  { buffer3_V_address0 mem_address 1 9 }  { buffer3_V_ce0 mem_ce 1 1 }  { buffer3_V_we0 mem_we 1 1 }  { buffer3_V_d0 mem_din 1 512 } } }
	baseaddress { ap_none {  { baseaddress in_data 0 25 } } }
	offset_kvs { ap_none {  { offset_kvs in_data 0 32 } } }
	travstate_i_kvs { ap_none {  { travstate_i_kvs in_data 0 32 } } }
	travstate_end_kvs { ap_none {  { travstate_end_kvs in_data 0 30 } } }
}
set moduleName readkeyvalues0_1
set isTopModule 0
set isTaskLevelControl 1
set isCombinational 0
set isDatapathOnly 0
set isFreeRunPipelineModule 0
set isPipelined 0
set pipeline_type none
set FunctionProtocol ap_ctrl_hs
set isOneStateSeq 0
set ProfileFlag 0
set StallSigGenFlag 0
set isEnableWaveformDebug 1
set C_modelName {readkeyvalues0.1}
set C_modelType { void 0 }
set C_modelArgList {
	{ enable int 1 regular  }
	{ kvdram_V int 512 regular {axi_master 0}  }
	{ kvdram_V_offset int 26 regular  }
	{ buffer0_V int 512 regular {array 512 { 0 3 } 0 1 }  }
	{ buffer1_V int 512 regular {array 512 { 0 3 } 0 1 }  }
	{ buffer2_V int 512 regular {array 512 { 0 3 } 0 1 }  }
	{ buffer3_V int 512 regular {array 512 { 0 3 } 0 1 }  }
	{ baseaddress int 25 regular  }
	{ offset_kvs int 32 regular  }
	{ travstate_i_kvs int 32 regular  }
	{ travstate_end_kvs int 30 regular  }
}
set C_modelArgMapList {[ 
	{ "Name" : "enable", "interface" : "wire", "bitwidth" : 1, "direction" : "READONLY"} , 
 	{ "Name" : "kvdram_V", "interface" : "axi_master", "bitwidth" : 512, "direction" : "READONLY"} , 
 	{ "Name" : "kvdram_V_offset", "interface" : "wire", "bitwidth" : 26, "direction" : "READONLY"} , 
 	{ "Name" : "buffer0_V", "interface" : "memory", "bitwidth" : 512, "direction" : "WRITEONLY"} , 
 	{ "Name" : "buffer1_V", "interface" : "memory", "bitwidth" : 512, "direction" : "WRITEONLY"} , 
 	{ "Name" : "buffer2_V", "interface" : "memory", "bitwidth" : 512, "direction" : "WRITEONLY"} , 
 	{ "Name" : "buffer3_V", "interface" : "memory", "bitwidth" : 512, "direction" : "WRITEONLY"} , 
 	{ "Name" : "baseaddress", "interface" : "wire", "bitwidth" : 25, "direction" : "READONLY"} , 
 	{ "Name" : "offset_kvs", "interface" : "wire", "bitwidth" : 32, "direction" : "READONLY"} , 
 	{ "Name" : "travstate_i_kvs", "interface" : "wire", "bitwidth" : 32, "direction" : "READONLY"} , 
 	{ "Name" : "travstate_end_kvs", "interface" : "wire", "bitwidth" : 30, "direction" : "READONLY"} ]}
# RTL Port declarations: 
set portNum 73
set portList { 
	{ ap_clk sc_in sc_logic 1 clock -1 } 
	{ ap_rst sc_in sc_logic 1 reset -1 active_high_sync } 
	{ ap_start sc_in sc_logic 1 start -1 } 
	{ ap_done sc_out sc_logic 1 predone -1 } 
	{ ap_idle sc_out sc_logic 1 done -1 } 
	{ ap_ready sc_out sc_logic 1 ready -1 } 
	{ enable sc_in sc_lv 1 signal 0 } 
	{ m_axi_kvdram_V_AWVALID sc_out sc_logic 1 signal 1 } 
	{ m_axi_kvdram_V_AWREADY sc_in sc_logic 1 signal 1 } 
	{ m_axi_kvdram_V_AWADDR sc_out sc_lv 32 signal 1 } 
	{ m_axi_kvdram_V_AWID sc_out sc_lv 1 signal 1 } 
	{ m_axi_kvdram_V_AWLEN sc_out sc_lv 32 signal 1 } 
	{ m_axi_kvdram_V_AWSIZE sc_out sc_lv 3 signal 1 } 
	{ m_axi_kvdram_V_AWBURST sc_out sc_lv 2 signal 1 } 
	{ m_axi_kvdram_V_AWLOCK sc_out sc_lv 2 signal 1 } 
	{ m_axi_kvdram_V_AWCACHE sc_out sc_lv 4 signal 1 } 
	{ m_axi_kvdram_V_AWPROT sc_out sc_lv 3 signal 1 } 
	{ m_axi_kvdram_V_AWQOS sc_out sc_lv 4 signal 1 } 
	{ m_axi_kvdram_V_AWREGION sc_out sc_lv 4 signal 1 } 
	{ m_axi_kvdram_V_AWUSER sc_out sc_lv 1 signal 1 } 
	{ m_axi_kvdram_V_WVALID sc_out sc_logic 1 signal 1 } 
	{ m_axi_kvdram_V_WREADY sc_in sc_logic 1 signal 1 } 
	{ m_axi_kvdram_V_WDATA sc_out sc_lv 512 signal 1 } 
	{ m_axi_kvdram_V_WSTRB sc_out sc_lv 64 signal 1 } 
	{ m_axi_kvdram_V_WLAST sc_out sc_logic 1 signal 1 } 
	{ m_axi_kvdram_V_WID sc_out sc_lv 1 signal 1 } 
	{ m_axi_kvdram_V_WUSER sc_out sc_lv 1 signal 1 } 
	{ m_axi_kvdram_V_ARVALID sc_out sc_logic 1 signal 1 } 
	{ m_axi_kvdram_V_ARREADY sc_in sc_logic 1 signal 1 } 
	{ m_axi_kvdram_V_ARADDR sc_out sc_lv 32 signal 1 } 
	{ m_axi_kvdram_V_ARID sc_out sc_lv 1 signal 1 } 
	{ m_axi_kvdram_V_ARLEN sc_out sc_lv 32 signal 1 } 
	{ m_axi_kvdram_V_ARSIZE sc_out sc_lv 3 signal 1 } 
	{ m_axi_kvdram_V_ARBURST sc_out sc_lv 2 signal 1 } 
	{ m_axi_kvdram_V_ARLOCK sc_out sc_lv 2 signal 1 } 
	{ m_axi_kvdram_V_ARCACHE sc_out sc_lv 4 signal 1 } 
	{ m_axi_kvdram_V_ARPROT sc_out sc_lv 3 signal 1 } 
	{ m_axi_kvdram_V_ARQOS sc_out sc_lv 4 signal 1 } 
	{ m_axi_kvdram_V_ARREGION sc_out sc_lv 4 signal 1 } 
	{ m_axi_kvdram_V_ARUSER sc_out sc_lv 1 signal 1 } 
	{ m_axi_kvdram_V_RVALID sc_in sc_logic 1 signal 1 } 
	{ m_axi_kvdram_V_RREADY sc_out sc_logic 1 signal 1 } 
	{ m_axi_kvdram_V_RDATA sc_in sc_lv 512 signal 1 } 
	{ m_axi_kvdram_V_RLAST sc_in sc_logic 1 signal 1 } 
	{ m_axi_kvdram_V_RID sc_in sc_lv 1 signal 1 } 
	{ m_axi_kvdram_V_RUSER sc_in sc_lv 1 signal 1 } 
	{ m_axi_kvdram_V_RRESP sc_in sc_lv 2 signal 1 } 
	{ m_axi_kvdram_V_BVALID sc_in sc_logic 1 signal 1 } 
	{ m_axi_kvdram_V_BREADY sc_out sc_logic 1 signal 1 } 
	{ m_axi_kvdram_V_BRESP sc_in sc_lv 2 signal 1 } 
	{ m_axi_kvdram_V_BID sc_in sc_lv 1 signal 1 } 
	{ m_axi_kvdram_V_BUSER sc_in sc_lv 1 signal 1 } 
	{ kvdram_V_offset sc_in sc_lv 26 signal 2 } 
	{ buffer0_V_address0 sc_out sc_lv 9 signal 3 } 
	{ buffer0_V_ce0 sc_out sc_logic 1 signal 3 } 
	{ buffer0_V_we0 sc_out sc_logic 1 signal 3 } 
	{ buffer0_V_d0 sc_out sc_lv 512 signal 3 } 
	{ buffer1_V_address0 sc_out sc_lv 9 signal 4 } 
	{ buffer1_V_ce0 sc_out sc_logic 1 signal 4 } 
	{ buffer1_V_we0 sc_out sc_logic 1 signal 4 } 
	{ buffer1_V_d0 sc_out sc_lv 512 signal 4 } 
	{ buffer2_V_address0 sc_out sc_lv 9 signal 5 } 
	{ buffer2_V_ce0 sc_out sc_logic 1 signal 5 } 
	{ buffer2_V_we0 sc_out sc_logic 1 signal 5 } 
	{ buffer2_V_d0 sc_out sc_lv 512 signal 5 } 
	{ buffer3_V_address0 sc_out sc_lv 9 signal 6 } 
	{ buffer3_V_ce0 sc_out sc_logic 1 signal 6 } 
	{ buffer3_V_we0 sc_out sc_logic 1 signal 6 } 
	{ buffer3_V_d0 sc_out sc_lv 512 signal 6 } 
	{ baseaddress sc_in sc_lv 25 signal 7 } 
	{ offset_kvs sc_in sc_lv 32 signal 8 } 
	{ travstate_i_kvs sc_in sc_lv 32 signal 9 } 
	{ travstate_end_kvs sc_in sc_lv 30 signal 10 } 
}
set NewPortList {[ 
	{ "name": "ap_clk", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "clock", "bundle":{"name": "ap_clk", "role": "default" }} , 
 	{ "name": "ap_rst", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "reset", "bundle":{"name": "ap_rst", "role": "default" }} , 
 	{ "name": "ap_start", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "start", "bundle":{"name": "ap_start", "role": "default" }} , 
 	{ "name": "ap_done", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "predone", "bundle":{"name": "ap_done", "role": "default" }} , 
 	{ "name": "ap_idle", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "done", "bundle":{"name": "ap_idle", "role": "default" }} , 
 	{ "name": "ap_ready", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "ready", "bundle":{"name": "ap_ready", "role": "default" }} , 
 	{ "name": "enable", "direction": "in", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "enable", "role": "default" }} , 
 	{ "name": "m_axi_kvdram_V_AWVALID", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "kvdram_V", "role": "AWVALID" }} , 
 	{ "name": "m_axi_kvdram_V_AWREADY", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "kvdram_V", "role": "AWREADY" }} , 
 	{ "name": "m_axi_kvdram_V_AWADDR", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "kvdram_V", "role": "AWADDR" }} , 
 	{ "name": "m_axi_kvdram_V_AWID", "direction": "out", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "kvdram_V", "role": "AWID" }} , 
 	{ "name": "m_axi_kvdram_V_AWLEN", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "kvdram_V", "role": "AWLEN" }} , 
 	{ "name": "m_axi_kvdram_V_AWSIZE", "direction": "out", "datatype": "sc_lv", "bitwidth":3, "type": "signal", "bundle":{"name": "kvdram_V", "role": "AWSIZE" }} , 
 	{ "name": "m_axi_kvdram_V_AWBURST", "direction": "out", "datatype": "sc_lv", "bitwidth":2, "type": "signal", "bundle":{"name": "kvdram_V", "role": "AWBURST" }} , 
 	{ "name": "m_axi_kvdram_V_AWLOCK", "direction": "out", "datatype": "sc_lv", "bitwidth":2, "type": "signal", "bundle":{"name": "kvdram_V", "role": "AWLOCK" }} , 
 	{ "name": "m_axi_kvdram_V_AWCACHE", "direction": "out", "datatype": "sc_lv", "bitwidth":4, "type": "signal", "bundle":{"name": "kvdram_V", "role": "AWCACHE" }} , 
 	{ "name": "m_axi_kvdram_V_AWPROT", "direction": "out", "datatype": "sc_lv", "bitwidth":3, "type": "signal", "bundle":{"name": "kvdram_V", "role": "AWPROT" }} , 
 	{ "name": "m_axi_kvdram_V_AWQOS", "direction": "out", "datatype": "sc_lv", "bitwidth":4, "type": "signal", "bundle":{"name": "kvdram_V", "role": "AWQOS" }} , 
 	{ "name": "m_axi_kvdram_V_AWREGION", "direction": "out", "datatype": "sc_lv", "bitwidth":4, "type": "signal", "bundle":{"name": "kvdram_V", "role": "AWREGION" }} , 
 	{ "name": "m_axi_kvdram_V_AWUSER", "direction": "out", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "kvdram_V", "role": "AWUSER" }} , 
 	{ "name": "m_axi_kvdram_V_WVALID", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "kvdram_V", "role": "WVALID" }} , 
 	{ "name": "m_axi_kvdram_V_WREADY", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "kvdram_V", "role": "WREADY" }} , 
 	{ "name": "m_axi_kvdram_V_WDATA", "direction": "out", "datatype": "sc_lv", "bitwidth":512, "type": "signal", "bundle":{"name": "kvdram_V", "role": "WDATA" }} , 
 	{ "name": "m_axi_kvdram_V_WSTRB", "direction": "out", "datatype": "sc_lv", "bitwidth":64, "type": "signal", "bundle":{"name": "kvdram_V", "role": "WSTRB" }} , 
 	{ "name": "m_axi_kvdram_V_WLAST", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "kvdram_V", "role": "WLAST" }} , 
 	{ "name": "m_axi_kvdram_V_WID", "direction": "out", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "kvdram_V", "role": "WID" }} , 
 	{ "name": "m_axi_kvdram_V_WUSER", "direction": "out", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "kvdram_V", "role": "WUSER" }} , 
 	{ "name": "m_axi_kvdram_V_ARVALID", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "kvdram_V", "role": "ARVALID" }} , 
 	{ "name": "m_axi_kvdram_V_ARREADY", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "kvdram_V", "role": "ARREADY" }} , 
 	{ "name": "m_axi_kvdram_V_ARADDR", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "kvdram_V", "role": "ARADDR" }} , 
 	{ "name": "m_axi_kvdram_V_ARID", "direction": "out", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "kvdram_V", "role": "ARID" }} , 
 	{ "name": "m_axi_kvdram_V_ARLEN", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "kvdram_V", "role": "ARLEN" }} , 
 	{ "name": "m_axi_kvdram_V_ARSIZE", "direction": "out", "datatype": "sc_lv", "bitwidth":3, "type": "signal", "bundle":{"name": "kvdram_V", "role": "ARSIZE" }} , 
 	{ "name": "m_axi_kvdram_V_ARBURST", "direction": "out", "datatype": "sc_lv", "bitwidth":2, "type": "signal", "bundle":{"name": "kvdram_V", "role": "ARBURST" }} , 
 	{ "name": "m_axi_kvdram_V_ARLOCK", "direction": "out", "datatype": "sc_lv", "bitwidth":2, "type": "signal", "bundle":{"name": "kvdram_V", "role": "ARLOCK" }} , 
 	{ "name": "m_axi_kvdram_V_ARCACHE", "direction": "out", "datatype": "sc_lv", "bitwidth":4, "type": "signal", "bundle":{"name": "kvdram_V", "role": "ARCACHE" }} , 
 	{ "name": "m_axi_kvdram_V_ARPROT", "direction": "out", "datatype": "sc_lv", "bitwidth":3, "type": "signal", "bundle":{"name": "kvdram_V", "role": "ARPROT" }} , 
 	{ "name": "m_axi_kvdram_V_ARQOS", "direction": "out", "datatype": "sc_lv", "bitwidth":4, "type": "signal", "bundle":{"name": "kvdram_V", "role": "ARQOS" }} , 
 	{ "name": "m_axi_kvdram_V_ARREGION", "direction": "out", "datatype": "sc_lv", "bitwidth":4, "type": "signal", "bundle":{"name": "kvdram_V", "role": "ARREGION" }} , 
 	{ "name": "m_axi_kvdram_V_ARUSER", "direction": "out", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "kvdram_V", "role": "ARUSER" }} , 
 	{ "name": "m_axi_kvdram_V_RVALID", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "kvdram_V", "role": "RVALID" }} , 
 	{ "name": "m_axi_kvdram_V_RREADY", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "kvdram_V", "role": "RREADY" }} , 
 	{ "name": "m_axi_kvdram_V_RDATA", "direction": "in", "datatype": "sc_lv", "bitwidth":512, "type": "signal", "bundle":{"name": "kvdram_V", "role": "RDATA" }} , 
 	{ "name": "m_axi_kvdram_V_RLAST", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "kvdram_V", "role": "RLAST" }} , 
 	{ "name": "m_axi_kvdram_V_RID", "direction": "in", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "kvdram_V", "role": "RID" }} , 
 	{ "name": "m_axi_kvdram_V_RUSER", "direction": "in", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "kvdram_V", "role": "RUSER" }} , 
 	{ "name": "m_axi_kvdram_V_RRESP", "direction": "in", "datatype": "sc_lv", "bitwidth":2, "type": "signal", "bundle":{"name": "kvdram_V", "role": "RRESP" }} , 
 	{ "name": "m_axi_kvdram_V_BVALID", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "kvdram_V", "role": "BVALID" }} , 
 	{ "name": "m_axi_kvdram_V_BREADY", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "kvdram_V", "role": "BREADY" }} , 
 	{ "name": "m_axi_kvdram_V_BRESP", "direction": "in", "datatype": "sc_lv", "bitwidth":2, "type": "signal", "bundle":{"name": "kvdram_V", "role": "BRESP" }} , 
 	{ "name": "m_axi_kvdram_V_BID", "direction": "in", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "kvdram_V", "role": "BID" }} , 
 	{ "name": "m_axi_kvdram_V_BUSER", "direction": "in", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "kvdram_V", "role": "BUSER" }} , 
 	{ "name": "kvdram_V_offset", "direction": "in", "datatype": "sc_lv", "bitwidth":26, "type": "signal", "bundle":{"name": "kvdram_V_offset", "role": "default" }} , 
 	{ "name": "buffer0_V_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":9, "type": "signal", "bundle":{"name": "buffer0_V", "role": "address0" }} , 
 	{ "name": "buffer0_V_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "buffer0_V", "role": "ce0" }} , 
 	{ "name": "buffer0_V_we0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "buffer0_V", "role": "we0" }} , 
 	{ "name": "buffer0_V_d0", "direction": "out", "datatype": "sc_lv", "bitwidth":512, "type": "signal", "bundle":{"name": "buffer0_V", "role": "d0" }} , 
 	{ "name": "buffer1_V_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":9, "type": "signal", "bundle":{"name": "buffer1_V", "role": "address0" }} , 
 	{ "name": "buffer1_V_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "buffer1_V", "role": "ce0" }} , 
 	{ "name": "buffer1_V_we0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "buffer1_V", "role": "we0" }} , 
 	{ "name": "buffer1_V_d0", "direction": "out", "datatype": "sc_lv", "bitwidth":512, "type": "signal", "bundle":{"name": "buffer1_V", "role": "d0" }} , 
 	{ "name": "buffer2_V_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":9, "type": "signal", "bundle":{"name": "buffer2_V", "role": "address0" }} , 
 	{ "name": "buffer2_V_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "buffer2_V", "role": "ce0" }} , 
 	{ "name": "buffer2_V_we0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "buffer2_V", "role": "we0" }} , 
 	{ "name": "buffer2_V_d0", "direction": "out", "datatype": "sc_lv", "bitwidth":512, "type": "signal", "bundle":{"name": "buffer2_V", "role": "d0" }} , 
 	{ "name": "buffer3_V_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":9, "type": "signal", "bundle":{"name": "buffer3_V", "role": "address0" }} , 
 	{ "name": "buffer3_V_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "buffer3_V", "role": "ce0" }} , 
 	{ "name": "buffer3_V_we0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "buffer3_V", "role": "we0" }} , 
 	{ "name": "buffer3_V_d0", "direction": "out", "datatype": "sc_lv", "bitwidth":512, "type": "signal", "bundle":{"name": "buffer3_V", "role": "d0" }} , 
 	{ "name": "baseaddress", "direction": "in", "datatype": "sc_lv", "bitwidth":25, "type": "signal", "bundle":{"name": "baseaddress", "role": "default" }} , 
 	{ "name": "offset_kvs", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "offset_kvs", "role": "default" }} , 
 	{ "name": "travstate_i_kvs", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "travstate_i_kvs", "role": "default" }} , 
 	{ "name": "travstate_end_kvs", "direction": "in", "datatype": "sc_lv", "bitwidth":30, "type": "signal", "bundle":{"name": "travstate_end_kvs", "role": "default" }}  ]}

set RtlHierarchyInfo {[
	{"ID" : "0", "Level" : "0", "Path" : "`AUTOTB_DUT_INST", "Parent" : "", "Child" : ["1"],
		"CDFG" : "readkeyvalues0_1",
		"Protocol" : "ap_ctrl_hs",
		"ControlExist" : "1", "ap_start" : "1", "ap_ready" : "1", "ap_done" : "1", "ap_continue" : "0", "ap_idle" : "1",
		"Pipeline" : "None", "UnalignedPipeline" : "0", "RewindPipeline" : "0", "ProcessNetwork" : "0",
		"II" : "0",
		"VariableLatency" : "1", "ExactLatency" : "-1", "EstimateLatencyMin" : "1", "EstimateLatencyMax" : "2096",
		"Combinational" : "0",
		"Datapath" : "0",
		"ClockEnable" : "0",
		"HasSubDataflow" : "0",
		"InDataflowNetwork" : "0",
		"HasNonBlockingOperation" : "0",
		"WaitState" : [
			{"State" : "ap_ST_fsm_state3", "FSM" : "ap_CS_fsm", "SubInstance" : "grp_readkeyvalues0_158_fu_94"},
			{"State" : "ap_ST_fsm_state5", "FSM" : "ap_CS_fsm", "SubInstance" : "grp_readkeyvalues0_158_fu_94"},
			{"State" : "ap_ST_fsm_state7", "FSM" : "ap_CS_fsm", "SubInstance" : "grp_readkeyvalues0_158_fu_94"},
			{"State" : "ap_ST_fsm_state9", "FSM" : "ap_CS_fsm", "SubInstance" : "grp_readkeyvalues0_158_fu_94"}],
		"Port" : [
			{"Name" : "enable", "Type" : "None", "Direction" : "I"},
			{"Name" : "kvdram_V", "Type" : "MAXI", "Direction" : "I",
				"SubConnect" : [
					{"ID" : "1", "SubInstance" : "grp_readkeyvalues0_158_fu_94", "Port" : "kvdram_V"}]},
			{"Name" : "kvdram_V_offset", "Type" : "None", "Direction" : "I"},
			{"Name" : "buffer0_V", "Type" : "Memory", "Direction" : "O",
				"SubConnect" : [
					{"ID" : "1", "SubInstance" : "grp_readkeyvalues0_158_fu_94", "Port" : "buffer_V"}]},
			{"Name" : "buffer1_V", "Type" : "Memory", "Direction" : "O",
				"SubConnect" : [
					{"ID" : "1", "SubInstance" : "grp_readkeyvalues0_158_fu_94", "Port" : "buffer_V"}]},
			{"Name" : "buffer2_V", "Type" : "Memory", "Direction" : "O",
				"SubConnect" : [
					{"ID" : "1", "SubInstance" : "grp_readkeyvalues0_158_fu_94", "Port" : "buffer_V"}]},
			{"Name" : "buffer3_V", "Type" : "Memory", "Direction" : "O",
				"SubConnect" : [
					{"ID" : "1", "SubInstance" : "grp_readkeyvalues0_158_fu_94", "Port" : "buffer_V"}]},
			{"Name" : "baseaddress", "Type" : "None", "Direction" : "I"},
			{"Name" : "offset_kvs", "Type" : "None", "Direction" : "I"},
			{"Name" : "travstate_i_kvs", "Type" : "None", "Direction" : "I"},
			{"Name" : "travstate_end_kvs", "Type" : "None", "Direction" : "I"}]},
	{"ID" : "1", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.grp_readkeyvalues0_158_fu_94", "Parent" : "0",
		"CDFG" : "readkeyvalues0_158",
		"Protocol" : "ap_ctrl_hs",
		"ControlExist" : "1", "ap_start" : "1", "ap_ready" : "1", "ap_done" : "1", "ap_continue" : "0", "ap_idle" : "1",
		"Pipeline" : "None", "UnalignedPipeline" : "0", "RewindPipeline" : "0", "ProcessNetwork" : "0",
		"II" : "0",
		"VariableLatency" : "1", "ExactLatency" : "-1", "EstimateLatencyMin" : "9", "EstimateLatencyMax" : "522",
		"Combinational" : "0",
		"Datapath" : "0",
		"ClockEnable" : "0",
		"HasSubDataflow" : "0",
		"InDataflowNetwork" : "0",
		"HasNonBlockingOperation" : "0",
		"Port" : [
			{"Name" : "kvdram_V", "Type" : "MAXI", "Direction" : "I",
				"BlockSignal" : [
					{"Name" : "kvdram_V_blk_n_AR", "Type" : "RtlSignal"},
					{"Name" : "kvdram_V_blk_n_R", "Type" : "RtlSignal"}]},
			{"Name" : "kvdram_V_offset", "Type" : "None", "Direction" : "I"},
			{"Name" : "buffer_V", "Type" : "Memory", "Direction" : "O"},
			{"Name" : "baseaddress", "Type" : "None", "Direction" : "I"},
			{"Name" : "offset_kvs", "Type" : "None", "Direction" : "I"},
			{"Name" : "size_kvs", "Type" : "None", "Direction" : "I"}]}]}


set ArgLastReadFirstWriteLatency {
	readkeyvalues0_1 {
		enable {Type I LastRead 0 FirstWrite -1}
		kvdram_V {Type I LastRead 9 FirstWrite -1}
		kvdram_V_offset {Type I LastRead 0 FirstWrite -1}
		buffer0_V {Type O LastRead -1 FirstWrite 10}
		buffer1_V {Type O LastRead -1 FirstWrite 10}
		buffer2_V {Type O LastRead -1 FirstWrite 10}
		buffer3_V {Type O LastRead -1 FirstWrite 10}
		baseaddress {Type I LastRead 0 FirstWrite -1}
		offset_kvs {Type I LastRead 0 FirstWrite -1}
		travstate_i_kvs {Type I LastRead 0 FirstWrite -1}
		travstate_end_kvs {Type I LastRead 0 FirstWrite -1}}
	readkeyvalues0_158 {
		kvdram_V {Type I LastRead 9 FirstWrite -1}
		kvdram_V_offset {Type I LastRead 0 FirstWrite -1}
		buffer_V {Type O LastRead -1 FirstWrite 10}
		baseaddress {Type I LastRead 0 FirstWrite -1}
		offset_kvs {Type I LastRead 0 FirstWrite -1}
		size_kvs {Type I LastRead 1 FirstWrite -1}}}

set hasDtUnsupportedChannel 0

set PerformanceInfo {[
	{"Name" : "Latency", "Min" : "1", "Max" : "2096"}
	, {"Name" : "Interval", "Min" : "1", "Max" : "2096"}
]}

set PipelineEnableSignalInfo {[
]}

set Spec2ImplPortList { 
	enable { ap_none {  { enable in_data 0 1 } } }
	kvdram_V { m_axi {  { m_axi_kvdram_V_AWVALID VALID 1 1 }  { m_axi_kvdram_V_AWREADY READY 0 1 }  { m_axi_kvdram_V_AWADDR ADDR 1 32 }  { m_axi_kvdram_V_AWID ID 1 1 }  { m_axi_kvdram_V_AWLEN LEN 1 32 }  { m_axi_kvdram_V_AWSIZE SIZE 1 3 }  { m_axi_kvdram_V_AWBURST BURST 1 2 }  { m_axi_kvdram_V_AWLOCK LOCK 1 2 }  { m_axi_kvdram_V_AWCACHE CACHE 1 4 }  { m_axi_kvdram_V_AWPROT PROT 1 3 }  { m_axi_kvdram_V_AWQOS QOS 1 4 }  { m_axi_kvdram_V_AWREGION REGION 1 4 }  { m_axi_kvdram_V_AWUSER USER 1 1 }  { m_axi_kvdram_V_WVALID VALID 1 1 }  { m_axi_kvdram_V_WREADY READY 0 1 }  { m_axi_kvdram_V_WDATA DATA 1 512 }  { m_axi_kvdram_V_WSTRB STRB 1 64 }  { m_axi_kvdram_V_WLAST LAST 1 1 }  { m_axi_kvdram_V_WID ID 1 1 }  { m_axi_kvdram_V_WUSER USER 1 1 }  { m_axi_kvdram_V_ARVALID VALID 1 1 }  { m_axi_kvdram_V_ARREADY READY 0 1 }  { m_axi_kvdram_V_ARADDR ADDR 1 32 }  { m_axi_kvdram_V_ARID ID 1 1 }  { m_axi_kvdram_V_ARLEN LEN 1 32 }  { m_axi_kvdram_V_ARSIZE SIZE 1 3 }  { m_axi_kvdram_V_ARBURST BURST 1 2 }  { m_axi_kvdram_V_ARLOCK LOCK 1 2 }  { m_axi_kvdram_V_ARCACHE CACHE 1 4 }  { m_axi_kvdram_V_ARPROT PROT 1 3 }  { m_axi_kvdram_V_ARQOS QOS 1 4 }  { m_axi_kvdram_V_ARREGION REGION 1 4 }  { m_axi_kvdram_V_ARUSER USER 1 1 }  { m_axi_kvdram_V_RVALID VALID 0 1 }  { m_axi_kvdram_V_RREADY READY 1 1 }  { m_axi_kvdram_V_RDATA DATA 0 512 }  { m_axi_kvdram_V_RLAST LAST 0 1 }  { m_axi_kvdram_V_RID ID 0 1 }  { m_axi_kvdram_V_RUSER USER 0 1 }  { m_axi_kvdram_V_RRESP RESP 0 2 }  { m_axi_kvdram_V_BVALID VALID 0 1 }  { m_axi_kvdram_V_BREADY READY 1 1 }  { m_axi_kvdram_V_BRESP RESP 0 2 }  { m_axi_kvdram_V_BID ID 0 1 }  { m_axi_kvdram_V_BUSER USER 0 1 } } }
	kvdram_V_offset { ap_none {  { kvdram_V_offset in_data 0 26 } } }
	buffer0_V { ap_memory {  { buffer0_V_address0 mem_address 1 9 }  { buffer0_V_ce0 mem_ce 1 1 }  { buffer0_V_we0 mem_we 1 1 }  { buffer0_V_d0 mem_din 1 512 } } }
	buffer1_V { ap_memory {  { buffer1_V_address0 mem_address 1 9 }  { buffer1_V_ce0 mem_ce 1 1 }  { buffer1_V_we0 mem_we 1 1 }  { buffer1_V_d0 mem_din 1 512 } } }
	buffer2_V { ap_memory {  { buffer2_V_address0 mem_address 1 9 }  { buffer2_V_ce0 mem_ce 1 1 }  { buffer2_V_we0 mem_we 1 1 }  { buffer2_V_d0 mem_din 1 512 } } }
	buffer3_V { ap_memory {  { buffer3_V_address0 mem_address 1 9 }  { buffer3_V_ce0 mem_ce 1 1 }  { buffer3_V_we0 mem_we 1 1 }  { buffer3_V_d0 mem_din 1 512 } } }
	baseaddress { ap_none {  { baseaddress in_data 0 25 } } }
	offset_kvs { ap_none {  { offset_kvs in_data 0 32 } } }
	travstate_i_kvs { ap_none {  { travstate_i_kvs in_data 0 32 } } }
	travstate_end_kvs { ap_none {  { travstate_end_kvs in_data 0 30 } } }
}
set moduleName readkeyvalues0_1
set isTopModule 0
set isTaskLevelControl 1
set isCombinational 0
set isDatapathOnly 0
set isFreeRunPipelineModule 0
set isPipelined 0
set pipeline_type none
set FunctionProtocol ap_ctrl_hs
set isOneStateSeq 0
set ProfileFlag 0
set StallSigGenFlag 0
set isEnableWaveformDebug 1
set C_modelName {readkeyvalues0.1}
set C_modelType { void 0 }
set C_modelArgList {
	{ kvdram_V int 512 regular {axi_master 0}  }
	{ kvdram_V_offset int 26 regular  }
	{ buffer00_key int 32 regular {array 2112 { 0 3 } 0 1 }  }
	{ buffer00_value int 32 regular {array 2112 { 0 3 } 0 1 }  }
	{ buffer01_key int 32 regular {array 2112 { 0 3 } 0 1 }  }
	{ buffer01_value int 32 regular {array 2112 { 0 3 } 0 1 }  }
	{ buffer02_key int 32 regular {array 2112 { 0 3 } 0 1 }  }
	{ buffer02_value int 32 regular {array 2112 { 0 3 } 0 1 }  }
	{ buffer03_key int 32 regular {array 2112 { 0 3 } 0 1 }  }
	{ buffer03_value int 32 regular {array 2112 { 0 3 } 0 1 }  }
	{ buffer04_key int 32 regular {array 2112 { 0 3 } 0 1 }  }
	{ buffer04_value int 32 regular {array 2112 { 0 3 } 0 1 }  }
	{ buffer05_key int 32 regular {array 2112 { 0 3 } 0 1 }  }
	{ buffer05_value int 32 regular {array 2112 { 0 3 } 0 1 }  }
	{ buffer06_key int 32 regular {array 2112 { 0 3 } 0 1 }  }
	{ buffer06_value int 32 regular {array 2112 { 0 3 } 0 1 }  }
	{ buffer07_key int 32 regular {array 2112 { 0 3 } 0 1 }  }
	{ buffer07_value int 32 regular {array 2112 { 0 3 } 0 1 }  }
	{ buffer10_key int 32 regular {array 2112 { 0 3 } 0 1 }  }
	{ buffer10_value int 32 regular {array 2112 { 0 3 } 0 1 }  }
	{ buffer11_key int 32 regular {array 2112 { 0 3 } 0 1 }  }
	{ buffer11_value int 32 regular {array 2112 { 0 3 } 0 1 }  }
	{ buffer12_key int 32 regular {array 2112 { 0 3 } 0 1 }  }
	{ buffer12_value int 32 regular {array 2112 { 0 3 } 0 1 }  }
	{ buffer13_key int 32 regular {array 2112 { 0 3 } 0 1 }  }
	{ buffer13_value int 32 regular {array 2112 { 0 3 } 0 1 }  }
	{ buffer14_key int 32 regular {array 2112 { 0 3 } 0 1 }  }
	{ buffer14_value int 32 regular {array 2112 { 0 3 } 0 1 }  }
	{ buffer15_key int 32 regular {array 2112 { 0 3 } 0 1 }  }
	{ buffer15_value int 32 regular {array 2112 { 0 3 } 0 1 }  }
	{ buffer16_key int 32 regular {array 2112 { 0 3 } 0 1 }  }
	{ buffer16_value int 32 regular {array 2112 { 0 3 } 0 1 }  }
	{ buffer17_key int 32 regular {array 2112 { 0 3 } 0 1 }  }
	{ buffer17_value int 32 regular {array 2112 { 0 3 } 0 1 }  }
	{ buffer20_key int 32 regular {array 2112 { 0 3 } 0 1 }  }
	{ buffer20_value int 32 regular {array 2112 { 0 3 } 0 1 }  }
	{ buffer21_key int 32 regular {array 2112 { 0 3 } 0 1 }  }
	{ buffer21_value int 32 regular {array 2112 { 0 3 } 0 1 }  }
	{ buffer22_key int 32 regular {array 2112 { 0 3 } 0 1 }  }
	{ buffer22_value int 32 regular {array 2112 { 0 3 } 0 1 }  }
	{ buffer23_key int 32 regular {array 2112 { 0 3 } 0 1 }  }
	{ buffer23_value int 32 regular {array 2112 { 0 3 } 0 1 }  }
	{ buffer24_key int 32 regular {array 2112 { 0 3 } 0 1 }  }
	{ buffer24_value int 32 regular {array 2112 { 0 3 } 0 1 }  }
	{ buffer25_key int 32 regular {array 2112 { 0 3 } 0 1 }  }
	{ buffer25_value int 32 regular {array 2112 { 0 3 } 0 1 }  }
	{ buffer26_key int 32 regular {array 2112 { 0 3 } 0 1 }  }
	{ buffer26_value int 32 regular {array 2112 { 0 3 } 0 1 }  }
	{ buffer27_key int 32 regular {array 2112 { 0 3 } 0 1 }  }
	{ buffer27_value int 32 regular {array 2112 { 0 3 } 0 1 }  }
	{ buffer30_key int 32 regular {array 2112 { 0 3 } 0 1 }  }
	{ buffer30_value int 32 regular {array 2112 { 0 3 } 0 1 }  }
	{ buffer31_key int 32 regular {array 2112 { 0 3 } 0 1 }  }
	{ buffer31_value int 32 regular {array 2112 { 0 3 } 0 1 }  }
	{ buffer32_key int 32 regular {array 2112 { 0 3 } 0 1 }  }
	{ buffer32_value int 32 regular {array 2112 { 0 3 } 0 1 }  }
	{ buffer33_key int 32 regular {array 2112 { 0 3 } 0 1 }  }
	{ buffer33_value int 32 regular {array 2112 { 0 3 } 0 1 }  }
	{ buffer34_key int 32 regular {array 2112 { 0 3 } 0 1 }  }
	{ buffer34_value int 32 regular {array 2112 { 0 3 } 0 1 }  }
	{ buffer35_key int 32 regular {array 2112 { 0 3 } 0 1 }  }
	{ buffer35_value int 32 regular {array 2112 { 0 3 } 0 1 }  }
	{ buffer36_key int 32 regular {array 2112 { 0 3 } 0 1 }  }
	{ buffer36_value int 32 regular {array 2112 { 0 3 } 0 1 }  }
	{ buffer37_key int 32 regular {array 2112 { 0 3 } 0 1 }  }
	{ buffer37_value int 32 regular {array 2112 { 0 3 } 0 1 }  }
	{ offset_kvs int 30 regular  }
}
set C_modelArgMapList {[ 
	{ "Name" : "kvdram_V", "interface" : "axi_master", "bitwidth" : 512, "direction" : "READONLY"} , 
 	{ "Name" : "kvdram_V_offset", "interface" : "wire", "bitwidth" : 26, "direction" : "READONLY"} , 
 	{ "Name" : "buffer00_key", "interface" : "memory", "bitwidth" : 32, "direction" : "WRITEONLY"} , 
 	{ "Name" : "buffer00_value", "interface" : "memory", "bitwidth" : 32, "direction" : "WRITEONLY"} , 
 	{ "Name" : "buffer01_key", "interface" : "memory", "bitwidth" : 32, "direction" : "WRITEONLY"} , 
 	{ "Name" : "buffer01_value", "interface" : "memory", "bitwidth" : 32, "direction" : "WRITEONLY"} , 
 	{ "Name" : "buffer02_key", "interface" : "memory", "bitwidth" : 32, "direction" : "WRITEONLY"} , 
 	{ "Name" : "buffer02_value", "interface" : "memory", "bitwidth" : 32, "direction" : "WRITEONLY"} , 
 	{ "Name" : "buffer03_key", "interface" : "memory", "bitwidth" : 32, "direction" : "WRITEONLY"} , 
 	{ "Name" : "buffer03_value", "interface" : "memory", "bitwidth" : 32, "direction" : "WRITEONLY"} , 
 	{ "Name" : "buffer04_key", "interface" : "memory", "bitwidth" : 32, "direction" : "WRITEONLY"} , 
 	{ "Name" : "buffer04_value", "interface" : "memory", "bitwidth" : 32, "direction" : "WRITEONLY"} , 
 	{ "Name" : "buffer05_key", "interface" : "memory", "bitwidth" : 32, "direction" : "WRITEONLY"} , 
 	{ "Name" : "buffer05_value", "interface" : "memory", "bitwidth" : 32, "direction" : "WRITEONLY"} , 
 	{ "Name" : "buffer06_key", "interface" : "memory", "bitwidth" : 32, "direction" : "WRITEONLY"} , 
 	{ "Name" : "buffer06_value", "interface" : "memory", "bitwidth" : 32, "direction" : "WRITEONLY"} , 
 	{ "Name" : "buffer07_key", "interface" : "memory", "bitwidth" : 32, "direction" : "WRITEONLY"} , 
 	{ "Name" : "buffer07_value", "interface" : "memory", "bitwidth" : 32, "direction" : "WRITEONLY"} , 
 	{ "Name" : "buffer10_key", "interface" : "memory", "bitwidth" : 32, "direction" : "WRITEONLY"} , 
 	{ "Name" : "buffer10_value", "interface" : "memory", "bitwidth" : 32, "direction" : "WRITEONLY"} , 
 	{ "Name" : "buffer11_key", "interface" : "memory", "bitwidth" : 32, "direction" : "WRITEONLY"} , 
 	{ "Name" : "buffer11_value", "interface" : "memory", "bitwidth" : 32, "direction" : "WRITEONLY"} , 
 	{ "Name" : "buffer12_key", "interface" : "memory", "bitwidth" : 32, "direction" : "WRITEONLY"} , 
 	{ "Name" : "buffer12_value", "interface" : "memory", "bitwidth" : 32, "direction" : "WRITEONLY"} , 
 	{ "Name" : "buffer13_key", "interface" : "memory", "bitwidth" : 32, "direction" : "WRITEONLY"} , 
 	{ "Name" : "buffer13_value", "interface" : "memory", "bitwidth" : 32, "direction" : "WRITEONLY"} , 
 	{ "Name" : "buffer14_key", "interface" : "memory", "bitwidth" : 32, "direction" : "WRITEONLY"} , 
 	{ "Name" : "buffer14_value", "interface" : "memory", "bitwidth" : 32, "direction" : "WRITEONLY"} , 
 	{ "Name" : "buffer15_key", "interface" : "memory", "bitwidth" : 32, "direction" : "WRITEONLY"} , 
 	{ "Name" : "buffer15_value", "interface" : "memory", "bitwidth" : 32, "direction" : "WRITEONLY"} , 
 	{ "Name" : "buffer16_key", "interface" : "memory", "bitwidth" : 32, "direction" : "WRITEONLY"} , 
 	{ "Name" : "buffer16_value", "interface" : "memory", "bitwidth" : 32, "direction" : "WRITEONLY"} , 
 	{ "Name" : "buffer17_key", "interface" : "memory", "bitwidth" : 32, "direction" : "WRITEONLY"} , 
 	{ "Name" : "buffer17_value", "interface" : "memory", "bitwidth" : 32, "direction" : "WRITEONLY"} , 
 	{ "Name" : "buffer20_key", "interface" : "memory", "bitwidth" : 32, "direction" : "WRITEONLY"} , 
 	{ "Name" : "buffer20_value", "interface" : "memory", "bitwidth" : 32, "direction" : "WRITEONLY"} , 
 	{ "Name" : "buffer21_key", "interface" : "memory", "bitwidth" : 32, "direction" : "WRITEONLY"} , 
 	{ "Name" : "buffer21_value", "interface" : "memory", "bitwidth" : 32, "direction" : "WRITEONLY"} , 
 	{ "Name" : "buffer22_key", "interface" : "memory", "bitwidth" : 32, "direction" : "WRITEONLY"} , 
 	{ "Name" : "buffer22_value", "interface" : "memory", "bitwidth" : 32, "direction" : "WRITEONLY"} , 
 	{ "Name" : "buffer23_key", "interface" : "memory", "bitwidth" : 32, "direction" : "WRITEONLY"} , 
 	{ "Name" : "buffer23_value", "interface" : "memory", "bitwidth" : 32, "direction" : "WRITEONLY"} , 
 	{ "Name" : "buffer24_key", "interface" : "memory", "bitwidth" : 32, "direction" : "WRITEONLY"} , 
 	{ "Name" : "buffer24_value", "interface" : "memory", "bitwidth" : 32, "direction" : "WRITEONLY"} , 
 	{ "Name" : "buffer25_key", "interface" : "memory", "bitwidth" : 32, "direction" : "WRITEONLY"} , 
 	{ "Name" : "buffer25_value", "interface" : "memory", "bitwidth" : 32, "direction" : "WRITEONLY"} , 
 	{ "Name" : "buffer26_key", "interface" : "memory", "bitwidth" : 32, "direction" : "WRITEONLY"} , 
 	{ "Name" : "buffer26_value", "interface" : "memory", "bitwidth" : 32, "direction" : "WRITEONLY"} , 
 	{ "Name" : "buffer27_key", "interface" : "memory", "bitwidth" : 32, "direction" : "WRITEONLY"} , 
 	{ "Name" : "buffer27_value", "interface" : "memory", "bitwidth" : 32, "direction" : "WRITEONLY"} , 
 	{ "Name" : "buffer30_key", "interface" : "memory", "bitwidth" : 32, "direction" : "WRITEONLY"} , 
 	{ "Name" : "buffer30_value", "interface" : "memory", "bitwidth" : 32, "direction" : "WRITEONLY"} , 
 	{ "Name" : "buffer31_key", "interface" : "memory", "bitwidth" : 32, "direction" : "WRITEONLY"} , 
 	{ "Name" : "buffer31_value", "interface" : "memory", "bitwidth" : 32, "direction" : "WRITEONLY"} , 
 	{ "Name" : "buffer32_key", "interface" : "memory", "bitwidth" : 32, "direction" : "WRITEONLY"} , 
 	{ "Name" : "buffer32_value", "interface" : "memory", "bitwidth" : 32, "direction" : "WRITEONLY"} , 
 	{ "Name" : "buffer33_key", "interface" : "memory", "bitwidth" : 32, "direction" : "WRITEONLY"} , 
 	{ "Name" : "buffer33_value", "interface" : "memory", "bitwidth" : 32, "direction" : "WRITEONLY"} , 
 	{ "Name" : "buffer34_key", "interface" : "memory", "bitwidth" : 32, "direction" : "WRITEONLY"} , 
 	{ "Name" : "buffer34_value", "interface" : "memory", "bitwidth" : 32, "direction" : "WRITEONLY"} , 
 	{ "Name" : "buffer35_key", "interface" : "memory", "bitwidth" : 32, "direction" : "WRITEONLY"} , 
 	{ "Name" : "buffer35_value", "interface" : "memory", "bitwidth" : 32, "direction" : "WRITEONLY"} , 
 	{ "Name" : "buffer36_key", "interface" : "memory", "bitwidth" : 32, "direction" : "WRITEONLY"} , 
 	{ "Name" : "buffer36_value", "interface" : "memory", "bitwidth" : 32, "direction" : "WRITEONLY"} , 
 	{ "Name" : "buffer37_key", "interface" : "memory", "bitwidth" : 32, "direction" : "WRITEONLY"} , 
 	{ "Name" : "buffer37_value", "interface" : "memory", "bitwidth" : 32, "direction" : "WRITEONLY"} , 
 	{ "Name" : "offset_kvs", "interface" : "wire", "bitwidth" : 30, "direction" : "READONLY"} ]}
# RTL Port declarations: 
set portNum 309
set portList { 
	{ ap_clk sc_in sc_logic 1 clock -1 } 
	{ ap_rst sc_in sc_logic 1 reset -1 active_high_sync } 
	{ ap_start sc_in sc_logic 1 start -1 } 
	{ ap_done sc_out sc_logic 1 predone -1 } 
	{ ap_idle sc_out sc_logic 1 done -1 } 
	{ ap_ready sc_out sc_logic 1 ready -1 } 
	{ m_axi_kvdram_V_AWVALID sc_out sc_logic 1 signal 0 } 
	{ m_axi_kvdram_V_AWREADY sc_in sc_logic 1 signal 0 } 
	{ m_axi_kvdram_V_AWADDR sc_out sc_lv 32 signal 0 } 
	{ m_axi_kvdram_V_AWID sc_out sc_lv 1 signal 0 } 
	{ m_axi_kvdram_V_AWLEN sc_out sc_lv 32 signal 0 } 
	{ m_axi_kvdram_V_AWSIZE sc_out sc_lv 3 signal 0 } 
	{ m_axi_kvdram_V_AWBURST sc_out sc_lv 2 signal 0 } 
	{ m_axi_kvdram_V_AWLOCK sc_out sc_lv 2 signal 0 } 
	{ m_axi_kvdram_V_AWCACHE sc_out sc_lv 4 signal 0 } 
	{ m_axi_kvdram_V_AWPROT sc_out sc_lv 3 signal 0 } 
	{ m_axi_kvdram_V_AWQOS sc_out sc_lv 4 signal 0 } 
	{ m_axi_kvdram_V_AWREGION sc_out sc_lv 4 signal 0 } 
	{ m_axi_kvdram_V_AWUSER sc_out sc_lv 1 signal 0 } 
	{ m_axi_kvdram_V_WVALID sc_out sc_logic 1 signal 0 } 
	{ m_axi_kvdram_V_WREADY sc_in sc_logic 1 signal 0 } 
	{ m_axi_kvdram_V_WDATA sc_out sc_lv 512 signal 0 } 
	{ m_axi_kvdram_V_WSTRB sc_out sc_lv 64 signal 0 } 
	{ m_axi_kvdram_V_WLAST sc_out sc_logic 1 signal 0 } 
	{ m_axi_kvdram_V_WID sc_out sc_lv 1 signal 0 } 
	{ m_axi_kvdram_V_WUSER sc_out sc_lv 1 signal 0 } 
	{ m_axi_kvdram_V_ARVALID sc_out sc_logic 1 signal 0 } 
	{ m_axi_kvdram_V_ARREADY sc_in sc_logic 1 signal 0 } 
	{ m_axi_kvdram_V_ARADDR sc_out sc_lv 32 signal 0 } 
	{ m_axi_kvdram_V_ARID sc_out sc_lv 1 signal 0 } 
	{ m_axi_kvdram_V_ARLEN sc_out sc_lv 32 signal 0 } 
	{ m_axi_kvdram_V_ARSIZE sc_out sc_lv 3 signal 0 } 
	{ m_axi_kvdram_V_ARBURST sc_out sc_lv 2 signal 0 } 
	{ m_axi_kvdram_V_ARLOCK sc_out sc_lv 2 signal 0 } 
	{ m_axi_kvdram_V_ARCACHE sc_out sc_lv 4 signal 0 } 
	{ m_axi_kvdram_V_ARPROT sc_out sc_lv 3 signal 0 } 
	{ m_axi_kvdram_V_ARQOS sc_out sc_lv 4 signal 0 } 
	{ m_axi_kvdram_V_ARREGION sc_out sc_lv 4 signal 0 } 
	{ m_axi_kvdram_V_ARUSER sc_out sc_lv 1 signal 0 } 
	{ m_axi_kvdram_V_RVALID sc_in sc_logic 1 signal 0 } 
	{ m_axi_kvdram_V_RREADY sc_out sc_logic 1 signal 0 } 
	{ m_axi_kvdram_V_RDATA sc_in sc_lv 512 signal 0 } 
	{ m_axi_kvdram_V_RLAST sc_in sc_logic 1 signal 0 } 
	{ m_axi_kvdram_V_RID sc_in sc_lv 1 signal 0 } 
	{ m_axi_kvdram_V_RUSER sc_in sc_lv 1 signal 0 } 
	{ m_axi_kvdram_V_RRESP sc_in sc_lv 2 signal 0 } 
	{ m_axi_kvdram_V_BVALID sc_in sc_logic 1 signal 0 } 
	{ m_axi_kvdram_V_BREADY sc_out sc_logic 1 signal 0 } 
	{ m_axi_kvdram_V_BRESP sc_in sc_lv 2 signal 0 } 
	{ m_axi_kvdram_V_BID sc_in sc_lv 1 signal 0 } 
	{ m_axi_kvdram_V_BUSER sc_in sc_lv 1 signal 0 } 
	{ kvdram_V_offset sc_in sc_lv 26 signal 1 } 
	{ buffer00_key_address0 sc_out sc_lv 12 signal 2 } 
	{ buffer00_key_ce0 sc_out sc_logic 1 signal 2 } 
	{ buffer00_key_we0 sc_out sc_logic 1 signal 2 } 
	{ buffer00_key_d0 sc_out sc_lv 32 signal 2 } 
	{ buffer00_value_address0 sc_out sc_lv 12 signal 3 } 
	{ buffer00_value_ce0 sc_out sc_logic 1 signal 3 } 
	{ buffer00_value_we0 sc_out sc_logic 1 signal 3 } 
	{ buffer00_value_d0 sc_out sc_lv 32 signal 3 } 
	{ buffer01_key_address0 sc_out sc_lv 12 signal 4 } 
	{ buffer01_key_ce0 sc_out sc_logic 1 signal 4 } 
	{ buffer01_key_we0 sc_out sc_logic 1 signal 4 } 
	{ buffer01_key_d0 sc_out sc_lv 32 signal 4 } 
	{ buffer01_value_address0 sc_out sc_lv 12 signal 5 } 
	{ buffer01_value_ce0 sc_out sc_logic 1 signal 5 } 
	{ buffer01_value_we0 sc_out sc_logic 1 signal 5 } 
	{ buffer01_value_d0 sc_out sc_lv 32 signal 5 } 
	{ buffer02_key_address0 sc_out sc_lv 12 signal 6 } 
	{ buffer02_key_ce0 sc_out sc_logic 1 signal 6 } 
	{ buffer02_key_we0 sc_out sc_logic 1 signal 6 } 
	{ buffer02_key_d0 sc_out sc_lv 32 signal 6 } 
	{ buffer02_value_address0 sc_out sc_lv 12 signal 7 } 
	{ buffer02_value_ce0 sc_out sc_logic 1 signal 7 } 
	{ buffer02_value_we0 sc_out sc_logic 1 signal 7 } 
	{ buffer02_value_d0 sc_out sc_lv 32 signal 7 } 
	{ buffer03_key_address0 sc_out sc_lv 12 signal 8 } 
	{ buffer03_key_ce0 sc_out sc_logic 1 signal 8 } 
	{ buffer03_key_we0 sc_out sc_logic 1 signal 8 } 
	{ buffer03_key_d0 sc_out sc_lv 32 signal 8 } 
	{ buffer03_value_address0 sc_out sc_lv 12 signal 9 } 
	{ buffer03_value_ce0 sc_out sc_logic 1 signal 9 } 
	{ buffer03_value_we0 sc_out sc_logic 1 signal 9 } 
	{ buffer03_value_d0 sc_out sc_lv 32 signal 9 } 
	{ buffer04_key_address0 sc_out sc_lv 12 signal 10 } 
	{ buffer04_key_ce0 sc_out sc_logic 1 signal 10 } 
	{ buffer04_key_we0 sc_out sc_logic 1 signal 10 } 
	{ buffer04_key_d0 sc_out sc_lv 32 signal 10 } 
	{ buffer04_value_address0 sc_out sc_lv 12 signal 11 } 
	{ buffer04_value_ce0 sc_out sc_logic 1 signal 11 } 
	{ buffer04_value_we0 sc_out sc_logic 1 signal 11 } 
	{ buffer04_value_d0 sc_out sc_lv 32 signal 11 } 
	{ buffer05_key_address0 sc_out sc_lv 12 signal 12 } 
	{ buffer05_key_ce0 sc_out sc_logic 1 signal 12 } 
	{ buffer05_key_we0 sc_out sc_logic 1 signal 12 } 
	{ buffer05_key_d0 sc_out sc_lv 32 signal 12 } 
	{ buffer05_value_address0 sc_out sc_lv 12 signal 13 } 
	{ buffer05_value_ce0 sc_out sc_logic 1 signal 13 } 
	{ buffer05_value_we0 sc_out sc_logic 1 signal 13 } 
	{ buffer05_value_d0 sc_out sc_lv 32 signal 13 } 
	{ buffer06_key_address0 sc_out sc_lv 12 signal 14 } 
	{ buffer06_key_ce0 sc_out sc_logic 1 signal 14 } 
	{ buffer06_key_we0 sc_out sc_logic 1 signal 14 } 
	{ buffer06_key_d0 sc_out sc_lv 32 signal 14 } 
	{ buffer06_value_address0 sc_out sc_lv 12 signal 15 } 
	{ buffer06_value_ce0 sc_out sc_logic 1 signal 15 } 
	{ buffer06_value_we0 sc_out sc_logic 1 signal 15 } 
	{ buffer06_value_d0 sc_out sc_lv 32 signal 15 } 
	{ buffer07_key_address0 sc_out sc_lv 12 signal 16 } 
	{ buffer07_key_ce0 sc_out sc_logic 1 signal 16 } 
	{ buffer07_key_we0 sc_out sc_logic 1 signal 16 } 
	{ buffer07_key_d0 sc_out sc_lv 32 signal 16 } 
	{ buffer07_value_address0 sc_out sc_lv 12 signal 17 } 
	{ buffer07_value_ce0 sc_out sc_logic 1 signal 17 } 
	{ buffer07_value_we0 sc_out sc_logic 1 signal 17 } 
	{ buffer07_value_d0 sc_out sc_lv 32 signal 17 } 
	{ buffer10_key_address0 sc_out sc_lv 12 signal 18 } 
	{ buffer10_key_ce0 sc_out sc_logic 1 signal 18 } 
	{ buffer10_key_we0 sc_out sc_logic 1 signal 18 } 
	{ buffer10_key_d0 sc_out sc_lv 32 signal 18 } 
	{ buffer10_value_address0 sc_out sc_lv 12 signal 19 } 
	{ buffer10_value_ce0 sc_out sc_logic 1 signal 19 } 
	{ buffer10_value_we0 sc_out sc_logic 1 signal 19 } 
	{ buffer10_value_d0 sc_out sc_lv 32 signal 19 } 
	{ buffer11_key_address0 sc_out sc_lv 12 signal 20 } 
	{ buffer11_key_ce0 sc_out sc_logic 1 signal 20 } 
	{ buffer11_key_we0 sc_out sc_logic 1 signal 20 } 
	{ buffer11_key_d0 sc_out sc_lv 32 signal 20 } 
	{ buffer11_value_address0 sc_out sc_lv 12 signal 21 } 
	{ buffer11_value_ce0 sc_out sc_logic 1 signal 21 } 
	{ buffer11_value_we0 sc_out sc_logic 1 signal 21 } 
	{ buffer11_value_d0 sc_out sc_lv 32 signal 21 } 
	{ buffer12_key_address0 sc_out sc_lv 12 signal 22 } 
	{ buffer12_key_ce0 sc_out sc_logic 1 signal 22 } 
	{ buffer12_key_we0 sc_out sc_logic 1 signal 22 } 
	{ buffer12_key_d0 sc_out sc_lv 32 signal 22 } 
	{ buffer12_value_address0 sc_out sc_lv 12 signal 23 } 
	{ buffer12_value_ce0 sc_out sc_logic 1 signal 23 } 
	{ buffer12_value_we0 sc_out sc_logic 1 signal 23 } 
	{ buffer12_value_d0 sc_out sc_lv 32 signal 23 } 
	{ buffer13_key_address0 sc_out sc_lv 12 signal 24 } 
	{ buffer13_key_ce0 sc_out sc_logic 1 signal 24 } 
	{ buffer13_key_we0 sc_out sc_logic 1 signal 24 } 
	{ buffer13_key_d0 sc_out sc_lv 32 signal 24 } 
	{ buffer13_value_address0 sc_out sc_lv 12 signal 25 } 
	{ buffer13_value_ce0 sc_out sc_logic 1 signal 25 } 
	{ buffer13_value_we0 sc_out sc_logic 1 signal 25 } 
	{ buffer13_value_d0 sc_out sc_lv 32 signal 25 } 
	{ buffer14_key_address0 sc_out sc_lv 12 signal 26 } 
	{ buffer14_key_ce0 sc_out sc_logic 1 signal 26 } 
	{ buffer14_key_we0 sc_out sc_logic 1 signal 26 } 
	{ buffer14_key_d0 sc_out sc_lv 32 signal 26 } 
	{ buffer14_value_address0 sc_out sc_lv 12 signal 27 } 
	{ buffer14_value_ce0 sc_out sc_logic 1 signal 27 } 
	{ buffer14_value_we0 sc_out sc_logic 1 signal 27 } 
	{ buffer14_value_d0 sc_out sc_lv 32 signal 27 } 
	{ buffer15_key_address0 sc_out sc_lv 12 signal 28 } 
	{ buffer15_key_ce0 sc_out sc_logic 1 signal 28 } 
	{ buffer15_key_we0 sc_out sc_logic 1 signal 28 } 
	{ buffer15_key_d0 sc_out sc_lv 32 signal 28 } 
	{ buffer15_value_address0 sc_out sc_lv 12 signal 29 } 
	{ buffer15_value_ce0 sc_out sc_logic 1 signal 29 } 
	{ buffer15_value_we0 sc_out sc_logic 1 signal 29 } 
	{ buffer15_value_d0 sc_out sc_lv 32 signal 29 } 
	{ buffer16_key_address0 sc_out sc_lv 12 signal 30 } 
	{ buffer16_key_ce0 sc_out sc_logic 1 signal 30 } 
	{ buffer16_key_we0 sc_out sc_logic 1 signal 30 } 
	{ buffer16_key_d0 sc_out sc_lv 32 signal 30 } 
	{ buffer16_value_address0 sc_out sc_lv 12 signal 31 } 
	{ buffer16_value_ce0 sc_out sc_logic 1 signal 31 } 
	{ buffer16_value_we0 sc_out sc_logic 1 signal 31 } 
	{ buffer16_value_d0 sc_out sc_lv 32 signal 31 } 
	{ buffer17_key_address0 sc_out sc_lv 12 signal 32 } 
	{ buffer17_key_ce0 sc_out sc_logic 1 signal 32 } 
	{ buffer17_key_we0 sc_out sc_logic 1 signal 32 } 
	{ buffer17_key_d0 sc_out sc_lv 32 signal 32 } 
	{ buffer17_value_address0 sc_out sc_lv 12 signal 33 } 
	{ buffer17_value_ce0 sc_out sc_logic 1 signal 33 } 
	{ buffer17_value_we0 sc_out sc_logic 1 signal 33 } 
	{ buffer17_value_d0 sc_out sc_lv 32 signal 33 } 
	{ buffer20_key_address0 sc_out sc_lv 12 signal 34 } 
	{ buffer20_key_ce0 sc_out sc_logic 1 signal 34 } 
	{ buffer20_key_we0 sc_out sc_logic 1 signal 34 } 
	{ buffer20_key_d0 sc_out sc_lv 32 signal 34 } 
	{ buffer20_value_address0 sc_out sc_lv 12 signal 35 } 
	{ buffer20_value_ce0 sc_out sc_logic 1 signal 35 } 
	{ buffer20_value_we0 sc_out sc_logic 1 signal 35 } 
	{ buffer20_value_d0 sc_out sc_lv 32 signal 35 } 
	{ buffer21_key_address0 sc_out sc_lv 12 signal 36 } 
	{ buffer21_key_ce0 sc_out sc_logic 1 signal 36 } 
	{ buffer21_key_we0 sc_out sc_logic 1 signal 36 } 
	{ buffer21_key_d0 sc_out sc_lv 32 signal 36 } 
	{ buffer21_value_address0 sc_out sc_lv 12 signal 37 } 
	{ buffer21_value_ce0 sc_out sc_logic 1 signal 37 } 
	{ buffer21_value_we0 sc_out sc_logic 1 signal 37 } 
	{ buffer21_value_d0 sc_out sc_lv 32 signal 37 } 
	{ buffer22_key_address0 sc_out sc_lv 12 signal 38 } 
	{ buffer22_key_ce0 sc_out sc_logic 1 signal 38 } 
	{ buffer22_key_we0 sc_out sc_logic 1 signal 38 } 
	{ buffer22_key_d0 sc_out sc_lv 32 signal 38 } 
	{ buffer22_value_address0 sc_out sc_lv 12 signal 39 } 
	{ buffer22_value_ce0 sc_out sc_logic 1 signal 39 } 
	{ buffer22_value_we0 sc_out sc_logic 1 signal 39 } 
	{ buffer22_value_d0 sc_out sc_lv 32 signal 39 } 
	{ buffer23_key_address0 sc_out sc_lv 12 signal 40 } 
	{ buffer23_key_ce0 sc_out sc_logic 1 signal 40 } 
	{ buffer23_key_we0 sc_out sc_logic 1 signal 40 } 
	{ buffer23_key_d0 sc_out sc_lv 32 signal 40 } 
	{ buffer23_value_address0 sc_out sc_lv 12 signal 41 } 
	{ buffer23_value_ce0 sc_out sc_logic 1 signal 41 } 
	{ buffer23_value_we0 sc_out sc_logic 1 signal 41 } 
	{ buffer23_value_d0 sc_out sc_lv 32 signal 41 } 
	{ buffer24_key_address0 sc_out sc_lv 12 signal 42 } 
	{ buffer24_key_ce0 sc_out sc_logic 1 signal 42 } 
	{ buffer24_key_we0 sc_out sc_logic 1 signal 42 } 
	{ buffer24_key_d0 sc_out sc_lv 32 signal 42 } 
	{ buffer24_value_address0 sc_out sc_lv 12 signal 43 } 
	{ buffer24_value_ce0 sc_out sc_logic 1 signal 43 } 
	{ buffer24_value_we0 sc_out sc_logic 1 signal 43 } 
	{ buffer24_value_d0 sc_out sc_lv 32 signal 43 } 
	{ buffer25_key_address0 sc_out sc_lv 12 signal 44 } 
	{ buffer25_key_ce0 sc_out sc_logic 1 signal 44 } 
	{ buffer25_key_we0 sc_out sc_logic 1 signal 44 } 
	{ buffer25_key_d0 sc_out sc_lv 32 signal 44 } 
	{ buffer25_value_address0 sc_out sc_lv 12 signal 45 } 
	{ buffer25_value_ce0 sc_out sc_logic 1 signal 45 } 
	{ buffer25_value_we0 sc_out sc_logic 1 signal 45 } 
	{ buffer25_value_d0 sc_out sc_lv 32 signal 45 } 
	{ buffer26_key_address0 sc_out sc_lv 12 signal 46 } 
	{ buffer26_key_ce0 sc_out sc_logic 1 signal 46 } 
	{ buffer26_key_we0 sc_out sc_logic 1 signal 46 } 
	{ buffer26_key_d0 sc_out sc_lv 32 signal 46 } 
	{ buffer26_value_address0 sc_out sc_lv 12 signal 47 } 
	{ buffer26_value_ce0 sc_out sc_logic 1 signal 47 } 
	{ buffer26_value_we0 sc_out sc_logic 1 signal 47 } 
	{ buffer26_value_d0 sc_out sc_lv 32 signal 47 } 
	{ buffer27_key_address0 sc_out sc_lv 12 signal 48 } 
	{ buffer27_key_ce0 sc_out sc_logic 1 signal 48 } 
	{ buffer27_key_we0 sc_out sc_logic 1 signal 48 } 
	{ buffer27_key_d0 sc_out sc_lv 32 signal 48 } 
	{ buffer27_value_address0 sc_out sc_lv 12 signal 49 } 
	{ buffer27_value_ce0 sc_out sc_logic 1 signal 49 } 
	{ buffer27_value_we0 sc_out sc_logic 1 signal 49 } 
	{ buffer27_value_d0 sc_out sc_lv 32 signal 49 } 
	{ buffer30_key_address0 sc_out sc_lv 12 signal 50 } 
	{ buffer30_key_ce0 sc_out sc_logic 1 signal 50 } 
	{ buffer30_key_we0 sc_out sc_logic 1 signal 50 } 
	{ buffer30_key_d0 sc_out sc_lv 32 signal 50 } 
	{ buffer30_value_address0 sc_out sc_lv 12 signal 51 } 
	{ buffer30_value_ce0 sc_out sc_logic 1 signal 51 } 
	{ buffer30_value_we0 sc_out sc_logic 1 signal 51 } 
	{ buffer30_value_d0 sc_out sc_lv 32 signal 51 } 
	{ buffer31_key_address0 sc_out sc_lv 12 signal 52 } 
	{ buffer31_key_ce0 sc_out sc_logic 1 signal 52 } 
	{ buffer31_key_we0 sc_out sc_logic 1 signal 52 } 
	{ buffer31_key_d0 sc_out sc_lv 32 signal 52 } 
	{ buffer31_value_address0 sc_out sc_lv 12 signal 53 } 
	{ buffer31_value_ce0 sc_out sc_logic 1 signal 53 } 
	{ buffer31_value_we0 sc_out sc_logic 1 signal 53 } 
	{ buffer31_value_d0 sc_out sc_lv 32 signal 53 } 
	{ buffer32_key_address0 sc_out sc_lv 12 signal 54 } 
	{ buffer32_key_ce0 sc_out sc_logic 1 signal 54 } 
	{ buffer32_key_we0 sc_out sc_logic 1 signal 54 } 
	{ buffer32_key_d0 sc_out sc_lv 32 signal 54 } 
	{ buffer32_value_address0 sc_out sc_lv 12 signal 55 } 
	{ buffer32_value_ce0 sc_out sc_logic 1 signal 55 } 
	{ buffer32_value_we0 sc_out sc_logic 1 signal 55 } 
	{ buffer32_value_d0 sc_out sc_lv 32 signal 55 } 
	{ buffer33_key_address0 sc_out sc_lv 12 signal 56 } 
	{ buffer33_key_ce0 sc_out sc_logic 1 signal 56 } 
	{ buffer33_key_we0 sc_out sc_logic 1 signal 56 } 
	{ buffer33_key_d0 sc_out sc_lv 32 signal 56 } 
	{ buffer33_value_address0 sc_out sc_lv 12 signal 57 } 
	{ buffer33_value_ce0 sc_out sc_logic 1 signal 57 } 
	{ buffer33_value_we0 sc_out sc_logic 1 signal 57 } 
	{ buffer33_value_d0 sc_out sc_lv 32 signal 57 } 
	{ buffer34_key_address0 sc_out sc_lv 12 signal 58 } 
	{ buffer34_key_ce0 sc_out sc_logic 1 signal 58 } 
	{ buffer34_key_we0 sc_out sc_logic 1 signal 58 } 
	{ buffer34_key_d0 sc_out sc_lv 32 signal 58 } 
	{ buffer34_value_address0 sc_out sc_lv 12 signal 59 } 
	{ buffer34_value_ce0 sc_out sc_logic 1 signal 59 } 
	{ buffer34_value_we0 sc_out sc_logic 1 signal 59 } 
	{ buffer34_value_d0 sc_out sc_lv 32 signal 59 } 
	{ buffer35_key_address0 sc_out sc_lv 12 signal 60 } 
	{ buffer35_key_ce0 sc_out sc_logic 1 signal 60 } 
	{ buffer35_key_we0 sc_out sc_logic 1 signal 60 } 
	{ buffer35_key_d0 sc_out sc_lv 32 signal 60 } 
	{ buffer35_value_address0 sc_out sc_lv 12 signal 61 } 
	{ buffer35_value_ce0 sc_out sc_logic 1 signal 61 } 
	{ buffer35_value_we0 sc_out sc_logic 1 signal 61 } 
	{ buffer35_value_d0 sc_out sc_lv 32 signal 61 } 
	{ buffer36_key_address0 sc_out sc_lv 12 signal 62 } 
	{ buffer36_key_ce0 sc_out sc_logic 1 signal 62 } 
	{ buffer36_key_we0 sc_out sc_logic 1 signal 62 } 
	{ buffer36_key_d0 sc_out sc_lv 32 signal 62 } 
	{ buffer36_value_address0 sc_out sc_lv 12 signal 63 } 
	{ buffer36_value_ce0 sc_out sc_logic 1 signal 63 } 
	{ buffer36_value_we0 sc_out sc_logic 1 signal 63 } 
	{ buffer36_value_d0 sc_out sc_lv 32 signal 63 } 
	{ buffer37_key_address0 sc_out sc_lv 12 signal 64 } 
	{ buffer37_key_ce0 sc_out sc_logic 1 signal 64 } 
	{ buffer37_key_we0 sc_out sc_logic 1 signal 64 } 
	{ buffer37_key_d0 sc_out sc_lv 32 signal 64 } 
	{ buffer37_value_address0 sc_out sc_lv 12 signal 65 } 
	{ buffer37_value_ce0 sc_out sc_logic 1 signal 65 } 
	{ buffer37_value_we0 sc_out sc_logic 1 signal 65 } 
	{ buffer37_value_d0 sc_out sc_lv 32 signal 65 } 
	{ offset_kvs sc_in sc_lv 30 signal 66 } 
}
set NewPortList {[ 
	{ "name": "ap_clk", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "clock", "bundle":{"name": "ap_clk", "role": "default" }} , 
 	{ "name": "ap_rst", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "reset", "bundle":{"name": "ap_rst", "role": "default" }} , 
 	{ "name": "ap_start", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "start", "bundle":{"name": "ap_start", "role": "default" }} , 
 	{ "name": "ap_done", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "predone", "bundle":{"name": "ap_done", "role": "default" }} , 
 	{ "name": "ap_idle", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "done", "bundle":{"name": "ap_idle", "role": "default" }} , 
 	{ "name": "ap_ready", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "ready", "bundle":{"name": "ap_ready", "role": "default" }} , 
 	{ "name": "m_axi_kvdram_V_AWVALID", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "kvdram_V", "role": "AWVALID" }} , 
 	{ "name": "m_axi_kvdram_V_AWREADY", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "kvdram_V", "role": "AWREADY" }} , 
 	{ "name": "m_axi_kvdram_V_AWADDR", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "kvdram_V", "role": "AWADDR" }} , 
 	{ "name": "m_axi_kvdram_V_AWID", "direction": "out", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "kvdram_V", "role": "AWID" }} , 
 	{ "name": "m_axi_kvdram_V_AWLEN", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "kvdram_V", "role": "AWLEN" }} , 
 	{ "name": "m_axi_kvdram_V_AWSIZE", "direction": "out", "datatype": "sc_lv", "bitwidth":3, "type": "signal", "bundle":{"name": "kvdram_V", "role": "AWSIZE" }} , 
 	{ "name": "m_axi_kvdram_V_AWBURST", "direction": "out", "datatype": "sc_lv", "bitwidth":2, "type": "signal", "bundle":{"name": "kvdram_V", "role": "AWBURST" }} , 
 	{ "name": "m_axi_kvdram_V_AWLOCK", "direction": "out", "datatype": "sc_lv", "bitwidth":2, "type": "signal", "bundle":{"name": "kvdram_V", "role": "AWLOCK" }} , 
 	{ "name": "m_axi_kvdram_V_AWCACHE", "direction": "out", "datatype": "sc_lv", "bitwidth":4, "type": "signal", "bundle":{"name": "kvdram_V", "role": "AWCACHE" }} , 
 	{ "name": "m_axi_kvdram_V_AWPROT", "direction": "out", "datatype": "sc_lv", "bitwidth":3, "type": "signal", "bundle":{"name": "kvdram_V", "role": "AWPROT" }} , 
 	{ "name": "m_axi_kvdram_V_AWQOS", "direction": "out", "datatype": "sc_lv", "bitwidth":4, "type": "signal", "bundle":{"name": "kvdram_V", "role": "AWQOS" }} , 
 	{ "name": "m_axi_kvdram_V_AWREGION", "direction": "out", "datatype": "sc_lv", "bitwidth":4, "type": "signal", "bundle":{"name": "kvdram_V", "role": "AWREGION" }} , 
 	{ "name": "m_axi_kvdram_V_AWUSER", "direction": "out", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "kvdram_V", "role": "AWUSER" }} , 
 	{ "name": "m_axi_kvdram_V_WVALID", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "kvdram_V", "role": "WVALID" }} , 
 	{ "name": "m_axi_kvdram_V_WREADY", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "kvdram_V", "role": "WREADY" }} , 
 	{ "name": "m_axi_kvdram_V_WDATA", "direction": "out", "datatype": "sc_lv", "bitwidth":512, "type": "signal", "bundle":{"name": "kvdram_V", "role": "WDATA" }} , 
 	{ "name": "m_axi_kvdram_V_WSTRB", "direction": "out", "datatype": "sc_lv", "bitwidth":64, "type": "signal", "bundle":{"name": "kvdram_V", "role": "WSTRB" }} , 
 	{ "name": "m_axi_kvdram_V_WLAST", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "kvdram_V", "role": "WLAST" }} , 
 	{ "name": "m_axi_kvdram_V_WID", "direction": "out", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "kvdram_V", "role": "WID" }} , 
 	{ "name": "m_axi_kvdram_V_WUSER", "direction": "out", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "kvdram_V", "role": "WUSER" }} , 
 	{ "name": "m_axi_kvdram_V_ARVALID", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "kvdram_V", "role": "ARVALID" }} , 
 	{ "name": "m_axi_kvdram_V_ARREADY", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "kvdram_V", "role": "ARREADY" }} , 
 	{ "name": "m_axi_kvdram_V_ARADDR", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "kvdram_V", "role": "ARADDR" }} , 
 	{ "name": "m_axi_kvdram_V_ARID", "direction": "out", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "kvdram_V", "role": "ARID" }} , 
 	{ "name": "m_axi_kvdram_V_ARLEN", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "kvdram_V", "role": "ARLEN" }} , 
 	{ "name": "m_axi_kvdram_V_ARSIZE", "direction": "out", "datatype": "sc_lv", "bitwidth":3, "type": "signal", "bundle":{"name": "kvdram_V", "role": "ARSIZE" }} , 
 	{ "name": "m_axi_kvdram_V_ARBURST", "direction": "out", "datatype": "sc_lv", "bitwidth":2, "type": "signal", "bundle":{"name": "kvdram_V", "role": "ARBURST" }} , 
 	{ "name": "m_axi_kvdram_V_ARLOCK", "direction": "out", "datatype": "sc_lv", "bitwidth":2, "type": "signal", "bundle":{"name": "kvdram_V", "role": "ARLOCK" }} , 
 	{ "name": "m_axi_kvdram_V_ARCACHE", "direction": "out", "datatype": "sc_lv", "bitwidth":4, "type": "signal", "bundle":{"name": "kvdram_V", "role": "ARCACHE" }} , 
 	{ "name": "m_axi_kvdram_V_ARPROT", "direction": "out", "datatype": "sc_lv", "bitwidth":3, "type": "signal", "bundle":{"name": "kvdram_V", "role": "ARPROT" }} , 
 	{ "name": "m_axi_kvdram_V_ARQOS", "direction": "out", "datatype": "sc_lv", "bitwidth":4, "type": "signal", "bundle":{"name": "kvdram_V", "role": "ARQOS" }} , 
 	{ "name": "m_axi_kvdram_V_ARREGION", "direction": "out", "datatype": "sc_lv", "bitwidth":4, "type": "signal", "bundle":{"name": "kvdram_V", "role": "ARREGION" }} , 
 	{ "name": "m_axi_kvdram_V_ARUSER", "direction": "out", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "kvdram_V", "role": "ARUSER" }} , 
 	{ "name": "m_axi_kvdram_V_RVALID", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "kvdram_V", "role": "RVALID" }} , 
 	{ "name": "m_axi_kvdram_V_RREADY", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "kvdram_V", "role": "RREADY" }} , 
 	{ "name": "m_axi_kvdram_V_RDATA", "direction": "in", "datatype": "sc_lv", "bitwidth":512, "type": "signal", "bundle":{"name": "kvdram_V", "role": "RDATA" }} , 
 	{ "name": "m_axi_kvdram_V_RLAST", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "kvdram_V", "role": "RLAST" }} , 
 	{ "name": "m_axi_kvdram_V_RID", "direction": "in", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "kvdram_V", "role": "RID" }} , 
 	{ "name": "m_axi_kvdram_V_RUSER", "direction": "in", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "kvdram_V", "role": "RUSER" }} , 
 	{ "name": "m_axi_kvdram_V_RRESP", "direction": "in", "datatype": "sc_lv", "bitwidth":2, "type": "signal", "bundle":{"name": "kvdram_V", "role": "RRESP" }} , 
 	{ "name": "m_axi_kvdram_V_BVALID", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "kvdram_V", "role": "BVALID" }} , 
 	{ "name": "m_axi_kvdram_V_BREADY", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "kvdram_V", "role": "BREADY" }} , 
 	{ "name": "m_axi_kvdram_V_BRESP", "direction": "in", "datatype": "sc_lv", "bitwidth":2, "type": "signal", "bundle":{"name": "kvdram_V", "role": "BRESP" }} , 
 	{ "name": "m_axi_kvdram_V_BID", "direction": "in", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "kvdram_V", "role": "BID" }} , 
 	{ "name": "m_axi_kvdram_V_BUSER", "direction": "in", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "kvdram_V", "role": "BUSER" }} , 
 	{ "name": "kvdram_V_offset", "direction": "in", "datatype": "sc_lv", "bitwidth":26, "type": "signal", "bundle":{"name": "kvdram_V_offset", "role": "default" }} , 
 	{ "name": "buffer00_key_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":12, "type": "signal", "bundle":{"name": "buffer00_key", "role": "address0" }} , 
 	{ "name": "buffer00_key_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "buffer00_key", "role": "ce0" }} , 
 	{ "name": "buffer00_key_we0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "buffer00_key", "role": "we0" }} , 
 	{ "name": "buffer00_key_d0", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "buffer00_key", "role": "d0" }} , 
 	{ "name": "buffer00_value_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":12, "type": "signal", "bundle":{"name": "buffer00_value", "role": "address0" }} , 
 	{ "name": "buffer00_value_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "buffer00_value", "role": "ce0" }} , 
 	{ "name": "buffer00_value_we0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "buffer00_value", "role": "we0" }} , 
 	{ "name": "buffer00_value_d0", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "buffer00_value", "role": "d0" }} , 
 	{ "name": "buffer01_key_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":12, "type": "signal", "bundle":{"name": "buffer01_key", "role": "address0" }} , 
 	{ "name": "buffer01_key_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "buffer01_key", "role": "ce0" }} , 
 	{ "name": "buffer01_key_we0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "buffer01_key", "role": "we0" }} , 
 	{ "name": "buffer01_key_d0", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "buffer01_key", "role": "d0" }} , 
 	{ "name": "buffer01_value_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":12, "type": "signal", "bundle":{"name": "buffer01_value", "role": "address0" }} , 
 	{ "name": "buffer01_value_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "buffer01_value", "role": "ce0" }} , 
 	{ "name": "buffer01_value_we0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "buffer01_value", "role": "we0" }} , 
 	{ "name": "buffer01_value_d0", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "buffer01_value", "role": "d0" }} , 
 	{ "name": "buffer02_key_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":12, "type": "signal", "bundle":{"name": "buffer02_key", "role": "address0" }} , 
 	{ "name": "buffer02_key_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "buffer02_key", "role": "ce0" }} , 
 	{ "name": "buffer02_key_we0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "buffer02_key", "role": "we0" }} , 
 	{ "name": "buffer02_key_d0", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "buffer02_key", "role": "d0" }} , 
 	{ "name": "buffer02_value_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":12, "type": "signal", "bundle":{"name": "buffer02_value", "role": "address0" }} , 
 	{ "name": "buffer02_value_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "buffer02_value", "role": "ce0" }} , 
 	{ "name": "buffer02_value_we0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "buffer02_value", "role": "we0" }} , 
 	{ "name": "buffer02_value_d0", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "buffer02_value", "role": "d0" }} , 
 	{ "name": "buffer03_key_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":12, "type": "signal", "bundle":{"name": "buffer03_key", "role": "address0" }} , 
 	{ "name": "buffer03_key_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "buffer03_key", "role": "ce0" }} , 
 	{ "name": "buffer03_key_we0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "buffer03_key", "role": "we0" }} , 
 	{ "name": "buffer03_key_d0", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "buffer03_key", "role": "d0" }} , 
 	{ "name": "buffer03_value_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":12, "type": "signal", "bundle":{"name": "buffer03_value", "role": "address0" }} , 
 	{ "name": "buffer03_value_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "buffer03_value", "role": "ce0" }} , 
 	{ "name": "buffer03_value_we0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "buffer03_value", "role": "we0" }} , 
 	{ "name": "buffer03_value_d0", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "buffer03_value", "role": "d0" }} , 
 	{ "name": "buffer04_key_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":12, "type": "signal", "bundle":{"name": "buffer04_key", "role": "address0" }} , 
 	{ "name": "buffer04_key_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "buffer04_key", "role": "ce0" }} , 
 	{ "name": "buffer04_key_we0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "buffer04_key", "role": "we0" }} , 
 	{ "name": "buffer04_key_d0", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "buffer04_key", "role": "d0" }} , 
 	{ "name": "buffer04_value_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":12, "type": "signal", "bundle":{"name": "buffer04_value", "role": "address0" }} , 
 	{ "name": "buffer04_value_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "buffer04_value", "role": "ce0" }} , 
 	{ "name": "buffer04_value_we0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "buffer04_value", "role": "we0" }} , 
 	{ "name": "buffer04_value_d0", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "buffer04_value", "role": "d0" }} , 
 	{ "name": "buffer05_key_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":12, "type": "signal", "bundle":{"name": "buffer05_key", "role": "address0" }} , 
 	{ "name": "buffer05_key_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "buffer05_key", "role": "ce0" }} , 
 	{ "name": "buffer05_key_we0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "buffer05_key", "role": "we0" }} , 
 	{ "name": "buffer05_key_d0", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "buffer05_key", "role": "d0" }} , 
 	{ "name": "buffer05_value_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":12, "type": "signal", "bundle":{"name": "buffer05_value", "role": "address0" }} , 
 	{ "name": "buffer05_value_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "buffer05_value", "role": "ce0" }} , 
 	{ "name": "buffer05_value_we0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "buffer05_value", "role": "we0" }} , 
 	{ "name": "buffer05_value_d0", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "buffer05_value", "role": "d0" }} , 
 	{ "name": "buffer06_key_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":12, "type": "signal", "bundle":{"name": "buffer06_key", "role": "address0" }} , 
 	{ "name": "buffer06_key_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "buffer06_key", "role": "ce0" }} , 
 	{ "name": "buffer06_key_we0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "buffer06_key", "role": "we0" }} , 
 	{ "name": "buffer06_key_d0", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "buffer06_key", "role": "d0" }} , 
 	{ "name": "buffer06_value_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":12, "type": "signal", "bundle":{"name": "buffer06_value", "role": "address0" }} , 
 	{ "name": "buffer06_value_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "buffer06_value", "role": "ce0" }} , 
 	{ "name": "buffer06_value_we0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "buffer06_value", "role": "we0" }} , 
 	{ "name": "buffer06_value_d0", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "buffer06_value", "role": "d0" }} , 
 	{ "name": "buffer07_key_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":12, "type": "signal", "bundle":{"name": "buffer07_key", "role": "address0" }} , 
 	{ "name": "buffer07_key_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "buffer07_key", "role": "ce0" }} , 
 	{ "name": "buffer07_key_we0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "buffer07_key", "role": "we0" }} , 
 	{ "name": "buffer07_key_d0", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "buffer07_key", "role": "d0" }} , 
 	{ "name": "buffer07_value_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":12, "type": "signal", "bundle":{"name": "buffer07_value", "role": "address0" }} , 
 	{ "name": "buffer07_value_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "buffer07_value", "role": "ce0" }} , 
 	{ "name": "buffer07_value_we0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "buffer07_value", "role": "we0" }} , 
 	{ "name": "buffer07_value_d0", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "buffer07_value", "role": "d0" }} , 
 	{ "name": "buffer10_key_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":12, "type": "signal", "bundle":{"name": "buffer10_key", "role": "address0" }} , 
 	{ "name": "buffer10_key_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "buffer10_key", "role": "ce0" }} , 
 	{ "name": "buffer10_key_we0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "buffer10_key", "role": "we0" }} , 
 	{ "name": "buffer10_key_d0", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "buffer10_key", "role": "d0" }} , 
 	{ "name": "buffer10_value_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":12, "type": "signal", "bundle":{"name": "buffer10_value", "role": "address0" }} , 
 	{ "name": "buffer10_value_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "buffer10_value", "role": "ce0" }} , 
 	{ "name": "buffer10_value_we0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "buffer10_value", "role": "we0" }} , 
 	{ "name": "buffer10_value_d0", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "buffer10_value", "role": "d0" }} , 
 	{ "name": "buffer11_key_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":12, "type": "signal", "bundle":{"name": "buffer11_key", "role": "address0" }} , 
 	{ "name": "buffer11_key_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "buffer11_key", "role": "ce0" }} , 
 	{ "name": "buffer11_key_we0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "buffer11_key", "role": "we0" }} , 
 	{ "name": "buffer11_key_d0", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "buffer11_key", "role": "d0" }} , 
 	{ "name": "buffer11_value_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":12, "type": "signal", "bundle":{"name": "buffer11_value", "role": "address0" }} , 
 	{ "name": "buffer11_value_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "buffer11_value", "role": "ce0" }} , 
 	{ "name": "buffer11_value_we0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "buffer11_value", "role": "we0" }} , 
 	{ "name": "buffer11_value_d0", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "buffer11_value", "role": "d0" }} , 
 	{ "name": "buffer12_key_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":12, "type": "signal", "bundle":{"name": "buffer12_key", "role": "address0" }} , 
 	{ "name": "buffer12_key_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "buffer12_key", "role": "ce0" }} , 
 	{ "name": "buffer12_key_we0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "buffer12_key", "role": "we0" }} , 
 	{ "name": "buffer12_key_d0", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "buffer12_key", "role": "d0" }} , 
 	{ "name": "buffer12_value_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":12, "type": "signal", "bundle":{"name": "buffer12_value", "role": "address0" }} , 
 	{ "name": "buffer12_value_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "buffer12_value", "role": "ce0" }} , 
 	{ "name": "buffer12_value_we0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "buffer12_value", "role": "we0" }} , 
 	{ "name": "buffer12_value_d0", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "buffer12_value", "role": "d0" }} , 
 	{ "name": "buffer13_key_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":12, "type": "signal", "bundle":{"name": "buffer13_key", "role": "address0" }} , 
 	{ "name": "buffer13_key_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "buffer13_key", "role": "ce0" }} , 
 	{ "name": "buffer13_key_we0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "buffer13_key", "role": "we0" }} , 
 	{ "name": "buffer13_key_d0", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "buffer13_key", "role": "d0" }} , 
 	{ "name": "buffer13_value_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":12, "type": "signal", "bundle":{"name": "buffer13_value", "role": "address0" }} , 
 	{ "name": "buffer13_value_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "buffer13_value", "role": "ce0" }} , 
 	{ "name": "buffer13_value_we0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "buffer13_value", "role": "we0" }} , 
 	{ "name": "buffer13_value_d0", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "buffer13_value", "role": "d0" }} , 
 	{ "name": "buffer14_key_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":12, "type": "signal", "bundle":{"name": "buffer14_key", "role": "address0" }} , 
 	{ "name": "buffer14_key_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "buffer14_key", "role": "ce0" }} , 
 	{ "name": "buffer14_key_we0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "buffer14_key", "role": "we0" }} , 
 	{ "name": "buffer14_key_d0", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "buffer14_key", "role": "d0" }} , 
 	{ "name": "buffer14_value_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":12, "type": "signal", "bundle":{"name": "buffer14_value", "role": "address0" }} , 
 	{ "name": "buffer14_value_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "buffer14_value", "role": "ce0" }} , 
 	{ "name": "buffer14_value_we0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "buffer14_value", "role": "we0" }} , 
 	{ "name": "buffer14_value_d0", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "buffer14_value", "role": "d0" }} , 
 	{ "name": "buffer15_key_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":12, "type": "signal", "bundle":{"name": "buffer15_key", "role": "address0" }} , 
 	{ "name": "buffer15_key_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "buffer15_key", "role": "ce0" }} , 
 	{ "name": "buffer15_key_we0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "buffer15_key", "role": "we0" }} , 
 	{ "name": "buffer15_key_d0", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "buffer15_key", "role": "d0" }} , 
 	{ "name": "buffer15_value_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":12, "type": "signal", "bundle":{"name": "buffer15_value", "role": "address0" }} , 
 	{ "name": "buffer15_value_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "buffer15_value", "role": "ce0" }} , 
 	{ "name": "buffer15_value_we0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "buffer15_value", "role": "we0" }} , 
 	{ "name": "buffer15_value_d0", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "buffer15_value", "role": "d0" }} , 
 	{ "name": "buffer16_key_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":12, "type": "signal", "bundle":{"name": "buffer16_key", "role": "address0" }} , 
 	{ "name": "buffer16_key_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "buffer16_key", "role": "ce0" }} , 
 	{ "name": "buffer16_key_we0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "buffer16_key", "role": "we0" }} , 
 	{ "name": "buffer16_key_d0", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "buffer16_key", "role": "d0" }} , 
 	{ "name": "buffer16_value_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":12, "type": "signal", "bundle":{"name": "buffer16_value", "role": "address0" }} , 
 	{ "name": "buffer16_value_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "buffer16_value", "role": "ce0" }} , 
 	{ "name": "buffer16_value_we0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "buffer16_value", "role": "we0" }} , 
 	{ "name": "buffer16_value_d0", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "buffer16_value", "role": "d0" }} , 
 	{ "name": "buffer17_key_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":12, "type": "signal", "bundle":{"name": "buffer17_key", "role": "address0" }} , 
 	{ "name": "buffer17_key_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "buffer17_key", "role": "ce0" }} , 
 	{ "name": "buffer17_key_we0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "buffer17_key", "role": "we0" }} , 
 	{ "name": "buffer17_key_d0", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "buffer17_key", "role": "d0" }} , 
 	{ "name": "buffer17_value_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":12, "type": "signal", "bundle":{"name": "buffer17_value", "role": "address0" }} , 
 	{ "name": "buffer17_value_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "buffer17_value", "role": "ce0" }} , 
 	{ "name": "buffer17_value_we0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "buffer17_value", "role": "we0" }} , 
 	{ "name": "buffer17_value_d0", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "buffer17_value", "role": "d0" }} , 
 	{ "name": "buffer20_key_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":12, "type": "signal", "bundle":{"name": "buffer20_key", "role": "address0" }} , 
 	{ "name": "buffer20_key_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "buffer20_key", "role": "ce0" }} , 
 	{ "name": "buffer20_key_we0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "buffer20_key", "role": "we0" }} , 
 	{ "name": "buffer20_key_d0", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "buffer20_key", "role": "d0" }} , 
 	{ "name": "buffer20_value_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":12, "type": "signal", "bundle":{"name": "buffer20_value", "role": "address0" }} , 
 	{ "name": "buffer20_value_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "buffer20_value", "role": "ce0" }} , 
 	{ "name": "buffer20_value_we0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "buffer20_value", "role": "we0" }} , 
 	{ "name": "buffer20_value_d0", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "buffer20_value", "role": "d0" }} , 
 	{ "name": "buffer21_key_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":12, "type": "signal", "bundle":{"name": "buffer21_key", "role": "address0" }} , 
 	{ "name": "buffer21_key_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "buffer21_key", "role": "ce0" }} , 
 	{ "name": "buffer21_key_we0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "buffer21_key", "role": "we0" }} , 
 	{ "name": "buffer21_key_d0", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "buffer21_key", "role": "d0" }} , 
 	{ "name": "buffer21_value_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":12, "type": "signal", "bundle":{"name": "buffer21_value", "role": "address0" }} , 
 	{ "name": "buffer21_value_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "buffer21_value", "role": "ce0" }} , 
 	{ "name": "buffer21_value_we0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "buffer21_value", "role": "we0" }} , 
 	{ "name": "buffer21_value_d0", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "buffer21_value", "role": "d0" }} , 
 	{ "name": "buffer22_key_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":12, "type": "signal", "bundle":{"name": "buffer22_key", "role": "address0" }} , 
 	{ "name": "buffer22_key_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "buffer22_key", "role": "ce0" }} , 
 	{ "name": "buffer22_key_we0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "buffer22_key", "role": "we0" }} , 
 	{ "name": "buffer22_key_d0", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "buffer22_key", "role": "d0" }} , 
 	{ "name": "buffer22_value_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":12, "type": "signal", "bundle":{"name": "buffer22_value", "role": "address0" }} , 
 	{ "name": "buffer22_value_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "buffer22_value", "role": "ce0" }} , 
 	{ "name": "buffer22_value_we0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "buffer22_value", "role": "we0" }} , 
 	{ "name": "buffer22_value_d0", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "buffer22_value", "role": "d0" }} , 
 	{ "name": "buffer23_key_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":12, "type": "signal", "bundle":{"name": "buffer23_key", "role": "address0" }} , 
 	{ "name": "buffer23_key_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "buffer23_key", "role": "ce0" }} , 
 	{ "name": "buffer23_key_we0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "buffer23_key", "role": "we0" }} , 
 	{ "name": "buffer23_key_d0", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "buffer23_key", "role": "d0" }} , 
 	{ "name": "buffer23_value_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":12, "type": "signal", "bundle":{"name": "buffer23_value", "role": "address0" }} , 
 	{ "name": "buffer23_value_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "buffer23_value", "role": "ce0" }} , 
 	{ "name": "buffer23_value_we0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "buffer23_value", "role": "we0" }} , 
 	{ "name": "buffer23_value_d0", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "buffer23_value", "role": "d0" }} , 
 	{ "name": "buffer24_key_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":12, "type": "signal", "bundle":{"name": "buffer24_key", "role": "address0" }} , 
 	{ "name": "buffer24_key_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "buffer24_key", "role": "ce0" }} , 
 	{ "name": "buffer24_key_we0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "buffer24_key", "role": "we0" }} , 
 	{ "name": "buffer24_key_d0", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "buffer24_key", "role": "d0" }} , 
 	{ "name": "buffer24_value_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":12, "type": "signal", "bundle":{"name": "buffer24_value", "role": "address0" }} , 
 	{ "name": "buffer24_value_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "buffer24_value", "role": "ce0" }} , 
 	{ "name": "buffer24_value_we0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "buffer24_value", "role": "we0" }} , 
 	{ "name": "buffer24_value_d0", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "buffer24_value", "role": "d0" }} , 
 	{ "name": "buffer25_key_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":12, "type": "signal", "bundle":{"name": "buffer25_key", "role": "address0" }} , 
 	{ "name": "buffer25_key_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "buffer25_key", "role": "ce0" }} , 
 	{ "name": "buffer25_key_we0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "buffer25_key", "role": "we0" }} , 
 	{ "name": "buffer25_key_d0", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "buffer25_key", "role": "d0" }} , 
 	{ "name": "buffer25_value_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":12, "type": "signal", "bundle":{"name": "buffer25_value", "role": "address0" }} , 
 	{ "name": "buffer25_value_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "buffer25_value", "role": "ce0" }} , 
 	{ "name": "buffer25_value_we0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "buffer25_value", "role": "we0" }} , 
 	{ "name": "buffer25_value_d0", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "buffer25_value", "role": "d0" }} , 
 	{ "name": "buffer26_key_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":12, "type": "signal", "bundle":{"name": "buffer26_key", "role": "address0" }} , 
 	{ "name": "buffer26_key_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "buffer26_key", "role": "ce0" }} , 
 	{ "name": "buffer26_key_we0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "buffer26_key", "role": "we0" }} , 
 	{ "name": "buffer26_key_d0", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "buffer26_key", "role": "d0" }} , 
 	{ "name": "buffer26_value_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":12, "type": "signal", "bundle":{"name": "buffer26_value", "role": "address0" }} , 
 	{ "name": "buffer26_value_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "buffer26_value", "role": "ce0" }} , 
 	{ "name": "buffer26_value_we0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "buffer26_value", "role": "we0" }} , 
 	{ "name": "buffer26_value_d0", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "buffer26_value", "role": "d0" }} , 
 	{ "name": "buffer27_key_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":12, "type": "signal", "bundle":{"name": "buffer27_key", "role": "address0" }} , 
 	{ "name": "buffer27_key_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "buffer27_key", "role": "ce0" }} , 
 	{ "name": "buffer27_key_we0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "buffer27_key", "role": "we0" }} , 
 	{ "name": "buffer27_key_d0", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "buffer27_key", "role": "d0" }} , 
 	{ "name": "buffer27_value_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":12, "type": "signal", "bundle":{"name": "buffer27_value", "role": "address0" }} , 
 	{ "name": "buffer27_value_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "buffer27_value", "role": "ce0" }} , 
 	{ "name": "buffer27_value_we0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "buffer27_value", "role": "we0" }} , 
 	{ "name": "buffer27_value_d0", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "buffer27_value", "role": "d0" }} , 
 	{ "name": "buffer30_key_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":12, "type": "signal", "bundle":{"name": "buffer30_key", "role": "address0" }} , 
 	{ "name": "buffer30_key_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "buffer30_key", "role": "ce0" }} , 
 	{ "name": "buffer30_key_we0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "buffer30_key", "role": "we0" }} , 
 	{ "name": "buffer30_key_d0", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "buffer30_key", "role": "d0" }} , 
 	{ "name": "buffer30_value_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":12, "type": "signal", "bundle":{"name": "buffer30_value", "role": "address0" }} , 
 	{ "name": "buffer30_value_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "buffer30_value", "role": "ce0" }} , 
 	{ "name": "buffer30_value_we0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "buffer30_value", "role": "we0" }} , 
 	{ "name": "buffer30_value_d0", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "buffer30_value", "role": "d0" }} , 
 	{ "name": "buffer31_key_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":12, "type": "signal", "bundle":{"name": "buffer31_key", "role": "address0" }} , 
 	{ "name": "buffer31_key_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "buffer31_key", "role": "ce0" }} , 
 	{ "name": "buffer31_key_we0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "buffer31_key", "role": "we0" }} , 
 	{ "name": "buffer31_key_d0", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "buffer31_key", "role": "d0" }} , 
 	{ "name": "buffer31_value_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":12, "type": "signal", "bundle":{"name": "buffer31_value", "role": "address0" }} , 
 	{ "name": "buffer31_value_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "buffer31_value", "role": "ce0" }} , 
 	{ "name": "buffer31_value_we0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "buffer31_value", "role": "we0" }} , 
 	{ "name": "buffer31_value_d0", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "buffer31_value", "role": "d0" }} , 
 	{ "name": "buffer32_key_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":12, "type": "signal", "bundle":{"name": "buffer32_key", "role": "address0" }} , 
 	{ "name": "buffer32_key_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "buffer32_key", "role": "ce0" }} , 
 	{ "name": "buffer32_key_we0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "buffer32_key", "role": "we0" }} , 
 	{ "name": "buffer32_key_d0", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "buffer32_key", "role": "d0" }} , 
 	{ "name": "buffer32_value_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":12, "type": "signal", "bundle":{"name": "buffer32_value", "role": "address0" }} , 
 	{ "name": "buffer32_value_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "buffer32_value", "role": "ce0" }} , 
 	{ "name": "buffer32_value_we0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "buffer32_value", "role": "we0" }} , 
 	{ "name": "buffer32_value_d0", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "buffer32_value", "role": "d0" }} , 
 	{ "name": "buffer33_key_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":12, "type": "signal", "bundle":{"name": "buffer33_key", "role": "address0" }} , 
 	{ "name": "buffer33_key_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "buffer33_key", "role": "ce0" }} , 
 	{ "name": "buffer33_key_we0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "buffer33_key", "role": "we0" }} , 
 	{ "name": "buffer33_key_d0", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "buffer33_key", "role": "d0" }} , 
 	{ "name": "buffer33_value_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":12, "type": "signal", "bundle":{"name": "buffer33_value", "role": "address0" }} , 
 	{ "name": "buffer33_value_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "buffer33_value", "role": "ce0" }} , 
 	{ "name": "buffer33_value_we0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "buffer33_value", "role": "we0" }} , 
 	{ "name": "buffer33_value_d0", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "buffer33_value", "role": "d0" }} , 
 	{ "name": "buffer34_key_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":12, "type": "signal", "bundle":{"name": "buffer34_key", "role": "address0" }} , 
 	{ "name": "buffer34_key_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "buffer34_key", "role": "ce0" }} , 
 	{ "name": "buffer34_key_we0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "buffer34_key", "role": "we0" }} , 
 	{ "name": "buffer34_key_d0", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "buffer34_key", "role": "d0" }} , 
 	{ "name": "buffer34_value_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":12, "type": "signal", "bundle":{"name": "buffer34_value", "role": "address0" }} , 
 	{ "name": "buffer34_value_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "buffer34_value", "role": "ce0" }} , 
 	{ "name": "buffer34_value_we0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "buffer34_value", "role": "we0" }} , 
 	{ "name": "buffer34_value_d0", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "buffer34_value", "role": "d0" }} , 
 	{ "name": "buffer35_key_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":12, "type": "signal", "bundle":{"name": "buffer35_key", "role": "address0" }} , 
 	{ "name": "buffer35_key_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "buffer35_key", "role": "ce0" }} , 
 	{ "name": "buffer35_key_we0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "buffer35_key", "role": "we0" }} , 
 	{ "name": "buffer35_key_d0", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "buffer35_key", "role": "d0" }} , 
 	{ "name": "buffer35_value_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":12, "type": "signal", "bundle":{"name": "buffer35_value", "role": "address0" }} , 
 	{ "name": "buffer35_value_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "buffer35_value", "role": "ce0" }} , 
 	{ "name": "buffer35_value_we0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "buffer35_value", "role": "we0" }} , 
 	{ "name": "buffer35_value_d0", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "buffer35_value", "role": "d0" }} , 
 	{ "name": "buffer36_key_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":12, "type": "signal", "bundle":{"name": "buffer36_key", "role": "address0" }} , 
 	{ "name": "buffer36_key_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "buffer36_key", "role": "ce0" }} , 
 	{ "name": "buffer36_key_we0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "buffer36_key", "role": "we0" }} , 
 	{ "name": "buffer36_key_d0", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "buffer36_key", "role": "d0" }} , 
 	{ "name": "buffer36_value_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":12, "type": "signal", "bundle":{"name": "buffer36_value", "role": "address0" }} , 
 	{ "name": "buffer36_value_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "buffer36_value", "role": "ce0" }} , 
 	{ "name": "buffer36_value_we0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "buffer36_value", "role": "we0" }} , 
 	{ "name": "buffer36_value_d0", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "buffer36_value", "role": "d0" }} , 
 	{ "name": "buffer37_key_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":12, "type": "signal", "bundle":{"name": "buffer37_key", "role": "address0" }} , 
 	{ "name": "buffer37_key_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "buffer37_key", "role": "ce0" }} , 
 	{ "name": "buffer37_key_we0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "buffer37_key", "role": "we0" }} , 
 	{ "name": "buffer37_key_d0", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "buffer37_key", "role": "d0" }} , 
 	{ "name": "buffer37_value_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":12, "type": "signal", "bundle":{"name": "buffer37_value", "role": "address0" }} , 
 	{ "name": "buffer37_value_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "buffer37_value", "role": "ce0" }} , 
 	{ "name": "buffer37_value_we0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "buffer37_value", "role": "we0" }} , 
 	{ "name": "buffer37_value_d0", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "buffer37_value", "role": "d0" }} , 
 	{ "name": "offset_kvs", "direction": "in", "datatype": "sc_lv", "bitwidth":30, "type": "signal", "bundle":{"name": "offset_kvs", "role": "default" }}  ]}

set RtlHierarchyInfo {[
	{"ID" : "0", "Level" : "0", "Path" : "`AUTOTB_DUT_INST", "Parent" : "",
		"CDFG" : "readkeyvalues0_1",
		"Protocol" : "ap_ctrl_hs",
		"ControlExist" : "1", "ap_start" : "1", "ap_ready" : "1", "ap_done" : "1", "ap_continue" : "0", "ap_idle" : "1",
		"Pipeline" : "None", "UnalignedPipeline" : "0", "RewindPipeline" : "0", "ProcessNetwork" : "0",
		"II" : "0",
		"VariableLatency" : "1", "ExactLatency" : "-1", "EstimateLatencyMin" : "266", "EstimateLatencyMax" : "266",
		"Combinational" : "0",
		"Datapath" : "0",
		"ClockEnable" : "0",
		"HasSubDataflow" : "0",
		"InDataflowNetwork" : "0",
		"HasNonBlockingOperation" : "0",
		"Port" : [
			{"Name" : "kvdram_V", "Type" : "MAXI", "Direction" : "I",
				"BlockSignal" : [
					{"Name" : "kvdram_V_blk_n_AR", "Type" : "RtlSignal"},
					{"Name" : "kvdram_V_blk_n_R", "Type" : "RtlSignal"}]},
			{"Name" : "kvdram_V_offset", "Type" : "None", "Direction" : "I"},
			{"Name" : "buffer00_key", "Type" : "Memory", "Direction" : "O"},
			{"Name" : "buffer00_value", "Type" : "Memory", "Direction" : "O"},
			{"Name" : "buffer01_key", "Type" : "Memory", "Direction" : "O"},
			{"Name" : "buffer01_value", "Type" : "Memory", "Direction" : "O"},
			{"Name" : "buffer02_key", "Type" : "Memory", "Direction" : "O"},
			{"Name" : "buffer02_value", "Type" : "Memory", "Direction" : "O"},
			{"Name" : "buffer03_key", "Type" : "Memory", "Direction" : "O"},
			{"Name" : "buffer03_value", "Type" : "Memory", "Direction" : "O"},
			{"Name" : "buffer04_key", "Type" : "Memory", "Direction" : "O"},
			{"Name" : "buffer04_value", "Type" : "Memory", "Direction" : "O"},
			{"Name" : "buffer05_key", "Type" : "Memory", "Direction" : "O"},
			{"Name" : "buffer05_value", "Type" : "Memory", "Direction" : "O"},
			{"Name" : "buffer06_key", "Type" : "Memory", "Direction" : "O"},
			{"Name" : "buffer06_value", "Type" : "Memory", "Direction" : "O"},
			{"Name" : "buffer07_key", "Type" : "Memory", "Direction" : "O"},
			{"Name" : "buffer07_value", "Type" : "Memory", "Direction" : "O"},
			{"Name" : "buffer10_key", "Type" : "Memory", "Direction" : "O"},
			{"Name" : "buffer10_value", "Type" : "Memory", "Direction" : "O"},
			{"Name" : "buffer11_key", "Type" : "Memory", "Direction" : "O"},
			{"Name" : "buffer11_value", "Type" : "Memory", "Direction" : "O"},
			{"Name" : "buffer12_key", "Type" : "Memory", "Direction" : "O"},
			{"Name" : "buffer12_value", "Type" : "Memory", "Direction" : "O"},
			{"Name" : "buffer13_key", "Type" : "Memory", "Direction" : "O"},
			{"Name" : "buffer13_value", "Type" : "Memory", "Direction" : "O"},
			{"Name" : "buffer14_key", "Type" : "Memory", "Direction" : "O"},
			{"Name" : "buffer14_value", "Type" : "Memory", "Direction" : "O"},
			{"Name" : "buffer15_key", "Type" : "Memory", "Direction" : "O"},
			{"Name" : "buffer15_value", "Type" : "Memory", "Direction" : "O"},
			{"Name" : "buffer16_key", "Type" : "Memory", "Direction" : "O"},
			{"Name" : "buffer16_value", "Type" : "Memory", "Direction" : "O"},
			{"Name" : "buffer17_key", "Type" : "Memory", "Direction" : "O"},
			{"Name" : "buffer17_value", "Type" : "Memory", "Direction" : "O"},
			{"Name" : "buffer20_key", "Type" : "Memory", "Direction" : "O"},
			{"Name" : "buffer20_value", "Type" : "Memory", "Direction" : "O"},
			{"Name" : "buffer21_key", "Type" : "Memory", "Direction" : "O"},
			{"Name" : "buffer21_value", "Type" : "Memory", "Direction" : "O"},
			{"Name" : "buffer22_key", "Type" : "Memory", "Direction" : "O"},
			{"Name" : "buffer22_value", "Type" : "Memory", "Direction" : "O"},
			{"Name" : "buffer23_key", "Type" : "Memory", "Direction" : "O"},
			{"Name" : "buffer23_value", "Type" : "Memory", "Direction" : "O"},
			{"Name" : "buffer24_key", "Type" : "Memory", "Direction" : "O"},
			{"Name" : "buffer24_value", "Type" : "Memory", "Direction" : "O"},
			{"Name" : "buffer25_key", "Type" : "Memory", "Direction" : "O"},
			{"Name" : "buffer25_value", "Type" : "Memory", "Direction" : "O"},
			{"Name" : "buffer26_key", "Type" : "Memory", "Direction" : "O"},
			{"Name" : "buffer26_value", "Type" : "Memory", "Direction" : "O"},
			{"Name" : "buffer27_key", "Type" : "Memory", "Direction" : "O"},
			{"Name" : "buffer27_value", "Type" : "Memory", "Direction" : "O"},
			{"Name" : "buffer30_key", "Type" : "Memory", "Direction" : "O"},
			{"Name" : "buffer30_value", "Type" : "Memory", "Direction" : "O"},
			{"Name" : "buffer31_key", "Type" : "Memory", "Direction" : "O"},
			{"Name" : "buffer31_value", "Type" : "Memory", "Direction" : "O"},
			{"Name" : "buffer32_key", "Type" : "Memory", "Direction" : "O"},
			{"Name" : "buffer32_value", "Type" : "Memory", "Direction" : "O"},
			{"Name" : "buffer33_key", "Type" : "Memory", "Direction" : "O"},
			{"Name" : "buffer33_value", "Type" : "Memory", "Direction" : "O"},
			{"Name" : "buffer34_key", "Type" : "Memory", "Direction" : "O"},
			{"Name" : "buffer34_value", "Type" : "Memory", "Direction" : "O"},
			{"Name" : "buffer35_key", "Type" : "Memory", "Direction" : "O"},
			{"Name" : "buffer35_value", "Type" : "Memory", "Direction" : "O"},
			{"Name" : "buffer36_key", "Type" : "Memory", "Direction" : "O"},
			{"Name" : "buffer36_value", "Type" : "Memory", "Direction" : "O"},
			{"Name" : "buffer37_key", "Type" : "Memory", "Direction" : "O"},
			{"Name" : "buffer37_value", "Type" : "Memory", "Direction" : "O"},
			{"Name" : "offset_kvs", "Type" : "None", "Direction" : "I"}]}]}


set ArgLastReadFirstWriteLatency {
	readkeyvalues0_1 {
		kvdram_V {Type I LastRead 9 FirstWrite -1}
		kvdram_V_offset {Type I LastRead 0 FirstWrite -1}
		buffer00_key {Type O LastRead -1 FirstWrite 10}
		buffer00_value {Type O LastRead -1 FirstWrite 10}
		buffer01_key {Type O LastRead -1 FirstWrite 10}
		buffer01_value {Type O LastRead -1 FirstWrite 10}
		buffer02_key {Type O LastRead -1 FirstWrite 10}
		buffer02_value {Type O LastRead -1 FirstWrite 10}
		buffer03_key {Type O LastRead -1 FirstWrite 10}
		buffer03_value {Type O LastRead -1 FirstWrite 10}
		buffer04_key {Type O LastRead -1 FirstWrite 10}
		buffer04_value {Type O LastRead -1 FirstWrite 10}
		buffer05_key {Type O LastRead -1 FirstWrite 10}
		buffer05_value {Type O LastRead -1 FirstWrite 10}
		buffer06_key {Type O LastRead -1 FirstWrite 10}
		buffer06_value {Type O LastRead -1 FirstWrite 10}
		buffer07_key {Type O LastRead -1 FirstWrite 10}
		buffer07_value {Type O LastRead -1 FirstWrite 10}
		buffer10_key {Type O LastRead -1 FirstWrite 10}
		buffer10_value {Type O LastRead -1 FirstWrite 10}
		buffer11_key {Type O LastRead -1 FirstWrite 10}
		buffer11_value {Type O LastRead -1 FirstWrite 10}
		buffer12_key {Type O LastRead -1 FirstWrite 10}
		buffer12_value {Type O LastRead -1 FirstWrite 10}
		buffer13_key {Type O LastRead -1 FirstWrite 10}
		buffer13_value {Type O LastRead -1 FirstWrite 10}
		buffer14_key {Type O LastRead -1 FirstWrite 10}
		buffer14_value {Type O LastRead -1 FirstWrite 10}
		buffer15_key {Type O LastRead -1 FirstWrite 10}
		buffer15_value {Type O LastRead -1 FirstWrite 10}
		buffer16_key {Type O LastRead -1 FirstWrite 10}
		buffer16_value {Type O LastRead -1 FirstWrite 10}
		buffer17_key {Type O LastRead -1 FirstWrite 10}
		buffer17_value {Type O LastRead -1 FirstWrite 10}
		buffer20_key {Type O LastRead -1 FirstWrite 10}
		buffer20_value {Type O LastRead -1 FirstWrite 10}
		buffer21_key {Type O LastRead -1 FirstWrite 10}
		buffer21_value {Type O LastRead -1 FirstWrite 10}
		buffer22_key {Type O LastRead -1 FirstWrite 10}
		buffer22_value {Type O LastRead -1 FirstWrite 10}
		buffer23_key {Type O LastRead -1 FirstWrite 10}
		buffer23_value {Type O LastRead -1 FirstWrite 10}
		buffer24_key {Type O LastRead -1 FirstWrite 10}
		buffer24_value {Type O LastRead -1 FirstWrite 10}
		buffer25_key {Type O LastRead -1 FirstWrite 10}
		buffer25_value {Type O LastRead -1 FirstWrite 10}
		buffer26_key {Type O LastRead -1 FirstWrite 10}
		buffer26_value {Type O LastRead -1 FirstWrite 10}
		buffer27_key {Type O LastRead -1 FirstWrite 10}
		buffer27_value {Type O LastRead -1 FirstWrite 10}
		buffer30_key {Type O LastRead -1 FirstWrite 10}
		buffer30_value {Type O LastRead -1 FirstWrite 10}
		buffer31_key {Type O LastRead -1 FirstWrite 10}
		buffer31_value {Type O LastRead -1 FirstWrite 10}
		buffer32_key {Type O LastRead -1 FirstWrite 10}
		buffer32_value {Type O LastRead -1 FirstWrite 10}
		buffer33_key {Type O LastRead -1 FirstWrite 10}
		buffer33_value {Type O LastRead -1 FirstWrite 10}
		buffer34_key {Type O LastRead -1 FirstWrite 10}
		buffer34_value {Type O LastRead -1 FirstWrite 10}
		buffer35_key {Type O LastRead -1 FirstWrite 10}
		buffer35_value {Type O LastRead -1 FirstWrite 10}
		buffer36_key {Type O LastRead -1 FirstWrite 10}
		buffer36_value {Type O LastRead -1 FirstWrite 10}
		buffer37_key {Type O LastRead -1 FirstWrite 10}
		buffer37_value {Type O LastRead -1 FirstWrite 10}
		offset_kvs {Type I LastRead 0 FirstWrite -1}}}

set hasDtUnsupportedChannel 0

set PerformanceInfo {[
	{"Name" : "Latency", "Min" : "266", "Max" : "266"}
	, {"Name" : "Interval", "Min" : "266", "Max" : "266"}
]}

set PipelineEnableSignalInfo {[
	{"Pipeline" : "0", "EnableSignal" : "ap_enable_pp0"}
]}

set Spec2ImplPortList { 
	kvdram_V { m_axi {  { m_axi_kvdram_V_AWVALID VALID 1 1 }  { m_axi_kvdram_V_AWREADY READY 0 1 }  { m_axi_kvdram_V_AWADDR ADDR 1 32 }  { m_axi_kvdram_V_AWID ID 1 1 }  { m_axi_kvdram_V_AWLEN LEN 1 32 }  { m_axi_kvdram_V_AWSIZE SIZE 1 3 }  { m_axi_kvdram_V_AWBURST BURST 1 2 }  { m_axi_kvdram_V_AWLOCK LOCK 1 2 }  { m_axi_kvdram_V_AWCACHE CACHE 1 4 }  { m_axi_kvdram_V_AWPROT PROT 1 3 }  { m_axi_kvdram_V_AWQOS QOS 1 4 }  { m_axi_kvdram_V_AWREGION REGION 1 4 }  { m_axi_kvdram_V_AWUSER USER 1 1 }  { m_axi_kvdram_V_WVALID VALID 1 1 }  { m_axi_kvdram_V_WREADY READY 0 1 }  { m_axi_kvdram_V_WDATA DATA 1 512 }  { m_axi_kvdram_V_WSTRB STRB 1 64 }  { m_axi_kvdram_V_WLAST LAST 1 1 }  { m_axi_kvdram_V_WID ID 1 1 }  { m_axi_kvdram_V_WUSER USER 1 1 }  { m_axi_kvdram_V_ARVALID VALID 1 1 }  { m_axi_kvdram_V_ARREADY READY 0 1 }  { m_axi_kvdram_V_ARADDR ADDR 1 32 }  { m_axi_kvdram_V_ARID ID 1 1 }  { m_axi_kvdram_V_ARLEN LEN 1 32 }  { m_axi_kvdram_V_ARSIZE SIZE 1 3 }  { m_axi_kvdram_V_ARBURST BURST 1 2 }  { m_axi_kvdram_V_ARLOCK LOCK 1 2 }  { m_axi_kvdram_V_ARCACHE CACHE 1 4 }  { m_axi_kvdram_V_ARPROT PROT 1 3 }  { m_axi_kvdram_V_ARQOS QOS 1 4 }  { m_axi_kvdram_V_ARREGION REGION 1 4 }  { m_axi_kvdram_V_ARUSER USER 1 1 }  { m_axi_kvdram_V_RVALID VALID 0 1 }  { m_axi_kvdram_V_RREADY READY 1 1 }  { m_axi_kvdram_V_RDATA DATA 0 512 }  { m_axi_kvdram_V_RLAST LAST 0 1 }  { m_axi_kvdram_V_RID ID 0 1 }  { m_axi_kvdram_V_RUSER USER 0 1 }  { m_axi_kvdram_V_RRESP RESP 0 2 }  { m_axi_kvdram_V_BVALID VALID 0 1 }  { m_axi_kvdram_V_BREADY READY 1 1 }  { m_axi_kvdram_V_BRESP RESP 0 2 }  { m_axi_kvdram_V_BID ID 0 1 }  { m_axi_kvdram_V_BUSER USER 0 1 } } }
	kvdram_V_offset { ap_none {  { kvdram_V_offset in_data 0 26 } } }
	buffer00_key { ap_memory {  { buffer00_key_address0 mem_address 1 12 }  { buffer00_key_ce0 mem_ce 1 1 }  { buffer00_key_we0 mem_we 1 1 }  { buffer00_key_d0 mem_din 1 32 } } }
	buffer00_value { ap_memory {  { buffer00_value_address0 mem_address 1 12 }  { buffer00_value_ce0 mem_ce 1 1 }  { buffer00_value_we0 mem_we 1 1 }  { buffer00_value_d0 mem_din 1 32 } } }
	buffer01_key { ap_memory {  { buffer01_key_address0 mem_address 1 12 }  { buffer01_key_ce0 mem_ce 1 1 }  { buffer01_key_we0 mem_we 1 1 }  { buffer01_key_d0 mem_din 1 32 } } }
	buffer01_value { ap_memory {  { buffer01_value_address0 mem_address 1 12 }  { buffer01_value_ce0 mem_ce 1 1 }  { buffer01_value_we0 mem_we 1 1 }  { buffer01_value_d0 mem_din 1 32 } } }
	buffer02_key { ap_memory {  { buffer02_key_address0 mem_address 1 12 }  { buffer02_key_ce0 mem_ce 1 1 }  { buffer02_key_we0 mem_we 1 1 }  { buffer02_key_d0 mem_din 1 32 } } }
	buffer02_value { ap_memory {  { buffer02_value_address0 mem_address 1 12 }  { buffer02_value_ce0 mem_ce 1 1 }  { buffer02_value_we0 mem_we 1 1 }  { buffer02_value_d0 mem_din 1 32 } } }
	buffer03_key { ap_memory {  { buffer03_key_address0 mem_address 1 12 }  { buffer03_key_ce0 mem_ce 1 1 }  { buffer03_key_we0 mem_we 1 1 }  { buffer03_key_d0 mem_din 1 32 } } }
	buffer03_value { ap_memory {  { buffer03_value_address0 mem_address 1 12 }  { buffer03_value_ce0 mem_ce 1 1 }  { buffer03_value_we0 mem_we 1 1 }  { buffer03_value_d0 mem_din 1 32 } } }
	buffer04_key { ap_memory {  { buffer04_key_address0 mem_address 1 12 }  { buffer04_key_ce0 mem_ce 1 1 }  { buffer04_key_we0 mem_we 1 1 }  { buffer04_key_d0 mem_din 1 32 } } }
	buffer04_value { ap_memory {  { buffer04_value_address0 mem_address 1 12 }  { buffer04_value_ce0 mem_ce 1 1 }  { buffer04_value_we0 mem_we 1 1 }  { buffer04_value_d0 mem_din 1 32 } } }
	buffer05_key { ap_memory {  { buffer05_key_address0 mem_address 1 12 }  { buffer05_key_ce0 mem_ce 1 1 }  { buffer05_key_we0 mem_we 1 1 }  { buffer05_key_d0 mem_din 1 32 } } }
	buffer05_value { ap_memory {  { buffer05_value_address0 mem_address 1 12 }  { buffer05_value_ce0 mem_ce 1 1 }  { buffer05_value_we0 mem_we 1 1 }  { buffer05_value_d0 mem_din 1 32 } } }
	buffer06_key { ap_memory {  { buffer06_key_address0 mem_address 1 12 }  { buffer06_key_ce0 mem_ce 1 1 }  { buffer06_key_we0 mem_we 1 1 }  { buffer06_key_d0 mem_din 1 32 } } }
	buffer06_value { ap_memory {  { buffer06_value_address0 mem_address 1 12 }  { buffer06_value_ce0 mem_ce 1 1 }  { buffer06_value_we0 mem_we 1 1 }  { buffer06_value_d0 mem_din 1 32 } } }
	buffer07_key { ap_memory {  { buffer07_key_address0 mem_address 1 12 }  { buffer07_key_ce0 mem_ce 1 1 }  { buffer07_key_we0 mem_we 1 1 }  { buffer07_key_d0 mem_din 1 32 } } }
	buffer07_value { ap_memory {  { buffer07_value_address0 mem_address 1 12 }  { buffer07_value_ce0 mem_ce 1 1 }  { buffer07_value_we0 mem_we 1 1 }  { buffer07_value_d0 mem_din 1 32 } } }
	buffer10_key { ap_memory {  { buffer10_key_address0 mem_address 1 12 }  { buffer10_key_ce0 mem_ce 1 1 }  { buffer10_key_we0 mem_we 1 1 }  { buffer10_key_d0 mem_din 1 32 } } }
	buffer10_value { ap_memory {  { buffer10_value_address0 mem_address 1 12 }  { buffer10_value_ce0 mem_ce 1 1 }  { buffer10_value_we0 mem_we 1 1 }  { buffer10_value_d0 mem_din 1 32 } } }
	buffer11_key { ap_memory {  { buffer11_key_address0 mem_address 1 12 }  { buffer11_key_ce0 mem_ce 1 1 }  { buffer11_key_we0 mem_we 1 1 }  { buffer11_key_d0 mem_din 1 32 } } }
	buffer11_value { ap_memory {  { buffer11_value_address0 mem_address 1 12 }  { buffer11_value_ce0 mem_ce 1 1 }  { buffer11_value_we0 mem_we 1 1 }  { buffer11_value_d0 mem_din 1 32 } } }
	buffer12_key { ap_memory {  { buffer12_key_address0 mem_address 1 12 }  { buffer12_key_ce0 mem_ce 1 1 }  { buffer12_key_we0 mem_we 1 1 }  { buffer12_key_d0 mem_din 1 32 } } }
	buffer12_value { ap_memory {  { buffer12_value_address0 mem_address 1 12 }  { buffer12_value_ce0 mem_ce 1 1 }  { buffer12_value_we0 mem_we 1 1 }  { buffer12_value_d0 mem_din 1 32 } } }
	buffer13_key { ap_memory {  { buffer13_key_address0 mem_address 1 12 }  { buffer13_key_ce0 mem_ce 1 1 }  { buffer13_key_we0 mem_we 1 1 }  { buffer13_key_d0 mem_din 1 32 } } }
	buffer13_value { ap_memory {  { buffer13_value_address0 mem_address 1 12 }  { buffer13_value_ce0 mem_ce 1 1 }  { buffer13_value_we0 mem_we 1 1 }  { buffer13_value_d0 mem_din 1 32 } } }
	buffer14_key { ap_memory {  { buffer14_key_address0 mem_address 1 12 }  { buffer14_key_ce0 mem_ce 1 1 }  { buffer14_key_we0 mem_we 1 1 }  { buffer14_key_d0 mem_din 1 32 } } }
	buffer14_value { ap_memory {  { buffer14_value_address0 mem_address 1 12 }  { buffer14_value_ce0 mem_ce 1 1 }  { buffer14_value_we0 mem_we 1 1 }  { buffer14_value_d0 mem_din 1 32 } } }
	buffer15_key { ap_memory {  { buffer15_key_address0 mem_address 1 12 }  { buffer15_key_ce0 mem_ce 1 1 }  { buffer15_key_we0 mem_we 1 1 }  { buffer15_key_d0 mem_din 1 32 } } }
	buffer15_value { ap_memory {  { buffer15_value_address0 mem_address 1 12 }  { buffer15_value_ce0 mem_ce 1 1 }  { buffer15_value_we0 mem_we 1 1 }  { buffer15_value_d0 mem_din 1 32 } } }
	buffer16_key { ap_memory {  { buffer16_key_address0 mem_address 1 12 }  { buffer16_key_ce0 mem_ce 1 1 }  { buffer16_key_we0 mem_we 1 1 }  { buffer16_key_d0 mem_din 1 32 } } }
	buffer16_value { ap_memory {  { buffer16_value_address0 mem_address 1 12 }  { buffer16_value_ce0 mem_ce 1 1 }  { buffer16_value_we0 mem_we 1 1 }  { buffer16_value_d0 mem_din 1 32 } } }
	buffer17_key { ap_memory {  { buffer17_key_address0 mem_address 1 12 }  { buffer17_key_ce0 mem_ce 1 1 }  { buffer17_key_we0 mem_we 1 1 }  { buffer17_key_d0 mem_din 1 32 } } }
	buffer17_value { ap_memory {  { buffer17_value_address0 mem_address 1 12 }  { buffer17_value_ce0 mem_ce 1 1 }  { buffer17_value_we0 mem_we 1 1 }  { buffer17_value_d0 mem_din 1 32 } } }
	buffer20_key { ap_memory {  { buffer20_key_address0 mem_address 1 12 }  { buffer20_key_ce0 mem_ce 1 1 }  { buffer20_key_we0 mem_we 1 1 }  { buffer20_key_d0 mem_din 1 32 } } }
	buffer20_value { ap_memory {  { buffer20_value_address0 mem_address 1 12 }  { buffer20_value_ce0 mem_ce 1 1 }  { buffer20_value_we0 mem_we 1 1 }  { buffer20_value_d0 mem_din 1 32 } } }
	buffer21_key { ap_memory {  { buffer21_key_address0 mem_address 1 12 }  { buffer21_key_ce0 mem_ce 1 1 }  { buffer21_key_we0 mem_we 1 1 }  { buffer21_key_d0 mem_din 1 32 } } }
	buffer21_value { ap_memory {  { buffer21_value_address0 mem_address 1 12 }  { buffer21_value_ce0 mem_ce 1 1 }  { buffer21_value_we0 mem_we 1 1 }  { buffer21_value_d0 mem_din 1 32 } } }
	buffer22_key { ap_memory {  { buffer22_key_address0 mem_address 1 12 }  { buffer22_key_ce0 mem_ce 1 1 }  { buffer22_key_we0 mem_we 1 1 }  { buffer22_key_d0 mem_din 1 32 } } }
	buffer22_value { ap_memory {  { buffer22_value_address0 mem_address 1 12 }  { buffer22_value_ce0 mem_ce 1 1 }  { buffer22_value_we0 mem_we 1 1 }  { buffer22_value_d0 mem_din 1 32 } } }
	buffer23_key { ap_memory {  { buffer23_key_address0 mem_address 1 12 }  { buffer23_key_ce0 mem_ce 1 1 }  { buffer23_key_we0 mem_we 1 1 }  { buffer23_key_d0 mem_din 1 32 } } }
	buffer23_value { ap_memory {  { buffer23_value_address0 mem_address 1 12 }  { buffer23_value_ce0 mem_ce 1 1 }  { buffer23_value_we0 mem_we 1 1 }  { buffer23_value_d0 mem_din 1 32 } } }
	buffer24_key { ap_memory {  { buffer24_key_address0 mem_address 1 12 }  { buffer24_key_ce0 mem_ce 1 1 }  { buffer24_key_we0 mem_we 1 1 }  { buffer24_key_d0 mem_din 1 32 } } }
	buffer24_value { ap_memory {  { buffer24_value_address0 mem_address 1 12 }  { buffer24_value_ce0 mem_ce 1 1 }  { buffer24_value_we0 mem_we 1 1 }  { buffer24_value_d0 mem_din 1 32 } } }
	buffer25_key { ap_memory {  { buffer25_key_address0 mem_address 1 12 }  { buffer25_key_ce0 mem_ce 1 1 }  { buffer25_key_we0 mem_we 1 1 }  { buffer25_key_d0 mem_din 1 32 } } }
	buffer25_value { ap_memory {  { buffer25_value_address0 mem_address 1 12 }  { buffer25_value_ce0 mem_ce 1 1 }  { buffer25_value_we0 mem_we 1 1 }  { buffer25_value_d0 mem_din 1 32 } } }
	buffer26_key { ap_memory {  { buffer26_key_address0 mem_address 1 12 }  { buffer26_key_ce0 mem_ce 1 1 }  { buffer26_key_we0 mem_we 1 1 }  { buffer26_key_d0 mem_din 1 32 } } }
	buffer26_value { ap_memory {  { buffer26_value_address0 mem_address 1 12 }  { buffer26_value_ce0 mem_ce 1 1 }  { buffer26_value_we0 mem_we 1 1 }  { buffer26_value_d0 mem_din 1 32 } } }
	buffer27_key { ap_memory {  { buffer27_key_address0 mem_address 1 12 }  { buffer27_key_ce0 mem_ce 1 1 }  { buffer27_key_we0 mem_we 1 1 }  { buffer27_key_d0 mem_din 1 32 } } }
	buffer27_value { ap_memory {  { buffer27_value_address0 mem_address 1 12 }  { buffer27_value_ce0 mem_ce 1 1 }  { buffer27_value_we0 mem_we 1 1 }  { buffer27_value_d0 mem_din 1 32 } } }
	buffer30_key { ap_memory {  { buffer30_key_address0 mem_address 1 12 }  { buffer30_key_ce0 mem_ce 1 1 }  { buffer30_key_we0 mem_we 1 1 }  { buffer30_key_d0 mem_din 1 32 } } }
	buffer30_value { ap_memory {  { buffer30_value_address0 mem_address 1 12 }  { buffer30_value_ce0 mem_ce 1 1 }  { buffer30_value_we0 mem_we 1 1 }  { buffer30_value_d0 mem_din 1 32 } } }
	buffer31_key { ap_memory {  { buffer31_key_address0 mem_address 1 12 }  { buffer31_key_ce0 mem_ce 1 1 }  { buffer31_key_we0 mem_we 1 1 }  { buffer31_key_d0 mem_din 1 32 } } }
	buffer31_value { ap_memory {  { buffer31_value_address0 mem_address 1 12 }  { buffer31_value_ce0 mem_ce 1 1 }  { buffer31_value_we0 mem_we 1 1 }  { buffer31_value_d0 mem_din 1 32 } } }
	buffer32_key { ap_memory {  { buffer32_key_address0 mem_address 1 12 }  { buffer32_key_ce0 mem_ce 1 1 }  { buffer32_key_we0 mem_we 1 1 }  { buffer32_key_d0 mem_din 1 32 } } }
	buffer32_value { ap_memory {  { buffer32_value_address0 mem_address 1 12 }  { buffer32_value_ce0 mem_ce 1 1 }  { buffer32_value_we0 mem_we 1 1 }  { buffer32_value_d0 mem_din 1 32 } } }
	buffer33_key { ap_memory {  { buffer33_key_address0 mem_address 1 12 }  { buffer33_key_ce0 mem_ce 1 1 }  { buffer33_key_we0 mem_we 1 1 }  { buffer33_key_d0 mem_din 1 32 } } }
	buffer33_value { ap_memory {  { buffer33_value_address0 mem_address 1 12 }  { buffer33_value_ce0 mem_ce 1 1 }  { buffer33_value_we0 mem_we 1 1 }  { buffer33_value_d0 mem_din 1 32 } } }
	buffer34_key { ap_memory {  { buffer34_key_address0 mem_address 1 12 }  { buffer34_key_ce0 mem_ce 1 1 }  { buffer34_key_we0 mem_we 1 1 }  { buffer34_key_d0 mem_din 1 32 } } }
	buffer34_value { ap_memory {  { buffer34_value_address0 mem_address 1 12 }  { buffer34_value_ce0 mem_ce 1 1 }  { buffer34_value_we0 mem_we 1 1 }  { buffer34_value_d0 mem_din 1 32 } } }
	buffer35_key { ap_memory {  { buffer35_key_address0 mem_address 1 12 }  { buffer35_key_ce0 mem_ce 1 1 }  { buffer35_key_we0 mem_we 1 1 }  { buffer35_key_d0 mem_din 1 32 } } }
	buffer35_value { ap_memory {  { buffer35_value_address0 mem_address 1 12 }  { buffer35_value_ce0 mem_ce 1 1 }  { buffer35_value_we0 mem_we 1 1 }  { buffer35_value_d0 mem_din 1 32 } } }
	buffer36_key { ap_memory {  { buffer36_key_address0 mem_address 1 12 }  { buffer36_key_ce0 mem_ce 1 1 }  { buffer36_key_we0 mem_we 1 1 }  { buffer36_key_d0 mem_din 1 32 } } }
	buffer36_value { ap_memory {  { buffer36_value_address0 mem_address 1 12 }  { buffer36_value_ce0 mem_ce 1 1 }  { buffer36_value_we0 mem_we 1 1 }  { buffer36_value_d0 mem_din 1 32 } } }
	buffer37_key { ap_memory {  { buffer37_key_address0 mem_address 1 12 }  { buffer37_key_ce0 mem_ce 1 1 }  { buffer37_key_we0 mem_we 1 1 }  { buffer37_key_d0 mem_din 1 32 } } }
	buffer37_value { ap_memory {  { buffer37_value_address0 mem_address 1 12 }  { buffer37_value_ce0 mem_ce 1 1 }  { buffer37_value_we0 mem_we 1 1 }  { buffer37_value_d0 mem_din 1 32 } } }
	offset_kvs { ap_none {  { offset_kvs in_data 0 30 } } }
}
