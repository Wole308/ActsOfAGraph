set moduleName savepartitions0
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
set C_modelName {savepartitions0}
set C_modelType { void 0 }
set C_modelArgList {
	{ kvdram_V int 512 regular {axi_master 1}  }
	{ kvdram_V_offset int 26 regular  }
	{ destbuffer0_V int 512 regular {array 2112 { 1 3 } 1 1 }  }
	{ destbuffer1_V int 512 regular {array 2112 { 1 3 } 1 1 }  }
	{ destbuffer2_V int 512 regular {array 2112 { 1 3 } 1 1 }  }
	{ destbuffer3_V int 512 regular {array 2112 { 1 3 } 1 1 }  }
	{ capsule0_key int 32 regular {array 32 { 1 1 } 1 1 }  }
	{ capsule0_value int 32 regular {array 32 { 1 3 } 1 1 }  }
	{ capsule1_key int 32 regular {array 32 { 1 1 } 1 1 }  }
	{ capsule1_value int 32 regular {array 32 { 1 3 } 1 1 }  }
	{ capsule2_key int 32 regular {array 32 { 1 1 } 1 1 }  }
	{ capsule2_value int 32 regular {array 32 { 1 3 } 1 1 }  }
	{ capsule3_key int 32 regular {array 32 { 1 1 } 1 1 }  }
	{ capsule3_value int 32 regular {array 32 { 1 3 } 1 1 }  }
	{ kvdeststats_tmp_key int 32 regular {array 32 { 1 1 } 1 1 }  }
	{ kvdeststats_tmp_valu int 32 regular {array 32 { 1 2 } 1 1 }  }
	{ kvdrambaseaddress int 25 regular  }
}
set C_modelArgMapList {[ 
	{ "Name" : "kvdram_V", "interface" : "axi_master", "bitwidth" : 512, "direction" : "WRITEONLY"} , 
 	{ "Name" : "kvdram_V_offset", "interface" : "wire", "bitwidth" : 26, "direction" : "READONLY"} , 
 	{ "Name" : "destbuffer0_V", "interface" : "memory", "bitwidth" : 512, "direction" : "READONLY"} , 
 	{ "Name" : "destbuffer1_V", "interface" : "memory", "bitwidth" : 512, "direction" : "READONLY"} , 
 	{ "Name" : "destbuffer2_V", "interface" : "memory", "bitwidth" : 512, "direction" : "READONLY"} , 
 	{ "Name" : "destbuffer3_V", "interface" : "memory", "bitwidth" : 512, "direction" : "READONLY"} , 
 	{ "Name" : "capsule0_key", "interface" : "memory", "bitwidth" : 32, "direction" : "READONLY"} , 
 	{ "Name" : "capsule0_value", "interface" : "memory", "bitwidth" : 32, "direction" : "READONLY"} , 
 	{ "Name" : "capsule1_key", "interface" : "memory", "bitwidth" : 32, "direction" : "READONLY"} , 
 	{ "Name" : "capsule1_value", "interface" : "memory", "bitwidth" : 32, "direction" : "READONLY"} , 
 	{ "Name" : "capsule2_key", "interface" : "memory", "bitwidth" : 32, "direction" : "READONLY"} , 
 	{ "Name" : "capsule2_value", "interface" : "memory", "bitwidth" : 32, "direction" : "READONLY"} , 
 	{ "Name" : "capsule3_key", "interface" : "memory", "bitwidth" : 32, "direction" : "READONLY"} , 
 	{ "Name" : "capsule3_value", "interface" : "memory", "bitwidth" : 32, "direction" : "READONLY"} , 
 	{ "Name" : "kvdeststats_tmp_key", "interface" : "memory", "bitwidth" : 32, "direction" : "READONLY"} , 
 	{ "Name" : "kvdeststats_tmp_valu", "interface" : "memory", "bitwidth" : 32, "direction" : "READWRITE"} , 
 	{ "Name" : "kvdrambaseaddress", "interface" : "wire", "bitwidth" : 25, "direction" : "READONLY"} ]}
# RTL Port declarations: 
set portNum 115
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
	{ destbuffer0_V_address0 sc_out sc_lv 12 signal 2 } 
	{ destbuffer0_V_ce0 sc_out sc_logic 1 signal 2 } 
	{ destbuffer0_V_q0 sc_in sc_lv 512 signal 2 } 
	{ destbuffer1_V_address0 sc_out sc_lv 12 signal 3 } 
	{ destbuffer1_V_ce0 sc_out sc_logic 1 signal 3 } 
	{ destbuffer1_V_q0 sc_in sc_lv 512 signal 3 } 
	{ destbuffer2_V_address0 sc_out sc_lv 12 signal 4 } 
	{ destbuffer2_V_ce0 sc_out sc_logic 1 signal 4 } 
	{ destbuffer2_V_q0 sc_in sc_lv 512 signal 4 } 
	{ destbuffer3_V_address0 sc_out sc_lv 12 signal 5 } 
	{ destbuffer3_V_ce0 sc_out sc_logic 1 signal 5 } 
	{ destbuffer3_V_q0 sc_in sc_lv 512 signal 5 } 
	{ capsule0_key_address0 sc_out sc_lv 5 signal 6 } 
	{ capsule0_key_ce0 sc_out sc_logic 1 signal 6 } 
	{ capsule0_key_q0 sc_in sc_lv 32 signal 6 } 
	{ capsule0_key_address1 sc_out sc_lv 5 signal 6 } 
	{ capsule0_key_ce1 sc_out sc_logic 1 signal 6 } 
	{ capsule0_key_q1 sc_in sc_lv 32 signal 6 } 
	{ capsule0_value_address0 sc_out sc_lv 5 signal 7 } 
	{ capsule0_value_ce0 sc_out sc_logic 1 signal 7 } 
	{ capsule0_value_q0 sc_in sc_lv 32 signal 7 } 
	{ capsule1_key_address0 sc_out sc_lv 5 signal 8 } 
	{ capsule1_key_ce0 sc_out sc_logic 1 signal 8 } 
	{ capsule1_key_q0 sc_in sc_lv 32 signal 8 } 
	{ capsule1_key_address1 sc_out sc_lv 5 signal 8 } 
	{ capsule1_key_ce1 sc_out sc_logic 1 signal 8 } 
	{ capsule1_key_q1 sc_in sc_lv 32 signal 8 } 
	{ capsule1_value_address0 sc_out sc_lv 5 signal 9 } 
	{ capsule1_value_ce0 sc_out sc_logic 1 signal 9 } 
	{ capsule1_value_q0 sc_in sc_lv 32 signal 9 } 
	{ capsule2_key_address0 sc_out sc_lv 5 signal 10 } 
	{ capsule2_key_ce0 sc_out sc_logic 1 signal 10 } 
	{ capsule2_key_q0 sc_in sc_lv 32 signal 10 } 
	{ capsule2_key_address1 sc_out sc_lv 5 signal 10 } 
	{ capsule2_key_ce1 sc_out sc_logic 1 signal 10 } 
	{ capsule2_key_q1 sc_in sc_lv 32 signal 10 } 
	{ capsule2_value_address0 sc_out sc_lv 5 signal 11 } 
	{ capsule2_value_ce0 sc_out sc_logic 1 signal 11 } 
	{ capsule2_value_q0 sc_in sc_lv 32 signal 11 } 
	{ capsule3_key_address0 sc_out sc_lv 5 signal 12 } 
	{ capsule3_key_ce0 sc_out sc_logic 1 signal 12 } 
	{ capsule3_key_q0 sc_in sc_lv 32 signal 12 } 
	{ capsule3_key_address1 sc_out sc_lv 5 signal 12 } 
	{ capsule3_key_ce1 sc_out sc_logic 1 signal 12 } 
	{ capsule3_key_q1 sc_in sc_lv 32 signal 12 } 
	{ capsule3_value_address0 sc_out sc_lv 5 signal 13 } 
	{ capsule3_value_ce0 sc_out sc_logic 1 signal 13 } 
	{ capsule3_value_q0 sc_in sc_lv 32 signal 13 } 
	{ kvdeststats_tmp_key_address0 sc_out sc_lv 5 signal 14 } 
	{ kvdeststats_tmp_key_ce0 sc_out sc_logic 1 signal 14 } 
	{ kvdeststats_tmp_key_q0 sc_in sc_lv 32 signal 14 } 
	{ kvdeststats_tmp_key_address1 sc_out sc_lv 5 signal 14 } 
	{ kvdeststats_tmp_key_ce1 sc_out sc_logic 1 signal 14 } 
	{ kvdeststats_tmp_key_q1 sc_in sc_lv 32 signal 14 } 
	{ kvdeststats_tmp_valu_address0 sc_out sc_lv 5 signal 15 } 
	{ kvdeststats_tmp_valu_ce0 sc_out sc_logic 1 signal 15 } 
	{ kvdeststats_tmp_valu_q0 sc_in sc_lv 32 signal 15 } 
	{ kvdeststats_tmp_valu_address1 sc_out sc_lv 5 signal 15 } 
	{ kvdeststats_tmp_valu_ce1 sc_out sc_logic 1 signal 15 } 
	{ kvdeststats_tmp_valu_we1 sc_out sc_logic 1 signal 15 } 
	{ kvdeststats_tmp_valu_d1 sc_out sc_lv 32 signal 15 } 
	{ kvdeststats_tmp_valu_q1 sc_in sc_lv 32 signal 15 } 
	{ kvdrambaseaddress sc_in sc_lv 25 signal 16 } 
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
 	{ "name": "destbuffer0_V_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":12, "type": "signal", "bundle":{"name": "destbuffer0_V", "role": "address0" }} , 
 	{ "name": "destbuffer0_V_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "destbuffer0_V", "role": "ce0" }} , 
 	{ "name": "destbuffer0_V_q0", "direction": "in", "datatype": "sc_lv", "bitwidth":512, "type": "signal", "bundle":{"name": "destbuffer0_V", "role": "q0" }} , 
 	{ "name": "destbuffer1_V_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":12, "type": "signal", "bundle":{"name": "destbuffer1_V", "role": "address0" }} , 
 	{ "name": "destbuffer1_V_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "destbuffer1_V", "role": "ce0" }} , 
 	{ "name": "destbuffer1_V_q0", "direction": "in", "datatype": "sc_lv", "bitwidth":512, "type": "signal", "bundle":{"name": "destbuffer1_V", "role": "q0" }} , 
 	{ "name": "destbuffer2_V_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":12, "type": "signal", "bundle":{"name": "destbuffer2_V", "role": "address0" }} , 
 	{ "name": "destbuffer2_V_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "destbuffer2_V", "role": "ce0" }} , 
 	{ "name": "destbuffer2_V_q0", "direction": "in", "datatype": "sc_lv", "bitwidth":512, "type": "signal", "bundle":{"name": "destbuffer2_V", "role": "q0" }} , 
 	{ "name": "destbuffer3_V_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":12, "type": "signal", "bundle":{"name": "destbuffer3_V", "role": "address0" }} , 
 	{ "name": "destbuffer3_V_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "destbuffer3_V", "role": "ce0" }} , 
 	{ "name": "destbuffer3_V_q0", "direction": "in", "datatype": "sc_lv", "bitwidth":512, "type": "signal", "bundle":{"name": "destbuffer3_V", "role": "q0" }} , 
 	{ "name": "capsule0_key_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":5, "type": "signal", "bundle":{"name": "capsule0_key", "role": "address0" }} , 
 	{ "name": "capsule0_key_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "capsule0_key", "role": "ce0" }} , 
 	{ "name": "capsule0_key_q0", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "capsule0_key", "role": "q0" }} , 
 	{ "name": "capsule0_key_address1", "direction": "out", "datatype": "sc_lv", "bitwidth":5, "type": "signal", "bundle":{"name": "capsule0_key", "role": "address1" }} , 
 	{ "name": "capsule0_key_ce1", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "capsule0_key", "role": "ce1" }} , 
 	{ "name": "capsule0_key_q1", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "capsule0_key", "role": "q1" }} , 
 	{ "name": "capsule0_value_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":5, "type": "signal", "bundle":{"name": "capsule0_value", "role": "address0" }} , 
 	{ "name": "capsule0_value_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "capsule0_value", "role": "ce0" }} , 
 	{ "name": "capsule0_value_q0", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "capsule0_value", "role": "q0" }} , 
 	{ "name": "capsule1_key_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":5, "type": "signal", "bundle":{"name": "capsule1_key", "role": "address0" }} , 
 	{ "name": "capsule1_key_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "capsule1_key", "role": "ce0" }} , 
 	{ "name": "capsule1_key_q0", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "capsule1_key", "role": "q0" }} , 
 	{ "name": "capsule1_key_address1", "direction": "out", "datatype": "sc_lv", "bitwidth":5, "type": "signal", "bundle":{"name": "capsule1_key", "role": "address1" }} , 
 	{ "name": "capsule1_key_ce1", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "capsule1_key", "role": "ce1" }} , 
 	{ "name": "capsule1_key_q1", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "capsule1_key", "role": "q1" }} , 
 	{ "name": "capsule1_value_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":5, "type": "signal", "bundle":{"name": "capsule1_value", "role": "address0" }} , 
 	{ "name": "capsule1_value_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "capsule1_value", "role": "ce0" }} , 
 	{ "name": "capsule1_value_q0", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "capsule1_value", "role": "q0" }} , 
 	{ "name": "capsule2_key_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":5, "type": "signal", "bundle":{"name": "capsule2_key", "role": "address0" }} , 
 	{ "name": "capsule2_key_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "capsule2_key", "role": "ce0" }} , 
 	{ "name": "capsule2_key_q0", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "capsule2_key", "role": "q0" }} , 
 	{ "name": "capsule2_key_address1", "direction": "out", "datatype": "sc_lv", "bitwidth":5, "type": "signal", "bundle":{"name": "capsule2_key", "role": "address1" }} , 
 	{ "name": "capsule2_key_ce1", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "capsule2_key", "role": "ce1" }} , 
 	{ "name": "capsule2_key_q1", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "capsule2_key", "role": "q1" }} , 
 	{ "name": "capsule2_value_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":5, "type": "signal", "bundle":{"name": "capsule2_value", "role": "address0" }} , 
 	{ "name": "capsule2_value_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "capsule2_value", "role": "ce0" }} , 
 	{ "name": "capsule2_value_q0", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "capsule2_value", "role": "q0" }} , 
 	{ "name": "capsule3_key_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":5, "type": "signal", "bundle":{"name": "capsule3_key", "role": "address0" }} , 
 	{ "name": "capsule3_key_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "capsule3_key", "role": "ce0" }} , 
 	{ "name": "capsule3_key_q0", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "capsule3_key", "role": "q0" }} , 
 	{ "name": "capsule3_key_address1", "direction": "out", "datatype": "sc_lv", "bitwidth":5, "type": "signal", "bundle":{"name": "capsule3_key", "role": "address1" }} , 
 	{ "name": "capsule3_key_ce1", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "capsule3_key", "role": "ce1" }} , 
 	{ "name": "capsule3_key_q1", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "capsule3_key", "role": "q1" }} , 
 	{ "name": "capsule3_value_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":5, "type": "signal", "bundle":{"name": "capsule3_value", "role": "address0" }} , 
 	{ "name": "capsule3_value_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "capsule3_value", "role": "ce0" }} , 
 	{ "name": "capsule3_value_q0", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "capsule3_value", "role": "q0" }} , 
 	{ "name": "kvdeststats_tmp_key_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":5, "type": "signal", "bundle":{"name": "kvdeststats_tmp_key", "role": "address0" }} , 
 	{ "name": "kvdeststats_tmp_key_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "kvdeststats_tmp_key", "role": "ce0" }} , 
 	{ "name": "kvdeststats_tmp_key_q0", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "kvdeststats_tmp_key", "role": "q0" }} , 
 	{ "name": "kvdeststats_tmp_key_address1", "direction": "out", "datatype": "sc_lv", "bitwidth":5, "type": "signal", "bundle":{"name": "kvdeststats_tmp_key", "role": "address1" }} , 
 	{ "name": "kvdeststats_tmp_key_ce1", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "kvdeststats_tmp_key", "role": "ce1" }} , 
 	{ "name": "kvdeststats_tmp_key_q1", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "kvdeststats_tmp_key", "role": "q1" }} , 
 	{ "name": "kvdeststats_tmp_valu_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":5, "type": "signal", "bundle":{"name": "kvdeststats_tmp_valu", "role": "address0" }} , 
 	{ "name": "kvdeststats_tmp_valu_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "kvdeststats_tmp_valu", "role": "ce0" }} , 
 	{ "name": "kvdeststats_tmp_valu_q0", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "kvdeststats_tmp_valu", "role": "q0" }} , 
 	{ "name": "kvdeststats_tmp_valu_address1", "direction": "out", "datatype": "sc_lv", "bitwidth":5, "type": "signal", "bundle":{"name": "kvdeststats_tmp_valu", "role": "address1" }} , 
 	{ "name": "kvdeststats_tmp_valu_ce1", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "kvdeststats_tmp_valu", "role": "ce1" }} , 
 	{ "name": "kvdeststats_tmp_valu_we1", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "kvdeststats_tmp_valu", "role": "we1" }} , 
 	{ "name": "kvdeststats_tmp_valu_d1", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "kvdeststats_tmp_valu", "role": "d1" }} , 
 	{ "name": "kvdeststats_tmp_valu_q1", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "kvdeststats_tmp_valu", "role": "q1" }} , 
 	{ "name": "kvdrambaseaddress", "direction": "in", "datatype": "sc_lv", "bitwidth":25, "type": "signal", "bundle":{"name": "kvdrambaseaddress", "role": "default" }}  ]}

