set moduleName generatepartitions0
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
set C_modelName {generatepartitions0}
set C_modelType { void 0 }
set C_modelArgList {
	{ kvsetdram_V int 512 regular {axi_master 2}  }
	{ kvsetdram_V_offset int 26 regular  }
	{ kvstats int 64 regular {axi_master 2}  }
	{ kvstats_offset int 29 regular  }
	{ globalparams_reducecommand int 32 regular  }
	{ globalparams_treedepth int 32 regular  }
	{ globalparams_LLOPnumpartitions int 32 regular  }
	{ globalparams_vbegin int 32 regular  }
}
set C_modelArgMapList {[ 
	{ "Name" : "kvsetdram_V", "interface" : "axi_master", "bitwidth" : 512, "direction" : "READWRITE"} , 
 	{ "Name" : "kvsetdram_V_offset", "interface" : "wire", "bitwidth" : 26, "direction" : "READONLY"} , 
 	{ "Name" : "kvstats", "interface" : "axi_master", "bitwidth" : 64, "direction" : "READWRITE"} , 
 	{ "Name" : "kvstats_offset", "interface" : "wire", "bitwidth" : 29, "direction" : "READONLY"} , 
 	{ "Name" : "globalparams_reducecommand", "interface" : "wire", "bitwidth" : 32, "direction" : "READONLY"} , 
 	{ "Name" : "globalparams_treedepth", "interface" : "wire", "bitwidth" : 32, "direction" : "READONLY"} , 
 	{ "Name" : "globalparams_LLOPnumpartitions", "interface" : "wire", "bitwidth" : 32, "direction" : "READONLY"} , 
 	{ "Name" : "globalparams_vbegin", "interface" : "wire", "bitwidth" : 32, "direction" : "READONLY"} ]}
