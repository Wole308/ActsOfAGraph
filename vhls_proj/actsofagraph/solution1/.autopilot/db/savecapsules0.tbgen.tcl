set moduleName savecapsules0
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
set C_modelName {savecapsules0}
set C_modelType { int 64 }
set C_modelArgList {
	{ kvdram_V int 512 regular {axi_master 1}  }
	{ kvdram_V_offset int 26 regular  }
	{ BIGcapsule0_V int 512 regular {array 512 { 2 3 } 1 1 }  }
	{ BIGcapsule1_V int 512 regular {array 512 { 2 3 } 1 1 }  }
	{ BIGcapsule2_V int 512 regular {array 512 { 2 3 } 1 1 }  }
	{ BIGcapsule3_V int 512 regular {array 512 { 2 3 } 1 1 }  }
	{ capsule0_key int 32 regular {array 32 { 2 3 } 1 1 }  }
	{ capsule0_value int 32 regular {array 32 { 2 3 } 1 1 }  }
	{ capsule1_key int 32 regular {array 32 { 2 3 } 1 1 }  }
	{ capsule1_value int 32 regular {array 32 { 2 3 } 1 1 }  }
	{ capsule2_key int 32 regular {array 32 { 2 3 } 1 1 }  }
	{ capsule2_value int 32 regular {array 32 { 2 3 } 1 1 }  }
	{ capsule3_key int 32 regular {array 32 { 2 3 } 1 1 }  }
	{ capsule3_value int 32 regular {array 32 { 2 3 } 1 1 }  }
	{ kvdeststats_tmp_valu int 32 regular {array 32 { 2 3 } 1 1 }  }
	{ offset int 24 regular  }
	{ travstate_i_kvs int 32 regular  }
	{ travstate_end_kvs int 30 regular  }
	{ travstate_skip_kvs int 14 regular  }
	{ capsulemetadata_globaloffset int 32 regular  }
	{ capsulemetadata_localoffset int 32 regular  }
}
set C_modelArgMapList {[ 
	{ "Name" : "kvdram_V", "interface" : "axi_master", "bitwidth" : 512, "direction" : "WRITEONLY"} , 
 	{ "Name" : "kvdram_V_offset", "interface" : "wire", "bitwidth" : 26, "direction" : "READONLY"} , 
 	{ "Name" : "BIGcapsule0_V", "interface" : "memory", "bitwidth" : 512, "direction" : "READWRITE"} , 
 	{ "Name" : "BIGcapsule1_V", "interface" : "memory", "bitwidth" : 512, "direction" : "READWRITE"} , 
 	{ "Name" : "BIGcapsule2_V", "interface" : "memory", "bitwidth" : 512, "direction" : "READWRITE"} , 
 	{ "Name" : "BIGcapsule3_V", "interface" : "memory", "bitwidth" : 512, "direction" : "READWRITE"} , 
 	{ "Name" : "capsule0_key", "interface" : "memory", "bitwidth" : 32, "direction" : "READWRITE"} , 
 	{ "Name" : "capsule0_value", "interface" : "memory", "bitwidth" : 32, "direction" : "READWRITE"} , 
 	{ "Name" : "capsule1_key", "interface" : "memory", "bitwidth" : 32, "direction" : "READWRITE"} , 
 	{ "Name" : "capsule1_value", "interface" : "memory", "bitwidth" : 32, "direction" : "READWRITE"} , 
 	{ "Name" : "capsule2_key", "interface" : "memory", "bitwidth" : 32, "direction" : "READWRITE"} , 
 	{ "Name" : "capsule2_value", "interface" : "memory", "bitwidth" : 32, "direction" : "READWRITE"} , 
 	{ "Name" : "capsule3_key", "interface" : "memory", "bitwidth" : 32, "direction" : "READWRITE"} , 
 	{ "Name" : "capsule3_value", "interface" : "memory", "bitwidth" : 32, "direction" : "READWRITE"} , 
 	{ "Name" : "kvdeststats_tmp_valu", "interface" : "memory", "bitwidth" : 32, "direction" : "READWRITE"} , 
 	{ "Name" : "offset", "interface" : "wire", "bitwidth" : 24, "direction" : "READONLY"} , 
 	{ "Name" : "travstate_i_kvs", "interface" : "wire", "bitwidth" : 32, "direction" : "READONLY"} , 
 	{ "Name" : "travstate_end_kvs", "interface" : "wire", "bitwidth" : 30, "direction" : "READONLY"} , 
 	{ "Name" : "travstate_skip_kvs", "interface" : "wire", "bitwidth" : 14, "direction" : "READONLY"} , 
 	{ "Name" : "capsulemetadata_globaloffset", "interface" : "wire", "bitwidth" : 32, "direction" : "READONLY"} , 
 	{ "Name" : "capsulemetadata_localoffset", "interface" : "wire", "bitwidth" : 32, "direction" : "READONLY"} , 
 	{ "Name" : "ap_return", "interface" : "wire", "bitwidth" : 64} ]}
# RTL Port declarations: 
set portNum 125
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
	{ BIGcapsule0_V_address0 sc_out sc_lv 9 signal 2 } 
	{ BIGcapsule0_V_ce0 sc_out sc_logic 1 signal 2 } 
	{ BIGcapsule0_V_we0 sc_out sc_lv 64 signal 2 } 
	{ BIGcapsule0_V_d0 sc_out sc_lv 512 signal 2 } 
	{ BIGcapsule0_V_q0 sc_in sc_lv 512 signal 2 } 
	{ BIGcapsule1_V_address0 sc_out sc_lv 9 signal 3 } 
	{ BIGcapsule1_V_ce0 sc_out sc_logic 1 signal 3 } 
	{ BIGcapsule1_V_we0 sc_out sc_lv 64 signal 3 } 
	{ BIGcapsule1_V_d0 sc_out sc_lv 512 signal 3 } 
	{ BIGcapsule1_V_q0 sc_in sc_lv 512 signal 3 } 
	{ BIGcapsule2_V_address0 sc_out sc_lv 9 signal 4 } 
	{ BIGcapsule2_V_ce0 sc_out sc_logic 1 signal 4 } 
	{ BIGcapsule2_V_we0 sc_out sc_lv 64 signal 4 } 
	{ BIGcapsule2_V_d0 sc_out sc_lv 512 signal 4 } 
	{ BIGcapsule2_V_q0 sc_in sc_lv 512 signal 4 } 
	{ BIGcapsule3_V_address0 sc_out sc_lv 9 signal 5 } 
	{ BIGcapsule3_V_ce0 sc_out sc_logic 1 signal 5 } 
	{ BIGcapsule3_V_we0 sc_out sc_lv 64 signal 5 } 
	{ BIGcapsule3_V_d0 sc_out sc_lv 512 signal 5 } 
	{ BIGcapsule3_V_q0 sc_in sc_lv 512 signal 5 } 
	{ capsule0_key_address0 sc_out sc_lv 5 signal 6 } 
	{ capsule0_key_ce0 sc_out sc_logic 1 signal 6 } 
	{ capsule0_key_we0 sc_out sc_logic 1 signal 6 } 
	{ capsule0_key_d0 sc_out sc_lv 32 signal 6 } 
	{ capsule0_key_q0 sc_in sc_lv 32 signal 6 } 
	{ capsule0_value_address0 sc_out sc_lv 5 signal 7 } 
	{ capsule0_value_ce0 sc_out sc_logic 1 signal 7 } 
	{ capsule0_value_we0 sc_out sc_logic 1 signal 7 } 
	{ capsule0_value_d0 sc_out sc_lv 32 signal 7 } 
	{ capsule0_value_q0 sc_in sc_lv 32 signal 7 } 
	{ capsule1_key_address0 sc_out sc_lv 5 signal 8 } 
	{ capsule1_key_ce0 sc_out sc_logic 1 signal 8 } 
	{ capsule1_key_we0 sc_out sc_logic 1 signal 8 } 
	{ capsule1_key_d0 sc_out sc_lv 32 signal 8 } 
	{ capsule1_key_q0 sc_in sc_lv 32 signal 8 } 
	{ capsule1_value_address0 sc_out sc_lv 5 signal 9 } 
	{ capsule1_value_ce0 sc_out sc_logic 1 signal 9 } 
	{ capsule1_value_we0 sc_out sc_logic 1 signal 9 } 
	{ capsule1_value_d0 sc_out sc_lv 32 signal 9 } 
	{ capsule1_value_q0 sc_in sc_lv 32 signal 9 } 
	{ capsule2_key_address0 sc_out sc_lv 5 signal 10 } 
	{ capsule2_key_ce0 sc_out sc_logic 1 signal 10 } 
	{ capsule2_key_we0 sc_out sc_logic 1 signal 10 } 
	{ capsule2_key_d0 sc_out sc_lv 32 signal 10 } 
	{ capsule2_key_q0 sc_in sc_lv 32 signal 10 } 
	{ capsule2_value_address0 sc_out sc_lv 5 signal 11 } 
	{ capsule2_value_ce0 sc_out sc_logic 1 signal 11 } 
	{ capsule2_value_we0 sc_out sc_logic 1 signal 11 } 
	{ capsule2_value_d0 sc_out sc_lv 32 signal 11 } 
	{ capsule2_value_q0 sc_in sc_lv 32 signal 11 } 
	{ capsule3_key_address0 sc_out sc_lv 5 signal 12 } 
	{ capsule3_key_ce0 sc_out sc_logic 1 signal 12 } 
	{ capsule3_key_we0 sc_out sc_logic 1 signal 12 } 
	{ capsule3_key_d0 sc_out sc_lv 32 signal 12 } 
	{ capsule3_key_q0 sc_in sc_lv 32 signal 12 } 
	{ capsule3_value_address0 sc_out sc_lv 5 signal 13 } 
	{ capsule3_value_ce0 sc_out sc_logic 1 signal 13 } 
	{ capsule3_value_we0 sc_out sc_logic 1 signal 13 } 
	{ capsule3_value_d0 sc_out sc_lv 32 signal 13 } 
	{ capsule3_value_q0 sc_in sc_lv 32 signal 13 } 
	{ kvdeststats_tmp_valu_address0 sc_out sc_lv 5 signal 14 } 
	{ kvdeststats_tmp_valu_ce0 sc_out sc_logic 1 signal 14 } 
	{ kvdeststats_tmp_valu_we0 sc_out sc_logic 1 signal 14 } 
	{ kvdeststats_tmp_valu_d0 sc_out sc_lv 32 signal 14 } 
	{ kvdeststats_tmp_valu_q0 sc_in sc_lv 32 signal 14 } 
	{ offset sc_in sc_lv 24 signal 15 } 
	{ travstate_i_kvs sc_in sc_lv 32 signal 16 } 
	{ travstate_end_kvs sc_in sc_lv 30 signal 17 } 
	{ travstate_skip_kvs sc_in sc_lv 14 signal 18 } 
	{ capsulemetadata_globaloffset sc_in sc_lv 32 signal 19 } 
	{ capsulemetadata_localoffset sc_in sc_lv 32 signal 20 } 
	{ ap_return_0 sc_out sc_lv 32 signal -1 } 
	{ ap_return_1 sc_out sc_lv 32 signal -1 } 
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
 	{ "name": "BIGcapsule0_V_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":9, "type": "signal", "bundle":{"name": "BIGcapsule0_V", "role": "address0" }} , 
 	{ "name": "BIGcapsule0_V_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "BIGcapsule0_V", "role": "ce0" }} , 
 	{ "name": "BIGcapsule0_V_we0", "direction": "out", "datatype": "sc_lv", "bitwidth":64, "type": "signal", "bundle":{"name": "BIGcapsule0_V", "role": "we0" }} , 
 	{ "name": "BIGcapsule0_V_d0", "direction": "out", "datatype": "sc_lv", "bitwidth":512, "type": "signal", "bundle":{"name": "BIGcapsule0_V", "role": "d0" }} , 
 	{ "name": "BIGcapsule0_V_q0", "direction": "in", "datatype": "sc_lv", "bitwidth":512, "type": "signal", "bundle":{"name": "BIGcapsule0_V", "role": "q0" }} , 
 	{ "name": "BIGcapsule1_V_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":9, "type": "signal", "bundle":{"name": "BIGcapsule1_V", "role": "address0" }} , 
 	{ "name": "BIGcapsule1_V_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "BIGcapsule1_V", "role": "ce0" }} , 
 	{ "name": "BIGcapsule1_V_we0", "direction": "out", "datatype": "sc_lv", "bitwidth":64, "type": "signal", "bundle":{"name": "BIGcapsule1_V", "role": "we0" }} , 
 	{ "name": "BIGcapsule1_V_d0", "direction": "out", "datatype": "sc_lv", "bitwidth":512, "type": "signal", "bundle":{"name": "BIGcapsule1_V", "role": "d0" }} , 
 	{ "name": "BIGcapsule1_V_q0", "direction": "in", "datatype": "sc_lv", "bitwidth":512, "type": "signal", "bundle":{"name": "BIGcapsule1_V", "role": "q0" }} , 
 	{ "name": "BIGcapsule2_V_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":9, "type": "signal", "bundle":{"name": "BIGcapsule2_V", "role": "address0" }} , 
 	{ "name": "BIGcapsule2_V_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "BIGcapsule2_V", "role": "ce0" }} , 
 	{ "name": "BIGcapsule2_V_we0", "direction": "out", "datatype": "sc_lv", "bitwidth":64, "type": "signal", "bundle":{"name": "BIGcapsule2_V", "role": "we0" }} , 
 	{ "name": "BIGcapsule2_V_d0", "direction": "out", "datatype": "sc_lv", "bitwidth":512, "type": "signal", "bundle":{"name": "BIGcapsule2_V", "role": "d0" }} , 
 	{ "name": "BIGcapsule2_V_q0", "direction": "in", "datatype": "sc_lv", "bitwidth":512, "type": "signal", "bundle":{"name": "BIGcapsule2_V", "role": "q0" }} , 
 	{ "name": "BIGcapsule3_V_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":9, "type": "signal", "bundle":{"name": "BIGcapsule3_V", "role": "address0" }} , 
 	{ "name": "BIGcapsule3_V_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "BIGcapsule3_V", "role": "ce0" }} , 
 	{ "name": "BIGcapsule3_V_we0", "direction": "out", "datatype": "sc_lv", "bitwidth":64, "type": "signal", "bundle":{"name": "BIGcapsule3_V", "role": "we0" }} , 
 	{ "name": "BIGcapsule3_V_d0", "direction": "out", "datatype": "sc_lv", "bitwidth":512, "type": "signal", "bundle":{"name": "BIGcapsule3_V", "role": "d0" }} , 
 	{ "name": "BIGcapsule3_V_q0", "direction": "in", "datatype": "sc_lv", "bitwidth":512, "type": "signal", "bundle":{"name": "BIGcapsule3_V", "role": "q0" }} , 
 	{ "name": "capsule0_key_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":5, "type": "signal", "bundle":{"name": "capsule0_key", "role": "address0" }} , 
 	{ "name": "capsule0_key_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "capsule0_key", "role": "ce0" }} , 
 	{ "name": "capsule0_key_we0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "capsule0_key", "role": "we0" }} , 
 	{ "name": "capsule0_key_d0", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "capsule0_key", "role": "d0" }} , 
 	{ "name": "capsule0_key_q0", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "capsule0_key", "role": "q0" }} , 
 	{ "name": "capsule0_value_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":5, "type": "signal", "bundle":{"name": "capsule0_value", "role": "address0" }} , 
 	{ "name": "capsule0_value_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "capsule0_value", "role": "ce0" }} , 
 	{ "name": "capsule0_value_we0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "capsule0_value", "role": "we0" }} , 
 	{ "name": "capsule0_value_d0", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "capsule0_value", "role": "d0" }} , 
 	{ "name": "capsule0_value_q0", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "capsule0_value", "role": "q0" }} , 
 	{ "name": "capsule1_key_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":5, "type": "signal", "bundle":{"name": "capsule1_key", "role": "address0" }} , 
 	{ "name": "capsule1_key_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "capsule1_key", "role": "ce0" }} , 
 	{ "name": "capsule1_key_we0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "capsule1_key", "role": "we0" }} , 
 	{ "name": "capsule1_key_d0", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "capsule1_key", "role": "d0" }} , 
 	{ "name": "capsule1_key_q0", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "capsule1_key", "role": "q0" }} , 
 	{ "name": "capsule1_value_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":5, "type": "signal", "bundle":{"name": "capsule1_value", "role": "address0" }} , 
 	{ "name": "capsule1_value_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "capsule1_value", "role": "ce0" }} , 
 	{ "name": "capsule1_value_we0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "capsule1_value", "role": "we0" }} , 
 	{ "name": "capsule1_value_d0", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "capsule1_value", "role": "d0" }} , 
 	{ "name": "capsule1_value_q0", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "capsule1_value", "role": "q0" }} , 
 	{ "name": "capsule2_key_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":5, "type": "signal", "bundle":{"name": "capsule2_key", "role": "address0" }} , 
 	{ "name": "capsule2_key_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "capsule2_key", "role": "ce0" }} , 
 	{ "name": "capsule2_key_we0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "capsule2_key", "role": "we0" }} , 
 	{ "name": "capsule2_key_d0", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "capsule2_key", "role": "d0" }} , 
 	{ "name": "capsule2_key_q0", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "capsule2_key", "role": "q0" }} , 
 	{ "name": "capsule2_value_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":5, "type": "signal", "bundle":{"name": "capsule2_value", "role": "address0" }} , 
 	{ "name": "capsule2_value_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "capsule2_value", "role": "ce0" }} , 
 	{ "name": "capsule2_value_we0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "capsule2_value", "role": "we0" }} , 
 	{ "name": "capsule2_value_d0", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "capsule2_value", "role": "d0" }} , 
 	{ "name": "capsule2_value_q0", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "capsule2_value", "role": "q0" }} , 
 	{ "name": "capsule3_key_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":5, "type": "signal", "bundle":{"name": "capsule3_key", "role": "address0" }} , 
 	{ "name": "capsule3_key_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "capsule3_key", "role": "ce0" }} , 
 	{ "name": "capsule3_key_we0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "capsule3_key", "role": "we0" }} , 
 	{ "name": "capsule3_key_d0", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "capsule3_key", "role": "d0" }} , 
 	{ "name": "capsule3_key_q0", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "capsule3_key", "role": "q0" }} , 
 	{ "name": "capsule3_value_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":5, "type": "signal", "bundle":{"name": "capsule3_value", "role": "address0" }} , 
 	{ "name": "capsule3_value_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "capsule3_value", "role": "ce0" }} , 
 	{ "name": "capsule3_value_we0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "capsule3_value", "role": "we0" }} , 
 	{ "name": "capsule3_value_d0", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "capsule3_value", "role": "d0" }} , 
 	{ "name": "capsule3_value_q0", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "capsule3_value", "role": "q0" }} , 
 	{ "name": "kvdeststats_tmp_valu_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":5, "type": "signal", "bundle":{"name": "kvdeststats_tmp_valu", "role": "address0" }} , 
 	{ "name": "kvdeststats_tmp_valu_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "kvdeststats_tmp_valu", "role": "ce0" }} , 
 	{ "name": "kvdeststats_tmp_valu_we0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "kvdeststats_tmp_valu", "role": "we0" }} , 
 	{ "name": "kvdeststats_tmp_valu_d0", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "kvdeststats_tmp_valu", "role": "d0" }} , 
 	{ "name": "kvdeststats_tmp_valu_q0", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "kvdeststats_tmp_valu", "role": "q0" }} , 
 	{ "name": "offset", "direction": "in", "datatype": "sc_lv", "bitwidth":24, "type": "signal", "bundle":{"name": "offset", "role": "default" }} , 
 	{ "name": "travstate_i_kvs", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "travstate_i_kvs", "role": "default" }} , 
 	{ "name": "travstate_end_kvs", "direction": "in", "datatype": "sc_lv", "bitwidth":30, "type": "signal", "bundle":{"name": "travstate_end_kvs", "role": "default" }} , 
 	{ "name": "travstate_skip_kvs", "direction": "in", "datatype": "sc_lv", "bitwidth":14, "type": "signal", "bundle":{"name": "travstate_skip_kvs", "role": "default" }} , 
 	{ "name": "capsulemetadata_globaloffset", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "capsulemetadata_globaloffset", "role": "default" }} , 
 	{ "name": "capsulemetadata_localoffset", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "capsulemetadata_localoffset", "role": "default" }} , 
 	{ "name": "ap_return_0", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "ap_return_0", "role": "default" }} , 
 	{ "name": "ap_return_1", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "ap_return_1", "role": "default" }}  ]}

