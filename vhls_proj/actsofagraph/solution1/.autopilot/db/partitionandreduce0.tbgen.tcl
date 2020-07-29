set moduleName partitionandreduce0
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
set C_modelName {partitionandreduce0}
set C_modelType { void 0 }
set C_modelArgList {
	{ kvsourcedram_V int 512 regular {axi_master 2}  }
	{ kvsourcedram_V_offset int 26 regular  }
	{ kvdestdram_V int 512 regular {axi_master 2}  }
	{ kvdestdram_V_offset int 26 regular  }
	{ kvstats int 64 regular {axi_master 2}  }
	{ kvstats_offset int 29 regular  }
	{ globalparams_runkernelcommand int 32 regular  }
	{ globalparams_partitioncommand int 32 regular  }
	{ globalparams_reducecommand int 32 regular  }
	{ globalparams_treedepth int 32 regular  }
	{ globalparams_runsize int 32 regular  }
	{ globalparams_vbegin int 32 regular  }
	{ globalparams_statsalreadycollected int 32 regular  }
}
set C_modelArgMapList {[ 
	{ "Name" : "kvsourcedram_V", "interface" : "axi_master", "bitwidth" : 512, "direction" : "READWRITE"} , 
 	{ "Name" : "kvsourcedram_V_offset", "interface" : "wire", "bitwidth" : 26, "direction" : "READONLY"} , 
 	{ "Name" : "kvdestdram_V", "interface" : "axi_master", "bitwidth" : 512, "direction" : "READWRITE"} , 
 	{ "Name" : "kvdestdram_V_offset", "interface" : "wire", "bitwidth" : 26, "direction" : "READONLY"} , 
 	{ "Name" : "kvstats", "interface" : "axi_master", "bitwidth" : 64, "direction" : "READWRITE"} , 
 	{ "Name" : "kvstats_offset", "interface" : "wire", "bitwidth" : 29, "direction" : "READONLY"} , 
 	{ "Name" : "globalparams_runkernelcommand", "interface" : "wire", "bitwidth" : 32, "direction" : "READONLY"} , 
 	{ "Name" : "globalparams_partitioncommand", "interface" : "wire", "bitwidth" : 32, "direction" : "READONLY"} , 
 	{ "Name" : "globalparams_reducecommand", "interface" : "wire", "bitwidth" : 32, "direction" : "READONLY"} , 
 	{ "Name" : "globalparams_treedepth", "interface" : "wire", "bitwidth" : 32, "direction" : "READONLY"} , 
 	{ "Name" : "globalparams_runsize", "interface" : "wire", "bitwidth" : 32, "direction" : "READONLY"} , 
 	{ "Name" : "globalparams_vbegin", "interface" : "wire", "bitwidth" : 32, "direction" : "READONLY"} , 
 	{ "Name" : "globalparams_statsalreadycollected", "interface" : "wire", "bitwidth" : 32, "direction" : "READONLY"} ]}
# RTL Port declarations: 
set portNum 151
set portList { 
	{ ap_clk sc_in sc_logic 1 clock -1 } 
	{ ap_rst sc_in sc_logic 1 reset -1 active_high_sync } 
	{ ap_start sc_in sc_logic 1 start -1 } 
	{ ap_done sc_out sc_logic 1 predone -1 } 
	{ ap_idle sc_out sc_logic 1 done -1 } 
	{ ap_ready sc_out sc_logic 1 ready -1 } 
	{ m_axi_kvsourcedram_V_AWVALID sc_out sc_logic 1 signal 0 } 
	{ m_axi_kvsourcedram_V_AWREADY sc_in sc_logic 1 signal 0 } 
	{ m_axi_kvsourcedram_V_AWADDR sc_out sc_lv 32 signal 0 } 
	{ m_axi_kvsourcedram_V_AWID sc_out sc_lv 1 signal 0 } 
	{ m_axi_kvsourcedram_V_AWLEN sc_out sc_lv 32 signal 0 } 
	{ m_axi_kvsourcedram_V_AWSIZE sc_out sc_lv 3 signal 0 } 
	{ m_axi_kvsourcedram_V_AWBURST sc_out sc_lv 2 signal 0 } 
	{ m_axi_kvsourcedram_V_AWLOCK sc_out sc_lv 2 signal 0 } 
	{ m_axi_kvsourcedram_V_AWCACHE sc_out sc_lv 4 signal 0 } 
	{ m_axi_kvsourcedram_V_AWPROT sc_out sc_lv 3 signal 0 } 
	{ m_axi_kvsourcedram_V_AWQOS sc_out sc_lv 4 signal 0 } 
	{ m_axi_kvsourcedram_V_AWREGION sc_out sc_lv 4 signal 0 } 
	{ m_axi_kvsourcedram_V_AWUSER sc_out sc_lv 1 signal 0 } 
	{ m_axi_kvsourcedram_V_WVALID sc_out sc_logic 1 signal 0 } 
	{ m_axi_kvsourcedram_V_WREADY sc_in sc_logic 1 signal 0 } 
	{ m_axi_kvsourcedram_V_WDATA sc_out sc_lv 512 signal 0 } 
	{ m_axi_kvsourcedram_V_WSTRB sc_out sc_lv 64 signal 0 } 
	{ m_axi_kvsourcedram_V_WLAST sc_out sc_logic 1 signal 0 } 
	{ m_axi_kvsourcedram_V_WID sc_out sc_lv 1 signal 0 } 
	{ m_axi_kvsourcedram_V_WUSER sc_out sc_lv 1 signal 0 } 
	{ m_axi_kvsourcedram_V_ARVALID sc_out sc_logic 1 signal 0 } 
	{ m_axi_kvsourcedram_V_ARREADY sc_in sc_logic 1 signal 0 } 
	{ m_axi_kvsourcedram_V_ARADDR sc_out sc_lv 32 signal 0 } 
	{ m_axi_kvsourcedram_V_ARID sc_out sc_lv 1 signal 0 } 
	{ m_axi_kvsourcedram_V_ARLEN sc_out sc_lv 32 signal 0 } 
	{ m_axi_kvsourcedram_V_ARSIZE sc_out sc_lv 3 signal 0 } 
	{ m_axi_kvsourcedram_V_ARBURST sc_out sc_lv 2 signal 0 } 
	{ m_axi_kvsourcedram_V_ARLOCK sc_out sc_lv 2 signal 0 } 
	{ m_axi_kvsourcedram_V_ARCACHE sc_out sc_lv 4 signal 0 } 
	{ m_axi_kvsourcedram_V_ARPROT sc_out sc_lv 3 signal 0 } 
	{ m_axi_kvsourcedram_V_ARQOS sc_out sc_lv 4 signal 0 } 
	{ m_axi_kvsourcedram_V_ARREGION sc_out sc_lv 4 signal 0 } 
	{ m_axi_kvsourcedram_V_ARUSER sc_out sc_lv 1 signal 0 } 
	{ m_axi_kvsourcedram_V_RVALID sc_in sc_logic 1 signal 0 } 
	{ m_axi_kvsourcedram_V_RREADY sc_out sc_logic 1 signal 0 } 
	{ m_axi_kvsourcedram_V_RDATA sc_in sc_lv 512 signal 0 } 
	{ m_axi_kvsourcedram_V_RLAST sc_in sc_logic 1 signal 0 } 
	{ m_axi_kvsourcedram_V_RID sc_in sc_lv 1 signal 0 } 
	{ m_axi_kvsourcedram_V_RUSER sc_in sc_lv 1 signal 0 } 
	{ m_axi_kvsourcedram_V_RRESP sc_in sc_lv 2 signal 0 } 
	{ m_axi_kvsourcedram_V_BVALID sc_in sc_logic 1 signal 0 } 
	{ m_axi_kvsourcedram_V_BREADY sc_out sc_logic 1 signal 0 } 
	{ m_axi_kvsourcedram_V_BRESP sc_in sc_lv 2 signal 0 } 
	{ m_axi_kvsourcedram_V_BID sc_in sc_lv 1 signal 0 } 
	{ m_axi_kvsourcedram_V_BUSER sc_in sc_lv 1 signal 0 } 
	{ kvsourcedram_V_offset sc_in sc_lv 26 signal 1 } 
	{ m_axi_kvdestdram_V_AWVALID sc_out sc_logic 1 signal 2 } 
	{ m_axi_kvdestdram_V_AWREADY sc_in sc_logic 1 signal 2 } 
	{ m_axi_kvdestdram_V_AWADDR sc_out sc_lv 32 signal 2 } 
	{ m_axi_kvdestdram_V_AWID sc_out sc_lv 1 signal 2 } 
	{ m_axi_kvdestdram_V_AWLEN sc_out sc_lv 32 signal 2 } 
	{ m_axi_kvdestdram_V_AWSIZE sc_out sc_lv 3 signal 2 } 
	{ m_axi_kvdestdram_V_AWBURST sc_out sc_lv 2 signal 2 } 
	{ m_axi_kvdestdram_V_AWLOCK sc_out sc_lv 2 signal 2 } 
	{ m_axi_kvdestdram_V_AWCACHE sc_out sc_lv 4 signal 2 } 
	{ m_axi_kvdestdram_V_AWPROT sc_out sc_lv 3 signal 2 } 
	{ m_axi_kvdestdram_V_AWQOS sc_out sc_lv 4 signal 2 } 
	{ m_axi_kvdestdram_V_AWREGION sc_out sc_lv 4 signal 2 } 
	{ m_axi_kvdestdram_V_AWUSER sc_out sc_lv 1 signal 2 } 
	{ m_axi_kvdestdram_V_WVALID sc_out sc_logic 1 signal 2 } 
	{ m_axi_kvdestdram_V_WREADY sc_in sc_logic 1 signal 2 } 
	{ m_axi_kvdestdram_V_WDATA sc_out sc_lv 512 signal 2 } 
	{ m_axi_kvdestdram_V_WSTRB sc_out sc_lv 64 signal 2 } 
	{ m_axi_kvdestdram_V_WLAST sc_out sc_logic 1 signal 2 } 
	{ m_axi_kvdestdram_V_WID sc_out sc_lv 1 signal 2 } 
	{ m_axi_kvdestdram_V_WUSER sc_out sc_lv 1 signal 2 } 
	{ m_axi_kvdestdram_V_ARVALID sc_out sc_logic 1 signal 2 } 
	{ m_axi_kvdestdram_V_ARREADY sc_in sc_logic 1 signal 2 } 
	{ m_axi_kvdestdram_V_ARADDR sc_out sc_lv 32 signal 2 } 
	{ m_axi_kvdestdram_V_ARID sc_out sc_lv 1 signal 2 } 
	{ m_axi_kvdestdram_V_ARLEN sc_out sc_lv 32 signal 2 } 
	{ m_axi_kvdestdram_V_ARSIZE sc_out sc_lv 3 signal 2 } 
	{ m_axi_kvdestdram_V_ARBURST sc_out sc_lv 2 signal 2 } 
	{ m_axi_kvdestdram_V_ARLOCK sc_out sc_lv 2 signal 2 } 
	{ m_axi_kvdestdram_V_ARCACHE sc_out sc_lv 4 signal 2 } 
	{ m_axi_kvdestdram_V_ARPROT sc_out sc_lv 3 signal 2 } 
	{ m_axi_kvdestdram_V_ARQOS sc_out sc_lv 4 signal 2 } 
	{ m_axi_kvdestdram_V_ARREGION sc_out sc_lv 4 signal 2 } 
	{ m_axi_kvdestdram_V_ARUSER sc_out sc_lv 1 signal 2 } 
	{ m_axi_kvdestdram_V_RVALID sc_in sc_logic 1 signal 2 } 
	{ m_axi_kvdestdram_V_RREADY sc_out sc_logic 1 signal 2 } 
	{ m_axi_kvdestdram_V_RDATA sc_in sc_lv 512 signal 2 } 
	{ m_axi_kvdestdram_V_RLAST sc_in sc_logic 1 signal 2 } 
	{ m_axi_kvdestdram_V_RID sc_in sc_lv 1 signal 2 } 
	{ m_axi_kvdestdram_V_RUSER sc_in sc_lv 1 signal 2 } 
	{ m_axi_kvdestdram_V_RRESP sc_in sc_lv 2 signal 2 } 
	{ m_axi_kvdestdram_V_BVALID sc_in sc_logic 1 signal 2 } 
	{ m_axi_kvdestdram_V_BREADY sc_out sc_logic 1 signal 2 } 
	{ m_axi_kvdestdram_V_BRESP sc_in sc_lv 2 signal 2 } 
	{ m_axi_kvdestdram_V_BID sc_in sc_lv 1 signal 2 } 
	{ m_axi_kvdestdram_V_BUSER sc_in sc_lv 1 signal 2 } 
	{ kvdestdram_V_offset sc_in sc_lv 26 signal 3 } 
	{ m_axi_kvstats_AWVALID sc_out sc_logic 1 signal 4 } 
	{ m_axi_kvstats_AWREADY sc_in sc_logic 1 signal 4 } 
	{ m_axi_kvstats_AWADDR sc_out sc_lv 32 signal 4 } 
	{ m_axi_kvstats_AWID sc_out sc_lv 1 signal 4 } 
	{ m_axi_kvstats_AWLEN sc_out sc_lv 32 signal 4 } 
	{ m_axi_kvstats_AWSIZE sc_out sc_lv 3 signal 4 } 
	{ m_axi_kvstats_AWBURST sc_out sc_lv 2 signal 4 } 
	{ m_axi_kvstats_AWLOCK sc_out sc_lv 2 signal 4 } 
	{ m_axi_kvstats_AWCACHE sc_out sc_lv 4 signal 4 } 
	{ m_axi_kvstats_AWPROT sc_out sc_lv 3 signal 4 } 
	{ m_axi_kvstats_AWQOS sc_out sc_lv 4 signal 4 } 
	{ m_axi_kvstats_AWREGION sc_out sc_lv 4 signal 4 } 
	{ m_axi_kvstats_AWUSER sc_out sc_lv 1 signal 4 } 
	{ m_axi_kvstats_WVALID sc_out sc_logic 1 signal 4 } 
	{ m_axi_kvstats_WREADY sc_in sc_logic 1 signal 4 } 
	{ m_axi_kvstats_WDATA sc_out sc_lv 64 signal 4 } 
	{ m_axi_kvstats_WSTRB sc_out sc_lv 8 signal 4 } 
	{ m_axi_kvstats_WLAST sc_out sc_logic 1 signal 4 } 
	{ m_axi_kvstats_WID sc_out sc_lv 1 signal 4 } 
	{ m_axi_kvstats_WUSER sc_out sc_lv 1 signal 4 } 
	{ m_axi_kvstats_ARVALID sc_out sc_logic 1 signal 4 } 
	{ m_axi_kvstats_ARREADY sc_in sc_logic 1 signal 4 } 
	{ m_axi_kvstats_ARADDR sc_out sc_lv 32 signal 4 } 
	{ m_axi_kvstats_ARID sc_out sc_lv 1 signal 4 } 
	{ m_axi_kvstats_ARLEN sc_out sc_lv 32 signal 4 } 
	{ m_axi_kvstats_ARSIZE sc_out sc_lv 3 signal 4 } 
	{ m_axi_kvstats_ARBURST sc_out sc_lv 2 signal 4 } 
	{ m_axi_kvstats_ARLOCK sc_out sc_lv 2 signal 4 } 
	{ m_axi_kvstats_ARCACHE sc_out sc_lv 4 signal 4 } 
	{ m_axi_kvstats_ARPROT sc_out sc_lv 3 signal 4 } 
	{ m_axi_kvstats_ARQOS sc_out sc_lv 4 signal 4 } 
	{ m_axi_kvstats_ARREGION sc_out sc_lv 4 signal 4 } 
	{ m_axi_kvstats_ARUSER sc_out sc_lv 1 signal 4 } 
	{ m_axi_kvstats_RVALID sc_in sc_logic 1 signal 4 } 
	{ m_axi_kvstats_RREADY sc_out sc_logic 1 signal 4 } 
	{ m_axi_kvstats_RDATA sc_in sc_lv 64 signal 4 } 
	{ m_axi_kvstats_RLAST sc_in sc_logic 1 signal 4 } 
	{ m_axi_kvstats_RID sc_in sc_lv 1 signal 4 } 
	{ m_axi_kvstats_RUSER sc_in sc_lv 1 signal 4 } 
	{ m_axi_kvstats_RRESP sc_in sc_lv 2 signal 4 } 
	{ m_axi_kvstats_BVALID sc_in sc_logic 1 signal 4 } 
	{ m_axi_kvstats_BREADY sc_out sc_logic 1 signal 4 } 
	{ m_axi_kvstats_BRESP sc_in sc_lv 2 signal 4 } 
	{ m_axi_kvstats_BID sc_in sc_lv 1 signal 4 } 
	{ m_axi_kvstats_BUSER sc_in sc_lv 1 signal 4 } 
	{ kvstats_offset sc_in sc_lv 29 signal 5 } 
	{ globalparams_runkernelcommand sc_in sc_lv 32 signal 6 } 
	{ globalparams_partitioncommand sc_in sc_lv 32 signal 7 } 
	{ globalparams_reducecommand sc_in sc_lv 32 signal 8 } 
	{ globalparams_treedepth sc_in sc_lv 32 signal 9 } 
	{ globalparams_runsize sc_in sc_lv 32 signal 10 } 
	{ globalparams_vbegin sc_in sc_lv 32 signal 11 } 
	{ globalparams_statsalreadycollected sc_in sc_lv 32 signal 12 } 
}
set NewPortList {[ 
	{ "name": "ap_clk", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "clock", "bundle":{"name": "ap_clk", "role": "default" }} , 
 	{ "name": "ap_rst", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "reset", "bundle":{"name": "ap_rst", "role": "default" }} , 
 	{ "name": "ap_start", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "start", "bundle":{"name": "ap_start", "role": "default" }} , 
 	{ "name": "ap_done", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "predone", "bundle":{"name": "ap_done", "role": "default" }} , 
 	{ "name": "ap_idle", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "done", "bundle":{"name": "ap_idle", "role": "default" }} , 
 	{ "name": "ap_ready", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "ready", "bundle":{"name": "ap_ready", "role": "default" }} , 
 	{ "name": "m_axi_kvsourcedram_V_AWVALID", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "kvsourcedram_V", "role": "AWVALID" }} , 
 	{ "name": "m_axi_kvsourcedram_V_AWREADY", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "kvsourcedram_V", "role": "AWREADY" }} , 
 	{ "name": "m_axi_kvsourcedram_V_AWADDR", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "kvsourcedram_V", "role": "AWADDR" }} , 
 	{ "name": "m_axi_kvsourcedram_V_AWID", "direction": "out", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "kvsourcedram_V", "role": "AWID" }} , 
 	{ "name": "m_axi_kvsourcedram_V_AWLEN", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "kvsourcedram_V", "role": "AWLEN" }} , 
 	{ "name": "m_axi_kvsourcedram_V_AWSIZE", "direction": "out", "datatype": "sc_lv", "bitwidth":3, "type": "signal", "bundle":{"name": "kvsourcedram_V", "role": "AWSIZE" }} , 
 	{ "name": "m_axi_kvsourcedram_V_AWBURST", "direction": "out", "datatype": "sc_lv", "bitwidth":2, "type": "signal", "bundle":{"name": "kvsourcedram_V", "role": "AWBURST" }} , 
 	{ "name": "m_axi_kvsourcedram_V_AWLOCK", "direction": "out", "datatype": "sc_lv", "bitwidth":2, "type": "signal", "bundle":{"name": "kvsourcedram_V", "role": "AWLOCK" }} , 
 	{ "name": "m_axi_kvsourcedram_V_AWCACHE", "direction": "out", "datatype": "sc_lv", "bitwidth":4, "type": "signal", "bundle":{"name": "kvsourcedram_V", "role": "AWCACHE" }} , 
 	{ "name": "m_axi_kvsourcedram_V_AWPROT", "direction": "out", "datatype": "sc_lv", "bitwidth":3, "type": "signal", "bundle":{"name": "kvsourcedram_V", "role": "AWPROT" }} , 
 	{ "name": "m_axi_kvsourcedram_V_AWQOS", "direction": "out", "datatype": "sc_lv", "bitwidth":4, "type": "signal", "bundle":{"name": "kvsourcedram_V", "role": "AWQOS" }} , 
 	{ "name": "m_axi_kvsourcedram_V_AWREGION", "direction": "out", "datatype": "sc_lv", "bitwidth":4, "type": "signal", "bundle":{"name": "kvsourcedram_V", "role": "AWREGION" }} , 
 	{ "name": "m_axi_kvsourcedram_V_AWUSER", "direction": "out", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "kvsourcedram_V", "role": "AWUSER" }} , 
 	{ "name": "m_axi_kvsourcedram_V_WVALID", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "kvsourcedram_V", "role": "WVALID" }} , 
 	{ "name": "m_axi_kvsourcedram_V_WREADY", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "kvsourcedram_V", "role": "WREADY" }} , 
 	{ "name": "m_axi_kvsourcedram_V_WDATA", "direction": "out", "datatype": "sc_lv", "bitwidth":512, "type": "signal", "bundle":{"name": "kvsourcedram_V", "role": "WDATA" }} , 
 	{ "name": "m_axi_kvsourcedram_V_WSTRB", "direction": "out", "datatype": "sc_lv", "bitwidth":64, "type": "signal", "bundle":{"name": "kvsourcedram_V", "role": "WSTRB" }} , 
 	{ "name": "m_axi_kvsourcedram_V_WLAST", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "kvsourcedram_V", "role": "WLAST" }} , 
 	{ "name": "m_axi_kvsourcedram_V_WID", "direction": "out", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "kvsourcedram_V", "role": "WID" }} , 
 	{ "name": "m_axi_kvsourcedram_V_WUSER", "direction": "out", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "kvsourcedram_V", "role": "WUSER" }} , 
 	{ "name": "m_axi_kvsourcedram_V_ARVALID", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "kvsourcedram_V", "role": "ARVALID" }} , 
 	{ "name": "m_axi_kvsourcedram_V_ARREADY", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "kvsourcedram_V", "role": "ARREADY" }} , 
 	{ "name": "m_axi_kvsourcedram_V_ARADDR", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "kvsourcedram_V", "role": "ARADDR" }} , 
 	{ "name": "m_axi_kvsourcedram_V_ARID", "direction": "out", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "kvsourcedram_V", "role": "ARID" }} , 
 	{ "name": "m_axi_kvsourcedram_V_ARLEN", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "kvsourcedram_V", "role": "ARLEN" }} , 
 	{ "name": "m_axi_kvsourcedram_V_ARSIZE", "direction": "out", "datatype": "sc_lv", "bitwidth":3, "type": "signal", "bundle":{"name": "kvsourcedram_V", "role": "ARSIZE" }} , 
 	{ "name": "m_axi_kvsourcedram_V_ARBURST", "direction": "out", "datatype": "sc_lv", "bitwidth":2, "type": "signal", "bundle":{"name": "kvsourcedram_V", "role": "ARBURST" }} , 
 	{ "name": "m_axi_kvsourcedram_V_ARLOCK", "direction": "out", "datatype": "sc_lv", "bitwidth":2, "type": "signal", "bundle":{"name": "kvsourcedram_V", "role": "ARLOCK" }} , 
 	{ "name": "m_axi_kvsourcedram_V_ARCACHE", "direction": "out", "datatype": "sc_lv", "bitwidth":4, "type": "signal", "bundle":{"name": "kvsourcedram_V", "role": "ARCACHE" }} , 
 	{ "name": "m_axi_kvsourcedram_V_ARPROT", "direction": "out", "datatype": "sc_lv", "bitwidth":3, "type": "signal", "bundle":{"name": "kvsourcedram_V", "role": "ARPROT" }} , 
 	{ "name": "m_axi_kvsourcedram_V_ARQOS", "direction": "out", "datatype": "sc_lv", "bitwidth":4, "type": "signal", "bundle":{"name": "kvsourcedram_V", "role": "ARQOS" }} , 
 	{ "name": "m_axi_kvsourcedram_V_ARREGION", "direction": "out", "datatype": "sc_lv", "bitwidth":4, "type": "signal", "bundle":{"name": "kvsourcedram_V", "role": "ARREGION" }} , 
 	{ "name": "m_axi_kvsourcedram_V_ARUSER", "direction": "out", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "kvsourcedram_V", "role": "ARUSER" }} , 
 	{ "name": "m_axi_kvsourcedram_V_RVALID", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "kvsourcedram_V", "role": "RVALID" }} , 
 	{ "name": "m_axi_kvsourcedram_V_RREADY", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "kvsourcedram_V", "role": "RREADY" }} , 
 	{ "name": "m_axi_kvsourcedram_V_RDATA", "direction": "in", "datatype": "sc_lv", "bitwidth":512, "type": "signal", "bundle":{"name": "kvsourcedram_V", "role": "RDATA" }} , 
 	{ "name": "m_axi_kvsourcedram_V_RLAST", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "kvsourcedram_V", "role": "RLAST" }} , 
 	{ "name": "m_axi_kvsourcedram_V_RID", "direction": "in", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "kvsourcedram_V", "role": "RID" }} , 
 	{ "name": "m_axi_kvsourcedram_V_RUSER", "direction": "in", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "kvsourcedram_V", "role": "RUSER" }} , 
 	{ "name": "m_axi_kvsourcedram_V_RRESP", "direction": "in", "datatype": "sc_lv", "bitwidth":2, "type": "signal", "bundle":{"name": "kvsourcedram_V", "role": "RRESP" }} , 
 	{ "name": "m_axi_kvsourcedram_V_BVALID", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "kvsourcedram_V", "role": "BVALID" }} , 
 	{ "name": "m_axi_kvsourcedram_V_BREADY", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "kvsourcedram_V", "role": "BREADY" }} , 
 	{ "name": "m_axi_kvsourcedram_V_BRESP", "direction": "in", "datatype": "sc_lv", "bitwidth":2, "type": "signal", "bundle":{"name": "kvsourcedram_V", "role": "BRESP" }} , 
 	{ "name": "m_axi_kvsourcedram_V_BID", "direction": "in", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "kvsourcedram_V", "role": "BID" }} , 
 	{ "name": "m_axi_kvsourcedram_V_BUSER", "direction": "in", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "kvsourcedram_V", "role": "BUSER" }} , 
 	{ "name": "kvsourcedram_V_offset", "direction": "in", "datatype": "sc_lv", "bitwidth":26, "type": "signal", "bundle":{"name": "kvsourcedram_V_offset", "role": "default" }} , 
 	{ "name": "m_axi_kvdestdram_V_AWVALID", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "kvdestdram_V", "role": "AWVALID" }} , 
 	{ "name": "m_axi_kvdestdram_V_AWREADY", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "kvdestdram_V", "role": "AWREADY" }} , 
 	{ "name": "m_axi_kvdestdram_V_AWADDR", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "kvdestdram_V", "role": "AWADDR" }} , 
 	{ "name": "m_axi_kvdestdram_V_AWID", "direction": "out", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "kvdestdram_V", "role": "AWID" }} , 
 	{ "name": "m_axi_kvdestdram_V_AWLEN", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "kvdestdram_V", "role": "AWLEN" }} , 
 	{ "name": "m_axi_kvdestdram_V_AWSIZE", "direction": "out", "datatype": "sc_lv", "bitwidth":3, "type": "signal", "bundle":{"name": "kvdestdram_V", "role": "AWSIZE" }} , 
 	{ "name": "m_axi_kvdestdram_V_AWBURST", "direction": "out", "datatype": "sc_lv", "bitwidth":2, "type": "signal", "bundle":{"name": "kvdestdram_V", "role": "AWBURST" }} , 
 	{ "name": "m_axi_kvdestdram_V_AWLOCK", "direction": "out", "datatype": "sc_lv", "bitwidth":2, "type": "signal", "bundle":{"name": "kvdestdram_V", "role": "AWLOCK" }} , 
 	{ "name": "m_axi_kvdestdram_V_AWCACHE", "direction": "out", "datatype": "sc_lv", "bitwidth":4, "type": "signal", "bundle":{"name": "kvdestdram_V", "role": "AWCACHE" }} , 
 	{ "name": "m_axi_kvdestdram_V_AWPROT", "direction": "out", "datatype": "sc_lv", "bitwidth":3, "type": "signal", "bundle":{"name": "kvdestdram_V", "role": "AWPROT" }} , 
 	{ "name": "m_axi_kvdestdram_V_AWQOS", "direction": "out", "datatype": "sc_lv", "bitwidth":4, "type": "signal", "bundle":{"name": "kvdestdram_V", "role": "AWQOS" }} , 
 	{ "name": "m_axi_kvdestdram_V_AWREGION", "direction": "out", "datatype": "sc_lv", "bitwidth":4, "type": "signal", "bundle":{"name": "kvdestdram_V", "role": "AWREGION" }} , 
 	{ "name": "m_axi_kvdestdram_V_AWUSER", "direction": "out", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "kvdestdram_V", "role": "AWUSER" }} , 
 	{ "name": "m_axi_kvdestdram_V_WVALID", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "kvdestdram_V", "role": "WVALID" }} , 
 	{ "name": "m_axi_kvdestdram_V_WREADY", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "kvdestdram_V", "role": "WREADY" }} , 
 	{ "name": "m_axi_kvdestdram_V_WDATA", "direction": "out", "datatype": "sc_lv", "bitwidth":512, "type": "signal", "bundle":{"name": "kvdestdram_V", "role": "WDATA" }} , 
 	{ "name": "m_axi_kvdestdram_V_WSTRB", "direction": "out", "datatype": "sc_lv", "bitwidth":64, "type": "signal", "bundle":{"name": "kvdestdram_V", "role": "WSTRB" }} , 
 	{ "name": "m_axi_kvdestdram_V_WLAST", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "kvdestdram_V", "role": "WLAST" }} , 
 	{ "name": "m_axi_kvdestdram_V_WID", "direction": "out", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "kvdestdram_V", "role": "WID" }} , 
 	{ "name": "m_axi_kvdestdram_V_WUSER", "direction": "out", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "kvdestdram_V", "role": "WUSER" }} , 
 	{ "name": "m_axi_kvdestdram_V_ARVALID", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "kvdestdram_V", "role": "ARVALID" }} , 
 	{ "name": "m_axi_kvdestdram_V_ARREADY", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "kvdestdram_V", "role": "ARREADY" }} , 
 	{ "name": "m_axi_kvdestdram_V_ARADDR", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "kvdestdram_V", "role": "ARADDR" }} , 
 	{ "name": "m_axi_kvdestdram_V_ARID", "direction": "out", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "kvdestdram_V", "role": "ARID" }} , 
 	{ "name": "m_axi_kvdestdram_V_ARLEN", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "kvdestdram_V", "role": "ARLEN" }} , 
 	{ "name": "m_axi_kvdestdram_V_ARSIZE", "direction": "out", "datatype": "sc_lv", "bitwidth":3, "type": "signal", "bundle":{"name": "kvdestdram_V", "role": "ARSIZE" }} , 
 	{ "name": "m_axi_kvdestdram_V_ARBURST", "direction": "out", "datatype": "sc_lv", "bitwidth":2, "type": "signal", "bundle":{"name": "kvdestdram_V", "role": "ARBURST" }} , 
 	{ "name": "m_axi_kvdestdram_V_ARLOCK", "direction": "out", "datatype": "sc_lv", "bitwidth":2, "type": "signal", "bundle":{"name": "kvdestdram_V", "role": "ARLOCK" }} , 
 	{ "name": "m_axi_kvdestdram_V_ARCACHE", "direction": "out", "datatype": "sc_lv", "bitwidth":4, "type": "signal", "bundle":{"name": "kvdestdram_V", "role": "ARCACHE" }} , 
 	{ "name": "m_axi_kvdestdram_V_ARPROT", "direction": "out", "datatype": "sc_lv", "bitwidth":3, "type": "signal", "bundle":{"name": "kvdestdram_V", "role": "ARPROT" }} , 
 	{ "name": "m_axi_kvdestdram_V_ARQOS", "direction": "out", "datatype": "sc_lv", "bitwidth":4, "type": "signal", "bundle":{"name": "kvdestdram_V", "role": "ARQOS" }} , 
 	{ "name": "m_axi_kvdestdram_V_ARREGION", "direction": "out", "datatype": "sc_lv", "bitwidth":4, "type": "signal", "bundle":{"name": "kvdestdram_V", "role": "ARREGION" }} , 
 	{ "name": "m_axi_kvdestdram_V_ARUSER", "direction": "out", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "kvdestdram_V", "role": "ARUSER" }} , 
 	{ "name": "m_axi_kvdestdram_V_RVALID", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "kvdestdram_V", "role": "RVALID" }} , 
 	{ "name": "m_axi_kvdestdram_V_RREADY", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "kvdestdram_V", "role": "RREADY" }} , 
 	{ "name": "m_axi_kvdestdram_V_RDATA", "direction": "in", "datatype": "sc_lv", "bitwidth":512, "type": "signal", "bundle":{"name": "kvdestdram_V", "role": "RDATA" }} , 
 	{ "name": "m_axi_kvdestdram_V_RLAST", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "kvdestdram_V", "role": "RLAST" }} , 
 	{ "name": "m_axi_kvdestdram_V_RID", "direction": "in", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "kvdestdram_V", "role": "RID" }} , 
 	{ "name": "m_axi_kvdestdram_V_RUSER", "direction": "in", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "kvdestdram_V", "role": "RUSER" }} , 
 	{ "name": "m_axi_kvdestdram_V_RRESP", "direction": "in", "datatype": "sc_lv", "bitwidth":2, "type": "signal", "bundle":{"name": "kvdestdram_V", "role": "RRESP" }} , 
 	{ "name": "m_axi_kvdestdram_V_BVALID", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "kvdestdram_V", "role": "BVALID" }} , 
 	{ "name": "m_axi_kvdestdram_V_BREADY", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "kvdestdram_V", "role": "BREADY" }} , 
 	{ "name": "m_axi_kvdestdram_V_BRESP", "direction": "in", "datatype": "sc_lv", "bitwidth":2, "type": "signal", "bundle":{"name": "kvdestdram_V", "role": "BRESP" }} , 
 	{ "name": "m_axi_kvdestdram_V_BID", "direction": "in", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "kvdestdram_V", "role": "BID" }} , 
 	{ "name": "m_axi_kvdestdram_V_BUSER", "direction": "in", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "kvdestdram_V", "role": "BUSER" }} , 
 	{ "name": "kvdestdram_V_offset", "direction": "in", "datatype": "sc_lv", "bitwidth":26, "type": "signal", "bundle":{"name": "kvdestdram_V_offset", "role": "default" }} , 
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
 	{ "name": "globalparams_runkernelcommand", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "globalparams_runkernelcommand", "role": "default" }} , 
 	{ "name": "globalparams_partitioncommand", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "globalparams_partitioncommand", "role": "default" }} , 
 	{ "name": "globalparams_reducecommand", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "globalparams_reducecommand", "role": "default" }} , 
 	{ "name": "globalparams_treedepth", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "globalparams_treedepth", "role": "default" }} , 
 	{ "name": "globalparams_runsize", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "globalparams_runsize", "role": "default" }} , 
 	{ "name": "globalparams_vbegin", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "globalparams_vbegin", "role": "default" }} , 
 	{ "name": "globalparams_statsalreadycollected", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "globalparams_statsalreadycollected", "role": "default" }}  ]}

set RtlHierarchyInfo {[
	{"ID" : "0", "Level" : "0", "Path" : "`AUTOTB_DUT_INST", "Parent" : "", "Child" : ["1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13", "14", "15", "16", "17", "18", "19", "20", "21", "22", "23", "24", "25", "26", "27", "40", "169", "178", "227", "229", "231", "233", "235", "236", "237", "238", "239"],
		"CDFG" : "partitionandreduce0",
		"Protocol" : "ap_ctrl_hs",
		"ControlExist" : "1", "ap_start" : "1", "ap_ready" : "1", "ap_done" : "1", "ap_continue" : "0", "ap_idle" : "1",
		"Pipeline" : "None", "UnalignedPipeline" : "0", "RewindPipeline" : "0", "ProcessNetwork" : "0",
		"II" : "0",
		"VariableLatency" : "1", "ExactLatency" : "-1", "EstimateLatencyMin" : "41", "EstimateLatencyMax" : "197852388",
		"Combinational" : "0",
		"Datapath" : "0",
		"ClockEnable" : "0",
		"HasSubDataflow" : "0",
		"InDataflowNetwork" : "0",
		"HasNonBlockingOperation" : "0",
		"WaitState" : [
			{"State" : "ap_ST_fsm_state52", "FSM" : "ap_CS_fsm", "SubInstance" : "grp_partitionkeyvalues00_fu_1486"},
			{"State" : "ap_ST_fsm_state55", "FSM" : "ap_CS_fsm", "SubInstance" : "grp_partitionkeyvalues00_fu_1486"},
			{"State" : "ap_ST_fsm_state54", "FSM" : "ap_CS_fsm", "SubInstance" : "grp_partitionkeyvalues00_fu_1486"},
			{"State" : "ap_ST_fsm_state75", "FSM" : "ap_CS_fsm", "SubInstance" : "grp_reducepartitions0_fu_1514"},
			{"State" : "ap_ST_fsm_state77", "FSM" : "ap_CS_fsm", "SubInstance" : "grp_reducepartitions0_fu_1514"},
			{"State" : "ap_ST_fsm_state57", "FSM" : "ap_CS_fsm", "SubInstance" : "grp_savepartitions0_fu_1529"},
			{"State" : "ap_ST_fsm_state36", "FSM" : "ap_CS_fsm", "SubInstance" : "grp_collectstats00_fu_1551"},
			{"State" : "ap_ST_fsm_state39", "FSM" : "ap_CS_fsm", "SubInstance" : "grp_collectstats00_fu_1551"},
			{"State" : "ap_ST_fsm_state38", "FSM" : "ap_CS_fsm", "SubInstance" : "grp_collectstats00_fu_1551"},
			{"State" : "ap_ST_fsm_state50", "FSM" : "ap_CS_fsm", "SubInstance" : "grp_readcapsules0_fu_1575"},
			{"State" : "ap_ST_fsm_state41", "FSM" : "ap_CS_fsm", "SubInstance" : "grp_savecapsules0_fu_1599"},
			{"State" : "ap_ST_fsm_state75", "FSM" : "ap_CS_fsm", "SubInstance" : "grp_readkeyvalues0_2_fu_1628"},
			{"State" : "ap_ST_fsm_state77", "FSM" : "ap_CS_fsm", "SubInstance" : "grp_readkeyvalues0_2_fu_1628"},
			{"State" : "ap_ST_fsm_state36", "FSM" : "ap_CS_fsm", "SubInstance" : "grp_readkeyvalues0_1_fu_1650"},
			{"State" : "ap_ST_fsm_state39", "FSM" : "ap_CS_fsm", "SubInstance" : "grp_readkeyvalues0_1_fu_1650"},
			{"State" : "ap_ST_fsm_state38", "FSM" : "ap_CS_fsm", "SubInstance" : "grp_readkeyvalues0_1_fu_1650"},
			{"State" : "ap_ST_fsm_state52", "FSM" : "ap_CS_fsm", "SubInstance" : "grp_readkeyvalues0_1_fu_1650"},
			{"State" : "ap_ST_fsm_state55", "FSM" : "ap_CS_fsm", "SubInstance" : "grp_readkeyvalues0_1_fu_1650"},
			{"State" : "ap_ST_fsm_state54", "FSM" : "ap_CS_fsm", "SubInstance" : "grp_readkeyvalues0_1_fu_1650"},
			{"State" : "ap_ST_fsm_state78", "FSM" : "ap_CS_fsm", "SubInstance" : "grp_savekeyvalues0_153_fu_1667"},
			{"State" : "ap_ST_fsm_state80", "FSM" : "ap_CS_fsm", "SubInstance" : "grp_savekeyvalues0_153_fu_1667"},
			{"State" : "ap_ST_fsm_state82", "FSM" : "ap_CS_fsm", "SubInstance" : "grp_savekeyvalues0_153_fu_1667"},
			{"State" : "ap_ST_fsm_state84", "FSM" : "ap_CS_fsm", "SubInstance" : "grp_savekeyvalues0_153_fu_1667"},
			{"State" : "ap_ST_fsm_state67", "FSM" : "ap_CS_fsm", "SubInstance" : "grp_readkeyvalues0_157_fu_1676"},
			{"State" : "ap_ST_fsm_state69", "FSM" : "ap_CS_fsm", "SubInstance" : "grp_readkeyvalues0_157_fu_1676"},
			{"State" : "ap_ST_fsm_state71", "FSM" : "ap_CS_fsm", "SubInstance" : "grp_readkeyvalues0_157_fu_1676"},
			{"State" : "ap_ST_fsm_state73", "FSM" : "ap_CS_fsm", "SubInstance" : "grp_readkeyvalues0_157_fu_1676"},
			{"State" : "ap_ST_fsm_state93", "FSM" : "ap_CS_fsm", "SubInstance" : "grp_savekeyvalues0_2_fu_1685"}],
		"Port" : [
			{"Name" : "kvsourcedram_V", "Type" : "MAXI", "Direction" : "IO",
				"SubConnect" : [
					{"ID" : "231", "SubInstance" : "grp_readkeyvalues0_2_fu_1628", "Port" : "kvdram_V"},
					{"ID" : "227", "SubInstance" : "grp_readcapsules0_fu_1575", "Port" : "kvdram_V"},
					{"ID" : "169", "SubInstance" : "grp_savepartitions0_fu_1529", "Port" : "kvdram_V"},
					{"ID" : "233", "SubInstance" : "grp_readkeyvalues0_1_fu_1650", "Port" : "kvdram_V"},
					{"ID" : "229", "SubInstance" : "grp_savecapsules0_fu_1599", "Port" : "kvdram_V"}]},
			{"Name" : "kvsourcedram_V_offset", "Type" : "None", "Direction" : "I"},
			{"Name" : "kvdestdram_V", "Type" : "MAXI", "Direction" : "IO",
				"SubConnect" : [
					{"ID" : "236", "SubInstance" : "grp_readkeyvalues0_157_fu_1676", "Port" : "kvdram_V"},
					{"ID" : "235", "SubInstance" : "grp_savekeyvalues0_153_fu_1667", "Port" : "kvdram_V"}]},
			{"Name" : "kvdestdram_V_offset", "Type" : "None", "Direction" : "I"},
			{"Name" : "kvstats", "Type" : "MAXI", "Direction" : "IO",
				"BlockSignal" : [
					{"Name" : "kvstats_blk_n_AW", "Type" : "RtlSignal"},
					{"Name" : "kvstats_blk_n_W", "Type" : "RtlSignal"},
					{"Name" : "kvstats_blk_n_B", "Type" : "RtlSignal"},
					{"Name" : "kvstats_blk_n_AR", "Type" : "RtlSignal"},
					{"Name" : "kvstats_blk_n_R", "Type" : "RtlSignal"}],
				"SubConnect" : [
					{"ID" : "238", "SubInstance" : "grp_savekeyvalues0_1_fu_1695", "Port" : "dram"},
					{"ID" : "237", "SubInstance" : "grp_savekeyvalues0_2_fu_1685", "Port" : "dram"}]},
			{"Name" : "kvstats_offset", "Type" : "None", "Direction" : "I"},
			{"Name" : "globalparams_runkernelcommand", "Type" : "None", "Direction" : "I"},
			{"Name" : "globalparams_partitioncommand", "Type" : "None", "Direction" : "I"},
			{"Name" : "globalparams_reducecommand", "Type" : "None", "Direction" : "I"},
			{"Name" : "globalparams_treedepth", "Type" : "None", "Direction" : "I"},
			{"Name" : "globalparams_runsize", "Type" : "None", "Direction" : "I"},
			{"Name" : "globalparams_vbegin", "Type" : "None", "Direction" : "I"},
			{"Name" : "globalparams_statsalreadycollected", "Type" : "None", "Direction" : "I"}],
		"SubInstanceBlock" : [
			{"SubInstance" : "grp_savekeyvalues0_1_fu_1695", "SubBlockPort" : ["dram_blk_n_AW", "dram_blk_n_W", "dram_blk_n_B"]}]},
	{"ID" : "1", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.sourcebuffer00_0_V_U", "Parent" : "0"},
	{"ID" : "2", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.sourcebuffer00_1_V_U", "Parent" : "0"},
	{"ID" : "3", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.destbuffer00_V_U", "Parent" : "0"},
	{"ID" : "4", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.capsule00_key_U", "Parent" : "0"},
	{"ID" : "5", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.capsule00_value_U", "Parent" : "0"},
	{"ID" : "6", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.BIGcapsule00_V_U", "Parent" : "0"},
	{"ID" : "7", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.sourcebuffer01_0_V_U", "Parent" : "0"},
	{"ID" : "8", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.sourcebuffer01_1_V_U", "Parent" : "0"},
	{"ID" : "9", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.destbuffer01_V_U", "Parent" : "0"},
	{"ID" : "10", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.capsule01_key_U", "Parent" : "0"},
	{"ID" : "11", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.capsule01_value_U", "Parent" : "0"},
	{"ID" : "12", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.BIGcapsule01_V_U", "Parent" : "0"},
	{"ID" : "13", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.sourcebuffer02_0_V_U", "Parent" : "0"},
	{"ID" : "14", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.sourcebuffer02_1_V_U", "Parent" : "0"},
	{"ID" : "15", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.destbuffer02_V_U", "Parent" : "0"},
	{"ID" : "16", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.capsule02_key_U", "Parent" : "0"},
	{"ID" : "17", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.capsule02_value_U", "Parent" : "0"},
	{"ID" : "18", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.BIGcapsule02_V_U", "Parent" : "0"},
	{"ID" : "19", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.sourcebuffer03_0_V_U", "Parent" : "0"},
	{"ID" : "20", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.sourcebuffer03_1_V_U", "Parent" : "0"},
	{"ID" : "21", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.destbuffer03_V_U", "Parent" : "0"},
	{"ID" : "22", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.capsule03_key_U", "Parent" : "0"},
	{"ID" : "23", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.capsule03_value_U", "Parent" : "0"},
	{"ID" : "24", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.BIGcapsule03_V_U", "Parent" : "0"},
	{"ID" : "25", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.kvdeststats_tmp_key_U", "Parent" : "0"},
	{"ID" : "26", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.kvdeststats_tmp_valu_U", "Parent" : "0"},
	{"ID" : "27", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.grp_partitionkeyvalues00_fu_1486", "Parent" : "0", "Child" : ["28", "29", "30", "31", "32", "33", "34", "35", "36", "37", "38", "39"],
		"CDFG" : "partitionkeyvalues00",
		"Protocol" : "ap_ctrl_hs",
		"ControlExist" : "1", "ap_start" : "1", "ap_ready" : "1", "ap_done" : "1", "ap_continue" : "0", "ap_idle" : "1",
		"Pipeline" : "None", "UnalignedPipeline" : "0", "RewindPipeline" : "0", "ProcessNetwork" : "0",
		"II" : "0",
		"VariableLatency" : "1", "ExactLatency" : "-1", "EstimateLatencyMin" : "1", "EstimateLatencyMax" : "4228",
		"Combinational" : "0",
		"Datapath" : "0",
		"ClockEnable" : "0",
		"HasSubDataflow" : "0",
		"InDataflowNetwork" : "0",
		"HasNonBlockingOperation" : "0",
		"Port" : [
			{"Name" : "enable", "Type" : "None", "Direction" : "I"},
			{"Name" : "sourcebuffer0_V", "Type" : "Memory", "Direction" : "I"},
			{"Name" : "sourcebuffer1_V", "Type" : "Memory", "Direction" : "I"},
			{"Name" : "sourcebuffer2_V", "Type" : "Memory", "Direction" : "I"},
			{"Name" : "sourcebuffer3_V", "Type" : "Memory", "Direction" : "I"},
			{"Name" : "destbuffer0_V", "Type" : "Memory", "Direction" : "O"},
			{"Name" : "destbuffer1_V", "Type" : "Memory", "Direction" : "O"},
			{"Name" : "destbuffer2_V", "Type" : "Memory", "Direction" : "O"},
			{"Name" : "destbuffer3_V", "Type" : "Memory", "Direction" : "O"},
			{"Name" : "capsule0_key", "Type" : "Memory", "Direction" : "I"},
			{"Name" : "capsule0_value", "Type" : "Memory", "Direction" : "IO"},
			{"Name" : "capsule1_key", "Type" : "Memory", "Direction" : "I"},
			{"Name" : "capsule1_value", "Type" : "Memory", "Direction" : "IO"},
			{"Name" : "capsule2_key", "Type" : "Memory", "Direction" : "I"},
			{"Name" : "capsule2_value", "Type" : "Memory", "Direction" : "IO"},
			{"Name" : "capsule3_key", "Type" : "Memory", "Direction" : "I"},
			{"Name" : "capsule3_value", "Type" : "Memory", "Direction" : "IO"},
			{"Name" : "travstate_i_kvs", "Type" : "None", "Direction" : "I"},
			{"Name" : "travstate_end_kvs", "Type" : "None", "Direction" : "I"},
			{"Name" : "llopparams_currentLOP", "Type" : "None", "Direction" : "I"},
			{"Name" : "llopparams_upperlimit", "Type" : "None", "Direction" : "I"}]},
	{"ID" : "28", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_partitionkeyvalues00_fu_1486.topkernel_mux_325_32_1_1_U85", "Parent" : "27"},
	{"ID" : "29", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_partitionkeyvalues00_fu_1486.topkernel_mux_325_32_1_1_U86", "Parent" : "27"},
	{"ID" : "30", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_partitionkeyvalues00_fu_1486.topkernel_mux_325_32_1_1_U87", "Parent" : "27"},
	{"ID" : "31", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_partitionkeyvalues00_fu_1486.topkernel_mux_325_32_1_1_U88", "Parent" : "27"},
	{"ID" : "32", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_partitionkeyvalues00_fu_1486.topkernel_mux_325_32_1_1_U89", "Parent" : "27"},
	{"ID" : "33", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_partitionkeyvalues00_fu_1486.topkernel_mux_325_32_1_1_U90", "Parent" : "27"},
	{"ID" : "34", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_partitionkeyvalues00_fu_1486.topkernel_mux_325_32_1_1_U91", "Parent" : "27"},
	{"ID" : "35", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_partitionkeyvalues00_fu_1486.topkernel_mux_325_32_1_1_U92", "Parent" : "27"},
	{"ID" : "36", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_partitionkeyvalues00_fu_1486.topkernel_mux_325_32_1_1_U93", "Parent" : "27"},
	{"ID" : "37", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_partitionkeyvalues00_fu_1486.topkernel_mux_325_32_1_1_U94", "Parent" : "27"},
	{"ID" : "38", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_partitionkeyvalues00_fu_1486.topkernel_mux_325_32_1_1_U95", "Parent" : "27"},
	{"ID" : "39", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_partitionkeyvalues00_fu_1486.topkernel_mux_325_32_1_1_U96", "Parent" : "27"},
	{"ID" : "40", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1514", "Parent" : "0", "Child" : ["41", "42", "43", "44", "45", "46", "47", "48", "49", "50", "51", "52", "53", "54", "55", "56", "57", "58", "59", "60", "61", "62", "63", "64", "65", "66", "67", "68", "69", "70", "71", "72", "73", "74", "75", "76", "77", "78", "79", "80", "81", "82", "83", "84", "85", "86", "87", "88", "89", "90", "91", "92", "93", "94", "95", "96", "97", "98", "99", "100", "101", "102", "103", "104", "105", "106", "107", "108", "109", "110", "111", "112", "113", "114", "115", "116", "117", "118", "119", "120", "121", "122", "123", "124", "125", "126", "127", "128", "129", "130", "131", "132", "133", "134", "135", "136", "137", "138", "139", "140", "141", "142", "143", "144", "145", "146", "147", "148", "149", "150", "151", "152", "153", "154", "155", "156", "157", "158", "159", "160", "161", "162", "163", "164", "165", "166", "167", "168"],
		"CDFG" : "reducepartitions0",
		"Protocol" : "ap_ctrl_hs",
		"ControlExist" : "1", "ap_start" : "1", "ap_ready" : "1", "ap_done" : "1", "ap_continue" : "0", "ap_idle" : "1",
		"Pipeline" : "None", "UnalignedPipeline" : "0", "RewindPipeline" : "0", "ProcessNetwork" : "0",
		"II" : "0",
		"VariableLatency" : "1", "ExactLatency" : "-1", "EstimateLatencyMin" : "1", "EstimateLatencyMax" : "6181",
		"Combinational" : "0",
		"Datapath" : "0",
		"ClockEnable" : "0",
		"HasSubDataflow" : "0",
		"InDataflowNetwork" : "0",
		"HasNonBlockingOperation" : "0",
		"Port" : [
			{"Name" : "enable", "Type" : "None", "Direction" : "I"},
			{"Name" : "sourcebuffer0_V", "Type" : "Memory", "Direction" : "I"},
			{"Name" : "sourcebuffer1_V", "Type" : "Memory", "Direction" : "I"},
			{"Name" : "sourcebuffer2_V", "Type" : "Memory", "Direction" : "I"},
			{"Name" : "sourcebuffer3_V", "Type" : "Memory", "Direction" : "I"},
			{"Name" : "destbuffer0_V", "Type" : "Memory", "Direction" : "IO"},
			{"Name" : "destbuffer1_V", "Type" : "Memory", "Direction" : "IO"},
			{"Name" : "destbuffer2_V", "Type" : "Memory", "Direction" : "IO"},
			{"Name" : "destbuffer3_V", "Type" : "Memory", "Direction" : "IO"},
			{"Name" : "globalparams_vbegin", "Type" : "None", "Direction" : "I"}]},
	{"ID" : "41", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1514.destbuffer00_key_U", "Parent" : "40"},
	{"ID" : "42", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1514.destbuffer00_value_U", "Parent" : "40"},
	{"ID" : "43", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1514.destbuffer01_key_U", "Parent" : "40"},
	{"ID" : "44", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1514.destbuffer01_value_U", "Parent" : "40"},
	{"ID" : "45", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1514.destbuffer02_key_U", "Parent" : "40"},
	{"ID" : "46", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1514.destbuffer02_value_U", "Parent" : "40"},
	{"ID" : "47", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1514.destbuffer03_key_U", "Parent" : "40"},
	{"ID" : "48", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1514.destbuffer03_value_U", "Parent" : "40"},
	{"ID" : "49", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1514.destbuffer04_key_U", "Parent" : "40"},
	{"ID" : "50", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1514.destbuffer04_value_U", "Parent" : "40"},
	{"ID" : "51", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1514.destbuffer05_key_U", "Parent" : "40"},
	{"ID" : "52", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1514.destbuffer05_value_U", "Parent" : "40"},
	{"ID" : "53", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1514.destbuffer06_key_U", "Parent" : "40"},
	{"ID" : "54", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1514.destbuffer06_value_U", "Parent" : "40"},
	{"ID" : "55", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1514.destbuffer07_key_U", "Parent" : "40"},
	{"ID" : "56", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1514.destbuffer07_value_U", "Parent" : "40"},
	{"ID" : "57", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1514.destbuffer10_key_U", "Parent" : "40"},
	{"ID" : "58", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1514.destbuffer10_value_U", "Parent" : "40"},
	{"ID" : "59", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1514.destbuffer11_key_U", "Parent" : "40"},
	{"ID" : "60", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1514.destbuffer11_value_U", "Parent" : "40"},
	{"ID" : "61", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1514.destbuffer12_key_U", "Parent" : "40"},
	{"ID" : "62", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1514.destbuffer12_value_U", "Parent" : "40"},
	{"ID" : "63", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1514.destbuffer13_key_U", "Parent" : "40"},
	{"ID" : "64", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1514.destbuffer13_value_U", "Parent" : "40"},
	{"ID" : "65", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1514.destbuffer14_key_U", "Parent" : "40"},
	{"ID" : "66", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1514.destbuffer14_value_U", "Parent" : "40"},
	{"ID" : "67", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1514.destbuffer15_key_U", "Parent" : "40"},
	{"ID" : "68", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1514.destbuffer15_value_U", "Parent" : "40"},
	{"ID" : "69", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1514.destbuffer16_key_U", "Parent" : "40"},
	{"ID" : "70", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1514.destbuffer16_value_U", "Parent" : "40"},
	{"ID" : "71", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1514.destbuffer17_key_U", "Parent" : "40"},
	{"ID" : "72", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1514.destbuffer17_value_U", "Parent" : "40"},
	{"ID" : "73", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1514.destbuffer20_key_U", "Parent" : "40"},
	{"ID" : "74", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1514.destbuffer20_value_U", "Parent" : "40"},
	{"ID" : "75", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1514.destbuffer21_key_U", "Parent" : "40"},
	{"ID" : "76", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1514.destbuffer21_value_U", "Parent" : "40"},
	{"ID" : "77", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1514.destbuffer22_key_U", "Parent" : "40"},
	{"ID" : "78", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1514.destbuffer22_value_U", "Parent" : "40"},
	{"ID" : "79", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1514.destbuffer23_key_U", "Parent" : "40"},
	{"ID" : "80", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1514.destbuffer23_value_U", "Parent" : "40"},
	{"ID" : "81", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1514.destbuffer24_key_U", "Parent" : "40"},
	{"ID" : "82", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1514.destbuffer24_value_U", "Parent" : "40"},
	{"ID" : "83", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1514.destbuffer25_key_U", "Parent" : "40"},
	{"ID" : "84", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1514.destbuffer25_value_U", "Parent" : "40"},
	{"ID" : "85", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1514.destbuffer26_key_U", "Parent" : "40"},
	{"ID" : "86", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1514.destbuffer26_value_U", "Parent" : "40"},
	{"ID" : "87", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1514.destbuffer27_key_U", "Parent" : "40"},
	{"ID" : "88", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1514.destbuffer27_value_U", "Parent" : "40"},
	{"ID" : "89", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1514.destbuffer30_key_U", "Parent" : "40"},
	{"ID" : "90", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1514.destbuffer30_value_U", "Parent" : "40"},
	{"ID" : "91", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1514.destbuffer31_key_U", "Parent" : "40"},
	{"ID" : "92", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1514.destbuffer31_value_U", "Parent" : "40"},
	{"ID" : "93", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1514.destbuffer32_key_U", "Parent" : "40"},
	{"ID" : "94", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1514.destbuffer32_value_U", "Parent" : "40"},
	{"ID" : "95", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1514.destbuffer33_key_U", "Parent" : "40"},
	{"ID" : "96", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1514.destbuffer33_value_U", "Parent" : "40"},
	{"ID" : "97", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1514.destbuffer34_key_U", "Parent" : "40"},
	{"ID" : "98", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1514.destbuffer34_value_U", "Parent" : "40"},
	{"ID" : "99", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1514.destbuffer35_key_U", "Parent" : "40"},
	{"ID" : "100", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1514.destbuffer35_value_U", "Parent" : "40"},
	{"ID" : "101", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1514.destbuffer36_key_U", "Parent" : "40"},
	{"ID" : "102", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1514.destbuffer36_value_U", "Parent" : "40"},
	{"ID" : "103", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1514.destbuffer37_key_U", "Parent" : "40"},
	{"ID" : "104", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1514.destbuffer37_value_U", "Parent" : "40"},
	{"ID" : "105", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1514.Vtemp0_0_key_U", "Parent" : "40"},
	{"ID" : "106", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1514.Vtemp0_1_key_U", "Parent" : "40"},
	{"ID" : "107", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1514.Vtemp0_2_key_U", "Parent" : "40"},
	{"ID" : "108", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1514.Vtemp0_3_key_U", "Parent" : "40"},
	{"ID" : "109", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1514.Vtemp0_4_key_U", "Parent" : "40"},
	{"ID" : "110", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1514.Vtemp0_5_key_U", "Parent" : "40"},
	{"ID" : "111", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1514.Vtemp0_6_key_U", "Parent" : "40"},
	{"ID" : "112", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1514.Vtemp0_7_key_U", "Parent" : "40"},
	{"ID" : "113", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1514.Vtemp0_0_value_U", "Parent" : "40"},
	{"ID" : "114", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1514.Vtemp0_1_value_U", "Parent" : "40"},
	{"ID" : "115", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1514.Vtemp0_2_value_U", "Parent" : "40"},
	{"ID" : "116", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1514.Vtemp0_3_value_U", "Parent" : "40"},
	{"ID" : "117", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1514.Vtemp0_4_value_U", "Parent" : "40"},
	{"ID" : "118", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1514.Vtemp0_5_value_U", "Parent" : "40"},
	{"ID" : "119", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1514.Vtemp0_6_value_U", "Parent" : "40"},
	{"ID" : "120", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1514.Vtemp0_7_value_U", "Parent" : "40"},
	{"ID" : "121", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1514.Vtemp1_0_key_U", "Parent" : "40"},
	{"ID" : "122", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1514.Vtemp1_1_key_U", "Parent" : "40"},
	{"ID" : "123", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1514.Vtemp1_2_key_U", "Parent" : "40"},
	{"ID" : "124", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1514.Vtemp1_3_key_U", "Parent" : "40"},
	{"ID" : "125", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1514.Vtemp1_4_key_U", "Parent" : "40"},
	{"ID" : "126", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1514.Vtemp1_5_key_U", "Parent" : "40"},
	{"ID" : "127", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1514.Vtemp1_6_key_U", "Parent" : "40"},
	{"ID" : "128", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1514.Vtemp1_7_key_U", "Parent" : "40"},
	{"ID" : "129", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1514.Vtemp1_0_value_U", "Parent" : "40"},
	{"ID" : "130", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1514.Vtemp1_1_value_U", "Parent" : "40"},
	{"ID" : "131", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1514.Vtemp1_2_value_U", "Parent" : "40"},
	{"ID" : "132", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1514.Vtemp1_3_value_U", "Parent" : "40"},
	{"ID" : "133", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1514.Vtemp1_4_value_U", "Parent" : "40"},
	{"ID" : "134", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1514.Vtemp1_5_value_U", "Parent" : "40"},
	{"ID" : "135", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1514.Vtemp1_6_value_U", "Parent" : "40"},
	{"ID" : "136", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1514.Vtemp1_7_value_U", "Parent" : "40"},
	{"ID" : "137", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1514.Vtemp2_0_key_U", "Parent" : "40"},
	{"ID" : "138", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1514.Vtemp2_1_key_U", "Parent" : "40"},
	{"ID" : "139", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1514.Vtemp2_2_key_U", "Parent" : "40"},
	{"ID" : "140", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1514.Vtemp2_3_key_U", "Parent" : "40"},
	{"ID" : "141", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1514.Vtemp2_4_key_U", "Parent" : "40"},
	{"ID" : "142", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1514.Vtemp2_5_key_U", "Parent" : "40"},
	{"ID" : "143", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1514.Vtemp2_6_key_U", "Parent" : "40"},
	{"ID" : "144", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1514.Vtemp2_7_key_U", "Parent" : "40"},
	{"ID" : "145", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1514.Vtemp2_0_value_U", "Parent" : "40"},
	{"ID" : "146", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1514.Vtemp2_1_value_U", "Parent" : "40"},
	{"ID" : "147", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1514.Vtemp2_2_value_U", "Parent" : "40"},
	{"ID" : "148", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1514.Vtemp2_3_value_U", "Parent" : "40"},
	{"ID" : "149", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1514.Vtemp2_4_value_U", "Parent" : "40"},
	{"ID" : "150", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1514.Vtemp2_5_value_U", "Parent" : "40"},
	{"ID" : "151", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1514.Vtemp2_6_value_U", "Parent" : "40"},
	{"ID" : "152", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1514.Vtemp2_7_value_U", "Parent" : "40"},
	{"ID" : "153", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1514.Vtemp3_0_key_U", "Parent" : "40"},
	{"ID" : "154", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1514.Vtemp3_1_key_U", "Parent" : "40"},
	{"ID" : "155", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1514.Vtemp3_2_key_U", "Parent" : "40"},
	{"ID" : "156", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1514.Vtemp3_3_key_U", "Parent" : "40"},
	{"ID" : "157", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1514.Vtemp3_4_key_U", "Parent" : "40"},
	{"ID" : "158", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1514.Vtemp3_5_key_U", "Parent" : "40"},
	{"ID" : "159", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1514.Vtemp3_6_key_U", "Parent" : "40"},
	{"ID" : "160", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1514.Vtemp3_7_key_U", "Parent" : "40"},
	{"ID" : "161", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1514.Vtemp3_0_value_U", "Parent" : "40"},
	{"ID" : "162", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1514.Vtemp3_1_value_U", "Parent" : "40"},
	{"ID" : "163", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1514.Vtemp3_2_value_U", "Parent" : "40"},
	{"ID" : "164", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1514.Vtemp3_3_value_U", "Parent" : "40"},
	{"ID" : "165", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1514.Vtemp3_4_value_U", "Parent" : "40"},
	{"ID" : "166", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1514.Vtemp3_5_value_U", "Parent" : "40"},
	{"ID" : "167", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1514.Vtemp3_6_value_U", "Parent" : "40"},
	{"ID" : "168", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1514.Vtemp3_7_value_U", "Parent" : "40"},
	{"ID" : "169", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.grp_savepartitions0_fu_1529", "Parent" : "0", "Child" : ["170", "171", "172", "173", "174", "175", "176", "177"],
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
	{"ID" : "170", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_savepartitions0_fu_1529.topkernel_urem_12ns_8ns_12_16_1_U119", "Parent" : "169"},
	{"ID" : "171", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_savepartitions0_fu_1529.topkernel_urem_12ns_8ns_12_16_1_U120", "Parent" : "169"},
	{"ID" : "172", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_savepartitions0_fu_1529.topkernel_urem_12ns_8ns_12_16_1_U121", "Parent" : "169"},
	{"ID" : "173", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_savepartitions0_fu_1529.topkernel_urem_12ns_8ns_12_16_1_U122", "Parent" : "169"},
	{"ID" : "174", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_savepartitions0_fu_1529.topkernel_urem_12ns_8ns_12_16_1_U123", "Parent" : "169"},
	{"ID" : "175", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_savepartitions0_fu_1529.topkernel_urem_12ns_8ns_12_16_1_U124", "Parent" : "169"},
	{"ID" : "176", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_savepartitions0_fu_1529.topkernel_urem_12ns_8ns_12_16_1_U125", "Parent" : "169"},
	{"ID" : "177", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_savepartitions0_fu_1529.topkernel_urem_12ns_8ns_12_16_1_U126", "Parent" : "169"},
	{"ID" : "178", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.grp_collectstats00_fu_1551", "Parent" : "0", "Child" : ["179", "180", "181", "182", "183", "184", "185", "186", "187", "188", "189", "190", "191", "192", "193", "194", "195", "196", "197", "198", "199", "200", "201", "202", "203", "204", "205", "206", "207", "208", "209", "210", "211", "212", "213", "214", "215", "216", "217", "218", "219", "220", "221", "222", "223", "224", "225", "226"],
		"CDFG" : "collectstats00",
		"Protocol" : "ap_ctrl_hs",
		"ControlExist" : "1", "ap_start" : "1", "ap_ready" : "1", "ap_done" : "1", "ap_continue" : "0", "ap_idle" : "1",
		"Pipeline" : "None", "UnalignedPipeline" : "0", "RewindPipeline" : "0", "ProcessNetwork" : "0",
		"II" : "0",
		"VariableLatency" : "1", "ExactLatency" : "-1", "EstimateLatencyMin" : "1", "EstimateLatencyMax" : "1126",
		"Combinational" : "0",
		"Datapath" : "0",
		"ClockEnable" : "0",
		"HasSubDataflow" : "0",
		"InDataflowNetwork" : "0",
		"HasNonBlockingOperation" : "0",
		"Port" : [
			{"Name" : "enable", "Type" : "None", "Direction" : "I"},
			{"Name" : "sourcebuffer0_V", "Type" : "Memory", "Direction" : "I"},
			{"Name" : "sourcebuffer1_V", "Type" : "Memory", "Direction" : "I"},
			{"Name" : "sourcebuffer2_V", "Type" : "Memory", "Direction" : "I"},
			{"Name" : "sourcebuffer3_V", "Type" : "Memory", "Direction" : "I"},
			{"Name" : "capsule0_key", "Type" : "Memory", "Direction" : "O"},
			{"Name" : "capsule0_value", "Type" : "Memory", "Direction" : "IO"},
			{"Name" : "capsule1_key", "Type" : "Memory", "Direction" : "O"},
			{"Name" : "capsule1_value", "Type" : "Memory", "Direction" : "IO"},
			{"Name" : "capsule2_key", "Type" : "Memory", "Direction" : "O"},
			{"Name" : "capsule2_value", "Type" : "Memory", "Direction" : "IO"},
			{"Name" : "capsule3_key", "Type" : "Memory", "Direction" : "O"},
			{"Name" : "capsule3_value", "Type" : "Memory", "Direction" : "IO"},
			{"Name" : "llopparams_currentLOP", "Type" : "None", "Direction" : "I"},
			{"Name" : "llopparams_upperlimit", "Type" : "None", "Direction" : "I"},
			{"Name" : "travstate_i_kvs", "Type" : "None", "Direction" : "I"},
			{"Name" : "travstate_end_kvs", "Type" : "None", "Direction" : "I"}]},
	{"ID" : "179", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_collectstats00_fu_1551.sizes00_U", "Parent" : "178"},
	{"ID" : "180", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_collectstats00_fu_1551.sizes01_U", "Parent" : "178"},
	{"ID" : "181", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_collectstats00_fu_1551.sizes02_U", "Parent" : "178"},
	{"ID" : "182", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_collectstats00_fu_1551.sizes03_U", "Parent" : "178"},
	{"ID" : "183", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_collectstats00_fu_1551.sizes04_U", "Parent" : "178"},
	{"ID" : "184", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_collectstats00_fu_1551.sizes05_U", "Parent" : "178"},
	{"ID" : "185", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_collectstats00_fu_1551.sizes06_U", "Parent" : "178"},
	{"ID" : "186", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_collectstats00_fu_1551.sizes07_U", "Parent" : "178"},
	{"ID" : "187", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_collectstats00_fu_1551.sizes10_U", "Parent" : "178"},
	{"ID" : "188", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_collectstats00_fu_1551.sizes11_U", "Parent" : "178"},
	{"ID" : "189", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_collectstats00_fu_1551.sizes12_U", "Parent" : "178"},
	{"ID" : "190", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_collectstats00_fu_1551.sizes13_U", "Parent" : "178"},
	{"ID" : "191", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_collectstats00_fu_1551.sizes14_U", "Parent" : "178"},
	{"ID" : "192", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_collectstats00_fu_1551.sizes15_U", "Parent" : "178"},
	{"ID" : "193", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_collectstats00_fu_1551.sizes16_U", "Parent" : "178"},
	{"ID" : "194", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_collectstats00_fu_1551.sizes17_U", "Parent" : "178"},
	{"ID" : "195", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_collectstats00_fu_1551.sizes20_U", "Parent" : "178"},
	{"ID" : "196", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_collectstats00_fu_1551.sizes21_U", "Parent" : "178"},
	{"ID" : "197", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_collectstats00_fu_1551.sizes22_U", "Parent" : "178"},
	{"ID" : "198", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_collectstats00_fu_1551.sizes23_U", "Parent" : "178"},
	{"ID" : "199", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_collectstats00_fu_1551.sizes24_U", "Parent" : "178"},
	{"ID" : "200", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_collectstats00_fu_1551.sizes25_U", "Parent" : "178"},
	{"ID" : "201", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_collectstats00_fu_1551.sizes26_U", "Parent" : "178"},
	{"ID" : "202", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_collectstats00_fu_1551.sizes27_U", "Parent" : "178"},
	{"ID" : "203", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_collectstats00_fu_1551.sizes30_U", "Parent" : "178"},
	{"ID" : "204", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_collectstats00_fu_1551.sizes31_U", "Parent" : "178"},
	{"ID" : "205", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_collectstats00_fu_1551.sizes32_U", "Parent" : "178"},
	{"ID" : "206", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_collectstats00_fu_1551.sizes33_U", "Parent" : "178"},
	{"ID" : "207", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_collectstats00_fu_1551.sizes34_U", "Parent" : "178"},
	{"ID" : "208", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_collectstats00_fu_1551.sizes35_U", "Parent" : "178"},
	{"ID" : "209", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_collectstats00_fu_1551.sizes36_U", "Parent" : "178"},
	{"ID" : "210", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_collectstats00_fu_1551.sizes37_U", "Parent" : "178"},
	{"ID" : "211", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_collectstats00_fu_1551.grp_getpartition_fu_1471", "Parent" : "178",
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
	{"ID" : "212", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_collectstats00_fu_1551.grp_getpartition_fu_1478", "Parent" : "178",
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
	{"ID" : "213", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_collectstats00_fu_1551.grp_getpartition_fu_1485", "Parent" : "178",
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
	{"ID" : "214", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_collectstats00_fu_1551.grp_getpartition_fu_1492", "Parent" : "178",
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
	{"ID" : "215", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_collectstats00_fu_1551.grp_getpartition_fu_1499", "Parent" : "178",
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
	{"ID" : "216", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_collectstats00_fu_1551.grp_getpartition_fu_1506", "Parent" : "178",
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
	{"ID" : "217", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_collectstats00_fu_1551.grp_getpartition_fu_1513", "Parent" : "178",
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
	{"ID" : "218", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_collectstats00_fu_1551.grp_getpartition_fu_1520", "Parent" : "178",
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
	{"ID" : "219", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_collectstats00_fu_1551.grp_getpartition_fu_1527", "Parent" : "178",
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
	{"ID" : "220", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_collectstats00_fu_1551.grp_getpartition_fu_1534", "Parent" : "178",
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
	{"ID" : "221", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_collectstats00_fu_1551.grp_getpartition_fu_1541", "Parent" : "178",
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
	{"ID" : "222", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_collectstats00_fu_1551.grp_getpartition_fu_1548", "Parent" : "178",
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
	{"ID" : "223", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_collectstats00_fu_1551.grp_getpartition_fu_1555", "Parent" : "178",
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
	{"ID" : "224", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_collectstats00_fu_1551.grp_getpartition_fu_1562", "Parent" : "178",
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
	{"ID" : "225", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_collectstats00_fu_1551.grp_getpartition_fu_1569", "Parent" : "178",
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
	{"ID" : "226", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_collectstats00_fu_1551.grp_getpartition_fu_1576", "Parent" : "178",
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
	{"ID" : "227", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.grp_readcapsules0_fu_1575", "Parent" : "0", "Child" : ["228"],
		"CDFG" : "readcapsules0",
		"Protocol" : "ap_ctrl_hs",
		"ControlExist" : "1", "ap_start" : "1", "ap_ready" : "1", "ap_done" : "1", "ap_continue" : "0", "ap_idle" : "1",
		"Pipeline" : "None", "UnalignedPipeline" : "0", "RewindPipeline" : "0", "ProcessNetwork" : "0",
		"II" : "0",
		"VariableLatency" : "1", "ExactLatency" : "-1", "EstimateLatencyMin" : "35", "EstimateLatencyMax" : "2129",
		"Combinational" : "0",
		"Datapath" : "0",
		"ClockEnable" : "0",
		"HasSubDataflow" : "0",
		"InDataflowNetwork" : "0",
		"HasNonBlockingOperation" : "0",
		"WaitState" : [
			{"State" : "ap_ST_fsm_state2", "FSM" : "ap_CS_fsm", "SubInstance" : "grp_readkeyvalues0_fu_418"},
			{"State" : "ap_ST_fsm_state4", "FSM" : "ap_CS_fsm", "SubInstance" : "grp_readkeyvalues0_fu_418"},
			{"State" : "ap_ST_fsm_state6", "FSM" : "ap_CS_fsm", "SubInstance" : "grp_readkeyvalues0_fu_418"},
			{"State" : "ap_ST_fsm_state8", "FSM" : "ap_CS_fsm", "SubInstance" : "grp_readkeyvalues0_fu_418"}],
		"Port" : [
			{"Name" : "kvdram_V", "Type" : "MAXI", "Direction" : "I",
				"SubConnect" : [
					{"ID" : "228", "SubInstance" : "grp_readkeyvalues0_fu_418", "Port" : "kvdram_V"}]},
			{"Name" : "kvdram_V_offset", "Type" : "None", "Direction" : "I"},
			{"Name" : "BIGcapsule0_V", "Type" : "Memory", "Direction" : "IO",
				"SubConnect" : [
					{"ID" : "228", "SubInstance" : "grp_readkeyvalues0_fu_418", "Port" : "buffer_V"}]},
			{"Name" : "BIGcapsule1_V", "Type" : "Memory", "Direction" : "IO",
				"SubConnect" : [
					{"ID" : "228", "SubInstance" : "grp_readkeyvalues0_fu_418", "Port" : "buffer_V"}]},
			{"Name" : "BIGcapsule2_V", "Type" : "Memory", "Direction" : "IO",
				"SubConnect" : [
					{"ID" : "228", "SubInstance" : "grp_readkeyvalues0_fu_418", "Port" : "buffer_V"}]},
			{"Name" : "BIGcapsule3_V", "Type" : "Memory", "Direction" : "IO",
				"SubConnect" : [
					{"ID" : "228", "SubInstance" : "grp_readkeyvalues0_fu_418", "Port" : "buffer_V"}]},
			{"Name" : "capsule0_key", "Type" : "Memory", "Direction" : "O"},
			{"Name" : "capsule0_value", "Type" : "Memory", "Direction" : "O"},
			{"Name" : "capsule1_key", "Type" : "Memory", "Direction" : "O"},
			{"Name" : "capsule1_value", "Type" : "Memory", "Direction" : "O"},
			{"Name" : "capsule2_key", "Type" : "Memory", "Direction" : "O"},
			{"Name" : "capsule2_value", "Type" : "Memory", "Direction" : "O"},
			{"Name" : "capsule3_key", "Type" : "Memory", "Direction" : "O"},
			{"Name" : "capsule3_value", "Type" : "Memory", "Direction" : "O"},
			{"Name" : "offset", "Type" : "None", "Direction" : "I"},
			{"Name" : "capsulemetadata_globaloffset", "Type" : "None", "Direction" : "I"},
			{"Name" : "capsulemetadata_localoffset", "Type" : "None", "Direction" : "I"}]},
	{"ID" : "228", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_readcapsules0_fu_1575.grp_readkeyvalues0_fu_418", "Parent" : "227",
		"CDFG" : "readkeyvalues0",
		"Protocol" : "ap_ctrl_hs",
		"ControlExist" : "1", "ap_start" : "1", "ap_ready" : "1", "ap_done" : "1", "ap_continue" : "0", "ap_idle" : "1",
		"Pipeline" : "None", "UnalignedPipeline" : "0", "RewindPipeline" : "0", "ProcessNetwork" : "0",
		"II" : "0",
		"VariableLatency" : "1", "ExactLatency" : "-1", "EstimateLatencyMin" : "522", "EstimateLatencyMax" : "522",
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
			{"Name" : "offset_kvs", "Type" : "None", "Direction" : "I"}]},
	{"ID" : "229", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.grp_savecapsules0_fu_1599", "Parent" : "0", "Child" : ["230"],
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
					{"ID" : "230", "SubInstance" : "grp_savekeyvalues0_fu_887", "Port" : "kvdram_V"}]},
			{"Name" : "kvdram_V_offset", "Type" : "None", "Direction" : "I"},
			{"Name" : "BIGcapsule0_V", "Type" : "Memory", "Direction" : "IO",
				"SubConnect" : [
					{"ID" : "230", "SubInstance" : "grp_savekeyvalues0_fu_887", "Port" : "buffer_V"}]},
			{"Name" : "BIGcapsule1_V", "Type" : "Memory", "Direction" : "IO",
				"SubConnect" : [
					{"ID" : "230", "SubInstance" : "grp_savekeyvalues0_fu_887", "Port" : "buffer_V"}]},
			{"Name" : "BIGcapsule2_V", "Type" : "Memory", "Direction" : "IO",
				"SubConnect" : [
					{"ID" : "230", "SubInstance" : "grp_savekeyvalues0_fu_887", "Port" : "buffer_V"}]},
			{"Name" : "BIGcapsule3_V", "Type" : "Memory", "Direction" : "IO",
				"SubConnect" : [
					{"ID" : "230", "SubInstance" : "grp_savekeyvalues0_fu_887", "Port" : "buffer_V"}]},
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
	{"ID" : "230", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_savecapsules0_fu_1599.grp_savekeyvalues0_fu_887", "Parent" : "229",
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
			{"Name" : "offset_kvs", "Type" : "None", "Direction" : "I"}]},
	{"ID" : "231", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.grp_readkeyvalues0_2_fu_1628", "Parent" : "0", "Child" : ["232"],
		"CDFG" : "readkeyvalues0_2",
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
			{"State" : "ap_ST_fsm_state3", "FSM" : "ap_CS_fsm", "SubInstance" : "grp_readkeyvalues0_158_fu_136"},
			{"State" : "ap_ST_fsm_state5", "FSM" : "ap_CS_fsm", "SubInstance" : "grp_readkeyvalues0_158_fu_136"},
			{"State" : "ap_ST_fsm_state7", "FSM" : "ap_CS_fsm", "SubInstance" : "grp_readkeyvalues0_158_fu_136"},
			{"State" : "ap_ST_fsm_state9", "FSM" : "ap_CS_fsm", "SubInstance" : "grp_readkeyvalues0_158_fu_136"}],
		"Port" : [
			{"Name" : "kvdram_V", "Type" : "MAXI", "Direction" : "I",
				"SubConnect" : [
					{"ID" : "232", "SubInstance" : "grp_readkeyvalues0_158_fu_136", "Port" : "kvdram_V"}]},
			{"Name" : "kvdram_V_offset", "Type" : "None", "Direction" : "I"},
			{"Name" : "buffer0_V", "Type" : "Memory", "Direction" : "O",
				"SubConnect" : [
					{"ID" : "232", "SubInstance" : "grp_readkeyvalues0_158_fu_136", "Port" : "buffer_V"}]},
			{"Name" : "buffer1_V", "Type" : "Memory", "Direction" : "O",
				"SubConnect" : [
					{"ID" : "232", "SubInstance" : "grp_readkeyvalues0_158_fu_136", "Port" : "buffer_V"}]},
			{"Name" : "buffer2_V", "Type" : "Memory", "Direction" : "O",
				"SubConnect" : [
					{"ID" : "232", "SubInstance" : "grp_readkeyvalues0_158_fu_136", "Port" : "buffer_V"}]},
			{"Name" : "buffer3_V", "Type" : "Memory", "Direction" : "O",
				"SubConnect" : [
					{"ID" : "232", "SubInstance" : "grp_readkeyvalues0_158_fu_136", "Port" : "buffer_V"}]},
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
	{"ID" : "232", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_readkeyvalues0_2_fu_1628.grp_readkeyvalues0_158_fu_136", "Parent" : "231",
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
			{"Name" : "size_kvs", "Type" : "None", "Direction" : "I"}]},
	{"ID" : "233", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.grp_readkeyvalues0_1_fu_1650", "Parent" : "0", "Child" : ["234"],
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
					{"ID" : "234", "SubInstance" : "grp_readkeyvalues0_158_fu_94", "Port" : "kvdram_V"}]},
			{"Name" : "kvdram_V_offset", "Type" : "None", "Direction" : "I"},
			{"Name" : "buffer0_V", "Type" : "Memory", "Direction" : "O",
				"SubConnect" : [
					{"ID" : "234", "SubInstance" : "grp_readkeyvalues0_158_fu_94", "Port" : "buffer_V"}]},
			{"Name" : "buffer1_V", "Type" : "Memory", "Direction" : "O",
				"SubConnect" : [
					{"ID" : "234", "SubInstance" : "grp_readkeyvalues0_158_fu_94", "Port" : "buffer_V"}]},
			{"Name" : "buffer2_V", "Type" : "Memory", "Direction" : "O",
				"SubConnect" : [
					{"ID" : "234", "SubInstance" : "grp_readkeyvalues0_158_fu_94", "Port" : "buffer_V"}]},
			{"Name" : "buffer3_V", "Type" : "Memory", "Direction" : "O",
				"SubConnect" : [
					{"ID" : "234", "SubInstance" : "grp_readkeyvalues0_158_fu_94", "Port" : "buffer_V"}]},
			{"Name" : "baseaddress", "Type" : "None", "Direction" : "I"},
			{"Name" : "offset_kvs", "Type" : "None", "Direction" : "I"},
			{"Name" : "travstate_i_kvs", "Type" : "None", "Direction" : "I"},
			{"Name" : "travstate_end_kvs", "Type" : "None", "Direction" : "I"}]},
	{"ID" : "234", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_readkeyvalues0_1_fu_1650.grp_readkeyvalues0_158_fu_94", "Parent" : "233",
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
			{"Name" : "size_kvs", "Type" : "None", "Direction" : "I"}]},
	{"ID" : "235", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.grp_savekeyvalues0_153_fu_1667", "Parent" : "0",
		"CDFG" : "savekeyvalues0_153",
		"Protocol" : "ap_ctrl_hs",
		"ControlExist" : "1", "ap_start" : "1", "ap_ready" : "1", "ap_done" : "1", "ap_continue" : "0", "ap_idle" : "1",
		"Pipeline" : "None", "UnalignedPipeline" : "0", "RewindPipeline" : "0", "ProcessNetwork" : "0",
		"II" : "0",
		"VariableLatency" : "1", "ExactLatency" : "-1", "EstimateLatencyMin" : "264", "EstimateLatencyMax" : "264",
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
			{"Name" : "offset_kvs", "Type" : "None", "Direction" : "I"}]},
	{"ID" : "236", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.grp_readkeyvalues0_157_fu_1676", "Parent" : "0",
		"CDFG" : "readkeyvalues0_157",
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
			{"Name" : "buffer_V", "Type" : "Memory", "Direction" : "O"},
			{"Name" : "offset_kvs", "Type" : "None", "Direction" : "I"}]},
	{"ID" : "237", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.grp_savekeyvalues0_2_fu_1685", "Parent" : "0",
		"CDFG" : "savekeyvalues0_2",
		"Protocol" : "ap_ctrl_hs",
		"ControlExist" : "1", "ap_start" : "1", "ap_ready" : "1", "ap_done" : "1", "ap_continue" : "0", "ap_idle" : "1",
		"Pipeline" : "None", "UnalignedPipeline" : "0", "RewindPipeline" : "0", "ProcessNetwork" : "0",
		"II" : "0",
		"VariableLatency" : "1", "ExactLatency" : "-1", "EstimateLatencyMin" : "40", "EstimateLatencyMax" : "40",
		"Combinational" : "0",
		"Datapath" : "0",
		"ClockEnable" : "0",
		"HasSubDataflow" : "0",
		"InDataflowNetwork" : "0",
		"HasNonBlockingOperation" : "0",
		"Port" : [
			{"Name" : "dram", "Type" : "MAXI", "Direction" : "O",
				"BlockSignal" : [
					{"Name" : "dram_blk_n_AW", "Type" : "RtlSignal"},
					{"Name" : "dram_blk_n_W", "Type" : "RtlSignal"},
					{"Name" : "dram_blk_n_B", "Type" : "RtlSignal"}]},
			{"Name" : "dram_offset", "Type" : "None", "Direction" : "I"},
			{"Name" : "buffer_key", "Type" : "Memory", "Direction" : "I"},
			{"Name" : "buffer_value", "Type" : "Memory", "Direction" : "I"},
			{"Name" : "offset_kvs", "Type" : "None", "Direction" : "I"}]},
	{"ID" : "238", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.grp_savekeyvalues0_1_fu_1695", "Parent" : "0",
		"CDFG" : "savekeyvalues0_1",
		"Protocol" : "ap_ctrl_hs",
		"ControlExist" : "1", "ap_start" : "1", "ap_ready" : "1", "ap_done" : "1", "ap_continue" : "0", "ap_idle" : "1",
		"Pipeline" : "Aligned", "UnalignedPipeline" : "0", "RewindPipeline" : "0", "ProcessNetwork" : "0",
		"II" : "1",
		"VariableLatency" : "0", "ExactLatency" : "7", "EstimateLatencyMin" : "7", "EstimateLatencyMax" : "7",
		"Combinational" : "0",
		"Datapath" : "0",
		"ClockEnable" : "0",
		"HasSubDataflow" : "0",
		"InDataflowNetwork" : "0",
		"HasNonBlockingOperation" : "0",
		"Port" : [
			{"Name" : "dram", "Type" : "MAXI", "Direction" : "O",
				"BlockSignal" : [
					{"Name" : "dram_blk_n_AW", "Type" : "RtlPort"},
					{"Name" : "dram_blk_n_W", "Type" : "RtlPort"},
					{"Name" : "dram_blk_n_B", "Type" : "RtlPort"}]},
			{"Name" : "dram_offset", "Type" : "None", "Direction" : "I"},
			{"Name" : "buffer_0_key_read", "Type" : "None", "Direction" : "I"},
			{"Name" : "buffer_0_value_read", "Type" : "None", "Direction" : "I"},
			{"Name" : "offset_kvs", "Type" : "None", "Direction" : "I"}]},
	{"ID" : "239", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.topkernel_mux_42_32_1_1_U191", "Parent" : "0"}]}


set ArgLastReadFirstWriteLatency {
	partitionandreduce0 {
		kvsourcedram_V {Type IO LastRead 73 FirstWrite -1}
		kvsourcedram_V_offset {Type I LastRead 0 FirstWrite -1}
		kvdestdram_V {Type IO LastRead 9 FirstWrite -1}
		kvdestdram_V_offset {Type I LastRead 0 FirstWrite -1}
		kvstats {Type IO LastRead 44 FirstWrite 2}
		kvstats_offset {Type I LastRead 0 FirstWrite -1}
		globalparams_runkernelcommand {Type I LastRead 0 FirstWrite -1}
		globalparams_partitioncommand {Type I LastRead 0 FirstWrite -1}
		globalparams_reducecommand {Type I LastRead 0 FirstWrite -1}
		globalparams_treedepth {Type I LastRead 0 FirstWrite -1}
		globalparams_runsize {Type I LastRead 0 FirstWrite -1}
		globalparams_vbegin {Type I LastRead 0 FirstWrite -1}
		globalparams_statsalreadycollected {Type I LastRead 0 FirstWrite -1}}
	partitionkeyvalues00 {
		enable {Type I LastRead 0 FirstWrite -1}
		sourcebuffer0_V {Type I LastRead 2 FirstWrite -1}
		sourcebuffer1_V {Type I LastRead 2 FirstWrite -1}
		sourcebuffer2_V {Type I LastRead 2 FirstWrite -1}
		sourcebuffer3_V {Type I LastRead 2 FirstWrite -1}
		destbuffer0_V {Type O LastRead -1 FirstWrite 3}
		destbuffer1_V {Type O LastRead -1 FirstWrite 3}
		destbuffer2_V {Type O LastRead -1 FirstWrite 3}
		destbuffer3_V {Type O LastRead -1 FirstWrite 3}
		capsule0_key {Type I LastRead 1 FirstWrite -1}
		capsule0_value {Type IO LastRead 1 FirstWrite 4}
		capsule1_key {Type I LastRead 1 FirstWrite -1}
		capsule1_value {Type IO LastRead 1 FirstWrite 4}
		capsule2_key {Type I LastRead 1 FirstWrite -1}
		capsule2_value {Type IO LastRead 1 FirstWrite 4}
		capsule3_key {Type I LastRead 1 FirstWrite -1}
		capsule3_value {Type IO LastRead 1 FirstWrite 4}
		travstate_i_kvs {Type I LastRead 0 FirstWrite -1}
		travstate_end_kvs {Type I LastRead 0 FirstWrite -1}
		llopparams_currentLOP {Type I LastRead 0 FirstWrite -1}
		llopparams_upperlimit {Type I LastRead 0 FirstWrite -1}}
	reducepartitions0 {
		enable {Type I LastRead 0 FirstWrite -1}
		sourcebuffer0_V {Type I LastRead 4 FirstWrite -1}
		sourcebuffer1_V {Type I LastRead 4 FirstWrite -1}
		sourcebuffer2_V {Type I LastRead 4 FirstWrite -1}
		sourcebuffer3_V {Type I LastRead 4 FirstWrite -1}
		destbuffer0_V {Type IO LastRead 1 FirstWrite 5}
		destbuffer1_V {Type IO LastRead 1 FirstWrite 5}
		destbuffer2_V {Type IO LastRead 1 FirstWrite 5}
		destbuffer3_V {Type IO LastRead 1 FirstWrite 5}
		globalparams_vbegin {Type I LastRead 0 FirstWrite -1}}
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
		kvdrambaseaddress {Type I LastRead 0 FirstWrite -1}}
	collectstats00 {
		enable {Type I LastRead 0 FirstWrite -1}
		sourcebuffer0_V {Type I LastRead 2 FirstWrite -1}
		sourcebuffer1_V {Type I LastRead 2 FirstWrite -1}
		sourcebuffer2_V {Type I LastRead 2 FirstWrite -1}
		sourcebuffer3_V {Type I LastRead 2 FirstWrite -1}
		capsule0_key {Type O LastRead -1 FirstWrite 4}
		capsule0_value {Type IO LastRead 4 FirstWrite 5}
		capsule1_key {Type O LastRead -1 FirstWrite 4}
		capsule1_value {Type IO LastRead 4 FirstWrite 5}
		capsule2_key {Type O LastRead -1 FirstWrite 4}
		capsule2_value {Type IO LastRead 4 FirstWrite 5}
		capsule3_key {Type O LastRead -1 FirstWrite 4}
		capsule3_value {Type IO LastRead 4 FirstWrite 5}
		llopparams_currentLOP {Type I LastRead 0 FirstWrite -1}
		llopparams_upperlimit {Type I LastRead 0 FirstWrite -1}
		travstate_i_kvs {Type I LastRead 0 FirstWrite -1}
		travstate_end_kvs {Type I LastRead 0 FirstWrite -1}}
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
		upperlimit {Type I LastRead 0 FirstWrite -1}}
	readcapsules0 {
		kvdram_V {Type I LastRead 9 FirstWrite -1}
		kvdram_V_offset {Type I LastRead 0 FirstWrite -1}
		BIGcapsule0_V {Type IO LastRead 8 FirstWrite 10}
		BIGcapsule1_V {Type IO LastRead 8 FirstWrite 10}
		BIGcapsule2_V {Type IO LastRead 8 FirstWrite 10}
		BIGcapsule3_V {Type IO LastRead 8 FirstWrite 10}
		capsule0_key {Type O LastRead -1 FirstWrite 9}
		capsule0_value {Type O LastRead -1 FirstWrite 9}
		capsule1_key {Type O LastRead -1 FirstWrite 9}
		capsule1_value {Type O LastRead -1 FirstWrite 9}
		capsule2_key {Type O LastRead -1 FirstWrite 9}
		capsule2_value {Type O LastRead -1 FirstWrite 9}
		capsule3_key {Type O LastRead -1 FirstWrite 9}
		capsule3_value {Type O LastRead -1 FirstWrite 9}
		offset {Type I LastRead 0 FirstWrite -1}
		capsulemetadata_globaloffset {Type I LastRead 0 FirstWrite -1}
		capsulemetadata_localoffset {Type I LastRead 0 FirstWrite -1}}
	readkeyvalues0 {
		kvdram_V {Type I LastRead 9 FirstWrite -1}
		kvdram_V_offset {Type I LastRead 0 FirstWrite -1}
		buffer_V {Type O LastRead -1 FirstWrite 10}
		offset_kvs {Type I LastRead 0 FirstWrite -1}}
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
		offset_kvs {Type I LastRead 0 FirstWrite -1}}
	readkeyvalues0_2 {
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
	readkeyvalues0_158 {
		kvdram_V {Type I LastRead 9 FirstWrite -1}
		kvdram_V_offset {Type I LastRead 0 FirstWrite -1}
		buffer_V {Type O LastRead -1 FirstWrite 10}
		baseaddress {Type I LastRead 0 FirstWrite -1}
		offset_kvs {Type I LastRead 0 FirstWrite -1}
		size_kvs {Type I LastRead 1 FirstWrite -1}}
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
		size_kvs {Type I LastRead 1 FirstWrite -1}}
	savekeyvalues0_153 {
		kvdram_V {Type O LastRead 3 FirstWrite 4}
		kvdram_V_offset {Type I LastRead 0 FirstWrite -1}
		buffer_V {Type I LastRead 2 FirstWrite -1}
		offset_kvs {Type I LastRead 0 FirstWrite -1}}
	readkeyvalues0_157 {
		kvdram_V {Type I LastRead 9 FirstWrite -1}
		kvdram_V_offset {Type I LastRead 0 FirstWrite -1}
		buffer_V {Type O LastRead -1 FirstWrite 10}
		offset_kvs {Type I LastRead 0 FirstWrite -1}}
	savekeyvalues0_2 {
		dram {Type O LastRead 3 FirstWrite 4}
		dram_offset {Type I LastRead 0 FirstWrite -1}
		buffer_key {Type I LastRead 2 FirstWrite -1}
		buffer_value {Type I LastRead 2 FirstWrite -1}
		offset_kvs {Type I LastRead 0 FirstWrite -1}}
	savekeyvalues0_1 {
		dram {Type O LastRead 3 FirstWrite 2}
		dram_offset {Type I LastRead 0 FirstWrite -1}
		buffer_0_key_read {Type I LastRead 0 FirstWrite -1}
		buffer_0_value_read {Type I LastRead 0 FirstWrite -1}
		offset_kvs {Type I LastRead 0 FirstWrite -1}}}

set hasDtUnsupportedChannel 0

set PerformanceInfo {[
	{"Name" : "Latency", "Min" : "41", "Max" : "197852388"}
	, {"Name" : "Interval", "Min" : "41", "Max" : "197852388"}
]}

set PipelineEnableSignalInfo {[
]}

set Spec2ImplPortList { 
	kvsourcedram_V { m_axi {  { m_axi_kvsourcedram_V_AWVALID VALID 1 1 }  { m_axi_kvsourcedram_V_AWREADY READY 0 1 }  { m_axi_kvsourcedram_V_AWADDR ADDR 1 32 }  { m_axi_kvsourcedram_V_AWID ID 1 1 }  { m_axi_kvsourcedram_V_AWLEN LEN 1 32 }  { m_axi_kvsourcedram_V_AWSIZE SIZE 1 3 }  { m_axi_kvsourcedram_V_AWBURST BURST 1 2 }  { m_axi_kvsourcedram_V_AWLOCK LOCK 1 2 }  { m_axi_kvsourcedram_V_AWCACHE CACHE 1 4 }  { m_axi_kvsourcedram_V_AWPROT PROT 1 3 }  { m_axi_kvsourcedram_V_AWQOS QOS 1 4 }  { m_axi_kvsourcedram_V_AWREGION REGION 1 4 }  { m_axi_kvsourcedram_V_AWUSER USER 1 1 }  { m_axi_kvsourcedram_V_WVALID VALID 1 1 }  { m_axi_kvsourcedram_V_WREADY READY 0 1 }  { m_axi_kvsourcedram_V_WDATA DATA 1 512 }  { m_axi_kvsourcedram_V_WSTRB STRB 1 64 }  { m_axi_kvsourcedram_V_WLAST LAST 1 1 }  { m_axi_kvsourcedram_V_WID ID 1 1 }  { m_axi_kvsourcedram_V_WUSER USER 1 1 }  { m_axi_kvsourcedram_V_ARVALID VALID 1 1 }  { m_axi_kvsourcedram_V_ARREADY READY 0 1 }  { m_axi_kvsourcedram_V_ARADDR ADDR 1 32 }  { m_axi_kvsourcedram_V_ARID ID 1 1 }  { m_axi_kvsourcedram_V_ARLEN LEN 1 32 }  { m_axi_kvsourcedram_V_ARSIZE SIZE 1 3 }  { m_axi_kvsourcedram_V_ARBURST BURST 1 2 }  { m_axi_kvsourcedram_V_ARLOCK LOCK 1 2 }  { m_axi_kvsourcedram_V_ARCACHE CACHE 1 4 }  { m_axi_kvsourcedram_V_ARPROT PROT 1 3 }  { m_axi_kvsourcedram_V_ARQOS QOS 1 4 }  { m_axi_kvsourcedram_V_ARREGION REGION 1 4 }  { m_axi_kvsourcedram_V_ARUSER USER 1 1 }  { m_axi_kvsourcedram_V_RVALID VALID 0 1 }  { m_axi_kvsourcedram_V_RREADY READY 1 1 }  { m_axi_kvsourcedram_V_RDATA DATA 0 512 }  { m_axi_kvsourcedram_V_RLAST LAST 0 1 }  { m_axi_kvsourcedram_V_RID ID 0 1 }  { m_axi_kvsourcedram_V_RUSER USER 0 1 }  { m_axi_kvsourcedram_V_RRESP RESP 0 2 }  { m_axi_kvsourcedram_V_BVALID VALID 0 1 }  { m_axi_kvsourcedram_V_BREADY READY 1 1 }  { m_axi_kvsourcedram_V_BRESP RESP 0 2 }  { m_axi_kvsourcedram_V_BID ID 0 1 }  { m_axi_kvsourcedram_V_BUSER USER 0 1 } } }
	kvsourcedram_V_offset { ap_none {  { kvsourcedram_V_offset in_data 0 26 } } }
	kvdestdram_V { m_axi {  { m_axi_kvdestdram_V_AWVALID VALID 1 1 }  { m_axi_kvdestdram_V_AWREADY READY 0 1 }  { m_axi_kvdestdram_V_AWADDR ADDR 1 32 }  { m_axi_kvdestdram_V_AWID ID 1 1 }  { m_axi_kvdestdram_V_AWLEN LEN 1 32 }  { m_axi_kvdestdram_V_AWSIZE SIZE 1 3 }  { m_axi_kvdestdram_V_AWBURST BURST 1 2 }  { m_axi_kvdestdram_V_AWLOCK LOCK 1 2 }  { m_axi_kvdestdram_V_AWCACHE CACHE 1 4 }  { m_axi_kvdestdram_V_AWPROT PROT 1 3 }  { m_axi_kvdestdram_V_AWQOS QOS 1 4 }  { m_axi_kvdestdram_V_AWREGION REGION 1 4 }  { m_axi_kvdestdram_V_AWUSER USER 1 1 }  { m_axi_kvdestdram_V_WVALID VALID 1 1 }  { m_axi_kvdestdram_V_WREADY READY 0 1 }  { m_axi_kvdestdram_V_WDATA DATA 1 512 }  { m_axi_kvdestdram_V_WSTRB STRB 1 64 }  { m_axi_kvdestdram_V_WLAST LAST 1 1 }  { m_axi_kvdestdram_V_WID ID 1 1 }  { m_axi_kvdestdram_V_WUSER USER 1 1 }  { m_axi_kvdestdram_V_ARVALID VALID 1 1 }  { m_axi_kvdestdram_V_ARREADY READY 0 1 }  { m_axi_kvdestdram_V_ARADDR ADDR 1 32 }  { m_axi_kvdestdram_V_ARID ID 1 1 }  { m_axi_kvdestdram_V_ARLEN LEN 1 32 }  { m_axi_kvdestdram_V_ARSIZE SIZE 1 3 }  { m_axi_kvdestdram_V_ARBURST BURST 1 2 }  { m_axi_kvdestdram_V_ARLOCK LOCK 1 2 }  { m_axi_kvdestdram_V_ARCACHE CACHE 1 4 }  { m_axi_kvdestdram_V_ARPROT PROT 1 3 }  { m_axi_kvdestdram_V_ARQOS QOS 1 4 }  { m_axi_kvdestdram_V_ARREGION REGION 1 4 }  { m_axi_kvdestdram_V_ARUSER USER 1 1 }  { m_axi_kvdestdram_V_RVALID VALID 0 1 }  { m_axi_kvdestdram_V_RREADY READY 1 1 }  { m_axi_kvdestdram_V_RDATA DATA 0 512 }  { m_axi_kvdestdram_V_RLAST LAST 0 1 }  { m_axi_kvdestdram_V_RID ID 0 1 }  { m_axi_kvdestdram_V_RUSER USER 0 1 }  { m_axi_kvdestdram_V_RRESP RESP 0 2 }  { m_axi_kvdestdram_V_BVALID VALID 0 1 }  { m_axi_kvdestdram_V_BREADY READY 1 1 }  { m_axi_kvdestdram_V_BRESP RESP 0 2 }  { m_axi_kvdestdram_V_BID ID 0 1 }  { m_axi_kvdestdram_V_BUSER USER 0 1 } } }
	kvdestdram_V_offset { ap_none {  { kvdestdram_V_offset in_data 0 26 } } }
	kvstats { m_axi {  { m_axi_kvstats_AWVALID VALID 1 1 }  { m_axi_kvstats_AWREADY READY 0 1 }  { m_axi_kvstats_AWADDR ADDR 1 32 }  { m_axi_kvstats_AWID ID 1 1 }  { m_axi_kvstats_AWLEN LEN 1 32 }  { m_axi_kvstats_AWSIZE SIZE 1 3 }  { m_axi_kvstats_AWBURST BURST 1 2 }  { m_axi_kvstats_AWLOCK LOCK 1 2 }  { m_axi_kvstats_AWCACHE CACHE 1 4 }  { m_axi_kvstats_AWPROT PROT 1 3 }  { m_axi_kvstats_AWQOS QOS 1 4 }  { m_axi_kvstats_AWREGION REGION 1 4 }  { m_axi_kvstats_AWUSER USER 1 1 }  { m_axi_kvstats_WVALID VALID 1 1 }  { m_axi_kvstats_WREADY READY 0 1 }  { m_axi_kvstats_WDATA DATA 1 64 }  { m_axi_kvstats_WSTRB STRB 1 8 }  { m_axi_kvstats_WLAST LAST 1 1 }  { m_axi_kvstats_WID ID 1 1 }  { m_axi_kvstats_WUSER USER 1 1 }  { m_axi_kvstats_ARVALID VALID 1 1 }  { m_axi_kvstats_ARREADY READY 0 1 }  { m_axi_kvstats_ARADDR ADDR 1 32 }  { m_axi_kvstats_ARID ID 1 1 }  { m_axi_kvstats_ARLEN LEN 1 32 }  { m_axi_kvstats_ARSIZE SIZE 1 3 }  { m_axi_kvstats_ARBURST BURST 1 2 }  { m_axi_kvstats_ARLOCK LOCK 1 2 }  { m_axi_kvstats_ARCACHE CACHE 1 4 }  { m_axi_kvstats_ARPROT PROT 1 3 }  { m_axi_kvstats_ARQOS QOS 1 4 }  { m_axi_kvstats_ARREGION REGION 1 4 }  { m_axi_kvstats_ARUSER USER 1 1 }  { m_axi_kvstats_RVALID VALID 0 1 }  { m_axi_kvstats_RREADY READY 1 1 }  { m_axi_kvstats_RDATA DATA 0 64 }  { m_axi_kvstats_RLAST LAST 0 1 }  { m_axi_kvstats_RID ID 0 1 }  { m_axi_kvstats_RUSER USER 0 1 }  { m_axi_kvstats_RRESP RESP 0 2 }  { m_axi_kvstats_BVALID VALID 0 1 }  { m_axi_kvstats_BREADY READY 1 1 }  { m_axi_kvstats_BRESP RESP 0 2 }  { m_axi_kvstats_BID ID 0 1 }  { m_axi_kvstats_BUSER USER 0 1 } } }
	kvstats_offset { ap_none {  { kvstats_offset in_data 0 29 } } }
	globalparams_runkernelcommand { ap_none {  { globalparams_runkernelcommand in_data 0 32 } } }
	globalparams_partitioncommand { ap_none {  { globalparams_partitioncommand in_data 0 32 } } }
	globalparams_reducecommand { ap_none {  { globalparams_reducecommand in_data 0 32 } } }
	globalparams_treedepth { ap_none {  { globalparams_treedepth in_data 0 32 } } }
	globalparams_runsize { ap_none {  { globalparams_runsize in_data 0 32 } } }
	globalparams_vbegin { ap_none {  { globalparams_vbegin in_data 0 32 } } }
	globalparams_statsalreadycollected { ap_none {  { globalparams_statsalreadycollected in_data 0 32 } } }
}
set moduleName partitionandreduce0
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
set C_modelName {partitionandreduce0}
set C_modelType { void 0 }
set C_modelArgList {
	{ kvsourcedram_V int 512 regular {axi_master 2}  }
	{ kvsourcedram_V_offset int 26 regular  }
	{ kvdestdram_V int 512 regular {axi_master 2}  }
	{ kvdestdram_V_offset int 26 regular  }
	{ kvstats int 64 regular {axi_master 2}  }
	{ kvstats_offset int 29 regular  }
	{ globalparams_runkernelcommand int 32 regular  }
	{ globalparams_partitioncommand int 32 regular  }
	{ globalparams_reducecommand int 32 regular  }
	{ globalparams_treedepth int 32 regular  }
	{ globalparams_runsize int 32 regular  }
	{ globalparams_vbegin int 32 regular  }
	{ globalparams_statsalreadycollected int 32 regular  }
}
set C_modelArgMapList {[ 
	{ "Name" : "kvsourcedram_V", "interface" : "axi_master", "bitwidth" : 512, "direction" : "READWRITE"} , 
 	{ "Name" : "kvsourcedram_V_offset", "interface" : "wire", "bitwidth" : 26, "direction" : "READONLY"} , 
 	{ "Name" : "kvdestdram_V", "interface" : "axi_master", "bitwidth" : 512, "direction" : "READWRITE"} , 
 	{ "Name" : "kvdestdram_V_offset", "interface" : "wire", "bitwidth" : 26, "direction" : "READONLY"} , 
 	{ "Name" : "kvstats", "interface" : "axi_master", "bitwidth" : 64, "direction" : "READWRITE"} , 
 	{ "Name" : "kvstats_offset", "interface" : "wire", "bitwidth" : 29, "direction" : "READONLY"} , 
 	{ "Name" : "globalparams_runkernelcommand", "interface" : "wire", "bitwidth" : 32, "direction" : "READONLY"} , 
 	{ "Name" : "globalparams_partitioncommand", "interface" : "wire", "bitwidth" : 32, "direction" : "READONLY"} , 
 	{ "Name" : "globalparams_reducecommand", "interface" : "wire", "bitwidth" : 32, "direction" : "READONLY"} , 
 	{ "Name" : "globalparams_treedepth", "interface" : "wire", "bitwidth" : 32, "direction" : "READONLY"} , 
 	{ "Name" : "globalparams_runsize", "interface" : "wire", "bitwidth" : 32, "direction" : "READONLY"} , 
 	{ "Name" : "globalparams_vbegin", "interface" : "wire", "bitwidth" : 32, "direction" : "READONLY"} , 
 	{ "Name" : "globalparams_statsalreadycollected", "interface" : "wire", "bitwidth" : 32, "direction" : "READONLY"} ]}
# RTL Port declarations: 
set portNum 151
set portList { 
	{ ap_clk sc_in sc_logic 1 clock -1 } 
	{ ap_rst sc_in sc_logic 1 reset -1 active_high_sync } 
	{ ap_start sc_in sc_logic 1 start -1 } 
	{ ap_done sc_out sc_logic 1 predone -1 } 
	{ ap_idle sc_out sc_logic 1 done -1 } 
	{ ap_ready sc_out sc_logic 1 ready -1 } 
	{ m_axi_kvsourcedram_V_AWVALID sc_out sc_logic 1 signal 0 } 
	{ m_axi_kvsourcedram_V_AWREADY sc_in sc_logic 1 signal 0 } 
	{ m_axi_kvsourcedram_V_AWADDR sc_out sc_lv 32 signal 0 } 
	{ m_axi_kvsourcedram_V_AWID sc_out sc_lv 1 signal 0 } 
	{ m_axi_kvsourcedram_V_AWLEN sc_out sc_lv 32 signal 0 } 
	{ m_axi_kvsourcedram_V_AWSIZE sc_out sc_lv 3 signal 0 } 
	{ m_axi_kvsourcedram_V_AWBURST sc_out sc_lv 2 signal 0 } 
	{ m_axi_kvsourcedram_V_AWLOCK sc_out sc_lv 2 signal 0 } 
	{ m_axi_kvsourcedram_V_AWCACHE sc_out sc_lv 4 signal 0 } 
	{ m_axi_kvsourcedram_V_AWPROT sc_out sc_lv 3 signal 0 } 
	{ m_axi_kvsourcedram_V_AWQOS sc_out sc_lv 4 signal 0 } 
	{ m_axi_kvsourcedram_V_AWREGION sc_out sc_lv 4 signal 0 } 
	{ m_axi_kvsourcedram_V_AWUSER sc_out sc_lv 1 signal 0 } 
	{ m_axi_kvsourcedram_V_WVALID sc_out sc_logic 1 signal 0 } 
	{ m_axi_kvsourcedram_V_WREADY sc_in sc_logic 1 signal 0 } 
	{ m_axi_kvsourcedram_V_WDATA sc_out sc_lv 512 signal 0 } 
	{ m_axi_kvsourcedram_V_WSTRB sc_out sc_lv 64 signal 0 } 
	{ m_axi_kvsourcedram_V_WLAST sc_out sc_logic 1 signal 0 } 
	{ m_axi_kvsourcedram_V_WID sc_out sc_lv 1 signal 0 } 
	{ m_axi_kvsourcedram_V_WUSER sc_out sc_lv 1 signal 0 } 
	{ m_axi_kvsourcedram_V_ARVALID sc_out sc_logic 1 signal 0 } 
	{ m_axi_kvsourcedram_V_ARREADY sc_in sc_logic 1 signal 0 } 
	{ m_axi_kvsourcedram_V_ARADDR sc_out sc_lv 32 signal 0 } 
	{ m_axi_kvsourcedram_V_ARID sc_out sc_lv 1 signal 0 } 
	{ m_axi_kvsourcedram_V_ARLEN sc_out sc_lv 32 signal 0 } 
	{ m_axi_kvsourcedram_V_ARSIZE sc_out sc_lv 3 signal 0 } 
	{ m_axi_kvsourcedram_V_ARBURST sc_out sc_lv 2 signal 0 } 
	{ m_axi_kvsourcedram_V_ARLOCK sc_out sc_lv 2 signal 0 } 
	{ m_axi_kvsourcedram_V_ARCACHE sc_out sc_lv 4 signal 0 } 
	{ m_axi_kvsourcedram_V_ARPROT sc_out sc_lv 3 signal 0 } 
	{ m_axi_kvsourcedram_V_ARQOS sc_out sc_lv 4 signal 0 } 
	{ m_axi_kvsourcedram_V_ARREGION sc_out sc_lv 4 signal 0 } 
	{ m_axi_kvsourcedram_V_ARUSER sc_out sc_lv 1 signal 0 } 
	{ m_axi_kvsourcedram_V_RVALID sc_in sc_logic 1 signal 0 } 
	{ m_axi_kvsourcedram_V_RREADY sc_out sc_logic 1 signal 0 } 
	{ m_axi_kvsourcedram_V_RDATA sc_in sc_lv 512 signal 0 } 
	{ m_axi_kvsourcedram_V_RLAST sc_in sc_logic 1 signal 0 } 
	{ m_axi_kvsourcedram_V_RID sc_in sc_lv 1 signal 0 } 
	{ m_axi_kvsourcedram_V_RUSER sc_in sc_lv 1 signal 0 } 
	{ m_axi_kvsourcedram_V_RRESP sc_in sc_lv 2 signal 0 } 
	{ m_axi_kvsourcedram_V_BVALID sc_in sc_logic 1 signal 0 } 
	{ m_axi_kvsourcedram_V_BREADY sc_out sc_logic 1 signal 0 } 
	{ m_axi_kvsourcedram_V_BRESP sc_in sc_lv 2 signal 0 } 
	{ m_axi_kvsourcedram_V_BID sc_in sc_lv 1 signal 0 } 
	{ m_axi_kvsourcedram_V_BUSER sc_in sc_lv 1 signal 0 } 
	{ kvsourcedram_V_offset sc_in sc_lv 26 signal 1 } 
	{ m_axi_kvdestdram_V_AWVALID sc_out sc_logic 1 signal 2 } 
	{ m_axi_kvdestdram_V_AWREADY sc_in sc_logic 1 signal 2 } 
	{ m_axi_kvdestdram_V_AWADDR sc_out sc_lv 32 signal 2 } 
	{ m_axi_kvdestdram_V_AWID sc_out sc_lv 1 signal 2 } 
	{ m_axi_kvdestdram_V_AWLEN sc_out sc_lv 32 signal 2 } 
	{ m_axi_kvdestdram_V_AWSIZE sc_out sc_lv 3 signal 2 } 
	{ m_axi_kvdestdram_V_AWBURST sc_out sc_lv 2 signal 2 } 
	{ m_axi_kvdestdram_V_AWLOCK sc_out sc_lv 2 signal 2 } 
	{ m_axi_kvdestdram_V_AWCACHE sc_out sc_lv 4 signal 2 } 
	{ m_axi_kvdestdram_V_AWPROT sc_out sc_lv 3 signal 2 } 
	{ m_axi_kvdestdram_V_AWQOS sc_out sc_lv 4 signal 2 } 
	{ m_axi_kvdestdram_V_AWREGION sc_out sc_lv 4 signal 2 } 
	{ m_axi_kvdestdram_V_AWUSER sc_out sc_lv 1 signal 2 } 
	{ m_axi_kvdestdram_V_WVALID sc_out sc_logic 1 signal 2 } 
	{ m_axi_kvdestdram_V_WREADY sc_in sc_logic 1 signal 2 } 
	{ m_axi_kvdestdram_V_WDATA sc_out sc_lv 512 signal 2 } 
	{ m_axi_kvdestdram_V_WSTRB sc_out sc_lv 64 signal 2 } 
	{ m_axi_kvdestdram_V_WLAST sc_out sc_logic 1 signal 2 } 
	{ m_axi_kvdestdram_V_WID sc_out sc_lv 1 signal 2 } 
	{ m_axi_kvdestdram_V_WUSER sc_out sc_lv 1 signal 2 } 
	{ m_axi_kvdestdram_V_ARVALID sc_out sc_logic 1 signal 2 } 
	{ m_axi_kvdestdram_V_ARREADY sc_in sc_logic 1 signal 2 } 
	{ m_axi_kvdestdram_V_ARADDR sc_out sc_lv 32 signal 2 } 
	{ m_axi_kvdestdram_V_ARID sc_out sc_lv 1 signal 2 } 
	{ m_axi_kvdestdram_V_ARLEN sc_out sc_lv 32 signal 2 } 
	{ m_axi_kvdestdram_V_ARSIZE sc_out sc_lv 3 signal 2 } 
	{ m_axi_kvdestdram_V_ARBURST sc_out sc_lv 2 signal 2 } 
	{ m_axi_kvdestdram_V_ARLOCK sc_out sc_lv 2 signal 2 } 
	{ m_axi_kvdestdram_V_ARCACHE sc_out sc_lv 4 signal 2 } 
	{ m_axi_kvdestdram_V_ARPROT sc_out sc_lv 3 signal 2 } 
	{ m_axi_kvdestdram_V_ARQOS sc_out sc_lv 4 signal 2 } 
	{ m_axi_kvdestdram_V_ARREGION sc_out sc_lv 4 signal 2 } 
	{ m_axi_kvdestdram_V_ARUSER sc_out sc_lv 1 signal 2 } 
	{ m_axi_kvdestdram_V_RVALID sc_in sc_logic 1 signal 2 } 
	{ m_axi_kvdestdram_V_RREADY sc_out sc_logic 1 signal 2 } 
	{ m_axi_kvdestdram_V_RDATA sc_in sc_lv 512 signal 2 } 
	{ m_axi_kvdestdram_V_RLAST sc_in sc_logic 1 signal 2 } 
	{ m_axi_kvdestdram_V_RID sc_in sc_lv 1 signal 2 } 
	{ m_axi_kvdestdram_V_RUSER sc_in sc_lv 1 signal 2 } 
	{ m_axi_kvdestdram_V_RRESP sc_in sc_lv 2 signal 2 } 
	{ m_axi_kvdestdram_V_BVALID sc_in sc_logic 1 signal 2 } 
	{ m_axi_kvdestdram_V_BREADY sc_out sc_logic 1 signal 2 } 
	{ m_axi_kvdestdram_V_BRESP sc_in sc_lv 2 signal 2 } 
	{ m_axi_kvdestdram_V_BID sc_in sc_lv 1 signal 2 } 
	{ m_axi_kvdestdram_V_BUSER sc_in sc_lv 1 signal 2 } 
	{ kvdestdram_V_offset sc_in sc_lv 26 signal 3 } 
	{ m_axi_kvstats_AWVALID sc_out sc_logic 1 signal 4 } 
	{ m_axi_kvstats_AWREADY sc_in sc_logic 1 signal 4 } 
	{ m_axi_kvstats_AWADDR sc_out sc_lv 32 signal 4 } 
	{ m_axi_kvstats_AWID sc_out sc_lv 1 signal 4 } 
	{ m_axi_kvstats_AWLEN sc_out sc_lv 32 signal 4 } 
	{ m_axi_kvstats_AWSIZE sc_out sc_lv 3 signal 4 } 
	{ m_axi_kvstats_AWBURST sc_out sc_lv 2 signal 4 } 
	{ m_axi_kvstats_AWLOCK sc_out sc_lv 2 signal 4 } 
	{ m_axi_kvstats_AWCACHE sc_out sc_lv 4 signal 4 } 
	{ m_axi_kvstats_AWPROT sc_out sc_lv 3 signal 4 } 
	{ m_axi_kvstats_AWQOS sc_out sc_lv 4 signal 4 } 
	{ m_axi_kvstats_AWREGION sc_out sc_lv 4 signal 4 } 
	{ m_axi_kvstats_AWUSER sc_out sc_lv 1 signal 4 } 
	{ m_axi_kvstats_WVALID sc_out sc_logic 1 signal 4 } 
	{ m_axi_kvstats_WREADY sc_in sc_logic 1 signal 4 } 
	{ m_axi_kvstats_WDATA sc_out sc_lv 64 signal 4 } 
	{ m_axi_kvstats_WSTRB sc_out sc_lv 8 signal 4 } 
	{ m_axi_kvstats_WLAST sc_out sc_logic 1 signal 4 } 
	{ m_axi_kvstats_WID sc_out sc_lv 1 signal 4 } 
	{ m_axi_kvstats_WUSER sc_out sc_lv 1 signal 4 } 
	{ m_axi_kvstats_ARVALID sc_out sc_logic 1 signal 4 } 
	{ m_axi_kvstats_ARREADY sc_in sc_logic 1 signal 4 } 
	{ m_axi_kvstats_ARADDR sc_out sc_lv 32 signal 4 } 
	{ m_axi_kvstats_ARID sc_out sc_lv 1 signal 4 } 
	{ m_axi_kvstats_ARLEN sc_out sc_lv 32 signal 4 } 
	{ m_axi_kvstats_ARSIZE sc_out sc_lv 3 signal 4 } 
	{ m_axi_kvstats_ARBURST sc_out sc_lv 2 signal 4 } 
	{ m_axi_kvstats_ARLOCK sc_out sc_lv 2 signal 4 } 
	{ m_axi_kvstats_ARCACHE sc_out sc_lv 4 signal 4 } 
	{ m_axi_kvstats_ARPROT sc_out sc_lv 3 signal 4 } 
	{ m_axi_kvstats_ARQOS sc_out sc_lv 4 signal 4 } 
	{ m_axi_kvstats_ARREGION sc_out sc_lv 4 signal 4 } 
	{ m_axi_kvstats_ARUSER sc_out sc_lv 1 signal 4 } 
	{ m_axi_kvstats_RVALID sc_in sc_logic 1 signal 4 } 
	{ m_axi_kvstats_RREADY sc_out sc_logic 1 signal 4 } 
	{ m_axi_kvstats_RDATA sc_in sc_lv 64 signal 4 } 
	{ m_axi_kvstats_RLAST sc_in sc_logic 1 signal 4 } 
	{ m_axi_kvstats_RID sc_in sc_lv 1 signal 4 } 
	{ m_axi_kvstats_RUSER sc_in sc_lv 1 signal 4 } 
	{ m_axi_kvstats_RRESP sc_in sc_lv 2 signal 4 } 
	{ m_axi_kvstats_BVALID sc_in sc_logic 1 signal 4 } 
	{ m_axi_kvstats_BREADY sc_out sc_logic 1 signal 4 } 
	{ m_axi_kvstats_BRESP sc_in sc_lv 2 signal 4 } 
	{ m_axi_kvstats_BID sc_in sc_lv 1 signal 4 } 
	{ m_axi_kvstats_BUSER sc_in sc_lv 1 signal 4 } 
	{ kvstats_offset sc_in sc_lv 29 signal 5 } 
	{ globalparams_runkernelcommand sc_in sc_lv 32 signal 6 } 
	{ globalparams_partitioncommand sc_in sc_lv 32 signal 7 } 
	{ globalparams_reducecommand sc_in sc_lv 32 signal 8 } 
	{ globalparams_treedepth sc_in sc_lv 32 signal 9 } 
	{ globalparams_runsize sc_in sc_lv 32 signal 10 } 
	{ globalparams_vbegin sc_in sc_lv 32 signal 11 } 
	{ globalparams_statsalreadycollected sc_in sc_lv 32 signal 12 } 
}
set NewPortList {[ 
	{ "name": "ap_clk", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "clock", "bundle":{"name": "ap_clk", "role": "default" }} , 
 	{ "name": "ap_rst", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "reset", "bundle":{"name": "ap_rst", "role": "default" }} , 
 	{ "name": "ap_start", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "start", "bundle":{"name": "ap_start", "role": "default" }} , 
 	{ "name": "ap_done", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "predone", "bundle":{"name": "ap_done", "role": "default" }} , 
 	{ "name": "ap_idle", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "done", "bundle":{"name": "ap_idle", "role": "default" }} , 
 	{ "name": "ap_ready", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "ready", "bundle":{"name": "ap_ready", "role": "default" }} , 
 	{ "name": "m_axi_kvsourcedram_V_AWVALID", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "kvsourcedram_V", "role": "AWVALID" }} , 
 	{ "name": "m_axi_kvsourcedram_V_AWREADY", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "kvsourcedram_V", "role": "AWREADY" }} , 
 	{ "name": "m_axi_kvsourcedram_V_AWADDR", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "kvsourcedram_V", "role": "AWADDR" }} , 
 	{ "name": "m_axi_kvsourcedram_V_AWID", "direction": "out", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "kvsourcedram_V", "role": "AWID" }} , 
 	{ "name": "m_axi_kvsourcedram_V_AWLEN", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "kvsourcedram_V", "role": "AWLEN" }} , 
 	{ "name": "m_axi_kvsourcedram_V_AWSIZE", "direction": "out", "datatype": "sc_lv", "bitwidth":3, "type": "signal", "bundle":{"name": "kvsourcedram_V", "role": "AWSIZE" }} , 
 	{ "name": "m_axi_kvsourcedram_V_AWBURST", "direction": "out", "datatype": "sc_lv", "bitwidth":2, "type": "signal", "bundle":{"name": "kvsourcedram_V", "role": "AWBURST" }} , 
 	{ "name": "m_axi_kvsourcedram_V_AWLOCK", "direction": "out", "datatype": "sc_lv", "bitwidth":2, "type": "signal", "bundle":{"name": "kvsourcedram_V", "role": "AWLOCK" }} , 
 	{ "name": "m_axi_kvsourcedram_V_AWCACHE", "direction": "out", "datatype": "sc_lv", "bitwidth":4, "type": "signal", "bundle":{"name": "kvsourcedram_V", "role": "AWCACHE" }} , 
 	{ "name": "m_axi_kvsourcedram_V_AWPROT", "direction": "out", "datatype": "sc_lv", "bitwidth":3, "type": "signal", "bundle":{"name": "kvsourcedram_V", "role": "AWPROT" }} , 
 	{ "name": "m_axi_kvsourcedram_V_AWQOS", "direction": "out", "datatype": "sc_lv", "bitwidth":4, "type": "signal", "bundle":{"name": "kvsourcedram_V", "role": "AWQOS" }} , 
 	{ "name": "m_axi_kvsourcedram_V_AWREGION", "direction": "out", "datatype": "sc_lv", "bitwidth":4, "type": "signal", "bundle":{"name": "kvsourcedram_V", "role": "AWREGION" }} , 
 	{ "name": "m_axi_kvsourcedram_V_AWUSER", "direction": "out", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "kvsourcedram_V", "role": "AWUSER" }} , 
 	{ "name": "m_axi_kvsourcedram_V_WVALID", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "kvsourcedram_V", "role": "WVALID" }} , 
 	{ "name": "m_axi_kvsourcedram_V_WREADY", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "kvsourcedram_V", "role": "WREADY" }} , 
 	{ "name": "m_axi_kvsourcedram_V_WDATA", "direction": "out", "datatype": "sc_lv", "bitwidth":512, "type": "signal", "bundle":{"name": "kvsourcedram_V", "role": "WDATA" }} , 
 	{ "name": "m_axi_kvsourcedram_V_WSTRB", "direction": "out", "datatype": "sc_lv", "bitwidth":64, "type": "signal", "bundle":{"name": "kvsourcedram_V", "role": "WSTRB" }} , 
 	{ "name": "m_axi_kvsourcedram_V_WLAST", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "kvsourcedram_V", "role": "WLAST" }} , 
 	{ "name": "m_axi_kvsourcedram_V_WID", "direction": "out", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "kvsourcedram_V", "role": "WID" }} , 
 	{ "name": "m_axi_kvsourcedram_V_WUSER", "direction": "out", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "kvsourcedram_V", "role": "WUSER" }} , 
 	{ "name": "m_axi_kvsourcedram_V_ARVALID", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "kvsourcedram_V", "role": "ARVALID" }} , 
 	{ "name": "m_axi_kvsourcedram_V_ARREADY", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "kvsourcedram_V", "role": "ARREADY" }} , 
 	{ "name": "m_axi_kvsourcedram_V_ARADDR", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "kvsourcedram_V", "role": "ARADDR" }} , 
 	{ "name": "m_axi_kvsourcedram_V_ARID", "direction": "out", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "kvsourcedram_V", "role": "ARID" }} , 
 	{ "name": "m_axi_kvsourcedram_V_ARLEN", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "kvsourcedram_V", "role": "ARLEN" }} , 
 	{ "name": "m_axi_kvsourcedram_V_ARSIZE", "direction": "out", "datatype": "sc_lv", "bitwidth":3, "type": "signal", "bundle":{"name": "kvsourcedram_V", "role": "ARSIZE" }} , 
 	{ "name": "m_axi_kvsourcedram_V_ARBURST", "direction": "out", "datatype": "sc_lv", "bitwidth":2, "type": "signal", "bundle":{"name": "kvsourcedram_V", "role": "ARBURST" }} , 
 	{ "name": "m_axi_kvsourcedram_V_ARLOCK", "direction": "out", "datatype": "sc_lv", "bitwidth":2, "type": "signal", "bundle":{"name": "kvsourcedram_V", "role": "ARLOCK" }} , 
 	{ "name": "m_axi_kvsourcedram_V_ARCACHE", "direction": "out", "datatype": "sc_lv", "bitwidth":4, "type": "signal", "bundle":{"name": "kvsourcedram_V", "role": "ARCACHE" }} , 
 	{ "name": "m_axi_kvsourcedram_V_ARPROT", "direction": "out", "datatype": "sc_lv", "bitwidth":3, "type": "signal", "bundle":{"name": "kvsourcedram_V", "role": "ARPROT" }} , 
 	{ "name": "m_axi_kvsourcedram_V_ARQOS", "direction": "out", "datatype": "sc_lv", "bitwidth":4, "type": "signal", "bundle":{"name": "kvsourcedram_V", "role": "ARQOS" }} , 
 	{ "name": "m_axi_kvsourcedram_V_ARREGION", "direction": "out", "datatype": "sc_lv", "bitwidth":4, "type": "signal", "bundle":{"name": "kvsourcedram_V", "role": "ARREGION" }} , 
 	{ "name": "m_axi_kvsourcedram_V_ARUSER", "direction": "out", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "kvsourcedram_V", "role": "ARUSER" }} , 
 	{ "name": "m_axi_kvsourcedram_V_RVALID", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "kvsourcedram_V", "role": "RVALID" }} , 
 	{ "name": "m_axi_kvsourcedram_V_RREADY", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "kvsourcedram_V", "role": "RREADY" }} , 
 	{ "name": "m_axi_kvsourcedram_V_RDATA", "direction": "in", "datatype": "sc_lv", "bitwidth":512, "type": "signal", "bundle":{"name": "kvsourcedram_V", "role": "RDATA" }} , 
 	{ "name": "m_axi_kvsourcedram_V_RLAST", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "kvsourcedram_V", "role": "RLAST" }} , 
 	{ "name": "m_axi_kvsourcedram_V_RID", "direction": "in", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "kvsourcedram_V", "role": "RID" }} , 
 	{ "name": "m_axi_kvsourcedram_V_RUSER", "direction": "in", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "kvsourcedram_V", "role": "RUSER" }} , 
 	{ "name": "m_axi_kvsourcedram_V_RRESP", "direction": "in", "datatype": "sc_lv", "bitwidth":2, "type": "signal", "bundle":{"name": "kvsourcedram_V", "role": "RRESP" }} , 
 	{ "name": "m_axi_kvsourcedram_V_BVALID", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "kvsourcedram_V", "role": "BVALID" }} , 
 	{ "name": "m_axi_kvsourcedram_V_BREADY", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "kvsourcedram_V", "role": "BREADY" }} , 
 	{ "name": "m_axi_kvsourcedram_V_BRESP", "direction": "in", "datatype": "sc_lv", "bitwidth":2, "type": "signal", "bundle":{"name": "kvsourcedram_V", "role": "BRESP" }} , 
 	{ "name": "m_axi_kvsourcedram_V_BID", "direction": "in", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "kvsourcedram_V", "role": "BID" }} , 
 	{ "name": "m_axi_kvsourcedram_V_BUSER", "direction": "in", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "kvsourcedram_V", "role": "BUSER" }} , 
 	{ "name": "kvsourcedram_V_offset", "direction": "in", "datatype": "sc_lv", "bitwidth":26, "type": "signal", "bundle":{"name": "kvsourcedram_V_offset", "role": "default" }} , 
 	{ "name": "m_axi_kvdestdram_V_AWVALID", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "kvdestdram_V", "role": "AWVALID" }} , 
 	{ "name": "m_axi_kvdestdram_V_AWREADY", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "kvdestdram_V", "role": "AWREADY" }} , 
 	{ "name": "m_axi_kvdestdram_V_AWADDR", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "kvdestdram_V", "role": "AWADDR" }} , 
 	{ "name": "m_axi_kvdestdram_V_AWID", "direction": "out", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "kvdestdram_V", "role": "AWID" }} , 
 	{ "name": "m_axi_kvdestdram_V_AWLEN", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "kvdestdram_V", "role": "AWLEN" }} , 
 	{ "name": "m_axi_kvdestdram_V_AWSIZE", "direction": "out", "datatype": "sc_lv", "bitwidth":3, "type": "signal", "bundle":{"name": "kvdestdram_V", "role": "AWSIZE" }} , 
 	{ "name": "m_axi_kvdestdram_V_AWBURST", "direction": "out", "datatype": "sc_lv", "bitwidth":2, "type": "signal", "bundle":{"name": "kvdestdram_V", "role": "AWBURST" }} , 
 	{ "name": "m_axi_kvdestdram_V_AWLOCK", "direction": "out", "datatype": "sc_lv", "bitwidth":2, "type": "signal", "bundle":{"name": "kvdestdram_V", "role": "AWLOCK" }} , 
 	{ "name": "m_axi_kvdestdram_V_AWCACHE", "direction": "out", "datatype": "sc_lv", "bitwidth":4, "type": "signal", "bundle":{"name": "kvdestdram_V", "role": "AWCACHE" }} , 
 	{ "name": "m_axi_kvdestdram_V_AWPROT", "direction": "out", "datatype": "sc_lv", "bitwidth":3, "type": "signal", "bundle":{"name": "kvdestdram_V", "role": "AWPROT" }} , 
 	{ "name": "m_axi_kvdestdram_V_AWQOS", "direction": "out", "datatype": "sc_lv", "bitwidth":4, "type": "signal", "bundle":{"name": "kvdestdram_V", "role": "AWQOS" }} , 
 	{ "name": "m_axi_kvdestdram_V_AWREGION", "direction": "out", "datatype": "sc_lv", "bitwidth":4, "type": "signal", "bundle":{"name": "kvdestdram_V", "role": "AWREGION" }} , 
 	{ "name": "m_axi_kvdestdram_V_AWUSER", "direction": "out", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "kvdestdram_V", "role": "AWUSER" }} , 
 	{ "name": "m_axi_kvdestdram_V_WVALID", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "kvdestdram_V", "role": "WVALID" }} , 
 	{ "name": "m_axi_kvdestdram_V_WREADY", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "kvdestdram_V", "role": "WREADY" }} , 
 	{ "name": "m_axi_kvdestdram_V_WDATA", "direction": "out", "datatype": "sc_lv", "bitwidth":512, "type": "signal", "bundle":{"name": "kvdestdram_V", "role": "WDATA" }} , 
 	{ "name": "m_axi_kvdestdram_V_WSTRB", "direction": "out", "datatype": "sc_lv", "bitwidth":64, "type": "signal", "bundle":{"name": "kvdestdram_V", "role": "WSTRB" }} , 
 	{ "name": "m_axi_kvdestdram_V_WLAST", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "kvdestdram_V", "role": "WLAST" }} , 
 	{ "name": "m_axi_kvdestdram_V_WID", "direction": "out", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "kvdestdram_V", "role": "WID" }} , 
 	{ "name": "m_axi_kvdestdram_V_WUSER", "direction": "out", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "kvdestdram_V", "role": "WUSER" }} , 
 	{ "name": "m_axi_kvdestdram_V_ARVALID", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "kvdestdram_V", "role": "ARVALID" }} , 
 	{ "name": "m_axi_kvdestdram_V_ARREADY", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "kvdestdram_V", "role": "ARREADY" }} , 
 	{ "name": "m_axi_kvdestdram_V_ARADDR", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "kvdestdram_V", "role": "ARADDR" }} , 
 	{ "name": "m_axi_kvdestdram_V_ARID", "direction": "out", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "kvdestdram_V", "role": "ARID" }} , 
 	{ "name": "m_axi_kvdestdram_V_ARLEN", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "kvdestdram_V", "role": "ARLEN" }} , 
 	{ "name": "m_axi_kvdestdram_V_ARSIZE", "direction": "out", "datatype": "sc_lv", "bitwidth":3, "type": "signal", "bundle":{"name": "kvdestdram_V", "role": "ARSIZE" }} , 
 	{ "name": "m_axi_kvdestdram_V_ARBURST", "direction": "out", "datatype": "sc_lv", "bitwidth":2, "type": "signal", "bundle":{"name": "kvdestdram_V", "role": "ARBURST" }} , 
 	{ "name": "m_axi_kvdestdram_V_ARLOCK", "direction": "out", "datatype": "sc_lv", "bitwidth":2, "type": "signal", "bundle":{"name": "kvdestdram_V", "role": "ARLOCK" }} , 
 	{ "name": "m_axi_kvdestdram_V_ARCACHE", "direction": "out", "datatype": "sc_lv", "bitwidth":4, "type": "signal", "bundle":{"name": "kvdestdram_V", "role": "ARCACHE" }} , 
 	{ "name": "m_axi_kvdestdram_V_ARPROT", "direction": "out", "datatype": "sc_lv", "bitwidth":3, "type": "signal", "bundle":{"name": "kvdestdram_V", "role": "ARPROT" }} , 
 	{ "name": "m_axi_kvdestdram_V_ARQOS", "direction": "out", "datatype": "sc_lv", "bitwidth":4, "type": "signal", "bundle":{"name": "kvdestdram_V", "role": "ARQOS" }} , 
 	{ "name": "m_axi_kvdestdram_V_ARREGION", "direction": "out", "datatype": "sc_lv", "bitwidth":4, "type": "signal", "bundle":{"name": "kvdestdram_V", "role": "ARREGION" }} , 
 	{ "name": "m_axi_kvdestdram_V_ARUSER", "direction": "out", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "kvdestdram_V", "role": "ARUSER" }} , 
 	{ "name": "m_axi_kvdestdram_V_RVALID", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "kvdestdram_V", "role": "RVALID" }} , 
 	{ "name": "m_axi_kvdestdram_V_RREADY", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "kvdestdram_V", "role": "RREADY" }} , 
 	{ "name": "m_axi_kvdestdram_V_RDATA", "direction": "in", "datatype": "sc_lv", "bitwidth":512, "type": "signal", "bundle":{"name": "kvdestdram_V", "role": "RDATA" }} , 
 	{ "name": "m_axi_kvdestdram_V_RLAST", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "kvdestdram_V", "role": "RLAST" }} , 
 	{ "name": "m_axi_kvdestdram_V_RID", "direction": "in", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "kvdestdram_V", "role": "RID" }} , 
 	{ "name": "m_axi_kvdestdram_V_RUSER", "direction": "in", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "kvdestdram_V", "role": "RUSER" }} , 
 	{ "name": "m_axi_kvdestdram_V_RRESP", "direction": "in", "datatype": "sc_lv", "bitwidth":2, "type": "signal", "bundle":{"name": "kvdestdram_V", "role": "RRESP" }} , 
 	{ "name": "m_axi_kvdestdram_V_BVALID", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "kvdestdram_V", "role": "BVALID" }} , 
 	{ "name": "m_axi_kvdestdram_V_BREADY", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "kvdestdram_V", "role": "BREADY" }} , 
 	{ "name": "m_axi_kvdestdram_V_BRESP", "direction": "in", "datatype": "sc_lv", "bitwidth":2, "type": "signal", "bundle":{"name": "kvdestdram_V", "role": "BRESP" }} , 
 	{ "name": "m_axi_kvdestdram_V_BID", "direction": "in", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "kvdestdram_V", "role": "BID" }} , 
 	{ "name": "m_axi_kvdestdram_V_BUSER", "direction": "in", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "kvdestdram_V", "role": "BUSER" }} , 
 	{ "name": "kvdestdram_V_offset", "direction": "in", "datatype": "sc_lv", "bitwidth":26, "type": "signal", "bundle":{"name": "kvdestdram_V_offset", "role": "default" }} , 
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
 	{ "name": "globalparams_runkernelcommand", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "globalparams_runkernelcommand", "role": "default" }} , 
 	{ "name": "globalparams_partitioncommand", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "globalparams_partitioncommand", "role": "default" }} , 
 	{ "name": "globalparams_reducecommand", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "globalparams_reducecommand", "role": "default" }} , 
 	{ "name": "globalparams_treedepth", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "globalparams_treedepth", "role": "default" }} , 
 	{ "name": "globalparams_runsize", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "globalparams_runsize", "role": "default" }} , 
 	{ "name": "globalparams_vbegin", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "globalparams_vbegin", "role": "default" }} , 
 	{ "name": "globalparams_statsalreadycollected", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "globalparams_statsalreadycollected", "role": "default" }}  ]}

set RtlHierarchyInfo {[
	{"ID" : "0", "Level" : "0", "Path" : "`AUTOTB_DUT_INST", "Parent" : "", "Child" : ["1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13", "14", "15", "16", "17", "18", "19", "20", "21", "22", "23", "24", "25", "26", "27", "156", "169", "178", "227", "229", "231", "233", "235", "236", "237", "238", "239"],
		"CDFG" : "partitionandreduce0",
		"Protocol" : "ap_ctrl_hs",
		"ControlExist" : "1", "ap_start" : "1", "ap_ready" : "1", "ap_done" : "1", "ap_continue" : "0", "ap_idle" : "1",
		"Pipeline" : "None", "UnalignedPipeline" : "0", "RewindPipeline" : "0", "ProcessNetwork" : "0",
		"II" : "0",
		"VariableLatency" : "1", "ExactLatency" : "-1", "EstimateLatencyMin" : "41", "EstimateLatencyMax" : "197850340",
		"Combinational" : "0",
		"Datapath" : "0",
		"ClockEnable" : "0",
		"HasSubDataflow" : "0",
		"InDataflowNetwork" : "0",
		"HasNonBlockingOperation" : "0",
		"WaitState" : [
			{"State" : "ap_ST_fsm_state75", "FSM" : "ap_CS_fsm", "SubInstance" : "grp_reducepartitions0_fu_1486"},
			{"State" : "ap_ST_fsm_state77", "FSM" : "ap_CS_fsm", "SubInstance" : "grp_reducepartitions0_fu_1486"},
			{"State" : "ap_ST_fsm_state52", "FSM" : "ap_CS_fsm", "SubInstance" : "grp_partitionkeyvalues00_fu_1501"},
			{"State" : "ap_ST_fsm_state55", "FSM" : "ap_CS_fsm", "SubInstance" : "grp_partitionkeyvalues00_fu_1501"},
			{"State" : "ap_ST_fsm_state54", "FSM" : "ap_CS_fsm", "SubInstance" : "grp_partitionkeyvalues00_fu_1501"},
			{"State" : "ap_ST_fsm_state57", "FSM" : "ap_CS_fsm", "SubInstance" : "grp_savepartitions0_fu_1529"},
			{"State" : "ap_ST_fsm_state36", "FSM" : "ap_CS_fsm", "SubInstance" : "grp_collectstats00_fu_1551"},
			{"State" : "ap_ST_fsm_state39", "FSM" : "ap_CS_fsm", "SubInstance" : "grp_collectstats00_fu_1551"},
			{"State" : "ap_ST_fsm_state38", "FSM" : "ap_CS_fsm", "SubInstance" : "grp_collectstats00_fu_1551"},
			{"State" : "ap_ST_fsm_state50", "FSM" : "ap_CS_fsm", "SubInstance" : "grp_readcapsules0_fu_1575"},
			{"State" : "ap_ST_fsm_state41", "FSM" : "ap_CS_fsm", "SubInstance" : "grp_savecapsules0_fu_1599"},
			{"State" : "ap_ST_fsm_state75", "FSM" : "ap_CS_fsm", "SubInstance" : "grp_readkeyvalues0_2_fu_1628"},
			{"State" : "ap_ST_fsm_state77", "FSM" : "ap_CS_fsm", "SubInstance" : "grp_readkeyvalues0_2_fu_1628"},
			{"State" : "ap_ST_fsm_state36", "FSM" : "ap_CS_fsm", "SubInstance" : "grp_readkeyvalues0_1_fu_1650"},
			{"State" : "ap_ST_fsm_state39", "FSM" : "ap_CS_fsm", "SubInstance" : "grp_readkeyvalues0_1_fu_1650"},
			{"State" : "ap_ST_fsm_state38", "FSM" : "ap_CS_fsm", "SubInstance" : "grp_readkeyvalues0_1_fu_1650"},
			{"State" : "ap_ST_fsm_state52", "FSM" : "ap_CS_fsm", "SubInstance" : "grp_readkeyvalues0_1_fu_1650"},
			{"State" : "ap_ST_fsm_state55", "FSM" : "ap_CS_fsm", "SubInstance" : "grp_readkeyvalues0_1_fu_1650"},
			{"State" : "ap_ST_fsm_state54", "FSM" : "ap_CS_fsm", "SubInstance" : "grp_readkeyvalues0_1_fu_1650"},
			{"State" : "ap_ST_fsm_state78", "FSM" : "ap_CS_fsm", "SubInstance" : "grp_savekeyvalues0_153_fu_1667"},
			{"State" : "ap_ST_fsm_state80", "FSM" : "ap_CS_fsm", "SubInstance" : "grp_savekeyvalues0_153_fu_1667"},
			{"State" : "ap_ST_fsm_state82", "FSM" : "ap_CS_fsm", "SubInstance" : "grp_savekeyvalues0_153_fu_1667"},
			{"State" : "ap_ST_fsm_state84", "FSM" : "ap_CS_fsm", "SubInstance" : "grp_savekeyvalues0_153_fu_1667"},
			{"State" : "ap_ST_fsm_state67", "FSM" : "ap_CS_fsm", "SubInstance" : "grp_readkeyvalues0_157_fu_1676"},
			{"State" : "ap_ST_fsm_state69", "FSM" : "ap_CS_fsm", "SubInstance" : "grp_readkeyvalues0_157_fu_1676"},
			{"State" : "ap_ST_fsm_state71", "FSM" : "ap_CS_fsm", "SubInstance" : "grp_readkeyvalues0_157_fu_1676"},
			{"State" : "ap_ST_fsm_state73", "FSM" : "ap_CS_fsm", "SubInstance" : "grp_readkeyvalues0_157_fu_1676"},
			{"State" : "ap_ST_fsm_state93", "FSM" : "ap_CS_fsm", "SubInstance" : "grp_savekeyvalues0_2_fu_1685"}],
		"Port" : [
			{"Name" : "kvsourcedram_V", "Type" : "MAXI", "Direction" : "IO",
				"SubConnect" : [
					{"ID" : "229", "SubInstance" : "grp_savecapsules0_fu_1599", "Port" : "kvdram_V"},
					{"ID" : "233", "SubInstance" : "grp_readkeyvalues0_1_fu_1650", "Port" : "kvdram_V"},
					{"ID" : "169", "SubInstance" : "grp_savepartitions0_fu_1529", "Port" : "kvdram_V"},
					{"ID" : "231", "SubInstance" : "grp_readkeyvalues0_2_fu_1628", "Port" : "kvdram_V"},
					{"ID" : "227", "SubInstance" : "grp_readcapsules0_fu_1575", "Port" : "kvdram_V"}]},
			{"Name" : "kvsourcedram_V_offset", "Type" : "None", "Direction" : "I"},
			{"Name" : "kvdestdram_V", "Type" : "MAXI", "Direction" : "IO",
				"SubConnect" : [
					{"ID" : "236", "SubInstance" : "grp_readkeyvalues0_157_fu_1676", "Port" : "kvdram_V"},
					{"ID" : "235", "SubInstance" : "grp_savekeyvalues0_153_fu_1667", "Port" : "kvdram_V"}]},
			{"Name" : "kvdestdram_V_offset", "Type" : "None", "Direction" : "I"},
			{"Name" : "kvstats", "Type" : "MAXI", "Direction" : "IO",
				"BlockSignal" : [
					{"Name" : "kvstats_blk_n_AW", "Type" : "RtlSignal"},
					{"Name" : "kvstats_blk_n_W", "Type" : "RtlSignal"},
					{"Name" : "kvstats_blk_n_B", "Type" : "RtlSignal"},
					{"Name" : "kvstats_blk_n_AR", "Type" : "RtlSignal"},
					{"Name" : "kvstats_blk_n_R", "Type" : "RtlSignal"}],
				"SubConnect" : [
					{"ID" : "238", "SubInstance" : "grp_savekeyvalues0_1_fu_1695", "Port" : "dram"},
					{"ID" : "237", "SubInstance" : "grp_savekeyvalues0_2_fu_1685", "Port" : "dram"}]},
			{"Name" : "kvstats_offset", "Type" : "None", "Direction" : "I"},
			{"Name" : "globalparams_runkernelcommand", "Type" : "None", "Direction" : "I"},
			{"Name" : "globalparams_partitioncommand", "Type" : "None", "Direction" : "I"},
			{"Name" : "globalparams_reducecommand", "Type" : "None", "Direction" : "I"},
			{"Name" : "globalparams_treedepth", "Type" : "None", "Direction" : "I"},
			{"Name" : "globalparams_runsize", "Type" : "None", "Direction" : "I"},
			{"Name" : "globalparams_vbegin", "Type" : "None", "Direction" : "I"},
			{"Name" : "globalparams_statsalreadycollected", "Type" : "None", "Direction" : "I"}],
		"SubInstanceBlock" : [
			{"SubInstance" : "grp_savekeyvalues0_1_fu_1695", "SubBlockPort" : ["dram_blk_n_AW", "dram_blk_n_W", "dram_blk_n_B"]}]},
	{"ID" : "1", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.sourcebuffer00_0_V_U", "Parent" : "0"},
	{"ID" : "2", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.sourcebuffer00_1_V_U", "Parent" : "0"},
	{"ID" : "3", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.destbuffer00_V_U", "Parent" : "0"},
	{"ID" : "4", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.capsule00_key_U", "Parent" : "0"},
	{"ID" : "5", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.capsule00_value_U", "Parent" : "0"},
	{"ID" : "6", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.BIGcapsule00_V_U", "Parent" : "0"},
	{"ID" : "7", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.sourcebuffer01_0_V_U", "Parent" : "0"},
	{"ID" : "8", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.sourcebuffer01_1_V_U", "Parent" : "0"},
	{"ID" : "9", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.destbuffer01_V_U", "Parent" : "0"},
	{"ID" : "10", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.capsule01_key_U", "Parent" : "0"},
	{"ID" : "11", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.capsule01_value_U", "Parent" : "0"},
	{"ID" : "12", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.BIGcapsule01_V_U", "Parent" : "0"},
	{"ID" : "13", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.sourcebuffer02_0_V_U", "Parent" : "0"},
	{"ID" : "14", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.sourcebuffer02_1_V_U", "Parent" : "0"},
	{"ID" : "15", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.destbuffer02_V_U", "Parent" : "0"},
	{"ID" : "16", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.capsule02_key_U", "Parent" : "0"},
	{"ID" : "17", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.capsule02_value_U", "Parent" : "0"},
	{"ID" : "18", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.BIGcapsule02_V_U", "Parent" : "0"},
	{"ID" : "19", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.sourcebuffer03_0_V_U", "Parent" : "0"},
	{"ID" : "20", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.sourcebuffer03_1_V_U", "Parent" : "0"},
	{"ID" : "21", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.destbuffer03_V_U", "Parent" : "0"},
	{"ID" : "22", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.capsule03_key_U", "Parent" : "0"},
	{"ID" : "23", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.capsule03_value_U", "Parent" : "0"},
	{"ID" : "24", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.BIGcapsule03_V_U", "Parent" : "0"},
	{"ID" : "25", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.kvdeststats_tmp_key_U", "Parent" : "0"},
	{"ID" : "26", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.kvdeststats_tmp_valu_U", "Parent" : "0"},
	{"ID" : "27", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1486", "Parent" : "0", "Child" : ["28", "29", "30", "31", "32", "33", "34", "35", "36", "37", "38", "39", "40", "41", "42", "43", "44", "45", "46", "47", "48", "49", "50", "51", "52", "53", "54", "55", "56", "57", "58", "59", "60", "61", "62", "63", "64", "65", "66", "67", "68", "69", "70", "71", "72", "73", "74", "75", "76", "77", "78", "79", "80", "81", "82", "83", "84", "85", "86", "87", "88", "89", "90", "91", "92", "93", "94", "95", "96", "97", "98", "99", "100", "101", "102", "103", "104", "105", "106", "107", "108", "109", "110", "111", "112", "113", "114", "115", "116", "117", "118", "119", "120", "121", "122", "123", "124", "125", "126", "127", "128", "129", "130", "131", "132", "133", "134", "135", "136", "137", "138", "139", "140", "141", "142", "143", "144", "145", "146", "147", "148", "149", "150", "151", "152", "153", "154", "155"],
		"CDFG" : "reducepartitions0",
		"Protocol" : "ap_ctrl_hs",
		"ControlExist" : "1", "ap_start" : "1", "ap_ready" : "1", "ap_done" : "1", "ap_continue" : "0", "ap_idle" : "1",
		"Pipeline" : "None", "UnalignedPipeline" : "0", "RewindPipeline" : "0", "ProcessNetwork" : "0",
		"II" : "0",
		"VariableLatency" : "1", "ExactLatency" : "-1", "EstimateLatencyMin" : "1", "EstimateLatencyMax" : "5157",
		"Combinational" : "0",
		"Datapath" : "0",
		"ClockEnable" : "0",
		"HasSubDataflow" : "0",
		"InDataflowNetwork" : "0",
		"HasNonBlockingOperation" : "0",
		"Port" : [
			{"Name" : "enable", "Type" : "None", "Direction" : "I"},
			{"Name" : "sourcebuffer0_V", "Type" : "Memory", "Direction" : "I"},
			{"Name" : "sourcebuffer1_V", "Type" : "Memory", "Direction" : "I"},
			{"Name" : "sourcebuffer2_V", "Type" : "Memory", "Direction" : "I"},
			{"Name" : "sourcebuffer3_V", "Type" : "Memory", "Direction" : "I"},
			{"Name" : "destbuffer0_V", "Type" : "Memory", "Direction" : "IO"},
			{"Name" : "destbuffer1_V", "Type" : "Memory", "Direction" : "IO"},
			{"Name" : "destbuffer2_V", "Type" : "Memory", "Direction" : "IO"},
			{"Name" : "destbuffer3_V", "Type" : "Memory", "Direction" : "IO"},
			{"Name" : "globalparams_vbegin", "Type" : "None", "Direction" : "I"}]},
	{"ID" : "28", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1486.destbuffer00_key_U", "Parent" : "27"},
	{"ID" : "29", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1486.destbuffer00_value_U", "Parent" : "27"},
	{"ID" : "30", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1486.destbuffer01_key_U", "Parent" : "27"},
	{"ID" : "31", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1486.destbuffer01_value_U", "Parent" : "27"},
	{"ID" : "32", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1486.destbuffer02_key_U", "Parent" : "27"},
	{"ID" : "33", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1486.destbuffer02_value_U", "Parent" : "27"},
	{"ID" : "34", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1486.destbuffer03_key_U", "Parent" : "27"},
	{"ID" : "35", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1486.destbuffer03_value_U", "Parent" : "27"},
	{"ID" : "36", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1486.destbuffer04_key_U", "Parent" : "27"},
	{"ID" : "37", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1486.destbuffer04_value_U", "Parent" : "27"},
	{"ID" : "38", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1486.destbuffer05_key_U", "Parent" : "27"},
	{"ID" : "39", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1486.destbuffer05_value_U", "Parent" : "27"},
	{"ID" : "40", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1486.destbuffer06_key_U", "Parent" : "27"},
	{"ID" : "41", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1486.destbuffer06_value_U", "Parent" : "27"},
	{"ID" : "42", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1486.destbuffer07_key_U", "Parent" : "27"},
	{"ID" : "43", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1486.destbuffer07_value_U", "Parent" : "27"},
	{"ID" : "44", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1486.destbuffer10_key_U", "Parent" : "27"},
	{"ID" : "45", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1486.destbuffer10_value_U", "Parent" : "27"},
	{"ID" : "46", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1486.destbuffer11_key_U", "Parent" : "27"},
	{"ID" : "47", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1486.destbuffer11_value_U", "Parent" : "27"},
	{"ID" : "48", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1486.destbuffer12_key_U", "Parent" : "27"},
	{"ID" : "49", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1486.destbuffer12_value_U", "Parent" : "27"},
	{"ID" : "50", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1486.destbuffer13_key_U", "Parent" : "27"},
	{"ID" : "51", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1486.destbuffer13_value_U", "Parent" : "27"},
	{"ID" : "52", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1486.destbuffer14_key_U", "Parent" : "27"},
	{"ID" : "53", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1486.destbuffer14_value_U", "Parent" : "27"},
	{"ID" : "54", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1486.destbuffer15_key_U", "Parent" : "27"},
	{"ID" : "55", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1486.destbuffer15_value_U", "Parent" : "27"},
	{"ID" : "56", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1486.destbuffer16_key_U", "Parent" : "27"},
	{"ID" : "57", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1486.destbuffer16_value_U", "Parent" : "27"},
	{"ID" : "58", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1486.destbuffer17_key_U", "Parent" : "27"},
	{"ID" : "59", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1486.destbuffer17_value_U", "Parent" : "27"},
	{"ID" : "60", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1486.destbuffer20_key_U", "Parent" : "27"},
	{"ID" : "61", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1486.destbuffer20_value_U", "Parent" : "27"},
	{"ID" : "62", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1486.destbuffer21_key_U", "Parent" : "27"},
	{"ID" : "63", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1486.destbuffer21_value_U", "Parent" : "27"},
	{"ID" : "64", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1486.destbuffer22_key_U", "Parent" : "27"},
	{"ID" : "65", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1486.destbuffer22_value_U", "Parent" : "27"},
	{"ID" : "66", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1486.destbuffer23_key_U", "Parent" : "27"},
	{"ID" : "67", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1486.destbuffer23_value_U", "Parent" : "27"},
	{"ID" : "68", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1486.destbuffer24_key_U", "Parent" : "27"},
	{"ID" : "69", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1486.destbuffer24_value_U", "Parent" : "27"},
	{"ID" : "70", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1486.destbuffer25_key_U", "Parent" : "27"},
	{"ID" : "71", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1486.destbuffer25_value_U", "Parent" : "27"},
	{"ID" : "72", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1486.destbuffer26_key_U", "Parent" : "27"},
	{"ID" : "73", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1486.destbuffer26_value_U", "Parent" : "27"},
	{"ID" : "74", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1486.destbuffer27_key_U", "Parent" : "27"},
	{"ID" : "75", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1486.destbuffer27_value_U", "Parent" : "27"},
	{"ID" : "76", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1486.destbuffer30_key_U", "Parent" : "27"},
	{"ID" : "77", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1486.destbuffer30_value_U", "Parent" : "27"},
	{"ID" : "78", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1486.destbuffer31_key_U", "Parent" : "27"},
	{"ID" : "79", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1486.destbuffer31_value_U", "Parent" : "27"},
	{"ID" : "80", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1486.destbuffer32_key_U", "Parent" : "27"},
	{"ID" : "81", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1486.destbuffer32_value_U", "Parent" : "27"},
	{"ID" : "82", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1486.destbuffer33_key_U", "Parent" : "27"},
	{"ID" : "83", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1486.destbuffer33_value_U", "Parent" : "27"},
	{"ID" : "84", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1486.destbuffer34_key_U", "Parent" : "27"},
	{"ID" : "85", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1486.destbuffer34_value_U", "Parent" : "27"},
	{"ID" : "86", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1486.destbuffer35_key_U", "Parent" : "27"},
	{"ID" : "87", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1486.destbuffer35_value_U", "Parent" : "27"},
	{"ID" : "88", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1486.destbuffer36_key_U", "Parent" : "27"},
	{"ID" : "89", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1486.destbuffer36_value_U", "Parent" : "27"},
	{"ID" : "90", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1486.destbuffer37_key_U", "Parent" : "27"},
	{"ID" : "91", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1486.destbuffer37_value_U", "Parent" : "27"},
	{"ID" : "92", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1486.topkernel_mux_83_32_1_1_U165", "Parent" : "27"},
	{"ID" : "93", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1486.topkernel_mux_83_32_1_1_U166", "Parent" : "27"},
	{"ID" : "94", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1486.topkernel_mux_83_32_1_1_U167", "Parent" : "27"},
	{"ID" : "95", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1486.topkernel_mux_83_32_1_1_U168", "Parent" : "27"},
	{"ID" : "96", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1486.topkernel_mux_83_32_1_1_U169", "Parent" : "27"},
	{"ID" : "97", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1486.topkernel_mux_83_32_1_1_U170", "Parent" : "27"},
	{"ID" : "98", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1486.topkernel_mux_83_32_1_1_U171", "Parent" : "27"},
	{"ID" : "99", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1486.topkernel_mux_83_32_1_1_U172", "Parent" : "27"},
	{"ID" : "100", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1486.topkernel_mux_83_32_1_1_U173", "Parent" : "27"},
	{"ID" : "101", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1486.topkernel_mux_83_32_1_1_U174", "Parent" : "27"},
	{"ID" : "102", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1486.topkernel_mux_83_32_1_1_U175", "Parent" : "27"},
	{"ID" : "103", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1486.topkernel_mux_83_32_1_1_U176", "Parent" : "27"},
	{"ID" : "104", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1486.topkernel_mux_83_32_1_1_U177", "Parent" : "27"},
	{"ID" : "105", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1486.topkernel_mux_83_32_1_1_U178", "Parent" : "27"},
	{"ID" : "106", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1486.topkernel_mux_83_32_1_1_U179", "Parent" : "27"},
	{"ID" : "107", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1486.topkernel_mux_83_32_1_1_U180", "Parent" : "27"},
	{"ID" : "108", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1486.topkernel_mux_83_32_1_1_U181", "Parent" : "27"},
	{"ID" : "109", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1486.topkernel_mux_83_32_1_1_U182", "Parent" : "27"},
	{"ID" : "110", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1486.topkernel_mux_83_32_1_1_U183", "Parent" : "27"},
	{"ID" : "111", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1486.topkernel_mux_83_32_1_1_U184", "Parent" : "27"},
	{"ID" : "112", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1486.topkernel_mux_83_32_1_1_U185", "Parent" : "27"},
	{"ID" : "113", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1486.topkernel_mux_83_32_1_1_U186", "Parent" : "27"},
	{"ID" : "114", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1486.topkernel_mux_83_32_1_1_U187", "Parent" : "27"},
	{"ID" : "115", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1486.topkernel_mux_83_32_1_1_U188", "Parent" : "27"},
	{"ID" : "116", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1486.topkernel_mux_83_32_1_1_U189", "Parent" : "27"},
	{"ID" : "117", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1486.topkernel_mux_83_32_1_1_U190", "Parent" : "27"},
	{"ID" : "118", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1486.topkernel_mux_83_32_1_1_U191", "Parent" : "27"},
	{"ID" : "119", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1486.topkernel_mux_83_32_1_1_U192", "Parent" : "27"},
	{"ID" : "120", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1486.topkernel_mux_83_32_1_1_U193", "Parent" : "27"},
	{"ID" : "121", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1486.topkernel_mux_83_32_1_1_U194", "Parent" : "27"},
	{"ID" : "122", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1486.topkernel_mux_83_32_1_1_U195", "Parent" : "27"},
	{"ID" : "123", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1486.topkernel_mux_83_32_1_1_U196", "Parent" : "27"},
	{"ID" : "124", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1486.topkernel_mux_83_32_1_1_U197", "Parent" : "27"},
	{"ID" : "125", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1486.topkernel_mux_83_32_1_1_U198", "Parent" : "27"},
	{"ID" : "126", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1486.topkernel_mux_83_32_1_1_U199", "Parent" : "27"},
	{"ID" : "127", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1486.topkernel_mux_83_32_1_1_U200", "Parent" : "27"},
	{"ID" : "128", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1486.topkernel_mux_83_32_1_1_U201", "Parent" : "27"},
	{"ID" : "129", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1486.topkernel_mux_83_32_1_1_U202", "Parent" : "27"},
	{"ID" : "130", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1486.topkernel_mux_83_32_1_1_U203", "Parent" : "27"},
	{"ID" : "131", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1486.topkernel_mux_83_32_1_1_U204", "Parent" : "27"},
	{"ID" : "132", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1486.topkernel_mux_83_32_1_1_U205", "Parent" : "27"},
	{"ID" : "133", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1486.topkernel_mux_83_32_1_1_U206", "Parent" : "27"},
	{"ID" : "134", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1486.topkernel_mux_83_32_1_1_U207", "Parent" : "27"},
	{"ID" : "135", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1486.topkernel_mux_83_32_1_1_U208", "Parent" : "27"},
	{"ID" : "136", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1486.topkernel_mux_83_32_1_1_U209", "Parent" : "27"},
	{"ID" : "137", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1486.topkernel_mux_83_32_1_1_U210", "Parent" : "27"},
	{"ID" : "138", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1486.topkernel_mux_83_32_1_1_U211", "Parent" : "27"},
	{"ID" : "139", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1486.topkernel_mux_83_32_1_1_U212", "Parent" : "27"},
	{"ID" : "140", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1486.topkernel_mux_83_32_1_1_U213", "Parent" : "27"},
	{"ID" : "141", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1486.topkernel_mux_83_32_1_1_U214", "Parent" : "27"},
	{"ID" : "142", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1486.topkernel_mux_83_32_1_1_U215", "Parent" : "27"},
	{"ID" : "143", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1486.topkernel_mux_83_32_1_1_U216", "Parent" : "27"},
	{"ID" : "144", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1486.topkernel_mux_83_32_1_1_U217", "Parent" : "27"},
	{"ID" : "145", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1486.topkernel_mux_83_32_1_1_U218", "Parent" : "27"},
	{"ID" : "146", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1486.topkernel_mux_83_32_1_1_U219", "Parent" : "27"},
	{"ID" : "147", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1486.topkernel_mux_83_32_1_1_U220", "Parent" : "27"},
	{"ID" : "148", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1486.topkernel_mux_83_32_1_1_U221", "Parent" : "27"},
	{"ID" : "149", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1486.topkernel_mux_83_32_1_1_U222", "Parent" : "27"},
	{"ID" : "150", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1486.topkernel_mux_83_32_1_1_U223", "Parent" : "27"},
	{"ID" : "151", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1486.topkernel_mux_83_32_1_1_U224", "Parent" : "27"},
	{"ID" : "152", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1486.topkernel_mux_83_32_1_1_U225", "Parent" : "27"},
	{"ID" : "153", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1486.topkernel_mux_83_32_1_1_U226", "Parent" : "27"},
	{"ID" : "154", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1486.topkernel_mux_83_32_1_1_U227", "Parent" : "27"},
	{"ID" : "155", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1486.topkernel_mux_83_32_1_1_U228", "Parent" : "27"},
	{"ID" : "156", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.grp_partitionkeyvalues00_fu_1501", "Parent" : "0", "Child" : ["157", "158", "159", "160", "161", "162", "163", "164", "165", "166", "167", "168"],
		"CDFG" : "partitionkeyvalues00",
		"Protocol" : "ap_ctrl_hs",
		"ControlExist" : "1", "ap_start" : "1", "ap_ready" : "1", "ap_done" : "1", "ap_continue" : "0", "ap_idle" : "1",
		"Pipeline" : "None", "UnalignedPipeline" : "0", "RewindPipeline" : "0", "ProcessNetwork" : "0",
		"II" : "0",
		"VariableLatency" : "1", "ExactLatency" : "-1", "EstimateLatencyMin" : "1", "EstimateLatencyMax" : "4228",
		"Combinational" : "0",
		"Datapath" : "0",
		"ClockEnable" : "0",
		"HasSubDataflow" : "0",
		"InDataflowNetwork" : "0",
		"HasNonBlockingOperation" : "0",
		"Port" : [
			{"Name" : "enable", "Type" : "None", "Direction" : "I"},
			{"Name" : "sourcebuffer0_V", "Type" : "Memory", "Direction" : "I"},
			{"Name" : "sourcebuffer1_V", "Type" : "Memory", "Direction" : "I"},
			{"Name" : "sourcebuffer2_V", "Type" : "Memory", "Direction" : "I"},
			{"Name" : "sourcebuffer3_V", "Type" : "Memory", "Direction" : "I"},
			{"Name" : "destbuffer0_V", "Type" : "Memory", "Direction" : "O"},
			{"Name" : "destbuffer1_V", "Type" : "Memory", "Direction" : "O"},
			{"Name" : "destbuffer2_V", "Type" : "Memory", "Direction" : "O"},
			{"Name" : "destbuffer3_V", "Type" : "Memory", "Direction" : "O"},
			{"Name" : "capsule0_key", "Type" : "Memory", "Direction" : "I"},
			{"Name" : "capsule0_value", "Type" : "Memory", "Direction" : "IO"},
			{"Name" : "capsule1_key", "Type" : "Memory", "Direction" : "I"},
			{"Name" : "capsule1_value", "Type" : "Memory", "Direction" : "IO"},
			{"Name" : "capsule2_key", "Type" : "Memory", "Direction" : "I"},
			{"Name" : "capsule2_value", "Type" : "Memory", "Direction" : "IO"},
			{"Name" : "capsule3_key", "Type" : "Memory", "Direction" : "I"},
			{"Name" : "capsule3_value", "Type" : "Memory", "Direction" : "IO"},
			{"Name" : "travstate_i_kvs", "Type" : "None", "Direction" : "I"},
			{"Name" : "travstate_end_kvs", "Type" : "None", "Direction" : "I"},
			{"Name" : "llopparams_currentLOP", "Type" : "None", "Direction" : "I"},
			{"Name" : "llopparams_upperlimit", "Type" : "None", "Direction" : "I"}]},
	{"ID" : "157", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_partitionkeyvalues00_fu_1501.topkernel_mux_325_32_1_1_U85", "Parent" : "156"},
	{"ID" : "158", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_partitionkeyvalues00_fu_1501.topkernel_mux_325_32_1_1_U86", "Parent" : "156"},
	{"ID" : "159", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_partitionkeyvalues00_fu_1501.topkernel_mux_325_32_1_1_U87", "Parent" : "156"},
	{"ID" : "160", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_partitionkeyvalues00_fu_1501.topkernel_mux_325_32_1_1_U88", "Parent" : "156"},
	{"ID" : "161", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_partitionkeyvalues00_fu_1501.topkernel_mux_325_32_1_1_U89", "Parent" : "156"},
	{"ID" : "162", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_partitionkeyvalues00_fu_1501.topkernel_mux_325_32_1_1_U90", "Parent" : "156"},
	{"ID" : "163", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_partitionkeyvalues00_fu_1501.topkernel_mux_325_32_1_1_U91", "Parent" : "156"},
	{"ID" : "164", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_partitionkeyvalues00_fu_1501.topkernel_mux_325_32_1_1_U92", "Parent" : "156"},
	{"ID" : "165", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_partitionkeyvalues00_fu_1501.topkernel_mux_325_32_1_1_U93", "Parent" : "156"},
	{"ID" : "166", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_partitionkeyvalues00_fu_1501.topkernel_mux_325_32_1_1_U94", "Parent" : "156"},
	{"ID" : "167", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_partitionkeyvalues00_fu_1501.topkernel_mux_325_32_1_1_U95", "Parent" : "156"},
	{"ID" : "168", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_partitionkeyvalues00_fu_1501.topkernel_mux_325_32_1_1_U96", "Parent" : "156"},
	{"ID" : "169", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.grp_savepartitions0_fu_1529", "Parent" : "0", "Child" : ["170", "171", "172", "173", "174", "175", "176", "177"],
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
	{"ID" : "170", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_savepartitions0_fu_1529.topkernel_urem_12ns_8ns_12_16_1_U119", "Parent" : "169"},
	{"ID" : "171", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_savepartitions0_fu_1529.topkernel_urem_12ns_8ns_12_16_1_U120", "Parent" : "169"},
	{"ID" : "172", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_savepartitions0_fu_1529.topkernel_urem_12ns_8ns_12_16_1_U121", "Parent" : "169"},
	{"ID" : "173", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_savepartitions0_fu_1529.topkernel_urem_12ns_8ns_12_16_1_U122", "Parent" : "169"},
	{"ID" : "174", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_savepartitions0_fu_1529.topkernel_urem_12ns_8ns_12_16_1_U123", "Parent" : "169"},
	{"ID" : "175", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_savepartitions0_fu_1529.topkernel_urem_12ns_8ns_12_16_1_U124", "Parent" : "169"},
	{"ID" : "176", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_savepartitions0_fu_1529.topkernel_urem_12ns_8ns_12_16_1_U125", "Parent" : "169"},
	{"ID" : "177", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_savepartitions0_fu_1529.topkernel_urem_12ns_8ns_12_16_1_U126", "Parent" : "169"},
	{"ID" : "178", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.grp_collectstats00_fu_1551", "Parent" : "0", "Child" : ["179", "180", "181", "182", "183", "184", "185", "186", "187", "188", "189", "190", "191", "192", "193", "194", "195", "196", "197", "198", "199", "200", "201", "202", "203", "204", "205", "206", "207", "208", "209", "210", "211", "212", "213", "214", "215", "216", "217", "218", "219", "220", "221", "222", "223", "224", "225", "226"],
		"CDFG" : "collectstats00",
		"Protocol" : "ap_ctrl_hs",
		"ControlExist" : "1", "ap_start" : "1", "ap_ready" : "1", "ap_done" : "1", "ap_continue" : "0", "ap_idle" : "1",
		"Pipeline" : "None", "UnalignedPipeline" : "0", "RewindPipeline" : "0", "ProcessNetwork" : "0",
		"II" : "0",
		"VariableLatency" : "1", "ExactLatency" : "-1", "EstimateLatencyMin" : "1", "EstimateLatencyMax" : "1126",
		"Combinational" : "0",
		"Datapath" : "0",
		"ClockEnable" : "0",
		"HasSubDataflow" : "0",
		"InDataflowNetwork" : "0",
		"HasNonBlockingOperation" : "0",
		"Port" : [
			{"Name" : "enable", "Type" : "None", "Direction" : "I"},
			{"Name" : "sourcebuffer0_V", "Type" : "Memory", "Direction" : "I"},
			{"Name" : "sourcebuffer1_V", "Type" : "Memory", "Direction" : "I"},
			{"Name" : "sourcebuffer2_V", "Type" : "Memory", "Direction" : "I"},
			{"Name" : "sourcebuffer3_V", "Type" : "Memory", "Direction" : "I"},
			{"Name" : "capsule0_key", "Type" : "Memory", "Direction" : "O"},
			{"Name" : "capsule0_value", "Type" : "Memory", "Direction" : "IO"},
			{"Name" : "capsule1_key", "Type" : "Memory", "Direction" : "O"},
			{"Name" : "capsule1_value", "Type" : "Memory", "Direction" : "IO"},
			{"Name" : "capsule2_key", "Type" : "Memory", "Direction" : "O"},
			{"Name" : "capsule2_value", "Type" : "Memory", "Direction" : "IO"},
			{"Name" : "capsule3_key", "Type" : "Memory", "Direction" : "O"},
			{"Name" : "capsule3_value", "Type" : "Memory", "Direction" : "IO"},
			{"Name" : "llopparams_currentLOP", "Type" : "None", "Direction" : "I"},
			{"Name" : "llopparams_upperlimit", "Type" : "None", "Direction" : "I"},
			{"Name" : "travstate_i_kvs", "Type" : "None", "Direction" : "I"},
			{"Name" : "travstate_end_kvs", "Type" : "None", "Direction" : "I"}]},
	{"ID" : "179", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_collectstats00_fu_1551.sizes00_U", "Parent" : "178"},
	{"ID" : "180", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_collectstats00_fu_1551.sizes01_U", "Parent" : "178"},
	{"ID" : "181", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_collectstats00_fu_1551.sizes02_U", "Parent" : "178"},
	{"ID" : "182", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_collectstats00_fu_1551.sizes03_U", "Parent" : "178"},
	{"ID" : "183", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_collectstats00_fu_1551.sizes04_U", "Parent" : "178"},
	{"ID" : "184", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_collectstats00_fu_1551.sizes05_U", "Parent" : "178"},
	{"ID" : "185", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_collectstats00_fu_1551.sizes06_U", "Parent" : "178"},
	{"ID" : "186", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_collectstats00_fu_1551.sizes07_U", "Parent" : "178"},
	{"ID" : "187", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_collectstats00_fu_1551.sizes10_U", "Parent" : "178"},
	{"ID" : "188", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_collectstats00_fu_1551.sizes11_U", "Parent" : "178"},
	{"ID" : "189", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_collectstats00_fu_1551.sizes12_U", "Parent" : "178"},
	{"ID" : "190", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_collectstats00_fu_1551.sizes13_U", "Parent" : "178"},
	{"ID" : "191", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_collectstats00_fu_1551.sizes14_U", "Parent" : "178"},
	{"ID" : "192", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_collectstats00_fu_1551.sizes15_U", "Parent" : "178"},
	{"ID" : "193", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_collectstats00_fu_1551.sizes16_U", "Parent" : "178"},
	{"ID" : "194", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_collectstats00_fu_1551.sizes17_U", "Parent" : "178"},
	{"ID" : "195", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_collectstats00_fu_1551.sizes20_U", "Parent" : "178"},
	{"ID" : "196", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_collectstats00_fu_1551.sizes21_U", "Parent" : "178"},
	{"ID" : "197", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_collectstats00_fu_1551.sizes22_U", "Parent" : "178"},
	{"ID" : "198", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_collectstats00_fu_1551.sizes23_U", "Parent" : "178"},
	{"ID" : "199", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_collectstats00_fu_1551.sizes24_U", "Parent" : "178"},
	{"ID" : "200", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_collectstats00_fu_1551.sizes25_U", "Parent" : "178"},
	{"ID" : "201", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_collectstats00_fu_1551.sizes26_U", "Parent" : "178"},
	{"ID" : "202", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_collectstats00_fu_1551.sizes27_U", "Parent" : "178"},
	{"ID" : "203", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_collectstats00_fu_1551.sizes30_U", "Parent" : "178"},
	{"ID" : "204", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_collectstats00_fu_1551.sizes31_U", "Parent" : "178"},
	{"ID" : "205", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_collectstats00_fu_1551.sizes32_U", "Parent" : "178"},
	{"ID" : "206", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_collectstats00_fu_1551.sizes33_U", "Parent" : "178"},
	{"ID" : "207", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_collectstats00_fu_1551.sizes34_U", "Parent" : "178"},
	{"ID" : "208", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_collectstats00_fu_1551.sizes35_U", "Parent" : "178"},
	{"ID" : "209", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_collectstats00_fu_1551.sizes36_U", "Parent" : "178"},
	{"ID" : "210", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_collectstats00_fu_1551.sizes37_U", "Parent" : "178"},
	{"ID" : "211", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_collectstats00_fu_1551.grp_getpartition_fu_1471", "Parent" : "178",
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
	{"ID" : "212", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_collectstats00_fu_1551.grp_getpartition_fu_1478", "Parent" : "178",
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
	{"ID" : "213", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_collectstats00_fu_1551.grp_getpartition_fu_1485", "Parent" : "178",
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
	{"ID" : "214", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_collectstats00_fu_1551.grp_getpartition_fu_1492", "Parent" : "178",
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
	{"ID" : "215", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_collectstats00_fu_1551.grp_getpartition_fu_1499", "Parent" : "178",
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
	{"ID" : "216", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_collectstats00_fu_1551.grp_getpartition_fu_1506", "Parent" : "178",
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
	{"ID" : "217", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_collectstats00_fu_1551.grp_getpartition_fu_1513", "Parent" : "178",
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
	{"ID" : "218", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_collectstats00_fu_1551.grp_getpartition_fu_1520", "Parent" : "178",
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
	{"ID" : "219", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_collectstats00_fu_1551.grp_getpartition_fu_1527", "Parent" : "178",
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
	{"ID" : "220", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_collectstats00_fu_1551.grp_getpartition_fu_1534", "Parent" : "178",
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
	{"ID" : "221", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_collectstats00_fu_1551.grp_getpartition_fu_1541", "Parent" : "178",
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
	{"ID" : "222", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_collectstats00_fu_1551.grp_getpartition_fu_1548", "Parent" : "178",
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
	{"ID" : "223", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_collectstats00_fu_1551.grp_getpartition_fu_1555", "Parent" : "178",
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
	{"ID" : "224", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_collectstats00_fu_1551.grp_getpartition_fu_1562", "Parent" : "178",
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
	{"ID" : "225", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_collectstats00_fu_1551.grp_getpartition_fu_1569", "Parent" : "178",
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
	{"ID" : "226", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_collectstats00_fu_1551.grp_getpartition_fu_1576", "Parent" : "178",
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
	{"ID" : "227", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.grp_readcapsules0_fu_1575", "Parent" : "0", "Child" : ["228"],
		"CDFG" : "readcapsules0",
		"Protocol" : "ap_ctrl_hs",
		"ControlExist" : "1", "ap_start" : "1", "ap_ready" : "1", "ap_done" : "1", "ap_continue" : "0", "ap_idle" : "1",
		"Pipeline" : "None", "UnalignedPipeline" : "0", "RewindPipeline" : "0", "ProcessNetwork" : "0",
		"II" : "0",
		"VariableLatency" : "1", "ExactLatency" : "-1", "EstimateLatencyMin" : "35", "EstimateLatencyMax" : "2129",
		"Combinational" : "0",
		"Datapath" : "0",
		"ClockEnable" : "0",
		"HasSubDataflow" : "0",
		"InDataflowNetwork" : "0",
		"HasNonBlockingOperation" : "0",
		"WaitState" : [
			{"State" : "ap_ST_fsm_state2", "FSM" : "ap_CS_fsm", "SubInstance" : "grp_readkeyvalues0_fu_418"},
			{"State" : "ap_ST_fsm_state4", "FSM" : "ap_CS_fsm", "SubInstance" : "grp_readkeyvalues0_fu_418"},
			{"State" : "ap_ST_fsm_state6", "FSM" : "ap_CS_fsm", "SubInstance" : "grp_readkeyvalues0_fu_418"},
			{"State" : "ap_ST_fsm_state8", "FSM" : "ap_CS_fsm", "SubInstance" : "grp_readkeyvalues0_fu_418"}],
		"Port" : [
			{"Name" : "kvdram_V", "Type" : "MAXI", "Direction" : "I",
				"SubConnect" : [
					{"ID" : "228", "SubInstance" : "grp_readkeyvalues0_fu_418", "Port" : "kvdram_V"}]},
			{"Name" : "kvdram_V_offset", "Type" : "None", "Direction" : "I"},
			{"Name" : "BIGcapsule0_V", "Type" : "Memory", "Direction" : "IO",
				"SubConnect" : [
					{"ID" : "228", "SubInstance" : "grp_readkeyvalues0_fu_418", "Port" : "buffer_V"}]},
			{"Name" : "BIGcapsule1_V", "Type" : "Memory", "Direction" : "IO",
				"SubConnect" : [
					{"ID" : "228", "SubInstance" : "grp_readkeyvalues0_fu_418", "Port" : "buffer_V"}]},
			{"Name" : "BIGcapsule2_V", "Type" : "Memory", "Direction" : "IO",
				"SubConnect" : [
					{"ID" : "228", "SubInstance" : "grp_readkeyvalues0_fu_418", "Port" : "buffer_V"}]},
			{"Name" : "BIGcapsule3_V", "Type" : "Memory", "Direction" : "IO",
				"SubConnect" : [
					{"ID" : "228", "SubInstance" : "grp_readkeyvalues0_fu_418", "Port" : "buffer_V"}]},
			{"Name" : "capsule0_key", "Type" : "Memory", "Direction" : "O"},
			{"Name" : "capsule0_value", "Type" : "Memory", "Direction" : "O"},
			{"Name" : "capsule1_key", "Type" : "Memory", "Direction" : "O"},
			{"Name" : "capsule1_value", "Type" : "Memory", "Direction" : "O"},
			{"Name" : "capsule2_key", "Type" : "Memory", "Direction" : "O"},
			{"Name" : "capsule2_value", "Type" : "Memory", "Direction" : "O"},
			{"Name" : "capsule3_key", "Type" : "Memory", "Direction" : "O"},
			{"Name" : "capsule3_value", "Type" : "Memory", "Direction" : "O"},
			{"Name" : "offset", "Type" : "None", "Direction" : "I"},
			{"Name" : "capsulemetadata_globaloffset", "Type" : "None", "Direction" : "I"},
			{"Name" : "capsulemetadata_localoffset", "Type" : "None", "Direction" : "I"}]},
	{"ID" : "228", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_readcapsules0_fu_1575.grp_readkeyvalues0_fu_418", "Parent" : "227",
		"CDFG" : "readkeyvalues0",
		"Protocol" : "ap_ctrl_hs",
		"ControlExist" : "1", "ap_start" : "1", "ap_ready" : "1", "ap_done" : "1", "ap_continue" : "0", "ap_idle" : "1",
		"Pipeline" : "None", "UnalignedPipeline" : "0", "RewindPipeline" : "0", "ProcessNetwork" : "0",
		"II" : "0",
		"VariableLatency" : "1", "ExactLatency" : "-1", "EstimateLatencyMin" : "522", "EstimateLatencyMax" : "522",
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
			{"Name" : "offset_kvs", "Type" : "None", "Direction" : "I"}]},
	{"ID" : "229", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.grp_savecapsules0_fu_1599", "Parent" : "0", "Child" : ["230"],
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
					{"ID" : "230", "SubInstance" : "grp_savekeyvalues0_fu_887", "Port" : "kvdram_V"}]},
			{"Name" : "kvdram_V_offset", "Type" : "None", "Direction" : "I"},
			{"Name" : "BIGcapsule0_V", "Type" : "Memory", "Direction" : "IO",
				"SubConnect" : [
					{"ID" : "230", "SubInstance" : "grp_savekeyvalues0_fu_887", "Port" : "buffer_V"}]},
			{"Name" : "BIGcapsule1_V", "Type" : "Memory", "Direction" : "IO",
				"SubConnect" : [
					{"ID" : "230", "SubInstance" : "grp_savekeyvalues0_fu_887", "Port" : "buffer_V"}]},
			{"Name" : "BIGcapsule2_V", "Type" : "Memory", "Direction" : "IO",
				"SubConnect" : [
					{"ID" : "230", "SubInstance" : "grp_savekeyvalues0_fu_887", "Port" : "buffer_V"}]},
			{"Name" : "BIGcapsule3_V", "Type" : "Memory", "Direction" : "IO",
				"SubConnect" : [
					{"ID" : "230", "SubInstance" : "grp_savekeyvalues0_fu_887", "Port" : "buffer_V"}]},
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
	{"ID" : "230", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_savecapsules0_fu_1599.grp_savekeyvalues0_fu_887", "Parent" : "229",
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
			{"Name" : "offset_kvs", "Type" : "None", "Direction" : "I"}]},
	{"ID" : "231", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.grp_readkeyvalues0_2_fu_1628", "Parent" : "0", "Child" : ["232"],
		"CDFG" : "readkeyvalues0_2",
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
			{"State" : "ap_ST_fsm_state3", "FSM" : "ap_CS_fsm", "SubInstance" : "grp_readkeyvalues0_158_fu_136"},
			{"State" : "ap_ST_fsm_state5", "FSM" : "ap_CS_fsm", "SubInstance" : "grp_readkeyvalues0_158_fu_136"},
			{"State" : "ap_ST_fsm_state7", "FSM" : "ap_CS_fsm", "SubInstance" : "grp_readkeyvalues0_158_fu_136"},
			{"State" : "ap_ST_fsm_state9", "FSM" : "ap_CS_fsm", "SubInstance" : "grp_readkeyvalues0_158_fu_136"}],
		"Port" : [
			{"Name" : "kvdram_V", "Type" : "MAXI", "Direction" : "I",
				"SubConnect" : [
					{"ID" : "232", "SubInstance" : "grp_readkeyvalues0_158_fu_136", "Port" : "kvdram_V"}]},
			{"Name" : "kvdram_V_offset", "Type" : "None", "Direction" : "I"},
			{"Name" : "buffer0_V", "Type" : "Memory", "Direction" : "O",
				"SubConnect" : [
					{"ID" : "232", "SubInstance" : "grp_readkeyvalues0_158_fu_136", "Port" : "buffer_V"}]},
			{"Name" : "buffer1_V", "Type" : "Memory", "Direction" : "O",
				"SubConnect" : [
					{"ID" : "232", "SubInstance" : "grp_readkeyvalues0_158_fu_136", "Port" : "buffer_V"}]},
			{"Name" : "buffer2_V", "Type" : "Memory", "Direction" : "O",
				"SubConnect" : [
					{"ID" : "232", "SubInstance" : "grp_readkeyvalues0_158_fu_136", "Port" : "buffer_V"}]},
			{"Name" : "buffer3_V", "Type" : "Memory", "Direction" : "O",
				"SubConnect" : [
					{"ID" : "232", "SubInstance" : "grp_readkeyvalues0_158_fu_136", "Port" : "buffer_V"}]},
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
	{"ID" : "232", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_readkeyvalues0_2_fu_1628.grp_readkeyvalues0_158_fu_136", "Parent" : "231",
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
			{"Name" : "size_kvs", "Type" : "None", "Direction" : "I"}]},
	{"ID" : "233", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.grp_readkeyvalues0_1_fu_1650", "Parent" : "0", "Child" : ["234"],
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
					{"ID" : "234", "SubInstance" : "grp_readkeyvalues0_158_fu_94", "Port" : "kvdram_V"}]},
			{"Name" : "kvdram_V_offset", "Type" : "None", "Direction" : "I"},
			{"Name" : "buffer0_V", "Type" : "Memory", "Direction" : "O",
				"SubConnect" : [
					{"ID" : "234", "SubInstance" : "grp_readkeyvalues0_158_fu_94", "Port" : "buffer_V"}]},
			{"Name" : "buffer1_V", "Type" : "Memory", "Direction" : "O",
				"SubConnect" : [
					{"ID" : "234", "SubInstance" : "grp_readkeyvalues0_158_fu_94", "Port" : "buffer_V"}]},
			{"Name" : "buffer2_V", "Type" : "Memory", "Direction" : "O",
				"SubConnect" : [
					{"ID" : "234", "SubInstance" : "grp_readkeyvalues0_158_fu_94", "Port" : "buffer_V"}]},
			{"Name" : "buffer3_V", "Type" : "Memory", "Direction" : "O",
				"SubConnect" : [
					{"ID" : "234", "SubInstance" : "grp_readkeyvalues0_158_fu_94", "Port" : "buffer_V"}]},
			{"Name" : "baseaddress", "Type" : "None", "Direction" : "I"},
			{"Name" : "offset_kvs", "Type" : "None", "Direction" : "I"},
			{"Name" : "travstate_i_kvs", "Type" : "None", "Direction" : "I"},
			{"Name" : "travstate_end_kvs", "Type" : "None", "Direction" : "I"}]},
	{"ID" : "234", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_readkeyvalues0_1_fu_1650.grp_readkeyvalues0_158_fu_94", "Parent" : "233",
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
			{"Name" : "size_kvs", "Type" : "None", "Direction" : "I"}]},
	{"ID" : "235", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.grp_savekeyvalues0_153_fu_1667", "Parent" : "0",
		"CDFG" : "savekeyvalues0_153",
		"Protocol" : "ap_ctrl_hs",
		"ControlExist" : "1", "ap_start" : "1", "ap_ready" : "1", "ap_done" : "1", "ap_continue" : "0", "ap_idle" : "1",
		"Pipeline" : "None", "UnalignedPipeline" : "0", "RewindPipeline" : "0", "ProcessNetwork" : "0",
		"II" : "0",
		"VariableLatency" : "1", "ExactLatency" : "-1", "EstimateLatencyMin" : "264", "EstimateLatencyMax" : "264",
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
			{"Name" : "offset_kvs", "Type" : "None", "Direction" : "I"}]},
	{"ID" : "236", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.grp_readkeyvalues0_157_fu_1676", "Parent" : "0",
		"CDFG" : "readkeyvalues0_157",
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
			{"Name" : "buffer_V", "Type" : "Memory", "Direction" : "O"},
			{"Name" : "offset_kvs", "Type" : "None", "Direction" : "I"}]},
	{"ID" : "237", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.grp_savekeyvalues0_2_fu_1685", "Parent" : "0",
		"CDFG" : "savekeyvalues0_2",
		"Protocol" : "ap_ctrl_hs",
		"ControlExist" : "1", "ap_start" : "1", "ap_ready" : "1", "ap_done" : "1", "ap_continue" : "0", "ap_idle" : "1",
		"Pipeline" : "None", "UnalignedPipeline" : "0", "RewindPipeline" : "0", "ProcessNetwork" : "0",
		"II" : "0",
		"VariableLatency" : "1", "ExactLatency" : "-1", "EstimateLatencyMin" : "40", "EstimateLatencyMax" : "40",
		"Combinational" : "0",
		"Datapath" : "0",
		"ClockEnable" : "0",
		"HasSubDataflow" : "0",
		"InDataflowNetwork" : "0",
		"HasNonBlockingOperation" : "0",
		"Port" : [
			{"Name" : "dram", "Type" : "MAXI", "Direction" : "O",
				"BlockSignal" : [
					{"Name" : "dram_blk_n_AW", "Type" : "RtlSignal"},
					{"Name" : "dram_blk_n_W", "Type" : "RtlSignal"},
					{"Name" : "dram_blk_n_B", "Type" : "RtlSignal"}]},
			{"Name" : "dram_offset", "Type" : "None", "Direction" : "I"},
			{"Name" : "buffer_key", "Type" : "Memory", "Direction" : "I"},
			{"Name" : "buffer_value", "Type" : "Memory", "Direction" : "I"},
			{"Name" : "offset_kvs", "Type" : "None", "Direction" : "I"}]},
	{"ID" : "238", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.grp_savekeyvalues0_1_fu_1695", "Parent" : "0",
		"CDFG" : "savekeyvalues0_1",
		"Protocol" : "ap_ctrl_hs",
		"ControlExist" : "1", "ap_start" : "1", "ap_ready" : "1", "ap_done" : "1", "ap_continue" : "0", "ap_idle" : "1",
		"Pipeline" : "Aligned", "UnalignedPipeline" : "0", "RewindPipeline" : "0", "ProcessNetwork" : "0",
		"II" : "1",
		"VariableLatency" : "0", "ExactLatency" : "7", "EstimateLatencyMin" : "7", "EstimateLatencyMax" : "7",
		"Combinational" : "0",
		"Datapath" : "0",
		"ClockEnable" : "0",
		"HasSubDataflow" : "0",
		"InDataflowNetwork" : "0",
		"HasNonBlockingOperation" : "0",
		"Port" : [
			{"Name" : "dram", "Type" : "MAXI", "Direction" : "O",
				"BlockSignal" : [
					{"Name" : "dram_blk_n_AW", "Type" : "RtlPort"},
					{"Name" : "dram_blk_n_W", "Type" : "RtlPort"},
					{"Name" : "dram_blk_n_B", "Type" : "RtlPort"}]},
			{"Name" : "dram_offset", "Type" : "None", "Direction" : "I"},
			{"Name" : "buffer_0_key_read", "Type" : "None", "Direction" : "I"},
			{"Name" : "buffer_0_value_read", "Type" : "None", "Direction" : "I"},
			{"Name" : "offset_kvs", "Type" : "None", "Direction" : "I"}]},
	{"ID" : "239", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.topkernel_mux_42_32_1_1_U255", "Parent" : "0"}]}


set ArgLastReadFirstWriteLatency {
	partitionandreduce0 {
		kvsourcedram_V {Type IO LastRead 73 FirstWrite -1}
		kvsourcedram_V_offset {Type I LastRead 0 FirstWrite -1}
		kvdestdram_V {Type IO LastRead 9 FirstWrite -1}
		kvdestdram_V_offset {Type I LastRead 0 FirstWrite -1}
		kvstats {Type IO LastRead 44 FirstWrite 2}
		kvstats_offset {Type I LastRead 0 FirstWrite -1}
		globalparams_runkernelcommand {Type I LastRead 0 FirstWrite -1}
		globalparams_partitioncommand {Type I LastRead 0 FirstWrite -1}
		globalparams_reducecommand {Type I LastRead 0 FirstWrite -1}
		globalparams_treedepth {Type I LastRead 0 FirstWrite -1}
		globalparams_runsize {Type I LastRead 0 FirstWrite -1}
		globalparams_vbegin {Type I LastRead 0 FirstWrite -1}
		globalparams_statsalreadycollected {Type I LastRead 0 FirstWrite -1}}
	reducepartitions0 {
		enable {Type I LastRead 0 FirstWrite -1}
		sourcebuffer0_V {Type I LastRead 4 FirstWrite -1}
		sourcebuffer1_V {Type I LastRead 4 FirstWrite -1}
		sourcebuffer2_V {Type I LastRead 4 FirstWrite -1}
		sourcebuffer3_V {Type I LastRead 4 FirstWrite -1}
		destbuffer0_V {Type IO LastRead 1 FirstWrite 5}
		destbuffer1_V {Type IO LastRead 1 FirstWrite 5}
		destbuffer2_V {Type IO LastRead 1 FirstWrite 5}
		destbuffer3_V {Type IO LastRead 1 FirstWrite 5}
		globalparams_vbegin {Type I LastRead 0 FirstWrite -1}}
	partitionkeyvalues00 {
		enable {Type I LastRead 0 FirstWrite -1}
		sourcebuffer0_V {Type I LastRead 2 FirstWrite -1}
		sourcebuffer1_V {Type I LastRead 2 FirstWrite -1}
		sourcebuffer2_V {Type I LastRead 2 FirstWrite -1}
		sourcebuffer3_V {Type I LastRead 2 FirstWrite -1}
		destbuffer0_V {Type O LastRead -1 FirstWrite 3}
		destbuffer1_V {Type O LastRead -1 FirstWrite 3}
		destbuffer2_V {Type O LastRead -1 FirstWrite 3}
		destbuffer3_V {Type O LastRead -1 FirstWrite 3}
		capsule0_key {Type I LastRead 1 FirstWrite -1}
		capsule0_value {Type IO LastRead 1 FirstWrite 4}
		capsule1_key {Type I LastRead 1 FirstWrite -1}
		capsule1_value {Type IO LastRead 1 FirstWrite 4}
		capsule2_key {Type I LastRead 1 FirstWrite -1}
		capsule2_value {Type IO LastRead 1 FirstWrite 4}
		capsule3_key {Type I LastRead 1 FirstWrite -1}
		capsule3_value {Type IO LastRead 1 FirstWrite 4}
		travstate_i_kvs {Type I LastRead 0 FirstWrite -1}
		travstate_end_kvs {Type I LastRead 0 FirstWrite -1}
		llopparams_currentLOP {Type I LastRead 0 FirstWrite -1}
		llopparams_upperlimit {Type I LastRead 0 FirstWrite -1}}
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
		kvdrambaseaddress {Type I LastRead 0 FirstWrite -1}}
	collectstats00 {
		enable {Type I LastRead 0 FirstWrite -1}
		sourcebuffer0_V {Type I LastRead 2 FirstWrite -1}
		sourcebuffer1_V {Type I LastRead 2 FirstWrite -1}
		sourcebuffer2_V {Type I LastRead 2 FirstWrite -1}
		sourcebuffer3_V {Type I LastRead 2 FirstWrite -1}
		capsule0_key {Type O LastRead -1 FirstWrite 4}
		capsule0_value {Type IO LastRead 4 FirstWrite 5}
		capsule1_key {Type O LastRead -1 FirstWrite 4}
		capsule1_value {Type IO LastRead 4 FirstWrite 5}
		capsule2_key {Type O LastRead -1 FirstWrite 4}
		capsule2_value {Type IO LastRead 4 FirstWrite 5}
		capsule3_key {Type O LastRead -1 FirstWrite 4}
		capsule3_value {Type IO LastRead 4 FirstWrite 5}
		llopparams_currentLOP {Type I LastRead 0 FirstWrite -1}
		llopparams_upperlimit {Type I LastRead 0 FirstWrite -1}
		travstate_i_kvs {Type I LastRead 0 FirstWrite -1}
		travstate_end_kvs {Type I LastRead 0 FirstWrite -1}}
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
		upperlimit {Type I LastRead 0 FirstWrite -1}}
	readcapsules0 {
		kvdram_V {Type I LastRead 9 FirstWrite -1}
		kvdram_V_offset {Type I LastRead 0 FirstWrite -1}
		BIGcapsule0_V {Type IO LastRead 8 FirstWrite 10}
		BIGcapsule1_V {Type IO LastRead 8 FirstWrite 10}
		BIGcapsule2_V {Type IO LastRead 8 FirstWrite 10}
		BIGcapsule3_V {Type IO LastRead 8 FirstWrite 10}
		capsule0_key {Type O LastRead -1 FirstWrite 9}
		capsule0_value {Type O LastRead -1 FirstWrite 9}
		capsule1_key {Type O LastRead -1 FirstWrite 9}
		capsule1_value {Type O LastRead -1 FirstWrite 9}
		capsule2_key {Type O LastRead -1 FirstWrite 9}
		capsule2_value {Type O LastRead -1 FirstWrite 9}
		capsule3_key {Type O LastRead -1 FirstWrite 9}
		capsule3_value {Type O LastRead -1 FirstWrite 9}
		offset {Type I LastRead 0 FirstWrite -1}
		capsulemetadata_globaloffset {Type I LastRead 0 FirstWrite -1}
		capsulemetadata_localoffset {Type I LastRead 0 FirstWrite -1}}
	readkeyvalues0 {
		kvdram_V {Type I LastRead 9 FirstWrite -1}
		kvdram_V_offset {Type I LastRead 0 FirstWrite -1}
		buffer_V {Type O LastRead -1 FirstWrite 10}
		offset_kvs {Type I LastRead 0 FirstWrite -1}}
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
		offset_kvs {Type I LastRead 0 FirstWrite -1}}
	readkeyvalues0_2 {
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
	readkeyvalues0_158 {
		kvdram_V {Type I LastRead 9 FirstWrite -1}
		kvdram_V_offset {Type I LastRead 0 FirstWrite -1}
		buffer_V {Type O LastRead -1 FirstWrite 10}
		baseaddress {Type I LastRead 0 FirstWrite -1}
		offset_kvs {Type I LastRead 0 FirstWrite -1}
		size_kvs {Type I LastRead 1 FirstWrite -1}}
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
		size_kvs {Type I LastRead 1 FirstWrite -1}}
	savekeyvalues0_153 {
		kvdram_V {Type O LastRead 3 FirstWrite 4}
		kvdram_V_offset {Type I LastRead 0 FirstWrite -1}
		buffer_V {Type I LastRead 2 FirstWrite -1}
		offset_kvs {Type I LastRead 0 FirstWrite -1}}
	readkeyvalues0_157 {
		kvdram_V {Type I LastRead 9 FirstWrite -1}
		kvdram_V_offset {Type I LastRead 0 FirstWrite -1}
		buffer_V {Type O LastRead -1 FirstWrite 10}
		offset_kvs {Type I LastRead 0 FirstWrite -1}}
	savekeyvalues0_2 {
		dram {Type O LastRead 3 FirstWrite 4}
		dram_offset {Type I LastRead 0 FirstWrite -1}
		buffer_key {Type I LastRead 2 FirstWrite -1}
		buffer_value {Type I LastRead 2 FirstWrite -1}
		offset_kvs {Type I LastRead 0 FirstWrite -1}}
	savekeyvalues0_1 {
		dram {Type O LastRead 3 FirstWrite 2}
		dram_offset {Type I LastRead 0 FirstWrite -1}
		buffer_0_key_read {Type I LastRead 0 FirstWrite -1}
		buffer_0_value_read {Type I LastRead 0 FirstWrite -1}
		offset_kvs {Type I LastRead 0 FirstWrite -1}}}

set hasDtUnsupportedChannel 0

set PerformanceInfo {[
	{"Name" : "Latency", "Min" : "41", "Max" : "197850340"}
	, {"Name" : "Interval", "Min" : "41", "Max" : "197850340"}
]}

set PipelineEnableSignalInfo {[
]}

set Spec2ImplPortList { 
	kvsourcedram_V { m_axi {  { m_axi_kvsourcedram_V_AWVALID VALID 1 1 }  { m_axi_kvsourcedram_V_AWREADY READY 0 1 }  { m_axi_kvsourcedram_V_AWADDR ADDR 1 32 }  { m_axi_kvsourcedram_V_AWID ID 1 1 }  { m_axi_kvsourcedram_V_AWLEN LEN 1 32 }  { m_axi_kvsourcedram_V_AWSIZE SIZE 1 3 }  { m_axi_kvsourcedram_V_AWBURST BURST 1 2 }  { m_axi_kvsourcedram_V_AWLOCK LOCK 1 2 }  { m_axi_kvsourcedram_V_AWCACHE CACHE 1 4 }  { m_axi_kvsourcedram_V_AWPROT PROT 1 3 }  { m_axi_kvsourcedram_V_AWQOS QOS 1 4 }  { m_axi_kvsourcedram_V_AWREGION REGION 1 4 }  { m_axi_kvsourcedram_V_AWUSER USER 1 1 }  { m_axi_kvsourcedram_V_WVALID VALID 1 1 }  { m_axi_kvsourcedram_V_WREADY READY 0 1 }  { m_axi_kvsourcedram_V_WDATA DATA 1 512 }  { m_axi_kvsourcedram_V_WSTRB STRB 1 64 }  { m_axi_kvsourcedram_V_WLAST LAST 1 1 }  { m_axi_kvsourcedram_V_WID ID 1 1 }  { m_axi_kvsourcedram_V_WUSER USER 1 1 }  { m_axi_kvsourcedram_V_ARVALID VALID 1 1 }  { m_axi_kvsourcedram_V_ARREADY READY 0 1 }  { m_axi_kvsourcedram_V_ARADDR ADDR 1 32 }  { m_axi_kvsourcedram_V_ARID ID 1 1 }  { m_axi_kvsourcedram_V_ARLEN LEN 1 32 }  { m_axi_kvsourcedram_V_ARSIZE SIZE 1 3 }  { m_axi_kvsourcedram_V_ARBURST BURST 1 2 }  { m_axi_kvsourcedram_V_ARLOCK LOCK 1 2 }  { m_axi_kvsourcedram_V_ARCACHE CACHE 1 4 }  { m_axi_kvsourcedram_V_ARPROT PROT 1 3 }  { m_axi_kvsourcedram_V_ARQOS QOS 1 4 }  { m_axi_kvsourcedram_V_ARREGION REGION 1 4 }  { m_axi_kvsourcedram_V_ARUSER USER 1 1 }  { m_axi_kvsourcedram_V_RVALID VALID 0 1 }  { m_axi_kvsourcedram_V_RREADY READY 1 1 }  { m_axi_kvsourcedram_V_RDATA DATA 0 512 }  { m_axi_kvsourcedram_V_RLAST LAST 0 1 }  { m_axi_kvsourcedram_V_RID ID 0 1 }  { m_axi_kvsourcedram_V_RUSER USER 0 1 }  { m_axi_kvsourcedram_V_RRESP RESP 0 2 }  { m_axi_kvsourcedram_V_BVALID VALID 0 1 }  { m_axi_kvsourcedram_V_BREADY READY 1 1 }  { m_axi_kvsourcedram_V_BRESP RESP 0 2 }  { m_axi_kvsourcedram_V_BID ID 0 1 }  { m_axi_kvsourcedram_V_BUSER USER 0 1 } } }
	kvsourcedram_V_offset { ap_none {  { kvsourcedram_V_offset in_data 0 26 } } }
	kvdestdram_V { m_axi {  { m_axi_kvdestdram_V_AWVALID VALID 1 1 }  { m_axi_kvdestdram_V_AWREADY READY 0 1 }  { m_axi_kvdestdram_V_AWADDR ADDR 1 32 }  { m_axi_kvdestdram_V_AWID ID 1 1 }  { m_axi_kvdestdram_V_AWLEN LEN 1 32 }  { m_axi_kvdestdram_V_AWSIZE SIZE 1 3 }  { m_axi_kvdestdram_V_AWBURST BURST 1 2 }  { m_axi_kvdestdram_V_AWLOCK LOCK 1 2 }  { m_axi_kvdestdram_V_AWCACHE CACHE 1 4 }  { m_axi_kvdestdram_V_AWPROT PROT 1 3 }  { m_axi_kvdestdram_V_AWQOS QOS 1 4 }  { m_axi_kvdestdram_V_AWREGION REGION 1 4 }  { m_axi_kvdestdram_V_AWUSER USER 1 1 }  { m_axi_kvdestdram_V_WVALID VALID 1 1 }  { m_axi_kvdestdram_V_WREADY READY 0 1 }  { m_axi_kvdestdram_V_WDATA DATA 1 512 }  { m_axi_kvdestdram_V_WSTRB STRB 1 64 }  { m_axi_kvdestdram_V_WLAST LAST 1 1 }  { m_axi_kvdestdram_V_WID ID 1 1 }  { m_axi_kvdestdram_V_WUSER USER 1 1 }  { m_axi_kvdestdram_V_ARVALID VALID 1 1 }  { m_axi_kvdestdram_V_ARREADY READY 0 1 }  { m_axi_kvdestdram_V_ARADDR ADDR 1 32 }  { m_axi_kvdestdram_V_ARID ID 1 1 }  { m_axi_kvdestdram_V_ARLEN LEN 1 32 }  { m_axi_kvdestdram_V_ARSIZE SIZE 1 3 }  { m_axi_kvdestdram_V_ARBURST BURST 1 2 }  { m_axi_kvdestdram_V_ARLOCK LOCK 1 2 }  { m_axi_kvdestdram_V_ARCACHE CACHE 1 4 }  { m_axi_kvdestdram_V_ARPROT PROT 1 3 }  { m_axi_kvdestdram_V_ARQOS QOS 1 4 }  { m_axi_kvdestdram_V_ARREGION REGION 1 4 }  { m_axi_kvdestdram_V_ARUSER USER 1 1 }  { m_axi_kvdestdram_V_RVALID VALID 0 1 }  { m_axi_kvdestdram_V_RREADY READY 1 1 }  { m_axi_kvdestdram_V_RDATA DATA 0 512 }  { m_axi_kvdestdram_V_RLAST LAST 0 1 }  { m_axi_kvdestdram_V_RID ID 0 1 }  { m_axi_kvdestdram_V_RUSER USER 0 1 }  { m_axi_kvdestdram_V_RRESP RESP 0 2 }  { m_axi_kvdestdram_V_BVALID VALID 0 1 }  { m_axi_kvdestdram_V_BREADY READY 1 1 }  { m_axi_kvdestdram_V_BRESP RESP 0 2 }  { m_axi_kvdestdram_V_BID ID 0 1 }  { m_axi_kvdestdram_V_BUSER USER 0 1 } } }
	kvdestdram_V_offset { ap_none {  { kvdestdram_V_offset in_data 0 26 } } }
	kvstats { m_axi {  { m_axi_kvstats_AWVALID VALID 1 1 }  { m_axi_kvstats_AWREADY READY 0 1 }  { m_axi_kvstats_AWADDR ADDR 1 32 }  { m_axi_kvstats_AWID ID 1 1 }  { m_axi_kvstats_AWLEN LEN 1 32 }  { m_axi_kvstats_AWSIZE SIZE 1 3 }  { m_axi_kvstats_AWBURST BURST 1 2 }  { m_axi_kvstats_AWLOCK LOCK 1 2 }  { m_axi_kvstats_AWCACHE CACHE 1 4 }  { m_axi_kvstats_AWPROT PROT 1 3 }  { m_axi_kvstats_AWQOS QOS 1 4 }  { m_axi_kvstats_AWREGION REGION 1 4 }  { m_axi_kvstats_AWUSER USER 1 1 }  { m_axi_kvstats_WVALID VALID 1 1 }  { m_axi_kvstats_WREADY READY 0 1 }  { m_axi_kvstats_WDATA DATA 1 64 }  { m_axi_kvstats_WSTRB STRB 1 8 }  { m_axi_kvstats_WLAST LAST 1 1 }  { m_axi_kvstats_WID ID 1 1 }  { m_axi_kvstats_WUSER USER 1 1 }  { m_axi_kvstats_ARVALID VALID 1 1 }  { m_axi_kvstats_ARREADY READY 0 1 }  { m_axi_kvstats_ARADDR ADDR 1 32 }  { m_axi_kvstats_ARID ID 1 1 }  { m_axi_kvstats_ARLEN LEN 1 32 }  { m_axi_kvstats_ARSIZE SIZE 1 3 }  { m_axi_kvstats_ARBURST BURST 1 2 }  { m_axi_kvstats_ARLOCK LOCK 1 2 }  { m_axi_kvstats_ARCACHE CACHE 1 4 }  { m_axi_kvstats_ARPROT PROT 1 3 }  { m_axi_kvstats_ARQOS QOS 1 4 }  { m_axi_kvstats_ARREGION REGION 1 4 }  { m_axi_kvstats_ARUSER USER 1 1 }  { m_axi_kvstats_RVALID VALID 0 1 }  { m_axi_kvstats_RREADY READY 1 1 }  { m_axi_kvstats_RDATA DATA 0 64 }  { m_axi_kvstats_RLAST LAST 0 1 }  { m_axi_kvstats_RID ID 0 1 }  { m_axi_kvstats_RUSER USER 0 1 }  { m_axi_kvstats_RRESP RESP 0 2 }  { m_axi_kvstats_BVALID VALID 0 1 }  { m_axi_kvstats_BREADY READY 1 1 }  { m_axi_kvstats_BRESP RESP 0 2 }  { m_axi_kvstats_BID ID 0 1 }  { m_axi_kvstats_BUSER USER 0 1 } } }
	kvstats_offset { ap_none {  { kvstats_offset in_data 0 29 } } }
	globalparams_runkernelcommand { ap_none {  { globalparams_runkernelcommand in_data 0 32 } } }
	globalparams_partitioncommand { ap_none {  { globalparams_partitioncommand in_data 0 32 } } }
	globalparams_reducecommand { ap_none {  { globalparams_reducecommand in_data 0 32 } } }
	globalparams_treedepth { ap_none {  { globalparams_treedepth in_data 0 32 } } }
	globalparams_runsize { ap_none {  { globalparams_runsize in_data 0 32 } } }
	globalparams_vbegin { ap_none {  { globalparams_vbegin in_data 0 32 } } }
	globalparams_statsalreadycollected { ap_none {  { globalparams_statsalreadycollected in_data 0 32 } } }
}
set moduleName partitionandreduce0
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
set C_modelName {partitionandreduce0}
set C_modelType { void 0 }
set C_modelArgList {
	{ kvsourcedram_V int 512 regular {axi_master 2}  }
	{ kvsourcedram_V_offset int 26 regular  }
	{ kvdestdram_V int 512 regular {axi_master 2}  }
	{ kvdestdram_V_offset int 26 regular  }
	{ kvstats int 64 regular {axi_master 2}  }
	{ kvstats_offset int 29 regular  }
	{ globalparams_runkernelcommand int 32 regular  }
	{ globalparams_partitioncommand int 32 regular  }
	{ globalparams_reducecommand int 32 regular  }
	{ globalparams_treedepth int 32 regular  }
	{ globalparams_runsize int 32 regular  }
	{ globalparams_vbegin int 32 regular  }
	{ globalparams_statsalreadycollected int 32 regular  }
}
set C_modelArgMapList {[ 
	{ "Name" : "kvsourcedram_V", "interface" : "axi_master", "bitwidth" : 512, "direction" : "READWRITE"} , 
 	{ "Name" : "kvsourcedram_V_offset", "interface" : "wire", "bitwidth" : 26, "direction" : "READONLY"} , 
 	{ "Name" : "kvdestdram_V", "interface" : "axi_master", "bitwidth" : 512, "direction" : "READWRITE"} , 
 	{ "Name" : "kvdestdram_V_offset", "interface" : "wire", "bitwidth" : 26, "direction" : "READONLY"} , 
 	{ "Name" : "kvstats", "interface" : "axi_master", "bitwidth" : 64, "direction" : "READWRITE"} , 
 	{ "Name" : "kvstats_offset", "interface" : "wire", "bitwidth" : 29, "direction" : "READONLY"} , 
 	{ "Name" : "globalparams_runkernelcommand", "interface" : "wire", "bitwidth" : 32, "direction" : "READONLY"} , 
 	{ "Name" : "globalparams_partitioncommand", "interface" : "wire", "bitwidth" : 32, "direction" : "READONLY"} , 
 	{ "Name" : "globalparams_reducecommand", "interface" : "wire", "bitwidth" : 32, "direction" : "READONLY"} , 
 	{ "Name" : "globalparams_treedepth", "interface" : "wire", "bitwidth" : 32, "direction" : "READONLY"} , 
 	{ "Name" : "globalparams_runsize", "interface" : "wire", "bitwidth" : 32, "direction" : "READONLY"} , 
 	{ "Name" : "globalparams_vbegin", "interface" : "wire", "bitwidth" : 32, "direction" : "READONLY"} , 
 	{ "Name" : "globalparams_statsalreadycollected", "interface" : "wire", "bitwidth" : 32, "direction" : "READONLY"} ]}
# RTL Port declarations: 
set portNum 151
set portList { 
	{ ap_clk sc_in sc_logic 1 clock -1 } 
	{ ap_rst sc_in sc_logic 1 reset -1 active_high_sync } 
	{ ap_start sc_in sc_logic 1 start -1 } 
	{ ap_done sc_out sc_logic 1 predone -1 } 
	{ ap_idle sc_out sc_logic 1 done -1 } 
	{ ap_ready sc_out sc_logic 1 ready -1 } 
	{ m_axi_kvsourcedram_V_AWVALID sc_out sc_logic 1 signal 0 } 
	{ m_axi_kvsourcedram_V_AWREADY sc_in sc_logic 1 signal 0 } 
	{ m_axi_kvsourcedram_V_AWADDR sc_out sc_lv 32 signal 0 } 
	{ m_axi_kvsourcedram_V_AWID sc_out sc_lv 1 signal 0 } 
	{ m_axi_kvsourcedram_V_AWLEN sc_out sc_lv 32 signal 0 } 
	{ m_axi_kvsourcedram_V_AWSIZE sc_out sc_lv 3 signal 0 } 
	{ m_axi_kvsourcedram_V_AWBURST sc_out sc_lv 2 signal 0 } 
	{ m_axi_kvsourcedram_V_AWLOCK sc_out sc_lv 2 signal 0 } 
	{ m_axi_kvsourcedram_V_AWCACHE sc_out sc_lv 4 signal 0 } 
	{ m_axi_kvsourcedram_V_AWPROT sc_out sc_lv 3 signal 0 } 
	{ m_axi_kvsourcedram_V_AWQOS sc_out sc_lv 4 signal 0 } 
	{ m_axi_kvsourcedram_V_AWREGION sc_out sc_lv 4 signal 0 } 
	{ m_axi_kvsourcedram_V_AWUSER sc_out sc_lv 1 signal 0 } 
	{ m_axi_kvsourcedram_V_WVALID sc_out sc_logic 1 signal 0 } 
	{ m_axi_kvsourcedram_V_WREADY sc_in sc_logic 1 signal 0 } 
	{ m_axi_kvsourcedram_V_WDATA sc_out sc_lv 512 signal 0 } 
	{ m_axi_kvsourcedram_V_WSTRB sc_out sc_lv 64 signal 0 } 
	{ m_axi_kvsourcedram_V_WLAST sc_out sc_logic 1 signal 0 } 
	{ m_axi_kvsourcedram_V_WID sc_out sc_lv 1 signal 0 } 
	{ m_axi_kvsourcedram_V_WUSER sc_out sc_lv 1 signal 0 } 
	{ m_axi_kvsourcedram_V_ARVALID sc_out sc_logic 1 signal 0 } 
	{ m_axi_kvsourcedram_V_ARREADY sc_in sc_logic 1 signal 0 } 
	{ m_axi_kvsourcedram_V_ARADDR sc_out sc_lv 32 signal 0 } 
	{ m_axi_kvsourcedram_V_ARID sc_out sc_lv 1 signal 0 } 
	{ m_axi_kvsourcedram_V_ARLEN sc_out sc_lv 32 signal 0 } 
	{ m_axi_kvsourcedram_V_ARSIZE sc_out sc_lv 3 signal 0 } 
	{ m_axi_kvsourcedram_V_ARBURST sc_out sc_lv 2 signal 0 } 
	{ m_axi_kvsourcedram_V_ARLOCK sc_out sc_lv 2 signal 0 } 
	{ m_axi_kvsourcedram_V_ARCACHE sc_out sc_lv 4 signal 0 } 
	{ m_axi_kvsourcedram_V_ARPROT sc_out sc_lv 3 signal 0 } 
	{ m_axi_kvsourcedram_V_ARQOS sc_out sc_lv 4 signal 0 } 
	{ m_axi_kvsourcedram_V_ARREGION sc_out sc_lv 4 signal 0 } 
	{ m_axi_kvsourcedram_V_ARUSER sc_out sc_lv 1 signal 0 } 
	{ m_axi_kvsourcedram_V_RVALID sc_in sc_logic 1 signal 0 } 
	{ m_axi_kvsourcedram_V_RREADY sc_out sc_logic 1 signal 0 } 
	{ m_axi_kvsourcedram_V_RDATA sc_in sc_lv 512 signal 0 } 
	{ m_axi_kvsourcedram_V_RLAST sc_in sc_logic 1 signal 0 } 
	{ m_axi_kvsourcedram_V_RID sc_in sc_lv 1 signal 0 } 
	{ m_axi_kvsourcedram_V_RUSER sc_in sc_lv 1 signal 0 } 
	{ m_axi_kvsourcedram_V_RRESP sc_in sc_lv 2 signal 0 } 
	{ m_axi_kvsourcedram_V_BVALID sc_in sc_logic 1 signal 0 } 
	{ m_axi_kvsourcedram_V_BREADY sc_out sc_logic 1 signal 0 } 
	{ m_axi_kvsourcedram_V_BRESP sc_in sc_lv 2 signal 0 } 
	{ m_axi_kvsourcedram_V_BID sc_in sc_lv 1 signal 0 } 
	{ m_axi_kvsourcedram_V_BUSER sc_in sc_lv 1 signal 0 } 
	{ kvsourcedram_V_offset sc_in sc_lv 26 signal 1 } 
	{ m_axi_kvdestdram_V_AWVALID sc_out sc_logic 1 signal 2 } 
	{ m_axi_kvdestdram_V_AWREADY sc_in sc_logic 1 signal 2 } 
	{ m_axi_kvdestdram_V_AWADDR sc_out sc_lv 32 signal 2 } 
	{ m_axi_kvdestdram_V_AWID sc_out sc_lv 1 signal 2 } 
	{ m_axi_kvdestdram_V_AWLEN sc_out sc_lv 32 signal 2 } 
	{ m_axi_kvdestdram_V_AWSIZE sc_out sc_lv 3 signal 2 } 
	{ m_axi_kvdestdram_V_AWBURST sc_out sc_lv 2 signal 2 } 
	{ m_axi_kvdestdram_V_AWLOCK sc_out sc_lv 2 signal 2 } 
	{ m_axi_kvdestdram_V_AWCACHE sc_out sc_lv 4 signal 2 } 
	{ m_axi_kvdestdram_V_AWPROT sc_out sc_lv 3 signal 2 } 
	{ m_axi_kvdestdram_V_AWQOS sc_out sc_lv 4 signal 2 } 
	{ m_axi_kvdestdram_V_AWREGION sc_out sc_lv 4 signal 2 } 
	{ m_axi_kvdestdram_V_AWUSER sc_out sc_lv 1 signal 2 } 
	{ m_axi_kvdestdram_V_WVALID sc_out sc_logic 1 signal 2 } 
	{ m_axi_kvdestdram_V_WREADY sc_in sc_logic 1 signal 2 } 
	{ m_axi_kvdestdram_V_WDATA sc_out sc_lv 512 signal 2 } 
	{ m_axi_kvdestdram_V_WSTRB sc_out sc_lv 64 signal 2 } 
	{ m_axi_kvdestdram_V_WLAST sc_out sc_logic 1 signal 2 } 
	{ m_axi_kvdestdram_V_WID sc_out sc_lv 1 signal 2 } 
	{ m_axi_kvdestdram_V_WUSER sc_out sc_lv 1 signal 2 } 
	{ m_axi_kvdestdram_V_ARVALID sc_out sc_logic 1 signal 2 } 
	{ m_axi_kvdestdram_V_ARREADY sc_in sc_logic 1 signal 2 } 
	{ m_axi_kvdestdram_V_ARADDR sc_out sc_lv 32 signal 2 } 
	{ m_axi_kvdestdram_V_ARID sc_out sc_lv 1 signal 2 } 
	{ m_axi_kvdestdram_V_ARLEN sc_out sc_lv 32 signal 2 } 
	{ m_axi_kvdestdram_V_ARSIZE sc_out sc_lv 3 signal 2 } 
	{ m_axi_kvdestdram_V_ARBURST sc_out sc_lv 2 signal 2 } 
	{ m_axi_kvdestdram_V_ARLOCK sc_out sc_lv 2 signal 2 } 
	{ m_axi_kvdestdram_V_ARCACHE sc_out sc_lv 4 signal 2 } 
	{ m_axi_kvdestdram_V_ARPROT sc_out sc_lv 3 signal 2 } 
	{ m_axi_kvdestdram_V_ARQOS sc_out sc_lv 4 signal 2 } 
	{ m_axi_kvdestdram_V_ARREGION sc_out sc_lv 4 signal 2 } 
	{ m_axi_kvdestdram_V_ARUSER sc_out sc_lv 1 signal 2 } 
	{ m_axi_kvdestdram_V_RVALID sc_in sc_logic 1 signal 2 } 
	{ m_axi_kvdestdram_V_RREADY sc_out sc_logic 1 signal 2 } 
	{ m_axi_kvdestdram_V_RDATA sc_in sc_lv 512 signal 2 } 
	{ m_axi_kvdestdram_V_RLAST sc_in sc_logic 1 signal 2 } 
	{ m_axi_kvdestdram_V_RID sc_in sc_lv 1 signal 2 } 
	{ m_axi_kvdestdram_V_RUSER sc_in sc_lv 1 signal 2 } 
	{ m_axi_kvdestdram_V_RRESP sc_in sc_lv 2 signal 2 } 
	{ m_axi_kvdestdram_V_BVALID sc_in sc_logic 1 signal 2 } 
	{ m_axi_kvdestdram_V_BREADY sc_out sc_logic 1 signal 2 } 
	{ m_axi_kvdestdram_V_BRESP sc_in sc_lv 2 signal 2 } 
	{ m_axi_kvdestdram_V_BID sc_in sc_lv 1 signal 2 } 
	{ m_axi_kvdestdram_V_BUSER sc_in sc_lv 1 signal 2 } 
	{ kvdestdram_V_offset sc_in sc_lv 26 signal 3 } 
	{ m_axi_kvstats_AWVALID sc_out sc_logic 1 signal 4 } 
	{ m_axi_kvstats_AWREADY sc_in sc_logic 1 signal 4 } 
	{ m_axi_kvstats_AWADDR sc_out sc_lv 32 signal 4 } 
	{ m_axi_kvstats_AWID sc_out sc_lv 1 signal 4 } 
	{ m_axi_kvstats_AWLEN sc_out sc_lv 32 signal 4 } 
	{ m_axi_kvstats_AWSIZE sc_out sc_lv 3 signal 4 } 
	{ m_axi_kvstats_AWBURST sc_out sc_lv 2 signal 4 } 
	{ m_axi_kvstats_AWLOCK sc_out sc_lv 2 signal 4 } 
	{ m_axi_kvstats_AWCACHE sc_out sc_lv 4 signal 4 } 
	{ m_axi_kvstats_AWPROT sc_out sc_lv 3 signal 4 } 
	{ m_axi_kvstats_AWQOS sc_out sc_lv 4 signal 4 } 
	{ m_axi_kvstats_AWREGION sc_out sc_lv 4 signal 4 } 
	{ m_axi_kvstats_AWUSER sc_out sc_lv 1 signal 4 } 
	{ m_axi_kvstats_WVALID sc_out sc_logic 1 signal 4 } 
	{ m_axi_kvstats_WREADY sc_in sc_logic 1 signal 4 } 
	{ m_axi_kvstats_WDATA sc_out sc_lv 64 signal 4 } 
	{ m_axi_kvstats_WSTRB sc_out sc_lv 8 signal 4 } 
	{ m_axi_kvstats_WLAST sc_out sc_logic 1 signal 4 } 
	{ m_axi_kvstats_WID sc_out sc_lv 1 signal 4 } 
	{ m_axi_kvstats_WUSER sc_out sc_lv 1 signal 4 } 
	{ m_axi_kvstats_ARVALID sc_out sc_logic 1 signal 4 } 
	{ m_axi_kvstats_ARREADY sc_in sc_logic 1 signal 4 } 
	{ m_axi_kvstats_ARADDR sc_out sc_lv 32 signal 4 } 
	{ m_axi_kvstats_ARID sc_out sc_lv 1 signal 4 } 
	{ m_axi_kvstats_ARLEN sc_out sc_lv 32 signal 4 } 
	{ m_axi_kvstats_ARSIZE sc_out sc_lv 3 signal 4 } 
	{ m_axi_kvstats_ARBURST sc_out sc_lv 2 signal 4 } 
	{ m_axi_kvstats_ARLOCK sc_out sc_lv 2 signal 4 } 
	{ m_axi_kvstats_ARCACHE sc_out sc_lv 4 signal 4 } 
	{ m_axi_kvstats_ARPROT sc_out sc_lv 3 signal 4 } 
	{ m_axi_kvstats_ARQOS sc_out sc_lv 4 signal 4 } 
	{ m_axi_kvstats_ARREGION sc_out sc_lv 4 signal 4 } 
	{ m_axi_kvstats_ARUSER sc_out sc_lv 1 signal 4 } 
	{ m_axi_kvstats_RVALID sc_in sc_logic 1 signal 4 } 
	{ m_axi_kvstats_RREADY sc_out sc_logic 1 signal 4 } 
	{ m_axi_kvstats_RDATA sc_in sc_lv 64 signal 4 } 
	{ m_axi_kvstats_RLAST sc_in sc_logic 1 signal 4 } 
	{ m_axi_kvstats_RID sc_in sc_lv 1 signal 4 } 
	{ m_axi_kvstats_RUSER sc_in sc_lv 1 signal 4 } 
	{ m_axi_kvstats_RRESP sc_in sc_lv 2 signal 4 } 
	{ m_axi_kvstats_BVALID sc_in sc_logic 1 signal 4 } 
	{ m_axi_kvstats_BREADY sc_out sc_logic 1 signal 4 } 
	{ m_axi_kvstats_BRESP sc_in sc_lv 2 signal 4 } 
	{ m_axi_kvstats_BID sc_in sc_lv 1 signal 4 } 
	{ m_axi_kvstats_BUSER sc_in sc_lv 1 signal 4 } 
	{ kvstats_offset sc_in sc_lv 29 signal 5 } 
	{ globalparams_runkernelcommand sc_in sc_lv 32 signal 6 } 
	{ globalparams_partitioncommand sc_in sc_lv 32 signal 7 } 
	{ globalparams_reducecommand sc_in sc_lv 32 signal 8 } 
	{ globalparams_treedepth sc_in sc_lv 32 signal 9 } 
	{ globalparams_runsize sc_in sc_lv 32 signal 10 } 
	{ globalparams_vbegin sc_in sc_lv 32 signal 11 } 
	{ globalparams_statsalreadycollected sc_in sc_lv 32 signal 12 } 
}
set NewPortList {[ 
	{ "name": "ap_clk", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "clock", "bundle":{"name": "ap_clk", "role": "default" }} , 
 	{ "name": "ap_rst", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "reset", "bundle":{"name": "ap_rst", "role": "default" }} , 
 	{ "name": "ap_start", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "start", "bundle":{"name": "ap_start", "role": "default" }} , 
 	{ "name": "ap_done", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "predone", "bundle":{"name": "ap_done", "role": "default" }} , 
 	{ "name": "ap_idle", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "done", "bundle":{"name": "ap_idle", "role": "default" }} , 
 	{ "name": "ap_ready", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "ready", "bundle":{"name": "ap_ready", "role": "default" }} , 
 	{ "name": "m_axi_kvsourcedram_V_AWVALID", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "kvsourcedram_V", "role": "AWVALID" }} , 
 	{ "name": "m_axi_kvsourcedram_V_AWREADY", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "kvsourcedram_V", "role": "AWREADY" }} , 
 	{ "name": "m_axi_kvsourcedram_V_AWADDR", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "kvsourcedram_V", "role": "AWADDR" }} , 
 	{ "name": "m_axi_kvsourcedram_V_AWID", "direction": "out", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "kvsourcedram_V", "role": "AWID" }} , 
 	{ "name": "m_axi_kvsourcedram_V_AWLEN", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "kvsourcedram_V", "role": "AWLEN" }} , 
 	{ "name": "m_axi_kvsourcedram_V_AWSIZE", "direction": "out", "datatype": "sc_lv", "bitwidth":3, "type": "signal", "bundle":{"name": "kvsourcedram_V", "role": "AWSIZE" }} , 
 	{ "name": "m_axi_kvsourcedram_V_AWBURST", "direction": "out", "datatype": "sc_lv", "bitwidth":2, "type": "signal", "bundle":{"name": "kvsourcedram_V", "role": "AWBURST" }} , 
 	{ "name": "m_axi_kvsourcedram_V_AWLOCK", "direction": "out", "datatype": "sc_lv", "bitwidth":2, "type": "signal", "bundle":{"name": "kvsourcedram_V", "role": "AWLOCK" }} , 
 	{ "name": "m_axi_kvsourcedram_V_AWCACHE", "direction": "out", "datatype": "sc_lv", "bitwidth":4, "type": "signal", "bundle":{"name": "kvsourcedram_V", "role": "AWCACHE" }} , 
 	{ "name": "m_axi_kvsourcedram_V_AWPROT", "direction": "out", "datatype": "sc_lv", "bitwidth":3, "type": "signal", "bundle":{"name": "kvsourcedram_V", "role": "AWPROT" }} , 
 	{ "name": "m_axi_kvsourcedram_V_AWQOS", "direction": "out", "datatype": "sc_lv", "bitwidth":4, "type": "signal", "bundle":{"name": "kvsourcedram_V", "role": "AWQOS" }} , 
 	{ "name": "m_axi_kvsourcedram_V_AWREGION", "direction": "out", "datatype": "sc_lv", "bitwidth":4, "type": "signal", "bundle":{"name": "kvsourcedram_V", "role": "AWREGION" }} , 
 	{ "name": "m_axi_kvsourcedram_V_AWUSER", "direction": "out", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "kvsourcedram_V", "role": "AWUSER" }} , 
 	{ "name": "m_axi_kvsourcedram_V_WVALID", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "kvsourcedram_V", "role": "WVALID" }} , 
 	{ "name": "m_axi_kvsourcedram_V_WREADY", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "kvsourcedram_V", "role": "WREADY" }} , 
 	{ "name": "m_axi_kvsourcedram_V_WDATA", "direction": "out", "datatype": "sc_lv", "bitwidth":512, "type": "signal", "bundle":{"name": "kvsourcedram_V", "role": "WDATA" }} , 
 	{ "name": "m_axi_kvsourcedram_V_WSTRB", "direction": "out", "datatype": "sc_lv", "bitwidth":64, "type": "signal", "bundle":{"name": "kvsourcedram_V", "role": "WSTRB" }} , 
 	{ "name": "m_axi_kvsourcedram_V_WLAST", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "kvsourcedram_V", "role": "WLAST" }} , 
 	{ "name": "m_axi_kvsourcedram_V_WID", "direction": "out", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "kvsourcedram_V", "role": "WID" }} , 
 	{ "name": "m_axi_kvsourcedram_V_WUSER", "direction": "out", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "kvsourcedram_V", "role": "WUSER" }} , 
 	{ "name": "m_axi_kvsourcedram_V_ARVALID", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "kvsourcedram_V", "role": "ARVALID" }} , 
 	{ "name": "m_axi_kvsourcedram_V_ARREADY", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "kvsourcedram_V", "role": "ARREADY" }} , 
 	{ "name": "m_axi_kvsourcedram_V_ARADDR", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "kvsourcedram_V", "role": "ARADDR" }} , 
 	{ "name": "m_axi_kvsourcedram_V_ARID", "direction": "out", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "kvsourcedram_V", "role": "ARID" }} , 
 	{ "name": "m_axi_kvsourcedram_V_ARLEN", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "kvsourcedram_V", "role": "ARLEN" }} , 
 	{ "name": "m_axi_kvsourcedram_V_ARSIZE", "direction": "out", "datatype": "sc_lv", "bitwidth":3, "type": "signal", "bundle":{"name": "kvsourcedram_V", "role": "ARSIZE" }} , 
 	{ "name": "m_axi_kvsourcedram_V_ARBURST", "direction": "out", "datatype": "sc_lv", "bitwidth":2, "type": "signal", "bundle":{"name": "kvsourcedram_V", "role": "ARBURST" }} , 
 	{ "name": "m_axi_kvsourcedram_V_ARLOCK", "direction": "out", "datatype": "sc_lv", "bitwidth":2, "type": "signal", "bundle":{"name": "kvsourcedram_V", "role": "ARLOCK" }} , 
 	{ "name": "m_axi_kvsourcedram_V_ARCACHE", "direction": "out", "datatype": "sc_lv", "bitwidth":4, "type": "signal", "bundle":{"name": "kvsourcedram_V", "role": "ARCACHE" }} , 
 	{ "name": "m_axi_kvsourcedram_V_ARPROT", "direction": "out", "datatype": "sc_lv", "bitwidth":3, "type": "signal", "bundle":{"name": "kvsourcedram_V", "role": "ARPROT" }} , 
 	{ "name": "m_axi_kvsourcedram_V_ARQOS", "direction": "out", "datatype": "sc_lv", "bitwidth":4, "type": "signal", "bundle":{"name": "kvsourcedram_V", "role": "ARQOS" }} , 
 	{ "name": "m_axi_kvsourcedram_V_ARREGION", "direction": "out", "datatype": "sc_lv", "bitwidth":4, "type": "signal", "bundle":{"name": "kvsourcedram_V", "role": "ARREGION" }} , 
 	{ "name": "m_axi_kvsourcedram_V_ARUSER", "direction": "out", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "kvsourcedram_V", "role": "ARUSER" }} , 
 	{ "name": "m_axi_kvsourcedram_V_RVALID", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "kvsourcedram_V", "role": "RVALID" }} , 
 	{ "name": "m_axi_kvsourcedram_V_RREADY", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "kvsourcedram_V", "role": "RREADY" }} , 
 	{ "name": "m_axi_kvsourcedram_V_RDATA", "direction": "in", "datatype": "sc_lv", "bitwidth":512, "type": "signal", "bundle":{"name": "kvsourcedram_V", "role": "RDATA" }} , 
 	{ "name": "m_axi_kvsourcedram_V_RLAST", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "kvsourcedram_V", "role": "RLAST" }} , 
 	{ "name": "m_axi_kvsourcedram_V_RID", "direction": "in", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "kvsourcedram_V", "role": "RID" }} , 
 	{ "name": "m_axi_kvsourcedram_V_RUSER", "direction": "in", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "kvsourcedram_V", "role": "RUSER" }} , 
 	{ "name": "m_axi_kvsourcedram_V_RRESP", "direction": "in", "datatype": "sc_lv", "bitwidth":2, "type": "signal", "bundle":{"name": "kvsourcedram_V", "role": "RRESP" }} , 
 	{ "name": "m_axi_kvsourcedram_V_BVALID", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "kvsourcedram_V", "role": "BVALID" }} , 
 	{ "name": "m_axi_kvsourcedram_V_BREADY", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "kvsourcedram_V", "role": "BREADY" }} , 
 	{ "name": "m_axi_kvsourcedram_V_BRESP", "direction": "in", "datatype": "sc_lv", "bitwidth":2, "type": "signal", "bundle":{"name": "kvsourcedram_V", "role": "BRESP" }} , 
 	{ "name": "m_axi_kvsourcedram_V_BID", "direction": "in", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "kvsourcedram_V", "role": "BID" }} , 
 	{ "name": "m_axi_kvsourcedram_V_BUSER", "direction": "in", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "kvsourcedram_V", "role": "BUSER" }} , 
 	{ "name": "kvsourcedram_V_offset", "direction": "in", "datatype": "sc_lv", "bitwidth":26, "type": "signal", "bundle":{"name": "kvsourcedram_V_offset", "role": "default" }} , 
 	{ "name": "m_axi_kvdestdram_V_AWVALID", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "kvdestdram_V", "role": "AWVALID" }} , 
 	{ "name": "m_axi_kvdestdram_V_AWREADY", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "kvdestdram_V", "role": "AWREADY" }} , 
 	{ "name": "m_axi_kvdestdram_V_AWADDR", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "kvdestdram_V", "role": "AWADDR" }} , 
 	{ "name": "m_axi_kvdestdram_V_AWID", "direction": "out", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "kvdestdram_V", "role": "AWID" }} , 
 	{ "name": "m_axi_kvdestdram_V_AWLEN", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "kvdestdram_V", "role": "AWLEN" }} , 
 	{ "name": "m_axi_kvdestdram_V_AWSIZE", "direction": "out", "datatype": "sc_lv", "bitwidth":3, "type": "signal", "bundle":{"name": "kvdestdram_V", "role": "AWSIZE" }} , 
 	{ "name": "m_axi_kvdestdram_V_AWBURST", "direction": "out", "datatype": "sc_lv", "bitwidth":2, "type": "signal", "bundle":{"name": "kvdestdram_V", "role": "AWBURST" }} , 
 	{ "name": "m_axi_kvdestdram_V_AWLOCK", "direction": "out", "datatype": "sc_lv", "bitwidth":2, "type": "signal", "bundle":{"name": "kvdestdram_V", "role": "AWLOCK" }} , 
 	{ "name": "m_axi_kvdestdram_V_AWCACHE", "direction": "out", "datatype": "sc_lv", "bitwidth":4, "type": "signal", "bundle":{"name": "kvdestdram_V", "role": "AWCACHE" }} , 
 	{ "name": "m_axi_kvdestdram_V_AWPROT", "direction": "out", "datatype": "sc_lv", "bitwidth":3, "type": "signal", "bundle":{"name": "kvdestdram_V", "role": "AWPROT" }} , 
 	{ "name": "m_axi_kvdestdram_V_AWQOS", "direction": "out", "datatype": "sc_lv", "bitwidth":4, "type": "signal", "bundle":{"name": "kvdestdram_V", "role": "AWQOS" }} , 
 	{ "name": "m_axi_kvdestdram_V_AWREGION", "direction": "out", "datatype": "sc_lv", "bitwidth":4, "type": "signal", "bundle":{"name": "kvdestdram_V", "role": "AWREGION" }} , 
 	{ "name": "m_axi_kvdestdram_V_AWUSER", "direction": "out", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "kvdestdram_V", "role": "AWUSER" }} , 
 	{ "name": "m_axi_kvdestdram_V_WVALID", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "kvdestdram_V", "role": "WVALID" }} , 
 	{ "name": "m_axi_kvdestdram_V_WREADY", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "kvdestdram_V", "role": "WREADY" }} , 
 	{ "name": "m_axi_kvdestdram_V_WDATA", "direction": "out", "datatype": "sc_lv", "bitwidth":512, "type": "signal", "bundle":{"name": "kvdestdram_V", "role": "WDATA" }} , 
 	{ "name": "m_axi_kvdestdram_V_WSTRB", "direction": "out", "datatype": "sc_lv", "bitwidth":64, "type": "signal", "bundle":{"name": "kvdestdram_V", "role": "WSTRB" }} , 
 	{ "name": "m_axi_kvdestdram_V_WLAST", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "kvdestdram_V", "role": "WLAST" }} , 
 	{ "name": "m_axi_kvdestdram_V_WID", "direction": "out", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "kvdestdram_V", "role": "WID" }} , 
 	{ "name": "m_axi_kvdestdram_V_WUSER", "direction": "out", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "kvdestdram_V", "role": "WUSER" }} , 
 	{ "name": "m_axi_kvdestdram_V_ARVALID", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "kvdestdram_V", "role": "ARVALID" }} , 
 	{ "name": "m_axi_kvdestdram_V_ARREADY", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "kvdestdram_V", "role": "ARREADY" }} , 
 	{ "name": "m_axi_kvdestdram_V_ARADDR", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "kvdestdram_V", "role": "ARADDR" }} , 
 	{ "name": "m_axi_kvdestdram_V_ARID", "direction": "out", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "kvdestdram_V", "role": "ARID" }} , 
 	{ "name": "m_axi_kvdestdram_V_ARLEN", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "kvdestdram_V", "role": "ARLEN" }} , 
 	{ "name": "m_axi_kvdestdram_V_ARSIZE", "direction": "out", "datatype": "sc_lv", "bitwidth":3, "type": "signal", "bundle":{"name": "kvdestdram_V", "role": "ARSIZE" }} , 
 	{ "name": "m_axi_kvdestdram_V_ARBURST", "direction": "out", "datatype": "sc_lv", "bitwidth":2, "type": "signal", "bundle":{"name": "kvdestdram_V", "role": "ARBURST" }} , 
 	{ "name": "m_axi_kvdestdram_V_ARLOCK", "direction": "out", "datatype": "sc_lv", "bitwidth":2, "type": "signal", "bundle":{"name": "kvdestdram_V", "role": "ARLOCK" }} , 
 	{ "name": "m_axi_kvdestdram_V_ARCACHE", "direction": "out", "datatype": "sc_lv", "bitwidth":4, "type": "signal", "bundle":{"name": "kvdestdram_V", "role": "ARCACHE" }} , 
 	{ "name": "m_axi_kvdestdram_V_ARPROT", "direction": "out", "datatype": "sc_lv", "bitwidth":3, "type": "signal", "bundle":{"name": "kvdestdram_V", "role": "ARPROT" }} , 
 	{ "name": "m_axi_kvdestdram_V_ARQOS", "direction": "out", "datatype": "sc_lv", "bitwidth":4, "type": "signal", "bundle":{"name": "kvdestdram_V", "role": "ARQOS" }} , 
 	{ "name": "m_axi_kvdestdram_V_ARREGION", "direction": "out", "datatype": "sc_lv", "bitwidth":4, "type": "signal", "bundle":{"name": "kvdestdram_V", "role": "ARREGION" }} , 
 	{ "name": "m_axi_kvdestdram_V_ARUSER", "direction": "out", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "kvdestdram_V", "role": "ARUSER" }} , 
 	{ "name": "m_axi_kvdestdram_V_RVALID", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "kvdestdram_V", "role": "RVALID" }} , 
 	{ "name": "m_axi_kvdestdram_V_RREADY", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "kvdestdram_V", "role": "RREADY" }} , 
 	{ "name": "m_axi_kvdestdram_V_RDATA", "direction": "in", "datatype": "sc_lv", "bitwidth":512, "type": "signal", "bundle":{"name": "kvdestdram_V", "role": "RDATA" }} , 
 	{ "name": "m_axi_kvdestdram_V_RLAST", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "kvdestdram_V", "role": "RLAST" }} , 
 	{ "name": "m_axi_kvdestdram_V_RID", "direction": "in", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "kvdestdram_V", "role": "RID" }} , 
 	{ "name": "m_axi_kvdestdram_V_RUSER", "direction": "in", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "kvdestdram_V", "role": "RUSER" }} , 
 	{ "name": "m_axi_kvdestdram_V_RRESP", "direction": "in", "datatype": "sc_lv", "bitwidth":2, "type": "signal", "bundle":{"name": "kvdestdram_V", "role": "RRESP" }} , 
 	{ "name": "m_axi_kvdestdram_V_BVALID", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "kvdestdram_V", "role": "BVALID" }} , 
 	{ "name": "m_axi_kvdestdram_V_BREADY", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "kvdestdram_V", "role": "BREADY" }} , 
 	{ "name": "m_axi_kvdestdram_V_BRESP", "direction": "in", "datatype": "sc_lv", "bitwidth":2, "type": "signal", "bundle":{"name": "kvdestdram_V", "role": "BRESP" }} , 
 	{ "name": "m_axi_kvdestdram_V_BID", "direction": "in", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "kvdestdram_V", "role": "BID" }} , 
 	{ "name": "m_axi_kvdestdram_V_BUSER", "direction": "in", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "kvdestdram_V", "role": "BUSER" }} , 
 	{ "name": "kvdestdram_V_offset", "direction": "in", "datatype": "sc_lv", "bitwidth":26, "type": "signal", "bundle":{"name": "kvdestdram_V_offset", "role": "default" }} , 
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
 	{ "name": "globalparams_runkernelcommand", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "globalparams_runkernelcommand", "role": "default" }} , 
 	{ "name": "globalparams_partitioncommand", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "globalparams_partitioncommand", "role": "default" }} , 
 	{ "name": "globalparams_reducecommand", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "globalparams_reducecommand", "role": "default" }} , 
 	{ "name": "globalparams_treedepth", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "globalparams_treedepth", "role": "default" }} , 
 	{ "name": "globalparams_runsize", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "globalparams_runsize", "role": "default" }} , 
 	{ "name": "globalparams_vbegin", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "globalparams_vbegin", "role": "default" }} , 
 	{ "name": "globalparams_statsalreadycollected", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "globalparams_statsalreadycollected", "role": "default" }}  ]}

set RtlHierarchyInfo {[
	{"ID" : "0", "Level" : "0", "Path" : "`AUTOTB_DUT_INST", "Parent" : "", "Child" : ["1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13", "14", "15", "16", "17", "18", "19", "20", "21", "22", "23", "24", "25", "26", "27", "28", "29", "30", "31", "32", "33", "34", "35", "36", "37", "38", "39", "40", "41", "42", "43", "44", "45", "46", "47", "48", "49", "50", "51", "52", "53", "54", "55", "56", "57", "58", "59", "60", "61", "62", "63", "64", "65", "66", "67", "68", "69", "70", "71", "72", "73", "74", "75", "76", "77", "78", "79", "80", "81", "82", "83", "84", "85", "86", "87", "88", "89", "90", "91", "156", "169", "178", "227", "229", "231", "233", "235", "236", "237", "238", "239"],
		"CDFG" : "partitionandreduce0",
		"Protocol" : "ap_ctrl_hs",
		"ControlExist" : "1", "ap_start" : "1", "ap_ready" : "1", "ap_done" : "1", "ap_continue" : "0", "ap_idle" : "1",
		"Pipeline" : "None", "UnalignedPipeline" : "0", "RewindPipeline" : "0", "ProcessNetwork" : "0",
		"II" : "0",
		"VariableLatency" : "1", "ExactLatency" : "-1", "EstimateLatencyMin" : "41", "EstimateLatencyMax" : "197844218",
		"Combinational" : "0",
		"Datapath" : "0",
		"ClockEnable" : "0",
		"HasSubDataflow" : "0",
		"InDataflowNetwork" : "0",
		"HasNonBlockingOperation" : "0",
		"WaitState" : [
			{"State" : "ap_ST_fsm_state75", "FSM" : "ap_CS_fsm", "SubInstance" : "grp_reducepartitions0_fu_1742"},
			{"State" : "ap_ST_fsm_state77", "FSM" : "ap_CS_fsm", "SubInstance" : "grp_reducepartitions0_fu_1742"},
			{"State" : "ap_ST_fsm_state52", "FSM" : "ap_CS_fsm", "SubInstance" : "grp_partitionkeyvalues00_fu_1785"},
			{"State" : "ap_ST_fsm_state55", "FSM" : "ap_CS_fsm", "SubInstance" : "grp_partitionkeyvalues00_fu_1785"},
			{"State" : "ap_ST_fsm_state54", "FSM" : "ap_CS_fsm", "SubInstance" : "grp_partitionkeyvalues00_fu_1785"},
			{"State" : "ap_ST_fsm_state57", "FSM" : "ap_CS_fsm", "SubInstance" : "grp_savepartitions0_fu_1813"},
			{"State" : "ap_ST_fsm_state36", "FSM" : "ap_CS_fsm", "SubInstance" : "grp_collectstats00_fu_1835"},
			{"State" : "ap_ST_fsm_state39", "FSM" : "ap_CS_fsm", "SubInstance" : "grp_collectstats00_fu_1835"},
			{"State" : "ap_ST_fsm_state38", "FSM" : "ap_CS_fsm", "SubInstance" : "grp_collectstats00_fu_1835"},
			{"State" : "ap_ST_fsm_state50", "FSM" : "ap_CS_fsm", "SubInstance" : "grp_readcapsules0_fu_1859"},
			{"State" : "ap_ST_fsm_state41", "FSM" : "ap_CS_fsm", "SubInstance" : "grp_savecapsules0_fu_1883"},
			{"State" : "ap_ST_fsm_state75", "FSM" : "ap_CS_fsm", "SubInstance" : "grp_readkeyvalues0_3_fu_1912"},
			{"State" : "ap_ST_fsm_state77", "FSM" : "ap_CS_fsm", "SubInstance" : "grp_readkeyvalues0_3_fu_1912"},
			{"State" : "ap_ST_fsm_state36", "FSM" : "ap_CS_fsm", "SubInstance" : "grp_readkeyvalues0_2_fu_1934"},
			{"State" : "ap_ST_fsm_state39", "FSM" : "ap_CS_fsm", "SubInstance" : "grp_readkeyvalues0_2_fu_1934"},
			{"State" : "ap_ST_fsm_state38", "FSM" : "ap_CS_fsm", "SubInstance" : "grp_readkeyvalues0_2_fu_1934"},
			{"State" : "ap_ST_fsm_state52", "FSM" : "ap_CS_fsm", "SubInstance" : "grp_readkeyvalues0_2_fu_1934"},
			{"State" : "ap_ST_fsm_state55", "FSM" : "ap_CS_fsm", "SubInstance" : "grp_readkeyvalues0_2_fu_1934"},
			{"State" : "ap_ST_fsm_state54", "FSM" : "ap_CS_fsm", "SubInstance" : "grp_readkeyvalues0_2_fu_1934"},
			{"State" : "ap_ST_fsm_state78", "FSM" : "ap_CS_fsm", "SubInstance" : "grp_savekeyvalues0_2_fu_1951"},
			{"State" : "ap_ST_fsm_state80", "FSM" : "ap_CS_fsm", "SubInstance" : "grp_savekeyvalues0_2_fu_1951"},
			{"State" : "ap_ST_fsm_state82", "FSM" : "ap_CS_fsm", "SubInstance" : "grp_savekeyvalues0_2_fu_1951"},
			{"State" : "ap_ST_fsm_state84", "FSM" : "ap_CS_fsm", "SubInstance" : "grp_savekeyvalues0_2_fu_1951"},
			{"State" : "ap_ST_fsm_state67", "FSM" : "ap_CS_fsm", "SubInstance" : "grp_readkeyvalues0_1_fu_1975"},
			{"State" : "ap_ST_fsm_state69", "FSM" : "ap_CS_fsm", "SubInstance" : "grp_readkeyvalues0_1_fu_1975"},
			{"State" : "ap_ST_fsm_state71", "FSM" : "ap_CS_fsm", "SubInstance" : "grp_readkeyvalues0_1_fu_1975"},
			{"State" : "ap_ST_fsm_state73", "FSM" : "ap_CS_fsm", "SubInstance" : "grp_readkeyvalues0_1_fu_1975"},
			{"State" : "ap_ST_fsm_state93", "FSM" : "ap_CS_fsm", "SubInstance" : "grp_savekeyvalues0_251_fu_2047"}],
		"Port" : [
			{"Name" : "kvsourcedram_V", "Type" : "MAXI", "Direction" : "IO",
				"SubConnect" : [
					{"ID" : "231", "SubInstance" : "grp_readkeyvalues0_3_fu_1912", "Port" : "kvdram_V"},
					{"ID" : "233", "SubInstance" : "grp_readkeyvalues0_2_fu_1934", "Port" : "kvdram_V"},
					{"ID" : "229", "SubInstance" : "grp_savecapsules0_fu_1883", "Port" : "kvdram_V"},
					{"ID" : "227", "SubInstance" : "grp_readcapsules0_fu_1859", "Port" : "kvdram_V"},
					{"ID" : "169", "SubInstance" : "grp_savepartitions0_fu_1813", "Port" : "kvdram_V"}]},
			{"Name" : "kvsourcedram_V_offset", "Type" : "None", "Direction" : "I"},
			{"Name" : "kvdestdram_V", "Type" : "MAXI", "Direction" : "IO",
				"SubConnect" : [
					{"ID" : "235", "SubInstance" : "grp_savekeyvalues0_2_fu_1951", "Port" : "kvdram_V"},
					{"ID" : "236", "SubInstance" : "grp_readkeyvalues0_1_fu_1975", "Port" : "kvdram_V"}]},
			{"Name" : "kvdestdram_V_offset", "Type" : "None", "Direction" : "I"},
			{"Name" : "kvstats", "Type" : "MAXI", "Direction" : "IO",
				"BlockSignal" : [
					{"Name" : "kvstats_blk_n_AW", "Type" : "RtlSignal"},
					{"Name" : "kvstats_blk_n_W", "Type" : "RtlSignal"},
					{"Name" : "kvstats_blk_n_B", "Type" : "RtlSignal"},
					{"Name" : "kvstats_blk_n_AR", "Type" : "RtlSignal"},
					{"Name" : "kvstats_blk_n_R", "Type" : "RtlSignal"}],
				"SubConnect" : [
					{"ID" : "238", "SubInstance" : "grp_savekeyvalues0_1_fu_2057", "Port" : "dram"},
					{"ID" : "237", "SubInstance" : "grp_savekeyvalues0_251_fu_2047", "Port" : "dram"}]},
			{"Name" : "kvstats_offset", "Type" : "None", "Direction" : "I"},
			{"Name" : "globalparams_runkernelcommand", "Type" : "None", "Direction" : "I"},
			{"Name" : "globalparams_partitioncommand", "Type" : "None", "Direction" : "I"},
			{"Name" : "globalparams_reducecommand", "Type" : "None", "Direction" : "I"},
			{"Name" : "globalparams_treedepth", "Type" : "None", "Direction" : "I"},
			{"Name" : "globalparams_runsize", "Type" : "None", "Direction" : "I"},
			{"Name" : "globalparams_vbegin", "Type" : "None", "Direction" : "I"},
			{"Name" : "globalparams_statsalreadycollected", "Type" : "None", "Direction" : "I"}],
		"SubInstanceBlock" : [
			{"SubInstance" : "grp_savekeyvalues0_1_fu_2057", "SubBlockPort" : ["dram_blk_n_AW", "dram_blk_n_W", "dram_blk_n_B"]}]},
	{"ID" : "1", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.sourcebuffer00_0_V_U", "Parent" : "0"},
	{"ID" : "2", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.sourcebuffer00_1_V_U", "Parent" : "0"},
	{"ID" : "3", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.destbuffer00_V_U", "Parent" : "0"},
	{"ID" : "4", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.capsule00_key_U", "Parent" : "0"},
	{"ID" : "5", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.capsule00_value_U", "Parent" : "0"},
	{"ID" : "6", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.BIGcapsule00_V_U", "Parent" : "0"},
	{"ID" : "7", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.sourcebuffer01_0_V_U", "Parent" : "0"},
	{"ID" : "8", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.sourcebuffer01_1_V_U", "Parent" : "0"},
	{"ID" : "9", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.destbuffer01_V_U", "Parent" : "0"},
	{"ID" : "10", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.capsule01_key_U", "Parent" : "0"},
	{"ID" : "11", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.capsule01_value_U", "Parent" : "0"},
	{"ID" : "12", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.BIGcapsule01_V_U", "Parent" : "0"},
	{"ID" : "13", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.sourcebuffer02_0_V_U", "Parent" : "0"},
	{"ID" : "14", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.sourcebuffer02_1_V_U", "Parent" : "0"},
	{"ID" : "15", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.destbuffer02_V_U", "Parent" : "0"},
	{"ID" : "16", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.capsule02_key_U", "Parent" : "0"},
	{"ID" : "17", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.capsule02_value_U", "Parent" : "0"},
	{"ID" : "18", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.BIGcapsule02_V_U", "Parent" : "0"},
	{"ID" : "19", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.sourcebuffer03_0_V_U", "Parent" : "0"},
	{"ID" : "20", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.sourcebuffer03_1_V_U", "Parent" : "0"},
	{"ID" : "21", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.destbuffer03_V_U", "Parent" : "0"},
	{"ID" : "22", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.capsule03_key_U", "Parent" : "0"},
	{"ID" : "23", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.capsule03_value_U", "Parent" : "0"},
	{"ID" : "24", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.BIGcapsule03_V_U", "Parent" : "0"},
	{"ID" : "25", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.destbuffer000_key_U", "Parent" : "0"},
	{"ID" : "26", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.destbuffer000_value_U", "Parent" : "0"},
	{"ID" : "27", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.destbuffer001_key_U", "Parent" : "0"},
	{"ID" : "28", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.destbuffer001_value_U", "Parent" : "0"},
	{"ID" : "29", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.destbuffer002_key_U", "Parent" : "0"},
	{"ID" : "30", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.destbuffer002_value_U", "Parent" : "0"},
	{"ID" : "31", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.destbuffer003_key_U", "Parent" : "0"},
	{"ID" : "32", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.destbuffer003_value_U", "Parent" : "0"},
	{"ID" : "33", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.destbuffer004_key_U", "Parent" : "0"},
	{"ID" : "34", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.destbuffer004_value_U", "Parent" : "0"},
	{"ID" : "35", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.destbuffer005_key_U", "Parent" : "0"},
	{"ID" : "36", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.destbuffer005_value_U", "Parent" : "0"},
	{"ID" : "37", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.destbuffer006_key_U", "Parent" : "0"},
	{"ID" : "38", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.destbuffer006_value_U", "Parent" : "0"},
	{"ID" : "39", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.destbuffer007_key_U", "Parent" : "0"},
	{"ID" : "40", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.destbuffer007_value_U", "Parent" : "0"},
	{"ID" : "41", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.destbuffer010_key_U", "Parent" : "0"},
	{"ID" : "42", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.destbuffer010_value_U", "Parent" : "0"},
	{"ID" : "43", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.destbuffer011_key_U", "Parent" : "0"},
	{"ID" : "44", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.destbuffer011_value_U", "Parent" : "0"},
	{"ID" : "45", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.destbuffer012_key_U", "Parent" : "0"},
	{"ID" : "46", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.destbuffer012_value_U", "Parent" : "0"},
	{"ID" : "47", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.destbuffer013_key_U", "Parent" : "0"},
	{"ID" : "48", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.destbuffer013_value_U", "Parent" : "0"},
	{"ID" : "49", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.destbuffer014_key_U", "Parent" : "0"},
	{"ID" : "50", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.destbuffer014_value_U", "Parent" : "0"},
	{"ID" : "51", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.destbuffer015_key_U", "Parent" : "0"},
	{"ID" : "52", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.destbuffer015_value_U", "Parent" : "0"},
	{"ID" : "53", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.destbuffer016_key_U", "Parent" : "0"},
	{"ID" : "54", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.destbuffer016_value_U", "Parent" : "0"},
	{"ID" : "55", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.destbuffer017_key_U", "Parent" : "0"},
	{"ID" : "56", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.destbuffer017_value_U", "Parent" : "0"},
	{"ID" : "57", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.destbuffer020_key_U", "Parent" : "0"},
	{"ID" : "58", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.destbuffer020_value_U", "Parent" : "0"},
	{"ID" : "59", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.destbuffer021_key_U", "Parent" : "0"},
	{"ID" : "60", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.destbuffer021_value_U", "Parent" : "0"},
	{"ID" : "61", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.destbuffer022_key_U", "Parent" : "0"},
	{"ID" : "62", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.destbuffer022_value_U", "Parent" : "0"},
	{"ID" : "63", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.destbuffer023_key_U", "Parent" : "0"},
	{"ID" : "64", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.destbuffer023_value_U", "Parent" : "0"},
	{"ID" : "65", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.destbuffer024_key_U", "Parent" : "0"},
	{"ID" : "66", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.destbuffer024_value_U", "Parent" : "0"},
	{"ID" : "67", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.destbuffer025_key_U", "Parent" : "0"},
	{"ID" : "68", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.destbuffer025_value_U", "Parent" : "0"},
	{"ID" : "69", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.destbuffer026_key_U", "Parent" : "0"},
	{"ID" : "70", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.destbuffer026_value_U", "Parent" : "0"},
	{"ID" : "71", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.destbuffer027_key_U", "Parent" : "0"},
	{"ID" : "72", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.destbuffer027_value_U", "Parent" : "0"},
	{"ID" : "73", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.destbuffer030_key_U", "Parent" : "0"},
	{"ID" : "74", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.destbuffer030_value_U", "Parent" : "0"},
	{"ID" : "75", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.destbuffer031_key_U", "Parent" : "0"},
	{"ID" : "76", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.destbuffer031_value_U", "Parent" : "0"},
	{"ID" : "77", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.destbuffer032_key_U", "Parent" : "0"},
	{"ID" : "78", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.destbuffer032_value_U", "Parent" : "0"},
	{"ID" : "79", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.destbuffer033_key_U", "Parent" : "0"},
	{"ID" : "80", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.destbuffer033_value_U", "Parent" : "0"},
	{"ID" : "81", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.destbuffer034_key_U", "Parent" : "0"},
	{"ID" : "82", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.destbuffer034_value_U", "Parent" : "0"},
	{"ID" : "83", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.destbuffer035_key_U", "Parent" : "0"},
	{"ID" : "84", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.destbuffer035_value_U", "Parent" : "0"},
	{"ID" : "85", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.destbuffer036_key_U", "Parent" : "0"},
	{"ID" : "86", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.destbuffer036_value_U", "Parent" : "0"},
	{"ID" : "87", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.destbuffer037_key_U", "Parent" : "0"},
	{"ID" : "88", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.destbuffer037_value_U", "Parent" : "0"},
	{"ID" : "89", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.kvdeststats_tmp_key_U", "Parent" : "0"},
	{"ID" : "90", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.kvdeststats_tmp_valu_U", "Parent" : "0"},
	{"ID" : "91", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1742", "Parent" : "0", "Child" : ["92", "93", "94", "95", "96", "97", "98", "99", "100", "101", "102", "103", "104", "105", "106", "107", "108", "109", "110", "111", "112", "113", "114", "115", "116", "117", "118", "119", "120", "121", "122", "123", "124", "125", "126", "127", "128", "129", "130", "131", "132", "133", "134", "135", "136", "137", "138", "139", "140", "141", "142", "143", "144", "145", "146", "147", "148", "149", "150", "151", "152", "153", "154", "155"],
		"CDFG" : "reducepartitions0",
		"Protocol" : "ap_ctrl_hs",
		"ControlExist" : "1", "ap_start" : "1", "ap_ready" : "1", "ap_done" : "1", "ap_continue" : "0", "ap_idle" : "1",
		"Pipeline" : "None", "UnalignedPipeline" : "0", "RewindPipeline" : "0", "ProcessNetwork" : "0",
		"II" : "0",
		"VariableLatency" : "1", "ExactLatency" : "-1", "EstimateLatencyMin" : "1", "EstimateLatencyMax" : "929",
		"Combinational" : "0",
		"Datapath" : "0",
		"ClockEnable" : "0",
		"HasSubDataflow" : "0",
		"InDataflowNetwork" : "0",
		"HasNonBlockingOperation" : "0",
		"Port" : [
			{"Name" : "enable", "Type" : "None", "Direction" : "I"},
			{"Name" : "sourcebuffer0_V", "Type" : "Memory", "Direction" : "I"},
			{"Name" : "sourcebuffer1_V", "Type" : "Memory", "Direction" : "I"},
			{"Name" : "sourcebuffer2_V", "Type" : "Memory", "Direction" : "I"},
			{"Name" : "sourcebuffer3_V", "Type" : "Memory", "Direction" : "I"},
			{"Name" : "destbuffer00_value", "Type" : "Memory", "Direction" : "IO"},
			{"Name" : "destbuffer01_value", "Type" : "Memory", "Direction" : "IO"},
			{"Name" : "destbuffer02_value", "Type" : "Memory", "Direction" : "IO"},
			{"Name" : "destbuffer03_value", "Type" : "Memory", "Direction" : "IO"},
			{"Name" : "destbuffer04_value", "Type" : "Memory", "Direction" : "IO"},
			{"Name" : "destbuffer05_value", "Type" : "Memory", "Direction" : "IO"},
			{"Name" : "destbuffer06_value", "Type" : "Memory", "Direction" : "IO"},
			{"Name" : "destbuffer07_value", "Type" : "Memory", "Direction" : "IO"},
			{"Name" : "destbuffer10_value", "Type" : "Memory", "Direction" : "IO"},
			{"Name" : "destbuffer11_value", "Type" : "Memory", "Direction" : "IO"},
			{"Name" : "destbuffer12_value", "Type" : "Memory", "Direction" : "IO"},
			{"Name" : "destbuffer13_value", "Type" : "Memory", "Direction" : "IO"},
			{"Name" : "destbuffer14_value", "Type" : "Memory", "Direction" : "IO"},
			{"Name" : "destbuffer15_value", "Type" : "Memory", "Direction" : "IO"},
			{"Name" : "destbuffer16_value", "Type" : "Memory", "Direction" : "IO"},
			{"Name" : "destbuffer17_value", "Type" : "Memory", "Direction" : "IO"},
			{"Name" : "destbuffer20_value", "Type" : "Memory", "Direction" : "IO"},
			{"Name" : "destbuffer21_value", "Type" : "Memory", "Direction" : "IO"},
			{"Name" : "destbuffer22_value", "Type" : "Memory", "Direction" : "IO"},
			{"Name" : "destbuffer23_value", "Type" : "Memory", "Direction" : "IO"},
			{"Name" : "destbuffer24_value", "Type" : "Memory", "Direction" : "IO"},
			{"Name" : "destbuffer25_value", "Type" : "Memory", "Direction" : "IO"},
			{"Name" : "destbuffer26_value", "Type" : "Memory", "Direction" : "IO"},
			{"Name" : "destbuffer27_value", "Type" : "Memory", "Direction" : "IO"},
			{"Name" : "destbuffer30_value", "Type" : "Memory", "Direction" : "IO"},
			{"Name" : "destbuffer31_value", "Type" : "Memory", "Direction" : "IO"},
			{"Name" : "destbuffer32_value", "Type" : "Memory", "Direction" : "IO"},
			{"Name" : "destbuffer33_value", "Type" : "Memory", "Direction" : "IO"},
			{"Name" : "destbuffer34_value", "Type" : "Memory", "Direction" : "IO"},
			{"Name" : "destbuffer35_value", "Type" : "Memory", "Direction" : "IO"},
			{"Name" : "destbuffer36_value", "Type" : "Memory", "Direction" : "IO"},
			{"Name" : "destbuffer37_value", "Type" : "Memory", "Direction" : "IO"},
			{"Name" : "globalparams_vbegin", "Type" : "None", "Direction" : "I"}]},
	{"ID" : "92", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1742.topkernel_mux_83_32_1_1_U228", "Parent" : "91"},
	{"ID" : "93", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1742.topkernel_mux_83_32_1_1_U229", "Parent" : "91"},
	{"ID" : "94", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1742.topkernel_mux_83_32_1_1_U230", "Parent" : "91"},
	{"ID" : "95", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1742.topkernel_mux_83_32_1_1_U231", "Parent" : "91"},
	{"ID" : "96", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1742.topkernel_mux_83_32_1_1_U232", "Parent" : "91"},
	{"ID" : "97", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1742.topkernel_mux_83_32_1_1_U233", "Parent" : "91"},
	{"ID" : "98", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1742.topkernel_mux_83_32_1_1_U234", "Parent" : "91"},
	{"ID" : "99", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1742.topkernel_mux_83_32_1_1_U235", "Parent" : "91"},
	{"ID" : "100", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1742.topkernel_mux_83_32_1_1_U236", "Parent" : "91"},
	{"ID" : "101", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1742.topkernel_mux_83_32_1_1_U237", "Parent" : "91"},
	{"ID" : "102", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1742.topkernel_mux_83_32_1_1_U238", "Parent" : "91"},
	{"ID" : "103", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1742.topkernel_mux_83_32_1_1_U239", "Parent" : "91"},
	{"ID" : "104", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1742.topkernel_mux_83_32_1_1_U240", "Parent" : "91"},
	{"ID" : "105", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1742.topkernel_mux_83_32_1_1_U241", "Parent" : "91"},
	{"ID" : "106", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1742.topkernel_mux_83_32_1_1_U242", "Parent" : "91"},
	{"ID" : "107", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1742.topkernel_mux_83_32_1_1_U243", "Parent" : "91"},
	{"ID" : "108", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1742.topkernel_mux_83_32_1_1_U244", "Parent" : "91"},
	{"ID" : "109", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1742.topkernel_mux_83_32_1_1_U245", "Parent" : "91"},
	{"ID" : "110", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1742.topkernel_mux_83_32_1_1_U246", "Parent" : "91"},
	{"ID" : "111", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1742.topkernel_mux_83_32_1_1_U247", "Parent" : "91"},
	{"ID" : "112", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1742.topkernel_mux_83_32_1_1_U248", "Parent" : "91"},
	{"ID" : "113", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1742.topkernel_mux_83_32_1_1_U249", "Parent" : "91"},
	{"ID" : "114", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1742.topkernel_mux_83_32_1_1_U250", "Parent" : "91"},
	{"ID" : "115", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1742.topkernel_mux_83_32_1_1_U251", "Parent" : "91"},
	{"ID" : "116", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1742.topkernel_mux_83_32_1_1_U252", "Parent" : "91"},
	{"ID" : "117", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1742.topkernel_mux_83_32_1_1_U253", "Parent" : "91"},
	{"ID" : "118", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1742.topkernel_mux_83_32_1_1_U254", "Parent" : "91"},
	{"ID" : "119", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1742.topkernel_mux_83_32_1_1_U255", "Parent" : "91"},
	{"ID" : "120", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1742.topkernel_mux_83_32_1_1_U256", "Parent" : "91"},
	{"ID" : "121", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1742.topkernel_mux_83_32_1_1_U257", "Parent" : "91"},
	{"ID" : "122", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1742.topkernel_mux_83_32_1_1_U258", "Parent" : "91"},
	{"ID" : "123", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1742.topkernel_mux_83_32_1_1_U259", "Parent" : "91"},
	{"ID" : "124", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1742.topkernel_mux_83_32_1_1_U260", "Parent" : "91"},
	{"ID" : "125", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1742.topkernel_mux_83_32_1_1_U261", "Parent" : "91"},
	{"ID" : "126", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1742.topkernel_mux_83_32_1_1_U262", "Parent" : "91"},
	{"ID" : "127", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1742.topkernel_mux_83_32_1_1_U263", "Parent" : "91"},
	{"ID" : "128", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1742.topkernel_mux_83_32_1_1_U264", "Parent" : "91"},
	{"ID" : "129", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1742.topkernel_mux_83_32_1_1_U265", "Parent" : "91"},
	{"ID" : "130", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1742.topkernel_mux_83_32_1_1_U266", "Parent" : "91"},
	{"ID" : "131", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1742.topkernel_mux_83_32_1_1_U267", "Parent" : "91"},
	{"ID" : "132", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1742.topkernel_mux_83_32_1_1_U268", "Parent" : "91"},
	{"ID" : "133", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1742.topkernel_mux_83_32_1_1_U269", "Parent" : "91"},
	{"ID" : "134", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1742.topkernel_mux_83_32_1_1_U270", "Parent" : "91"},
	{"ID" : "135", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1742.topkernel_mux_83_32_1_1_U271", "Parent" : "91"},
	{"ID" : "136", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1742.topkernel_mux_83_32_1_1_U272", "Parent" : "91"},
	{"ID" : "137", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1742.topkernel_mux_83_32_1_1_U273", "Parent" : "91"},
	{"ID" : "138", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1742.topkernel_mux_83_32_1_1_U274", "Parent" : "91"},
	{"ID" : "139", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1742.topkernel_mux_83_32_1_1_U275", "Parent" : "91"},
	{"ID" : "140", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1742.topkernel_mux_83_32_1_1_U276", "Parent" : "91"},
	{"ID" : "141", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1742.topkernel_mux_83_32_1_1_U277", "Parent" : "91"},
	{"ID" : "142", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1742.topkernel_mux_83_32_1_1_U278", "Parent" : "91"},
	{"ID" : "143", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1742.topkernel_mux_83_32_1_1_U279", "Parent" : "91"},
	{"ID" : "144", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1742.topkernel_mux_83_32_1_1_U280", "Parent" : "91"},
	{"ID" : "145", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1742.topkernel_mux_83_32_1_1_U281", "Parent" : "91"},
	{"ID" : "146", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1742.topkernel_mux_83_32_1_1_U282", "Parent" : "91"},
	{"ID" : "147", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1742.topkernel_mux_83_32_1_1_U283", "Parent" : "91"},
	{"ID" : "148", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1742.topkernel_mux_83_32_1_1_U284", "Parent" : "91"},
	{"ID" : "149", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1742.topkernel_mux_83_32_1_1_U285", "Parent" : "91"},
	{"ID" : "150", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1742.topkernel_mux_83_32_1_1_U286", "Parent" : "91"},
	{"ID" : "151", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1742.topkernel_mux_83_32_1_1_U287", "Parent" : "91"},
	{"ID" : "152", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1742.topkernel_mux_83_32_1_1_U288", "Parent" : "91"},
	{"ID" : "153", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1742.topkernel_mux_83_32_1_1_U289", "Parent" : "91"},
	{"ID" : "154", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1742.topkernel_mux_83_32_1_1_U290", "Parent" : "91"},
	{"ID" : "155", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_reducepartitions0_fu_1742.topkernel_mux_83_32_1_1_U291", "Parent" : "91"},
	{"ID" : "156", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.grp_partitionkeyvalues00_fu_1785", "Parent" : "0", "Child" : ["157", "158", "159", "160", "161", "162", "163", "164", "165", "166", "167", "168"],
		"CDFG" : "partitionkeyvalues00",
		"Protocol" : "ap_ctrl_hs",
		"ControlExist" : "1", "ap_start" : "1", "ap_ready" : "1", "ap_done" : "1", "ap_continue" : "0", "ap_idle" : "1",
		"Pipeline" : "None", "UnalignedPipeline" : "0", "RewindPipeline" : "0", "ProcessNetwork" : "0",
		"II" : "0",
		"VariableLatency" : "1", "ExactLatency" : "-1", "EstimateLatencyMin" : "1", "EstimateLatencyMax" : "4228",
		"Combinational" : "0",
		"Datapath" : "0",
		"ClockEnable" : "0",
		"HasSubDataflow" : "0",
		"InDataflowNetwork" : "0",
		"HasNonBlockingOperation" : "0",
		"Port" : [
			{"Name" : "enable", "Type" : "None", "Direction" : "I"},
			{"Name" : "sourcebuffer0_V", "Type" : "Memory", "Direction" : "I"},
			{"Name" : "sourcebuffer1_V", "Type" : "Memory", "Direction" : "I"},
			{"Name" : "sourcebuffer2_V", "Type" : "Memory", "Direction" : "I"},
			{"Name" : "sourcebuffer3_V", "Type" : "Memory", "Direction" : "I"},
			{"Name" : "destbuffer0_V", "Type" : "Memory", "Direction" : "O"},
			{"Name" : "destbuffer1_V", "Type" : "Memory", "Direction" : "O"},
			{"Name" : "destbuffer2_V", "Type" : "Memory", "Direction" : "O"},
			{"Name" : "destbuffer3_V", "Type" : "Memory", "Direction" : "O"},
			{"Name" : "capsule0_key", "Type" : "Memory", "Direction" : "I"},
			{"Name" : "capsule0_value", "Type" : "Memory", "Direction" : "IO"},
			{"Name" : "capsule1_key", "Type" : "Memory", "Direction" : "I"},
			{"Name" : "capsule1_value", "Type" : "Memory", "Direction" : "IO"},
			{"Name" : "capsule2_key", "Type" : "Memory", "Direction" : "I"},
			{"Name" : "capsule2_value", "Type" : "Memory", "Direction" : "IO"},
			{"Name" : "capsule3_key", "Type" : "Memory", "Direction" : "I"},
			{"Name" : "capsule3_value", "Type" : "Memory", "Direction" : "IO"},
			{"Name" : "travstate_i_kvs", "Type" : "None", "Direction" : "I"},
			{"Name" : "travstate_end_kvs", "Type" : "None", "Direction" : "I"},
			{"Name" : "llopparams_currentLOP", "Type" : "None", "Direction" : "I"},
			{"Name" : "llopparams_upperlimit", "Type" : "None", "Direction" : "I"}]},
	{"ID" : "157", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_partitionkeyvalues00_fu_1785.topkernel_mux_325_32_1_1_U85", "Parent" : "156"},
	{"ID" : "158", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_partitionkeyvalues00_fu_1785.topkernel_mux_325_32_1_1_U86", "Parent" : "156"},
	{"ID" : "159", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_partitionkeyvalues00_fu_1785.topkernel_mux_325_32_1_1_U87", "Parent" : "156"},
	{"ID" : "160", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_partitionkeyvalues00_fu_1785.topkernel_mux_325_32_1_1_U88", "Parent" : "156"},
	{"ID" : "161", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_partitionkeyvalues00_fu_1785.topkernel_mux_325_32_1_1_U89", "Parent" : "156"},
	{"ID" : "162", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_partitionkeyvalues00_fu_1785.topkernel_mux_325_32_1_1_U90", "Parent" : "156"},
	{"ID" : "163", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_partitionkeyvalues00_fu_1785.topkernel_mux_325_32_1_1_U91", "Parent" : "156"},
	{"ID" : "164", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_partitionkeyvalues00_fu_1785.topkernel_mux_325_32_1_1_U92", "Parent" : "156"},
	{"ID" : "165", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_partitionkeyvalues00_fu_1785.topkernel_mux_325_32_1_1_U93", "Parent" : "156"},
	{"ID" : "166", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_partitionkeyvalues00_fu_1785.topkernel_mux_325_32_1_1_U94", "Parent" : "156"},
	{"ID" : "167", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_partitionkeyvalues00_fu_1785.topkernel_mux_325_32_1_1_U95", "Parent" : "156"},
	{"ID" : "168", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_partitionkeyvalues00_fu_1785.topkernel_mux_325_32_1_1_U96", "Parent" : "156"},
	{"ID" : "169", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.grp_savepartitions0_fu_1813", "Parent" : "0", "Child" : ["170", "171", "172", "173", "174", "175", "176", "177"],
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
	{"ID" : "170", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_savepartitions0_fu_1813.topkernel_urem_12ns_8ns_12_16_1_U119", "Parent" : "169"},
	{"ID" : "171", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_savepartitions0_fu_1813.topkernel_urem_12ns_8ns_12_16_1_U120", "Parent" : "169"},
	{"ID" : "172", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_savepartitions0_fu_1813.topkernel_urem_12ns_8ns_12_16_1_U121", "Parent" : "169"},
	{"ID" : "173", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_savepartitions0_fu_1813.topkernel_urem_12ns_8ns_12_16_1_U122", "Parent" : "169"},
	{"ID" : "174", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_savepartitions0_fu_1813.topkernel_urem_12ns_8ns_12_16_1_U123", "Parent" : "169"},
	{"ID" : "175", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_savepartitions0_fu_1813.topkernel_urem_12ns_8ns_12_16_1_U124", "Parent" : "169"},
	{"ID" : "176", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_savepartitions0_fu_1813.topkernel_urem_12ns_8ns_12_16_1_U125", "Parent" : "169"},
	{"ID" : "177", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_savepartitions0_fu_1813.topkernel_urem_12ns_8ns_12_16_1_U126", "Parent" : "169"},
	{"ID" : "178", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.grp_collectstats00_fu_1835", "Parent" : "0", "Child" : ["179", "180", "181", "182", "183", "184", "185", "186", "187", "188", "189", "190", "191", "192", "193", "194", "195", "196", "197", "198", "199", "200", "201", "202", "203", "204", "205", "206", "207", "208", "209", "210", "211", "212", "213", "214", "215", "216", "217", "218", "219", "220", "221", "222", "223", "224", "225", "226"],
		"CDFG" : "collectstats00",
		"Protocol" : "ap_ctrl_hs",
		"ControlExist" : "1", "ap_start" : "1", "ap_ready" : "1", "ap_done" : "1", "ap_continue" : "0", "ap_idle" : "1",
		"Pipeline" : "None", "UnalignedPipeline" : "0", "RewindPipeline" : "0", "ProcessNetwork" : "0",
		"II" : "0",
		"VariableLatency" : "1", "ExactLatency" : "-1", "EstimateLatencyMin" : "1", "EstimateLatencyMax" : "1126",
		"Combinational" : "0",
		"Datapath" : "0",
		"ClockEnable" : "0",
		"HasSubDataflow" : "0",
		"InDataflowNetwork" : "0",
		"HasNonBlockingOperation" : "0",
		"Port" : [
			{"Name" : "enable", "Type" : "None", "Direction" : "I"},
			{"Name" : "sourcebuffer0_V", "Type" : "Memory", "Direction" : "I"},
			{"Name" : "sourcebuffer1_V", "Type" : "Memory", "Direction" : "I"},
			{"Name" : "sourcebuffer2_V", "Type" : "Memory", "Direction" : "I"},
			{"Name" : "sourcebuffer3_V", "Type" : "Memory", "Direction" : "I"},
			{"Name" : "capsule0_key", "Type" : "Memory", "Direction" : "O"},
			{"Name" : "capsule0_value", "Type" : "Memory", "Direction" : "IO"},
			{"Name" : "capsule1_key", "Type" : "Memory", "Direction" : "O"},
			{"Name" : "capsule1_value", "Type" : "Memory", "Direction" : "IO"},
			{"Name" : "capsule2_key", "Type" : "Memory", "Direction" : "O"},
			{"Name" : "capsule2_value", "Type" : "Memory", "Direction" : "IO"},
			{"Name" : "capsule3_key", "Type" : "Memory", "Direction" : "O"},
			{"Name" : "capsule3_value", "Type" : "Memory", "Direction" : "IO"},
			{"Name" : "llopparams_currentLOP", "Type" : "None", "Direction" : "I"},
			{"Name" : "llopparams_upperlimit", "Type" : "None", "Direction" : "I"},
			{"Name" : "travstate_i_kvs", "Type" : "None", "Direction" : "I"},
			{"Name" : "travstate_end_kvs", "Type" : "None", "Direction" : "I"}]},
	{"ID" : "179", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_collectstats00_fu_1835.sizes00_U", "Parent" : "178"},
	{"ID" : "180", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_collectstats00_fu_1835.sizes01_U", "Parent" : "178"},
	{"ID" : "181", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_collectstats00_fu_1835.sizes02_U", "Parent" : "178"},
	{"ID" : "182", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_collectstats00_fu_1835.sizes03_U", "Parent" : "178"},
	{"ID" : "183", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_collectstats00_fu_1835.sizes04_U", "Parent" : "178"},
	{"ID" : "184", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_collectstats00_fu_1835.sizes05_U", "Parent" : "178"},
	{"ID" : "185", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_collectstats00_fu_1835.sizes06_U", "Parent" : "178"},
	{"ID" : "186", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_collectstats00_fu_1835.sizes07_U", "Parent" : "178"},
	{"ID" : "187", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_collectstats00_fu_1835.sizes10_U", "Parent" : "178"},
	{"ID" : "188", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_collectstats00_fu_1835.sizes11_U", "Parent" : "178"},
	{"ID" : "189", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_collectstats00_fu_1835.sizes12_U", "Parent" : "178"},
	{"ID" : "190", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_collectstats00_fu_1835.sizes13_U", "Parent" : "178"},
	{"ID" : "191", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_collectstats00_fu_1835.sizes14_U", "Parent" : "178"},
	{"ID" : "192", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_collectstats00_fu_1835.sizes15_U", "Parent" : "178"},
	{"ID" : "193", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_collectstats00_fu_1835.sizes16_U", "Parent" : "178"},
	{"ID" : "194", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_collectstats00_fu_1835.sizes17_U", "Parent" : "178"},
	{"ID" : "195", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_collectstats00_fu_1835.sizes20_U", "Parent" : "178"},
	{"ID" : "196", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_collectstats00_fu_1835.sizes21_U", "Parent" : "178"},
	{"ID" : "197", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_collectstats00_fu_1835.sizes22_U", "Parent" : "178"},
	{"ID" : "198", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_collectstats00_fu_1835.sizes23_U", "Parent" : "178"},
	{"ID" : "199", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_collectstats00_fu_1835.sizes24_U", "Parent" : "178"},
	{"ID" : "200", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_collectstats00_fu_1835.sizes25_U", "Parent" : "178"},
	{"ID" : "201", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_collectstats00_fu_1835.sizes26_U", "Parent" : "178"},
	{"ID" : "202", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_collectstats00_fu_1835.sizes27_U", "Parent" : "178"},
	{"ID" : "203", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_collectstats00_fu_1835.sizes30_U", "Parent" : "178"},
	{"ID" : "204", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_collectstats00_fu_1835.sizes31_U", "Parent" : "178"},
	{"ID" : "205", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_collectstats00_fu_1835.sizes32_U", "Parent" : "178"},
	{"ID" : "206", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_collectstats00_fu_1835.sizes33_U", "Parent" : "178"},
	{"ID" : "207", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_collectstats00_fu_1835.sizes34_U", "Parent" : "178"},
	{"ID" : "208", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_collectstats00_fu_1835.sizes35_U", "Parent" : "178"},
	{"ID" : "209", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_collectstats00_fu_1835.sizes36_U", "Parent" : "178"},
	{"ID" : "210", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_collectstats00_fu_1835.sizes37_U", "Parent" : "178"},
	{"ID" : "211", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_collectstats00_fu_1835.grp_getpartition_fu_1471", "Parent" : "178",
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
	{"ID" : "212", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_collectstats00_fu_1835.grp_getpartition_fu_1478", "Parent" : "178",
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
	{"ID" : "213", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_collectstats00_fu_1835.grp_getpartition_fu_1485", "Parent" : "178",
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
	{"ID" : "214", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_collectstats00_fu_1835.grp_getpartition_fu_1492", "Parent" : "178",
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
	{"ID" : "215", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_collectstats00_fu_1835.grp_getpartition_fu_1499", "Parent" : "178",
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
	{"ID" : "216", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_collectstats00_fu_1835.grp_getpartition_fu_1506", "Parent" : "178",
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
	{"ID" : "217", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_collectstats00_fu_1835.grp_getpartition_fu_1513", "Parent" : "178",
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
	{"ID" : "218", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_collectstats00_fu_1835.grp_getpartition_fu_1520", "Parent" : "178",
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
	{"ID" : "219", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_collectstats00_fu_1835.grp_getpartition_fu_1527", "Parent" : "178",
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
	{"ID" : "220", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_collectstats00_fu_1835.grp_getpartition_fu_1534", "Parent" : "178",
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
	{"ID" : "221", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_collectstats00_fu_1835.grp_getpartition_fu_1541", "Parent" : "178",
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
	{"ID" : "222", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_collectstats00_fu_1835.grp_getpartition_fu_1548", "Parent" : "178",
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
	{"ID" : "223", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_collectstats00_fu_1835.grp_getpartition_fu_1555", "Parent" : "178",
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
	{"ID" : "224", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_collectstats00_fu_1835.grp_getpartition_fu_1562", "Parent" : "178",
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
	{"ID" : "225", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_collectstats00_fu_1835.grp_getpartition_fu_1569", "Parent" : "178",
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
	{"ID" : "226", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_collectstats00_fu_1835.grp_getpartition_fu_1576", "Parent" : "178",
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
	{"ID" : "227", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.grp_readcapsules0_fu_1859", "Parent" : "0", "Child" : ["228"],
		"CDFG" : "readcapsules0",
		"Protocol" : "ap_ctrl_hs",
		"ControlExist" : "1", "ap_start" : "1", "ap_ready" : "1", "ap_done" : "1", "ap_continue" : "0", "ap_idle" : "1",
		"Pipeline" : "None", "UnalignedPipeline" : "0", "RewindPipeline" : "0", "ProcessNetwork" : "0",
		"II" : "0",
		"VariableLatency" : "1", "ExactLatency" : "-1", "EstimateLatencyMin" : "35", "EstimateLatencyMax" : "2129",
		"Combinational" : "0",
		"Datapath" : "0",
		"ClockEnable" : "0",
		"HasSubDataflow" : "0",
		"InDataflowNetwork" : "0",
		"HasNonBlockingOperation" : "0",
		"WaitState" : [
			{"State" : "ap_ST_fsm_state2", "FSM" : "ap_CS_fsm", "SubInstance" : "grp_readkeyvalues0_fu_418"},
			{"State" : "ap_ST_fsm_state4", "FSM" : "ap_CS_fsm", "SubInstance" : "grp_readkeyvalues0_fu_418"},
			{"State" : "ap_ST_fsm_state6", "FSM" : "ap_CS_fsm", "SubInstance" : "grp_readkeyvalues0_fu_418"},
			{"State" : "ap_ST_fsm_state8", "FSM" : "ap_CS_fsm", "SubInstance" : "grp_readkeyvalues0_fu_418"}],
		"Port" : [
			{"Name" : "kvdram_V", "Type" : "MAXI", "Direction" : "I",
				"SubConnect" : [
					{"ID" : "228", "SubInstance" : "grp_readkeyvalues0_fu_418", "Port" : "kvdram_V"}]},
			{"Name" : "kvdram_V_offset", "Type" : "None", "Direction" : "I"},
			{"Name" : "BIGcapsule0_V", "Type" : "Memory", "Direction" : "IO",
				"SubConnect" : [
					{"ID" : "228", "SubInstance" : "grp_readkeyvalues0_fu_418", "Port" : "buffer_V"}]},
			{"Name" : "BIGcapsule1_V", "Type" : "Memory", "Direction" : "IO",
				"SubConnect" : [
					{"ID" : "228", "SubInstance" : "grp_readkeyvalues0_fu_418", "Port" : "buffer_V"}]},
			{"Name" : "BIGcapsule2_V", "Type" : "Memory", "Direction" : "IO",
				"SubConnect" : [
					{"ID" : "228", "SubInstance" : "grp_readkeyvalues0_fu_418", "Port" : "buffer_V"}]},
			{"Name" : "BIGcapsule3_V", "Type" : "Memory", "Direction" : "IO",
				"SubConnect" : [
					{"ID" : "228", "SubInstance" : "grp_readkeyvalues0_fu_418", "Port" : "buffer_V"}]},
			{"Name" : "capsule0_key", "Type" : "Memory", "Direction" : "O"},
			{"Name" : "capsule0_value", "Type" : "Memory", "Direction" : "O"},
			{"Name" : "capsule1_key", "Type" : "Memory", "Direction" : "O"},
			{"Name" : "capsule1_value", "Type" : "Memory", "Direction" : "O"},
			{"Name" : "capsule2_key", "Type" : "Memory", "Direction" : "O"},
			{"Name" : "capsule2_value", "Type" : "Memory", "Direction" : "O"},
			{"Name" : "capsule3_key", "Type" : "Memory", "Direction" : "O"},
			{"Name" : "capsule3_value", "Type" : "Memory", "Direction" : "O"},
			{"Name" : "offset", "Type" : "None", "Direction" : "I"},
			{"Name" : "capsulemetadata_globaloffset", "Type" : "None", "Direction" : "I"},
			{"Name" : "capsulemetadata_localoffset", "Type" : "None", "Direction" : "I"}]},
	{"ID" : "228", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_readcapsules0_fu_1859.grp_readkeyvalues0_fu_418", "Parent" : "227",
		"CDFG" : "readkeyvalues0",
		"Protocol" : "ap_ctrl_hs",
		"ControlExist" : "1", "ap_start" : "1", "ap_ready" : "1", "ap_done" : "1", "ap_continue" : "0", "ap_idle" : "1",
		"Pipeline" : "None", "UnalignedPipeline" : "0", "RewindPipeline" : "0", "ProcessNetwork" : "0",
		"II" : "0",
		"VariableLatency" : "1", "ExactLatency" : "-1", "EstimateLatencyMin" : "522", "EstimateLatencyMax" : "522",
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
			{"Name" : "offset_kvs", "Type" : "None", "Direction" : "I"}]},
	{"ID" : "229", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.grp_savecapsules0_fu_1883", "Parent" : "0", "Child" : ["230"],
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
					{"ID" : "230", "SubInstance" : "grp_savekeyvalues0_fu_887", "Port" : "kvdram_V"}]},
			{"Name" : "kvdram_V_offset", "Type" : "None", "Direction" : "I"},
			{"Name" : "BIGcapsule0_V", "Type" : "Memory", "Direction" : "IO",
				"SubConnect" : [
					{"ID" : "230", "SubInstance" : "grp_savekeyvalues0_fu_887", "Port" : "buffer_V"}]},
			{"Name" : "BIGcapsule1_V", "Type" : "Memory", "Direction" : "IO",
				"SubConnect" : [
					{"ID" : "230", "SubInstance" : "grp_savekeyvalues0_fu_887", "Port" : "buffer_V"}]},
			{"Name" : "BIGcapsule2_V", "Type" : "Memory", "Direction" : "IO",
				"SubConnect" : [
					{"ID" : "230", "SubInstance" : "grp_savekeyvalues0_fu_887", "Port" : "buffer_V"}]},
			{"Name" : "BIGcapsule3_V", "Type" : "Memory", "Direction" : "IO",
				"SubConnect" : [
					{"ID" : "230", "SubInstance" : "grp_savekeyvalues0_fu_887", "Port" : "buffer_V"}]},
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
	{"ID" : "230", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_savecapsules0_fu_1883.grp_savekeyvalues0_fu_887", "Parent" : "229",
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
			{"Name" : "offset_kvs", "Type" : "None", "Direction" : "I"}]},
	{"ID" : "231", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.grp_readkeyvalues0_3_fu_1912", "Parent" : "0", "Child" : ["232"],
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
					{"ID" : "232", "SubInstance" : "grp_readkeyvalues0_166_fu_136", "Port" : "kvdram_V"}]},
			{"Name" : "kvdram_V_offset", "Type" : "None", "Direction" : "I"},
			{"Name" : "buffer0_V", "Type" : "Memory", "Direction" : "O",
				"SubConnect" : [
					{"ID" : "232", "SubInstance" : "grp_readkeyvalues0_166_fu_136", "Port" : "buffer_V"}]},
			{"Name" : "buffer1_V", "Type" : "Memory", "Direction" : "O",
				"SubConnect" : [
					{"ID" : "232", "SubInstance" : "grp_readkeyvalues0_166_fu_136", "Port" : "buffer_V"}]},
			{"Name" : "buffer2_V", "Type" : "Memory", "Direction" : "O",
				"SubConnect" : [
					{"ID" : "232", "SubInstance" : "grp_readkeyvalues0_166_fu_136", "Port" : "buffer_V"}]},
			{"Name" : "buffer3_V", "Type" : "Memory", "Direction" : "O",
				"SubConnect" : [
					{"ID" : "232", "SubInstance" : "grp_readkeyvalues0_166_fu_136", "Port" : "buffer_V"}]},
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
	{"ID" : "232", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_readkeyvalues0_3_fu_1912.grp_readkeyvalues0_166_fu_136", "Parent" : "231",
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
			{"Name" : "size_kvs", "Type" : "None", "Direction" : "I"}]},
	{"ID" : "233", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.grp_readkeyvalues0_2_fu_1934", "Parent" : "0", "Child" : ["234"],
		"CDFG" : "readkeyvalues0_2",
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
			{"State" : "ap_ST_fsm_state3", "FSM" : "ap_CS_fsm", "SubInstance" : "grp_readkeyvalues0_166_fu_94"},
			{"State" : "ap_ST_fsm_state5", "FSM" : "ap_CS_fsm", "SubInstance" : "grp_readkeyvalues0_166_fu_94"},
			{"State" : "ap_ST_fsm_state7", "FSM" : "ap_CS_fsm", "SubInstance" : "grp_readkeyvalues0_166_fu_94"},
			{"State" : "ap_ST_fsm_state9", "FSM" : "ap_CS_fsm", "SubInstance" : "grp_readkeyvalues0_166_fu_94"}],
		"Port" : [
			{"Name" : "enable", "Type" : "None", "Direction" : "I"},
			{"Name" : "kvdram_V", "Type" : "MAXI", "Direction" : "I",
				"SubConnect" : [
					{"ID" : "234", "SubInstance" : "grp_readkeyvalues0_166_fu_94", "Port" : "kvdram_V"}]},
			{"Name" : "kvdram_V_offset", "Type" : "None", "Direction" : "I"},
			{"Name" : "buffer0_V", "Type" : "Memory", "Direction" : "O",
				"SubConnect" : [
					{"ID" : "234", "SubInstance" : "grp_readkeyvalues0_166_fu_94", "Port" : "buffer_V"}]},
			{"Name" : "buffer1_V", "Type" : "Memory", "Direction" : "O",
				"SubConnect" : [
					{"ID" : "234", "SubInstance" : "grp_readkeyvalues0_166_fu_94", "Port" : "buffer_V"}]},
			{"Name" : "buffer2_V", "Type" : "Memory", "Direction" : "O",
				"SubConnect" : [
					{"ID" : "234", "SubInstance" : "grp_readkeyvalues0_166_fu_94", "Port" : "buffer_V"}]},
			{"Name" : "buffer3_V", "Type" : "Memory", "Direction" : "O",
				"SubConnect" : [
					{"ID" : "234", "SubInstance" : "grp_readkeyvalues0_166_fu_94", "Port" : "buffer_V"}]},
			{"Name" : "baseaddress", "Type" : "None", "Direction" : "I"},
			{"Name" : "offset_kvs", "Type" : "None", "Direction" : "I"},
			{"Name" : "travstate_i_kvs", "Type" : "None", "Direction" : "I"},
			{"Name" : "travstate_end_kvs", "Type" : "None", "Direction" : "I"}]},
	{"ID" : "234", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_readkeyvalues0_2_fu_1934.grp_readkeyvalues0_166_fu_94", "Parent" : "233",
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
			{"Name" : "size_kvs", "Type" : "None", "Direction" : "I"}]},
	{"ID" : "235", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.grp_savekeyvalues0_2_fu_1951", "Parent" : "0",
		"CDFG" : "savekeyvalues0_2",
		"Protocol" : "ap_ctrl_hs",
		"ControlExist" : "1", "ap_start" : "1", "ap_ready" : "1", "ap_done" : "1", "ap_continue" : "0", "ap_idle" : "1",
		"Pipeline" : "None", "UnalignedPipeline" : "0", "RewindPipeline" : "0", "ProcessNetwork" : "0",
		"II" : "0",
		"VariableLatency" : "1", "ExactLatency" : "-1", "EstimateLatencyMin" : "264", "EstimateLatencyMax" : "264",
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
			{"Name" : "buffer30_key", "Type" : "Memory", "Direction" : "I"},
			{"Name" : "buffer30_value", "Type" : "Memory", "Direction" : "I"},
			{"Name" : "buffer31_key", "Type" : "Memory", "Direction" : "I"},
			{"Name" : "buffer31_value", "Type" : "Memory", "Direction" : "I"},
			{"Name" : "buffer32_key", "Type" : "Memory", "Direction" : "I"},
			{"Name" : "buffer32_value", "Type" : "Memory", "Direction" : "I"},
			{"Name" : "buffer33_key", "Type" : "Memory", "Direction" : "I"},
			{"Name" : "buffer33_value", "Type" : "Memory", "Direction" : "I"},
			{"Name" : "buffer34_key", "Type" : "Memory", "Direction" : "I"},
			{"Name" : "buffer34_value", "Type" : "Memory", "Direction" : "I"},
			{"Name" : "buffer35_key", "Type" : "Memory", "Direction" : "I"},
			{"Name" : "buffer35_value", "Type" : "Memory", "Direction" : "I"},
			{"Name" : "buffer36_key", "Type" : "Memory", "Direction" : "I"},
			{"Name" : "buffer36_value", "Type" : "Memory", "Direction" : "I"},
			{"Name" : "buffer37_key", "Type" : "Memory", "Direction" : "I"},
			{"Name" : "buffer37_value", "Type" : "Memory", "Direction" : "I"},
			{"Name" : "offset_kvs", "Type" : "None", "Direction" : "I"}]},
	{"ID" : "236", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.grp_readkeyvalues0_1_fu_1975", "Parent" : "0",
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
			{"Name" : "offset_kvs", "Type" : "None", "Direction" : "I"}]},
	{"ID" : "237", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.grp_savekeyvalues0_251_fu_2047", "Parent" : "0",
		"CDFG" : "savekeyvalues0_251",
		"Protocol" : "ap_ctrl_hs",
		"ControlExist" : "1", "ap_start" : "1", "ap_ready" : "1", "ap_done" : "1", "ap_continue" : "0", "ap_idle" : "1",
		"Pipeline" : "None", "UnalignedPipeline" : "0", "RewindPipeline" : "0", "ProcessNetwork" : "0",
		"II" : "0",
		"VariableLatency" : "1", "ExactLatency" : "-1", "EstimateLatencyMin" : "40", "EstimateLatencyMax" : "40",
		"Combinational" : "0",
		"Datapath" : "0",
		"ClockEnable" : "0",
		"HasSubDataflow" : "0",
		"InDataflowNetwork" : "0",
		"HasNonBlockingOperation" : "0",
		"Port" : [
			{"Name" : "dram", "Type" : "MAXI", "Direction" : "O",
				"BlockSignal" : [
					{"Name" : "dram_blk_n_AW", "Type" : "RtlSignal"},
					{"Name" : "dram_blk_n_W", "Type" : "RtlSignal"},
					{"Name" : "dram_blk_n_B", "Type" : "RtlSignal"}]},
			{"Name" : "dram_offset", "Type" : "None", "Direction" : "I"},
			{"Name" : "buffer_key", "Type" : "Memory", "Direction" : "I"},
			{"Name" : "buffer_value", "Type" : "Memory", "Direction" : "I"},
			{"Name" : "offset_kvs", "Type" : "None", "Direction" : "I"}]},
	{"ID" : "238", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.grp_savekeyvalues0_1_fu_2057", "Parent" : "0",
		"CDFG" : "savekeyvalues0_1",
		"Protocol" : "ap_ctrl_hs",
		"ControlExist" : "1", "ap_start" : "1", "ap_ready" : "1", "ap_done" : "1", "ap_continue" : "0", "ap_idle" : "1",
		"Pipeline" : "Aligned", "UnalignedPipeline" : "0", "RewindPipeline" : "0", "ProcessNetwork" : "0",
		"II" : "1",
		"VariableLatency" : "0", "ExactLatency" : "7", "EstimateLatencyMin" : "7", "EstimateLatencyMax" : "7",
		"Combinational" : "0",
		"Datapath" : "0",
		"ClockEnable" : "0",
		"HasSubDataflow" : "0",
		"InDataflowNetwork" : "0",
		"HasNonBlockingOperation" : "0",
		"Port" : [
			{"Name" : "dram", "Type" : "MAXI", "Direction" : "O",
				"BlockSignal" : [
					{"Name" : "dram_blk_n_AW", "Type" : "RtlPort"},
					{"Name" : "dram_blk_n_W", "Type" : "RtlPort"},
					{"Name" : "dram_blk_n_B", "Type" : "RtlPort"}]},
			{"Name" : "dram_offset", "Type" : "None", "Direction" : "I"},
			{"Name" : "buffer_0_key_read", "Type" : "None", "Direction" : "I"},
			{"Name" : "buffer_0_value_read", "Type" : "None", "Direction" : "I"},
			{"Name" : "offset_kvs", "Type" : "None", "Direction" : "I"}]},
	{"ID" : "239", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.topkernel_mux_42_32_1_1_U360", "Parent" : "0"}]}


set ArgLastReadFirstWriteLatency {
	partitionandreduce0 {
		kvsourcedram_V {Type IO LastRead 73 FirstWrite -1}
		kvsourcedram_V_offset {Type I LastRead 0 FirstWrite -1}
		kvdestdram_V {Type IO LastRead 9 FirstWrite -1}
		kvdestdram_V_offset {Type I LastRead 0 FirstWrite -1}
		kvstats {Type IO LastRead 44 FirstWrite 2}
		kvstats_offset {Type I LastRead 0 FirstWrite -1}
		globalparams_runkernelcommand {Type I LastRead 0 FirstWrite -1}
		globalparams_partitioncommand {Type I LastRead 0 FirstWrite -1}
		globalparams_reducecommand {Type I LastRead 0 FirstWrite -1}
		globalparams_treedepth {Type I LastRead 0 FirstWrite -1}
		globalparams_runsize {Type I LastRead 0 FirstWrite -1}
		globalparams_vbegin {Type I LastRead 0 FirstWrite -1}
		globalparams_statsalreadycollected {Type I LastRead 0 FirstWrite -1}}
	reducepartitions0 {
		enable {Type I LastRead 0 FirstWrite -1}
		sourcebuffer0_V {Type I LastRead 2 FirstWrite -1}
		sourcebuffer1_V {Type I LastRead 2 FirstWrite -1}
		sourcebuffer2_V {Type I LastRead 2 FirstWrite -1}
		sourcebuffer3_V {Type I LastRead 2 FirstWrite -1}
		destbuffer00_value {Type IO LastRead 4 FirstWrite 5}
		destbuffer01_value {Type IO LastRead 4 FirstWrite 5}
		destbuffer02_value {Type IO LastRead 4 FirstWrite 5}
		destbuffer03_value {Type IO LastRead 4 FirstWrite 5}
		destbuffer04_value {Type IO LastRead 4 FirstWrite 5}
		destbuffer05_value {Type IO LastRead 4 FirstWrite 5}
		destbuffer06_value {Type IO LastRead 4 FirstWrite 5}
		destbuffer07_value {Type IO LastRead 4 FirstWrite 5}
		destbuffer10_value {Type IO LastRead 4 FirstWrite 5}
		destbuffer11_value {Type IO LastRead 4 FirstWrite 5}
		destbuffer12_value {Type IO LastRead 4 FirstWrite 5}
		destbuffer13_value {Type IO LastRead 4 FirstWrite 5}
		destbuffer14_value {Type IO LastRead 4 FirstWrite 5}
		destbuffer15_value {Type IO LastRead 4 FirstWrite 5}
		destbuffer16_value {Type IO LastRead 4 FirstWrite 5}
		destbuffer17_value {Type IO LastRead 4 FirstWrite 5}
		destbuffer20_value {Type IO LastRead 4 FirstWrite 5}
		destbuffer21_value {Type IO LastRead 4 FirstWrite 5}
		destbuffer22_value {Type IO LastRead 4 FirstWrite 5}
		destbuffer23_value {Type IO LastRead 4 FirstWrite 5}
		destbuffer24_value {Type IO LastRead 4 FirstWrite 5}
		destbuffer25_value {Type IO LastRead 4 FirstWrite 5}
		destbuffer26_value {Type IO LastRead 4 FirstWrite 5}
		destbuffer27_value {Type IO LastRead 4 FirstWrite 5}
		destbuffer30_value {Type IO LastRead 4 FirstWrite 5}
		destbuffer31_value {Type IO LastRead 4 FirstWrite 5}
		destbuffer32_value {Type IO LastRead 4 FirstWrite 5}
		destbuffer33_value {Type IO LastRead 4 FirstWrite 5}
		destbuffer34_value {Type IO LastRead 4 FirstWrite 5}
		destbuffer35_value {Type IO LastRead 4 FirstWrite 5}
		destbuffer36_value {Type IO LastRead 4 FirstWrite 5}
		destbuffer37_value {Type IO LastRead 4 FirstWrite 5}
		globalparams_vbegin {Type I LastRead 0 FirstWrite -1}}
	partitionkeyvalues00 {
		enable {Type I LastRead 0 FirstWrite -1}
		sourcebuffer0_V {Type I LastRead 2 FirstWrite -1}
		sourcebuffer1_V {Type I LastRead 2 FirstWrite -1}
		sourcebuffer2_V {Type I LastRead 2 FirstWrite -1}
		sourcebuffer3_V {Type I LastRead 2 FirstWrite -1}
		destbuffer0_V {Type O LastRead -1 FirstWrite 3}
		destbuffer1_V {Type O LastRead -1 FirstWrite 3}
		destbuffer2_V {Type O LastRead -1 FirstWrite 3}
		destbuffer3_V {Type O LastRead -1 FirstWrite 3}
		capsule0_key {Type I LastRead 1 FirstWrite -1}
		capsule0_value {Type IO LastRead 1 FirstWrite 4}
		capsule1_key {Type I LastRead 1 FirstWrite -1}
		capsule1_value {Type IO LastRead 1 FirstWrite 4}
		capsule2_key {Type I LastRead 1 FirstWrite -1}
		capsule2_value {Type IO LastRead 1 FirstWrite 4}
		capsule3_key {Type I LastRead 1 FirstWrite -1}
		capsule3_value {Type IO LastRead 1 FirstWrite 4}
		travstate_i_kvs {Type I LastRead 0 FirstWrite -1}
		travstate_end_kvs {Type I LastRead 0 FirstWrite -1}
		llopparams_currentLOP {Type I LastRead 0 FirstWrite -1}
		llopparams_upperlimit {Type I LastRead 0 FirstWrite -1}}
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
		kvdrambaseaddress {Type I LastRead 0 FirstWrite -1}}
	collectstats00 {
		enable {Type I LastRead 0 FirstWrite -1}
		sourcebuffer0_V {Type I LastRead 2 FirstWrite -1}
		sourcebuffer1_V {Type I LastRead 2 FirstWrite -1}
		sourcebuffer2_V {Type I LastRead 2 FirstWrite -1}
		sourcebuffer3_V {Type I LastRead 2 FirstWrite -1}
		capsule0_key {Type O LastRead -1 FirstWrite 4}
		capsule0_value {Type IO LastRead 4 FirstWrite 5}
		capsule1_key {Type O LastRead -1 FirstWrite 4}
		capsule1_value {Type IO LastRead 4 FirstWrite 5}
		capsule2_key {Type O LastRead -1 FirstWrite 4}
		capsule2_value {Type IO LastRead 4 FirstWrite 5}
		capsule3_key {Type O LastRead -1 FirstWrite 4}
		capsule3_value {Type IO LastRead 4 FirstWrite 5}
		llopparams_currentLOP {Type I LastRead 0 FirstWrite -1}
		llopparams_upperlimit {Type I LastRead 0 FirstWrite -1}
		travstate_i_kvs {Type I LastRead 0 FirstWrite -1}
		travstate_end_kvs {Type I LastRead 0 FirstWrite -1}}
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
		upperlimit {Type I LastRead 0 FirstWrite -1}}
	readcapsules0 {
		kvdram_V {Type I LastRead 9 FirstWrite -1}
		kvdram_V_offset {Type I LastRead 0 FirstWrite -1}
		BIGcapsule0_V {Type IO LastRead 8 FirstWrite 10}
		BIGcapsule1_V {Type IO LastRead 8 FirstWrite 10}
		BIGcapsule2_V {Type IO LastRead 8 FirstWrite 10}
		BIGcapsule3_V {Type IO LastRead 8 FirstWrite 10}
		capsule0_key {Type O LastRead -1 FirstWrite 9}
		capsule0_value {Type O LastRead -1 FirstWrite 9}
		capsule1_key {Type O LastRead -1 FirstWrite 9}
		capsule1_value {Type O LastRead -1 FirstWrite 9}
		capsule2_key {Type O LastRead -1 FirstWrite 9}
		capsule2_value {Type O LastRead -1 FirstWrite 9}
		capsule3_key {Type O LastRead -1 FirstWrite 9}
		capsule3_value {Type O LastRead -1 FirstWrite 9}
		offset {Type I LastRead 0 FirstWrite -1}
		capsulemetadata_globaloffset {Type I LastRead 0 FirstWrite -1}
		capsulemetadata_localoffset {Type I LastRead 0 FirstWrite -1}}
	readkeyvalues0 {
		kvdram_V {Type I LastRead 9 FirstWrite -1}
		kvdram_V_offset {Type I LastRead 0 FirstWrite -1}
		buffer_V {Type O LastRead -1 FirstWrite 10}
		offset_kvs {Type I LastRead 0 FirstWrite -1}}
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
		offset_kvs {Type I LastRead 0 FirstWrite -1}}
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
		size_kvs {Type I LastRead 1 FirstWrite -1}}
	readkeyvalues0_2 {
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
	readkeyvalues0_166 {
		kvdram_V {Type I LastRead 9 FirstWrite -1}
		kvdram_V_offset {Type I LastRead 0 FirstWrite -1}
		buffer_V {Type O LastRead -1 FirstWrite 10}
		baseaddress {Type I LastRead 0 FirstWrite -1}
		offset_kvs {Type I LastRead 0 FirstWrite -1}
		size_kvs {Type I LastRead 1 FirstWrite -1}}
	savekeyvalues0_2 {
		kvdram_V {Type O LastRead 3 FirstWrite 4}
		kvdram_V_offset {Type I LastRead 0 FirstWrite -1}
		buffer30_key {Type I LastRead 2 FirstWrite -1}
		buffer30_value {Type I LastRead 2 FirstWrite -1}
		buffer31_key {Type I LastRead 2 FirstWrite -1}
		buffer31_value {Type I LastRead 2 FirstWrite -1}
		buffer32_key {Type I LastRead 2 FirstWrite -1}
		buffer32_value {Type I LastRead 2 FirstWrite -1}
		buffer33_key {Type I LastRead 2 FirstWrite -1}
		buffer33_value {Type I LastRead 2 FirstWrite -1}
		buffer34_key {Type I LastRead 2 FirstWrite -1}
		buffer34_value {Type I LastRead 2 FirstWrite -1}
		buffer35_key {Type I LastRead 2 FirstWrite -1}
		buffer35_value {Type I LastRead 2 FirstWrite -1}
		buffer36_key {Type I LastRead 2 FirstWrite -1}
		buffer36_value {Type I LastRead 2 FirstWrite -1}
		buffer37_key {Type I LastRead 2 FirstWrite -1}
		buffer37_value {Type I LastRead 2 FirstWrite -1}
		offset_kvs {Type I LastRead 0 FirstWrite -1}}
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
		offset_kvs {Type I LastRead 0 FirstWrite -1}}
	savekeyvalues0_251 {
		dram {Type O LastRead 3 FirstWrite 4}
		dram_offset {Type I LastRead 0 FirstWrite -1}
		buffer_key {Type I LastRead 2 FirstWrite -1}
		buffer_value {Type I LastRead 2 FirstWrite -1}
		offset_kvs {Type I LastRead 0 FirstWrite -1}}
	savekeyvalues0_1 {
		dram {Type O LastRead 3 FirstWrite 2}
		dram_offset {Type I LastRead 0 FirstWrite -1}
		buffer_0_key_read {Type I LastRead 0 FirstWrite -1}
		buffer_0_value_read {Type I LastRead 0 FirstWrite -1}
		offset_kvs {Type I LastRead 0 FirstWrite -1}}}

set hasDtUnsupportedChannel 0

set PerformanceInfo {[
	{"Name" : "Latency", "Min" : "41", "Max" : "197844218"}
	, {"Name" : "Interval", "Min" : "41", "Max" : "197844218"}
]}

set PipelineEnableSignalInfo {[
]}

set Spec2ImplPortList { 
	kvsourcedram_V { m_axi {  { m_axi_kvsourcedram_V_AWVALID VALID 1 1 }  { m_axi_kvsourcedram_V_AWREADY READY 0 1 }  { m_axi_kvsourcedram_V_AWADDR ADDR 1 32 }  { m_axi_kvsourcedram_V_AWID ID 1 1 }  { m_axi_kvsourcedram_V_AWLEN LEN 1 32 }  { m_axi_kvsourcedram_V_AWSIZE SIZE 1 3 }  { m_axi_kvsourcedram_V_AWBURST BURST 1 2 }  { m_axi_kvsourcedram_V_AWLOCK LOCK 1 2 }  { m_axi_kvsourcedram_V_AWCACHE CACHE 1 4 }  { m_axi_kvsourcedram_V_AWPROT PROT 1 3 }  { m_axi_kvsourcedram_V_AWQOS QOS 1 4 }  { m_axi_kvsourcedram_V_AWREGION REGION 1 4 }  { m_axi_kvsourcedram_V_AWUSER USER 1 1 }  { m_axi_kvsourcedram_V_WVALID VALID 1 1 }  { m_axi_kvsourcedram_V_WREADY READY 0 1 }  { m_axi_kvsourcedram_V_WDATA DATA 1 512 }  { m_axi_kvsourcedram_V_WSTRB STRB 1 64 }  { m_axi_kvsourcedram_V_WLAST LAST 1 1 }  { m_axi_kvsourcedram_V_WID ID 1 1 }  { m_axi_kvsourcedram_V_WUSER USER 1 1 }  { m_axi_kvsourcedram_V_ARVALID VALID 1 1 }  { m_axi_kvsourcedram_V_ARREADY READY 0 1 }  { m_axi_kvsourcedram_V_ARADDR ADDR 1 32 }  { m_axi_kvsourcedram_V_ARID ID 1 1 }  { m_axi_kvsourcedram_V_ARLEN LEN 1 32 }  { m_axi_kvsourcedram_V_ARSIZE SIZE 1 3 }  { m_axi_kvsourcedram_V_ARBURST BURST 1 2 }  { m_axi_kvsourcedram_V_ARLOCK LOCK 1 2 }  { m_axi_kvsourcedram_V_ARCACHE CACHE 1 4 }  { m_axi_kvsourcedram_V_ARPROT PROT 1 3 }  { m_axi_kvsourcedram_V_ARQOS QOS 1 4 }  { m_axi_kvsourcedram_V_ARREGION REGION 1 4 }  { m_axi_kvsourcedram_V_ARUSER USER 1 1 }  { m_axi_kvsourcedram_V_RVALID VALID 0 1 }  { m_axi_kvsourcedram_V_RREADY READY 1 1 }  { m_axi_kvsourcedram_V_RDATA DATA 0 512 }  { m_axi_kvsourcedram_V_RLAST LAST 0 1 }  { m_axi_kvsourcedram_V_RID ID 0 1 }  { m_axi_kvsourcedram_V_RUSER USER 0 1 }  { m_axi_kvsourcedram_V_RRESP RESP 0 2 }  { m_axi_kvsourcedram_V_BVALID VALID 0 1 }  { m_axi_kvsourcedram_V_BREADY READY 1 1 }  { m_axi_kvsourcedram_V_BRESP RESP 0 2 }  { m_axi_kvsourcedram_V_BID ID 0 1 }  { m_axi_kvsourcedram_V_BUSER USER 0 1 } } }
	kvsourcedram_V_offset { ap_none {  { kvsourcedram_V_offset in_data 0 26 } } }
	kvdestdram_V { m_axi {  { m_axi_kvdestdram_V_AWVALID VALID 1 1 }  { m_axi_kvdestdram_V_AWREADY READY 0 1 }  { m_axi_kvdestdram_V_AWADDR ADDR 1 32 }  { m_axi_kvdestdram_V_AWID ID 1 1 }  { m_axi_kvdestdram_V_AWLEN LEN 1 32 }  { m_axi_kvdestdram_V_AWSIZE SIZE 1 3 }  { m_axi_kvdestdram_V_AWBURST BURST 1 2 }  { m_axi_kvdestdram_V_AWLOCK LOCK 1 2 }  { m_axi_kvdestdram_V_AWCACHE CACHE 1 4 }  { m_axi_kvdestdram_V_AWPROT PROT 1 3 }  { m_axi_kvdestdram_V_AWQOS QOS 1 4 }  { m_axi_kvdestdram_V_AWREGION REGION 1 4 }  { m_axi_kvdestdram_V_AWUSER USER 1 1 }  { m_axi_kvdestdram_V_WVALID VALID 1 1 }  { m_axi_kvdestdram_V_WREADY READY 0 1 }  { m_axi_kvdestdram_V_WDATA DATA 1 512 }  { m_axi_kvdestdram_V_WSTRB STRB 1 64 }  { m_axi_kvdestdram_V_WLAST LAST 1 1 }  { m_axi_kvdestdram_V_WID ID 1 1 }  { m_axi_kvdestdram_V_WUSER USER 1 1 }  { m_axi_kvdestdram_V_ARVALID VALID 1 1 }  { m_axi_kvdestdram_V_ARREADY READY 0 1 }  { m_axi_kvdestdram_V_ARADDR ADDR 1 32 }  { m_axi_kvdestdram_V_ARID ID 1 1 }  { m_axi_kvdestdram_V_ARLEN LEN 1 32 }  { m_axi_kvdestdram_V_ARSIZE SIZE 1 3 }  { m_axi_kvdestdram_V_ARBURST BURST 1 2 }  { m_axi_kvdestdram_V_ARLOCK LOCK 1 2 }  { m_axi_kvdestdram_V_ARCACHE CACHE 1 4 }  { m_axi_kvdestdram_V_ARPROT PROT 1 3 }  { m_axi_kvdestdram_V_ARQOS QOS 1 4 }  { m_axi_kvdestdram_V_ARREGION REGION 1 4 }  { m_axi_kvdestdram_V_ARUSER USER 1 1 }  { m_axi_kvdestdram_V_RVALID VALID 0 1 }  { m_axi_kvdestdram_V_RREADY READY 1 1 }  { m_axi_kvdestdram_V_RDATA DATA 0 512 }  { m_axi_kvdestdram_V_RLAST LAST 0 1 }  { m_axi_kvdestdram_V_RID ID 0 1 }  { m_axi_kvdestdram_V_RUSER USER 0 1 }  { m_axi_kvdestdram_V_RRESP RESP 0 2 }  { m_axi_kvdestdram_V_BVALID VALID 0 1 }  { m_axi_kvdestdram_V_BREADY READY 1 1 }  { m_axi_kvdestdram_V_BRESP RESP 0 2 }  { m_axi_kvdestdram_V_BID ID 0 1 }  { m_axi_kvdestdram_V_BUSER USER 0 1 } } }
	kvdestdram_V_offset { ap_none {  { kvdestdram_V_offset in_data 0 26 } } }
	kvstats { m_axi {  { m_axi_kvstats_AWVALID VALID 1 1 }  { m_axi_kvstats_AWREADY READY 0 1 }  { m_axi_kvstats_AWADDR ADDR 1 32 }  { m_axi_kvstats_AWID ID 1 1 }  { m_axi_kvstats_AWLEN LEN 1 32 }  { m_axi_kvstats_AWSIZE SIZE 1 3 }  { m_axi_kvstats_AWBURST BURST 1 2 }  { m_axi_kvstats_AWLOCK LOCK 1 2 }  { m_axi_kvstats_AWCACHE CACHE 1 4 }  { m_axi_kvstats_AWPROT PROT 1 3 }  { m_axi_kvstats_AWQOS QOS 1 4 }  { m_axi_kvstats_AWREGION REGION 1 4 }  { m_axi_kvstats_AWUSER USER 1 1 }  { m_axi_kvstats_WVALID VALID 1 1 }  { m_axi_kvstats_WREADY READY 0 1 }  { m_axi_kvstats_WDATA DATA 1 64 }  { m_axi_kvstats_WSTRB STRB 1 8 }  { m_axi_kvstats_WLAST LAST 1 1 }  { m_axi_kvstats_WID ID 1 1 }  { m_axi_kvstats_WUSER USER 1 1 }  { m_axi_kvstats_ARVALID VALID 1 1 }  { m_axi_kvstats_ARREADY READY 0 1 }  { m_axi_kvstats_ARADDR ADDR 1 32 }  { m_axi_kvstats_ARID ID 1 1 }  { m_axi_kvstats_ARLEN LEN 1 32 }  { m_axi_kvstats_ARSIZE SIZE 1 3 }  { m_axi_kvstats_ARBURST BURST 1 2 }  { m_axi_kvstats_ARLOCK LOCK 1 2 }  { m_axi_kvstats_ARCACHE CACHE 1 4 }  { m_axi_kvstats_ARPROT PROT 1 3 }  { m_axi_kvstats_ARQOS QOS 1 4 }  { m_axi_kvstats_ARREGION REGION 1 4 }  { m_axi_kvstats_ARUSER USER 1 1 }  { m_axi_kvstats_RVALID VALID 0 1 }  { m_axi_kvstats_RREADY READY 1 1 }  { m_axi_kvstats_RDATA DATA 0 64 }  { m_axi_kvstats_RLAST LAST 0 1 }  { m_axi_kvstats_RID ID 0 1 }  { m_axi_kvstats_RUSER USER 0 1 }  { m_axi_kvstats_RRESP RESP 0 2 }  { m_axi_kvstats_BVALID VALID 0 1 }  { m_axi_kvstats_BREADY READY 1 1 }  { m_axi_kvstats_BRESP RESP 0 2 }  { m_axi_kvstats_BID ID 0 1 }  { m_axi_kvstats_BUSER USER 0 1 } } }
	kvstats_offset { ap_none {  { kvstats_offset in_data 0 29 } } }
	globalparams_runkernelcommand { ap_none {  { globalparams_runkernelcommand in_data 0 32 } } }
	globalparams_partitioncommand { ap_none {  { globalparams_partitioncommand in_data 0 32 } } }
	globalparams_reducecommand { ap_none {  { globalparams_reducecommand in_data 0 32 } } }
	globalparams_treedepth { ap_none {  { globalparams_treedepth in_data 0 32 } } }
	globalparams_runsize { ap_none {  { globalparams_runsize in_data 0 32 } } }
	globalparams_vbegin { ap_none {  { globalparams_vbegin in_data 0 32 } } }
	globalparams_statsalreadycollected { ap_none {  { globalparams_statsalreadycollected in_data 0 32 } } }
}