# RTL Port declarations: 
set portNum 102
set portList { 
	{ ap_clk sc_in sc_logic 1 clock -1 } 
	{ ap_rst sc_in sc_logic 1 reset -1 active_high_sync } 
	{ ap_start sc_in sc_logic 1 start -1 } 
	{ ap_done sc_out sc_logic 1 predone -1 } 
	{ ap_idle sc_out sc_logic 1 done -1 } 
	{ ap_ready sc_out sc_logic 1 ready -1 } 
	{ m_axi_kvsetdram_V_AWVALID sc_out sc_logic 1 signal 0 } 
	{ m_axi_kvsetdram_V_AWREADY sc_in sc_logic 1 signal 0 } 
	{ m_axi_kvsetdram_V_AWADDR sc_out sc_lv 32 signal 0 } 
	{ m_axi_kvsetdram_V_AWID sc_out sc_lv 1 signal 0 } 
	{ m_axi_kvsetdram_V_AWLEN sc_out sc_lv 32 signal 0 } 
	{ m_axi_kvsetdram_V_AWSIZE sc_out sc_lv 3 signal 0 } 
	{ m_axi_kvsetdram_V_AWBURST sc_out sc_lv 2 signal 0 } 
	{ m_axi_kvsetdram_V_AWLOCK sc_out sc_lv 2 signal 0 } 
	{ m_axi_kvsetdram_V_AWCACHE sc_out sc_lv 4 signal 0 } 
	{ m_axi_kvsetdram_V_AWPROT sc_out sc_lv 3 signal 0 } 
	{ m_axi_kvsetdram_V_AWQOS sc_out sc_lv 4 signal 0 } 
	{ m_axi_kvsetdram_V_AWREGION sc_out sc_lv 4 signal 0 } 
	{ m_axi_kvsetdram_V_AWUSER sc_out sc_lv 1 signal 0 } 
	{ m_axi_kvsetdram_V_WVALID sc_out sc_logic 1 signal 0 } 
	{ m_axi_kvsetdram_V_WREADY sc_in sc_logic 1 signal 0 } 
	{ m_axi_kvsetdram_V_WDATA sc_out sc_lv 512 signal 0 } 
	{ m_axi_kvsetdram_V_WSTRB sc_out sc_lv 64 signal 0 } 
	{ m_axi_kvsetdram_V_WLAST sc_out sc_logic 1 signal 0 } 
	{ m_axi_kvsetdram_V_WID sc_out sc_lv 1 signal 0 } 
	{ m_axi_kvsetdram_V_WUSER sc_out sc_lv 1 signal 0 } 
	{ m_axi_kvsetdram_V_ARVALID sc_out sc_logic 1 signal 0 } 
	{ m_axi_kvsetdram_V_ARREADY sc_in sc_logic 1 signal 0 } 
	{ m_axi_kvsetdram_V_ARADDR sc_out sc_lv 32 signal 0 } 
	{ m_axi_kvsetdram_V_ARID sc_out sc_lv 1 signal 0 } 
	{ m_axi_kvsetdram_V_ARLEN sc_out sc_lv 32 signal 0 } 
	{ m_axi_kvsetdram_V_ARSIZE sc_out sc_lv 3 signal 0 } 
	{ m_axi_kvsetdram_V_ARBURST sc_out sc_lv 2 signal 0 } 
	{ m_axi_kvsetdram_V_ARLOCK sc_out sc_lv 2 signal 0 } 
	{ m_axi_kvsetdram_V_ARCACHE sc_out sc_lv 4 signal 0 } 
	{ m_axi_kvsetdram_V_ARPROT sc_out sc_lv 3 signal 0 } 
	{ m_axi_kvsetdram_V_ARQOS sc_out sc_lv 4 signal 0 } 
	{ m_axi_kvsetdram_V_ARREGION sc_out sc_lv 4 signal 0 } 
	{ m_axi_kvsetdram_V_ARUSER sc_out sc_lv 1 signal 0 } 
	{ m_axi_kvsetdram_V_RVALID sc_in sc_logic 1 signal 0 } 
	{ m_axi_kvsetdram_V_RREADY sc_out sc_logic 1 signal 0 } 
	{ m_axi_kvsetdram_V_RDATA sc_in sc_lv 512 signal 0 } 
	{ m_axi_kvsetdram_V_RLAST sc_in sc_logic 1 signal 0 } 
	{ m_axi_kvsetdram_V_RID sc_in sc_lv 1 signal 0 } 
	{ m_axi_kvsetdram_V_RUSER sc_in sc_lv 1 signal 0 } 
	{ m_axi_kvsetdram_V_RRESP sc_in sc_lv 2 signal 0 } 
	{ m_axi_kvsetdram_V_BVALID sc_in sc_logic 1 signal 0 } 
	{ m_axi_kvsetdram_V_BREADY sc_out sc_logic 1 signal 0 } 
	{ m_axi_kvsetdram_V_BRESP sc_in sc_lv 2 signal 0 } 
	{ m_axi_kvsetdram_V_BID sc_in sc_lv 1 signal 0 } 
	{ m_axi_kvsetdram_V_BUSER sc_in sc_lv 1 signal 0 } 
	{ kvsetdram_V_offset sc_in sc_lv 26 signal 1 } 
	{ m_axi_kvstats_AWVALID sc_out sc_logic 1 signal 2 } 
	{ m_axi_kvstats_AWREADY sc_in sc_logic 1 signal 2 } 
	{ m_axi_kvstats_AWADDR sc_out sc_lv 32 signal 2 } 
	{ m_axi_kvstats_AWID sc_out sc_lv 1 signal 2 } 
	{ m_axi_kvstats_AWLEN sc_out sc_lv 32 signal 2 } 
	{ m_axi_kvstats_AWSIZE sc_out sc_lv 3 signal 2 } 
	{ m_axi_kvstats_AWBURST sc_out sc_lv 2 signal 2 } 
	{ m_axi_kvstats_AWLOCK sc_out sc_lv 2 signal 2 } 
	{ m_axi_kvstats_AWCACHE sc_out sc_lv 4 signal 2 } 
	{ m_axi_kvstats_AWPROT sc_out sc_lv 3 signal 2 } 
	{ m_axi_kvstats_AWQOS sc_out sc_lv 4 signal 2 } 
	{ m_axi_kvstats_AWREGION sc_out sc_lv 4 signal 2 } 
	{ m_axi_kvstats_AWUSER sc_out sc_lv 1 signal 2 } 
	{ m_axi_kvstats_WVALID sc_out sc_logic 1 signal 2 } 
	{ m_axi_kvstats_WREADY sc_in sc_logic 1 signal 2 } 
	{ m_axi_kvstats_WDATA sc_out sc_lv 64 signal 2 } 
	{ m_axi_kvstats_WSTRB sc_out sc_lv 8 signal 2 } 
	{ m_axi_kvstats_WLAST sc_out sc_logic 1 signal 2 } 
	{ m_axi_kvstats_WID sc_out sc_lv 1 signal 2 } 
	{ m_axi_kvstats_WUSER sc_out sc_lv 1 signal 2 } 
	{ m_axi_kvstats_ARVALID sc_out sc_logic 1 signal 2 } 
	{ m_axi_kvstats_ARREADY sc_in sc_logic 1 signal 2 } 
	{ m_axi_kvstats_ARADDR sc_out sc_lv 32 signal 2 } 
	{ m_axi_kvstats_ARID sc_out sc_lv 1 signal 2 } 
	{ m_axi_kvstats_ARLEN sc_out sc_lv 32 signal 2 } 
	{ m_axi_kvstats_ARSIZE sc_out sc_lv 3 signal 2 } 
	{ m_axi_kvstats_ARBURST sc_out sc_lv 2 signal 2 } 
	{ m_axi_kvstats_ARLOCK sc_out sc_lv 2 signal 2 } 
	{ m_axi_kvstats_ARCACHE sc_out sc_lv 4 signal 2 } 
	{ m_axi_kvstats_ARPROT sc_out sc_lv 3 signal 2 } 
	{ m_axi_kvstats_ARQOS sc_out sc_lv 4 signal 2 } 
	{ m_axi_kvstats_ARREGION sc_out sc_lv 4 signal 2 } 
	{ m_axi_kvstats_ARUSER sc_out sc_lv 1 signal 2 } 
	{ m_axi_kvstats_RVALID sc_in sc_logic 1 signal 2 } 
	{ m_axi_kvstats_RREADY sc_out sc_logic 1 signal 2 } 
	{ m_axi_kvstats_RDATA sc_in sc_lv 64 signal 2 } 
	{ m_axi_kvstats_RLAST sc_in sc_logic 1 signal 2 } 
	{ m_axi_kvstats_RID sc_in sc_lv 1 signal 2 } 
	{ m_axi_kvstats_RUSER sc_in sc_lv 1 signal 2 } 
	{ m_axi_kvstats_RRESP sc_in sc_lv 2 signal 2 } 
	{ m_axi_kvstats_BVALID sc_in sc_logic 1 signal 2 } 
	{ m_axi_kvstats_BREADY sc_out sc_logic 1 signal 2 } 
	{ m_axi_kvstats_BRESP sc_in sc_lv 2 signal 2 } 
	{ m_axi_kvstats_BID sc_in sc_lv 1 signal 2 } 
	{ m_axi_kvstats_BUSER sc_in sc_lv 1 signal 2 } 
	{ kvstats_offset sc_in sc_lv 29 signal 3 } 
	{ globalparams_reducecommand sc_in sc_lv 32 signal 4 } 
	{ globalparams_treedepth sc_in sc_lv 32 signal 5 } 
	{ globalparams_LLOPnumpartitions sc_in sc_lv 32 signal 6 } 
	{ globalparams_vbegin sc_in sc_lv 32 signal 7 } 
}
set NewPortList {[ 
	{ "name": "ap_clk", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "clock", "bundle":{"name": "ap_clk", "role": "default" }} , 
 	{ "name": "ap_rst", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "reset", "bundle":{"name": "ap_rst", "role": "default" }} , 
 	{ "name": "ap_start", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "start", "bundle":{"name": "ap_start", "role": "default" }} , 
 	{ "name": "ap_done", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "predone", "bundle":{"name": "ap_done", "role": "default" }} , 
 	{ "name": "ap_idle", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "done", "bundle":{"name": "ap_idle", "role": "default" }} , 
 	{ "name": "ap_ready", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "ready", "bundle":{"name": "ap_ready", "role": "default" }} , 
 	{ "name": "m_axi_kvsetdram_V_AWVALID", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "kvsetdram_V", "role": "AWVALID" }} , 
 	{ "name": "m_axi_kvsetdram_V_AWREADY", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "kvsetdram_V", "role": "AWREADY" }} , 
 	{ "name": "m_axi_kvsetdram_V_AWADDR", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "kvsetdram_V", "role": "AWADDR" }} , 
 	{ "name": "m_axi_kvsetdram_V_AWID", "direction": "out", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "kvsetdram_V", "role": "AWID" }} , 
 	{ "name": "m_axi_kvsetdram_V_AWLEN", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "kvsetdram_V", "role": "AWLEN" }} , 
 	{ "name": "m_axi_kvsetdram_V_AWSIZE", "direction": "out", "datatype": "sc_lv", "bitwidth":3, "type": "signal", "bundle":{"name": "kvsetdram_V", "role": "AWSIZE" }} , 
 	{ "name": "m_axi_kvsetdram_V_AWBURST", "direction": "out", "datatype": "sc_lv", "bitwidth":2, "type": "signal", "bundle":{"name": "kvsetdram_V", "role": "AWBURST" }} , 
 	{ "name": "m_axi_kvsetdram_V_AWLOCK", "direction": "out", "datatype": "sc_lv", "bitwidth":2, "type": "signal", "bundle":{"name": "kvsetdram_V", "role": "AWLOCK" }} , 
 	{ "name": "m_axi_kvsetdram_V_AWCACHE", "direction": "out", "datatype": "sc_lv", "bitwidth":4, "type": "signal", "bundle":{"name": "kvsetdram_V", "role": "AWCACHE" }} , 
 	{ "name": "m_axi_kvsetdram_V_AWPROT", "direction": "out", "datatype": "sc_lv", "bitwidth":3, "type": "signal", "bundle":{"name": "kvsetdram_V", "role": "AWPROT" }} , 
 	{ "name": "m_axi_kvsetdram_V_AWQOS", "direction": "out", "datatype": "sc_lv", "bitwidth":4, "type": "signal", "bundle":{"name": "kvsetdram_V", "role": "AWQOS" }} , 
 	{ "name": "m_axi_kvsetdram_V_AWREGION", "direction": "out", "datatype": "sc_lv", "bitwidth":4, "type": "signal", "bundle":{"name": "kvsetdram_V", "role": "AWREGION" }} , 
 	{ "name": "m_axi_kvsetdram_V_AWUSER", "direction": "out", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "kvsetdram_V", "role": "AWUSER" }} , 
 	{ "name": "m_axi_kvsetdram_V_WVALID", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "kvsetdram_V", "role": "WVALID" }} , 
 	{ "name": "m_axi_kvsetdram_V_WREADY", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "kvsetdram_V", "role": "WREADY" }} , 
 	{ "name": "m_axi_kvsetdram_V_WDATA", "direction": "out", "datatype": "sc_lv", "bitwidth":512, "type": "signal", "bundle":{"name": "kvsetdram_V", "role": "WDATA" }} , 
 	{ "name": "m_axi_kvsetdram_V_WSTRB", "direction": "out", "datatype": "sc_lv", "bitwidth":64, "type": "signal", "bundle":{"name": "kvsetdram_V", "role": "WSTRB" }} , 
 	{ "name": "m_axi_kvsetdram_V_WLAST", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "kvsetdram_V", "role": "WLAST" }} , 
 	{ "name": "m_axi_kvsetdram_V_WID", "direction": "out", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "kvsetdram_V", "role": "WID" }} , 
 	{ "name": "m_axi_kvsetdram_V_WUSER", "direction": "out", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "kvsetdram_V", "role": "WUSER" }} , 
 	{ "name": "m_axi_kvsetdram_V_ARVALID", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "kvsetdram_V", "role": "ARVALID" }} , 
 	{ "name": "m_axi_kvsetdram_V_ARREADY", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "kvsetdram_V", "role": "ARREADY" }} , 
 	{ "name": "m_axi_kvsetdram_V_ARADDR", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "kvsetdram_V", "role": "ARADDR" }} , 
 	{ "name": "m_axi_kvsetdram_V_ARID", "direction": "out", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "kvsetdram_V", "role": "ARID" }} , 
 	{ "name": "m_axi_kvsetdram_V_ARLEN", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "kvsetdram_V", "role": "ARLEN" }} , 
 	{ "name": "m_axi_kvsetdram_V_ARSIZE", "direction": "out", "datatype": "sc_lv", "bitwidth":3, "type": "signal", "bundle":{"name": "kvsetdram_V", "role": "ARSIZE" }} , 
 	{ "name": "m_axi_kvsetdram_V_ARBURST", "direction": "out", "datatype": "sc_lv", "bitwidth":2, "type": "signal", "bundle":{"name": "kvsetdram_V", "role": "ARBURST" }} , 
 	{ "name": "m_axi_kvsetdram_V_ARLOCK", "direction": "out", "datatype": "sc_lv", "bitwidth":2, "type": "signal", "bundle":{"name": "kvsetdram_V", "role": "ARLOCK" }} , 
 	{ "name": "m_axi_kvsetdram_V_ARCACHE", "direction": "out", "datatype": "sc_lv", "bitwidth":4, "type": "signal", "bundle":{"name": "kvsetdram_V", "role": "ARCACHE" }} , 
 	{ "name": "m_axi_kvsetdram_V_ARPROT", "direction": "out", "datatype": "sc_lv", "bitwidth":3, "type": "signal", "bundle":{"name": "kvsetdram_V", "role": "ARPROT" }} , 
 	{ "name": "m_axi_kvsetdram_V_ARQOS", "direction": "out", "datatype": "sc_lv", "bitwidth":4, "type": "signal", "bundle":{"name": "kvsetdram_V", "role": "ARQOS" }} , 
 	{ "name": "m_axi_kvsetdram_V_ARREGION", "direction": "out", "datatype": "sc_lv", "bitwidth":4, "type": "signal", "bundle":{"name": "kvsetdram_V", "role": "ARREGION" }} , 
 	{ "name": "m_axi_kvsetdram_V_ARUSER", "direction": "out", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "kvsetdram_V", "role": "ARUSER" }} , 
 	{ "name": "m_axi_kvsetdram_V_RVALID", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "kvsetdram_V", "role": "RVALID" }} , 
 	{ "name": "m_axi_kvsetdram_V_RREADY", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "kvsetdram_V", "role": "RREADY" }} , 
 	{ "name": "m_axi_kvsetdram_V_RDATA", "direction": "in", "datatype": "sc_lv", "bitwidth":512, "type": "signal", "bundle":{"name": "kvsetdram_V", "role": "RDATA" }} , 
 	{ "name": "m_axi_kvsetdram_V_RLAST", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "kvsetdram_V", "role": "RLAST" }} , 
 	{ "name": "m_axi_kvsetdram_V_RID", "direction": "in", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "kvsetdram_V", "role": "RID" }} , 
 	{ "name": "m_axi_kvsetdram_V_RUSER", "direction": "in", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "kvsetdram_V", "role": "RUSER" }} , 
 	{ "name": "m_axi_kvsetdram_V_RRESP", "direction": "in", "datatype": "sc_lv", "bitwidth":2, "type": "signal", "bundle":{"name": "kvsetdram_V", "role": "RRESP" }} , 
 	{ "name": "m_axi_kvsetdram_V_BVALID", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "kvsetdram_V", "role": "BVALID" }} , 
 	{ "name": "m_axi_kvsetdram_V_BREADY", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "kvsetdram_V", "role": "BREADY" }} , 
 	{ "name": "m_axi_kvsetdram_V_BRESP", "direction": "in", "datatype": "sc_lv", "bitwidth":2, "type": "signal", "bundle":{"name": "kvsetdram_V", "role": "BRESP" }} , 
 	{ "name": "m_axi_kvsetdram_V_BID", "direction": "in", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "kvsetdram_V", "role": "BID" }} , 
 	{ "name": "m_axi_kvsetdram_V_BUSER", "direction": "in", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "kvsetdram_V", "role": "BUSER" }} , 
 	{ "name": "kvsetdram_V_offset", "direction": "in", "datatype": "sc_lv", "bitwidth":26, "type": "signal", "bundle":{"name": "kvsetdram_V_offset", "role": "default" }} , 
 	{ "name": "m_axi_kvstats_AWVALID", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "kvstats", "role": "AWVALID" }} , 
 	{ "name": "m_axi_kvstats_AWREADY", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "kvstats", "role": "AWREADY" }} , 
 	{ "name": "m_axi_kvstats_AWADDR", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "kvstats", "role": "AWADDR" }} , 
 	{ "name": "m_axi_kvstats_AWID", "direction": "out", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "kvstats", "role": "AWID" }} , 
 	{ "name": "m_axi_kvstats_AWLEN", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "kvstats", "role": "AWLEN" }} , 
 	{ "name": "m_axi_kvstats_AWSIZE", "direction": "out", "datatype": "sc_lv", "bitwidth":3, "type": "signal", "bundle":{"name": "kvstats", "role": "AWSIZE" }} , 
 	{ "name": "m_axi_kvstats_AWBURST", "direction": "out", "datatype": "sc_lv", "bitwidth":2, "type": "signal", "bundle":{"name": "kvstats", "role": "AWBURST" }} , 
 	{ "name": "m_axi_kvstats_AWLOCK", "direction": "out", "datatype": "sc_lv", "bitwidth":2, "type": "signal", "bundle":{"name": "kvstats", "role": "AWLOCK" }} , 
 	{ "name": "m_axi_kvstats_AWCACHE", "direction": "out", "datatype": "sc_lv", "bitwidth":4, "type": "signal", "bundle":{"name": "kvstats", "role": "AWCACHE" }} , 
 	{ "name": "m_axi_kvstats_AWPROT", "direction": "out", "datatype": "sc_lv", "bitwidth":3, "type": "signal", "bundle":{"name": "kvstats", "role": "AWPROT" }} , 
 	{ "name": "m_axi_kvstats_AWQOS", "direction": "out", "datatype": "sc_lv", "bitwidth":4, "type": "signal", "bundle":{"name": "kvstats", "role": "AWQOS" }} , 
 	{ "name": "m_axi_kvstats_AWREGION", "direction": "out", "datatype": "sc_lv", "bitwidth":4, "type": "signal", "bundle":{"name": "kvstats", "role": "AWREGION" }} , 
 	{ "name": "m_axi_kvstats_AWUSER", "direction": "out", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "kvstats", "role": "AWUSER" }} , 
 	{ "name": "m_axi_kvstats_WVALID", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "kvstats", "role": "WVALID" }} , 
 	{ "name": "m_axi_kvstats_WREADY", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "kvstats", "role": "WREADY" }} , 
 	{ "name": "m_axi_kvstats_WDATA", "direction": "out", "datatype": "sc_lv", "bitwidth":64, "type": "signal", "bundle":{"name": "kvstats", "role": "WDATA" }} , 
 	{ "name": "m_axi_kvstats_WSTRB", "direction": "out", "datatype": "sc_lv", "bitwidth":8, "type": "signal", "bundle":{"name": "kvstats", "role": "WSTRB" }} , 
 	{ "name": "m_axi_kvstats_WLAST", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "kvstats", "role": "WLAST" }} , 
 	{ "name": "m_axi_kvstats_WID", "direction": "out", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "kvstats", "role": "WID" }} , 
 	{ "name": "m_axi_kvstats_WUSER", "direction": "out", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "kvstats", "role": "WUSER" }} , 
 	{ "name": "m_axi_kvstats_ARVALID", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "kvstats", "role": "ARVALID" }} , 
 	{ "name": "m_axi_kvstats_ARREADY", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "kvstats", "role": "ARREADY" }} , 
 	{ "name": "m_axi_kvstats_ARADDR", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "kvstats", "role": "ARADDR" }} , 
 	{ "name": "m_axi_kvstats_ARID", "direction": "out", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "kvstats", "role": "ARID" }} , 
 	{ "name": "m_axi_kvstats_ARLEN", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "kvstats", "role": "ARLEN" }} , 
 	{ "name": "m_axi_kvstats_ARSIZE", "direction": "out", "datatype": "sc_lv", "bitwidth":3, "type": "signal", "bundle":{"name": "kvstats", "role": "ARSIZE" }} , 
 	{ "name": "m_axi_kvstats_ARBURST", "direction": "out", "datatype": "sc_lv", "bitwidth":2, "type": "signal", "bundle":{"name": "kvstats", "role": "ARBURST" }} , 
 	{ "name": "m_axi_kvstats_ARLOCK", "direction": "out", "datatype": "sc_lv", "bitwidth":2, "type": "signal", "bundle":{"name": "kvstats", "role": "ARLOCK" }} , 
 	{ "name": "m_axi_kvstats_ARCACHE", "direction": "out", "datatype": "sc_lv", "bitwidth":4, "type": "signal", "bundle":{"name": "kvstats", "role": "ARCACHE" }} , 
 	{ "name": "m_axi_kvstats_ARPROT", "direction": "out", "datatype": "sc_lv", "bitwidth":3, "type": "signal", "bundle":{"name": "kvstats", "role": "ARPROT" }} , 
 	{ "name": "m_axi_kvstats_ARQOS", "direction": "out", "datatype": "sc_lv", "bitwidth":4, "type": "signal", "bundle":{"name": "kvstats", "role": "ARQOS" }} , 
 	{ "name": "m_axi_kvstats_ARREGION", "direction": "out", "datatype": "sc_lv", "bitwidth":4, "type": "signal", "bundle":{"name": "kvstats", "role": "ARREGION" }} , 
 	{ "name": "m_axi_kvstats_ARUSER", "direction": "out", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "kvstats", "role": "ARUSER" }} , 
 	{ "name": "m_axi_kvstats_RVALID", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "kvstats", "role": "RVALID" }} , 
 	{ "name": "m_axi_kvstats_RREADY", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "kvstats", "role": "RREADY" }} , 
 	{ "name": "m_axi_kvstats_RDATA", "direction": "in", "datatype": "sc_lv", "bitwidth":64, "type": "signal", "bundle":{"name": "kvstats", "role": "RDATA" }} , 
 	{ "name": "m_axi_kvstats_RLAST", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "kvstats", "role": "RLAST" }} , 
 	{ "name": "m_axi_kvstats_RID", "direction": "in", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "kvstats", "role": "RID" }} , 
 	{ "name": "m_axi_kvstats_RUSER", "direction": "in", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "kvstats", "role": "RUSER" }} , 
 	{ "name": "m_axi_kvstats_RRESP", "direction": "in", "datatype": "sc_lv", "bitwidth":2, "type": "signal", "bundle":{"name": "kvstats", "role": "RRESP" }} , 
 	{ "name": "m_axi_kvstats_BVALID", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "kvstats", "role": "BVALID" }} , 
 	{ "name": "m_axi_kvstats_BREADY", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "kvstats", "role": "BREADY" }} , 
 	{ "name": "m_axi_kvstats_BRESP", "direction": "in", "datatype": "sc_lv", "bitwidth":2, "type": "signal", "bundle":{"name": "kvstats", "role": "BRESP" }} , 
 	{ "name": "m_axi_kvstats_BID", "direction": "in", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "kvstats", "role": "BID" }} , 
 	{ "name": "m_axi_kvstats_BUSER", "direction": "in", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "kvstats", "role": "BUSER" }} , 
 	{ "name": "kvstats_offset", "direction": "in", "datatype": "sc_lv", "bitwidth":29, "type": "signal", "bundle":{"name": "kvstats_offset", "role": "default" }} , 
 	{ "name": "globalparams_reducecommand", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "globalparams_reducecommand", "role": "default" }} , 
 	{ "name": "globalparams_treedepth", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "globalparams_treedepth", "role": "default" }} , 
 	{ "name": "globalparams_LLOPnumpartitions", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "globalparams_LLOPnumpartitions", "role": "default" }} , 
 	{ "name": "globalparams_vbegin", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "globalparams_vbegin", "role": "default" }}  ]}