set RtlHierarchyInfo {[
	{"ID" : "0", "Level" : "0", "Path" : "`AUTOTB_DUT_INST", "Parent" : "", "Child" : ["1"],
		"CDFG" : "savecapsules0",
		"Protocol" : "ap_ctrl_hs",
		"ControlExist" : "1", "ap_start" : "1", "ap_ready" : "1", "ap_done" : "1", "ap_continue" : "0", "ap_idle" : "1",
		"Pipeline" : "None", "UnalignedPipeline" : "0", "RewindPipeline" : "0", "ProcessNetwork" : "0",
		"II" : "0",
		"VariableLatency" : "1", "ExactLatency" : "-1", "EstimateLatencyMin" : "358", "EstimateLatencyMax" : "2444",
		"Combinational" : "0",
		"Datapath" : "0",
		"ClockEnable" : "0",
		"HasSubDataflow" : "0",
		"InDataflowNetwork" : "0",
		"HasNonBlockingOperation" : "0",
		"WaitState" : [
			{"State" : "ap_ST_fsm_state15", "FSM" : "ap_CS_fsm", "SubInstance" : "grp_savekeyvalues0_fu_887"},
			{"State" : "ap_ST_fsm_state17", "FSM" : "ap_CS_fsm", "SubInstance" : "grp_savekeyvalues0_fu_887"},
			{"State" : "ap_ST_fsm_state19", "FSM" : "ap_CS_fsm", "SubInstance" : "grp_savekeyvalues0_fu_887"},
			{"State" : "ap_ST_fsm_state21", "FSM" : "ap_CS_fsm", "SubInstance" : "grp_savekeyvalues0_fu_887"}],
		"Port" : [
			{"Name" : "kvdram_V", "Type" : "MAXI", "Direction" : "O",
				"SubConnect" : [
					{"ID" : "1", "SubInstance" : "grp_savekeyvalues0_fu_887", "Port" : "kvdram_V"}]},
			{"Name" : "kvdram_V_offset", "Type" : "None", "Direction" : "I"},
			{"Name" : "BIGcapsule0_V", "Type" : "Memory", "Direction" : "IO",
				"SubConnect" : [
					{"ID" : "1", "SubInstance" : "grp_savekeyvalues0_fu_887", "Port" : "buffer_V"}]},
			{"Name" : "BIGcapsule1_V", "Type" : "Memory", "Direction" : "IO",
				"SubConnect" : [
					{"ID" : "1", "SubInstance" : "grp_savekeyvalues0_fu_887", "Port" : "buffer_V"}]},
			{"Name" : "BIGcapsule2_V", "Type" : "Memory", "Direction" : "IO",
				"SubConnect" : [
					{"ID" : "1", "SubInstance" : "grp_savekeyvalues0_fu_887", "Port" : "buffer_V"}]},
			{"Name" : "BIGcapsule3_V", "Type" : "Memory", "Direction" : "IO",
				"SubConnect" : [
					{"ID" : "1", "SubInstance" : "grp_savekeyvalues0_fu_887", "Port" : "buffer_V"}]},
			{"Name" : "capsule0_key", "Type" : "Memory", "Direction" : "IO"},
			{"Name" : "capsule0_value", "Type" : "Memory", "Direction" : "IO"},
			{"Name" : "capsule1_key", "Type" : "Memory", "Direction" : "IO"},
			{"Name" : "capsule1_value", "Type" : "Memory", "Direction" : "IO"},
			{"Name" : "capsule2_key", "Type" : "Memory", "Direction" : "IO"},
			{"Name" : "capsule2_value", "Type" : "Memory", "Direction" : "IO"},
			{"Name" : "capsule3_key", "Type" : "Memory", "Direction" : "IO"},
			{"Name" : "capsule3_value", "Type" : "Memory", "Direction" : "IO"},
			{"Name" : "kvdeststats_tmp_valu", "Type" : "Memory", "Direction" : "IO"},
			{"Name" : "offset", "Type" : "None", "Direction" : "I"},
			{"Name" : "travstate_i_kvs", "Type" : "None", "Direction" : "I"},
			{"Name" : "travstate_end_kvs", "Type" : "None", "Direction" : "I"},
			{"Name" : "travstate_skip_kvs", "Type" : "None", "Direction" : "I"},
			{"Name" : "capsulemetadata_globaloffset", "Type" : "None", "Direction" : "I"},
			{"Name" : "capsulemetadata_localoffset", "Type" : "None", "Direction" : "I"}]},
	{"ID" : "1", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.grp_savekeyvalues0_fu_887", "Parent" : "0",
		"CDFG" : "savekeyvalues0",
		"Protocol" : "ap_ctrl_hs",
		"ControlExist" : "1", "ap_start" : "1", "ap_ready" : "1", "ap_done" : "1", "ap_continue" : "0", "ap_idle" : "1",
		"Pipeline" : "None", "UnalignedPipeline" : "0", "RewindPipeline" : "0", "ProcessNetwork" : "0",
		"II" : "0",
		"VariableLatency" : "1", "ExactLatency" : "-1", "EstimateLatencyMin" : "520", "EstimateLatencyMax" : "520",
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
			{"Name" : "buffer_V", "Type" : "Memory", "Direction" : "I"},
			{"Name" : "offset_kvs", "Type" : "None", "Direction" : "I"}]}]}


set ArgLastReadFirstWriteLatency {
	savecapsules0 {
		kvdram_V {Type O LastRead 3 FirstWrite 4}
		kvdram_V_offset {Type I LastRead 0 FirstWrite -1}
		BIGcapsule0_V {Type IO LastRead 2 FirstWrite -1}
		BIGcapsule1_V {Type IO LastRead 2 FirstWrite -1}
		BIGcapsule2_V {Type IO LastRead 2 FirstWrite -1}
		BIGcapsule3_V {Type IO LastRead 2 FirstWrite -1}
		capsule0_key {Type IO LastRead 6 FirstWrite 4}
		capsule0_value {Type IO LastRead 5 FirstWrite 6}
		capsule1_key {Type IO LastRead 6 FirstWrite 4}
		capsule1_value {Type IO LastRead 5 FirstWrite 6}
		capsule2_key {Type IO LastRead 6 FirstWrite 4}
		capsule2_value {Type IO LastRead 5 FirstWrite 6}
		capsule3_key {Type IO LastRead 6 FirstWrite 4}
		capsule3_value {Type IO LastRead 5 FirstWrite 6}
		kvdeststats_tmp_valu {Type IO LastRead 4 FirstWrite 2}
		offset {Type I LastRead 0 FirstWrite -1}
		travstate_i_kvs {Type I LastRead 0 FirstWrite -1}
		travstate_end_kvs {Type I LastRead 0 FirstWrite -1}
		travstate_skip_kvs {Type I LastRead 0 FirstWrite -1}
		capsulemetadata_globaloffset {Type I LastRead 0 FirstWrite -1}
		capsulemetadata_localoffset {Type I LastRead 0 FirstWrite -1}}
	savekeyvalues0 {
		kvdram_V {Type O LastRead 3 FirstWrite 4}
		kvdram_V_offset {Type I LastRead 0 FirstWrite -1}
		buffer_V {Type I LastRead 2 FirstWrite -1}
		offset_kvs {Type I LastRead 0 FirstWrite -1}}}

