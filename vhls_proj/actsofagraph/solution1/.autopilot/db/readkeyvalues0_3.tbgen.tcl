set moduleName readkeyvalues0_3
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
set C_modelName {readkeyvalues0.3}
set C_modelType { void 0 }
set C_modelArgList {
	{ kvdram_V int 512 regular {axi_master 0}  }
	{ kvdram_V_offset int 26 regular  }
	{ buffer0_V int 512 regular {array 512 { 0 3 } 0 1 }  }
	{ buffer1_V int 512 regular {array 512 { 0 3 } 0 1 }  }
	{ buffer2_V int 512 regular {array 512 { 0 3 } 0 1 }  }
	{ buffer3_V int 512 regular {array 512 { 0 3 } 0 1 }  }
	{ baseaddress int 25 regular  }
	{ sourcestats_0_key_r int 32 regular  }
	{ sourcestats_1_key_r int 32 regular  }
	{ sourcestats_2_key_r int 32 regular  }
	{ sourcestats_3_key_r int 32 regular  }
	{ sourcestats_0_value int 32 regular  }
	{ sourcestats_1_value int 32 regular  }
	{ sourcestats_2_value int 32 regular  }
	{ sourcestats_3_value int 32 regular  }
	{ travstate_i_kvs int 32 regular  }
}
set C_modelArgMapList {[ 
	{ "Name" : "kvdram_V", "interface" : "axi_master", "bitwidth" : 512, "direction" : "READONLY"} , 
 	{ "Name" : "kvdram_V_offset", "interface" : "wire", "bitwidth" : 26, "direction" : "READONLY"} , 
 	{ "Name" : "buffer0_V", "interface" : "memory", "bitwidth" : 512, "direction" : "WRITEONLY"} , 
 	{ "Name" : "buffer1_V", "interface" : "memory", "bitwidth" : 512, "direction" : "WRITEONLY"} , 
 	{ "Name" : "buffer2_V", "interface" : "memory", "bitwidth" : 512, "direction" : "WRITEONLY"} , 
 	{ "Name" : "buffer3_V", "interface" : "memory", "bitwidth" : 512, "direction" : "WRITEONLY"} , 
 	{ "Name" : "baseaddress", "interface" : "wire", "bitwidth" : 25, "direction" : "READONLY"} , 
 	{ "Name" : "sourcestats_0_key_r", "interface" : "wire", "bitwidth" : 32, "direction" : "READONLY"} , 
 	{ "Name" : "sourcestats_1_key_r", "interface" : "wire", "bitwidth" : 32, "direction" : "READONLY"} , 
 	{ "Name" : "sourcestats_2_key_r", "interface" : "wire", "bitwidth" : 32, "direction" : "READONLY"} , 
 	{ "Name" : "sourcestats_3_key_r", "interface" : "wire", "bitwidth" : 32, "direction" : "READONLY"} , 
 	{ "Name" : "sourcestats_0_value", "interface" : "wire", "bitwidth" : 32, "direction" : "READONLY"} , 
 	{ "Name" : "sourcestats_1_value", "interface" : "wire", "bitwidth" : 32, "direction" : "READONLY"} , 
 	{ "Name" : "sourcestats_2_value", "interface" : "wire", "bitwidth" : 32, "direction" : "READONLY"} , 
 	{ "Name" : "sourcestats_3_value", "interface" : "wire", "bitwidth" : 32, "direction" : "READONLY"} , 
 	{ "Name" : "travstate_i_kvs", "interface" : "wire", "bitwidth" : 32, "direction" : "READONLY"} ]}