set RtlHierarchyInfo {[
	{"ID" : "0", "Level" : "0", "Path" : "`AUTOTB_DUT_INST", "Parent" : "", "Child" : ["1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13", "14", "15", "16", "17", "18", "19", "20", "21", "22", "23", "24", "25", "26", "27", "28", "29", "30", "31", "32", "33", "34", "35", "36", "37", "38", "39", "40", "41", "42"],
		"CDFG" : "generatepartitions0",
		"Protocol" : "ap_ctrl_hs",
		"ControlExist" : "1", "ap_start" : "1", "ap_ready" : "1", "ap_done" : "1", "ap_continue" : "0", "ap_idle" : "1",
		"Pipeline" : "None", "UnalignedPipeline" : "0", "RewindPipeline" : "0", "ProcessNetwork" : "0",
		"II" : "0",
		"VariableLatency" : "1", "ExactLatency" : "-1", "EstimateLatencyMin" : "2", "EstimateLatencyMax" : "302088260",
		"Combinational" : "0",
		"Datapath" : "0",
		"ClockEnable" : "0",
		"HasSubDataflow" : "0",
		"InDataflowNetwork" : "0",
		"HasNonBlockingOperation" : "0",
		"Port" : [
			{"Name" : "kvsetdram_V", "Type" : "MAXI", "Direction" : "IO",
				"BlockSignal" : [
					{"Name" : "kvsetdram_V_blk_n_AR", "Type" : "RtlSignal"},
					{"Name" : "kvsetdram_V_blk_n_R", "Type" : "RtlSignal"},
					{"Name" : "kvsetdram_V_blk_n_AW", "Type" : "RtlSignal"},
					{"Name" : "kvsetdram_V_blk_n_W", "Type" : "RtlSignal"},
					{"Name" : "kvsetdram_V_blk_n_B", "Type" : "RtlSignal"}]},
			{"Name" : "kvsetdram_V_offset", "Type" : "None", "Direction" : "I"},
			{"Name" : "kvstats", "Type" : "MAXI", "Direction" : "IO",
				"BlockSignal" : [
					{"Name" : "kvstats_blk_n_AR", "Type" : "RtlSignal"},
					{"Name" : "kvstats_blk_n_R", "Type" : "RtlSignal"},
					{"Name" : "kvstats_blk_n_AW", "Type" : "RtlSignal"},
					{"Name" : "kvstats_blk_n_W", "Type" : "RtlSignal"},
					{"Name" : "kvstats_blk_n_B", "Type" : "RtlSignal"}]},
			{"Name" : "kvstats_offset", "Type" : "None", "Direction" : "I"},
			{"Name" : "globalparams_reducecommand", "Type" : "None", "Direction" : "I"},
			{"Name" : "globalparams_treedepth", "Type" : "None", "Direction" : "I"},
			{"Name" : "globalparams_LLOPnumpartitions", "Type" : "None", "Direction" : "I"},
			{"Name" : "globalparams_vbegin", "Type" : "None", "Direction" : "I"}]},
	{"ID" : "1", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.v1_local0_V_U", "Parent" : "0"},
	{"ID" : "2", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.result_local0_V_U", "Parent" : "0"},
	{"ID" : "3", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.capsule0_key_U", "Parent" : "0"},
	{"ID" : "4", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.capsule0_value_U", "Parent" : "0"},
	{"ID" : "5", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.v1_local1_V_U", "Parent" : "0"},
	{"ID" : "6", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.result_local1_V_U", "Parent" : "0"},
	{"ID" : "7", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.capsule1_key_U", "Parent" : "0"},
	{"ID" : "8", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.capsule1_value_U", "Parent" : "0"},
	{"ID" : "9", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.v1_local2_V_U", "Parent" : "0"},
	{"ID" : "10", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.result_local2_V_U", "Parent" : "0"},
	{"ID" : "11", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.capsule2_key_U", "Parent" : "0"},
	{"ID" : "12", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.capsule2_value_U", "Parent" : "0"},
	{"ID" : "13", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.v1_local3_V_U", "Parent" : "0"},
	{"ID" : "14", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.result_local3_V_U", "Parent" : "0"},
	{"ID" : "15", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.capsule3_key_U", "Parent" : "0"},
	{"ID" : "16", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.capsule3_value_U", "Parent" : "0"},
	{"ID" : "17", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.v1_local4_V_U", "Parent" : "0"},
	{"ID" : "18", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.result_local4_V_U", "Parent" : "0"},
	{"ID" : "19", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.capsule4_key_U", "Parent" : "0"},
	{"ID" : "20", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.capsule4_value_U", "Parent" : "0"},
	{"ID" : "21", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.v1_local5_V_U", "Parent" : "0"},
	{"ID" : "22", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.result_local5_V_U", "Parent" : "0"},
	{"ID" : "23", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.capsule5_key_U", "Parent" : "0"},
	{"ID" : "24", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.capsule5_value_U", "Parent" : "0"},
	{"ID" : "25", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.v1_local6_V_U", "Parent" : "0"},
	{"ID" : "26", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.result_local6_V_U", "Parent" : "0"},
	{"ID" : "27", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.capsule6_key_U", "Parent" : "0"},
	{"ID" : "28", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.capsule6_value_U", "Parent" : "0"},
	{"ID" : "29", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.v1_local7_V_U", "Parent" : "0"},
	{"ID" : "30", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.result_local7_V_U", "Parent" : "0"},
	{"ID" : "31", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.capsule7_key_U", "Parent" : "0"},
	{"ID" : "32", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.capsule7_value_U", "Parent" : "0"},
	{"ID" : "33", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.kvdeststats_tmp_key_U", "Parent" : "0"},
	{"ID" : "34", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.kvdeststats_tmp_valu_U", "Parent" : "0"},
	{"ID" : "35", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.grp_getpartition_fu_4335", "Parent" : "0",
		"CDFG" : "getpartition",
		"Protocol" : "ap_ctrl_hs",
		"ControlExist" : "0", "ap_start" : "0", "ap_ready" : "1", "ap_done" : "0", "ap_continue" : "0", "ap_idle" : "0",
		"Pipeline" : "None", "UnalignedPipeline" : "0", "RewindPipeline" : "0", "ProcessNetwork" : "0",
		"II" : "1",
		"VariableLatency" : "0", "ExactLatency" : "0", "EstimateLatencyMin" : "0", "EstimateLatencyMax" : "0",
		"Combinational" : "1",
		"Datapath" : "0",
		"ClockEnable" : "0",
		"HasSubDataflow" : "0",
		"InDataflowNetwork" : "0",
		"HasNonBlockingOperation" : "0",
		"Port" : [
			{"Name" : "keyvalue_key", "Type" : "None", "Direction" : "I"},
			{"Name" : "currentLOP", "Type" : "None", "Direction" : "I"},
			{"Name" : "upperlimit", "Type" : "None", "Direction" : "I"}]},
	{"ID" : "36", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.grp_getpartition_fu_4343", "Parent" : "0",
		"CDFG" : "getpartition",
		"Protocol" : "ap_ctrl_hs",
		"ControlExist" : "0", "ap_start" : "0", "ap_ready" : "1", "ap_done" : "0", "ap_continue" : "0", "ap_idle" : "0",
		"Pipeline" : "None", "UnalignedPipeline" : "0", "RewindPipeline" : "0", "ProcessNetwork" : "0",
		"II" : "1",
		"VariableLatency" : "0", "ExactLatency" : "0", "EstimateLatencyMin" : "0", "EstimateLatencyMax" : "0",
		"Combinational" : "1",
		"Datapath" : "0",
		"ClockEnable" : "0",
		"HasSubDataflow" : "0",
		"InDataflowNetwork" : "0",
		"HasNonBlockingOperation" : "0",
		"Port" : [
			{"Name" : "keyvalue_key", "Type" : "None", "Direction" : "I"},
			{"Name" : "currentLOP", "Type" : "None", "Direction" : "I"},
			{"Name" : "upperlimit", "Type" : "None", "Direction" : "I"}]},
	{"ID" : "37", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.grp_getpartition_fu_4351", "Parent" : "0",
		"CDFG" : "getpartition",
		"Protocol" : "ap_ctrl_hs",
		"ControlExist" : "0", "ap_start" : "0", "ap_ready" : "1", "ap_done" : "0", "ap_continue" : "0", "ap_idle" : "0",
		"Pipeline" : "None", "UnalignedPipeline" : "0", "RewindPipeline" : "0", "ProcessNetwork" : "0",
		"II" : "1",
		"VariableLatency" : "0", "ExactLatency" : "0", "EstimateLatencyMin" : "0", "EstimateLatencyMax" : "0",
		"Combinational" : "1",
		"Datapath" : "0",
		"ClockEnable" : "0",
		"HasSubDataflow" : "0",
		"InDataflowNetwork" : "0",
		"HasNonBlockingOperation" : "0",
		"Port" : [
			{"Name" : "keyvalue_key", "Type" : "None", "Direction" : "I"},
			{"Name" : "currentLOP", "Type" : "None", "Direction" : "I"},
			{"Name" : "upperlimit", "Type" : "None", "Direction" : "I"}]},
	{"ID" : "38", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.grp_getpartition_fu_4359", "Parent" : "0",
		"CDFG" : "getpartition",
		"Protocol" : "ap_ctrl_hs",
		"ControlExist" : "0", "ap_start" : "0", "ap_ready" : "1", "ap_done" : "0", "ap_continue" : "0", "ap_idle" : "0",
		"Pipeline" : "None", "UnalignedPipeline" : "0", "RewindPipeline" : "0", "ProcessNetwork" : "0",
		"II" : "1",
		"VariableLatency" : "0", "ExactLatency" : "0", "EstimateLatencyMin" : "0", "EstimateLatencyMax" : "0",
		"Combinational" : "1",
		"Datapath" : "0",
		"ClockEnable" : "0",
		"HasSubDataflow" : "0",
		"InDataflowNetwork" : "0",
		"HasNonBlockingOperation" : "0",
		"Port" : [
			{"Name" : "keyvalue_key", "Type" : "None", "Direction" : "I"},
			{"Name" : "currentLOP", "Type" : "None", "Direction" : "I"},
			{"Name" : "upperlimit", "Type" : "None", "Direction" : "I"}]},
	{"ID" : "39", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.topkernel_urem_32ns_17ns_30_36_1_U4", "Parent" : "0"},
	{"ID" : "40", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.topkernel_urem_32ns_17ns_30_36_seq_1_U5", "Parent" : "0"},
	{"ID" : "41", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.topkernel_urem_32ns_17ns_30_36_1_U6", "Parent" : "0"},
	{"ID" : "42", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.topkernel_urem_32ns_17ns_30_36_seq_1_U7", "Parent" : "0"}]}


set ArgLastReadFirstWriteLatency {
	generatepartitions0 {
		kvsetdram_V {Type IO LastRead 170 FirstWrite 54}
		kvsetdram_V_offset {Type I LastRead 0 FirstWrite -1}
		kvstats {Type IO LastRead 98 FirstWrite 97}
		kvstats_offset {Type I LastRead 0 FirstWrite -1}
		globalparams_reducecommand {Type I LastRead 0 FirstWrite -1}
		globalparams_treedepth {Type I LastRead 0 FirstWrite -1}
		globalparams_LLOPnumpartitions {Type I LastRead 0 FirstWrite -1}
		globalparams_vbegin {Type I LastRead 0 FirstWrite -1}}
	getpartition {
		keyvalue_key {Type I LastRead 0 FirstWrite -1}
		currentLOP {Type I LastRead 0 FirstWrite -1}
		upperlimit {Type I LastRead 0 FirstWrite -1}}
	getpartition {
		keyvalue_key {Type I LastRead 0 FirstWrite -1}
		currentLOP {Type I LastRead 0 FirstWrite -1}
		upperlimit {Type I LastRead 0 FirstWrite -1}}
	getpartition {
		keyvalue_key {Type I LastRead 0 FirstWrite -1}
		currentLOP {Type I LastRead 0 FirstWrite -1}
		upperlimit {Type I LastRead 0 FirstWrite -1}}
	getpartition {
		keyvalue_key {Type I LastRead 0 FirstWrite -1}
		currentLOP {Type I LastRead 0 FirstWrite -1}
		upperlimit {Type I LastRead 0 FirstWrite -1}}}

set hasDtUnsupportedChannel 0

set PerformanceInfo {[
	{"Name" : "Latency", "Min" : "2", "Max" : "302088260"}
	, {"Name" : "Interval", "Min" : "2", "Max" : "302088260"}
]}

set PipelineEnableSignalInfo {[
	{"Pipeline" : "0", "EnableSignal" : "ap_enable_pp0"}
	{"Pipeline" : "3", "EnableSignal" : "ap_enable_pp3"}
	{"Pipeline" : "4", "EnableSignal" : "ap_enable_pp4"}
	{"Pipeline" : "5", "EnableSignal" : "ap_enable_pp5"}
	{"Pipeline" : "6", "EnableSignal" : "ap_enable_pp6"}
	{"Pipeline" : "7", "EnableSignal" : "ap_enable_pp7"}
	{"Pipeline" : "8", "EnableSignal" : "ap_enable_pp8"}
	{"Pipeline" : "9", "EnableSignal" : "ap_enable_pp9"}
	{"Pipeline" : "10", "EnableSignal" : "ap_enable_pp10"}
	{"Pipeline" : "11", "EnableSignal" : "ap_enable_pp11"}
	{"Pipeline" : "12", "EnableSignal" : "ap_enable_pp12"}
	{"Pipeline" : "13", "EnableSignal" : "ap_enable_pp13"}
	{"Pipeline" : "14", "EnableSignal" : "ap_enable_pp14"}
	{"Pipeline" : "15", "EnableSignal" : "ap_enable_pp15"}
	{"Pipeline" : "16", "EnableSignal" : "ap_enable_pp16"}
	{"Pipeline" : "17", "EnableSignal" : "ap_enable_pp17"}
	{"Pipeline" : "18", "EnableSignal" : "ap_enable_pp18"}
	{"Pipeline" : "19", "EnableSignal" : "ap_enable_pp19"}
	{"Pipeline" : "20", "EnableSignal" : "ap_enable_pp20"}
	{"Pipeline" : "21", "EnableSignal" : "ap_enable_pp21"}
	{"Pipeline" : "23", "EnableSignal" : "ap_enable_pp23"}
	{"Pipeline" : "24", "EnableSignal" : "ap_enable_pp24"}
	{"Pipeline" : "25", "EnableSignal" : "ap_enable_pp25"}
	{"Pipeline" : "26", "EnableSignal" : "ap_enable_pp26"}
	{"Pipeline" : "27", "EnableSignal" : "ap_enable_pp27"}
	{"Pipeline" : "28", "EnableSignal" : "ap_enable_pp28"}
	{"Pipeline" : "29", "EnableSignal" : "ap_enable_pp29"}
	{"Pipeline" : "30", "EnableSignal" : "ap_enable_pp30"}
	{"Pipeline" : "31", "EnableSignal" : "ap_enable_pp31"}
	{"Pipeline" : "32", "EnableSignal" : "ap_enable_pp32"}
	{"Pipeline" : "33", "EnableSignal" : "ap_enable_pp33"}
	{"Pipeline" : "34", "EnableSignal" : "ap_enable_pp34"}
	{"Pipeline" : "35", "EnableSignal" : "ap_enable_pp35"}
	{"Pipeline" : "36", "EnableSignal" : "ap_enable_pp36"}
	{"Pipeline" : "37", "EnableSignal" : "ap_enable_pp37"}
	{"Pipeline" : "38", "EnableSignal" : "ap_enable_pp38"}
	{"Pipeline" : "39", "EnableSignal" : "ap_enable_pp39"}
]}

set Spec2ImplPortList { 
	kvsetdram_V { m_axi {  { m_axi_kvsetdram_V_AWVALID VALID 1 1 }  { m_axi_kvsetdram_V_AWREADY READY 0 1 }  { m_axi_kvsetdram_V_AWADDR ADDR 1 32 }  { m_axi_kvsetdram_V_AWID ID 1 1 }  { m_axi_kvsetdram_V_AWLEN LEN 1 32 }  { m_axi_kvsetdram_V_AWSIZE SIZE 1 3 }  { m_axi_kvsetdram_V_AWBURST BURST 1 2 }  { m_axi_kvsetdram_V_AWLOCK LOCK 1 2 }  { m_axi_kvsetdram_V_AWCACHE CACHE 1 4 }  { m_axi_kvsetdram_V_AWPROT PROT 1 3 }  { m_axi_kvsetdram_V_AWQOS QOS 1 4 }  { m_axi_kvsetdram_V_AWREGION REGION 1 4 }  { m_axi_kvsetdram_V_AWUSER USER 1 1 }  { m_axi_kvsetdram_V_WVALID VALID 1 1 }  { m_axi_kvsetdram_V_WREADY READY 0 1 }  { m_axi_kvsetdram_V_WDATA DATA 1 512 }  { m_axi_kvsetdram_V_WSTRB STRB 1 64 }  { m_axi_kvsetdram_V_WLAST LAST 1 1 }  { m_axi_kvsetdram_V_WID ID 1 1 }  { m_axi_kvsetdram_V_WUSER USER 1 1 }  { m_axi_kvsetdram_V_ARVALID VALID 1 1 }  { m_axi_kvsetdram_V_ARREADY READY 0 1 }  { m_axi_kvsetdram_V_ARADDR ADDR 1 32 }  { m_axi_kvsetdram_V_ARID ID 1 1 }  { m_axi_kvsetdram_V_ARLEN LEN 1 32 }  { m_axi_kvsetdram_V_ARSIZE SIZE 1 3 }  { m_axi_kvsetdram_V_ARBURST BURST 1 2 }  { m_axi_kvsetdram_V_ARLOCK LOCK 1 2 }  { m_axi_kvsetdram_V_ARCACHE CACHE 1 4 }  { m_axi_kvsetdram_V_ARPROT PROT 1 3 }  { m_axi_kvsetdram_V_ARQOS QOS 1 4 }  { m_axi_kvsetdram_V_ARREGION REGION 1 4 }  { m_axi_kvsetdram_V_ARUSER USER 1 1 }  { m_axi_kvsetdram_V_RVALID VALID 0 1 }  { m_axi_kvsetdram_V_RREADY READY 1 1 }  { m_axi_kvsetdram_V_RDATA DATA 0 512 }  { m_axi_kvsetdram_V_RLAST LAST 0 1 }  { m_axi_kvsetdram_V_RID ID 0 1 }  { m_axi_kvsetdram_V_RUSER USER 0 1 }  { m_axi_kvsetdram_V_RRESP RESP 0 2 }  { m_axi_kvsetdram_V_BVALID VALID 0 1 }  { m_axi_kvsetdram_V_BREADY READY 1 1 }  { m_axi_kvsetdram_V_BRESP RESP 0 2 }  { m_axi_kvsetdram_V_BID ID 0 1 }  { m_axi_kvsetdram_V_BUSER USER 0 1 } } }
	kvsetdram_V_offset { ap_none {  { kvsetdram_V_offset in_data 0 26 } } }
	kvstats { m_axi {  { m_axi_kvstats_AWVALID VALID 1 1 }  { m_axi_kvstats_AWREADY READY 0 1 }  { m_axi_kvstats_AWADDR ADDR 1 32 }  { m_axi_kvstats_AWID ID 1 1 }  { m_axi_kvstats_AWLEN LEN 1 32 }  { m_axi_kvstats_AWSIZE SIZE 1 3 }  { m_axi_kvstats_AWBURST BURST 1 2 }  { m_axi_kvstats_AWLOCK LOCK 1 2 }  { m_axi_kvstats_AWCACHE CACHE 1 4 }  { m_axi_kvstats_AWPROT PROT 1 3 }  { m_axi_kvstats_AWQOS QOS 1 4 }  { m_axi_kvstats_AWREGION REGION 1 4 }  { m_axi_kvstats_AWUSER USER 1 1 }  { m_axi_kvstats_WVALID VALID 1 1 }  { m_axi_kvstats_WREADY READY 0 1 }  { m_axi_kvstats_WDATA DATA 1 64 }  { m_axi_kvstats_WSTRB STRB 1 8 }  { m_axi_kvstats_WLAST LAST 1 1 }  { m_axi_kvstats_WID ID 1 1 }  { m_axi_kvstats_WUSER USER 1 1 }  { m_axi_kvstats_ARVALID VALID 1 1 }  { m_axi_kvstats_ARREADY READY 0 1 }  { m_axi_kvstats_ARADDR ADDR 1 32 }  { m_axi_kvstats_ARID ID 1 1 }  { m_axi_kvstats_ARLEN LEN 1 32 }  { m_axi_kvstats_ARSIZE SIZE 1 3 }  { m_axi_kvstats_ARBURST BURST 1 2 }  { m_axi_kvstats_ARLOCK LOCK 1 2 }  { m_axi_kvstats_ARCACHE CACHE 1 4 }  { m_axi_kvstats_ARPROT PROT 1 3 }  { m_axi_kvstats_ARQOS QOS 1 4 }  { m_axi_kvstats_ARREGION REGION 1 4 }  { m_axi_kvstats_ARUSER USER 1 1 }  { m_axi_kvstats_RVALID VALID 0 1 }  { m_axi_kvstats_RREADY READY 1 1 }  { m_axi_kvstats_RDATA DATA 0 64 }  { m_axi_kvstats_RLAST LAST 0 1 }  { m_axi_kvstats_RID ID 0 1 }  { m_axi_kvstats_RUSER USER 0 1 }  { m_axi_kvstats_RRESP RESP 0 2 }  { m_axi_kvstats_BVALID VALID 0 1 }  { m_axi_kvstats_BREADY READY 1 1 }  { m_axi_kvstats_BRESP RESP 0 2 }  { m_axi_kvstats_BID ID 0 1 }  { m_axi_kvstats_BUSER USER 0 1 } } }
	kvstats_offset { ap_none {  { kvstats_offset in_data 0 29 } } }
	globalparams_reducecommand { ap_none {  { globalparams_reducecommand in_data 0 32 } } }
	globalparams_treedepth { ap_none {  { globalparams_treedepth in_data 0 32 } } }
	globalparams_LLOPnumpartitions { ap_none {  { globalparams_LLOPnumpartitions in_data 0 32 } } }
	globalparams_vbegin { ap_none {  { globalparams_vbegin in_data 0 32 } } }
}