set hasDtUnsupportedChannel 0

set PerformanceInfo {[
	{"Name" : "Latency", "Min" : "358", "Max" : "2444"}
	, {"Name" : "Interval", "Min" : "358", "Max" : "2444"}
]}

set PipelineEnableSignalInfo {[
	{"Pipeline" : "0", "EnableSignal" : "ap_enable_pp0"}
]}

set Spec2ImplPortList { 
	kvdram_V { m_axi {  { m_axi_kvdram_V_AWVALID VALID 1 1 }  { m_axi_kvdram_V_AWREADY READY 0 1 }  { m_axi_kvdram_V_AWADDR ADDR 1 32 }  { m_axi_kvdram_V_AWID ID 1 1 }  { m_axi_kvdram_V_AWLEN LEN 1 32 }  { m_axi_kvdram_V_AWSIZE SIZE 1 3 }  { m_axi_kvdram_V_AWBURST BURST 1 2 }  { m_axi_kvdram_V_AWLOCK LOCK 1 2 }  { m_axi_kvdram_V_AWCACHE CACHE 1 4 }  { m_axi_kvdram_V_AWPROT PROT 1 3 }  { m_axi_kvdram_V_AWQOS QOS 1 4 }  { m_axi_kvdram_V_AWREGION REGION 1 4 }  { m_axi_kvdram_V_AWUSER USER 1 1 }  { m_axi_kvdram_V_WVALID VALID 1 1 }  { m_axi_kvdram_V_WREADY READY 0 1 }  { m_axi_kvdram_V_WDATA DATA 1 512 }  { m_axi_kvdram_V_WSTRB STRB 1 64 }  { m_axi_kvdram_V_WLAST LAST 1 1 }  { m_axi_kvdram_V_WID ID 1 1 }  { m_axi_kvdram_V_WUSER USER 1 1 }  { m_axi_kvdram_V_ARVALID VALID 1 1 }  { m_axi_kvdram_V_ARREADY READY 0 1 }  { m_axi_kvdram_V_ARADDR ADDR 1 32 }  { m_axi_kvdram_V_ARID ID 1 1 }  { m_axi_kvdram_V_ARLEN LEN 1 32 }  { m_axi_kvdram_V_ARSIZE SIZE 1 3 }  { m_axi_kvdram_V_ARBURST BURST 1 2 }  { m_axi_kvdram_V_ARLOCK LOCK 1 2 }  { m_axi_kvdram_V_ARCACHE CACHE 1 4 }  { m_axi_kvdram_V_ARPROT PROT 1 3 }  { m_axi_kvdram_V_ARQOS QOS 1 4 }  { m_axi_kvdram_V_ARREGION REGION 1 4 }  { m_axi_kvdram_V_ARUSER USER 1 1 }  { m_axi_kvdram_V_RVALID VALID 0 1 }  { m_axi_kvdram_V_RREADY READY 1 1 }  { m_axi_kvdram_V_RDATA DATA 0 512 }  { m_axi_kvdram_V_RLAST LAST 0 1 }  { m_axi_kvdram_V_RID ID 0 1 }  { m_axi_kvdram_V_RUSER USER 0 1 }  { m_axi_kvdram_V_RRESP RESP 0 2 }  { m_axi_kvdram_V_BVALID VALID 0 1 }  { m_axi_kvdram_V_BREADY READY 1 1 }  { m_axi_kvdram_V_BRESP RESP 0 2 }  { m_axi_kvdram_V_BID ID 0 1 }  { m_axi_kvdram_V_BUSER USER 0 1 } } }
	kvdram_V_offset { ap_none {  { kvdram_V_offset in_data 0 26 } } }
	BIGcapsule0_V { ap_memory {  { BIGcapsule0_V_address0 mem_address 1 9 }  { BIGcapsule0_V_ce0 mem_ce 1 1 }  { BIGcapsule0_V_we0 mem_we 1 64 }  { BIGcapsule0_V_d0 mem_din 1 512 }  { BIGcapsule0_V_q0 mem_dout 0 512 } } }
	BIGcapsule1_V { ap_memory {  { BIGcapsule1_V_address0 mem_address 1 9 }  { BIGcapsule1_V_ce0 mem_ce 1 1 }  { BIGcapsule1_V_we0 mem_we 1 64 }  { BIGcapsule1_V_d0 mem_din 1 512 }  { BIGcapsule1_V_q0 mem_dout 0 512 } } }
	BIGcapsule2_V { ap_memory {  { BIGcapsule2_V_address0 mem_address 1 9 }  { BIGcapsule2_V_ce0 mem_ce 1 1 }  { BIGcapsule2_V_we0 mem_we 1 64 }  { BIGcapsule2_V_d0 mem_din 1 512 }  { BIGcapsule2_V_q0 mem_dout 0 512 } } }
	BIGcapsule3_V { ap_memory {  { BIGcapsule3_V_address0 mem_address 1 9 }  { BIGcapsule3_V_ce0 mem_ce 1 1 }  { BIGcapsule3_V_we0 mem_we 1 64 }  { BIGcapsule3_V_d0 mem_din 1 512 }  { BIGcapsule3_V_q0 mem_dout 0 512 } } }
	capsule0_key { ap_memory {  { capsule0_key_address0 mem_address 1 5 }  { capsule0_key_ce0 mem_ce 1 1 }  { capsule0_key_we0 mem_we 1 1 }  { capsule0_key_d0 mem_din 1 32 }  { capsule0_key_q0 mem_dout 0 32 } } }
	capsule0_value { ap_memory {  { capsule0_value_address0 mem_address 1 5 }  { capsule0_value_ce0 mem_ce 1 1 }  { capsule0_value_we0 mem_we 1 1 }  { capsule0_value_d0 mem_din 1 32 }  { capsule0_value_q0 mem_dout 0 32 } } }
	capsule1_key { ap_memory {  { capsule1_key_address0 mem_address 1 5 }  { capsule1_key_ce0 mem_ce 1 1 }  { capsule1_key_we0 mem_we 1 1 }  { capsule1_key_d0 mem_din 1 32 }  { capsule1_key_q0 mem_dout 0 32 } } }
	capsule1_value { ap_memory {  { capsule1_value_address0 mem_address 1 5 }  { capsule1_value_ce0 mem_ce 1 1 }  { capsule1_value_we0 mem_we 1 1 }  { capsule1_value_d0 mem_din 1 32 }  { capsule1_value_q0 mem_dout 0 32 } } }
	capsule2_key { ap_memory {  { capsule2_key_address0 mem_address 1 5 }  { capsule2_key_ce0 mem_ce 1 1 }  { capsule2_key_we0 mem_we 1 1 }  { capsule2_key_d0 mem_din 1 32 }  { capsule2_key_q0 mem_dout 0 32 } } }
	capsule2_value { ap_memory {  { capsule2_value_address0 mem_address 1 5 }  { capsule2_value_ce0 mem_ce 1 1 }  { capsule2_value_we0 mem_we 1 1 }  { capsule2_value_d0 mem_din 1 32 }  { capsule2_value_q0 mem_dout 0 32 } } }
	capsule3_key { ap_memory {  { capsule3_key_address0 mem_address 1 5 }  { capsule3_key_ce0 mem_ce 1 1 }  { capsule3_key_we0 mem_we 1 1 }  { capsule3_key_d0 mem_din 1 32 }  { capsule3_key_q0 mem_dout 0 32 } } }
	capsule3_value { ap_memory {  { capsule3_value_address0 mem_address 1 5 }  { capsule3_value_ce0 mem_ce 1 1 }  { capsule3_value_we0 mem_we 1 1 }  { capsule3_value_d0 mem_din 1 32 }  { capsule3_value_q0 mem_dout 0 32 } } }
	kvdeststats_tmp_valu { ap_memory {  { kvdeststats_tmp_valu_address0 mem_address 1 5 }  { kvdeststats_tmp_valu_ce0 mem_ce 1 1 }  { kvdeststats_tmp_valu_we0 mem_we 1 1 }  { kvdeststats_tmp_valu_d0 mem_din 1 32 }  { kvdeststats_tmp_valu_q0 mem_dout 0 32 } } }
	offset { ap_none {  { offset in_data 0 24 } } }
	travstate_i_kvs { ap_none {  { travstate_i_kvs in_data 0 32 } } }
	travstate_end_kvs { ap_none {  { travstate_end_kvs in_data 0 30 } } }
	travstate_skip_kvs { ap_none {  { travstate_skip_kvs in_data 0 14 } } }
	capsulemetadata_globaloffset { ap_none {  { capsulemetadata_globaloffset in_data 0 32 } } }
	capsulemetadata_localoffset { ap_none {  { capsulemetadata_localoffset in_data 0 32 } } }
}
set moduleName savecapsules0
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
set C_modelName {savecapsules0}
set C_modelType { int 64 }
set C_modelArgList {
	{ kvdram_V int 512 regular {axi_master 1}  }
	{ kvdram_V_offset int 26 regular  }
	{ BIGcapsule0_V int 512 regular {array 512 { 2 3 } 1 1 }  }
	{ BIGcapsule1_V int 512 regular {array 512 { 2 3 } 1 1 }  }
	{ BIGcapsule2_V int 512 regular {array 512 { 2 3 } 1 1 }  }
	{ BIGcapsule3_V int 512 regular {array 512 { 2 3 } 1 1 }  }
	{ capsule0_key int 32 regular {array 32 { 2 3 } 1 1 }  }
	{ capsule0_value int 32 regular {array 32 { 2 3 } 1 1 }  }
	{ capsule1_key int 32 regular {array 32 { 2 3 } 1 1 }  }
	{ capsule1_value int 32 regular {array 32 { 2 3 } 1 1 }  }
	{ capsule2_key int 32 regular {array 32 { 2 3 } 1 1 }  }
	{ capsule2_value int 32 regular {array 32 { 2 3 } 1 1 }  }
	{ capsule3_key int 32 regular {array 32 { 2 3 } 1 1 }  }
	{ capsule3_value int 32 regular {array 32 { 2 3 } 1 1 }  }
	{ kvdeststats_tmp_valu int 32 regular {array 32 { 2 3 } 1 1 }  }
	{ offset int 24 regular  }
	{ travstate_i_kvs int 32 regular  }
	{ travstate_end_kvs int 30 regular  }
	{ travstate_skip_kvs int 14 regular  }
	{ capsulemetadata_globaloffset int 32 regular  }
	{ capsulemetadata_localoffset int 32 regular  }
}
set C_modelArgMapList {[ 
	{ "Name" : "kvdram_V", "interface" : "axi_master", "bitwidth" : 512, "direction" : "WRITEONLY"} , 
 	{ "Name" : "kvdram_V_offset", "interface" : "wire", "bitwidth" : 26, "direction" : "READONLY"} , 
 	{ "Name" : "BIGcapsule0_V", "interface" : "memory", "bitwidth" : 512, "direction" : "READWRITE"} , 
 	{ "Name" : "BIGcapsule1_V", "interface" : "memory", "bitwidth" : 512, "direction" : "READWRITE"} , 
 	{ "Name" : "BIGcapsule2_V", "interface" : "memory", "bitwidth" : 512, "direction" : "READWRITE"} , 
 	{ "Name" : "BIGcapsule3_V", "interface" : "memory", "bitwidth" : 512, "direction" : "READWRITE"} , 
 	{ "Name" : "capsule0_key", "interface" : "memory", "bitwidth" : 32, "direction" : "READWRITE"} , 
 	{ "Name" : "capsule0_value", "interface" : "memory", "bitwidth" : 32, "direction" : "READWRITE"} , 
 	{ "Name" : "capsule1_key", "interface" : "memory", "bitwidth" : 32, "direction" : "READWRITE"} , 
 	{ "Name" : "capsule1_value", "interface" : "memory", "bitwidth" : 32, "direction" : "READWRITE"} , 
 	{ "Name" : "capsule2_key", "interface" : "memory", "bitwidth" : 32, "direction" : "READWRITE"} , 
 	{ "Name" : "capsule2_value", "interface" : "memory", "bitwidth" : 32, "direction" : "READWRITE"} , 
 	{ "Name" : "capsule3_key", "interface" : "memory", "bitwidth" : 32, "direction" : "READWRITE"} , 
 	{ "Name" : "capsule3_value", "interface" : "memory", "bitwidth" : 32, "direction" : "READWRITE"} , 
 	{ "Name" : "kvdeststats_tmp_valu", "interface" : "memory", "bitwidth" : 32, "direction" : "READWRITE"} , 
 	{ "Name" : "offset", "interface" : "wire", "bitwidth" : 24, "direction" : "READONLY"} , 
 	{ "Name" : "travstate_i_kvs", "interface" : "wire", "bitwidth" : 32, "direction" : "READONLY"} , 
 	{ "Name" : "travstate_end_kvs", "interface" : "wire", "bitwidth" : 30, "direction" : "READONLY"} , 
 	{ "Name" : "travstate_skip_kvs", "interface" : "wire", "bitwidth" : 14, "direction" : "READONLY"} , 
 	{ "Name" : "capsulemetadata_globaloffset", "interface" : "wire", "bitwidth" : 32, "direction" : "READONLY"} , 
 	{ "Name" : "capsulemetadata_localoffset", "interface" : "wire", "bitwidth" : 32, "direction" : "READONLY"} , 
 	{ "Name" : "ap_return", "interface" : "wire", "bitwidth" : 64} ]}