# RTL Port declarations: 
set portNum 78
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
	{ buffer0_V_address0 sc_out sc_lv 9 signal 2 } 
	{ buffer0_V_ce0 sc_out sc_logic 1 signal 2 } 
	{ buffer0_V_we0 sc_out sc_logic 1 signal 2 } 
	{ buffer0_V_d0 sc_out sc_lv 512 signal 2 } 
	{ buffer1_V_address0 sc_out sc_lv 9 signal 3 } 
	{ buffer1_V_ce0 sc_out sc_logic 1 signal 3 } 
	{ buffer1_V_we0 sc_out sc_logic 1 signal 3 } 
	{ buffer1_V_d0 sc_out sc_lv 512 signal 3 } 
	{ buffer2_V_address0 sc_out sc_lv 9 signal 4 } 
	{ buffer2_V_ce0 sc_out sc_logic 1 signal 4 } 
	{ buffer2_V_we0 sc_out sc_logic 1 signal 4 } 
	{ buffer2_V_d0 sc_out sc_lv 512 signal 4 } 
	{ buffer3_V_address0 sc_out sc_lv 9 signal 5 } 
	{ buffer3_V_ce0 sc_out sc_logic 1 signal 5 } 
	{ buffer3_V_we0 sc_out sc_logic 1 signal 5 } 
	{ buffer3_V_d0 sc_out sc_lv 512 signal 5 } 
	{ baseaddress sc_in sc_lv 25 signal 6 } 
	{ sourcestats_0_key_r sc_in sc_lv 32 signal 7 } 
	{ sourcestats_1_key_r sc_in sc_lv 32 signal 8 } 
	{ sourcestats_2_key_r sc_in sc_lv 32 signal 9 } 
	{ sourcestats_3_key_r sc_in sc_lv 32 signal 10 } 
	{ sourcestats_0_value sc_in sc_lv 32 signal 11 } 
	{ sourcestats_1_value sc_in sc_lv 32 signal 12 } 
	{ sourcestats_2_value sc_in sc_lv 32 signal 13 } 
	{ sourcestats_3_value sc_in sc_lv 32 signal 14 } 
	{ travstate_i_kvs sc_in sc_lv 32 signal 15 } 
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
 	{ "name": "sourcestats_0_key_r", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "sourcestats_0_key_r", "role": "default" }} , 
 	{ "name": "sourcestats_1_key_r", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "sourcestats_1_key_r", "role": "default" }} , 
 	{ "name": "sourcestats_2_key_r", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "sourcestats_2_key_r", "role": "default" }} , 
 	{ "name": "sourcestats_3_key_r", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "sourcestats_3_key_r", "role": "default" }} , 
 	{ "name": "sourcestats_0_value", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "sourcestats_0_value", "role": "default" }} , 
 	{ "name": "sourcestats_1_value", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "sourcestats_1_value", "role": "default" }} , 
 	{ "name": "sourcestats_2_value", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "sourcestats_2_value", "role": "default" }} , 
 	{ "name": "sourcestats_3_value", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "sourcestats_3_value", "role": "default" }} , 
 	{ "name": "travstate_i_kvs", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "travstate_i_kvs", "role": "default" }}  ]}