set RtlHierarchyInfo {[
	{"ID" : "0", "Level" : "0", "Path" : "`AUTOTB_DUT_INST", "Parent" : "", "Child" : ["1", "2", "3", "4", "5", "6", "7", "8"],
		"CDFG" : "savepartitions0",
		"Protocol" : "ap_ctrl_hs",
		"ControlExist" : "1", "ap_start" : "1", "ap_ready" : "1", "ap_done" : "1", "ap_continue" : "0", "ap_idle" : "1",
		"Pipeline" : "None", "UnalignedPipeline" : "0", "RewindPipeline" : "0", "ProcessNetwork" : "0",
		"II" : "0",
		"VariableLatency" : "1", "ExactLatency" : "-1", "EstimateLatencyMin" : "8800", "EstimateLatencyMax" : "8800",
		"Combinational" : "0",
		"Datapath" : "0",
		"ClockEnable" : "0",
		"HasSubDataflow" : "0",
		"InDataflowNetwork" : "0",
		"HasNonBlockingOperation" : "0",
		"Port" : [
			{"Name" : "kvdram_V", "Type" : "MAXI", "Direction" : "O",
				"BlockSignal" : [
					{"Name" : "kvdram_V_blk_n_AW", "Type" : "RtlSignal"},
					{"Name" : "kvdram_V_blk_n_W", "Type" : "RtlSignal"},
					{"Name" : "kvdram_V_blk_n_B", "Type" : "RtlSignal"}]},
			{"Name" : "kvdram_V_offset", "Type" : "None", "Direction" : "I"},
			{"Name" : "destbuffer0_V", "Type" : "Memory", "Direction" : "I"},
			{"Name" : "destbuffer1_V", "Type" : "Memory", "Direction" : "I"},
			{"Name" : "destbuffer2_V", "Type" : "Memory", "Direction" : "I"},
			{"Name" : "destbuffer3_V", "Type" : "Memory", "Direction" : "I"},
			{"Name" : "capsule0_key", "Type" : "Memory", "Direction" : "I"},
			{"Name" : "capsule0_value", "Type" : "Memory", "Direction" : "I"},
			{"Name" : "capsule1_key", "Type" : "Memory", "Direction" : "I"},
			{"Name" : "capsule1_value", "Type" : "Memory", "Direction" : "I"},
			{"Name" : "capsule2_key", "Type" : "Memory", "Direction" : "I"},
			{"Name" : "capsule2_value", "Type" : "Memory", "Direction" : "I"},
			{"Name" : "capsule3_key", "Type" : "Memory", "Direction" : "I"},
			{"Name" : "capsule3_value", "Type" : "Memory", "Direction" : "I"},
			{"Name" : "kvdeststats_tmp_key", "Type" : "Memory", "Direction" : "I"},
			{"Name" : "kvdeststats_tmp_valu", "Type" : "Memory", "Direction" : "IO"},
			{"Name" : "kvdrambaseaddress", "Type" : "None", "Direction" : "I"}]},
	{"ID" : "1", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.topkernel_urem_12ns_8ns_12_16_1_U119", "Parent" : "0"},
	{"ID" : "2", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.topkernel_urem_12ns_8ns_12_16_1_U120", "Parent" : "0"},
	{"ID" : "3", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.topkernel_urem_12ns_8ns_12_16_1_U121", "Parent" : "0"},
	{"ID" : "4", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.topkernel_urem_12ns_8ns_12_16_1_U122", "Parent" : "0"},
	{"ID" : "5", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.topkernel_urem_12ns_8ns_12_16_1_U123", "Parent" : "0"},
	{"ID" : "6", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.topkernel_urem_12ns_8ns_12_16_1_U124", "Parent" : "0"},
	{"ID" : "7", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.topkernel_urem_12ns_8ns_12_16_1_U125", "Parent" : "0"},
	{"ID" : "8", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.topkernel_urem_12ns_8ns_12_16_1_U126", "Parent" : "0"}]}


set ArgLastReadFirstWriteLatency {
	savepartitions0 {
		kvdram_V {Type O LastRead 73 FirstWrite 18}
		kvdram_V_offset {Type I LastRead 0 FirstWrite -1}
		destbuffer0_V {Type I LastRead 16 FirstWrite -1}
		destbuffer1_V {Type I LastRead 34 FirstWrite -1}
		destbuffer2_V {Type I LastRead 52 FirstWrite -1}
		destbuffer3_V {Type I LastRead 70 FirstWrite -1}
		capsule0_key {Type I LastRead 16 FirstWrite -1}
		capsule0_value {Type I LastRead 18 FirstWrite -1}
		capsule1_key {Type I LastRead 34 FirstWrite -1}
		capsule1_value {Type I LastRead 36 FirstWrite -1}
		capsule2_key {Type I LastRead 52 FirstWrite -1}
		capsule2_value {Type I LastRead 54 FirstWrite -1}
		capsule3_key {Type I LastRead 70 FirstWrite -1}
		capsule3_value {Type I LastRead 72 FirstWrite -1}
		kvdeststats_tmp_key {Type I LastRead 70 FirstWrite -1}
		kvdeststats_tmp_valu {Type IO LastRead 72 FirstWrite 19}
		kvdrambaseaddress {Type I LastRead 0 FirstWrite -1}}}

set hasDtUnsupportedChannel 0

set PerformanceInfo {[
	{"Name" : "Latency", "Min" : "8800", "Max" : "8800"}
	, {"Name" : "Interval", "Min" : "8800", "Max" : "8800"}
]}

set PipelineEnableSignalInfo {[
	{"Pipeline" : "0", "EnableSignal" : "ap_enable_pp0"}
	{"Pipeline" : "1", "EnableSignal" : "ap_enable_pp1"}
	{"Pipeline" : "2", "EnableSignal" : "ap_enable_pp2"}
	{"Pipeline" : "3", "EnableSignal" : "ap_enable_pp3"}
]}

set Spec2ImplPortList { 
	kvdram_V { m_axi {  { m_axi_kvdram_V_AWVALID VALID 1 1 }  { m_axi_kvdram_V_AWREADY READY 0 1 }  { m_axi_kvdram_V_AWADDR ADDR 1 32 }  { m_axi_kvdram_V_AWID ID 1 1 }  { m_axi_kvdram_V_AWLEN LEN 1 32 }  { m_axi_kvdram_V_AWSIZE SIZE 1 3 }  { m_axi_kvdram_V_AWBURST BURST 1 2 }  { m_axi_kvdram_V_AWLOCK LOCK 1 2 }  { m_axi_kvdram_V_AWCACHE CACHE 1 4 }  { m_axi_kvdram_V_AWPROT PROT 1 3 }  { m_axi_kvdram_V_AWQOS QOS 1 4 }  { m_axi_kvdram_V_AWREGION REGION 1 4 }  { m_axi_kvdram_V_AWUSER USER 1 1 }  { m_axi_kvdram_V_WVALID VALID 1 1 }  { m_axi_kvdram_V_WREADY READY 0 1 }  { m_axi_kvdram_V_WDATA DATA 1 512 }  { m_axi_kvdram_V_WSTRB STRB 1 64 }  { m_axi_kvdram_V_WLAST LAST 1 1 }  { m_axi_kvdram_V_WID ID 1 1 }  { m_axi_kvdram_V_WUSER USER 1 1 }  { m_axi_kvdram_V_ARVALID VALID 1 1 }  { m_axi_kvdram_V_ARREADY READY 0 1 }  { m_axi_kvdram_V_ARADDR ADDR 1 32 }  { m_axi_kvdram_V_ARID ID 1 1 }  { m_axi_kvdram_V_ARLEN LEN 1 32 }  { m_axi_kvdram_V_ARSIZE SIZE 1 3 }  { m_axi_kvdram_V_ARBURST BURST 1 2 }  { m_axi_kvdram_V_ARLOCK LOCK 1 2 }  { m_axi_kvdram_V_ARCACHE CACHE 1 4 }  { m_axi_kvdram_V_ARPROT PROT 1 3 }  { m_axi_kvdram_V_ARQOS QOS 1 4 }  { m_axi_kvdram_V_ARREGION REGION 1 4 }  { m_axi_kvdram_V_ARUSER USER 1 1 }  { m_axi_kvdram_V_RVALID VALID 0 1 }  { m_axi_kvdram_V_RREADY READY 1 1 }  { m_axi_kvdram_V_RDATA DATA 0 512 }  { m_axi_kvdram_V_RLAST LAST 0 1 }  { m_axi_kvdram_V_RID ID 0 1 }  { m_axi_kvdram_V_RUSER USER 0 1 }  { m_axi_kvdram_V_RRESP RESP 0 2 }  { m_axi_kvdram_V_BVALID VALID 0 1 }  { m_axi_kvdram_V_BREADY READY 1 1 }  { m_axi_kvdram_V_BRESP RESP 0 2 }  { m_axi_kvdram_V_BID ID 0 1 }  { m_axi_kvdram_V_BUSER USER 0 1 } } }
	kvdram_V_offset { ap_none {  { kvdram_V_offset in_data 0 26 } } }
	destbuffer0_V { ap_memory {  { destbuffer0_V_address0 mem_address 1 12 }  { destbuffer0_V_ce0 mem_ce 1 1 }  { destbuffer0_V_q0 mem_dout 0 512 } } }
	destbuffer1_V { ap_memory {  { destbuffer1_V_address0 mem_address 1 12 }  { destbuffer1_V_ce0 mem_ce 1 1 }  { destbuffer1_V_q0 mem_dout 0 512 } } }
	destbuffer2_V { ap_memory {  { destbuffer2_V_address0 mem_address 1 12 }  { destbuffer2_V_ce0 mem_ce 1 1 }  { destbuffer2_V_q0 mem_dout 0 512 } } }
	destbuffer3_V { ap_memory {  { destbuffer3_V_address0 mem_address 1 12 }  { destbuffer3_V_ce0 mem_ce 1 1 }  { destbuffer3_V_q0 mem_dout 0 512 } } }
	capsule0_key { ap_memory {  { capsule0_key_address0 mem_address 1 5 }  { capsule0_key_ce0 mem_ce 1 1 }  { capsule0_key_q0 mem_dout 0 32 }  { capsule0_key_address1 MemPortADDR2 1 5 }  { capsule0_key_ce1 MemPortCE2 1 1 }  { capsule0_key_q1 MemPortDOUT2 0 32 } } }
	capsule0_value { ap_memory {  { capsule0_value_address0 mem_address 1 5 }  { capsule0_value_ce0 mem_ce 1 1 }  { capsule0_value_q0 mem_dout 0 32 } } }
	capsule1_key { ap_memory {  { capsule1_key_address0 mem_address 1 5 }  { capsule1_key_ce0 mem_ce 1 1 }  { capsule1_key_q0 mem_dout 0 32 }  { capsule1_key_address1 MemPortADDR2 1 5 }  { capsule1_key_ce1 MemPortCE2 1 1 }  { capsule1_key_q1 MemPortDOUT2 0 32 } } }
	capsule1_value { ap_memory {  { capsule1_value_address0 mem_address 1 5 }  { capsule1_value_ce0 mem_ce 1 1 }  { capsule1_value_q0 mem_dout 0 32 } } }
	capsule2_key { ap_memory {  { capsule2_key_address0 mem_address 1 5 }  { capsule2_key_ce0 mem_ce 1 1 }  { capsule2_key_q0 mem_dout 0 32 }  { capsule2_key_address1 MemPortADDR2 1 5 }  { capsule2_key_ce1 MemPortCE2 1 1 }  { capsule2_key_q1 MemPortDOUT2 0 32 } } }
	capsule2_value { ap_memory {  { capsule2_value_address0 mem_address 1 5 }  { capsule2_value_ce0 mem_ce 1 1 }  { capsule2_value_q0 mem_dout 0 32 } } }
	capsule3_key { ap_memory {  { capsule3_key_address0 mem_address 1 5 }  { capsule3_key_ce0 mem_ce 1 1 }  { capsule3_key_q0 mem_dout 0 32 }  { capsule3_key_address1 MemPortADDR2 1 5 }  { capsule3_key_ce1 MemPortCE2 1 1 }  { capsule3_key_q1 MemPortDOUT2 0 32 } } }
	capsule3_value { ap_memory {  { capsule3_value_address0 mem_address 1 5 }  { capsule3_value_ce0 mem_ce 1 1 }  { capsule3_value_q0 mem_dout 0 32 } } }
	kvdeststats_tmp_key { ap_memory {  { kvdeststats_tmp_key_address0 mem_address 1 5 }  { kvdeststats_tmp_key_ce0 mem_ce 1 1 }  { kvdeststats_tmp_key_q0 mem_dout 0 32 }  { kvdeststats_tmp_key_address1 MemPortADDR2 1 5 }  { kvdeststats_tmp_key_ce1 MemPortCE2 1 1 }  { kvdeststats_tmp_key_q1 MemPortDOUT2 0 32 } } }
	kvdeststats_tmp_valu { ap_memory {  { kvdeststats_tmp_valu_address0 mem_address 1 5 }  { kvdeststats_tmp_valu_ce0 mem_ce 1 1 }  { kvdeststats_tmp_valu_q0 mem_dout 0 32 }  { kvdeststats_tmp_valu_address1 MemPortADDR2 1 5 }  { kvdeststats_tmp_valu_ce1 MemPortCE2 1 1 }  { kvdeststats_tmp_valu_we1 MemPortWE2 1 1 }  { kvdeststats_tmp_valu_d1 MemPortDIN2 1 32 }  { kvdeststats_tmp_valu_q1 MemPortDOUT2 0 32 } } }
	kvdrambaseaddress { ap_none {  { kvdrambaseaddress in_data 0 25 } } }
}
set moduleName savepartitions0
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
set C_modelName {savepartitions0}
set C_modelType { void 0 }
set C_modelArgList {
	{ kvdram_V int 512 regular {axi_master 1}  }
	{ kvdram_V_offset int 26 regular  }
	{ destbuffer0_V int 512 regular {array 2112 { 1 3 } 1 1 }  }
	{ destbuffer1_V int 512 regular {array 2112 { 1 3 } 1 1 }  }
	{ destbuffer2_V int 512 regular {array 2112 { 1 3 } 1 1 }  }
	{ destbuffer3_V int 512 regular {array 2112 { 1 3 } 1 1 }  }
	{ capsule0_key int 32 regular {array 32 { 1 1 } 1 1 }  }
	{ capsule0_value int 32 regular {array 32 { 1 3 } 1 1 }  }
	{ capsule1_key int 32 regular {array 32 { 1 1 } 1 1 }  }
	{ capsule1_value int 32 regular {array 32 { 1 3 } 1 1 }  }
	{ capsule2_key int 32 regular {array 32 { 1 1 } 1 1 }  }
	{ capsule2_value int 32 regular {array 32 { 1 3 } 1 1 }  }
	{ capsule3_key int 32 regular {array 32 { 1 1 } 1 1 }  }
	{ capsule3_value int 32 regular {array 32 { 1 3 } 1 1 }  }
	{ kvdeststats_tmp_key int 32 regular {array 32 { 1 1 } 1 1 }  }
	{ kvdeststats_tmp_valu int 32 regular {array 32 { 1 2 } 1 1 }  }
	{ kvdrambaseaddress int 25 regular  }
}
set C_modelArgMapList {[ 
	{ "Name" : "kvdram_V", "interface" : "axi_master", "bitwidth" : 512, "direction" : "WRITEONLY"} , 
 	{ "Name" : "kvdram_V_offset", "interface" : "wire", "bitwidth" : 26, "direction" : "READONLY"} , 
 	{ "Name" : "destbuffer0_V", "interface" : "memory", "bitwidth" : 512, "direction" : "READONLY"} , 
 	{ "Name" : "destbuffer1_V", "interface" : "memory", "bitwidth" : 512, "direction" : "READONLY"} , 
 	{ "Name" : "destbuffer2_V", "interface" : "memory", "bitwidth" : 512, "direction" : "READONLY"} , 
 	{ "Name" : "destbuffer3_V", "interface" : "memory", "bitwidth" : 512, "direction" : "READONLY"} , 
 	{ "Name" : "capsule0_key", "interface" : "memory", "bitwidth" : 32, "direction" : "READONLY"} , 
 	{ "Name" : "capsule0_value", "interface" : "memory", "bitwidth" : 32, "direction" : "READONLY"} , 
 	{ "Name" : "capsule1_key", "interface" : "memory", "bitwidth" : 32, "direction" : "READONLY"} , 
 	{ "Name" : "capsule1_value", "interface" : "memory", "bitwidth" : 32, "direction" : "READONLY"} , 
 	{ "Name" : "capsule2_key", "interface" : "memory", "bitwidth" : 32, "direction" : "READONLY"} , 
 	{ "Name" : "capsule2_value", "interface" : "memory", "bitwidth" : 32, "direction" : "READONLY"} , 
 	{ "Name" : "capsule3_key", "interface" : "memory", "bitwidth" : 32, "direction" : "READONLY"} , 
 	{ "Name" : "capsule3_value", "interface" : "memory", "bitwidth" : 32, "direction" : "READONLY"} , 
 	{ "Name" : "kvdeststats_tmp_key", "interface" : "memory", "bitwidth" : 32, "direction" : "READONLY"} , 
 	{ "Name" : "kvdeststats_tmp_valu", "interface" : "memory", "bitwidth" : 32, "direction" : "READWRITE"} , 
 	{ "Name" : "kvdrambaseaddress", "interface" : "wire", "bitwidth" : 25, "direction" : "READONLY"} ]}
# RTL Port declarations: 
set portNum 115
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
	{ destbuffer0_V_address0 sc_out sc_lv 12 signal 2 } 
	{ destbuffer0_V_ce0 sc_out sc_logic 1 signal 2 } 
	{ destbuffer0_V_q0 sc_in sc_lv 512 signal 2 } 
	{ destbuffer1_V_address0 sc_out sc_lv 12 signal 3 } 
	{ destbuffer1_V_ce0 sc_out sc_logic 1 signal 3 } 
	{ destbuffer1_V_q0 sc_in sc_lv 512 signal 3 } 
	{ destbuffer2_V_address0 sc_out sc_lv 12 signal 4 } 
	{ destbuffer2_V_ce0 sc_out sc_logic 1 signal 4 } 
	{ destbuffer2_V_q0 sc_in sc_lv 512 signal 4 } 
	{ destbuffer3_V_address0 sc_out sc_lv 12 signal 5 } 
	{ destbuffer3_V_ce0 sc_out sc_logic 1 signal 5 } 
	{ destbuffer3_V_q0 sc_in sc_lv 512 signal 5 } 
	{ capsule0_key_address0 sc_out sc_lv 5 signal 6 } 
	{ capsule0_key_ce0 sc_out sc_logic 1 signal 6 } 
	{ capsule0_key_q0 sc_in sc_lv 32 signal 6 } 
	{ capsule0_key_address1 sc_out sc_lv 5 signal 6 } 
	{ capsule0_key_ce1 sc_out sc_logic 1 signal 6 } 
	{ capsule0_key_q1 sc_in sc_lv 32 signal 6 } 
	{ capsule0_value_address0 sc_out sc_lv 5 signal 7 } 
	{ capsule0_value_ce0 sc_out sc_logic 1 signal 7 } 
	{ capsule0_value_q0 sc_in sc_lv 32 signal 7 } 
	{ capsule1_key_address0 sc_out sc_lv 5 signal 8 } 
	{ capsule1_key_ce0 sc_out sc_logic 1 signal 8 } 
	{ capsule1_key_q0 sc_in sc_lv 32 signal 8 } 
	{ capsule1_key_address1 sc_out sc_lv 5 signal 8 } 
	{ capsule1_key_ce1 sc_out sc_logic 1 signal 8 } 
	{ capsule1_key_q1 sc_in sc_lv 32 signal 8 } 
	{ capsule1_value_address0 sc_out sc_lv 5 signal 9 } 
	{ capsule1_value_ce0 sc_out sc_logic 1 signal 9 } 
	{ capsule1_value_q0 sc_in sc_lv 32 signal 9 } 
	{ capsule2_key_address0 sc_out sc_lv 5 signal 10 } 
	{ capsule2_key_ce0 sc_out sc_logic 1 signal 10 } 
	{ capsule2_key_q0 sc_in sc_lv 32 signal 10 } 
	{ capsule2_key_address1 sc_out sc_lv 5 signal 10 } 
	{ capsule2_key_ce1 sc_out sc_logic 1 signal 10 } 
	{ capsule2_key_q1 sc_in sc_lv 32 signal 10 } 
	{ capsule2_value_address0 sc_out sc_lv 5 signal 11 } 
	{ capsule2_value_ce0 sc_out sc_logic 1 signal 11 } 
	{ capsule2_value_q0 sc_in sc_lv 32 signal 11 } 
	{ capsule3_key_address0 sc_out sc_lv 5 signal 12 } 
	{ capsule3_key_ce0 sc_out sc_logic 1 signal 12 } 
	{ capsule3_key_q0 sc_in sc_lv 32 signal 12 } 
	{ capsule3_key_address1 sc_out sc_lv 5 signal 12 } 
	{ capsule3_key_ce1 sc_out sc_logic 1 signal 12 } 
	{ capsule3_key_q1 sc_in sc_lv 32 signal 12 } 
	{ capsule3_value_address0 sc_out sc_lv 5 signal 13 } 
	{ capsule3_value_ce0 sc_out sc_logic 1 signal 13 } 
	{ capsule3_value_q0 sc_in sc_lv 32 signal 13 } 
	{ kvdeststats_tmp_key_address0 sc_out sc_lv 5 signal 14 } 
	{ kvdeststats_tmp_key_ce0 sc_out sc_logic 1 signal 14 } 
	{ kvdeststats_tmp_key_q0 sc_in sc_lv 32 signal 14 } 
	{ kvdeststats_tmp_key_address1 sc_out sc_lv 5 signal 14 } 
	{ kvdeststats_tmp_key_ce1 sc_out sc_logic 1 signal 14 } 
	{ kvdeststats_tmp_key_q1 sc_in sc_lv 32 signal 14 } 
	{ kvdeststats_tmp_valu_address0 sc_out sc_lv 5 signal 15 } 
	{ kvdeststats_tmp_valu_ce0 sc_out sc_logic 1 signal 15 } 
	{ kvdeststats_tmp_valu_q0 sc_in sc_lv 32 signal 15 } 
	{ kvdeststats_tmp_valu_address1 sc_out sc_lv 5 signal 15 } 
	{ kvdeststats_tmp_valu_ce1 sc_out sc_logic 1 signal 15 } 
	{ kvdeststats_tmp_valu_we1 sc_out sc_logic 1 signal 15 } 
	{ kvdeststats_tmp_valu_d1 sc_out sc_lv 32 signal 15 } 
	{ kvdeststats_tmp_valu_q1 sc_in sc_lv 32 signal 15 } 
	{ kvdrambaseaddress sc_in sc_lv 25 signal 16 } 
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
 	{ "name": "destbuffer0_V_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":12, "type": "signal", "bundle":{"name": "destbuffer0_V", "role": "address0" }} , 
 	{ "name": "destbuffer0_V_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "destbuffer0_V", "role": "ce0" }} , 
 	{ "name": "destbuffer0_V_q0", "direction": "in", "datatype": "sc_lv", "bitwidth":512, "type": "signal", "bundle":{"name": "destbuffer0_V", "role": "q0" }} , 
 	{ "name": "destbuffer1_V_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":12, "type": "signal", "bundle":{"name": "destbuffer1_V", "role": "address0" }} , 
 	{ "name": "destbuffer1_V_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "destbuffer1_V", "role": "ce0" }} , 
 	{ "name": "destbuffer1_V_q0", "direction": "in", "datatype": "sc_lv", "bitwidth":512, "type": "signal", "bundle":{"name": "destbuffer1_V", "role": "q0" }} , 
 	{ "name": "destbuffer2_V_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":12, "type": "signal", "bundle":{"name": "destbuffer2_V", "role": "address0" }} , 
 	{ "name": "destbuffer2_V_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "destbuffer2_V", "role": "ce0" }} , 
 	{ "name": "destbuffer2_V_q0", "direction": "in", "datatype": "sc_lv", "bitwidth":512, "type": "signal", "bundle":{"name": "destbuffer2_V", "role": "q0" }} , 
 	{ "name": "destbuffer3_V_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":12, "type": "signal", "bundle":{"name": "destbuffer3_V", "role": "address0" }} , 
 	{ "name": "destbuffer3_V_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "destbuffer3_V", "role": "ce0" }} , 
 	{ "name": "destbuffer3_V_q0", "direction": "in", "datatype": "sc_lv", "bitwidth":512, "type": "signal", "bundle":{"name": "destbuffer3_V", "role": "q0" }} , 
 	{ "name": "capsule0_key_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":5, "type": "signal", "bundle":{"name": "capsule0_key", "role": "address0" }} , 
 	{ "name": "capsule0_key_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "capsule0_key", "role": "ce0" }} , 
 	{ "name": "capsule0_key_q0", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "capsule0_key", "role": "q0" }} , 
 	{ "name": "capsule0_key_address1", "direction": "out", "datatype": "sc_lv", "bitwidth":5, "type": "signal", "bundle":{"name": "capsule0_key", "role": "address1" }} , 
 	{ "name": "capsule0_key_ce1", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "capsule0_key", "role": "ce1" }} , 
 	{ "name": "capsule0_key_q1", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "capsule0_key", "role": "q1" }} , 
 	{ "name": "capsule0_value_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":5, "type": "signal", "bundle":{"name": "capsule0_value", "role": "address0" }} , 
 	{ "name": "capsule0_value_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "capsule0_value", "role": "ce0" }} , 
 	{ "name": "capsule0_value_q0", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "capsule0_value", "role": "q0" }} , 
 	{ "name": "capsule1_key_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":5, "type": "signal", "bundle":{"name": "capsule1_key", "role": "address0" }} , 
 	{ "name": "capsule1_key_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "capsule1_key", "role": "ce0" }} , 
 	{ "name": "capsule1_key_q0", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "capsule1_key", "role": "q0" }} , 
 	{ "name": "capsule1_key_address1", "direction": "out", "datatype": "sc_lv", "bitwidth":5, "type": "signal", "bundle":{"name": "capsule1_key", "role": "address1" }} , 
 	{ "name": "capsule1_key_ce1", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "capsule1_key", "role": "ce1" }} , 
 	{ "name": "capsule1_key_q1", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "capsule1_key", "role": "q1" }} , 
 	{ "name": "capsule1_value_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":5, "type": "signal", "bundle":{"name": "capsule1_value", "role": "address0" }} , 
 	{ "name": "capsule1_value_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "capsule1_value", "role": "ce0" }} , 
 	{ "name": "capsule1_value_q0", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "capsule1_value", "role": "q0" }} , 
 	{ "name": "capsule2_key_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":5, "type": "signal", "bundle":{"name": "capsule2_key", "role": "address0" }} , 
 	{ "name": "capsule2_key_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "capsule2_key", "role": "ce0" }} , 
 	{ "name": "capsule2_key_q0", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "capsule2_key", "role": "q0" }} , 
 	{ "name": "capsule2_key_address1", "direction": "out", "datatype": "sc_lv", "bitwidth":5, "type": "signal", "bundle":{"name": "capsule2_key", "role": "address1" }} , 
 	{ "name": "capsule2_key_ce1", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "capsule2_key", "role": "ce1" }} , 
 	{ "name": "capsule2_key_q1", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "capsule2_key", "role": "q1" }} , 
 	{ "name": "capsule2_value_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":5, "type": "signal", "bundle":{"name": "capsule2_value", "role": "address0" }} , 
 	{ "name": "capsule2_value_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "capsule2_value", "role": "ce0" }} , 
 	{ "name": "capsule2_value_q0", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "capsule2_value", "role": "q0" }} , 
 	{ "name": "capsule3_key_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":5, "type": "signal", "bundle":{"name": "capsule3_key", "role": "address0" }} , 
 	{ "name": "capsule3_key_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "capsule3_key", "role": "ce0" }} , 
 	{ "name": "capsule3_key_q0", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "capsule3_key", "role": "q0" }} , 
 	{ "name": "capsule3_key_address1", "direction": "out", "datatype": "sc_lv", "bitwidth":5, "type": "signal", "bundle":{"name": "capsule3_key", "role": "address1" }} , 
 	{ "name": "capsule3_key_ce1", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "capsule3_key", "role": "ce1" }} , 
 	{ "name": "capsule3_key_q1", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "capsule3_key", "role": "q1" }} , 
 	{ "name": "capsule3_value_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":5, "type": "signal", "bundle":{"name": "capsule3_value", "role": "address0" }} , 
 	{ "name": "capsule3_value_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "capsule3_value", "role": "ce0" }} , 
 	{ "name": "capsule3_value_q0", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "capsule3_value", "role": "q0" }} , 
 	{ "name": "kvdeststats_tmp_key_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":5, "type": "signal", "bundle":{"name": "kvdeststats_tmp_key", "role": "address0" }} , 
 	{ "name": "kvdeststats_tmp_key_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "kvdeststats_tmp_key", "role": "ce0" }} , 
 	{ "name": "kvdeststats_tmp_key_q0", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "kvdeststats_tmp_key", "role": "q0" }} , 
 	{ "name": "kvdeststats_tmp_key_address1", "direction": "out", "datatype": "sc_lv", "bitwidth":5, "type": "signal", "bundle":{"name": "kvdeststats_tmp_key", "role": "address1" }} , 
 	{ "name": "kvdeststats_tmp_key_ce1", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "kvdeststats_tmp_key", "role": "ce1" }} , 
 	{ "name": "kvdeststats_tmp_key_q1", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "kvdeststats_tmp_key", "role": "q1" }} , 
 	{ "name": "kvdeststats_tmp_valu_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":5, "type": "signal", "bundle":{"name": "kvdeststats_tmp_valu", "role": "address0" }} , 
 	{ "name": "kvdeststats_tmp_valu_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "kvdeststats_tmp_valu", "role": "ce0" }} , 
 	{ "name": "kvdeststats_tmp_valu_q0", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "kvdeststats_tmp_valu", "role": "q0" }} , 
 	{ "name": "kvdeststats_tmp_valu_address1", "direction": "out", "datatype": "sc_lv", "bitwidth":5, "type": "signal", "bundle":{"name": "kvdeststats_tmp_valu", "role": "address1" }} , 
 	{ "name": "kvdeststats_tmp_valu_ce1", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "kvdeststats_tmp_valu", "role": "ce1" }} , 
 	{ "name": "kvdeststats_tmp_valu_we1", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "kvdeststats_tmp_valu", "role": "we1" }} , 
 	{ "name": "kvdeststats_tmp_valu_d1", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "kvdeststats_tmp_valu", "role": "d1" }} , 
 	{ "name": "kvdeststats_tmp_valu_q1", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "kvdeststats_tmp_valu", "role": "q1" }} , 
 	{ "name": "kvdrambaseaddress", "direction": "in", "datatype": "sc_lv", "bitwidth":25, "type": "signal", "bundle":{"name": "kvdrambaseaddress", "role": "default" }}  ]}