# RTL Port declarations: 
set portNum 125
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
	{ BIGcapsule0_V_address0 sc_out sc_lv 9 signal 2 } 
	{ BIGcapsule0_V_ce0 sc_out sc_logic 1 signal 2 } 
	{ BIGcapsule0_V_we0 sc_out sc_lv 64 signal 2 } 
	{ BIGcapsule0_V_d0 sc_out sc_lv 512 signal 2 } 
	{ BIGcapsule0_V_q0 sc_in sc_lv 512 signal 2 } 
	{ BIGcapsule1_V_address0 sc_out sc_lv 9 signal 3 } 
	{ BIGcapsule1_V_ce0 sc_out sc_logic 1 signal 3 } 
	{ BIGcapsule1_V_we0 sc_out sc_lv 64 signal 3 } 
	{ BIGcapsule1_V_d0 sc_out sc_lv 512 signal 3 } 
	{ BIGcapsule1_V_q0 sc_in sc_lv 512 signal 3 } 
	{ BIGcapsule2_V_address0 sc_out sc_lv 9 signal 4 } 
	{ BIGcapsule2_V_ce0 sc_out sc_logic 1 signal 4 } 
	{ BIGcapsule2_V_we0 sc_out sc_lv 64 signal 4 } 
	{ BIGcapsule2_V_d0 sc_out sc_lv 512 signal 4 } 
	{ BIGcapsule2_V_q0 sc_in sc_lv 512 signal 4 } 
	{ BIGcapsule3_V_address0 sc_out sc_lv 9 signal 5 } 
	{ BIGcapsule3_V_ce0 sc_out sc_logic 1 signal 5 } 
	{ BIGcapsule3_V_we0 sc_out sc_lv 64 signal 5 } 
	{ BIGcapsule3_V_d0 sc_out sc_lv 512 signal 5 } 
	{ BIGcapsule3_V_q0 sc_in sc_lv 512 signal 5 } 
	{ capsule0_key_address0 sc_out sc_lv 5 signal 6 } 
	{ capsule0_key_ce0 sc_out sc_logic 1 signal 6 } 
	{ capsule0_key_we0 sc_out sc_logic 1 signal 6 } 
	{ capsule0_key_d0 sc_out sc_lv 32 signal 6 } 
	{ capsule0_key_q0 sc_in sc_lv 32 signal 6 } 
	{ capsule0_value_address0 sc_out sc_lv 5 signal 7 } 
	{ capsule0_value_ce0 sc_out sc_logic 1 signal 7 } 
	{ capsule0_value_we0 sc_out sc_logic 1 signal 7 } 
	{ capsule0_value_d0 sc_out sc_lv 32 signal 7 } 
	{ capsule0_value_q0 sc_in sc_lv 32 signal 7 } 
	{ capsule1_key_address0 sc_out sc_lv 5 signal 8 } 
	{ capsule1_key_ce0 sc_out sc_logic 1 signal 8 } 
	{ capsule1_key_we0 sc_out sc_logic 1 signal 8 } 
	{ capsule1_key_d0 sc_out sc_lv 32 signal 8 } 
	{ capsule1_key_q0 sc_in sc_lv 32 signal 8 } 
	{ capsule1_value_address0 sc_out sc_lv 5 signal 9 } 
	{ capsule1_value_ce0 sc_out sc_logic 1 signal 9 } 
	{ capsule1_value_we0 sc_out sc_logic 1 signal 9 } 
	{ capsule1_value_d0 sc_out sc_lv 32 signal 9 } 
	{ capsule1_value_q0 sc_in sc_lv 32 signal 9 } 
	{ capsule2_key_address0 sc_out sc_lv 5 signal 10 } 
	{ capsule2_key_ce0 sc_out sc_logic 1 signal 10 } 
	{ capsule2_key_we0 sc_out sc_logic 1 signal 10 } 
	{ capsule2_key_d0 sc_out sc_lv 32 signal 10 } 
	{ capsule2_key_q0 sc_in sc_lv 32 signal 10 } 
	{ capsule2_value_address0 sc_out sc_lv 5 signal 11 } 
	{ capsule2_value_ce0 sc_out sc_logic 1 signal 11 } 
	{ capsule2_value_we0 sc_out sc_logic 1 signal 11 } 
	{ capsule2_value_d0 sc_out sc_lv 32 signal 11 } 
	{ capsule2_value_q0 sc_in sc_lv 32 signal 11 } 
	{ capsule3_key_address0 sc_out sc_lv 5 signal 12 } 
	{ capsule3_key_ce0 sc_out sc_logic 1 signal 12 } 
	{ capsule3_key_we0 sc_out sc_logic 1 signal 12 } 
	{ capsule3_key_d0 sc_out sc_lv 32 signal 12 } 
	{ capsule3_key_q0 sc_in sc_lv 32 signal 12 } 
	{ capsule3_value_address0 sc_out sc_lv 5 signal 13 } 
	{ capsule3_value_ce0 sc_out sc_logic 1 signal 13 } 
	{ capsule3_value_we0 sc_out sc_logic 1 signal 13 } 
	{ capsule3_value_d0 sc_out sc_lv 32 signal 13 } 
	{ capsule3_value_q0 sc_in sc_lv 32 signal 13 } 
	{ kvdeststats_tmp_valu_address0 sc_out sc_lv 5 signal 14 } 
	{ kvdeststats_tmp_valu_ce0 sc_out sc_logic 1 signal 14 } 
	{ kvdeststats_tmp_valu_we0 sc_out sc_logic 1 signal 14 } 
	{ kvdeststats_tmp_valu_d0 sc_out sc_lv 32 signal 14 } 
	{ kvdeststats_tmp_valu_q0 sc_in sc_lv 32 signal 14 } 
	{ offset sc_in sc_lv 24 signal 15 } 
	{ travstate_i_kvs sc_in sc_lv 32 signal 16 } 
	{ travstate_end_kvs sc_in sc_lv 30 signal 17 } 
	{ travstate_skip_kvs sc_in sc_lv 14 signal 18 } 
	{ capsulemetadata_globaloffset sc_in sc_lv 32 signal 19 } 
	{ capsulemetadata_localoffset sc_in sc_lv 32 signal 20 } 
	{ ap_return_0 sc_out sc_lv 32 signal -1 } 
	{ ap_return_1 sc_out sc_lv 32 signal -1 } 
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
 	{ "name": "BIGcapsule0_V_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":9, "type": "signal", "bundle":{"name": "BIGcapsule0_V", "role": "address0" }} , 
 	{ "name": "BIGcapsule0_V_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "BIGcapsule0_V", "role": "ce0" }} , 
 	{ "name": "BIGcapsule0_V_we0", "direction": "out", "datatype": "sc_lv", "bitwidth":64, "type": "signal", "bundle":{"name": "BIGcapsule0_V", "role": "we0" }} , 
 	{ "name": "BIGcapsule0_V_d0", "direction": "out", "datatype": "sc_lv", "bitwidth":512, "type": "signal", "bundle":{"name": "BIGcapsule0_V", "role": "d0" }} , 
 	{ "name": "BIGcapsule0_V_q0", "direction": "in", "datatype": "sc_lv", "bitwidth":512, "type": "signal", "bundle":{"name": "BIGcapsule0_V", "role": "q0" }} , 
 	{ "name": "BIGcapsule1_V_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":9, "type": "signal", "bundle":{"name": "BIGcapsule1_V", "role": "address0" }} , 
 	{ "name": "BIGcapsule1_V_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "BIGcapsule1_V", "role": "ce0" }} , 
 	{ "name": "BIGcapsule1_V_we0", "direction": "out", "datatype": "sc_lv", "bitwidth":64, "type": "signal", "bundle":{"name": "BIGcapsule1_V", "role": "we0" }} , 
 	{ "name": "BIGcapsule1_V_d0", "direction": "out", "datatype": "sc_lv", "bitwidth":512, "type": "signal", "bundle":{"name": "BIGcapsule1_V", "role": "d0" }} , 
 	{ "name": "BIGcapsule1_V_q0", "direction": "in", "datatype": "sc_lv", "bitwidth":512, "type": "signal", "bundle":{"name": "BIGcapsule1_V", "role": "q0" }} , 
 	{ "name": "BIGcapsule2_V_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":9, "type": "signal", "bundle":{"name": "BIGcapsule2_V", "role": "address0" }} , 
 	{ "name": "BIGcapsule2_V_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "BIGcapsule2_V", "role": "ce0" }} , 
 	{ "name": "BIGcapsule2_V_we0", "direction": "out", "datatype": "sc_lv", "bitwidth":64, "type": "signal", "bundle":{"name": "BIGcapsule2_V", "role": "we0" }} , 
 	{ "name": "BIGcapsule2_V_d0", "direction": "out", "datatype": "sc_lv", "bitwidth":512, "type": "signal", "bundle":{"name": "BIGcapsule2_V", "role": "d0" }} , 
 	{ "name": "BIGcapsule2_V_q0", "direction": "in", "datatype": "sc_lv", "bitwidth":512, "type": "signal", "bundle":{"name": "BIGcapsule2_V", "role": "q0" }} , 
 	{ "name": "BIGcapsule3_V_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":9, "type": "signal", "bundle":{"name": "BIGcapsule3_V", "role": "address0" }} , 
 	{ "name": "BIGcapsule3_V_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "BIGcapsule3_V", "role": "ce0" }} , 
 	{ "name": "BIGcapsule3_V_we0", "direction": "out", "datatype": "sc_lv", "bitwidth":64, "type": "signal", "bundle":{"name": "BIGcapsule3_V", "role": "we0" }} , 
 	{ "name": "BIGcapsule3_V_d0", "direction": "out", "datatype": "sc_lv", "bitwidth":512, "type": "signal", "bundle":{"name": "BIGcapsule3_V", "role": "d0" }} , 
 	{ "name": "BIGcapsule3_V_q0", "direction": "in", "datatype": "sc_lv", "bitwidth":512, "type": "signal", "bundle":{"name": "BIGcapsule3_V", "role": "q0" }} , 
 	{ "name": "capsule0_key_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":5, "type": "signal", "bundle":{"name": "capsule0_key", "role": "address0" }} , 
 	{ "name": "capsule0_key_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "capsule0_key", "role": "ce0" }} , 
 	{ "name": "capsule0_key_we0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "capsule0_key", "role": "we0" }} , 
 	{ "name": "capsule0_key_d0", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "capsule0_key", "role": "d0" }} , 
 	{ "name": "capsule0_key_q0", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "capsule0_key", "role": "q0" }} , 
 	{ "name": "capsule0_value_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":5, "type": "signal", "bundle":{"name": "capsule0_value", "role": "address0" }} , 
 	{ "name": "capsule0_value_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "capsule0_value", "role": "ce0" }} , 
 	{ "name": "capsule0_value_we0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "capsule0_value", "role": "we0" }} , 
 	{ "name": "capsule0_value_d0", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "capsule0_value", "role": "d0" }} , 
 	{ "name": "capsule0_value_q0", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "capsule0_value", "role": "q0" }} , 
 	{ "name": "capsule1_key_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":5, "type": "signal", "bundle":{"name": "capsule1_key", "role": "address0" }} , 
 	{ "name": "capsule1_key_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "capsule1_key", "role": "ce0" }} , 
 	{ "name": "capsule1_key_we0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "capsule1_key", "role": "we0" }} , 
 	{ "name": "capsule1_key_d0", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "capsule1_key", "role": "d0" }} , 
 	{ "name": "capsule1_key_q0", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "capsule1_key", "role": "q0" }} , 
 	{ "name": "capsule1_value_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":5, "type": "signal", "bundle":{"name": "capsule1_value", "role": "address0" }} , 
 	{ "name": "capsule1_value_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "capsule1_value", "role": "ce0" }} , 
 	{ "name": "capsule1_value_we0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "capsule1_value", "role": "we0" }} , 
 	{ "name": "capsule1_value_d0", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "capsule1_value", "role": "d0" }} , 
 	{ "name": "capsule1_value_q0", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "capsule1_value", "role": "q0" }} , 
 	{ "name": "capsule2_key_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":5, "type": "signal", "bundle":{"name": "capsule2_key", "role": "address0" }} , 
 	{ "name": "capsule2_key_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "capsule2_key", "role": "ce0" }} , 
 	{ "name": "capsule2_key_we0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "capsule2_key", "role": "we0" }} , 
 	{ "name": "capsule2_key_d0", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "capsule2_key", "role": "d0" }} , 
 	{ "name": "capsule2_key_q0", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "capsule2_key", "role": "q0" }} , 
 	{ "name": "capsule2_value_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":5, "type": "signal", "bundle":{"name": "capsule2_value", "role": "address0" }} , 
 	{ "name": "capsule2_value_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "capsule2_value", "role": "ce0" }} , 
 	{ "name": "capsule2_value_we0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "capsule2_value", "role": "we0" }} , 
 	{ "name": "capsule2_value_d0", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "capsule2_value", "role": "d0" }} , 
 	{ "name": "capsule2_value_q0", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "capsule2_value", "role": "q0" }} , 
 	{ "name": "capsule3_key_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":5, "type": "signal", "bundle":{"name": "capsule3_key", "role": "address0" }} , 
 	{ "name": "capsule3_key_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "capsule3_key", "role": "ce0" }} , 
 	{ "name": "capsule3_key_we0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "capsule3_key", "role": "we0" }} , 
 	{ "name": "capsule3_key_d0", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "capsule3_key", "role": "d0" }} , 
 	{ "name": "capsule3_key_q0", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "capsule3_key", "role": "q0" }} , 
 	{ "name": "capsule3_value_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":5, "type": "signal", "bundle":{"name": "capsule3_value", "role": "address0" }} , 
 	{ "name": "capsule3_value_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "capsule3_value", "role": "ce0" }} , 
 	{ "name": "capsule3_value_we0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "capsule3_value", "role": "we0" }} , 
 	{ "name": "capsule3_value_d0", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "capsule3_value", "role": "d0" }} , 
 	{ "name": "capsule3_value_q0", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "capsule3_value", "role": "q0" }} , 
 	{ "name": "kvdeststats_tmp_valu_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":5, "type": "signal", "bundle":{"name": "kvdeststats_tmp_valu", "role": "address0" }} , 
 	{ "name": "kvdeststats_tmp_valu_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "kvdeststats_tmp_valu", "role": "ce0" }} , 
 	{ "name": "kvdeststats_tmp_valu_we0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "kvdeststats_tmp_valu", "role": "we0" }} , 
 	{ "name": "kvdeststats_tmp_valu_d0", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "kvdeststats_tmp_valu", "role": "d0" }} , 
 	{ "name": "kvdeststats_tmp_valu_q0", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "kvdeststats_tmp_valu", "role": "q0" }} , 
 	{ "name": "offset", "direction": "in", "datatype": "sc_lv", "bitwidth":24, "type": "signal", "bundle":{"name": "offset", "role": "default" }} , 
 	{ "name": "travstate_i_kvs", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "travstate_i_kvs", "role": "default" }} , 
 	{ "name": "travstate_end_kvs", "direction": "in", "datatype": "sc_lv", "bitwidth":30, "type": "signal", "bundle":{"name": "travstate_end_kvs", "role": "default" }} , 
 	{ "name": "travstate_skip_kvs", "direction": "in", "datatype": "sc_lv", "bitwidth":14, "type": "signal", "bundle":{"name": "travstate_skip_kvs", "role": "default" }} , 
 	{ "name": "capsulemetadata_globaloffset", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "capsulemetadata_globaloffset", "role": "default" }} , 
 	{ "name": "capsulemetadata_localoffset", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "capsulemetadata_localoffset", "role": "default" }} , 
 	{ "name": "ap_return_0", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "ap_return_0", "role": "default" }} , 
 	{ "name": "ap_return_1", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "ap_return_1", "role": "default" }}  ]}