set RtlHierarchyInfo {[
	{"ID" : "0", "Level" : "0", "Path" : "`AUTOTB_DUT_INST", "Parent" : "", "Child" : ["1"],
		"CDFG" : "readkeyvalues0_3",
		"Protocol" : "ap_ctrl_hs",
		"ControlExist" : "1", "ap_start" : "1", "ap_ready" : "1", "ap_done" : "1", "ap_continue" : "0", "ap_idle" : "1",
		"Pipeline" : "None", "UnalignedPipeline" : "0", "RewindPipeline" : "0", "ProcessNetwork" : "0",
		"II" : "0",
		"VariableLatency" : "1", "ExactLatency" : "-1", "EstimateLatencyMin" : "44", "EstimateLatencyMax" : "2096",
		"Combinational" : "0",
		"Datapath" : "0",
		"ClockEnable" : "0",
		"HasSubDataflow" : "0",
		"InDataflowNetwork" : "0",
		"HasNonBlockingOperation" : "0",
		"WaitState" : [
			{"State" : "ap_ST_fsm_state3", "FSM" : "ap_CS_fsm", "SubInstance" : "grp_readkeyvalues0_166_fu_136"},
			{"State" : "ap_ST_fsm_state5", "FSM" : "ap_CS_fsm", "SubInstance" : "grp_readkeyvalues0_166_fu_136"},
			{"State" : "ap_ST_fsm_state7", "FSM" : "ap_CS_fsm", "SubInstance" : "grp_readkeyvalues0_166_fu_136"},
			{"State" : "ap_ST_fsm_state9", "FSM" : "ap_CS_fsm", "SubInstance" : "grp_readkeyvalues0_166_fu_136"}],
		"Port" : [
			{"Name" : "kvdram_V", "Type" : "MAXI", "Direction" : "I",
				"SubConnect" : [
					{"ID" : "1", "SubInstance" : "grp_readkeyvalues0_166_fu_136", "Port" : "kvdram_V"}]},
			{"Name" : "kvdram_V_offset", "Type" : "None", "Direction" : "I"},
			{"Name" : "buffer0_V", "Type" : "Memory", "Direction" : "O",
				"SubConnect" : [
					{"ID" : "1", "SubInstance" : "grp_readkeyvalues0_166_fu_136", "Port" : "buffer_V"}]},
			{"Name" : "buffer1_V", "Type" : "Memory", "Direction" : "O",
				"SubConnect" : [
					{"ID" : "1", "SubInstance" : "grp_readkeyvalues0_166_fu_136", "Port" : "buffer_V"}]},
			{"Name" : "buffer2_V", "Type" : "Memory", "Direction" : "O",
				"SubConnect" : [
					{"ID" : "1", "SubInstance" : "grp_readkeyvalues0_166_fu_136", "Port" : "buffer_V"}]},
			{"Name" : "buffer3_V", "Type" : "Memory", "Direction" : "O",
				"SubConnect" : [
					{"ID" : "1", "SubInstance" : "grp_readkeyvalues0_166_fu_136", "Port" : "buffer_V"}]},
			{"Name" : "baseaddress", "Type" : "None", "Direction" : "I"},
			{"Name" : "sourcestats_0_key_r", "Type" : "None", "Direction" : "I"},
			{"Name" : "sourcestats_1_key_r", "Type" : "None", "Direction" : "I"},
			{"Name" : "sourcestats_2_key_r", "Type" : "None", "Direction" : "I"},
			{"Name" : "sourcestats_3_key_r", "Type" : "None", "Direction" : "I"},
			{"Name" : "sourcestats_0_value", "Type" : "None", "Direction" : "I"},
			{"Name" : "sourcestats_1_value", "Type" : "None", "Direction" : "I"},
			{"Name" : "sourcestats_2_value", "Type" : "None", "Direction" : "I"},
			{"Name" : "sourcestats_3_value", "Type" : "None", "Direction" : "I"},
			{"Name" : "travstate_i_kvs", "Type" : "None", "Direction" : "I"}]},
	{"ID" : "1", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.grp_readkeyvalues0_166_fu_136", "Parent" : "0",
		"CDFG" : "readkeyvalues0_166",
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
	readkeyvalues0_3 {
		kvdram_V {Type I LastRead 9 FirstWrite -1}
		kvdram_V_offset {Type I LastRead 1 FirstWrite -1}
		buffer0_V {Type O LastRead -1 FirstWrite 10}
		buffer1_V {Type O LastRead -1 FirstWrite 10}
		buffer2_V {Type O LastRead -1 FirstWrite 10}
		buffer3_V {Type O LastRead -1 FirstWrite 10}
		baseaddress {Type I LastRead 1 FirstWrite -1}
		sourcestats_0_key_r {Type I LastRead 0 FirstWrite -1}
		sourcestats_1_key_r {Type I LastRead 2 FirstWrite -1}
		sourcestats_2_key_r {Type I LastRead 4 FirstWrite -1}
		sourcestats_3_key_r {Type I LastRead 6 FirstWrite -1}
		sourcestats_0_value {Type I LastRead 0 FirstWrite -1}
		sourcestats_1_value {Type I LastRead 2 FirstWrite -1}
		sourcestats_2_value {Type I LastRead 4 FirstWrite -1}
		sourcestats_3_value {Type I LastRead 6 FirstWrite -1}
		travstate_i_kvs {Type I LastRead 0 FirstWrite -1}}
	readkeyvalues0_166 {
		kvdram_V {Type I LastRead 9 FirstWrite -1}
		kvdram_V_offset {Type I LastRead 0 FirstWrite -1}
		buffer_V {Type O LastRead -1 FirstWrite 10}
		baseaddress {Type I LastRead 0 FirstWrite -1}
		offset_kvs {Type I LastRead 0 FirstWrite -1}
		size_kvs {Type I LastRead 1 FirstWrite -1}}}

set hasDtUnsupportedChannel 0

set PerformanceInfo {[
	{"Name" : "Latency", "Min" : "44", "Max" : "2096"}
	, {"Name" : "Interval", "Min" : "44", "Max" : "2096"}
]}

set PipelineEnableSignalInfo {[
]}

set Spec2ImplPortList { 
	kvdram_V { m_axi {  { m_axi_kvdram_V_AWVALID VALID 1 1 }  { m_axi_kvdram_V_AWREADY READY 0 1 }  { m_axi_kvdram_V_AWADDR ADDR 1 32 }  { m_axi_kvdram_V_AWID ID 1 1 }  { m_axi_kvdram_V_AWLEN LEN 1 32 }  { m_axi_kvdram_V_AWSIZE SIZE 1 3 }  { m_axi_kvdram_V_AWBURST BURST 1 2 }  { m_axi_kvdram_V_AWLOCK LOCK 1 2 }  { m_axi_kvdram_V_AWCACHE CACHE 1 4 }  { m_axi_kvdram_V_AWPROT PROT 1 3 }  { m_axi_kvdram_V_AWQOS QOS 1 4 }  { m_axi_kvdram_V_AWREGION REGION 1 4 }  { m_axi_kvdram_V_AWUSER USER 1 1 }  { m_axi_kvdram_V_WVALID VALID 1 1 }  { m_axi_kvdram_V_WREADY READY 0 1 }  { m_axi_kvdram_V_WDATA DATA 1 512 }  { m_axi_kvdram_V_WSTRB STRB 1 64 }  { m_axi_kvdram_V_WLAST LAST 1 1 }  { m_axi_kvdram_V_WID ID 1 1 }  { m_axi_kvdram_V_WUSER USER 1 1 }  { m_axi_kvdram_V_ARVALID VALID 1 1 }  { m_axi_kvdram_V_ARREADY READY 0 1 }  { m_axi_kvdram_V_ARADDR ADDR 1 32 }  { m_axi_kvdram_V_ARID ID 1 1 }  { m_axi_kvdram_V_ARLEN LEN 1 32 }  { m_axi_kvdram_V_ARSIZE SIZE 1 3 }  { m_axi_kvdram_V_ARBURST BURST 1 2 }  { m_axi_kvdram_V_ARLOCK LOCK 1 2 }  { m_axi_kvdram_V_ARCACHE CACHE 1 4 }  { m_axi_kvdram_V_ARPROT PROT 1 3 }  { m_axi_kvdram_V_ARQOS QOS 1 4 }  { m_axi_kvdram_V_ARREGION REGION 1 4 }  { m_axi_kvdram_V_ARUSER USER 1 1 }  { m_axi_kvdram_V_RVALID VALID 0 1 }  { m_axi_kvdram_V_RREADY READY 1 1 }  { m_axi_kvdram_V_RDATA DATA 0 512 }  { m_axi_kvdram_V_RLAST LAST 0 1 }  { m_axi_kvdram_V_RID ID 0 1 }  { m_axi_kvdram_V_RUSER USER 0 1 }  { m_axi_kvdram_V_RRESP RESP 0 2 }  { m_axi_kvdram_V_BVALID VALID 0 1 }  { m_axi_kvdram_V_BREADY READY 1 1 }  { m_axi_kvdram_V_BRESP RESP 0 2 }  { m_axi_kvdram_V_BID ID 0 1 }  { m_axi_kvdram_V_BUSER USER 0 1 } } }
	kvdram_V_offset { ap_none {  { kvdram_V_offset in_data 0 26 } } }
	buffer0_V { ap_memory {  { buffer0_V_address0 mem_address 1 9 }  { buffer0_V_ce0 mem_ce 1 1 }  { buffer0_V_we0 mem_we 1 1 }  { buffer0_V_d0 mem_din 1 512 } } }
	buffer1_V { ap_memory {  { buffer1_V_address0 mem_address 1 9 }  { buffer1_V_ce0 mem_ce 1 1 }  { buffer1_V_we0 mem_we 1 1 }  { buffer1_V_d0 mem_din 1 512 } } }
	buffer2_V { ap_memory {  { buffer2_V_address0 mem_address 1 9 }  { buffer2_V_ce0 mem_ce 1 1 }  { buffer2_V_we0 mem_we 1 1 }  { buffer2_V_d0 mem_din 1 512 } } }
	buffer3_V { ap_memory {  { buffer3_V_address0 mem_address 1 9 }  { buffer3_V_ce0 mem_ce 1 1 }  { buffer3_V_we0 mem_we 1 1 }  { buffer3_V_d0 mem_din 1 512 } } }
	baseaddress { ap_none {  { baseaddress in_data 0 25 } } }
	sourcestats_0_key_r { ap_none {  { sourcestats_0_key_r in_data 0 32 } } }
	sourcestats_1_key_r { ap_none {  { sourcestats_1_key_r in_data 0 32 } } }
	sourcestats_2_key_r { ap_none {  { sourcestats_2_key_r in_data 0 32 } } }
	sourcestats_3_key_r { ap_none {  { sourcestats_3_key_r in_data 0 32 } } }
	sourcestats_0_value { ap_none {  { sourcestats_0_value in_data 0 32 } } }
	sourcestats_1_value { ap_none {  { sourcestats_1_value in_data 0 32 } } }
	sourcestats_2_value { ap_none {  { sourcestats_2_value in_data 0 32 } } }
	sourcestats_3_value { ap_none {  { sourcestats_3_value in_data 0 32 } } }
	travstate_i_kvs { ap_none {  { travstate_i_kvs in_data 0 32 } } }
}