set RtlHierarchyInfo {[
	{"ID" : "0", "Level" : "0", "Path" : "`AUTOTB_DUT_INST", "Parent" : "", "Child" : ["1", "2", "3", "4", "5", "6", "7", "8"],
		"CDFG" : "savepartitions0",
		"Protocol" : "ap_ctrl_hs",
		"ControlExist" : "1", "ap_start" : "1", "ap_ready" : "1", "ap_done" : "1", "ap_continue" : "0", "ap_idle" : "1",
		"Pipeline" : "None", "UnalignedPipeline" : "0", "RewindPipeline" : "0", "ProcessNetwork" : "0",
		"II" : "0",
		"VariableLatency" : "1", "ExactLatency" : "-1", "EstimateLatencyMin" : "8800", "EstimateLatencyMax" : "8800",
		"Combinational" : "0",
		"Datapath" : "0",
		"ClockEnable" : "0",
		"HasSubDataflow" : "0",
		"InDataflowNetwork" : "0",
		"HasNonBlockingOperation" : "0",
		"Port" : [
			{"Name" : "kvdram_V", "Type" : "MAXI", "Direction" : "O",
				"BlockSignal" : [
					{"Name" : "kvdram_V_blk_n_AW", "Type" : "RtlSignal"},
					{"Name" : "kvdram_V_blk_n_W", "Type" : "RtlSignal"},
					{"Name" : "kvdram_V_blk_n_B", "Type" : "RtlSignal"}]},
			{"Name" : "kvdram_V_offset", "Type" : "None", "Direction" : "I"},
			{"Name" : "destbuffer0_V", "Type" : "Memory", "Direction" : "I"},
			{"Name" : "destbuffer1_V", "Type" : "Memory", "Direction" : "I"},
			{"Name" : "destbuffer2_V", "Type" : "Memory", "Direction" : "I"},
			{"Name" : "destbuffer3_V", "Type" : "Memory", "Direction" : "I"},
			{"Name" : "capsule0_key", "Type" : "Memory", "Direction" : "I"},
			{"Name" : "capsule0_value", "Type" : "Memory", "Direction" : "I"},
			{"Name" : "capsule1_key", "Type" : "Memory", "Direction" : "I"},
			{"Name" : "capsule1_value", "Type" : "Memory", "Direction" : "I"},
			{"Name" : "capsule2_key", "Type" : "Memory", "Direction" : "I"},
			{"Name" : "capsule2_value", "Type" : "Memory", "Direction" : "I"},
			{"Name" : "capsule3_key", "Type" : "Memory", "Direction" : "I"},
			{"Name" : "capsule3_value", "Type" : "Memory", "Direction" : "I"},
			{"Name" : "kvdeststats_tmp_key", "Type" : "Memory", "Direction" : "I"},
			{"Name" : "kvdeststats_tmp_valu", "Type" : "Memory", "Direction" : "IO"},
			{"Name" : "kvdrambaseaddress", "Type" : "None", "Direction" : "I"}]},
	{"ID" : "1", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.topkernel_urem_12ns_8ns_12_16_1_U119", "Parent" : "0"},
	{"ID" : "2", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.topkernel_urem_12ns_8ns_12_16_1_U120", "Parent" : "0"},
	{"ID" : "3", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.topkernel_urem_12ns_8ns_12_16_1_U121", "Parent" : "0"},
	{"ID" : "4", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.topkernel_urem_12ns_8ns_12_16_1_U122", "Parent" : "0"},
	{"ID" : "5", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.topkernel_urem_12ns_8ns_12_16_1_U123", "Parent" : "0"},
	{"ID" : "6", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.topkernel_urem_12ns_8ns_12_16_1_U124", "Parent" : "0"},
	{"ID" : "7", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.topkernel_urem_12ns_8ns_12_16_1_U125", "Parent" : "0"},
	{"ID" : "8", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.topkernel_urem_12ns_8ns_12_16_1_U126", "Parent" : "0"}]}


set ArgLastReadFirstWriteLatency {
	savepartitions0 {
		kvdram_V {Type O LastRead 73 FirstWrite 18}
		kvdram_V_offset {Type I LastRead 0 FirstWrite -1}
		destbuffer0_V {Type I LastRead 16 FirstWrite -1}
		destbuffer1_V {Type I LastRead 34 FirstWrite -1}
		destbuffer2_V {Type I LastRead 52 FirstWrite -1}
		destbuffer3_V {Type I LastRead 70 FirstWrite -1}
		capsule0_key {Type I LastRead 16 FirstWrite -1}
		capsule0_value {Type I LastRead 18 FirstWrite -1}
		capsule1_key {Type I LastRead 34 FirstWrite -1}
		capsule1_value {Type I LastRead 36 FirstWrite -1}
		capsule2_key {Type I LastRead 52 FirstWrite -1}
		capsule2_value {Type I LastRead 54 FirstWrite -1}
		capsule3_key {Type I LastRead 70 FirstWrite -1}
		capsule3_value {Type I LastRead 72 FirstWrite -1}
		kvdeststats_tmp_key {Type I LastRead 70 FirstWrite -1}
		kvdeststats_tmp_valu {Type IO LastRead 72 FirstWrite 19}
		kvdrambaseaddress {Type I LastRead 0 FirstWrite -1}}}

set hasDtUnsupportedChannel 0

set PerformanceInfo {[
	{"Name" : "Latency", "Min" : "8800", "Max" : "8800"}
	, {"Name" : "Interval", "Min" : "8800", "Max" : "8800"}
]}

set PipelineEnableSignalInfo {[
	{"Pipeline" : "0", "EnableSignal" : "ap_enable_pp0"}
	{"Pipeline" : "1", "EnableSignal" : "ap_enable_pp1"}
	{"Pipeline" : "2", "EnableSignal" : "ap_enable_pp2"}
	{"Pipeline" : "3", "EnableSignal" : "ap_enable_pp3"}
]}

set Spec2ImplPortList { 
	kvdram_V { m_axi {  { m_axi_kvdram_V_AWVALID VALID 1 1 }  { m_axi_kvdram_V_AWREADY READY 0 1 }  { m_axi_kvdram_V_AWADDR ADDR 1 32 }  { m_axi_kvdram_V_AWID ID 1 1 }  { m_axi_kvdram_V_AWLEN LEN 1 32 }  { m_axi_kvdram_V_AWSIZE SIZE 1 3 }  { m_axi_kvdram_V_AWBURST BURST 1 2 }  { m_axi_kvdram_V_AWLOCK LOCK 1 2 }  { m_axi_kvdram_V_AWCACHE CACHE 1 4 }  { m_axi_kvdram_V_AWPROT PROT 1 3 }  { m_axi_kvdram_V_AWQOS QOS 1 4 }  { m_axi_kvdram_V_AWREGION REGION 1 4 }  { m_axi_kvdram_V_AWUSER USER 1 1 }  { m_axi_kvdram_V_WVALID VALID 1 1 }  { m_axi_kvdram_V_WREADY READY 0 1 }  { m_axi_kvdram_V_WDATA DATA 1 512 }  { m_axi_kvdram_V_WSTRB STRB 1 64 }  { m_axi_kvdram_V_WLAST LAST 1 1 }  { m_axi_kvdram_V_WID ID 1 1 }  { m_axi_kvdram_V_WUSER USER 1 1 }  { m_axi_kvdram_V_ARVALID VALID 1 1 }  { m_axi_kvdram_V_ARREADY READY 0 1 }  { m_axi_kvdram_V_ARADDR ADDR 1 32 }  { m_axi_kvdram_V_ARID ID 1 1 }  { m_axi_kvdram_V_ARLEN LEN 1 32 }  { m_axi_kvdram_V_ARSIZE SIZE 1 3 }  { m_axi_kvdram_V_ARBURST BURST 1 2 }  { m_axi_kvdram_V_ARLOCK LOCK 1 2 }  { m_axi_kvdram_V_ARCACHE CACHE 1 4 }  { m_axi_kvdram_V_ARPROT PROT 1 3 }  { m_axi_kvdram_V_ARQOS QOS 1 4 }  { m_axi_kvdram_V_ARREGION REGION 1 4 }  { m_axi_kvdram_V_ARUSER USER 1 1 }  { m_axi_kvdram_V_RVALID VALID 0 1 }  { m_axi_kvdram_V_RREADY READY 1 1 }  { m_axi_kvdram_V_RDATA DATA 0 512 }  { m_axi_kvdram_V_RLAST LAST 0 1 }  { m_axi_kvdram_V_RID ID 0 1 }  { m_axi_kvdram_V_RUSER USER 0 1 }  { m_axi_kvdram_V_RRESP RESP 0 2 }  { m_axi_kvdram_V_BVALID VALID 0 1 }  { m_axi_kvdram_V_BREADY READY 1 1 }  { m_axi_kvdram_V_BRESP RESP 0 2 }  { m_axi_kvdram_V_BID ID 0 1 }  { m_axi_kvdram_V_BUSER USER 0 1 } } }
	kvdram_V_offset { ap_none {  { kvdram_V_offset in_data 0 26 } } }
	destbuffer0_V { ap_memory {  { destbuffer0_V_address0 mem_address 1 12 }  { destbuffer0_V_ce0 mem_ce 1 1 }  { destbuffer0_V_q0 mem_dout 0 512 } } }
	destbuffer1_V { ap_memory {  { destbuffer1_V_address0 mem_address 1 12 }  { destbuffer1_V_ce0 mem_ce 1 1 }  { destbuffer1_V_q0 mem_dout 0 512 } } }
	destbuffer2_V { ap_memory {  { destbuffer2_V_address0 mem_address 1 12 }  { destbuffer2_V_ce0 mem_ce 1 1 }  { destbuffer2_V_q0 mem_dout 0 512 } } }
	destbuffer3_V { ap_memory {  { destbuffer3_V_address0 mem_address 1 12 }  { destbuffer3_V_ce0 mem_ce 1 1 }  { destbuffer3_V_q0 mem_dout 0 512 } } }
	capsule0_key { ap_memory {  { capsule0_key_address0 mem_address 1 5 }  { capsule0_key_ce0 mem_ce 1 1 }  { capsule0_key_q0 mem_dout 0 32 }  { capsule0_key_address1 MemPortADDR2 1 5 }  { capsule0_key_ce1 MemPortCE2 1 1 }  { capsule0_key_q1 MemPortDOUT2 0 32 } } }
	capsule0_value { ap_memory {  { capsule0_value_address0 mem_address 1 5 }  { capsule0_value_ce0 mem_ce 1 1 }  { capsule0_value_q0 mem_dout 0 32 } } }
	capsule1_key { ap_memory {  { capsule1_key_address0 mem_address 1 5 }  { capsule1_key_ce0 mem_ce 1 1 }  { capsule1_key_q0 mem_dout 0 32 }  { capsule1_key_address1 MemPortADDR2 1 5 }  { capsule1_key_ce1 MemPortCE2 1 1 }  { capsule1_key_q1 MemPortDOUT2 0 32 } } }
	capsule1_value { ap_memory {  { capsule1_value_address0 mem_address 1 5 }  { capsule1_value_ce0 mem_ce 1 1 }  { capsule1_value_q0 mem_dout 0 32 } } }
	capsule2_key { ap_memory {  { capsule2_key_address0 mem_address 1 5 }  { capsule2_key_ce0 mem_ce 1 1 }  { capsule2_key_q0 mem_dout 0 32 }  { capsule2_key_address1 MemPortADDR2 1 5 }  { capsule2_key_ce1 MemPortCE2 1 1 }  { capsule2_key_q1 MemPortDOUT2 0 32 } } }
	capsule2_value { ap_memory {  { capsule2_value_address0 mem_address 1 5 }  { capsule2_value_ce0 mem_ce 1 1 }  { capsule2_value_q0 mem_dout 0 32 } } }
	capsule3_key { ap_memory {  { capsule3_key_address0 mem_address 1 5 }  { capsule3_key_ce0 mem_ce 1 1 }  { capsule3_key_q0 mem_dout 0 32 }  { capsule3_key_address1 MemPortADDR2 1 5 }  { capsule3_key_ce1 MemPortCE2 1 1 }  { capsule3_key_q1 MemPortDOUT2 0 32 } } }
	capsule3_value { ap_memory {  { capsule3_value_address0 mem_address 1 5 }  { capsule3_value_ce0 mem_ce 1 1 }  { capsule3_value_q0 mem_dout 0 32 } } }
	kvdeststats_tmp_key { ap_memory {  { kvdeststats_tmp_key_address0 mem_address 1 5 }  { kvdeststats_tmp_key_ce0 mem_ce 1 1 }  { kvdeststats_tmp_key_q0 mem_dout 0 32 }  { kvdeststats_tmp_key_address1 MemPortADDR2 1 5 }  { kvdeststats_tmp_key_ce1 MemPortCE2 1 1 }  { kvdeststats_tmp_key_q1 MemPortDOUT2 0 32 } } }
	kvdeststats_tmp_valu { ap_memory {  { kvdeststats_tmp_valu_address0 mem_address 1 5 }  { kvdeststats_tmp_valu_ce0 mem_ce 1 1 }  { kvdeststats_tmp_valu_q0 mem_dout 0 32 }  { kvdeststats_tmp_valu_address1 MemPortADDR2 1 5 }  { kvdeststats_tmp_valu_ce1 MemPortCE2 1 1 }  { kvdeststats_tmp_valu_we1 MemPortWE2 1 1 }  { kvdeststats_tmp_valu_d1 MemPortDIN2 1 32 }  { kvdeststats_tmp_valu_q1 MemPortDOUT2 0 32 } } }
	kvdrambaseaddress { ap_none {  { kvdrambaseaddress in_data 0 25 } } }
}
set moduleName savepartitions0
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
set C_modelName {savepartitions0}
set C_modelType { void 0 }
set C_modelArgList {
	{ kvdram_V int 512 regular {axi_master 1}  }
	{ kvdram_V_offset int 26 regular  }
	{ destbuffer0_V int 512 regular {array 2112 { 1 3 } 1 1 }  }
	{ destbuffer1_V int 512 regular {array 2112 { 1 3 } 1 1 }  }
	{ destbuffer2_V int 512 regular {array 2112 { 1 3 } 1 1 }  }
	{ destbuffer3_V int 512 regular {array 2112 { 1 3 } 1 1 }  }
	{ capsule0_key int 32 regular {array 32 { 1 1 } 1 1 }  }
	{ capsule0_value int 32 regular {array 32 { 1 3 } 1 1 }  }
	{ capsule1_key int 32 regular {array 32 { 1 1 } 1 1 }  }
	{ capsule1_value int 32 regular {array 32 { 1 3 } 1 1 }  }
	{ capsule2_key int 32 regular {array 32 { 1 1 } 1 1 }  }
	{ capsule2_value int 32 regular {array 32 { 1 3 } 1 1 }  }
	{ capsule3_key int 32 regular {array 32 { 1 1 } 1 1 }  }
	{ capsule3_value int 32 regular {array 32 { 1 3 } 1 1 }  }
	{ kvdeststats_tmp_key int 32 regular {array 32 { 1 1 } 1 1 }  }
	{ kvdeststats_tmp_valu int 32 regular {array 32 { 1 2 } 1 1 }  }
	{ kvdrambaseaddress int 25 regular  }
}
set C_modelArgMapList {[ 
	{ "Name" : "kvdram_V", "interface" : "axi_master", "bitwidth" : 512, "direction" : "WRITEONLY"} , 
 	{ "Name" : "kvdram_V_offset", "interface" : "wire", "bitwidth" : 26, "direction" : "READONLY"} , 
 	{ "Name" : "destbuffer0_V", "interface" : "memory", "bitwidth" : 512, "direction" : "READONLY"} , 
 	{ "Name" : "destbuffer1_V", "interface" : "memory", "bitwidth" : 512, "direction" : "READONLY"} , 
 	{ "Name" : "destbuffer2_V", "interface" : "memory", "bitwidth" : 512, "direction" : "READONLY"} , 
 	{ "Name" : "destbuffer3_V", "interface" : "memory", "bitwidth" : 512, "direction" : "READONLY"} , 
 	{ "Name" : "capsule0_key", "interface" : "memory", "bitwidth" : 32, "direction" : "READONLY"} , 
 	{ "Name" : "capsule0_value", "interface" : "memory", "bitwidth" : 32, "direction" : "READONLY"} , 
 	{ "Name" : "capsule1_key", "interface" : "memory", "bitwidth" : 32, "direction" : "READONLY"} , 
 	{ "Name" : "capsule1_value", "interface" : "memory", "bitwidth" : 32, "direction" : "READONLY"} , 
 	{ "Name" : "capsule2_key", "interface" : "memory", "bitwidth" : 32, "direction" : "READONLY"} , 
 	{ "Name" : "capsule2_value", "interface" : "memory", "bitwidth" : 32, "direction" : "READONLY"} , 
 	{ "Name" : "capsule3_key", "interface" : "memory", "bitwidth" : 32, "direction" : "READONLY"} , 
 	{ "Name" : "capsule3_value", "interface" : "memory", "bitwidth" : 32, "direction" : "READONLY"} , 
 	{ "Name" : "kvdeststats_tmp_key", "interface" : "memory", "bitwidth" : 32, "direction" : "READONLY"} , 
 	{ "Name" : "kvdeststats_tmp_valu", "interface" : "memory", "bitwidth" : 32, "direction" : "READWRITE"} , 
 	{ "Name" : "kvdrambaseaddress", "interface" : "wire", "bitwidth" : 25, "direction" : "READONLY"} ]}
# RTL Port declarations: 
set portNum 115
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
	{ destbuffer0_V_address0 sc_out sc_lv 12 signal 2 } 
	{ destbuffer0_V_ce0 sc_out sc_logic 1 signal 2 } 
	{ destbuffer0_V_q0 sc_in sc_lv 512 signal 2 } 
	{ destbuffer1_V_address0 sc_out sc_lv 12 signal 3 } 
	{ destbuffer1_V_ce0 sc_out sc_logic 1 signal 3 } 
	{ destbuffer1_V_q0 sc_in sc_lv 512 signal 3 } 
	{ destbuffer2_V_address0 sc_out sc_lv 12 signal 4 } 
	{ destbuffer2_V_ce0 sc_out sc_logic 1 signal 4 } 
	{ destbuffer2_V_q0 sc_in sc_lv 512 signal 4 } 
	{ destbuffer3_V_address0 sc_out sc_lv 12 signal 5 } 
	{ destbuffer3_V_ce0 sc_out sc_logic 1 signal 5 } 
	{ destbuffer3_V_q0 sc_in sc_lv 512 signal 5 } 
	{ capsule0_key_address0 sc_out sc_lv 5 signal 6 } 
	{ capsule0_key_ce0 sc_out sc_logic 1 signal 6 } 
	{ capsule0_key_q0 sc_in sc_lv 32 signal 6 } 
	{ capsule0_key_address1 sc_out sc_lv 5 signal 6 } 
	{ capsule0_key_ce1 sc_out sc_logic 1 signal 6 } 
	{ capsule0_key_q1 sc_in sc_lv 32 signal 6 } 
	{ capsule0_value_address0 sc_out sc_lv 5 signal 7 } 
	{ capsule0_value_ce0 sc_out sc_logic 1 signal 7 } 
	{ capsule0_value_q0 sc_in sc_lv 32 signal 7 } 
	{ capsule1_key_address0 sc_out sc_lv 5 signal 8 } 
	{ capsule1_key_ce0 sc_out sc_logic 1 signal 8 } 
	{ capsule1_key_q0 sc_in sc_lv 32 signal 8 } 
	{ capsule1_key_address1 sc_out sc_lv 5 signal 8 } 
	{ capsule1_key_ce1 sc_out sc_logic 1 signal 8 } 
	{ capsule1_key_q1 sc_in sc_lv 32 signal 8 } 
	{ capsule1_value_address0 sc_out sc_lv 5 signal 9 } 
	{ capsule1_value_ce0 sc_out sc_logic 1 signal 9 } 
	{ capsule1_value_q0 sc_in sc_lv 32 signal 9 } 
	{ capsule2_key_address0 sc_out sc_lv 5 signal 10 } 
	{ capsule2_key_ce0 sc_out sc_logic 1 signal 10 } 
	{ capsule2_key_q0 sc_in sc_lv 32 signal 10 } 
	{ capsule2_key_address1 sc_out sc_lv 5 signal 10 } 
	{ capsule2_key_ce1 sc_out sc_logic 1 signal 10 } 
	{ capsule2_key_q1 sc_in sc_lv 32 signal 10 } 
	{ capsule2_value_address0 sc_out sc_lv 5 signal 11 } 
	{ capsule2_value_ce0 sc_out sc_logic 1 signal 11 } 
	{ capsule2_value_q0 sc_in sc_lv 32 signal 11 } 
	{ capsule3_key_address0 sc_out sc_lv 5 signal 12 } 
	{ capsule3_key_ce0 sc_out sc_logic 1 signal 12 } 
	{ capsule3_key_q0 sc_in sc_lv 32 signal 12 } 
	{ capsule3_key_address1 sc_out sc_lv 5 signal 12 } 
	{ capsule3_key_ce1 sc_out sc_logic 1 signal 12 } 
	{ capsule3_key_q1 sc_in sc_lv 32 signal 12 } 
	{ capsule3_value_address0 sc_out sc_lv 5 signal 13 } 
	{ capsule3_value_ce0 sc_out sc_logic 1 signal 13 } 
	{ capsule3_value_q0 sc_in sc_lv 32 signal 13 } 
	{ kvdeststats_tmp_key_address0 sc_out sc_lv 5 signal 14 } 
	{ kvdeststats_tmp_key_ce0 sc_out sc_logic 1 signal 14 } 
	{ kvdeststats_tmp_key_q0 sc_in sc_lv 32 signal 14 } 
	{ kvdeststats_tmp_key_address1 sc_out sc_lv 5 signal 14 } 
	{ kvdeststats_tmp_key_ce1 sc_out sc_logic 1 signal 14 } 
	{ kvdeststats_tmp_key_q1 sc_in sc_lv 32 signal 14 } 
	{ kvdeststats_tmp_valu_address0 sc_out sc_lv 5 signal 15 } 
	{ kvdeststats_tmp_valu_ce0 sc_out sc_logic 1 signal 15 } 
	{ kvdeststats_tmp_valu_q0 sc_in sc_lv 32 signal 15 } 
	{ kvdeststats_tmp_valu_address1 sc_out sc_lv 5 signal 15 } 
	{ kvdeststats_tmp_valu_ce1 sc_out sc_logic 1 signal 15 } 
	{ kvdeststats_tmp_valu_we1 sc_out sc_logic 1 signal 15 } 
	{ kvdeststats_tmp_valu_d1 sc_out sc_lv 32 signal 15 } 
	{ kvdeststats_tmp_valu_q1 sc_in sc_lv 32 signal 15 } 
	{ kvdrambaseaddress sc_in sc_lv 25 signal 16 } 
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
 	{ "name": "destbuffer0_V_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":12, "type": "signal", "bundle":{"name": "destbuffer0_V", "role": "address0" }} , 
 	{ "name": "destbuffer0_V_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "destbuffer0_V", "role": "ce0" }} , 
 	{ "name": "destbuffer0_V_q0", "direction": "in", "datatype": "sc_lv", "bitwidth":512, "type": "signal", "bundle":{"name": "destbuffer0_V", "role": "q0" }} , 
 	{ "name": "destbuffer1_V_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":12, "type": "signal", "bundle":{"name": "destbuffer1_V", "role": "address0" }} , 
 	{ "name": "destbuffer1_V_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "destbuffer1_V", "role": "ce0" }} , 
 	{ "name": "destbuffer1_V_q0", "direction": "in", "datatype": "sc_lv", "bitwidth":512, "type": "signal", "bundle":{"name": "destbuffer1_V", "role": "q0" }} , 
 	{ "name": "destbuffer2_V_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":12, "type": "signal", "bundle":{"name": "destbuffer2_V", "role": "address0" }} , 
 	{ "name": "destbuffer2_V_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "destbuffer2_V", "role": "ce0" }} , 
 	{ "name": "destbuffer2_V_q0", "direction": "in", "datatype": "sc_lv", "bitwidth":512, "type": "signal", "bundle":{"name": "destbuffer2_V", "role": "q0" }} , 
 	{ "name": "destbuffer3_V_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":12, "type": "signal", "bundle":{"name": "destbuffer3_V", "role": "address0" }} , 
 	{ "name": "destbuffer3_V_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "destbuffer3_V", "role": "ce0" }} , 
 	{ "name": "destbuffer3_V_q0", "direction": "in", "datatype": "sc_lv", "bitwidth":512, "type": "signal", "bundle":{"name": "destbuffer3_V", "role": "q0" }} , 
 	{ "name": "capsule0_key_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":5, "type": "signal", "bundle":{"name": "capsule0_key", "role": "address0" }} , 
 	{ "name": "capsule0_key_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "capsule0_key", "role": "ce0" }} , 
 	{ "name": "capsule0_key_q0", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "capsule0_key", "role": "q0" }} , 
 	{ "name": "capsule0_key_address1", "direction": "out", "datatype": "sc_lv", "bitwidth":5, "type": "signal", "bundle":{"name": "capsule0_key", "role": "address1" }} , 
 	{ "name": "capsule0_key_ce1", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "capsule0_key", "role": "ce1" }} , 
 	{ "name": "capsule0_key_q1", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "capsule0_key", "role": "q1" }} , 
 	{ "name": "capsule0_value_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":5, "type": "signal", "bundle":{"name": "capsule0_value", "role": "address0" }} , 
 	{ "name": "capsule0_value_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "capsule0_value", "role": "ce0" }} , 
 	{ "name": "capsule0_value_q0", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "capsule0_value", "role": "q0" }} , 
 	{ "name": "capsule1_key_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":5, "type": "signal", "bundle":{"name": "capsule1_key", "role": "address0" }} , 
 	{ "name": "capsule1_key_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "capsule1_key", "role": "ce0" }} , 
 	{ "name": "capsule1_key_q0", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "capsule1_key", "role": "q0" }} , 
 	{ "name": "capsule1_key_address1", "direction": "out", "datatype": "sc_lv", "bitwidth":5, "type": "signal", "bundle":{"name": "capsule1_key", "role": "address1" }} , 
 	{ "name": "capsule1_key_ce1", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "capsule1_key", "role": "ce1" }} , 
 	{ "name": "capsule1_key_q1", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "capsule1_key", "role": "q1" }} , 
 	{ "name": "capsule1_value_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":5, "type": "signal", "bundle":{"name": "capsule1_value", "role": "address0" }} , 
 	{ "name": "capsule1_value_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "capsule1_value", "role": "ce0" }} , 
 	{ "name": "capsule1_value_q0", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "capsule1_value", "role": "q0" }} , 
 	{ "name": "capsule2_key_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":5, "type": "signal", "bundle":{"name": "capsule2_key", "role": "address0" }} , 
 	{ "name": "capsule2_key_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "capsule2_key", "role": "ce0" }} , 
 	{ "name": "capsule2_key_q0", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "capsule2_key", "role": "q0" }} , 
 	{ "name": "capsule2_key_address1", "direction": "out", "datatype": "sc_lv", "bitwidth":5, "type": "signal", "bundle":{"name": "capsule2_key", "role": "address1" }} , 
 	{ "name": "capsule2_key_ce1", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "capsule2_key", "role": "ce1" }} , 
 	{ "name": "capsule2_key_q1", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "capsule2_key", "role": "q1" }} , 
 	{ "name": "capsule2_value_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":5, "type": "signal", "bundle":{"name": "capsule2_value", "role": "address0" }} , 
 	{ "name": "capsule2_value_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "capsule2_value", "role": "ce0" }} , 
 	{ "name": "capsule2_value_q0", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "capsule2_value", "role": "q0" }} , 
 	{ "name": "capsule3_key_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":5, "type": "signal", "bundle":{"name": "capsule3_key", "role": "address0" }} , 
 	{ "name": "capsule3_key_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "capsule3_key", "role": "ce0" }} , 
 	{ "name": "capsule3_key_q0", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "capsule3_key", "role": "q0" }} , 
 	{ "name": "capsule3_key_address1", "direction": "out", "datatype": "sc_lv", "bitwidth":5, "type": "signal", "bundle":{"name": "capsule3_key", "role": "address1" }} , 
 	{ "name": "capsule3_key_ce1", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "capsule3_key", "role": "ce1" }} , 
 	{ "name": "capsule3_key_q1", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "capsule3_key", "role": "q1" }} , 
 	{ "name": "capsule3_value_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":5, "type": "signal", "bundle":{"name": "capsule3_value", "role": "address0" }} , 
 	{ "name": "capsule3_value_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "capsule3_value", "role": "ce0" }} , 
 	{ "name": "capsule3_value_q0", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "capsule3_value", "role": "q0" }} , 
 	{ "name": "kvdeststats_tmp_key_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":5, "type": "signal", "bundle":{"name": "kvdeststats_tmp_key", "role": "address0" }} , 
 	{ "name": "kvdeststats_tmp_key_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "kvdeststats_tmp_key", "role": "ce0" }} , 
 	{ "name": "kvdeststats_tmp_key_q0", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "kvdeststats_tmp_key", "role": "q0" }} , 
 	{ "name": "kvdeststats_tmp_key_address1", "direction": "out", "datatype": "sc_lv", "bitwidth":5, "type": "signal", "bundle":{"name": "kvdeststats_tmp_key", "role": "address1" }} , 
 	{ "name": "kvdeststats_tmp_key_ce1", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "kvdeststats_tmp_key", "role": "ce1" }} , 
 	{ "name": "kvdeststats_tmp_key_q1", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "kvdeststats_tmp_key", "role": "q1" }} , 
 	{ "name": "kvdeststats_tmp_valu_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":5, "type": "signal", "bundle":{"name": "kvdeststats_tmp_valu", "role": "address0" }} , 
 	{ "name": "kvdeststats_tmp_valu_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "kvdeststats_tmp_valu", "role": "ce0" }} , 
 	{ "name": "kvdeststats_tmp_valu_q0", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "kvdeststats_tmp_valu", "role": "q0" }} , 
 	{ "name": "kvdeststats_tmp_valu_address1", "direction": "out", "datatype": "sc_lv", "bitwidth":5, "type": "signal", "bundle":{"name": "kvdeststats_tmp_valu", "role": "address1" }} , 
 	{ "name": "kvdeststats_tmp_valu_ce1", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "kvdeststats_tmp_valu", "role": "ce1" }} , 
 	{ "name": "kvdeststats_tmp_valu_we1", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "kvdeststats_tmp_valu", "role": "we1" }} , 
 	{ "name": "kvdeststats_tmp_valu_d1", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "kvdeststats_tmp_valu", "role": "d1" }} , 
 	{ "name": "kvdeststats_tmp_valu_q1", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "kvdeststats_tmp_valu", "role": "q1" }} , 
 	{ "name": "kvdrambaseaddress", "direction": "in", "datatype": "sc_lv", "bitwidth":25, "type": "signal", "bundle":{"name": "kvdrambaseaddress", "role": "default" }}  ]}