set RtlHierarchyInfo {[
	{"ID" : "0", "Level" : "0", "Path" : "`AUTOTB_DUT_INST", "Parent" : "", "Child" : ["1"],
		"CDFG" : "savecapsules0",
		"Protocol" : "ap_ctrl_hs",
		"ControlExist" : "1", "ap_start" : "1", "ap_ready" : "1", "ap_done" : "1", "ap_continue" : "0", "ap_idle" : "1",
		"Pipeline" : "None", "UnalignedPipeline" : "0", "RewindPipeline" : "0", "ProcessNetwork" : "0",
		"II" : "0",
		"VariableLatency" : "1", "ExactLatency" : "-1", "EstimateLatencyMin" : "358", "EstimateLatencyMax" : "2444",
		"Combinational" : "0",
		"Datapath" : "0",
		"ClockEnable" : "0",
		"HasSubDataflow" : "0",
		"InDataflowNetwork" : "0",
		"HasNonBlockingOperation" : "0",
		"WaitState" : [
			{"State" : "ap_ST_fsm_state15", "FSM" : "ap_CS_fsm", "SubInstance" : "grp_savekeyvalues0_fu_887"},
			{"State" : "ap_ST_fsm_state17", "FSM" : "ap_CS_fsm", "SubInstance" : "grp_savekeyvalues0_fu_887"},
			{"State" : "ap_ST_fsm_state19", "FSM" : "ap_CS_fsm", "SubInstance" : "grp_savekeyvalues0_fu_887"},
			{"State" : "ap_ST_fsm_state21", "FSM" : "ap_CS_fsm", "SubInstance" : "grp_savekeyvalues0_fu_887"}],
		"Port" : [
			{"Name" : "kvdram_V", "Type" : "MAXI", "Direction" : "O",
				"SubConnect" : [
					{"ID" : "1", "SubInstance" : "grp_savekeyvalues0_fu_887", "Port" : "kvdram_V"}]},
			{"Name" : "kvdram_V_offset", "Type" : "None", "Direction" : "I"},
			{"Name" : "BIGcapsule0_V", "Type" : "Memory", "Direction" : "IO",
				"SubConnect" : [
					{"ID" : "1", "SubInstance" : "grp_savekeyvalues0_fu_887", "Port" : "buffer_V"}]},
			{"Name" : "BIGcapsule1_V", "Type" : "Memory", "Direction" : "IO",
				"SubConnect" : [
					{"ID" : "1", "SubInstance" : "grp_savekeyvalues0_fu_887", "Port" : "buffer_V"}]},
			{"Name" : "BIGcapsule2_V", "Type" : "Memory", "Direction" : "IO",
				"SubConnect" : [
					{"ID" : "1", "SubInstance" : "grp_savekeyvalues0_fu_887", "Port" : "buffer_V"}]},
			{"Name" : "BIGcapsule3_V", "Type" : "Memory", "Direction" : "IO",
				"SubConnect" : [
					{"ID" : "1", "SubInstance" : "grp_savekeyvalues0_fu_887", "Port" : "buffer_V"}]},
			{"Name" : "capsule0_key", "Type" : "Memory", "Direction" : "IO"},
			{"Name" : "capsule0_value", "Type" : "Memory", "Direction" : "IO"},
			{"Name" : "capsule1_key", "Type" : "Memory", "Direction" : "IO"},
			{"Name" : "capsule1_value", "Type" : "Memory", "Direction" : "IO"},
			{"Name" : "capsule2_key", "Type" : "Memory", "Direction" : "IO"},
			{"Name" : "capsule2_value", "Type" : "Memory", "Direction" : "IO"},
			{"Name" : "capsule3_key", "Type" : "Memory", "Direction" : "IO"},
			{"Name" : "capsule3_value", "Type" : "Memory", "Direction" : "IO"},
			{"Name" : "kvdeststats_tmp_valu", "Type" : "Memory", "Direction" : "IO"},
			{"Name" : "offset", "Type" : "None", "Direction" : "I"},
			{"Name" : "travstate_i_kvs", "Type" : "None", "Direction" : "I"},
			{"Name" : "travstate_end_kvs", "Type" : "None", "Direction" : "I"},
			{"Name" : "travstate_skip_kvs", "Type" : "None", "Direction" : "I"},
			{"Name" : "capsulemetadata_globaloffset", "Type" : "None", "Direction" : "I"},
			{"Name" : "capsulemetadata_localoffset", "Type" : "None", "Direction" : "I"}]},
	{"ID" : "1", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.grp_savekeyvalues0_fu_887", "Parent" : "0",
		"CDFG" : "savekeyvalues0",
		"Protocol" : "ap_ctrl_hs",
		"ControlExist" : "1", "ap_start" : "1", "ap_ready" : "1", "ap_done" : "1", "ap_continue" : "0", "ap_idle" : "1",
		"Pipeline" : "None", "UnalignedPipeline" : "0", "RewindPipeline" : "0", "ProcessNetwork" : "0",
		"II" : "0",
		"VariableLatency" : "1", "ExactLatency" : "-1", "EstimateLatencyMin" : "520", "EstimateLatencyMax" : "520",
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
			{"Name" : "buffer_V", "Type" : "Memory", "Direction" : "I"},
			{"Name" : "offset_kvs", "Type" : "None", "Direction" : "I"}]}]}


set ArgLastReadFirstWriteLatency {
	savecapsules0 {
		kvdram_V {Type O LastRead 3 FirstWrite 4}
		kvdram_V_offset {Type I LastRead 0 FirstWrite -1}
		BIGcapsule0_V {Type IO LastRead 2 FirstWrite -1}
		BIGcapsule1_V {Type IO LastRead 2 FirstWrite -1}
		BIGcapsule2_V {Type IO LastRead 2 FirstWrite -1}
		BIGcapsule3_V {Type IO LastRead 2 FirstWrite -1}
		capsule0_key {Type IO LastRead 6 FirstWrite 4}
		capsule0_value {Type IO LastRead 5 FirstWrite 6}
		capsule1_key {Type IO LastRead 6 FirstWrite 4}
		capsule1_value {Type IO LastRead 5 FirstWrite 6}
		capsule2_key {Type IO LastRead 6 FirstWrite 4}
		capsule2_value {Type IO LastRead 5 FirstWrite 6}
		capsule3_key {Type IO LastRead 6 FirstWrite 4}
		capsule3_value {Type IO LastRead 5 FirstWrite 6}
		kvdeststats_tmp_valu {Type IO LastRead 4 FirstWrite 2}
		offset {Type I LastRead 0 FirstWrite -1}
		travstate_i_kvs {Type I LastRead 0 FirstWrite -1}
		travstate_end_kvs {Type I LastRead 0 FirstWrite -1}
		travstate_skip_kvs {Type I LastRead 0 FirstWrite -1}
		capsulemetadata_globaloffset {Type I LastRead 0 FirstWrite -1}
		capsulemetadata_localoffset {Type I LastRead 0 FirstWrite -1}}
	savekeyvalues0 {
		kvdram_V {Type O LastRead 3 FirstWrite 4}
		kvdram_V_offset {Type I LastRead 0 FirstWrite -1}
		buffer_V {Type I LastRead 2 FirstWrite -1}
		offset_kvs {Type I LastRead 0 FirstWrite -1}}}

set hasDtUnsupportedChannel 0

set PerformanceInfo {[
	{"Name" : "Latency", "Min" : "358", "Max" : "2444"}
	, {"Name" : "Interval", "Min" : "358", "Max" : "2444"}
]}

set PipelineEnableSignalInfo {[
	{"Pipeline" : "0", "EnableSignal" : "ap_enable_pp0"}
]}

set Spec2ImplPortList { 
	kvdram_V { m_axi {  { m_axi_kvdram_V_AWVALID VALID 1 1 }  { m_axi_kvdram_V_AWREADY READY 0 1 }  { m_axi_kvdram_V_AWADDR ADDR 1 32 }  { m_axi_kvdram_V_AWID ID 1 1 }  { m_axi_kvdram_V_AWLEN LEN 1 32 }  { m_axi_kvdram_V_AWSIZE SIZE 1 3 }  { m_axi_kvdram_V_AWBURST BURST 1 2 }  { m_axi_kvdram_V_AWLOCK LOCK 1 2 }  { m_axi_kvdram_V_AWCACHE CACHE 1 4 }  { m_axi_kvdram_V_AWPROT PROT 1 3 }  { m_axi_kvdram_V_AWQOS QOS 1 4 }  { m_axi_kvdram_V_AWREGION REGION 1 4 }  { m_axi_kvdram_V_AWUSER USER 1 1 }  { m_axi_kvdram_V_WVALID VALID 1 1 }  { m_axi_kvdram_V_WREADY READY 0 1 }  { m_axi_kvdram_V_WDATA DATA 1 512 }  { m_axi_kvdram_V_WSTRB STRB 1 64 }  { m_axi_kvdram_V_WLAST LAST 1 1 }  { m_axi_kvdram_V_WID ID 1 1 }  { m_axi_kvdram_V_WUSER USER 1 1 }  { m_axi_kvdram_V_ARVALID VALID 1 1 }  { m_axi_kvdram_V_ARREADY READY 0 1 }  { m_axi_kvdram_V_ARADDR ADDR 1 32 }  { m_axi_kvdram_V_ARID ID 1 1 }  { m_axi_kvdram_V_ARLEN LEN 1 32 }  { m_axi_kvdram_V_ARSIZE SIZE 1 3 }  { m_axi_kvdram_V_ARBURST BURST 1 2 }  { m_axi_kvdram_V_ARLOCK LOCK 1 2 }  { m_axi_kvdram_V_ARCACHE CACHE 1 4 }  { m_axi_kvdram_V_ARPROT PROT 1 3 }  { m_axi_kvdram_V_ARQOS QOS 1 4 }  { m_axi_kvdram_V_ARREGION REGION 1 4 }  { m_axi_kvdram_V_ARUSER USER 1 1 }  { m_axi_kvdram_V_RVALID VALID 0 1 }  { m_axi_kvdram_V_RREADY READY 1 1 }  { m_axi_kvdram_V_RDATA DATA 0 512 }  { m_axi_kvdram_V_RLAST LAST 0 1 }  { m_axi_kvdram_V_RID ID 0 1 }  { m_axi_kvdram_V_RUSER USER 0 1 }  { m_axi_kvdram_V_RRESP RESP 0 2 }  { m_axi_kvdram_V_BVALID VALID 0 1 }  { m_axi_kvdram_V_BREADY READY 1 1 }  { m_axi_kvdram_V_BRESP RESP 0 2 }  { m_axi_kvdram_V_BID ID 0 1 }  { m_axi_kvdram_V_BUSER USER 0 1 } } }
	kvdram_V_offset { ap_none {  { kvdram_V_offset in_data 0 26 } } }
	BIGcapsule0_V { ap_memory {  { BIGcapsule0_V_address0 mem_address 1 9 }  { BIGcapsule0_V_ce0 mem_ce 1 1 }  { BIGcapsule0_V_we0 mem_we 1 64 }  { BIGcapsule0_V_d0 mem_din 1 512 }  { BIGcapsule0_V_q0 mem_dout 0 512 } } }
	BIGcapsule1_V { ap_memory {  { BIGcapsule1_V_address0 mem_address 1 9 }  { BIGcapsule1_V_ce0 mem_ce 1 1 }  { BIGcapsule1_V_we0 mem_we 1 64 }  { BIGcapsule1_V_d0 mem_din 1 512 }  { BIGcapsule1_V_q0 mem_dout 0 512 } } }
	BIGcapsule2_V { ap_memory {  { BIGcapsule2_V_address0 mem_address 1 9 }  { BIGcapsule2_V_ce0 mem_ce 1 1 }  { BIGcapsule2_V_we0 mem_we 1 64 }  { BIGcapsule2_V_d0 mem_din 1 512 }  { BIGcapsule2_V_q0 mem_dout 0 512 } } }
	BIGcapsule3_V { ap_memory {  { BIGcapsule3_V_address0 mem_address 1 9 }  { BIGcapsule3_V_ce0 mem_ce 1 1 }  { BIGcapsule3_V_we0 mem_we 1 64 }  { BIGcapsule3_V_d0 mem_din 1 512 }  { BIGcapsule3_V_q0 mem_dout 0 512 } } }
	capsule0_key { ap_memory {  { capsule0_key_address0 mem_address 1 5 }  { capsule0_key_ce0 mem_ce 1 1 }  { capsule0_key_we0 mem_we 1 1 }  { capsule0_key_d0 mem_din 1 32 }  { capsule0_key_q0 mem_dout 0 32 } } }
	capsule0_value { ap_memory {  { capsule0_value_address0 mem_address 1 5 }  { capsule0_value_ce0 mem_ce 1 1 }  { capsule0_value_we0 mem_we 1 1 }  { capsule0_value_d0 mem_din 1 32 }  { capsule0_value_q0 mem_dout 0 32 } } }
	capsule1_key { ap_memory {  { capsule1_key_address0 mem_address 1 5 }  { capsule1_key_ce0 mem_ce 1 1 }  { capsule1_key_we0 mem_we 1 1 }  { capsule1_key_d0 mem_din 1 32 }  { capsule1_key_q0 mem_dout 0 32 } } }
	capsule1_value { ap_memory {  { capsule1_value_address0 mem_address 1 5 }  { capsule1_value_ce0 mem_ce 1 1 }  { capsule1_value_we0 mem_we 1 1 }  { capsule1_value_d0 mem_din 1 32 }  { capsule1_value_q0 mem_dout 0 32 } } }
	capsule2_key { ap_memory {  { capsule2_key_address0 mem_address 1 5 }  { capsule2_key_ce0 mem_ce 1 1 }  { capsule2_key_we0 mem_we 1 1 }  { capsule2_key_d0 mem_din 1 32 }  { capsule2_key_q0 mem_dout 0 32 } } }
	capsule2_value { ap_memory {  { capsule2_value_address0 mem_address 1 5 }  { capsule2_value_ce0 mem_ce 1 1 }  { capsule2_value_we0 mem_we 1 1 }  { capsule2_value_d0 mem_din 1 32 }  { capsule2_value_q0 mem_dout 0 32 } } }
	capsule3_key { ap_memory {  { capsule3_key_address0 mem_address 1 5 }  { capsule3_key_ce0 mem_ce 1 1 }  { capsule3_key_we0 mem_we 1 1 }  { capsule3_key_d0 mem_din 1 32 }  { capsule3_key_q0 mem_dout 0 32 } } }
	capsule3_value { ap_memory {  { capsule3_value_address0 mem_address 1 5 }  { capsule3_value_ce0 mem_ce 1 1 }  { capsule3_value_we0 mem_we 1 1 }  { capsule3_value_d0 mem_din 1 32 }  { capsule3_value_q0 mem_dout 0 32 } } }
	kvdeststats_tmp_valu { ap_memory {  { kvdeststats_tmp_valu_address0 mem_address 1 5 }  { kvdeststats_tmp_valu_ce0 mem_ce 1 1 }  { kvdeststats_tmp_valu_we0 mem_we 1 1 }  { kvdeststats_tmp_valu_d0 mem_din 1 32 }  { kvdeststats_tmp_valu_q0 mem_dout 0 32 } } }
	offset { ap_none {  { offset in_data 0 24 } } }
	travstate_i_kvs { ap_none {  { travstate_i_kvs in_data 0 32 } } }
	travstate_end_kvs { ap_none {  { travstate_end_kvs in_data 0 30 } } }
	travstate_skip_kvs { ap_none {  { travstate_skip_kvs in_data 0 14 } } }
	capsulemetadata_globaloffset { ap_none {  { capsulemetadata_globaloffset in_data 0 32 } } }
	capsulemetadata_localoffset { ap_none {  { capsulemetadata_localoffset in_data 0 32 } } }
}
set moduleName savecapsules0
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
set C_modelName {savecapsules0}
set C_modelType { int 64 }
set C_modelArgList {
	{ kvdram_V int 512 regular {axi_master 1}  }
	{ kvdram_V_offset int 26 regular  }
	{ BIGcapsule0_V int 512 regular {array 512 { 2 3 } 1 1 }  }
	{ BIGcapsule1_V int 512 regular {array 512 { 2 3 } 1 1 }  }
	{ BIGcapsule2_V int 512 regular {array 512 { 2 3 } 1 1 }  }
	{ BIGcapsule3_V int 512 regular {array 512 { 2 3 } 1 1 }  }
	{ capsule0_key int 32 regular {array 32 { 2 3 } 1 1 }  }
	{ capsule0_value int 32 regular {array 32 { 2 3 } 1 1 }  }
	{ capsule1_key int 32 regular {array 32 { 2 3 } 1 1 }  }
	{ capsule1_value int 32 regular {array 32 { 2 3 } 1 1 }  }
	{ capsule2_key int 32 regular {array 32 { 2 3 } 1 1 }  }
	{ capsule2_value int 32 regular {array 32 { 2 3 } 1 1 }  }
	{ capsule3_key int 32 regular {array 32 { 2 3 } 1 1 }  }
	{ capsule3_value int 32 regular {array 32 { 2 3 } 1 1 }  }
	{ kvdeststats_tmp_valu int 32 regular {array 32 { 2 3 } 1 1 }  }
	{ offset int 24 regular  }
	{ travstate_i_kvs int 32 regular  }
	{ travstate_end_kvs int 30 regular  }
	{ travstate_skip_kvs int 14 regular  }
	{ capsulemetadata_globaloffset int 32 regular  }
	{ capsulemetadata_localoffset int 32 regular  }
}
set C_modelArgMapList {[ 
	{ "Name" : "kvdram_V", "interface" : "axi_master", "bitwidth" : 512, "direction" : "WRITEONLY"} , 
 	{ "Name" : "kvdram_V_offset", "interface" : "wire", "bitwidth" : 26, "direction" : "READONLY"} , 
 	{ "Name" : "BIGcapsule0_V", "interface" : "memory", "bitwidth" : 512, "direction" : "READWRITE"} , 
 	{ "Name" : "BIGcapsule1_V", "interface" : "memory", "bitwidth" : 512, "direction" : "READWRITE"} , 
 	{ "Name" : "BIGcapsule2_V", "interface" : "memory", "bitwidth" : 512, "direction" : "READWRITE"} , 
 	{ "Name" : "BIGcapsule3_V", "interface" : "memory", "bitwidth" : 512, "direction" : "READWRITE"} , 
 	{ "Name" : "capsule0_key", "interface" : "memory", "bitwidth" : 32, "direction" : "READWRITE"} , 
 	{ "Name" : "capsule0_value", "interface" : "memory", "bitwidth" : 32, "direction" : "READWRITE"} , 
 	{ "Name" : "capsule1_key", "interface" : "memory", "bitwidth" : 32, "direction" : "READWRITE"} , 
 	{ "Name" : "capsule1_value", "interface" : "memory", "bitwidth" : 32, "direction" : "READWRITE"} , 
 	{ "Name" : "capsule2_key", "interface" : "memory", "bitwidth" : 32, "direction" : "READWRITE"} , 
 	{ "Name" : "capsule2_value", "interface" : "memory", "bitwidth" : 32, "direction" : "READWRITE"} , 
 	{ "Name" : "capsule3_key", "interface" : "memory", "bitwidth" : 32, "direction" : "READWRITE"} , 
 	{ "Name" : "capsule3_value", "interface" : "memory", "bitwidth" : 32, "direction" : "READWRITE"} , 
 	{ "Name" : "kvdeststats_tmp_valu", "interface" : "memory", "bitwidth" : 32, "direction" : "READWRITE"} , 
 	{ "Name" : "offset", "interface" : "wire", "bitwidth" : 24, "direction" : "READONLY"} , 
 	{ "Name" : "travstate_i_kvs", "interface" : "wire", "bitwidth" : 32, "direction" : "READONLY"} , 
 	{ "Name" : "travstate_end_kvs", "interface" : "wire", "bitwidth" : 30, "direction" : "READONLY"} , 
 	{ "Name" : "travstate_skip_kvs", "interface" : "wire", "bitwidth" : 14, "direction" : "READONLY"} , 
 	{ "Name" : "capsulemetadata_globaloffset", "interface" : "wire", "bitwidth" : 32, "direction" : "READONLY"} , 
 	{ "Name" : "capsulemetadata_localoffset", "interface" : "wire", "bitwidth" : 32, "direction" : "READONLY"} , 
 	{ "Name" : "ap_return", "interface" : "wire", "bitwidth" : 64} ]}