set RtlHierarchyInfo {[
	{"ID" : "0", "Level" : "0", "Path" : "`AUTOTB_DUT_INST", "Parent" : "", "Child" : ["1", "2", "3", "4", "5", "6", "7", "8"],
		"CDFG" : "savepartitions0",
		"Protocol" : "ap_ctrl_hs",
		"ControlExist" : "1", "ap_start" : "1", "ap_ready" : "1", "ap_done" : "1", "ap_continue" : "0", "ap_idle" : "1",
		"Pipeline" : "None", "UnalignedPipeline" : "0", "RewindPipeline" : "0", "ProcessNetwork" : "0",
		"II" : "0",
		"VariableLatency" : "1", "ExactLatency" : "-1", "EstimateLatencyMin" : "8800", "EstimateLatencyMax" : "8800",
		"Combinational" : "0",
		"Datapath" : "0",
		"ClockEnable" : "0",
		"HasSubDataflow" : "0",
		"InDataflowNetwork" : "0",
		"HasNonBlockingOperation" : "0",
		"Port" : [
			{"Name" : "kvdram_V", "Type" : "MAXI", "Direction" : "O",
				"BlockSignal" : [
					{"Name" : "kvdram_V_blk_n_AW", "Type" : "RtlSignal"},
					{"Name" : "kvdram_V_blk_n_W", "Type" : "RtlSignal"},
					{"Name" : "kvdram_V_blk_n_B", "Type" : "RtlSignal"}]},
			{"Name" : "kvdram_V_offset", "Type" : "None", "Direction" : "I"},
			{"Name" : "destbuffer0_V", "Type" : "Memory", "Direction" : "I"},
			{"Name" : "destbuffer1_V", "Type" : "Memory", "Direction" : "I"},
			{"Name" : "destbuffer2_V", "Type" : "Memory", "Direction" : "I"},
			{"Name" : "destbuffer3_V", "Type" : "Memory", "Direction" : "I"},
			{"Name" : "capsule0_key", "Type" : "Memory", "Direction" : "I"},
			{"Name" : "capsule0_value", "Type" : "Memory", "Direction" : "I"},
			{"Name" : "capsule1_key", "Type" : "Memory", "Direction" : "I"},
			{"Name" : "capsule1_value", "Type" : "Memory", "Direction" : "I"},
			{"Name" : "capsule2_key", "Type" : "Memory", "Direction" : "I"},
			{"Name" : "capsule2_value", "Type" : "Memory", "Direction" : "I"},
			{"Name" : "capsule3_key", "Type" : "Memory", "Direction" : "I"},
			{"Name" : "capsule3_value", "Type" : "Memory", "Direction" : "I"},
			{"Name" : "kvdeststats_tmp_key", "Type" : "Memory", "Direction" : "I"},
			{"Name" : "kvdeststats_tmp_valu", "Type" : "Memory", "Direction" : "IO"},
			{"Name" : "kvdrambaseaddress", "Type" : "None", "Direction" : "I"}]},
	{"ID" : "1", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.topkernel_urem_12ns_8ns_12_16_1_U119", "Parent" : "0"},
	{"ID" : "2", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.topkernel_urem_12ns_8ns_12_16_1_U120", "Parent" : "0"},
	{"ID" : "3", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.topkernel_urem_12ns_8ns_12_16_1_U121", "Parent" : "0"},
	{"ID" : "4", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.topkernel_urem_12ns_8ns_12_16_1_U122", "Parent" : "0"},
	{"ID" : "5", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.topkernel_urem_12ns_8ns_12_16_1_U123", "Parent" : "0"},
	{"ID" : "6", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.topkernel_urem_12ns_8ns_12_16_1_U124", "Parent" : "0"},
	{"ID" : "7", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.topkernel_urem_12ns_8ns_12_16_1_U125", "Parent" : "0"},
	{"ID" : "8", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.topkernel_urem_12ns_8ns_12_16_1_U126", "Parent" : "0"}]}


set ArgLastReadFirstWriteLatency {
	savepartitions0 {
		kvdram_V {Type O LastRead 73 FirstWrite 18}
		kvdram_V_offset {Type I LastRead 0 FirstWrite -1}
		destbuffer0_V {Type I LastRead 16 FirstWrite -1}
		destbuffer1_V {Type I LastRead 34 FirstWrite -1}
		destbuffer2_V {Type I LastRead 52 FirstWrite -1}
		destbuffer3_V {Type I LastRead 70 FirstWrite -1}
		capsule0_key {Type I LastRead 16 FirstWrite -1}
		capsule0_value {Type I LastRead 18 FirstWrite -1}
		capsule1_key {Type I LastRead 34 FirstWrite -1}
		capsule1_value {Type I LastRead 36 FirstWrite -1}
		capsule2_key {Type I LastRead 52 FirstWrite -1}
		capsule2_value {Type I LastRead 54 FirstWrite -1}
		capsule3_key {Type I LastRead 70 FirstWrite -1}
		capsule3_value {Type I LastRead 72 FirstWrite -1}
		kvdeststats_tmp_key {Type I LastRead 70 FirstWrite -1}
		kvdeststats_tmp_valu {Type IO LastRead 72 FirstWrite 19}
		kvdrambaseaddress {Type I LastRead 0 FirstWrite -1}}}

set hasDtUnsupportedChannel 0

set PerformanceInfo {[
	{"Name" : "Latency", "Min" : "8800", "Max" : "8800"}
	, {"Name" : "Interval", "Min" : "8800", "Max" : "8800"}
]}

set PipelineEnableSignalInfo {[
	{"Pipeline" : "0", "EnableSignal" : "ap_enable_pp0"}
	{"Pipeline" : "1", "EnableSignal" : "ap_enable_pp1"}
	{"Pipeline" : "2", "EnableSignal" : "ap_enable_pp2"}
	{"Pipeline" : "3", "EnableSignal" : "ap_enable_pp3"}
]}

set Spec2ImplPortList { 
	kvdram_V { m_axi {  { m_axi_kvdram_V_AWVALID VALID 1 1 }  { m_axi_kvdram_V_AWREADY READY 0 1 }  { m_axi_kvdram_V_AWADDR ADDR 1 32 }  { m_axi_kvdram_V_AWID ID 1 1 }  { m_axi_kvdram_V_AWLEN LEN 1 32 }  { m_axi_kvdram_V_AWSIZE SIZE 1 3 }  { m_axi_kvdram_V_AWBURST BURST 1 2 }  { m_axi_kvdram_V_AWLOCK LOCK 1 2 }  { m_axi_kvdram_V_AWCACHE CACHE 1 4 }  { m_axi_kvdram_V_AWPROT PROT 1 3 }  { m_axi_kvdram_V_AWQOS QOS 1 4 }  { m_axi_kvdram_V_AWREGION REGION 1 4 }  { m_axi_kvdram_V_AWUSER USER 1 1 }  { m_axi_kvdram_V_WVALID VALID 1 1 }  { m_axi_kvdram_V_WREADY READY 0 1 }  { m_axi_kvdram_V_WDATA DATA 1 512 }  { m_axi_kvdram_V_WSTRB STRB 1 64 }  { m_axi_kvdram_V_WLAST LAST 1 1 }  { m_axi_kvdram_V_WID ID 1 1 }  { m_axi_kvdram_V_WUSER USER 1 1 }  { m_axi_kvdram_V_ARVALID VALID 1 1 }  { m_axi_kvdram_V_ARREADY READY 0 1 }  { m_axi_kvdram_V_ARADDR ADDR 1 32 }  { m_axi_kvdram_V_ARID ID 1 1 }  { m_axi_kvdram_V_ARLEN LEN 1 32 }  { m_axi_kvdram_V_ARSIZE SIZE 1 3 }  { m_axi_kvdram_V_ARBURST BURST 1 2 }  { m_axi_kvdram_V_ARLOCK LOCK 1 2 }  { m_axi_kvdram_V_ARCACHE CACHE 1 4 }  { m_axi_kvdram_V_ARPROT PROT 1 3 }  { m_axi_kvdram_V_ARQOS QOS 1 4 }  { m_axi_kvdram_V_ARREGION REGION 1 4 }  { m_axi_kvdram_V_ARUSER USER 1 1 }  { m_axi_kvdram_V_RVALID VALID 0 1 }  { m_axi_kvdram_V_RREADY READY 1 1 }  { m_axi_kvdram_V_RDATA DATA 0 512 }  { m_axi_kvdram_V_RLAST LAST 0 1 }  { m_axi_kvdram_V_RID ID 0 1 }  { m_axi_kvdram_V_RUSER USER 0 1 }  { m_axi_kvdram_V_RRESP RESP 0 2 }  { m_axi_kvdram_V_BVALID VALID 0 1 }  { m_axi_kvdram_V_BREADY READY 1 1 }  { m_axi_kvdram_V_BRESP RESP 0 2 }  { m_axi_kvdram_V_BID ID 0 1 }  { m_axi_kvdram_V_BUSER USER 0 1 } } }
	kvdram_V_offset { ap_none {  { kvdram_V_offset in_data 0 26 } } }
	destbuffer0_V { ap_memory {  { destbuffer0_V_address0 mem_address 1 12 }  { destbuffer0_V_ce0 mem_ce 1 1 }  { destbuffer0_V_q0 mem_dout 0 512 } } }
	destbuffer1_V { ap_memory {  { destbuffer1_V_address0 mem_address 1 12 }  { destbuffer1_V_ce0 mem_ce 1 1 }  { destbuffer1_V_q0 mem_dout 0 512 } } }
	destbuffer2_V { ap_memory {  { destbuffer2_V_address0 mem_address 1 12 }  { destbuffer2_V_ce0 mem_ce 1 1 }  { destbuffer2_V_q0 mem_dout 0 512 } } }
	destbuffer3_V { ap_memory {  { destbuffer3_V_address0 mem_address 1 12 }  { destbuffer3_V_ce0 mem_ce 1 1 }  { destbuffer3_V_q0 mem_dout 0 512 } } }
	capsule0_key { ap_memory {  { capsule0_key_address0 mem_address 1 5 }  { capsule0_key_ce0 mem_ce 1 1 }  { capsule0_key_q0 mem_dout 0 32 }  { capsule0_key_address1 MemPortADDR2 1 5 }  { capsule0_key_ce1 MemPortCE2 1 1 }  { capsule0_key_q1 MemPortDOUT2 0 32 } } }
	capsule0_value { ap_memory {  { capsule0_value_address0 mem_address 1 5 }  { capsule0_value_ce0 mem_ce 1 1 }  { capsule0_value_q0 mem_dout 0 32 } } }
	capsule1_key { ap_memory {  { capsule1_key_address0 mem_address 1 5 }  { capsule1_key_ce0 mem_ce 1 1 }  { capsule1_key_q0 mem_dout 0 32 }  { capsule1_key_address1 MemPortADDR2 1 5 }  { capsule1_key_ce1 MemPortCE2 1 1 }  { capsule1_key_q1 MemPortDOUT2 0 32 } } }
	capsule1_value { ap_memory {  { capsule1_value_address0 mem_address 1 5 }  { capsule1_value_ce0 mem_ce 1 1 }  { capsule1_value_q0 mem_dout 0 32 } } }
	capsule2_key { ap_memory {  { capsule2_key_address0 mem_address 1 5 }  { capsule2_key_ce0 mem_ce 1 1 }  { capsule2_key_q0 mem_dout 0 32 }  { capsule2_key_address1 MemPortADDR2 1 5 }  { capsule2_key_ce1 MemPortCE2 1 1 }  { capsule2_key_q1 MemPortDOUT2 0 32 } } }
	capsule2_value { ap_memory {  { capsule2_value_address0 mem_address 1 5 }  { capsule2_value_ce0 mem_ce 1 1 }  { capsule2_value_q0 mem_dout 0 32 } } }
	capsule3_key { ap_memory {  { capsule3_key_address0 mem_address 1 5 }  { capsule3_key_ce0 mem_ce 1 1 }  { capsule3_key_q0 mem_dout 0 32 }  { capsule3_key_address1 MemPortADDR2 1 5 }  { capsule3_key_ce1 MemPortCE2 1 1 }  { capsule3_key_q1 MemPortDOUT2 0 32 } } }
	capsule3_value { ap_memory {  { capsule3_value_address0 mem_address 1 5 }  { capsule3_value_ce0 mem_ce 1 1 }  { capsule3_value_q0 mem_dout 0 32 } } }
	kvdeststats_tmp_key { ap_memory {  { kvdeststats_tmp_key_address0 mem_address 1 5 }  { kvdeststats_tmp_key_ce0 mem_ce 1 1 }  { kvdeststats_tmp_key_q0 mem_dout 0 32 }  { kvdeststats_tmp_key_address1 MemPortADDR2 1 5 }  { kvdeststats_tmp_key_ce1 MemPortCE2 1 1 }  { kvdeststats_tmp_key_q1 MemPortDOUT2 0 32 } } }
	kvdeststats_tmp_valu { ap_memory {  { kvdeststats_tmp_valu_address0 mem_address 1 5 }  { kvdeststats_tmp_valu_ce0 mem_ce 1 1 }  { kvdeststats_tmp_valu_q0 mem_dout 0 32 }  { kvdeststats_tmp_valu_address1 MemPortADDR2 1 5 }  { kvdeststats_tmp_valu_ce1 MemPortCE2 1 1 }  { kvdeststats_tmp_valu_we1 MemPortWE2 1 1 }  { kvdeststats_tmp_valu_d1 MemPortDIN2 1 32 }  { kvdeststats_tmp_valu_q1 MemPortDOUT2 0 32 } } }
	kvdrambaseaddress { ap_none {  { kvdrambaseaddress in_data 0 25 } } }
}