# RTL Port declarations: 
set portNum 125
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
	{ BIGcapsule0_V_address0 sc_out sc_lv 9 signal 2 } 
	{ BIGcapsule0_V_ce0 sc_out sc_logic 1 signal 2 } 
	{ BIGcapsule0_V_we0 sc_out sc_lv 64 signal 2 } 
	{ BIGcapsule0_V_d0 sc_out sc_lv 512 signal 2 } 
	{ BIGcapsule0_V_q0 sc_in sc_lv 512 signal 2 } 
	{ BIGcapsule1_V_address0 sc_out sc_lv 9 signal 3 } 
	{ BIGcapsule1_V_ce0 sc_out sc_logic 1 signal 3 } 
	{ BIGcapsule1_V_we0 sc_out sc_lv 64 signal 3 } 
	{ BIGcapsule1_V_d0 sc_out sc_lv 512 signal 3 } 
	{ BIGcapsule1_V_q0 sc_in sc_lv 512 signal 3 } 
	{ BIGcapsule2_V_address0 sc_out sc_lv 9 signal 4 } 
	{ BIGcapsule2_V_ce0 sc_out sc_logic 1 signal 4 } 
	{ BIGcapsule2_V_we0 sc_out sc_lv 64 signal 4 } 
	{ BIGcapsule2_V_d0 sc_out sc_lv 512 signal 4 } 
	{ BIGcapsule2_V_q0 sc_in sc_lv 512 signal 4 } 
	{ BIGcapsule3_V_address0 sc_out sc_lv 9 signal 5 } 
	{ BIGcapsule3_V_ce0 sc_out sc_logic 1 signal 5 } 
	{ BIGcapsule3_V_we0 sc_out sc_lv 64 signal 5 } 
	{ BIGcapsule3_V_d0 sc_out sc_lv 512 signal 5 } 
	{ BIGcapsule3_V_q0 sc_in sc_lv 512 signal 5 } 
	{ capsule0_key_address0 sc_out sc_lv 5 signal 6 } 
	{ capsule0_key_ce0 sc_out sc_logic 1 signal 6 } 
	{ capsule0_key_we0 sc_out sc_logic 1 signal 6 } 
	{ capsule0_key_d0 sc_out sc_lv 32 signal 6 } 
	{ capsule0_key_q0 sc_in sc_lv 32 signal 6 } 
	{ capsule0_value_address0 sc_out sc_lv 5 signal 7 } 
	{ capsule0_value_ce0 sc_out sc_logic 1 signal 7 } 
	{ capsule0_value_we0 sc_out sc_logic 1 signal 7 } 
	{ capsule0_value_d0 sc_out sc_lv 32 signal 7 } 
	{ capsule0_value_q0 sc_in sc_lv 32 signal 7 } 
	{ capsule1_key_address0 sc_out sc_lv 5 signal 8 } 
	{ capsule1_key_ce0 sc_out sc_logic 1 signal 8 } 
	{ capsule1_key_we0 sc_out sc_logic 1 signal 8 } 
	{ capsule1_key_d0 sc_out sc_lv 32 signal 8 } 
	{ capsule1_key_q0 sc_in sc_lv 32 signal 8 } 
	{ capsule1_value_address0 sc_out sc_lv 5 signal 9 } 
	{ capsule1_value_ce0 sc_out sc_logic 1 signal 9 } 
	{ capsule1_value_we0 sc_out sc_logic 1 signal 9 } 
	{ capsule1_value_d0 sc_out sc_lv 32 signal 9 } 
	{ capsule1_value_q0 sc_in sc_lv 32 signal 9 } 
	{ capsule2_key_address0 sc_out sc_lv 5 signal 10 } 
	{ capsule2_key_ce0 sc_out sc_logic 1 signal 10 } 
	{ capsule2_key_we0 sc_out sc_logic 1 signal 10 } 
	{ capsule2_key_d0 sc_out sc_lv 32 signal 10 } 
	{ capsule2_key_q0 sc_in sc_lv 32 signal 10 } 
	{ capsule2_value_address0 sc_out sc_lv 5 signal 11 } 
	{ capsule2_value_ce0 sc_out sc_logic 1 signal 11 } 
	{ capsule2_value_we0 sc_out sc_logic 1 signal 11 } 
	{ capsule2_value_d0 sc_out sc_lv 32 signal 11 } 
	{ capsule2_value_q0 sc_in sc_lv 32 signal 11 } 
	{ capsule3_key_address0 sc_out sc_lv 5 signal 12 } 
	{ capsule3_key_ce0 sc_out sc_logic 1 signal 12 } 
	{ capsule3_key_we0 sc_out sc_logic 1 signal 12 } 
	{ capsule3_key_d0 sc_out sc_lv 32 signal 12 } 
	{ capsule3_key_q0 sc_in sc_lv 32 signal 12 } 
	{ capsule3_value_address0 sc_out sc_lv 5 signal 13 } 
	{ capsule3_value_ce0 sc_out sc_logic 1 signal 13 } 
	{ capsule3_value_we0 sc_out sc_logic 1 signal 13 } 
	{ capsule3_value_d0 sc_out sc_lv 32 signal 13 } 
	{ capsule3_value_q0 sc_in sc_lv 32 signal 13 } 
	{ kvdeststats_tmp_valu_address0 sc_out sc_lv 5 signal 14 } 
	{ kvdeststats_tmp_valu_ce0 sc_out sc_logic 1 signal 14 } 
	{ kvdeststats_tmp_valu_we0 sc_out sc_logic 1 signal 14 } 
	{ kvdeststats_tmp_valu_d0 sc_out sc_lv 32 signal 14 } 
	{ kvdeststats_tmp_valu_q0 sc_in sc_lv 32 signal 14 } 
	{ offset sc_in sc_lv 24 signal 15 } 
	{ travstate_i_kvs sc_in sc_lv 32 signal 16 } 
	{ travstate_end_kvs sc_in sc_lv 30 signal 17 } 
	{ travstate_skip_kvs sc_in sc_lv 14 signal 18 } 
	{ capsulemetadata_globaloffset sc_in sc_lv 32 signal 19 } 
	{ capsulemetadata_localoffset sc_in sc_lv 32 signal 20 } 
	{ ap_return_0 sc_out sc_lv 32 signal -1 } 
	{ ap_return_1 sc_out sc_lv 32 signal -1 } 
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
 	{ "name": "BIGcapsule0_V_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":9, "type": "signal", "bundle":{"name": "BIGcapsule0_V", "role": "address0" }} , 
 	{ "name": "BIGcapsule0_V_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "BIGcapsule0_V", "role": "ce0" }} , 
 	{ "name": "BIGcapsule0_V_we0", "direction": "out", "datatype": "sc_lv", "bitwidth":64, "type": "signal", "bundle":{"name": "BIGcapsule0_V", "role": "we0" }} , 
 	{ "name": "BIGcapsule0_V_d0", "direction": "out", "datatype": "sc_lv", "bitwidth":512, "type": "signal", "bundle":{"name": "BIGcapsule0_V", "role": "d0" }} , 
 	{ "name": "BIGcapsule0_V_q0", "direction": "in", "datatype": "sc_lv", "bitwidth":512, "type": "signal", "bundle":{"name": "BIGcapsule0_V", "role": "q0" }} , 
 	{ "name": "BIGcapsule1_V_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":9, "type": "signal", "bundle":{"name": "BIGcapsule1_V", "role": "address0" }} , 
 	{ "name": "BIGcapsule1_V_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "BIGcapsule1_V", "role": "ce0" }} , 
 	{ "name": "BIGcapsule1_V_we0", "direction": "out", "datatype": "sc_lv", "bitwidth":64, "type": "signal", "bundle":{"name": "BIGcapsule1_V", "role": "we0" }} , 
 	{ "name": "BIGcapsule1_V_d0", "direction": "out", "datatype": "sc_lv", "bitwidth":512, "type": "signal", "bundle":{"name": "BIGcapsule1_V", "role": "d0" }} , 
 	{ "name": "BIGcapsule1_V_q0", "direction": "in", "datatype": "sc_lv", "bitwidth":512, "type": "signal", "bundle":{"name": "BIGcapsule1_V", "role": "q0" }} , 
 	{ "name": "BIGcapsule2_V_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":9, "type": "signal", "bundle":{"name": "BIGcapsule2_V", "role": "address0" }} , 
 	{ "name": "BIGcapsule2_V_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "BIGcapsule2_V", "role": "ce0" }} , 
 	{ "name": "BIGcapsule2_V_we0", "direction": "out", "datatype": "sc_lv", "bitwidth":64, "type": "signal", "bundle":{"name": "BIGcapsule2_V", "role": "we0" }} , 
 	{ "name": "BIGcapsule2_V_d0", "direction": "out", "datatype": "sc_lv", "bitwidth":512, "type": "signal", "bundle":{"name": "BIGcapsule2_V", "role": "d0" }} , 
 	{ "name": "BIGcapsule2_V_q0", "direction": "in", "datatype": "sc_lv", "bitwidth":512, "type": "signal", "bundle":{"name": "BIGcapsule2_V", "role": "q0" }} , 
 	{ "name": "BIGcapsule3_V_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":9, "type": "signal", "bundle":{"name": "BIGcapsule3_V", "role": "address0" }} , 
 	{ "name": "BIGcapsule3_V_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "BIGcapsule3_V", "role": "ce0" }} , 
 	{ "name": "BIGcapsule3_V_we0", "direction": "out", "datatype": "sc_lv", "bitwidth":64, "type": "signal", "bundle":{"name": "BIGcapsule3_V", "role": "we0" }} , 
 	{ "name": "BIGcapsule3_V_d0", "direction": "out", "datatype": "sc_lv", "bitwidth":512, "type": "signal", "bundle":{"name": "BIGcapsule3_V", "role": "d0" }} , 
 	{ "name": "BIGcapsule3_V_q0", "direction": "in", "datatype": "sc_lv", "bitwidth":512, "type": "signal", "bundle":{"name": "BIGcapsule3_V", "role": "q0" }} , 
 	{ "name": "capsule0_key_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":5, "type": "signal", "bundle":{"name": "capsule0_key", "role": "address0" }} , 
 	{ "name": "capsule0_key_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "capsule0_key", "role": "ce0" }} , 
 	{ "name": "capsule0_key_we0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "capsule0_key", "role": "we0" }} , 
 	{ "name": "capsule0_key_d0", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "capsule0_key", "role": "d0" }} , 
 	{ "name": "capsule0_key_q0", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "capsule0_key", "role": "q0" }} , 
 	{ "name": "capsule0_value_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":5, "type": "signal", "bundle":{"name": "capsule0_value", "role": "address0" }} , 
 	{ "name": "capsule0_value_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "capsule0_value", "role": "ce0" }} , 
 	{ "name": "capsule0_value_we0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "capsule0_value", "role": "we0" }} , 
 	{ "name": "capsule0_value_d0", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "capsule0_value", "role": "d0" }} , 
 	{ "name": "capsule0_value_q0", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "capsule0_value", "role": "q0" }} , 
 	{ "name": "capsule1_key_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":5, "type": "signal", "bundle":{"name": "capsule1_key", "role": "address0" }} , 
 	{ "name": "capsule1_key_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "capsule1_key", "role": "ce0" }} , 
 	{ "name": "capsule1_key_we0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "capsule1_key", "role": "we0" }} , 
 	{ "name": "capsule1_key_d0", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "capsule1_key", "role": "d0" }} , 
 	{ "name": "capsule1_key_q0", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "capsule1_key", "role": "q0" }} , 
 	{ "name": "capsule1_value_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":5, "type": "signal", "bundle":{"name": "capsule1_value", "role": "address0" }} , 
 	{ "name": "capsule1_value_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "capsule1_value", "role": "ce0" }} , 
 	{ "name": "capsule1_value_we0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "capsule1_value", "role": "we0" }} , 
 	{ "name": "capsule1_value_d0", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "capsule1_value", "role": "d0" }} , 
 	{ "name": "capsule1_value_q0", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "capsule1_value", "role": "q0" }} , 
 	{ "name": "capsule2_key_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":5, "type": "signal", "bundle":{"name": "capsule2_key", "role": "address0" }} , 
 	{ "name": "capsule2_key_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "capsule2_key", "role": "ce0" }} , 
 	{ "name": "capsule2_key_we0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "capsule2_key", "role": "we0" }} , 
 	{ "name": "capsule2_key_d0", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "capsule2_key", "role": "d0" }} , 
 	{ "name": "capsule2_key_q0", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "capsule2_key", "role": "q0" }} , 
 	{ "name": "capsule2_value_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":5, "type": "signal", "bundle":{"name": "capsule2_value", "role": "address0" }} , 
 	{ "name": "capsule2_value_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "capsule2_value", "role": "ce0" }} , 
 	{ "name": "capsule2_value_we0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "capsule2_value", "role": "we0" }} , 
 	{ "name": "capsule2_value_d0", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "capsule2_value", "role": "d0" }} , 
 	{ "name": "capsule2_value_q0", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "capsule2_value", "role": "q0" }} , 
 	{ "name": "capsule3_key_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":5, "type": "signal", "bundle":{"name": "capsule3_key", "role": "address0" }} , 
 	{ "name": "capsule3_key_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "capsule3_key", "role": "ce0" }} , 
 	{ "name": "capsule3_key_we0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "capsule3_key", "role": "we0" }} , 
 	{ "name": "capsule3_key_d0", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "capsule3_key", "role": "d0" }} , 
 	{ "name": "capsule3_key_q0", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "capsule3_key", "role": "q0" }} , 
 	{ "name": "capsule3_value_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":5, "type": "signal", "bundle":{"name": "capsule3_value", "role": "address0" }} , 
 	{ "name": "capsule3_value_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "capsule3_value", "role": "ce0" }} , 
 	{ "name": "capsule3_value_we0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "capsule3_value", "role": "we0" }} , 
 	{ "name": "capsule3_value_d0", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "capsule3_value", "role": "d0" }} , 
 	{ "name": "capsule3_value_q0", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "capsule3_value", "role": "q0" }} , 
 	{ "name": "kvdeststats_tmp_valu_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":5, "type": "signal", "bundle":{"name": "kvdeststats_tmp_valu", "role": "address0" }} , 
 	{ "name": "kvdeststats_tmp_valu_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "kvdeststats_tmp_valu", "role": "ce0" }} , 
 	{ "name": "kvdeststats_tmp_valu_we0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "kvdeststats_tmp_valu", "role": "we0" }} , 
 	{ "name": "kvdeststats_tmp_valu_d0", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "kvdeststats_tmp_valu", "role": "d0" }} , 
 	{ "name": "kvdeststats_tmp_valu_q0", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "kvdeststats_tmp_valu", "role": "q0" }} , 
 	{ "name": "offset", "direction": "in", "datatype": "sc_lv", "bitwidth":24, "type": "signal", "bundle":{"name": "offset", "role": "default" }} , 
 	{ "name": "travstate_i_kvs", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "travstate_i_kvs", "role": "default" }} , 
 	{ "name": "travstate_end_kvs", "direction": "in", "datatype": "sc_lv", "bitwidth":30, "type": "signal", "bundle":{"name": "travstate_end_kvs", "role": "default" }} , 
 	{ "name": "travstate_skip_kvs", "direction": "in", "datatype": "sc_lv", "bitwidth":14, "type": "signal", "bundle":{"name": "travstate_skip_kvs", "role": "default" }} , 
 	{ "name": "capsulemetadata_globaloffset", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "capsulemetadata_globaloffset", "role": "default" }} , 
 	{ "name": "capsulemetadata_localoffset", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "capsulemetadata_localoffset", "role": "default" }} , 
 	{ "name": "ap_return_0", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "ap_return_0", "role": "default" }} , 
 	{ "name": "ap_return_1", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "ap_return_1", "role": "default" }}  ]}

set RtlHierarchyInfo {[
	{"ID" : "0", "Level" : "0", "Path" : "`AUTOTB_DUT_INST", "Parent" : "", "Child" : ["1"],
		"CDFG" : "savecapsules0",
		"Protocol" : "ap_ctrl_hs",
		"ControlExist" : "1", "ap_start" : "1", "ap_ready" : "1", "ap_done" : "1", "ap_continue" : "0", "ap_idle" : "1",
		"Pipeline" : "None", "UnalignedPipeline" : "0", "RewindPipeline" : "0", "ProcessNetwork" : "0",
		"II" : "0",
		"VariableLatency" : "1", "ExactLatency" : "-1", "EstimateLatencyMin" : "358", "EstimateLatencyMax" : "2444",
		"Combinational" : "0",
		"Datapath" : "0",
		"ClockEnable" : "0",
		"HasSubDataflow" : "0",
		"InDataflowNetwork" : "0",
		"HasNonBlockingOperation" : "0",
		"WaitState" : [
			{"State" : "ap_ST_fsm_state15", "FSM" : "ap_CS_fsm", "SubInstance" : "grp_savekeyvalues0_fu_887"},
			{"State" : "ap_ST_fsm_state17", "FSM" : "ap_CS_fsm", "SubInstance" : "grp_savekeyvalues0_fu_887"},
			{"State" : "ap_ST_fsm_state19", "FSM" : "ap_CS_fsm", "SubInstance" : "grp_savekeyvalues0_fu_887"},
			{"State" : "ap_ST_fsm_state21", "FSM" : "ap_CS_fsm", "SubInstance" : "grp_savekeyvalues0_fu_887"}],
		"Port" : [
			{"Name" : "kvdram_V", "Type" : "MAXI", "Direction" : "O",
				"SubConnect" : [
					{"ID" : "1", "SubInstance" : "grp_savekeyvalues0_fu_887", "Port" : "kvdram_V"}]},
			{"Name" : "kvdram_V_offset", "Type" : "None", "Direction" : "I"},
			{"Name" : "BIGcapsule0_V", "Type" : "Memory", "Direction" : "IO",
				"SubConnect" : [
					{"ID" : "1", "SubInstance" : "grp_savekeyvalues0_fu_887", "Port" : "buffer_V"}]},
			{"Name" : "BIGcapsule1_V", "Type" : "Memory", "Direction" : "IO",
				"SubConnect" : [
					{"ID" : "1", "SubInstance" : "grp_savekeyvalues0_fu_887", "Port" : "buffer_V"}]},
			{"Name" : "BIGcapsule2_V", "Type" : "Memory", "Direction" : "IO",
				"SubConnect" : [
					{"ID" : "1", "SubInstance" : "grp_savekeyvalues0_fu_887", "Port" : "buffer_V"}]},
			{"Name" : "BIGcapsule3_V", "Type" : "Memory", "Direction" : "IO",
				"SubConnect" : [
					{"ID" : "1", "SubInstance" : "grp_savekeyvalues0_fu_887", "Port" : "buffer_V"}]},
			{"Name" : "capsule0_key", "Type" : "Memory", "Direction" : "IO"},
			{"Name" : "capsule0_value", "Type" : "Memory", "Direction" : "IO"},
			{"Name" : "capsule1_key", "Type" : "Memory", "Direction" : "IO"},
			{"Name" : "capsule1_value", "Type" : "Memory", "Direction" : "IO"},
			{"Name" : "capsule2_key", "Type" : "Memory", "Direction" : "IO"},
			{"Name" : "capsule2_value", "Type" : "Memory", "Direction" : "IO"},
			{"Name" : "capsule3_key", "Type" : "Memory", "Direction" : "IO"},
			{"Name" : "capsule3_value", "Type" : "Memory", "Direction" : "IO"},
			{"Name" : "kvdeststats_tmp_valu", "Type" : "Memory", "Direction" : "IO"},
			{"Name" : "offset", "Type" : "None", "Direction" : "I"},
			{"Name" : "travstate_i_kvs", "Type" : "None", "Direction" : "I"},
			{"Name" : "travstate_end_kvs", "Type" : "None", "Direction" : "I"},
			{"Name" : "travstate_skip_kvs", "Type" : "None", "Direction" : "I"},
			{"Name" : "capsulemetadata_globaloffset", "Type" : "None", "Direction" : "I"},
			{"Name" : "capsulemetadata_localoffset", "Type" : "None", "Direction" : "I"}]},
	{"ID" : "1", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.grp_savekeyvalues0_fu_887", "Parent" : "0",
		"CDFG" : "savekeyvalues0",
		"Protocol" : "ap_ctrl_hs",
		"ControlExist" : "1", "ap_start" : "1", "ap_ready" : "1", "ap_done" : "1", "ap_continue" : "0", "ap_idle" : "1",
		"Pipeline" : "None", "UnalignedPipeline" : "0", "RewindPipeline" : "0", "ProcessNetwork" : "0",
		"II" : "0",
		"VariableLatency" : "1", "ExactLatency" : "-1", "EstimateLatencyMin" : "520", "EstimateLatencyMax" : "520",
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
			{"Name" : "buffer_V", "Type" : "Memory", "Direction" : "I"},
			{"Name" : "offset_kvs", "Type" : "None", "Direction" : "I"}]}]}


set ArgLastReadFirstWriteLatency {
	savecapsules0 {
		kvdram_V {Type O LastRead 3 FirstWrite 4}
		kvdram_V_offset {Type I LastRead 0 FirstWrite -1}
		BIGcapsule0_V {Type IO LastRead 2 FirstWrite -1}
		BIGcapsule1_V {Type IO LastRead 2 FirstWrite -1}
		BIGcapsule2_V {Type IO LastRead 2 FirstWrite -1}
		BIGcapsule3_V {Type IO LastRead 2 FirstWrite -1}
		capsule0_key {Type IO LastRead 6 FirstWrite 4}
		capsule0_value {Type IO LastRead 5 FirstWrite 6}
		capsule1_key {Type IO LastRead 6 FirstWrite 4}
		capsule1_value {Type IO LastRead 5 FirstWrite 6}
		capsule2_key {Type IO LastRead 6 FirstWrite 4}
		capsule2_value {Type IO LastRead 5 FirstWrite 6}
		capsule3_key {Type IO LastRead 6 FirstWrite 4}
		capsule3_value {Type IO LastRead 5 FirstWrite 6}
		kvdeststats_tmp_valu {Type IO LastRead 4 FirstWrite 2}
		offset {Type I LastRead 0 FirstWrite -1}
		travstate_i_kvs {Type I LastRead 0 FirstWrite -1}
		travstate_end_kvs {Type I LastRead 0 FirstWrite -1}
		travstate_skip_kvs {Type I LastRead 0 FirstWrite -1}
		capsulemetadata_globaloffset {Type I LastRead 0 FirstWrite -1}
		capsulemetadata_localoffset {Type I LastRead 0 FirstWrite -1}}
	savekeyvalues0 {
		kvdram_V {Type O LastRead 3 FirstWrite 4}
		kvdram_V_offset {Type I LastRead 0 FirstWrite -1}
		buffer_V {Type I LastRead 2 FirstWrite -1}
		offset_kvs {Type I LastRead 0 FirstWrite -1}}}

set hasDtUnsupportedChannel 0

set PerformanceInfo {[
	{"Name" : "Latency", "Min" : "358", "Max" : "2444"}
	, {"Name" : "Interval", "Min" : "358", "Max" : "2444"}
]}

set PipelineEnableSignalInfo {[
	{"Pipeline" : "0", "EnableSignal" : "ap_enable_pp0"}
]}

set Spec2ImplPortList { 
	kvdram_V { m_axi {  { m_axi_kvdram_V_AWVALID VALID 1 1 }  { m_axi_kvdram_V_AWREADY READY 0 1 }  { m_axi_kvdram_V_AWADDR ADDR 1 32 }  { m_axi_kvdram_V_AWID ID 1 1 }  { m_axi_kvdram_V_AWLEN LEN 1 32 }  { m_axi_kvdram_V_AWSIZE SIZE 1 3 }  { m_axi_kvdram_V_AWBURST BURST 1 2 }  { m_axi_kvdram_V_AWLOCK LOCK 1 2 }  { m_axi_kvdram_V_AWCACHE CACHE 1 4 }  { m_axi_kvdram_V_AWPROT PROT 1 3 }  { m_axi_kvdram_V_AWQOS QOS 1 4 }  { m_axi_kvdram_V_AWREGION REGION 1 4 }  { m_axi_kvdram_V_AWUSER USER 1 1 }  { m_axi_kvdram_V_WVALID VALID 1 1 }  { m_axi_kvdram_V_WREADY READY 0 1 }  { m_axi_kvdram_V_WDATA DATA 1 512 }  { m_axi_kvdram_V_WSTRB STRB 1 64 }  { m_axi_kvdram_V_WLAST LAST 1 1 }  { m_axi_kvdram_V_WID ID 1 1 }  { m_axi_kvdram_V_WUSER USER 1 1 }  { m_axi_kvdram_V_ARVALID VALID 1 1 }  { m_axi_kvdram_V_ARREADY READY 0 1 }  { m_axi_kvdram_V_ARADDR ADDR 1 32 }  { m_axi_kvdram_V_ARID ID 1 1 }  { m_axi_kvdram_V_ARLEN LEN 1 32 }  { m_axi_kvdram_V_ARSIZE SIZE 1 3 }  { m_axi_kvdram_V_ARBURST BURST 1 2 }  { m_axi_kvdram_V_ARLOCK LOCK 1 2 }  { m_axi_kvdram_V_ARCACHE CACHE 1 4 }  { m_axi_kvdram_V_ARPROT PROT 1 3 }  { m_axi_kvdram_V_ARQOS QOS 1 4 }  { m_axi_kvdram_V_ARREGION REGION 1 4 }  { m_axi_kvdram_V_ARUSER USER 1 1 }  { m_axi_kvdram_V_RVALID VALID 0 1 }  { m_axi_kvdram_V_RREADY READY 1 1 }  { m_axi_kvdram_V_RDATA DATA 0 512 }  { m_axi_kvdram_V_RLAST LAST 0 1 }  { m_axi_kvdram_V_RID ID 0 1 }  { m_axi_kvdram_V_RUSER USER 0 1 }  { m_axi_kvdram_V_RRESP RESP 0 2 }  { m_axi_kvdram_V_BVALID VALID 0 1 }  { m_axi_kvdram_V_BREADY READY 1 1 }  { m_axi_kvdram_V_BRESP RESP 0 2 }  { m_axi_kvdram_V_BID ID 0 1 }  { m_axi_kvdram_V_BUSER USER 0 1 } } }
	kvdram_V_offset { ap_none {  { kvdram_V_offset in_data 0 26 } } }
	BIGcapsule0_V { ap_memory {  { BIGcapsule0_V_address0 mem_address 1 9 }  { BIGcapsule0_V_ce0 mem_ce 1 1 }  { BIGcapsule0_V_we0 mem_we 1 64 }  { BIGcapsule0_V_d0 mem_din 1 512 }  { BIGcapsule0_V_q0 mem_dout 0 512 } } }
	BIGcapsule1_V { ap_memory {  { BIGcapsule1_V_address0 mem_address 1 9 }  { BIGcapsule1_V_ce0 mem_ce 1 1 }  { BIGcapsule1_V_we0 mem_we 1 64 }  { BIGcapsule1_V_d0 mem_din 1 512 }  { BIGcapsule1_V_q0 mem_dout 0 512 } } }
	BIGcapsule2_V { ap_memory {  { BIGcapsule2_V_address0 mem_address 1 9 }  { BIGcapsule2_V_ce0 mem_ce 1 1 }  { BIGcapsule2_V_we0 mem_we 1 64 }  { BIGcapsule2_V_d0 mem_din 1 512 }  { BIGcapsule2_V_q0 mem_dout 0 512 } } }
	BIGcapsule3_V { ap_memory {  { BIGcapsule3_V_address0 mem_address 1 9 }  { BIGcapsule3_V_ce0 mem_ce 1 1 }  { BIGcapsule3_V_we0 mem_we 1 64 }  { BIGcapsule3_V_d0 mem_din 1 512 }  { BIGcapsule3_V_q0 mem_dout 0 512 } } }
	capsule0_key { ap_memory {  { capsule0_key_address0 mem_address 1 5 }  { capsule0_key_ce0 mem_ce 1 1 }  { capsule0_key_we0 mem_we 1 1 }  { capsule0_key_d0 mem_din 1 32 }  { capsule0_key_q0 mem_dout 0 32 } } }
	capsule0_value { ap_memory {  { capsule0_value_address0 mem_address 1 5 }  { capsule0_value_ce0 mem_ce 1 1 }  { capsule0_value_we0 mem_we 1 1 }  { capsule0_value_d0 mem_din 1 32 }  { capsule0_value_q0 mem_dout 0 32 } } }
	capsule1_key { ap_memory {  { capsule1_key_address0 mem_address 1 5 }  { capsule1_key_ce0 mem_ce 1 1 }  { capsule1_key_we0 mem_we 1 1 }  { capsule1_key_d0 mem_din 1 32 }  { capsule1_key_q0 mem_dout 0 32 } } }
	capsule1_value { ap_memory {  { capsule1_value_address0 mem_address 1 5 }  { capsule1_value_ce0 mem_ce 1 1 }  { capsule1_value_we0 mem_we 1 1 }  { capsule1_value_d0 mem_din 1 32 }  { capsule1_value_q0 mem_dout 0 32 } } }
	capsule2_key { ap_memory {  { capsule2_key_address0 mem_address 1 5 }  { capsule2_key_ce0 mem_ce 1 1 }  { capsule2_key_we0 mem_we 1 1 }  { capsule2_key_d0 mem_din 1 32 }  { capsule2_key_q0 mem_dout 0 32 } } }
	capsule2_value { ap_memory {  { capsule2_value_address0 mem_address 1 5 }  { capsule2_value_ce0 mem_ce 1 1 }  { capsule2_value_we0 mem_we 1 1 }  { capsule2_value_d0 mem_din 1 32 }  { capsule2_value_q0 mem_dout 0 32 } } }
	capsule3_key { ap_memory {  { capsule3_key_address0 mem_address 1 5 }  { capsule3_key_ce0 mem_ce 1 1 }  { capsule3_key_we0 mem_we 1 1 }  { capsule3_key_d0 mem_din 1 32 }  { capsule3_key_q0 mem_dout 0 32 } } }
	capsule3_value { ap_memory {  { capsule3_value_address0 mem_address 1 5 }  { capsule3_value_ce0 mem_ce 1 1 }  { capsule3_value_we0 mem_we 1 1 }  { capsule3_value_d0 mem_din 1 32 }  { capsule3_value_q0 mem_dout 0 32 } } }
	kvdeststats_tmp_valu { ap_memory {  { kvdeststats_tmp_valu_address0 mem_address 1 5 }  { kvdeststats_tmp_valu_ce0 mem_ce 1 1 }  { kvdeststats_tmp_valu_we0 mem_we 1 1 }  { kvdeststats_tmp_valu_d0 mem_din 1 32 }  { kvdeststats_tmp_valu_q0 mem_dout 0 32 } } }
	offset { ap_none {  { offset in_data 0 24 } } }
	travstate_i_kvs { ap_none {  { travstate_i_kvs in_data 0 32 } } }
	travstate_end_kvs { ap_none {  { travstate_end_kvs in_data 0 30 } } }
	travstate_skip_kvs { ap_none {  { travstate_skip_kvs in_data 0 14 } } }
	capsulemetadata_globaloffset { ap_none {  { capsulemetadata_globaloffset in_data 0 32 } } }
	capsulemetadata_localoffset { ap_none {  { capsulemetadata_localoffset in_data 0 32 } } }
}
