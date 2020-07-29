set moduleName topkernel
set isTopModule 1
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
set C_modelName {topkernel}
set C_modelType { void 0 }
set C_modelArgList {
	{ gmem0 int 512 regular {axi_master 2}  }
	{ gmem1 int 512 regular {axi_master 2}  }
	{ gmem2 int 64 regular {axi_master 2}  }
	{ kvsourcedramA_V int 32 regular {axi_slave 0}  }
	{ kvdestdramA_V int 32 regular {axi_slave 0}  }
	{ kvstatsA int 32 regular {axi_slave 0}  }
}
set C_modelArgMapList {[ 
	{ "Name" : "gmem0", "interface" : "axi_master", "bitwidth" : 512, "direction" : "READWRITE", "bitSlice":[{"low":0,"up":511,"cElement": [{"cName": "kvsourcedramA.V","cData": "uint512","bit_use": { "low": 0,"up": 511},"offset": { "type": "dynamic","port_name": "kvsourcedramA_V","bundle": "control"},"direction": "READWRITE","cArray": [{"low" : 0,"up" : 0,"step" : 1}]}]}]} , 
 	{ "Name" : "gmem1", "interface" : "axi_master", "bitwidth" : 512, "direction" : "READWRITE", "bitSlice":[{"low":0,"up":511,"cElement": [{"cName": "kvdestdramA.V","cData": "uint512","bit_use": { "low": 0,"up": 511},"offset": { "type": "dynamic","port_name": "kvdestdramA_V","bundle": "control"},"direction": "READWRITE","cArray": [{"low" : 0,"up" : 0,"step" : 1}]}]}]} , 
 	{ "Name" : "gmem2", "interface" : "axi_master", "bitwidth" : 64, "direction" : "READWRITE", "bitSlice":[{"low":0,"up":31,"cElement": [{"cName": "kvstatsA.key","cData": "unsigned int","bit_use": { "low": 0,"up": 31},"offset": { "type": "dynamic","port_name": "kvstatsA"},"direction": "READWRITE","cArray": [{"low" : 0,"up" : 0,"step" : 1}]}]},{"low":32,"up":63,"cElement": [{"cName": "kvstatsA.value","cData": "unsigned int","bit_use": { "low": 0,"up": 31},"offset": { "type": "dynamic","port_name": "kvstatsA"},"direction": "READWRITE","cArray": [{"low" : 0,"up" : 0,"step" : 1}]}]}]} , 
 	{ "Name" : "kvsourcedramA_V", "interface" : "axi_slave", "bundle":"control","type":"ap_none","bitwidth" : 32, "direction" : "READONLY", "offset" : {"in":16}, "offset_end" : {"in":23}} , 
 	{ "Name" : "kvdestdramA_V", "interface" : "axi_slave", "bundle":"control","type":"ap_none","bitwidth" : 32, "direction" : "READONLY", "offset" : {"in":24}, "offset_end" : {"in":31}} , 
 	{ "Name" : "kvstatsA", "interface" : "axi_slave", "bundle":"control","type":"ap_none","bitwidth" : 32, "direction" : "READONLY", "offset" : {"in":32}, "offset_end" : {"in":39}} ]}
# RTL Port declarations: 
set portNum 155
set portList { 
	{ ap_clk sc_in sc_logic 1 clock -1 } 
	{ ap_rst_n sc_in sc_logic 1 reset -1 active_low_sync } 
	{ m_axi_gmem0_AWVALID sc_out sc_logic 1 signal 0 } 
	{ m_axi_gmem0_AWREADY sc_in sc_logic 1 signal 0 } 
	{ m_axi_gmem0_AWADDR sc_out sc_lv 32 signal 0 } 
	{ m_axi_gmem0_AWID sc_out sc_lv 1 signal 0 } 
	{ m_axi_gmem0_AWLEN sc_out sc_lv 8 signal 0 } 
	{ m_axi_gmem0_AWSIZE sc_out sc_lv 3 signal 0 } 
	{ m_axi_gmem0_AWBURST sc_out sc_lv 2 signal 0 } 
	{ m_axi_gmem0_AWLOCK sc_out sc_lv 2 signal 0 } 
	{ m_axi_gmem0_AWCACHE sc_out sc_lv 4 signal 0 } 
	{ m_axi_gmem0_AWPROT sc_out sc_lv 3 signal 0 } 
	{ m_axi_gmem0_AWQOS sc_out sc_lv 4 signal 0 } 
	{ m_axi_gmem0_AWREGION sc_out sc_lv 4 signal 0 } 
	{ m_axi_gmem0_AWUSER sc_out sc_lv 1 signal 0 } 
	{ m_axi_gmem0_WVALID sc_out sc_logic 1 signal 0 } 
	{ m_axi_gmem0_WREADY sc_in sc_logic 1 signal 0 } 
	{ m_axi_gmem0_WDATA sc_out sc_lv 512 signal 0 } 
	{ m_axi_gmem0_WSTRB sc_out sc_lv 64 signal 0 } 
	{ m_axi_gmem0_WLAST sc_out sc_logic 1 signal 0 } 
	{ m_axi_gmem0_WID sc_out sc_lv 1 signal 0 } 
	{ m_axi_gmem0_WUSER sc_out sc_lv 1 signal 0 } 
	{ m_axi_gmem0_ARVALID sc_out sc_logic 1 signal 0 } 
	{ m_axi_gmem0_ARREADY sc_in sc_logic 1 signal 0 } 
	{ m_axi_gmem0_ARADDR sc_out sc_lv 32 signal 0 } 
	{ m_axi_gmem0_ARID sc_out sc_lv 1 signal 0 } 
	{ m_axi_gmem0_ARLEN sc_out sc_lv 8 signal 0 } 
	{ m_axi_gmem0_ARSIZE sc_out sc_lv 3 signal 0 } 
	{ m_axi_gmem0_ARBURST sc_out sc_lv 2 signal 0 } 
	{ m_axi_gmem0_ARLOCK sc_out sc_lv 2 signal 0 } 
	{ m_axi_gmem0_ARCACHE sc_out sc_lv 4 signal 0 } 
	{ m_axi_gmem0_ARPROT sc_out sc_lv 3 signal 0 } 
	{ m_axi_gmem0_ARQOS sc_out sc_lv 4 signal 0 } 
	{ m_axi_gmem0_ARREGION sc_out sc_lv 4 signal 0 } 
	{ m_axi_gmem0_ARUSER sc_out sc_lv 1 signal 0 } 
	{ m_axi_gmem0_RVALID sc_in sc_logic 1 signal 0 } 
	{ m_axi_gmem0_RREADY sc_out sc_logic 1 signal 0 } 
	{ m_axi_gmem0_RDATA sc_in sc_lv 512 signal 0 } 
	{ m_axi_gmem0_RLAST sc_in sc_logic 1 signal 0 } 
	{ m_axi_gmem0_RID sc_in sc_lv 1 signal 0 } 
	{ m_axi_gmem0_RUSER sc_in sc_lv 1 signal 0 } 
	{ m_axi_gmem0_RRESP sc_in sc_lv 2 signal 0 } 
	{ m_axi_gmem0_BVALID sc_in sc_logic 1 signal 0 } 
	{ m_axi_gmem0_BREADY sc_out sc_logic 1 signal 0 } 
	{ m_axi_gmem0_BRESP sc_in sc_lv 2 signal 0 } 
	{ m_axi_gmem0_BID sc_in sc_lv 1 signal 0 } 
	{ m_axi_gmem0_BUSER sc_in sc_lv 1 signal 0 } 
	{ m_axi_gmem1_AWVALID sc_out sc_logic 1 signal 1 } 
	{ m_axi_gmem1_AWREADY sc_in sc_logic 1 signal 1 } 
	{ m_axi_gmem1_AWADDR sc_out sc_lv 32 signal 1 } 
	{ m_axi_gmem1_AWID sc_out sc_lv 1 signal 1 } 
	{ m_axi_gmem1_AWLEN sc_out sc_lv 8 signal 1 } 
	{ m_axi_gmem1_AWSIZE sc_out sc_lv 3 signal 1 } 
	{ m_axi_gmem1_AWBURST sc_out sc_lv 2 signal 1 } 
	{ m_axi_gmem1_AWLOCK sc_out sc_lv 2 signal 1 } 
	{ m_axi_gmem1_AWCACHE sc_out sc_lv 4 signal 1 } 
	{ m_axi_gmem1_AWPROT sc_out sc_lv 3 signal 1 } 
	{ m_axi_gmem1_AWQOS sc_out sc_lv 4 signal 1 } 
	{ m_axi_gmem1_AWREGION sc_out sc_lv 4 signal 1 } 
	{ m_axi_gmem1_AWUSER sc_out sc_lv 1 signal 1 } 
	{ m_axi_gmem1_WVALID sc_out sc_logic 1 signal 1 } 
	{ m_axi_gmem1_WREADY sc_in sc_logic 1 signal 1 } 
	{ m_axi_gmem1_WDATA sc_out sc_lv 512 signal 1 } 
	{ m_axi_gmem1_WSTRB sc_out sc_lv 64 signal 1 } 
	{ m_axi_gmem1_WLAST sc_out sc_logic 1 signal 1 } 
	{ m_axi_gmem1_WID sc_out sc_lv 1 signal 1 } 
	{ m_axi_gmem1_WUSER sc_out sc_lv 1 signal 1 } 
	{ m_axi_gmem1_ARVALID sc_out sc_logic 1 signal 1 } 
	{ m_axi_gmem1_ARREADY sc_in sc_logic 1 signal 1 } 
	{ m_axi_gmem1_ARADDR sc_out sc_lv 32 signal 1 } 
	{ m_axi_gmem1_ARID sc_out sc_lv 1 signal 1 } 
	{ m_axi_gmem1_ARLEN sc_out sc_lv 8 signal 1 } 
	{ m_axi_gmem1_ARSIZE sc_out sc_lv 3 signal 1 } 
	{ m_axi_gmem1_ARBURST sc_out sc_lv 2 signal 1 } 
	{ m_axi_gmem1_ARLOCK sc_out sc_lv 2 signal 1 } 
	{ m_axi_gmem1_ARCACHE sc_out sc_lv 4 signal 1 } 
	{ m_axi_gmem1_ARPROT sc_out sc_lv 3 signal 1 } 
	{ m_axi_gmem1_ARQOS sc_out sc_lv 4 signal 1 } 
	{ m_axi_gmem1_ARREGION sc_out sc_lv 4 signal 1 } 
	{ m_axi_gmem1_ARUSER sc_out sc_lv 1 signal 1 } 
	{ m_axi_gmem1_RVALID sc_in sc_logic 1 signal 1 } 
	{ m_axi_gmem1_RREADY sc_out sc_logic 1 signal 1 } 
	{ m_axi_gmem1_RDATA sc_in sc_lv 512 signal 1 } 
	{ m_axi_gmem1_RLAST sc_in sc_logic 1 signal 1 } 
	{ m_axi_gmem1_RID sc_in sc_lv 1 signal 1 } 
	{ m_axi_gmem1_RUSER sc_in sc_lv 1 signal 1 } 
	{ m_axi_gmem1_RRESP sc_in sc_lv 2 signal 1 } 
	{ m_axi_gmem1_BVALID sc_in sc_logic 1 signal 1 } 
	{ m_axi_gmem1_BREADY sc_out sc_logic 1 signal 1 } 
	{ m_axi_gmem1_BRESP sc_in sc_lv 2 signal 1 } 
	{ m_axi_gmem1_BID sc_in sc_lv 1 signal 1 } 
	{ m_axi_gmem1_BUSER sc_in sc_lv 1 signal 1 } 
	{ m_axi_gmem2_AWVALID sc_out sc_logic 1 signal 2 } 
	{ m_axi_gmem2_AWREADY sc_in sc_logic 1 signal 2 } 
	{ m_axi_gmem2_AWADDR sc_out sc_lv 32 signal 2 } 
	{ m_axi_gmem2_AWID sc_out sc_lv 1 signal 2 } 
	{ m_axi_gmem2_AWLEN sc_out sc_lv 8 signal 2 } 
	{ m_axi_gmem2_AWSIZE sc_out sc_lv 3 signal 2 } 
	{ m_axi_gmem2_AWBURST sc_out sc_lv 2 signal 2 } 
	{ m_axi_gmem2_AWLOCK sc_out sc_lv 2 signal 2 } 
	{ m_axi_gmem2_AWCACHE sc_out sc_lv 4 signal 2 } 
	{ m_axi_gmem2_AWPROT sc_out sc_lv 3 signal 2 } 
	{ m_axi_gmem2_AWQOS sc_out sc_lv 4 signal 2 } 
	{ m_axi_gmem2_AWREGION sc_out sc_lv 4 signal 2 } 
	{ m_axi_gmem2_AWUSER sc_out sc_lv 1 signal 2 } 
	{ m_axi_gmem2_WVALID sc_out sc_logic 1 signal 2 } 
	{ m_axi_gmem2_WREADY sc_in sc_logic 1 signal 2 } 
	{ m_axi_gmem2_WDATA sc_out sc_lv 64 signal 2 } 
	{ m_axi_gmem2_WSTRB sc_out sc_lv 8 signal 2 } 
	{ m_axi_gmem2_WLAST sc_out sc_logic 1 signal 2 } 
	{ m_axi_gmem2_WID sc_out sc_lv 1 signal 2 } 
	{ m_axi_gmem2_WUSER sc_out sc_lv 1 signal 2 } 
	{ m_axi_gmem2_ARVALID sc_out sc_logic 1 signal 2 } 
	{ m_axi_gmem2_ARREADY sc_in sc_logic 1 signal 2 } 
	{ m_axi_gmem2_ARADDR sc_out sc_lv 32 signal 2 } 
	{ m_axi_gmem2_ARID sc_out sc_lv 1 signal 2 } 
	{ m_axi_gmem2_ARLEN sc_out sc_lv 8 signal 2 } 
	{ m_axi_gmem2_ARSIZE sc_out sc_lv 3 signal 2 } 
	{ m_axi_gmem2_ARBURST sc_out sc_lv 2 signal 2 } 
	{ m_axi_gmem2_ARLOCK sc_out sc_lv 2 signal 2 } 
	{ m_axi_gmem2_ARCACHE sc_out sc_lv 4 signal 2 } 
	{ m_axi_gmem2_ARPROT sc_out sc_lv 3 signal 2 } 
	{ m_axi_gmem2_ARQOS sc_out sc_lv 4 signal 2 } 
	{ m_axi_gmem2_ARREGION sc_out sc_lv 4 signal 2 } 
	{ m_axi_gmem2_ARUSER sc_out sc_lv 1 signal 2 } 
	{ m_axi_gmem2_RVALID sc_in sc_logic 1 signal 2 } 
	{ m_axi_gmem2_RREADY sc_out sc_logic 1 signal 2 } 
	{ m_axi_gmem2_RDATA sc_in sc_lv 64 signal 2 } 
	{ m_axi_gmem2_RLAST sc_in sc_logic 1 signal 2 } 
	{ m_axi_gmem2_RID sc_in sc_lv 1 signal 2 } 
	{ m_axi_gmem2_RUSER sc_in sc_lv 1 signal 2 } 
	{ m_axi_gmem2_RRESP sc_in sc_lv 2 signal 2 } 
	{ m_axi_gmem2_BVALID sc_in sc_logic 1 signal 2 } 
	{ m_axi_gmem2_BREADY sc_out sc_logic 1 signal 2 } 
	{ m_axi_gmem2_BRESP sc_in sc_lv 2 signal 2 } 
	{ m_axi_gmem2_BID sc_in sc_lv 1 signal 2 } 
	{ m_axi_gmem2_BUSER sc_in sc_lv 1 signal 2 } 
	{ s_axi_control_AWVALID sc_in sc_logic 1 signal -1 } 
	{ s_axi_control_AWREADY sc_out sc_logic 1 signal -1 } 
	{ s_axi_control_AWADDR sc_in sc_lv 6 signal -1 } 
	{ s_axi_control_WVALID sc_in sc_logic 1 signal -1 } 
	{ s_axi_control_WREADY sc_out sc_logic 1 signal -1 } 
	{ s_axi_control_WDATA sc_in sc_lv 32 signal -1 } 
	{ s_axi_control_WSTRB sc_in sc_lv 4 signal -1 } 
	{ s_axi_control_ARVALID sc_in sc_logic 1 signal -1 } 
	{ s_axi_control_ARREADY sc_out sc_logic 1 signal -1 } 
	{ s_axi_control_ARADDR sc_in sc_lv 6 signal -1 } 
	{ s_axi_control_RVALID sc_out sc_logic 1 signal -1 } 
	{ s_axi_control_RREADY sc_in sc_logic 1 signal -1 } 
	{ s_axi_control_RDATA sc_out sc_lv 32 signal -1 } 
	{ s_axi_control_RRESP sc_out sc_lv 2 signal -1 } 
	{ s_axi_control_BVALID sc_out sc_logic 1 signal -1 } 
	{ s_axi_control_BREADY sc_in sc_logic 1 signal -1 } 
	{ s_axi_control_BRESP sc_out sc_lv 2 signal -1 } 
	{ interrupt sc_out sc_logic 1 signal -1 } 
}
set NewPortList {[ 
	{ "name": "s_axi_control_AWADDR", "direction": "in", "datatype": "sc_lv", "bitwidth":6, "type": "signal", "bundle":{"name": "control", "role": "AWADDR" },"address":[{"name":"topkernel","role":"start","value":"0","valid_bit":"0"},{"name":"topkernel","role":"continue","value":"0","valid_bit":"4"},{"name":"topkernel","role":"auto_start","value":"0","valid_bit":"7"},{"name":"kvsourcedramA_V","role":"data","value":"16"},{"name":"kvdestdramA_V","role":"data","value":"24"},{"name":"kvstatsA","role":"data","value":"32"}] },
	{ "name": "s_axi_control_AWVALID", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "control", "role": "AWVALID" } },
	{ "name": "s_axi_control_AWREADY", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "control", "role": "AWREADY" } },
	{ "name": "s_axi_control_WVALID", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "control", "role": "WVALID" } },
	{ "name": "s_axi_control_WREADY", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "control", "role": "WREADY" } },
	{ "name": "s_axi_control_WDATA", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "control", "role": "WDATA" } },
	{ "name": "s_axi_control_WSTRB", "direction": "in", "datatype": "sc_lv", "bitwidth":4, "type": "signal", "bundle":{"name": "control", "role": "WSTRB" } },
	{ "name": "s_axi_control_ARADDR", "direction": "in", "datatype": "sc_lv", "bitwidth":6, "type": "signal", "bundle":{"name": "control", "role": "ARADDR" },"address":[{"name":"topkernel","role":"start","value":"0","valid_bit":"0"},{"name":"topkernel","role":"done","value":"0","valid_bit":"1"},{"name":"topkernel","role":"idle","value":"0","valid_bit":"2"},{"name":"topkernel","role":"ready","value":"0","valid_bit":"3"},{"name":"topkernel","role":"auto_start","value":"0","valid_bit":"7"}] },
	{ "name": "s_axi_control_ARVALID", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "control", "role": "ARVALID" } },
	{ "name": "s_axi_control_ARREADY", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "control", "role": "ARREADY" } },
	{ "name": "s_axi_control_RVALID", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "control", "role": "RVALID" } },
	{ "name": "s_axi_control_RREADY", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "control", "role": "RREADY" } },
	{ "name": "s_axi_control_RDATA", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "control", "role": "RDATA" } },
	{ "name": "s_axi_control_RRESP", "direction": "out", "datatype": "sc_lv", "bitwidth":2, "type": "signal", "bundle":{"name": "control", "role": "RRESP" } },
	{ "name": "s_axi_control_BVALID", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "control", "role": "BVALID" } },
	{ "name": "s_axi_control_BREADY", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "control", "role": "BREADY" } },
	{ "name": "s_axi_control_BRESP", "direction": "out", "datatype": "sc_lv", "bitwidth":2, "type": "signal", "bundle":{"name": "control", "role": "BRESP" } },
	{ "name": "interrupt", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "control", "role": "interrupt" } }, 
 	{ "name": "ap_clk", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "clock", "bundle":{"name": "ap_clk", "role": "default" }} , 
 	{ "name": "ap_rst_n", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "reset", "bundle":{"name": "ap_rst_n", "role": "default" }} , 
 	{ "name": "m_axi_gmem0_AWVALID", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "gmem0", "role": "AWVALID" }} , 
 	{ "name": "m_axi_gmem0_AWREADY", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "gmem0", "role": "AWREADY" }} , 
 	{ "name": "m_axi_gmem0_AWADDR", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "gmem0", "role": "AWADDR" }} , 
 	{ "name": "m_axi_gmem0_AWID", "direction": "out", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "gmem0", "role": "AWID" }} , 
 	{ "name": "m_axi_gmem0_AWLEN", "direction": "out", "datatype": "sc_lv", "bitwidth":8, "type": "signal", "bundle":{"name": "gmem0", "role": "AWLEN" }} , 
 	{ "name": "m_axi_gmem0_AWSIZE", "direction": "out", "datatype": "sc_lv", "bitwidth":3, "type": "signal", "bundle":{"name": "gmem0", "role": "AWSIZE" }} , 
 	{ "name": "m_axi_gmem0_AWBURST", "direction": "out", "datatype": "sc_lv", "bitwidth":2, "type": "signal", "bundle":{"name": "gmem0", "role": "AWBURST" }} , 
 	{ "name": "m_axi_gmem0_AWLOCK", "direction": "out", "datatype": "sc_lv", "bitwidth":2, "type": "signal", "bundle":{"name": "gmem0", "role": "AWLOCK" }} , 
 	{ "name": "m_axi_gmem0_AWCACHE", "direction": "out", "datatype": "sc_lv", "bitwidth":4, "type": "signal", "bundle":{"name": "gmem0", "role": "AWCACHE" }} , 
 	{ "name": "m_axi_gmem0_AWPROT", "direction": "out", "datatype": "sc_lv", "bitwidth":3, "type": "signal", "bundle":{"name": "gmem0", "role": "AWPROT" }} , 
 	{ "name": "m_axi_gmem0_AWQOS", "direction": "out", "datatype": "sc_lv", "bitwidth":4, "type": "signal", "bundle":{"name": "gmem0", "role": "AWQOS" }} , 
 	{ "name": "m_axi_gmem0_AWREGION", "direction": "out", "datatype": "sc_lv", "bitwidth":4, "type": "signal", "bundle":{"name": "gmem0", "role": "AWREGION" }} , 
 	{ "name": "m_axi_gmem0_AWUSER", "direction": "out", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "gmem0", "role": "AWUSER" }} , 
 	{ "name": "m_axi_gmem0_WVALID", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "gmem0", "role": "WVALID" }} , 
 	{ "name": "m_axi_gmem0_WREADY", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "gmem0", "role": "WREADY" }} , 
 	{ "name": "m_axi_gmem0_WDATA", "direction": "out", "datatype": "sc_lv", "bitwidth":512, "type": "signal", "bundle":{"name": "gmem0", "role": "WDATA" }} , 
 	{ "name": "m_axi_gmem0_WSTRB", "direction": "out", "datatype": "sc_lv", "bitwidth":64, "type": "signal", "bundle":{"name": "gmem0", "role": "WSTRB" }} , 
 	{ "name": "m_axi_gmem0_WLAST", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "gmem0", "role": "WLAST" }} , 
 	{ "name": "m_axi_gmem0_WID", "direction": "out", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "gmem0", "role": "WID" }} , 
 	{ "name": "m_axi_gmem0_WUSER", "direction": "out", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "gmem0", "role": "WUSER" }} , 
 	{ "name": "m_axi_gmem0_ARVALID", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "gmem0", "role": "ARVALID" }} , 
 	{ "name": "m_axi_gmem0_ARREADY", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "gmem0", "role": "ARREADY" }} , 
 	{ "name": "m_axi_gmem0_ARADDR", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "gmem0", "role": "ARADDR" }} , 
 	{ "name": "m_axi_gmem0_ARID", "direction": "out", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "gmem0", "role": "ARID" }} , 
 	{ "name": "m_axi_gmem0_ARLEN", "direction": "out", "datatype": "sc_lv", "bitwidth":8, "type": "signal", "bundle":{"name": "gmem0", "role": "ARLEN" }} , 
 	{ "name": "m_axi_gmem0_ARSIZE", "direction": "out", "datatype": "sc_lv", "bitwidth":3, "type": "signal", "bundle":{"name": "gmem0", "role": "ARSIZE" }} , 
 	{ "name": "m_axi_gmem0_ARBURST", "direction": "out", "datatype": "sc_lv", "bitwidth":2, "type": "signal", "bundle":{"name": "gmem0", "role": "ARBURST" }} , 
 	{ "name": "m_axi_gmem0_ARLOCK", "direction": "out", "datatype": "sc_lv", "bitwidth":2, "type": "signal", "bundle":{"name": "gmem0", "role": "ARLOCK" }} , 
 	{ "name": "m_axi_gmem0_ARCACHE", "direction": "out", "datatype": "sc_lv", "bitwidth":4, "type": "signal", "bundle":{"name": "gmem0", "role": "ARCACHE" }} , 
 	{ "name": "m_axi_gmem0_ARPROT", "direction": "out", "datatype": "sc_lv", "bitwidth":3, "type": "signal", "bundle":{"name": "gmem0", "role": "ARPROT" }} , 
 	{ "name": "m_axi_gmem0_ARQOS", "direction": "out", "datatype": "sc_lv", "bitwidth":4, "type": "signal", "bundle":{"name": "gmem0", "role": "ARQOS" }} , 
 	{ "name": "m_axi_gmem0_ARREGION", "direction": "out", "datatype": "sc_lv", "bitwidth":4, "type": "signal", "bundle":{"name": "gmem0", "role": "ARREGION" }} , 
 	{ "name": "m_axi_gmem0_ARUSER", "direction": "out", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "gmem0", "role": "ARUSER" }} , 
 	{ "name": "m_axi_gmem0_RVALID", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "gmem0", "role": "RVALID" }} , 
 	{ "name": "m_axi_gmem0_RREADY", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "gmem0", "role": "RREADY" }} , 
 	{ "name": "m_axi_gmem0_RDATA", "direction": "in", "datatype": "sc_lv", "bitwidth":512, "type": "signal", "bundle":{"name": "gmem0", "role": "RDATA" }} , 
 	{ "name": "m_axi_gmem0_RLAST", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "gmem0", "role": "RLAST" }} , 
 	{ "name": "m_axi_gmem0_RID", "direction": "in", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "gmem0", "role": "RID" }} , 
 	{ "name": "m_axi_gmem0_RUSER", "direction": "in", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "gmem0", "role": "RUSER" }} , 
 	{ "name": "m_axi_gmem0_RRESP", "direction": "in", "datatype": "sc_lv", "bitwidth":2, "type": "signal", "bundle":{"name": "gmem0", "role": "RRESP" }} , 
 	{ "name": "m_axi_gmem0_BVALID", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "gmem0", "role": "BVALID" }} , 
 	{ "name": "m_axi_gmem0_BREADY", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "gmem0", "role": "BREADY" }} , 
 	{ "name": "m_axi_gmem0_BRESP", "direction": "in", "datatype": "sc_lv", "bitwidth":2, "type": "signal", "bundle":{"name": "gmem0", "role": "BRESP" }} , 
 	{ "name": "m_axi_gmem0_BID", "direction": "in", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "gmem0", "role": "BID" }} , 
 	{ "name": "m_axi_gmem0_BUSER", "direction": "in", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "gmem0", "role": "BUSER" }} , 
 	{ "name": "m_axi_gmem1_AWVALID", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "gmem1", "role": "AWVALID" }} , 
 	{ "name": "m_axi_gmem1_AWREADY", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "gmem1", "role": "AWREADY" }} , 
 	{ "name": "m_axi_gmem1_AWADDR", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "gmem1", "role": "AWADDR" }} , 
 	{ "name": "m_axi_gmem1_AWID", "direction": "out", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "gmem1", "role": "AWID" }} , 
 	{ "name": "m_axi_gmem1_AWLEN", "direction": "out", "datatype": "sc_lv", "bitwidth":8, "type": "signal", "bundle":{"name": "gmem1", "role": "AWLEN" }} , 
 	{ "name": "m_axi_gmem1_AWSIZE", "direction": "out", "datatype": "sc_lv", "bitwidth":3, "type": "signal", "bundle":{"name": "gmem1", "role": "AWSIZE" }} , 
 	{ "name": "m_axi_gmem1_AWBURST", "direction": "out", "datatype": "sc_lv", "bitwidth":2, "type": "signal", "bundle":{"name": "gmem1", "role": "AWBURST" }} , 
 	{ "name": "m_axi_gmem1_AWLOCK", "direction": "out", "datatype": "sc_lv", "bitwidth":2, "type": "signal", "bundle":{"name": "gmem1", "role": "AWLOCK" }} , 
 	{ "name": "m_axi_gmem1_AWCACHE", "direction": "out", "datatype": "sc_lv", "bitwidth":4, "type": "signal", "bundle":{"name": "gmem1", "role": "AWCACHE" }} , 
 	{ "name": "m_axi_gmem1_AWPROT", "direction": "out", "datatype": "sc_lv", "bitwidth":3, "type": "signal", "bundle":{"name": "gmem1", "role": "AWPROT" }} , 
 	{ "name": "m_axi_gmem1_AWQOS", "direction": "out", "datatype": "sc_lv", "bitwidth":4, "type": "signal", "bundle":{"name": "gmem1", "role": "AWQOS" }} , 
 	{ "name": "m_axi_gmem1_AWREGION", "direction": "out", "datatype": "sc_lv", "bitwidth":4, "type": "signal", "bundle":{"name": "gmem1", "role": "AWREGION" }} , 
 	{ "name": "m_axi_gmem1_AWUSER", "direction": "out", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "gmem1", "role": "AWUSER" }} , 
 	{ "name": "m_axi_gmem1_WVALID", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "gmem1", "role": "WVALID" }} , 
 	{ "name": "m_axi_gmem1_WREADY", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "gmem1", "role": "WREADY" }} , 
 	{ "name": "m_axi_gmem1_WDATA", "direction": "out", "datatype": "sc_lv", "bitwidth":512, "type": "signal", "bundle":{"name": "gmem1", "role": "WDATA" }} , 
 	{ "name": "m_axi_gmem1_WSTRB", "direction": "out", "datatype": "sc_lv", "bitwidth":64, "type": "signal", "bundle":{"name": "gmem1", "role": "WSTRB" }} , 
 	{ "name": "m_axi_gmem1_WLAST", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "gmem1", "role": "WLAST" }} , 
 	{ "name": "m_axi_gmem1_WID", "direction": "out", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "gmem1", "role": "WID" }} , 
 	{ "name": "m_axi_gmem1_WUSER", "direction": "out", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "gmem1", "role": "WUSER" }} , 
 	{ "name": "m_axi_gmem1_ARVALID", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "gmem1", "role": "ARVALID" }} , 
 	{ "name": "m_axi_gmem1_ARREADY", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "gmem1", "role": "ARREADY" }} , 
 	{ "name": "m_axi_gmem1_ARADDR", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "gmem1", "role": "ARADDR" }} , 
 	{ "name": "m_axi_gmem1_ARID", "direction": "out", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "gmem1", "role": "ARID" }} , 
 	{ "name": "m_axi_gmem1_ARLEN", "direction": "out", "datatype": "sc_lv", "bitwidth":8, "type": "signal", "bundle":{"name": "gmem1", "role": "ARLEN" }} , 
 	{ "name": "m_axi_gmem1_ARSIZE", "direction": "out", "datatype": "sc_lv", "bitwidth":3, "type": "signal", "bundle":{"name": "gmem1", "role": "ARSIZE" }} , 
 	{ "name": "m_axi_gmem1_ARBURST", "direction": "out", "datatype": "sc_lv", "bitwidth":2, "type": "signal", "bundle":{"name": "gmem1", "role": "ARBURST" }} , 
 	{ "name": "m_axi_gmem1_ARLOCK", "direction": "out", "datatype": "sc_lv", "bitwidth":2, "type": "signal", "bundle":{"name": "gmem1", "role": "ARLOCK" }} , 
 	{ "name": "m_axi_gmem1_ARCACHE", "direction": "out", "datatype": "sc_lv", "bitwidth":4, "type": "signal", "bundle":{"name": "gmem1", "role": "ARCACHE" }} , 
 	{ "name": "m_axi_gmem1_ARPROT", "direction": "out", "datatype": "sc_lv", "bitwidth":3, "type": "signal", "bundle":{"name": "gmem1", "role": "ARPROT" }} , 
 	{ "name": "m_axi_gmem1_ARQOS", "direction": "out", "datatype": "sc_lv", "bitwidth":4, "type": "signal", "bundle":{"name": "gmem1", "role": "ARQOS" }} , 
 	{ "name": "m_axi_gmem1_ARREGION", "direction": "out", "datatype": "sc_lv", "bitwidth":4, "type": "signal", "bundle":{"name": "gmem1", "role": "ARREGION" }} , 
 	{ "name": "m_axi_gmem1_ARUSER", "direction": "out", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "gmem1", "role": "ARUSER" }} , 
 	{ "name": "m_axi_gmem1_RVALID", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "gmem1", "role": "RVALID" }} , 
 	{ "name": "m_axi_gmem1_RREADY", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "gmem1", "role": "RREADY" }} , 
 	{ "name": "m_axi_gmem1_RDATA", "direction": "in", "datatype": "sc_lv", "bitwidth":512, "type": "signal", "bundle":{"name": "gmem1", "role": "RDATA" }} , 
 	{ "name": "m_axi_gmem1_RLAST", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "gmem1", "role": "RLAST" }} , 
 	{ "name": "m_axi_gmem1_RID", "direction": "in", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "gmem1", "role": "RID" }} , 
 	{ "name": "m_axi_gmem1_RUSER", "direction": "in", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "gmem1", "role": "RUSER" }} , 
 	{ "name": "m_axi_gmem1_RRESP", "direction": "in", "datatype": "sc_lv", "bitwidth":2, "type": "signal", "bundle":{"name": "gmem1", "role": "RRESP" }} , 
 	{ "name": "m_axi_gmem1_BVALID", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "gmem1", "role": "BVALID" }} , 
 	{ "name": "m_axi_gmem1_BREADY", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "gmem1", "role": "BREADY" }} , 
 	{ "name": "m_axi_gmem1_BRESP", "direction": "in", "datatype": "sc_lv", "bitwidth":2, "type": "signal", "bundle":{"name": "gmem1", "role": "BRESP" }} , 
 	{ "name": "m_axi_gmem1_BID", "direction": "in", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "gmem1", "role": "BID" }} , 
 	{ "name": "m_axi_gmem1_BUSER", "direction": "in", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "gmem1", "role": "BUSER" }} , 
 	{ "name": "m_axi_gmem2_AWVALID", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "gmem2", "role": "AWVALID" }} , 
 	{ "name": "m_axi_gmem2_AWREADY", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "gmem2", "role": "AWREADY" }} , 
 	{ "name": "m_axi_gmem2_AWADDR", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "gmem2", "role": "AWADDR" }} , 
 	{ "name": "m_axi_gmem2_AWID", "direction": "out", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "gmem2", "role": "AWID" }} , 
 	{ "name": "m_axi_gmem2_AWLEN", "direction": "out", "datatype": "sc_lv", "bitwidth":8, "type": "signal", "bundle":{"name": "gmem2", "role": "AWLEN" }} , 
 	{ "name": "m_axi_gmem2_AWSIZE", "direction": "out", "datatype": "sc_lv", "bitwidth":3, "type": "signal", "bundle":{"name": "gmem2", "role": "AWSIZE" }} , 
 	{ "name": "m_axi_gmem2_AWBURST", "direction": "out", "datatype": "sc_lv", "bitwidth":2, "type": "signal", "bundle":{"name": "gmem2", "role": "AWBURST" }} , 
 	{ "name": "m_axi_gmem2_AWLOCK", "direction": "out", "datatype": "sc_lv", "bitwidth":2, "type": "signal", "bundle":{"name": "gmem2", "role": "AWLOCK" }} , 
 	{ "name": "m_axi_gmem2_AWCACHE", "direction": "out", "datatype": "sc_lv", "bitwidth":4, "type": "signal", "bundle":{"name": "gmem2", "role": "AWCACHE" }} , 
 	{ "name": "m_axi_gmem2_AWPROT", "direction": "out", "datatype": "sc_lv", "bitwidth":3, "type": "signal", "bundle":{"name": "gmem2", "role": "AWPROT" }} , 
 	{ "name": "m_axi_gmem2_AWQOS", "direction": "out", "datatype": "sc_lv", "bitwidth":4, "type": "signal", "bundle":{"name": "gmem2", "role": "AWQOS" }} , 
 	{ "name": "m_axi_gmem2_AWREGION", "direction": "out", "datatype": "sc_lv", "bitwidth":4, "type": "signal", "bundle":{"name": "gmem2", "role": "AWREGION" }} , 
 	{ "name": "m_axi_gmem2_AWUSER", "direction": "out", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "gmem2", "role": "AWUSER" }} , 
 	{ "name": "m_axi_gmem2_WVALID", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "gmem2", "role": "WVALID" }} , 
 	{ "name": "m_axi_gmem2_WREADY", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "gmem2", "role": "WREADY" }} , 
 	{ "name": "m_axi_gmem2_WDATA", "direction": "out", "datatype": "sc_lv", "bitwidth":64, "type": "signal", "bundle":{"name": "gmem2", "role": "WDATA" }} , 
 	{ "name": "m_axi_gmem2_WSTRB", "direction": "out", "datatype": "sc_lv", "bitwidth":8, "type": "signal", "bundle":{"name": "gmem2", "role": "WSTRB" }} , 
 	{ "name": "m_axi_gmem2_WLAST", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "gmem2", "role": "WLAST" }} , 
 	{ "name": "m_axi_gmem2_WID", "direction": "out", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "gmem2", "role": "WID" }} , 
 	{ "name": "m_axi_gmem2_WUSER", "direction": "out", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "gmem2", "role": "WUSER" }} , 
 	{ "name": "m_axi_gmem2_ARVALID", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "gmem2", "role": "ARVALID" }} , 
 	{ "name": "m_axi_gmem2_ARREADY", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "gmem2", "role": "ARREADY" }} , 
 	{ "name": "m_axi_gmem2_ARADDR", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "gmem2", "role": "ARADDR" }} , 
 	{ "name": "m_axi_gmem2_ARID", "direction": "out", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "gmem2", "role": "ARID" }} , 
 	{ "name": "m_axi_gmem2_ARLEN", "direction": "out", "datatype": "sc_lv", "bitwidth":8, "type": "signal", "bundle":{"name": "gmem2", "role": "ARLEN" }} , 
 	{ "name": "m_axi_gmem2_ARSIZE", "direction": "out", "datatype": "sc_lv", "bitwidth":3, "type": "signal", "bundle":{"name": "gmem2", "role": "ARSIZE" }} , 
 	{ "name": "m_axi_gmem2_ARBURST", "direction": "out", "datatype": "sc_lv", "bitwidth":2, "type": "signal", "bundle":{"name": "gmem2", "role": "ARBURST" }} , 
 	{ "name": "m_axi_gmem2_ARLOCK", "direction": "out", "datatype": "sc_lv", "bitwidth":2, "type": "signal", "bundle":{"name": "gmem2", "role": "ARLOCK" }} , 
 	{ "name": "m_axi_gmem2_ARCACHE", "direction": "out", "datatype": "sc_lv", "bitwidth":4, "type": "signal", "bundle":{"name": "gmem2", "role": "ARCACHE" }} , 
 	{ "name": "m_axi_gmem2_ARPROT", "direction": "out", "datatype": "sc_lv", "bitwidth":3, "type": "signal", "bundle":{"name": "gmem2", "role": "ARPROT" }} , 
 	{ "name": "m_axi_gmem2_ARQOS", "direction": "out", "datatype": "sc_lv", "bitwidth":4, "type": "signal", "bundle":{"name": "gmem2", "role": "ARQOS" }} , 
 	{ "name": "m_axi_gmem2_ARREGION", "direction": "out", "datatype": "sc_lv", "bitwidth":4, "type": "signal", "bundle":{"name": "gmem2", "role": "ARREGION" }} , 
 	{ "name": "m_axi_gmem2_ARUSER", "direction": "out", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "gmem2", "role": "ARUSER" }} , 
 	{ "name": "m_axi_gmem2_RVALID", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "gmem2", "role": "RVALID" }} , 
 	{ "name": "m_axi_gmem2_RREADY", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "gmem2", "role": "RREADY" }} , 
 	{ "name": "m_axi_gmem2_RDATA", "direction": "in", "datatype": "sc_lv", "bitwidth":64, "type": "signal", "bundle":{"name": "gmem2", "role": "RDATA" }} , 
 	{ "name": "m_axi_gmem2_RLAST", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "gmem2", "role": "RLAST" }} , 
 	{ "name": "m_axi_gmem2_RID", "direction": "in", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "gmem2", "role": "RID" }} , 
 	{ "name": "m_axi_gmem2_RUSER", "direction": "in", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "gmem2", "role": "RUSER" }} , 
 	{ "name": "m_axi_gmem2_RRESP", "direction": "in", "datatype": "sc_lv", "bitwidth":2, "type": "signal", "bundle":{"name": "gmem2", "role": "RRESP" }} , 
 	{ "name": "m_axi_gmem2_BVALID", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "gmem2", "role": "BVALID" }} , 
 	{ "name": "m_axi_gmem2_BREADY", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "gmem2", "role": "BREADY" }} , 
 	{ "name": "m_axi_gmem2_BRESP", "direction": "in", "datatype": "sc_lv", "bitwidth":2, "type": "signal", "bundle":{"name": "gmem2", "role": "BRESP" }} , 
 	{ "name": "m_axi_gmem2_BID", "direction": "in", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "gmem2", "role": "BID" }} , 
 	{ "name": "m_axi_gmem2_BUSER", "direction": "in", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "gmem2", "role": "BUSER" }}  ]}

set RtlHierarchyInfo {[
	{"ID" : "0", "Level" : "0", "Path" : "`AUTOTB_DUT_INST", "Parent" : "", "Child" : ["1", "2", "3", "4", "5"],
		"CDFG" : "topkernel",
		"Protocol" : "ap_ctrl_hs",
		"ControlExist" : "1", "ap_start" : "1", "ap_ready" : "1", "ap_done" : "1", "ap_continue" : "0", "ap_idle" : "1",
		"Pipeline" : "None", "UnalignedPipeline" : "0", "RewindPipeline" : "0", "ProcessNetwork" : "0",
		"II" : "0",
		"VariableLatency" : "1", "ExactLatency" : "-1", "EstimateLatencyMin" : "57", "EstimateLatencyMax" : "197852404",
		"Combinational" : "0",
		"Datapath" : "0",
		"ClockEnable" : "0",
		"HasSubDataflow" : "0",
		"InDataflowNetwork" : "0",
		"HasNonBlockingOperation" : "0",
		"WaitState" : [
			{"State" : "ap_ST_fsm_state17", "FSM" : "ap_CS_fsm", "SubInstance" : "grp_partitionandreduce0_fu_178"}],
		"Port" : [
			{"Name" : "gmem0", "Type" : "MAXI", "Direction" : "IO",
				"SubConnect" : [
					{"ID" : "5", "SubInstance" : "grp_partitionandreduce0_fu_178", "Port" : "kvsourcedram_V"}]},
			{"Name" : "gmem1", "Type" : "MAXI", "Direction" : "IO",
				"SubConnect" : [
					{"ID" : "5", "SubInstance" : "grp_partitionandreduce0_fu_178", "Port" : "kvdestdram_V"}]},
			{"Name" : "gmem2", "Type" : "MAXI", "Direction" : "IO",
				"BlockSignal" : [
					{"Name" : "gmem2_blk_n_AR", "Type" : "RtlSignal"},
					{"Name" : "gmem2_blk_n_R", "Type" : "RtlSignal"}],
				"SubConnect" : [
					{"ID" : "5", "SubInstance" : "grp_partitionandreduce0_fu_178", "Port" : "kvstats"}]},
			{"Name" : "kvsourcedramA_V", "Type" : "None", "Direction" : "I"},
			{"Name" : "kvdestdramA_V", "Type" : "None", "Direction" : "I"},
			{"Name" : "kvstatsA", "Type" : "None", "Direction" : "I"}]},
	{"ID" : "1", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.topkernel_control_s_axi_U", "Parent" : "0"},
	{"ID" : "2", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.topkernel_gmem0_m_axi_U", "Parent" : "0"},
	{"ID" : "3", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.topkernel_gmem1_m_axi_U", "Parent" : "0"},
	{"ID" : "4", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.topkernel_gmem2_m_axi_U", "Parent" : "0"},
	{"ID" : "5", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178", "Parent" : "0", "Child" : ["6", "7", "8", "9", "10", "11", "12", "13", "14", "15", "16", "17", "18", "19", "20", "21", "22", "23", "24", "25", "26", "27", "28", "29", "30", "31", "32", "45", "174", "183", "232", "234", "236", "238", "240", "241", "242", "243", "244"],
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
					{"ID" : "236", "SubInstance" : "grp_readkeyvalues0_2_fu_1628", "Port" : "kvdram_V"},
					{"ID" : "232", "SubInstance" : "grp_readcapsules0_fu_1575", "Port" : "kvdram_V"},
					{"ID" : "174", "SubInstance" : "grp_savepartitions0_fu_1529", "Port" : "kvdram_V"},
					{"ID" : "238", "SubInstance" : "grp_readkeyvalues0_1_fu_1650", "Port" : "kvdram_V"},
					{"ID" : "234", "SubInstance" : "grp_savecapsules0_fu_1599", "Port" : "kvdram_V"}]},
			{"Name" : "kvsourcedram_V_offset", "Type" : "None", "Direction" : "I"},
			{"Name" : "kvdestdram_V", "Type" : "MAXI", "Direction" : "IO",
				"SubConnect" : [
					{"ID" : "241", "SubInstance" : "grp_readkeyvalues0_157_fu_1676", "Port" : "kvdram_V"},
					{"ID" : "240", "SubInstance" : "grp_savekeyvalues0_153_fu_1667", "Port" : "kvdram_V"}]},
			{"Name" : "kvdestdram_V_offset", "Type" : "None", "Direction" : "I"},
			{"Name" : "kvstats", "Type" : "MAXI", "Direction" : "IO",
				"BlockSignal" : [
					{"Name" : "kvstats_blk_n_AW", "Type" : "RtlSignal"},
					{"Name" : "kvstats_blk_n_W", "Type" : "RtlSignal"},
					{"Name" : "kvstats_blk_n_B", "Type" : "RtlSignal"},
					{"Name" : "kvstats_blk_n_AR", "Type" : "RtlSignal"},
					{"Name" : "kvstats_blk_n_R", "Type" : "RtlSignal"}],
				"SubConnect" : [
					{"ID" : "243", "SubInstance" : "grp_savekeyvalues0_1_fu_1695", "Port" : "dram"},
					{"ID" : "242", "SubInstance" : "grp_savekeyvalues0_2_fu_1685", "Port" : "dram"}]},
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
	{"ID" : "6", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.sourcebuffer00_0_V_U", "Parent" : "5"},
	{"ID" : "7", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.sourcebuffer00_1_V_U", "Parent" : "5"},
	{"ID" : "8", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.destbuffer00_V_U", "Parent" : "5"},
	{"ID" : "9", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.capsule00_key_U", "Parent" : "5"},
	{"ID" : "10", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.capsule00_value_U", "Parent" : "5"},
	{"ID" : "11", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.BIGcapsule00_V_U", "Parent" : "5"},
	{"ID" : "12", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.sourcebuffer01_0_V_U", "Parent" : "5"},
	{"ID" : "13", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.sourcebuffer01_1_V_U", "Parent" : "5"},
	{"ID" : "14", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.destbuffer01_V_U", "Parent" : "5"},
	{"ID" : "15", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.capsule01_key_U", "Parent" : "5"},
	{"ID" : "16", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.capsule01_value_U", "Parent" : "5"},
	{"ID" : "17", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.BIGcapsule01_V_U", "Parent" : "5"},
	{"ID" : "18", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.sourcebuffer02_0_V_U", "Parent" : "5"},
	{"ID" : "19", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.sourcebuffer02_1_V_U", "Parent" : "5"},
	{"ID" : "20", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.destbuffer02_V_U", "Parent" : "5"},
	{"ID" : "21", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.capsule02_key_U", "Parent" : "5"},
	{"ID" : "22", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.capsule02_value_U", "Parent" : "5"},
	{"ID" : "23", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.BIGcapsule02_V_U", "Parent" : "5"},
	{"ID" : "24", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.sourcebuffer03_0_V_U", "Parent" : "5"},
	{"ID" : "25", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.sourcebuffer03_1_V_U", "Parent" : "5"},
	{"ID" : "26", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.destbuffer03_V_U", "Parent" : "5"},
	{"ID" : "27", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.capsule03_key_U", "Parent" : "5"},
	{"ID" : "28", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.capsule03_value_U", "Parent" : "5"},
	{"ID" : "29", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.BIGcapsule03_V_U", "Parent" : "5"},
	{"ID" : "30", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.kvdeststats_tmp_key_U", "Parent" : "5"},
	{"ID" : "31", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.kvdeststats_tmp_valu_U", "Parent" : "5"},
	{"ID" : "32", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_partitionkeyvalues00_fu_1486", "Parent" : "5", "Child" : ["33", "34", "35", "36", "37", "38", "39", "40", "41", "42", "43", "44"],
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
	{"ID" : "33", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_partitionkeyvalues00_fu_1486.topkernel_mux_325_32_1_1_U85", "Parent" : "32"},
	{"ID" : "34", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_partitionkeyvalues00_fu_1486.topkernel_mux_325_32_1_1_U86", "Parent" : "32"},
	{"ID" : "35", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_partitionkeyvalues00_fu_1486.topkernel_mux_325_32_1_1_U87", "Parent" : "32"},
	{"ID" : "36", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_partitionkeyvalues00_fu_1486.topkernel_mux_325_32_1_1_U88", "Parent" : "32"},
	{"ID" : "37", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_partitionkeyvalues00_fu_1486.topkernel_mux_325_32_1_1_U89", "Parent" : "32"},
	{"ID" : "38", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_partitionkeyvalues00_fu_1486.topkernel_mux_325_32_1_1_U90", "Parent" : "32"},
	{"ID" : "39", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_partitionkeyvalues00_fu_1486.topkernel_mux_325_32_1_1_U91", "Parent" : "32"},
	{"ID" : "40", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_partitionkeyvalues00_fu_1486.topkernel_mux_325_32_1_1_U92", "Parent" : "32"},
	{"ID" : "41", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_partitionkeyvalues00_fu_1486.topkernel_mux_325_32_1_1_U93", "Parent" : "32"},
	{"ID" : "42", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_partitionkeyvalues00_fu_1486.topkernel_mux_325_32_1_1_U94", "Parent" : "32"},
	{"ID" : "43", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_partitionkeyvalues00_fu_1486.topkernel_mux_325_32_1_1_U95", "Parent" : "32"},
	{"ID" : "44", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_partitionkeyvalues00_fu_1486.topkernel_mux_325_32_1_1_U96", "Parent" : "32"},
	{"ID" : "45", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1514", "Parent" : "5", "Child" : ["46", "47", "48", "49", "50", "51", "52", "53", "54", "55", "56", "57", "58", "59", "60", "61", "62", "63", "64", "65", "66", "67", "68", "69", "70", "71", "72", "73", "74", "75", "76", "77", "78", "79", "80", "81", "82", "83", "84", "85", "86", "87", "88", "89", "90", "91", "92", "93", "94", "95", "96", "97", "98", "99", "100", "101", "102", "103", "104", "105", "106", "107", "108", "109", "110", "111", "112", "113", "114", "115", "116", "117", "118", "119", "120", "121", "122", "123", "124", "125", "126", "127", "128", "129", "130", "131", "132", "133", "134", "135", "136", "137", "138", "139", "140", "141", "142", "143", "144", "145", "146", "147", "148", "149", "150", "151", "152", "153", "154", "155", "156", "157", "158", "159", "160", "161", "162", "163", "164", "165", "166", "167", "168", "169", "170", "171", "172", "173"],
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
	{"ID" : "46", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1514.destbuffer00_key_U", "Parent" : "45"},
	{"ID" : "47", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1514.destbuffer00_value_U", "Parent" : "45"},
	{"ID" : "48", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1514.destbuffer01_key_U", "Parent" : "45"},
	{"ID" : "49", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1514.destbuffer01_value_U", "Parent" : "45"},
	{"ID" : "50", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1514.destbuffer02_key_U", "Parent" : "45"},
	{"ID" : "51", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1514.destbuffer02_value_U", "Parent" : "45"},
	{"ID" : "52", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1514.destbuffer03_key_U", "Parent" : "45"},
	{"ID" : "53", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1514.destbuffer03_value_U", "Parent" : "45"},
	{"ID" : "54", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1514.destbuffer04_key_U", "Parent" : "45"},
	{"ID" : "55", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1514.destbuffer04_value_U", "Parent" : "45"},
	{"ID" : "56", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1514.destbuffer05_key_U", "Parent" : "45"},
	{"ID" : "57", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1514.destbuffer05_value_U", "Parent" : "45"},
	{"ID" : "58", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1514.destbuffer06_key_U", "Parent" : "45"},
	{"ID" : "59", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1514.destbuffer06_value_U", "Parent" : "45"},
	{"ID" : "60", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1514.destbuffer07_key_U", "Parent" : "45"},
	{"ID" : "61", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1514.destbuffer07_value_U", "Parent" : "45"},
	{"ID" : "62", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1514.destbuffer10_key_U", "Parent" : "45"},
	{"ID" : "63", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1514.destbuffer10_value_U", "Parent" : "45"},
	{"ID" : "64", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1514.destbuffer11_key_U", "Parent" : "45"},
	{"ID" : "65", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1514.destbuffer11_value_U", "Parent" : "45"},
	{"ID" : "66", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1514.destbuffer12_key_U", "Parent" : "45"},
	{"ID" : "67", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1514.destbuffer12_value_U", "Parent" : "45"},
	{"ID" : "68", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1514.destbuffer13_key_U", "Parent" : "45"},
	{"ID" : "69", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1514.destbuffer13_value_U", "Parent" : "45"},
	{"ID" : "70", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1514.destbuffer14_key_U", "Parent" : "45"},
	{"ID" : "71", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1514.destbuffer14_value_U", "Parent" : "45"},
	{"ID" : "72", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1514.destbuffer15_key_U", "Parent" : "45"},
	{"ID" : "73", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1514.destbuffer15_value_U", "Parent" : "45"},
	{"ID" : "74", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1514.destbuffer16_key_U", "Parent" : "45"},
	{"ID" : "75", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1514.destbuffer16_value_U", "Parent" : "45"},
	{"ID" : "76", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1514.destbuffer17_key_U", "Parent" : "45"},
	{"ID" : "77", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1514.destbuffer17_value_U", "Parent" : "45"},
	{"ID" : "78", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1514.destbuffer20_key_U", "Parent" : "45"},
	{"ID" : "79", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1514.destbuffer20_value_U", "Parent" : "45"},
	{"ID" : "80", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1514.destbuffer21_key_U", "Parent" : "45"},
	{"ID" : "81", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1514.destbuffer21_value_U", "Parent" : "45"},
	{"ID" : "82", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1514.destbuffer22_key_U", "Parent" : "45"},
	{"ID" : "83", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1514.destbuffer22_value_U", "Parent" : "45"},
	{"ID" : "84", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1514.destbuffer23_key_U", "Parent" : "45"},
	{"ID" : "85", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1514.destbuffer23_value_U", "Parent" : "45"},
	{"ID" : "86", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1514.destbuffer24_key_U", "Parent" : "45"},
	{"ID" : "87", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1514.destbuffer24_value_U", "Parent" : "45"},
	{"ID" : "88", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1514.destbuffer25_key_U", "Parent" : "45"},
	{"ID" : "89", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1514.destbuffer25_value_U", "Parent" : "45"},
	{"ID" : "90", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1514.destbuffer26_key_U", "Parent" : "45"},
	{"ID" : "91", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1514.destbuffer26_value_U", "Parent" : "45"},
	{"ID" : "92", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1514.destbuffer27_key_U", "Parent" : "45"},
	{"ID" : "93", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1514.destbuffer27_value_U", "Parent" : "45"},
	{"ID" : "94", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1514.destbuffer30_key_U", "Parent" : "45"},
	{"ID" : "95", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1514.destbuffer30_value_U", "Parent" : "45"},
	{"ID" : "96", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1514.destbuffer31_key_U", "Parent" : "45"},
	{"ID" : "97", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1514.destbuffer31_value_U", "Parent" : "45"},
	{"ID" : "98", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1514.destbuffer32_key_U", "Parent" : "45"},
	{"ID" : "99", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1514.destbuffer32_value_U", "Parent" : "45"},
	{"ID" : "100", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1514.destbuffer33_key_U", "Parent" : "45"},
	{"ID" : "101", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1514.destbuffer33_value_U", "Parent" : "45"},
	{"ID" : "102", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1514.destbuffer34_key_U", "Parent" : "45"},
	{"ID" : "103", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1514.destbuffer34_value_U", "Parent" : "45"},
	{"ID" : "104", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1514.destbuffer35_key_U", "Parent" : "45"},
	{"ID" : "105", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1514.destbuffer35_value_U", "Parent" : "45"},
	{"ID" : "106", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1514.destbuffer36_key_U", "Parent" : "45"},
	{"ID" : "107", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1514.destbuffer36_value_U", "Parent" : "45"},
	{"ID" : "108", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1514.destbuffer37_key_U", "Parent" : "45"},
	{"ID" : "109", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1514.destbuffer37_value_U", "Parent" : "45"},
	{"ID" : "110", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1514.Vtemp0_0_key_U", "Parent" : "45"},
	{"ID" : "111", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1514.Vtemp0_1_key_U", "Parent" : "45"},
	{"ID" : "112", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1514.Vtemp0_2_key_U", "Parent" : "45"},
	{"ID" : "113", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1514.Vtemp0_3_key_U", "Parent" : "45"},
	{"ID" : "114", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1514.Vtemp0_4_key_U", "Parent" : "45"},
	{"ID" : "115", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1514.Vtemp0_5_key_U", "Parent" : "45"},
	{"ID" : "116", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1514.Vtemp0_6_key_U", "Parent" : "45"},
	{"ID" : "117", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1514.Vtemp0_7_key_U", "Parent" : "45"},
	{"ID" : "118", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1514.Vtemp0_0_value_U", "Parent" : "45"},
	{"ID" : "119", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1514.Vtemp0_1_value_U", "Parent" : "45"},
	{"ID" : "120", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1514.Vtemp0_2_value_U", "Parent" : "45"},
	{"ID" : "121", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1514.Vtemp0_3_value_U", "Parent" : "45"},
	{"ID" : "122", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1514.Vtemp0_4_value_U", "Parent" : "45"},
	{"ID" : "123", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1514.Vtemp0_5_value_U", "Parent" : "45"},
	{"ID" : "124", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1514.Vtemp0_6_value_U", "Parent" : "45"},
	{"ID" : "125", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1514.Vtemp0_7_value_U", "Parent" : "45"},
	{"ID" : "126", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1514.Vtemp1_0_key_U", "Parent" : "45"},
	{"ID" : "127", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1514.Vtemp1_1_key_U", "Parent" : "45"},
	{"ID" : "128", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1514.Vtemp1_2_key_U", "Parent" : "45"},
	{"ID" : "129", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1514.Vtemp1_3_key_U", "Parent" : "45"},
	{"ID" : "130", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1514.Vtemp1_4_key_U", "Parent" : "45"},
	{"ID" : "131", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1514.Vtemp1_5_key_U", "Parent" : "45"},
	{"ID" : "132", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1514.Vtemp1_6_key_U", "Parent" : "45"},
	{"ID" : "133", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1514.Vtemp1_7_key_U", "Parent" : "45"},
	{"ID" : "134", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1514.Vtemp1_0_value_U", "Parent" : "45"},
	{"ID" : "135", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1514.Vtemp1_1_value_U", "Parent" : "45"},
	{"ID" : "136", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1514.Vtemp1_2_value_U", "Parent" : "45"},
	{"ID" : "137", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1514.Vtemp1_3_value_U", "Parent" : "45"},
	{"ID" : "138", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1514.Vtemp1_4_value_U", "Parent" : "45"},
	{"ID" : "139", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1514.Vtemp1_5_value_U", "Parent" : "45"},
	{"ID" : "140", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1514.Vtemp1_6_value_U", "Parent" : "45"},
	{"ID" : "141", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1514.Vtemp1_7_value_U", "Parent" : "45"},
	{"ID" : "142", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1514.Vtemp2_0_key_U", "Parent" : "45"},
	{"ID" : "143", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1514.Vtemp2_1_key_U", "Parent" : "45"},
	{"ID" : "144", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1514.Vtemp2_2_key_U", "Parent" : "45"},
	{"ID" : "145", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1514.Vtemp2_3_key_U", "Parent" : "45"},
	{"ID" : "146", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1514.Vtemp2_4_key_U", "Parent" : "45"},
	{"ID" : "147", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1514.Vtemp2_5_key_U", "Parent" : "45"},
	{"ID" : "148", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1514.Vtemp2_6_key_U", "Parent" : "45"},
	{"ID" : "149", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1514.Vtemp2_7_key_U", "Parent" : "45"},
	{"ID" : "150", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1514.Vtemp2_0_value_U", "Parent" : "45"},
	{"ID" : "151", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1514.Vtemp2_1_value_U", "Parent" : "45"},
	{"ID" : "152", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1514.Vtemp2_2_value_U", "Parent" : "45"},
	{"ID" : "153", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1514.Vtemp2_3_value_U", "Parent" : "45"},
	{"ID" : "154", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1514.Vtemp2_4_value_U", "Parent" : "45"},
	{"ID" : "155", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1514.Vtemp2_5_value_U", "Parent" : "45"},
	{"ID" : "156", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1514.Vtemp2_6_value_U", "Parent" : "45"},
	{"ID" : "157", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1514.Vtemp2_7_value_U", "Parent" : "45"},
	{"ID" : "158", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1514.Vtemp3_0_key_U", "Parent" : "45"},
	{"ID" : "159", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1514.Vtemp3_1_key_U", "Parent" : "45"},
	{"ID" : "160", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1514.Vtemp3_2_key_U", "Parent" : "45"},
	{"ID" : "161", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1514.Vtemp3_3_key_U", "Parent" : "45"},
	{"ID" : "162", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1514.Vtemp3_4_key_U", "Parent" : "45"},
	{"ID" : "163", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1514.Vtemp3_5_key_U", "Parent" : "45"},
	{"ID" : "164", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1514.Vtemp3_6_key_U", "Parent" : "45"},
	{"ID" : "165", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1514.Vtemp3_7_key_U", "Parent" : "45"},
	{"ID" : "166", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1514.Vtemp3_0_value_U", "Parent" : "45"},
	{"ID" : "167", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1514.Vtemp3_1_value_U", "Parent" : "45"},
	{"ID" : "168", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1514.Vtemp3_2_value_U", "Parent" : "45"},
	{"ID" : "169", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1514.Vtemp3_3_value_U", "Parent" : "45"},
	{"ID" : "170", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1514.Vtemp3_4_value_U", "Parent" : "45"},
	{"ID" : "171", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1514.Vtemp3_5_value_U", "Parent" : "45"},
	{"ID" : "172", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1514.Vtemp3_6_value_U", "Parent" : "45"},
	{"ID" : "173", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1514.Vtemp3_7_value_U", "Parent" : "45"},
	{"ID" : "174", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_savepartitions0_fu_1529", "Parent" : "5", "Child" : ["175", "176", "177", "178", "179", "180", "181", "182"],
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
	{"ID" : "175", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_savepartitions0_fu_1529.topkernel_urem_12ns_8ns_12_16_1_U119", "Parent" : "174"},
	{"ID" : "176", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_savepartitions0_fu_1529.topkernel_urem_12ns_8ns_12_16_1_U120", "Parent" : "174"},
	{"ID" : "177", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_savepartitions0_fu_1529.topkernel_urem_12ns_8ns_12_16_1_U121", "Parent" : "174"},
	{"ID" : "178", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_savepartitions0_fu_1529.topkernel_urem_12ns_8ns_12_16_1_U122", "Parent" : "174"},
	{"ID" : "179", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_savepartitions0_fu_1529.topkernel_urem_12ns_8ns_12_16_1_U123", "Parent" : "174"},
	{"ID" : "180", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_savepartitions0_fu_1529.topkernel_urem_12ns_8ns_12_16_1_U124", "Parent" : "174"},
	{"ID" : "181", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_savepartitions0_fu_1529.topkernel_urem_12ns_8ns_12_16_1_U125", "Parent" : "174"},
	{"ID" : "182", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_savepartitions0_fu_1529.topkernel_urem_12ns_8ns_12_16_1_U126", "Parent" : "174"},
	{"ID" : "183", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_collectstats00_fu_1551", "Parent" : "5", "Child" : ["184", "185", "186", "187", "188", "189", "190", "191", "192", "193", "194", "195", "196", "197", "198", "199", "200", "201", "202", "203", "204", "205", "206", "207", "208", "209", "210", "211", "212", "213", "214", "215", "216", "217", "218", "219", "220", "221", "222", "223", "224", "225", "226", "227", "228", "229", "230", "231"],
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
	{"ID" : "184", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_collectstats00_fu_1551.sizes00_U", "Parent" : "183"},
	{"ID" : "185", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_collectstats00_fu_1551.sizes01_U", "Parent" : "183"},
	{"ID" : "186", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_collectstats00_fu_1551.sizes02_U", "Parent" : "183"},
	{"ID" : "187", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_collectstats00_fu_1551.sizes03_U", "Parent" : "183"},
	{"ID" : "188", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_collectstats00_fu_1551.sizes04_U", "Parent" : "183"},
	{"ID" : "189", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_collectstats00_fu_1551.sizes05_U", "Parent" : "183"},
	{"ID" : "190", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_collectstats00_fu_1551.sizes06_U", "Parent" : "183"},
	{"ID" : "191", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_collectstats00_fu_1551.sizes07_U", "Parent" : "183"},
	{"ID" : "192", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_collectstats00_fu_1551.sizes10_U", "Parent" : "183"},
	{"ID" : "193", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_collectstats00_fu_1551.sizes11_U", "Parent" : "183"},
	{"ID" : "194", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_collectstats00_fu_1551.sizes12_U", "Parent" : "183"},
	{"ID" : "195", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_collectstats00_fu_1551.sizes13_U", "Parent" : "183"},
	{"ID" : "196", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_collectstats00_fu_1551.sizes14_U", "Parent" : "183"},
	{"ID" : "197", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_collectstats00_fu_1551.sizes15_U", "Parent" : "183"},
	{"ID" : "198", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_collectstats00_fu_1551.sizes16_U", "Parent" : "183"},
	{"ID" : "199", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_collectstats00_fu_1551.sizes17_U", "Parent" : "183"},
	{"ID" : "200", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_collectstats00_fu_1551.sizes20_U", "Parent" : "183"},
	{"ID" : "201", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_collectstats00_fu_1551.sizes21_U", "Parent" : "183"},
	{"ID" : "202", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_collectstats00_fu_1551.sizes22_U", "Parent" : "183"},
	{"ID" : "203", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_collectstats00_fu_1551.sizes23_U", "Parent" : "183"},
	{"ID" : "204", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_collectstats00_fu_1551.sizes24_U", "Parent" : "183"},
	{"ID" : "205", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_collectstats00_fu_1551.sizes25_U", "Parent" : "183"},
	{"ID" : "206", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_collectstats00_fu_1551.sizes26_U", "Parent" : "183"},
	{"ID" : "207", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_collectstats00_fu_1551.sizes27_U", "Parent" : "183"},
	{"ID" : "208", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_collectstats00_fu_1551.sizes30_U", "Parent" : "183"},
	{"ID" : "209", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_collectstats00_fu_1551.sizes31_U", "Parent" : "183"},
	{"ID" : "210", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_collectstats00_fu_1551.sizes32_U", "Parent" : "183"},
	{"ID" : "211", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_collectstats00_fu_1551.sizes33_U", "Parent" : "183"},
	{"ID" : "212", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_collectstats00_fu_1551.sizes34_U", "Parent" : "183"},
	{"ID" : "213", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_collectstats00_fu_1551.sizes35_U", "Parent" : "183"},
	{"ID" : "214", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_collectstats00_fu_1551.sizes36_U", "Parent" : "183"},
	{"ID" : "215", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_collectstats00_fu_1551.sizes37_U", "Parent" : "183"},
	{"ID" : "216", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_collectstats00_fu_1551.grp_getpartition_fu_1471", "Parent" : "183",
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
	{"ID" : "217", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_collectstats00_fu_1551.grp_getpartition_fu_1478", "Parent" : "183",
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
	{"ID" : "218", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_collectstats00_fu_1551.grp_getpartition_fu_1485", "Parent" : "183",
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
	{"ID" : "219", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_collectstats00_fu_1551.grp_getpartition_fu_1492", "Parent" : "183",
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
	{"ID" : "220", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_collectstats00_fu_1551.grp_getpartition_fu_1499", "Parent" : "183",
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
	{"ID" : "221", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_collectstats00_fu_1551.grp_getpartition_fu_1506", "Parent" : "183",
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
	{"ID" : "222", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_collectstats00_fu_1551.grp_getpartition_fu_1513", "Parent" : "183",
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
	{"ID" : "223", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_collectstats00_fu_1551.grp_getpartition_fu_1520", "Parent" : "183",
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
	{"ID" : "224", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_collectstats00_fu_1551.grp_getpartition_fu_1527", "Parent" : "183",
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
	{"ID" : "225", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_collectstats00_fu_1551.grp_getpartition_fu_1534", "Parent" : "183",
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
	{"ID" : "226", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_collectstats00_fu_1551.grp_getpartition_fu_1541", "Parent" : "183",
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
	{"ID" : "227", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_collectstats00_fu_1551.grp_getpartition_fu_1548", "Parent" : "183",
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
	{"ID" : "228", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_collectstats00_fu_1551.grp_getpartition_fu_1555", "Parent" : "183",
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
	{"ID" : "229", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_collectstats00_fu_1551.grp_getpartition_fu_1562", "Parent" : "183",
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
	{"ID" : "230", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_collectstats00_fu_1551.grp_getpartition_fu_1569", "Parent" : "183",
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
	{"ID" : "231", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_collectstats00_fu_1551.grp_getpartition_fu_1576", "Parent" : "183",
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
	{"ID" : "232", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_readcapsules0_fu_1575", "Parent" : "5", "Child" : ["233"],
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
					{"ID" : "233", "SubInstance" : "grp_readkeyvalues0_fu_418", "Port" : "kvdram_V"}]},
			{"Name" : "kvdram_V_offset", "Type" : "None", "Direction" : "I"},
			{"Name" : "BIGcapsule0_V", "Type" : "Memory", "Direction" : "IO",
				"SubConnect" : [
					{"ID" : "233", "SubInstance" : "grp_readkeyvalues0_fu_418", "Port" : "buffer_V"}]},
			{"Name" : "BIGcapsule1_V", "Type" : "Memory", "Direction" : "IO",
				"SubConnect" : [
					{"ID" : "233", "SubInstance" : "grp_readkeyvalues0_fu_418", "Port" : "buffer_V"}]},
			{"Name" : "BIGcapsule2_V", "Type" : "Memory", "Direction" : "IO",
				"SubConnect" : [
					{"ID" : "233", "SubInstance" : "grp_readkeyvalues0_fu_418", "Port" : "buffer_V"}]},
			{"Name" : "BIGcapsule3_V", "Type" : "Memory", "Direction" : "IO",
				"SubConnect" : [
					{"ID" : "233", "SubInstance" : "grp_readkeyvalues0_fu_418", "Port" : "buffer_V"}]},
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
	{"ID" : "233", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_readcapsules0_fu_1575.grp_readkeyvalues0_fu_418", "Parent" : "232",
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
	{"ID" : "234", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_savecapsules0_fu_1599", "Parent" : "5", "Child" : ["235"],
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
					{"ID" : "235", "SubInstance" : "grp_savekeyvalues0_fu_887", "Port" : "kvdram_V"}]},
			{"Name" : "kvdram_V_offset", "Type" : "None", "Direction" : "I"},
			{"Name" : "BIGcapsule0_V", "Type" : "Memory", "Direction" : "IO",
				"SubConnect" : [
					{"ID" : "235", "SubInstance" : "grp_savekeyvalues0_fu_887", "Port" : "buffer_V"}]},
			{"Name" : "BIGcapsule1_V", "Type" : "Memory", "Direction" : "IO",
				"SubConnect" : [
					{"ID" : "235", "SubInstance" : "grp_savekeyvalues0_fu_887", "Port" : "buffer_V"}]},
			{"Name" : "BIGcapsule2_V", "Type" : "Memory", "Direction" : "IO",
				"SubConnect" : [
					{"ID" : "235", "SubInstance" : "grp_savekeyvalues0_fu_887", "Port" : "buffer_V"}]},
			{"Name" : "BIGcapsule3_V", "Type" : "Memory", "Direction" : "IO",
				"SubConnect" : [
					{"ID" : "235", "SubInstance" : "grp_savekeyvalues0_fu_887", "Port" : "buffer_V"}]},
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
	{"ID" : "235", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_savecapsules0_fu_1599.grp_savekeyvalues0_fu_887", "Parent" : "234",
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
	{"ID" : "236", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_readkeyvalues0_2_fu_1628", "Parent" : "5", "Child" : ["237"],
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
					{"ID" : "237", "SubInstance" : "grp_readkeyvalues0_158_fu_136", "Port" : "kvdram_V"}]},
			{"Name" : "kvdram_V_offset", "Type" : "None", "Direction" : "I"},
			{"Name" : "buffer0_V", "Type" : "Memory", "Direction" : "O",
				"SubConnect" : [
					{"ID" : "237", "SubInstance" : "grp_readkeyvalues0_158_fu_136", "Port" : "buffer_V"}]},
			{"Name" : "buffer1_V", "Type" : "Memory", "Direction" : "O",
				"SubConnect" : [
					{"ID" : "237", "SubInstance" : "grp_readkeyvalues0_158_fu_136", "Port" : "buffer_V"}]},
			{"Name" : "buffer2_V", "Type" : "Memory", "Direction" : "O",
				"SubConnect" : [
					{"ID" : "237", "SubInstance" : "grp_readkeyvalues0_158_fu_136", "Port" : "buffer_V"}]},
			{"Name" : "buffer3_V", "Type" : "Memory", "Direction" : "O",
				"SubConnect" : [
					{"ID" : "237", "SubInstance" : "grp_readkeyvalues0_158_fu_136", "Port" : "buffer_V"}]},
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
	{"ID" : "237", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_readkeyvalues0_2_fu_1628.grp_readkeyvalues0_158_fu_136", "Parent" : "236",
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
	{"ID" : "238", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_readkeyvalues0_1_fu_1650", "Parent" : "5", "Child" : ["239"],
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
					{"ID" : "239", "SubInstance" : "grp_readkeyvalues0_158_fu_94", "Port" : "kvdram_V"}]},
			{"Name" : "kvdram_V_offset", "Type" : "None", "Direction" : "I"},
			{"Name" : "buffer0_V", "Type" : "Memory", "Direction" : "O",
				"SubConnect" : [
					{"ID" : "239", "SubInstance" : "grp_readkeyvalues0_158_fu_94", "Port" : "buffer_V"}]},
			{"Name" : "buffer1_V", "Type" : "Memory", "Direction" : "O",
				"SubConnect" : [
					{"ID" : "239", "SubInstance" : "grp_readkeyvalues0_158_fu_94", "Port" : "buffer_V"}]},
			{"Name" : "buffer2_V", "Type" : "Memory", "Direction" : "O",
				"SubConnect" : [
					{"ID" : "239", "SubInstance" : "grp_readkeyvalues0_158_fu_94", "Port" : "buffer_V"}]},
			{"Name" : "buffer3_V", "Type" : "Memory", "Direction" : "O",
				"SubConnect" : [
					{"ID" : "239", "SubInstance" : "grp_readkeyvalues0_158_fu_94", "Port" : "buffer_V"}]},
			{"Name" : "baseaddress", "Type" : "None", "Direction" : "I"},
			{"Name" : "offset_kvs", "Type" : "None", "Direction" : "I"},
			{"Name" : "travstate_i_kvs", "Type" : "None", "Direction" : "I"},
			{"Name" : "travstate_end_kvs", "Type" : "None", "Direction" : "I"}]},
	{"ID" : "239", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_readkeyvalues0_1_fu_1650.grp_readkeyvalues0_158_fu_94", "Parent" : "238",
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
	{"ID" : "240", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_savekeyvalues0_153_fu_1667", "Parent" : "5",
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
	{"ID" : "241", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_readkeyvalues0_157_fu_1676", "Parent" : "5",
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
	{"ID" : "242", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_savekeyvalues0_2_fu_1685", "Parent" : "5",
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
	{"ID" : "243", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_savekeyvalues0_1_fu_1695", "Parent" : "5",
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
	{"ID" : "244", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.topkernel_mux_42_32_1_1_U191", "Parent" : "5"}]}


set ArgLastReadFirstWriteLatency {
	topkernel {
		gmem0 {Type IO LastRead 73 FirstWrite -1}
		gmem1 {Type IO LastRead 9 FirstWrite -1}
		gmem2 {Type IO LastRead 44 FirstWrite 2}
		kvsourcedramA_V {Type I LastRead 0 FirstWrite -1}
		kvdestdramA_V {Type I LastRead 0 FirstWrite -1}
		kvstatsA {Type I LastRead 0 FirstWrite -1}}
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
	{"Name" : "Latency", "Min" : "57", "Max" : "197852404"}
	, {"Name" : "Interval", "Min" : "58", "Max" : "197852405"}
]}

set PipelineEnableSignalInfo {[
]}

set Spec2ImplPortList { 
	gmem0 { m_axi {  { m_axi_gmem0_AWVALID VALID 1 1 }  { m_axi_gmem0_AWREADY READY 0 1 }  { m_axi_gmem0_AWADDR ADDR 1 32 }  { m_axi_gmem0_AWID ID 1 1 }  { m_axi_gmem0_AWLEN LEN 1 8 }  { m_axi_gmem0_AWSIZE SIZE 1 3 }  { m_axi_gmem0_AWBURST BURST 1 2 }  { m_axi_gmem0_AWLOCK LOCK 1 2 }  { m_axi_gmem0_AWCACHE CACHE 1 4 }  { m_axi_gmem0_AWPROT PROT 1 3 }  { m_axi_gmem0_AWQOS QOS 1 4 }  { m_axi_gmem0_AWREGION REGION 1 4 }  { m_axi_gmem0_AWUSER USER 1 1 }  { m_axi_gmem0_WVALID VALID 1 1 }  { m_axi_gmem0_WREADY READY 0 1 }  { m_axi_gmem0_WDATA DATA 1 512 }  { m_axi_gmem0_WSTRB STRB 1 64 }  { m_axi_gmem0_WLAST LAST 1 1 }  { m_axi_gmem0_WID ID 1 1 }  { m_axi_gmem0_WUSER USER 1 1 }  { m_axi_gmem0_ARVALID VALID 1 1 }  { m_axi_gmem0_ARREADY READY 0 1 }  { m_axi_gmem0_ARADDR ADDR 1 32 }  { m_axi_gmem0_ARID ID 1 1 }  { m_axi_gmem0_ARLEN LEN 1 8 }  { m_axi_gmem0_ARSIZE SIZE 1 3 }  { m_axi_gmem0_ARBURST BURST 1 2 }  { m_axi_gmem0_ARLOCK LOCK 1 2 }  { m_axi_gmem0_ARCACHE CACHE 1 4 }  { m_axi_gmem0_ARPROT PROT 1 3 }  { m_axi_gmem0_ARQOS QOS 1 4 }  { m_axi_gmem0_ARREGION REGION 1 4 }  { m_axi_gmem0_ARUSER USER 1 1 }  { m_axi_gmem0_RVALID VALID 0 1 }  { m_axi_gmem0_RREADY READY 1 1 }  { m_axi_gmem0_RDATA DATA 0 512 }  { m_axi_gmem0_RLAST LAST 0 1 }  { m_axi_gmem0_RID ID 0 1 }  { m_axi_gmem0_RUSER USER 0 1 }  { m_axi_gmem0_RRESP RESP 0 2 }  { m_axi_gmem0_BVALID VALID 0 1 }  { m_axi_gmem0_BREADY READY 1 1 }  { m_axi_gmem0_BRESP RESP 0 2 }  { m_axi_gmem0_BID ID 0 1 }  { m_axi_gmem0_BUSER USER 0 1 } } }
	gmem1 { m_axi {  { m_axi_gmem1_AWVALID VALID 1 1 }  { m_axi_gmem1_AWREADY READY 0 1 }  { m_axi_gmem1_AWADDR ADDR 1 32 }  { m_axi_gmem1_AWID ID 1 1 }  { m_axi_gmem1_AWLEN LEN 1 8 }  { m_axi_gmem1_AWSIZE SIZE 1 3 }  { m_axi_gmem1_AWBURST BURST 1 2 }  { m_axi_gmem1_AWLOCK LOCK 1 2 }  { m_axi_gmem1_AWCACHE CACHE 1 4 }  { m_axi_gmem1_AWPROT PROT 1 3 }  { m_axi_gmem1_AWQOS QOS 1 4 }  { m_axi_gmem1_AWREGION REGION 1 4 }  { m_axi_gmem1_AWUSER USER 1 1 }  { m_axi_gmem1_WVALID VALID 1 1 }  { m_axi_gmem1_WREADY READY 0 1 }  { m_axi_gmem1_WDATA DATA 1 512 }  { m_axi_gmem1_WSTRB STRB 1 64 }  { m_axi_gmem1_WLAST LAST 1 1 }  { m_axi_gmem1_WID ID 1 1 }  { m_axi_gmem1_WUSER USER 1 1 }  { m_axi_gmem1_ARVALID VALID 1 1 }  { m_axi_gmem1_ARREADY READY 0 1 }  { m_axi_gmem1_ARADDR ADDR 1 32 }  { m_axi_gmem1_ARID ID 1 1 }  { m_axi_gmem1_ARLEN LEN 1 8 }  { m_axi_gmem1_ARSIZE SIZE 1 3 }  { m_axi_gmem1_ARBURST BURST 1 2 }  { m_axi_gmem1_ARLOCK LOCK 1 2 }  { m_axi_gmem1_ARCACHE CACHE 1 4 }  { m_axi_gmem1_ARPROT PROT 1 3 }  { m_axi_gmem1_ARQOS QOS 1 4 }  { m_axi_gmem1_ARREGION REGION 1 4 }  { m_axi_gmem1_ARUSER USER 1 1 }  { m_axi_gmem1_RVALID VALID 0 1 }  { m_axi_gmem1_RREADY READY 1 1 }  { m_axi_gmem1_RDATA DATA 0 512 }  { m_axi_gmem1_RLAST LAST 0 1 }  { m_axi_gmem1_RID ID 0 1 }  { m_axi_gmem1_RUSER USER 0 1 }  { m_axi_gmem1_RRESP RESP 0 2 }  { m_axi_gmem1_BVALID VALID 0 1 }  { m_axi_gmem1_BREADY READY 1 1 }  { m_axi_gmem1_BRESP RESP 0 2 }  { m_axi_gmem1_BID ID 0 1 }  { m_axi_gmem1_BUSER USER 0 1 } } }
	gmem2 { m_axi {  { m_axi_gmem2_AWVALID VALID 1 1 }  { m_axi_gmem2_AWREADY READY 0 1 }  { m_axi_gmem2_AWADDR ADDR 1 32 }  { m_axi_gmem2_AWID ID 1 1 }  { m_axi_gmem2_AWLEN LEN 1 8 }  { m_axi_gmem2_AWSIZE SIZE 1 3 }  { m_axi_gmem2_AWBURST BURST 1 2 }  { m_axi_gmem2_AWLOCK LOCK 1 2 }  { m_axi_gmem2_AWCACHE CACHE 1 4 }  { m_axi_gmem2_AWPROT PROT 1 3 }  { m_axi_gmem2_AWQOS QOS 1 4 }  { m_axi_gmem2_AWREGION REGION 1 4 }  { m_axi_gmem2_AWUSER USER 1 1 }  { m_axi_gmem2_WVALID VALID 1 1 }  { m_axi_gmem2_WREADY READY 0 1 }  { m_axi_gmem2_WDATA DATA 1 64 }  { m_axi_gmem2_WSTRB STRB 1 8 }  { m_axi_gmem2_WLAST LAST 1 1 }  { m_axi_gmem2_WID ID 1 1 }  { m_axi_gmem2_WUSER USER 1 1 }  { m_axi_gmem2_ARVALID VALID 1 1 }  { m_axi_gmem2_ARREADY READY 0 1 }  { m_axi_gmem2_ARADDR ADDR 1 32 }  { m_axi_gmem2_ARID ID 1 1 }  { m_axi_gmem2_ARLEN LEN 1 8 }  { m_axi_gmem2_ARSIZE SIZE 1 3 }  { m_axi_gmem2_ARBURST BURST 1 2 }  { m_axi_gmem2_ARLOCK LOCK 1 2 }  { m_axi_gmem2_ARCACHE CACHE 1 4 }  { m_axi_gmem2_ARPROT PROT 1 3 }  { m_axi_gmem2_ARQOS QOS 1 4 }  { m_axi_gmem2_ARREGION REGION 1 4 }  { m_axi_gmem2_ARUSER USER 1 1 }  { m_axi_gmem2_RVALID VALID 0 1 }  { m_axi_gmem2_RREADY READY 1 1 }  { m_axi_gmem2_RDATA DATA 0 64 }  { m_axi_gmem2_RLAST LAST 0 1 }  { m_axi_gmem2_RID ID 0 1 }  { m_axi_gmem2_RUSER USER 0 1 }  { m_axi_gmem2_RRESP RESP 0 2 }  { m_axi_gmem2_BVALID VALID 0 1 }  { m_axi_gmem2_BREADY READY 1 1 }  { m_axi_gmem2_BRESP RESP 0 2 }  { m_axi_gmem2_BID ID 0 1 }  { m_axi_gmem2_BUSER USER 0 1 } } }
}

set busDeadlockParameterList { 
	{ gmem0 { NUM_READ_OUTSTANDING 16 NUM_WRITE_OUTSTANDING 16 MAX_READ_BURST_LENGTH 64 MAX_WRITE_BURST_LENGTH 64 } } \
	{ gmem1 { NUM_READ_OUTSTANDING 16 NUM_WRITE_OUTSTANDING 16 MAX_READ_BURST_LENGTH 64 MAX_WRITE_BURST_LENGTH 64 } } \
	{ gmem2 { NUM_READ_OUTSTANDING 16 NUM_WRITE_OUTSTANDING 16 MAX_READ_BURST_LENGTH 16 MAX_WRITE_BURST_LENGTH 16 } } \
}

# RTL port scheduling information:
set fifoSchedulingInfoList { 
}

# RTL bus port read request latency information:
set busReadReqLatencyList { 
	{ gmem0 1 }
	{ gmem1 1 }
	{ gmem2 1 }
}

# RTL bus port write response latency information:
set busWriteResLatencyList { 
	{ gmem0 1 }
	{ gmem1 1 }
	{ gmem2 1 }
}

# RTL array port load latency information:
set memoryLoadLatencyList { 
}
set moduleName topkernel
set isTopModule 1
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
set C_modelName {topkernel}
set C_modelType { void 0 }
set C_modelArgList {
	{ gmem0 int 512 regular {axi_master 2}  }
	{ gmem1 int 512 regular {axi_master 2}  }
	{ gmem2 int 64 regular {axi_master 2}  }
	{ kvsourcedramA_V int 32 regular {axi_slave 0}  }
	{ kvdestdramA_V int 32 regular {axi_slave 0}  }
	{ kvstatsA int 32 regular {axi_slave 0}  }
}
set C_modelArgMapList {[ 
	{ "Name" : "gmem0", "interface" : "axi_master", "bitwidth" : 512, "direction" : "READWRITE", "bitSlice":[{"low":0,"up":511,"cElement": [{"cName": "kvsourcedramA.V","cData": "uint512","bit_use": { "low": 0,"up": 511},"offset": { "type": "dynamic","port_name": "kvsourcedramA_V","bundle": "control"},"direction": "READWRITE","cArray": [{"low" : 0,"up" : 0,"step" : 1}]}]}]} , 
 	{ "Name" : "gmem1", "interface" : "axi_master", "bitwidth" : 512, "direction" : "READWRITE", "bitSlice":[{"low":0,"up":511,"cElement": [{"cName": "kvdestdramA.V","cData": "uint512","bit_use": { "low": 0,"up": 511},"offset": { "type": "dynamic","port_name": "kvdestdramA_V","bundle": "control"},"direction": "READWRITE","cArray": [{"low" : 0,"up" : 0,"step" : 1}]}]}]} , 
 	{ "Name" : "gmem2", "interface" : "axi_master", "bitwidth" : 64, "direction" : "READWRITE", "bitSlice":[{"low":0,"up":31,"cElement": [{"cName": "kvstatsA.key","cData": "unsigned int","bit_use": { "low": 0,"up": 31},"offset": { "type": "dynamic","port_name": "kvstatsA"},"direction": "READWRITE","cArray": [{"low" : 0,"up" : 0,"step" : 1}]}]},{"low":32,"up":63,"cElement": [{"cName": "kvstatsA.value","cData": "unsigned int","bit_use": { "low": 0,"up": 31},"offset": { "type": "dynamic","port_name": "kvstatsA"},"direction": "READWRITE","cArray": [{"low" : 0,"up" : 0,"step" : 1}]}]}]} , 
 	{ "Name" : "kvsourcedramA_V", "interface" : "axi_slave", "bundle":"control","type":"ap_none","bitwidth" : 32, "direction" : "READONLY", "offset" : {"in":16}, "offset_end" : {"in":23}} , 
 	{ "Name" : "kvdestdramA_V", "interface" : "axi_slave", "bundle":"control","type":"ap_none","bitwidth" : 32, "direction" : "READONLY", "offset" : {"in":24}, "offset_end" : {"in":31}} , 
 	{ "Name" : "kvstatsA", "interface" : "axi_slave", "bundle":"control","type":"ap_none","bitwidth" : 32, "direction" : "READONLY", "offset" : {"in":32}, "offset_end" : {"in":39}} ]}
# RTL Port declarations: 
set portNum 155
set portList { 
	{ ap_clk sc_in sc_logic 1 clock -1 } 
	{ ap_rst_n sc_in sc_logic 1 reset -1 active_low_sync } 
	{ m_axi_gmem0_AWVALID sc_out sc_logic 1 signal 0 } 
	{ m_axi_gmem0_AWREADY sc_in sc_logic 1 signal 0 } 
	{ m_axi_gmem0_AWADDR sc_out sc_lv 32 signal 0 } 
	{ m_axi_gmem0_AWID sc_out sc_lv 1 signal 0 } 
	{ m_axi_gmem0_AWLEN sc_out sc_lv 8 signal 0 } 
	{ m_axi_gmem0_AWSIZE sc_out sc_lv 3 signal 0 } 
	{ m_axi_gmem0_AWBURST sc_out sc_lv 2 signal 0 } 
	{ m_axi_gmem0_AWLOCK sc_out sc_lv 2 signal 0 } 
	{ m_axi_gmem0_AWCACHE sc_out sc_lv 4 signal 0 } 
	{ m_axi_gmem0_AWPROT sc_out sc_lv 3 signal 0 } 
	{ m_axi_gmem0_AWQOS sc_out sc_lv 4 signal 0 } 
	{ m_axi_gmem0_AWREGION sc_out sc_lv 4 signal 0 } 
	{ m_axi_gmem0_AWUSER sc_out sc_lv 1 signal 0 } 
	{ m_axi_gmem0_WVALID sc_out sc_logic 1 signal 0 } 
	{ m_axi_gmem0_WREADY sc_in sc_logic 1 signal 0 } 
	{ m_axi_gmem0_WDATA sc_out sc_lv 512 signal 0 } 
	{ m_axi_gmem0_WSTRB sc_out sc_lv 64 signal 0 } 
	{ m_axi_gmem0_WLAST sc_out sc_logic 1 signal 0 } 
	{ m_axi_gmem0_WID sc_out sc_lv 1 signal 0 } 
	{ m_axi_gmem0_WUSER sc_out sc_lv 1 signal 0 } 
	{ m_axi_gmem0_ARVALID sc_out sc_logic 1 signal 0 } 
	{ m_axi_gmem0_ARREADY sc_in sc_logic 1 signal 0 } 
	{ m_axi_gmem0_ARADDR sc_out sc_lv 32 signal 0 } 
	{ m_axi_gmem0_ARID sc_out sc_lv 1 signal 0 } 
	{ m_axi_gmem0_ARLEN sc_out sc_lv 8 signal 0 } 
	{ m_axi_gmem0_ARSIZE sc_out sc_lv 3 signal 0 } 
	{ m_axi_gmem0_ARBURST sc_out sc_lv 2 signal 0 } 
	{ m_axi_gmem0_ARLOCK sc_out sc_lv 2 signal 0 } 
	{ m_axi_gmem0_ARCACHE sc_out sc_lv 4 signal 0 } 
	{ m_axi_gmem0_ARPROT sc_out sc_lv 3 signal 0 } 
	{ m_axi_gmem0_ARQOS sc_out sc_lv 4 signal 0 } 
	{ m_axi_gmem0_ARREGION sc_out sc_lv 4 signal 0 } 
	{ m_axi_gmem0_ARUSER sc_out sc_lv 1 signal 0 } 
	{ m_axi_gmem0_RVALID sc_in sc_logic 1 signal 0 } 
	{ m_axi_gmem0_RREADY sc_out sc_logic 1 signal 0 } 
	{ m_axi_gmem0_RDATA sc_in sc_lv 512 signal 0 } 
	{ m_axi_gmem0_RLAST sc_in sc_logic 1 signal 0 } 
	{ m_axi_gmem0_RID sc_in sc_lv 1 signal 0 } 
	{ m_axi_gmem0_RUSER sc_in sc_lv 1 signal 0 } 
	{ m_axi_gmem0_RRESP sc_in sc_lv 2 signal 0 } 
	{ m_axi_gmem0_BVALID sc_in sc_logic 1 signal 0 } 
	{ m_axi_gmem0_BREADY sc_out sc_logic 1 signal 0 } 
	{ m_axi_gmem0_BRESP sc_in sc_lv 2 signal 0 } 
	{ m_axi_gmem0_BID sc_in sc_lv 1 signal 0 } 
	{ m_axi_gmem0_BUSER sc_in sc_lv 1 signal 0 } 
	{ m_axi_gmem1_AWVALID sc_out sc_logic 1 signal 1 } 
	{ m_axi_gmem1_AWREADY sc_in sc_logic 1 signal 1 } 
	{ m_axi_gmem1_AWADDR sc_out sc_lv 32 signal 1 } 
	{ m_axi_gmem1_AWID sc_out sc_lv 1 signal 1 } 
	{ m_axi_gmem1_AWLEN sc_out sc_lv 8 signal 1 } 
	{ m_axi_gmem1_AWSIZE sc_out sc_lv 3 signal 1 } 
	{ m_axi_gmem1_AWBURST sc_out sc_lv 2 signal 1 } 
	{ m_axi_gmem1_AWLOCK sc_out sc_lv 2 signal 1 } 
	{ m_axi_gmem1_AWCACHE sc_out sc_lv 4 signal 1 } 
	{ m_axi_gmem1_AWPROT sc_out sc_lv 3 signal 1 } 
	{ m_axi_gmem1_AWQOS sc_out sc_lv 4 signal 1 } 
	{ m_axi_gmem1_AWREGION sc_out sc_lv 4 signal 1 } 
	{ m_axi_gmem1_AWUSER sc_out sc_lv 1 signal 1 } 
	{ m_axi_gmem1_WVALID sc_out sc_logic 1 signal 1 } 
	{ m_axi_gmem1_WREADY sc_in sc_logic 1 signal 1 } 
	{ m_axi_gmem1_WDATA sc_out sc_lv 512 signal 1 } 
	{ m_axi_gmem1_WSTRB sc_out sc_lv 64 signal 1 } 
	{ m_axi_gmem1_WLAST sc_out sc_logic 1 signal 1 } 
	{ m_axi_gmem1_WID sc_out sc_lv 1 signal 1 } 
	{ m_axi_gmem1_WUSER sc_out sc_lv 1 signal 1 } 
	{ m_axi_gmem1_ARVALID sc_out sc_logic 1 signal 1 } 
	{ m_axi_gmem1_ARREADY sc_in sc_logic 1 signal 1 } 
	{ m_axi_gmem1_ARADDR sc_out sc_lv 32 signal 1 } 
	{ m_axi_gmem1_ARID sc_out sc_lv 1 signal 1 } 
	{ m_axi_gmem1_ARLEN sc_out sc_lv 8 signal 1 } 
	{ m_axi_gmem1_ARSIZE sc_out sc_lv 3 signal 1 } 
	{ m_axi_gmem1_ARBURST sc_out sc_lv 2 signal 1 } 
	{ m_axi_gmem1_ARLOCK sc_out sc_lv 2 signal 1 } 
	{ m_axi_gmem1_ARCACHE sc_out sc_lv 4 signal 1 } 
	{ m_axi_gmem1_ARPROT sc_out sc_lv 3 signal 1 } 
	{ m_axi_gmem1_ARQOS sc_out sc_lv 4 signal 1 } 
	{ m_axi_gmem1_ARREGION sc_out sc_lv 4 signal 1 } 
	{ m_axi_gmem1_ARUSER sc_out sc_lv 1 signal 1 } 
	{ m_axi_gmem1_RVALID sc_in sc_logic 1 signal 1 } 
	{ m_axi_gmem1_RREADY sc_out sc_logic 1 signal 1 } 
	{ m_axi_gmem1_RDATA sc_in sc_lv 512 signal 1 } 
	{ m_axi_gmem1_RLAST sc_in sc_logic 1 signal 1 } 
	{ m_axi_gmem1_RID sc_in sc_lv 1 signal 1 } 
	{ m_axi_gmem1_RUSER sc_in sc_lv 1 signal 1 } 
	{ m_axi_gmem1_RRESP sc_in sc_lv 2 signal 1 } 
	{ m_axi_gmem1_BVALID sc_in sc_logic 1 signal 1 } 
	{ m_axi_gmem1_BREADY sc_out sc_logic 1 signal 1 } 
	{ m_axi_gmem1_BRESP sc_in sc_lv 2 signal 1 } 
	{ m_axi_gmem1_BID sc_in sc_lv 1 signal 1 } 
	{ m_axi_gmem1_BUSER sc_in sc_lv 1 signal 1 } 
	{ m_axi_gmem2_AWVALID sc_out sc_logic 1 signal 2 } 
	{ m_axi_gmem2_AWREADY sc_in sc_logic 1 signal 2 } 
	{ m_axi_gmem2_AWADDR sc_out sc_lv 32 signal 2 } 
	{ m_axi_gmem2_AWID sc_out sc_lv 1 signal 2 } 
	{ m_axi_gmem2_AWLEN sc_out sc_lv 8 signal 2 } 
	{ m_axi_gmem2_AWSIZE sc_out sc_lv 3 signal 2 } 
	{ m_axi_gmem2_AWBURST sc_out sc_lv 2 signal 2 } 
	{ m_axi_gmem2_AWLOCK sc_out sc_lv 2 signal 2 } 
	{ m_axi_gmem2_AWCACHE sc_out sc_lv 4 signal 2 } 
	{ m_axi_gmem2_AWPROT sc_out sc_lv 3 signal 2 } 
	{ m_axi_gmem2_AWQOS sc_out sc_lv 4 signal 2 } 
	{ m_axi_gmem2_AWREGION sc_out sc_lv 4 signal 2 } 
	{ m_axi_gmem2_AWUSER sc_out sc_lv 1 signal 2 } 
	{ m_axi_gmem2_WVALID sc_out sc_logic 1 signal 2 } 
	{ m_axi_gmem2_WREADY sc_in sc_logic 1 signal 2 } 
	{ m_axi_gmem2_WDATA sc_out sc_lv 64 signal 2 } 
	{ m_axi_gmem2_WSTRB sc_out sc_lv 8 signal 2 } 
	{ m_axi_gmem2_WLAST sc_out sc_logic 1 signal 2 } 
	{ m_axi_gmem2_WID sc_out sc_lv 1 signal 2 } 
	{ m_axi_gmem2_WUSER sc_out sc_lv 1 signal 2 } 
	{ m_axi_gmem2_ARVALID sc_out sc_logic 1 signal 2 } 
	{ m_axi_gmem2_ARREADY sc_in sc_logic 1 signal 2 } 
	{ m_axi_gmem2_ARADDR sc_out sc_lv 32 signal 2 } 
	{ m_axi_gmem2_ARID sc_out sc_lv 1 signal 2 } 
	{ m_axi_gmem2_ARLEN sc_out sc_lv 8 signal 2 } 
	{ m_axi_gmem2_ARSIZE sc_out sc_lv 3 signal 2 } 
	{ m_axi_gmem2_ARBURST sc_out sc_lv 2 signal 2 } 
	{ m_axi_gmem2_ARLOCK sc_out sc_lv 2 signal 2 } 
	{ m_axi_gmem2_ARCACHE sc_out sc_lv 4 signal 2 } 
	{ m_axi_gmem2_ARPROT sc_out sc_lv 3 signal 2 } 
	{ m_axi_gmem2_ARQOS sc_out sc_lv 4 signal 2 } 
	{ m_axi_gmem2_ARREGION sc_out sc_lv 4 signal 2 } 
	{ m_axi_gmem2_ARUSER sc_out sc_lv 1 signal 2 } 
	{ m_axi_gmem2_RVALID sc_in sc_logic 1 signal 2 } 
	{ m_axi_gmem2_RREADY sc_out sc_logic 1 signal 2 } 
	{ m_axi_gmem2_RDATA sc_in sc_lv 64 signal 2 } 
	{ m_axi_gmem2_RLAST sc_in sc_logic 1 signal 2 } 
	{ m_axi_gmem2_RID sc_in sc_lv 1 signal 2 } 
	{ m_axi_gmem2_RUSER sc_in sc_lv 1 signal 2 } 
	{ m_axi_gmem2_RRESP sc_in sc_lv 2 signal 2 } 
	{ m_axi_gmem2_BVALID sc_in sc_logic 1 signal 2 } 
	{ m_axi_gmem2_BREADY sc_out sc_logic 1 signal 2 } 
	{ m_axi_gmem2_BRESP sc_in sc_lv 2 signal 2 } 
	{ m_axi_gmem2_BID sc_in sc_lv 1 signal 2 } 
	{ m_axi_gmem2_BUSER sc_in sc_lv 1 signal 2 } 
	{ s_axi_control_AWVALID sc_in sc_logic 1 signal -1 } 
	{ s_axi_control_AWREADY sc_out sc_logic 1 signal -1 } 
	{ s_axi_control_AWADDR sc_in sc_lv 6 signal -1 } 
	{ s_axi_control_WVALID sc_in sc_logic 1 signal -1 } 
	{ s_axi_control_WREADY sc_out sc_logic 1 signal -1 } 
	{ s_axi_control_WDATA sc_in sc_lv 32 signal -1 } 
	{ s_axi_control_WSTRB sc_in sc_lv 4 signal -1 } 
	{ s_axi_control_ARVALID sc_in sc_logic 1 signal -1 } 
	{ s_axi_control_ARREADY sc_out sc_logic 1 signal -1 } 
	{ s_axi_control_ARADDR sc_in sc_lv 6 signal -1 } 
	{ s_axi_control_RVALID sc_out sc_logic 1 signal -1 } 
	{ s_axi_control_RREADY sc_in sc_logic 1 signal -1 } 
	{ s_axi_control_RDATA sc_out sc_lv 32 signal -1 } 
	{ s_axi_control_RRESP sc_out sc_lv 2 signal -1 } 
	{ s_axi_control_BVALID sc_out sc_logic 1 signal -1 } 
	{ s_axi_control_BREADY sc_in sc_logic 1 signal -1 } 
	{ s_axi_control_BRESP sc_out sc_lv 2 signal -1 } 
	{ interrupt sc_out sc_logic 1 signal -1 } 
}
set NewPortList {[ 
	{ "name": "s_axi_control_AWADDR", "direction": "in", "datatype": "sc_lv", "bitwidth":6, "type": "signal", "bundle":{"name": "control", "role": "AWADDR" },"address":[{"name":"topkernel","role":"start","value":"0","valid_bit":"0"},{"name":"topkernel","role":"continue","value":"0","valid_bit":"4"},{"name":"topkernel","role":"auto_start","value":"0","valid_bit":"7"},{"name":"kvsourcedramA_V","role":"data","value":"16"},{"name":"kvdestdramA_V","role":"data","value":"24"},{"name":"kvstatsA","role":"data","value":"32"}] },
	{ "name": "s_axi_control_AWVALID", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "control", "role": "AWVALID" } },
	{ "name": "s_axi_control_AWREADY", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "control", "role": "AWREADY" } },
	{ "name": "s_axi_control_WVALID", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "control", "role": "WVALID" } },
	{ "name": "s_axi_control_WREADY", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "control", "role": "WREADY" } },
	{ "name": "s_axi_control_WDATA", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "control", "role": "WDATA" } },
	{ "name": "s_axi_control_WSTRB", "direction": "in", "datatype": "sc_lv", "bitwidth":4, "type": "signal", "bundle":{"name": "control", "role": "WSTRB" } },
	{ "name": "s_axi_control_ARADDR", "direction": "in", "datatype": "sc_lv", "bitwidth":6, "type": "signal", "bundle":{"name": "control", "role": "ARADDR" },"address":[{"name":"topkernel","role":"start","value":"0","valid_bit":"0"},{"name":"topkernel","role":"done","value":"0","valid_bit":"1"},{"name":"topkernel","role":"idle","value":"0","valid_bit":"2"},{"name":"topkernel","role":"ready","value":"0","valid_bit":"3"},{"name":"topkernel","role":"auto_start","value":"0","valid_bit":"7"}] },
	{ "name": "s_axi_control_ARVALID", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "control", "role": "ARVALID" } },
	{ "name": "s_axi_control_ARREADY", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "control", "role": "ARREADY" } },
	{ "name": "s_axi_control_RVALID", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "control", "role": "RVALID" } },
	{ "name": "s_axi_control_RREADY", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "control", "role": "RREADY" } },
	{ "name": "s_axi_control_RDATA", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "control", "role": "RDATA" } },
	{ "name": "s_axi_control_RRESP", "direction": "out", "datatype": "sc_lv", "bitwidth":2, "type": "signal", "bundle":{"name": "control", "role": "RRESP" } },
	{ "name": "s_axi_control_BVALID", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "control", "role": "BVALID" } },
	{ "name": "s_axi_control_BREADY", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "control", "role": "BREADY" } },
	{ "name": "s_axi_control_BRESP", "direction": "out", "datatype": "sc_lv", "bitwidth":2, "type": "signal", "bundle":{"name": "control", "role": "BRESP" } },
	{ "name": "interrupt", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "control", "role": "interrupt" } }, 
 	{ "name": "ap_clk", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "clock", "bundle":{"name": "ap_clk", "role": "default" }} , 
 	{ "name": "ap_rst_n", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "reset", "bundle":{"name": "ap_rst_n", "role": "default" }} , 
 	{ "name": "m_axi_gmem0_AWVALID", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "gmem0", "role": "AWVALID" }} , 
 	{ "name": "m_axi_gmem0_AWREADY", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "gmem0", "role": "AWREADY" }} , 
 	{ "name": "m_axi_gmem0_AWADDR", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "gmem0", "role": "AWADDR" }} , 
 	{ "name": "m_axi_gmem0_AWID", "direction": "out", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "gmem0", "role": "AWID" }} , 
 	{ "name": "m_axi_gmem0_AWLEN", "direction": "out", "datatype": "sc_lv", "bitwidth":8, "type": "signal", "bundle":{"name": "gmem0", "role": "AWLEN" }} , 
 	{ "name": "m_axi_gmem0_AWSIZE", "direction": "out", "datatype": "sc_lv", "bitwidth":3, "type": "signal", "bundle":{"name": "gmem0", "role": "AWSIZE" }} , 
 	{ "name": "m_axi_gmem0_AWBURST", "direction": "out", "datatype": "sc_lv", "bitwidth":2, "type": "signal", "bundle":{"name": "gmem0", "role": "AWBURST" }} , 
 	{ "name": "m_axi_gmem0_AWLOCK", "direction": "out", "datatype": "sc_lv", "bitwidth":2, "type": "signal", "bundle":{"name": "gmem0", "role": "AWLOCK" }} , 
 	{ "name": "m_axi_gmem0_AWCACHE", "direction": "out", "datatype": "sc_lv", "bitwidth":4, "type": "signal", "bundle":{"name": "gmem0", "role": "AWCACHE" }} , 
 	{ "name": "m_axi_gmem0_AWPROT", "direction": "out", "datatype": "sc_lv", "bitwidth":3, "type": "signal", "bundle":{"name": "gmem0", "role": "AWPROT" }} , 
 	{ "name": "m_axi_gmem0_AWQOS", "direction": "out", "datatype": "sc_lv", "bitwidth":4, "type": "signal", "bundle":{"name": "gmem0", "role": "AWQOS" }} , 
 	{ "name": "m_axi_gmem0_AWREGION", "direction": "out", "datatype": "sc_lv", "bitwidth":4, "type": "signal", "bundle":{"name": "gmem0", "role": "AWREGION" }} , 
 	{ "name": "m_axi_gmem0_AWUSER", "direction": "out", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "gmem0", "role": "AWUSER" }} , 
 	{ "name": "m_axi_gmem0_WVALID", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "gmem0", "role": "WVALID" }} , 
 	{ "name": "m_axi_gmem0_WREADY", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "gmem0", "role": "WREADY" }} , 
 	{ "name": "m_axi_gmem0_WDATA", "direction": "out", "datatype": "sc_lv", "bitwidth":512, "type": "signal", "bundle":{"name": "gmem0", "role": "WDATA" }} , 
 	{ "name": "m_axi_gmem0_WSTRB", "direction": "out", "datatype": "sc_lv", "bitwidth":64, "type": "signal", "bundle":{"name": "gmem0", "role": "WSTRB" }} , 
 	{ "name": "m_axi_gmem0_WLAST", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "gmem0", "role": "WLAST" }} , 
 	{ "name": "m_axi_gmem0_WID", "direction": "out", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "gmem0", "role": "WID" }} , 
 	{ "name": "m_axi_gmem0_WUSER", "direction": "out", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "gmem0", "role": "WUSER" }} , 
 	{ "name": "m_axi_gmem0_ARVALID", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "gmem0", "role": "ARVALID" }} , 
 	{ "name": "m_axi_gmem0_ARREADY", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "gmem0", "role": "ARREADY" }} , 
 	{ "name": "m_axi_gmem0_ARADDR", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "gmem0", "role": "ARADDR" }} , 
 	{ "name": "m_axi_gmem0_ARID", "direction": "out", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "gmem0", "role": "ARID" }} , 
 	{ "name": "m_axi_gmem0_ARLEN", "direction": "out", "datatype": "sc_lv", "bitwidth":8, "type": "signal", "bundle":{"name": "gmem0", "role": "ARLEN" }} , 
 	{ "name": "m_axi_gmem0_ARSIZE", "direction": "out", "datatype": "sc_lv", "bitwidth":3, "type": "signal", "bundle":{"name": "gmem0", "role": "ARSIZE" }} , 
 	{ "name": "m_axi_gmem0_ARBURST", "direction": "out", "datatype": "sc_lv", "bitwidth":2, "type": "signal", "bundle":{"name": "gmem0", "role": "ARBURST" }} , 
 	{ "name": "m_axi_gmem0_ARLOCK", "direction": "out", "datatype": "sc_lv", "bitwidth":2, "type": "signal", "bundle":{"name": "gmem0", "role": "ARLOCK" }} , 
 	{ "name": "m_axi_gmem0_ARCACHE", "direction": "out", "datatype": "sc_lv", "bitwidth":4, "type": "signal", "bundle":{"name": "gmem0", "role": "ARCACHE" }} , 
 	{ "name": "m_axi_gmem0_ARPROT", "direction": "out", "datatype": "sc_lv", "bitwidth":3, "type": "signal", "bundle":{"name": "gmem0", "role": "ARPROT" }} , 
 	{ "name": "m_axi_gmem0_ARQOS", "direction": "out", "datatype": "sc_lv", "bitwidth":4, "type": "signal", "bundle":{"name": "gmem0", "role": "ARQOS" }} , 
 	{ "name": "m_axi_gmem0_ARREGION", "direction": "out", "datatype": "sc_lv", "bitwidth":4, "type": "signal", "bundle":{"name": "gmem0", "role": "ARREGION" }} , 
 	{ "name": "m_axi_gmem0_ARUSER", "direction": "out", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "gmem0", "role": "ARUSER" }} , 
 	{ "name": "m_axi_gmem0_RVALID", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "gmem0", "role": "RVALID" }} , 
 	{ "name": "m_axi_gmem0_RREADY", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "gmem0", "role": "RREADY" }} , 
 	{ "name": "m_axi_gmem0_RDATA", "direction": "in", "datatype": "sc_lv", "bitwidth":512, "type": "signal", "bundle":{"name": "gmem0", "role": "RDATA" }} , 
 	{ "name": "m_axi_gmem0_RLAST", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "gmem0", "role": "RLAST" }} , 
 	{ "name": "m_axi_gmem0_RID", "direction": "in", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "gmem0", "role": "RID" }} , 
 	{ "name": "m_axi_gmem0_RUSER", "direction": "in", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "gmem0", "role": "RUSER" }} , 
 	{ "name": "m_axi_gmem0_RRESP", "direction": "in", "datatype": "sc_lv", "bitwidth":2, "type": "signal", "bundle":{"name": "gmem0", "role": "RRESP" }} , 
 	{ "name": "m_axi_gmem0_BVALID", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "gmem0", "role": "BVALID" }} , 
 	{ "name": "m_axi_gmem0_BREADY", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "gmem0", "role": "BREADY" }} , 
 	{ "name": "m_axi_gmem0_BRESP", "direction": "in", "datatype": "sc_lv", "bitwidth":2, "type": "signal", "bundle":{"name": "gmem0", "role": "BRESP" }} , 
 	{ "name": "m_axi_gmem0_BID", "direction": "in", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "gmem0", "role": "BID" }} , 
 	{ "name": "m_axi_gmem0_BUSER", "direction": "in", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "gmem0", "role": "BUSER" }} , 
 	{ "name": "m_axi_gmem1_AWVALID", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "gmem1", "role": "AWVALID" }} , 
 	{ "name": "m_axi_gmem1_AWREADY", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "gmem1", "role": "AWREADY" }} , 
 	{ "name": "m_axi_gmem1_AWADDR", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "gmem1", "role": "AWADDR" }} , 
 	{ "name": "m_axi_gmem1_AWID", "direction": "out", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "gmem1", "role": "AWID" }} , 
 	{ "name": "m_axi_gmem1_AWLEN", "direction": "out", "datatype": "sc_lv", "bitwidth":8, "type": "signal", "bundle":{"name": "gmem1", "role": "AWLEN" }} , 
 	{ "name": "m_axi_gmem1_AWSIZE", "direction": "out", "datatype": "sc_lv", "bitwidth":3, "type": "signal", "bundle":{"name": "gmem1", "role": "AWSIZE" }} , 
 	{ "name": "m_axi_gmem1_AWBURST", "direction": "out", "datatype": "sc_lv", "bitwidth":2, "type": "signal", "bundle":{"name": "gmem1", "role": "AWBURST" }} , 
 	{ "name": "m_axi_gmem1_AWLOCK", "direction": "out", "datatype": "sc_lv", "bitwidth":2, "type": "signal", "bundle":{"name": "gmem1", "role": "AWLOCK" }} , 
 	{ "name": "m_axi_gmem1_AWCACHE", "direction": "out", "datatype": "sc_lv", "bitwidth":4, "type": "signal", "bundle":{"name": "gmem1", "role": "AWCACHE" }} , 
 	{ "name": "m_axi_gmem1_AWPROT", "direction": "out", "datatype": "sc_lv", "bitwidth":3, "type": "signal", "bundle":{"name": "gmem1", "role": "AWPROT" }} , 
 	{ "name": "m_axi_gmem1_AWQOS", "direction": "out", "datatype": "sc_lv", "bitwidth":4, "type": "signal", "bundle":{"name": "gmem1", "role": "AWQOS" }} , 
 	{ "name": "m_axi_gmem1_AWREGION", "direction": "out", "datatype": "sc_lv", "bitwidth":4, "type": "signal", "bundle":{"name": "gmem1", "role": "AWREGION" }} , 
 	{ "name": "m_axi_gmem1_AWUSER", "direction": "out", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "gmem1", "role": "AWUSER" }} , 
 	{ "name": "m_axi_gmem1_WVALID", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "gmem1", "role": "WVALID" }} , 
 	{ "name": "m_axi_gmem1_WREADY", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "gmem1", "role": "WREADY" }} , 
 	{ "name": "m_axi_gmem1_WDATA", "direction": "out", "datatype": "sc_lv", "bitwidth":512, "type": "signal", "bundle":{"name": "gmem1", "role": "WDATA" }} , 
 	{ "name": "m_axi_gmem1_WSTRB", "direction": "out", "datatype": "sc_lv", "bitwidth":64, "type": "signal", "bundle":{"name": "gmem1", "role": "WSTRB" }} , 
 	{ "name": "m_axi_gmem1_WLAST", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "gmem1", "role": "WLAST" }} , 
 	{ "name": "m_axi_gmem1_WID", "direction": "out", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "gmem1", "role": "WID" }} , 
 	{ "name": "m_axi_gmem1_WUSER", "direction": "out", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "gmem1", "role": "WUSER" }} , 
 	{ "name": "m_axi_gmem1_ARVALID", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "gmem1", "role": "ARVALID" }} , 
 	{ "name": "m_axi_gmem1_ARREADY", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "gmem1", "role": "ARREADY" }} , 
 	{ "name": "m_axi_gmem1_ARADDR", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "gmem1", "role": "ARADDR" }} , 
 	{ "name": "m_axi_gmem1_ARID", "direction": "out", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "gmem1", "role": "ARID" }} , 
 	{ "name": "m_axi_gmem1_ARLEN", "direction": "out", "datatype": "sc_lv", "bitwidth":8, "type": "signal", "bundle":{"name": "gmem1", "role": "ARLEN" }} , 
 	{ "name": "m_axi_gmem1_ARSIZE", "direction": "out", "datatype": "sc_lv", "bitwidth":3, "type": "signal", "bundle":{"name": "gmem1", "role": "ARSIZE" }} , 
 	{ "name": "m_axi_gmem1_ARBURST", "direction": "out", "datatype": "sc_lv", "bitwidth":2, "type": "signal", "bundle":{"name": "gmem1", "role": "ARBURST" }} , 
 	{ "name": "m_axi_gmem1_ARLOCK", "direction": "out", "datatype": "sc_lv", "bitwidth":2, "type": "signal", "bundle":{"name": "gmem1", "role": "ARLOCK" }} , 
 	{ "name": "m_axi_gmem1_ARCACHE", "direction": "out", "datatype": "sc_lv", "bitwidth":4, "type": "signal", "bundle":{"name": "gmem1", "role": "ARCACHE" }} , 
 	{ "name": "m_axi_gmem1_ARPROT", "direction": "out", "datatype": "sc_lv", "bitwidth":3, "type": "signal", "bundle":{"name": "gmem1", "role": "ARPROT" }} , 
 	{ "name": "m_axi_gmem1_ARQOS", "direction": "out", "datatype": "sc_lv", "bitwidth":4, "type": "signal", "bundle":{"name": "gmem1", "role": "ARQOS" }} , 
 	{ "name": "m_axi_gmem1_ARREGION", "direction": "out", "datatype": "sc_lv", "bitwidth":4, "type": "signal", "bundle":{"name": "gmem1", "role": "ARREGION" }} , 
 	{ "name": "m_axi_gmem1_ARUSER", "direction": "out", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "gmem1", "role": "ARUSER" }} , 
 	{ "name": "m_axi_gmem1_RVALID", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "gmem1", "role": "RVALID" }} , 
 	{ "name": "m_axi_gmem1_RREADY", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "gmem1", "role": "RREADY" }} , 
 	{ "name": "m_axi_gmem1_RDATA", "direction": "in", "datatype": "sc_lv", "bitwidth":512, "type": "signal", "bundle":{"name": "gmem1", "role": "RDATA" }} , 
 	{ "name": "m_axi_gmem1_RLAST", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "gmem1", "role": "RLAST" }} , 
 	{ "name": "m_axi_gmem1_RID", "direction": "in", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "gmem1", "role": "RID" }} , 
 	{ "name": "m_axi_gmem1_RUSER", "direction": "in", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "gmem1", "role": "RUSER" }} , 
 	{ "name": "m_axi_gmem1_RRESP", "direction": "in", "datatype": "sc_lv", "bitwidth":2, "type": "signal", "bundle":{"name": "gmem1", "role": "RRESP" }} , 
 	{ "name": "m_axi_gmem1_BVALID", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "gmem1", "role": "BVALID" }} , 
 	{ "name": "m_axi_gmem1_BREADY", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "gmem1", "role": "BREADY" }} , 
 	{ "name": "m_axi_gmem1_BRESP", "direction": "in", "datatype": "sc_lv", "bitwidth":2, "type": "signal", "bundle":{"name": "gmem1", "role": "BRESP" }} , 
 	{ "name": "m_axi_gmem1_BID", "direction": "in", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "gmem1", "role": "BID" }} , 
 	{ "name": "m_axi_gmem1_BUSER", "direction": "in", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "gmem1", "role": "BUSER" }} , 
 	{ "name": "m_axi_gmem2_AWVALID", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "gmem2", "role": "AWVALID" }} , 
 	{ "name": "m_axi_gmem2_AWREADY", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "gmem2", "role": "AWREADY" }} , 
 	{ "name": "m_axi_gmem2_AWADDR", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "gmem2", "role": "AWADDR" }} , 
 	{ "name": "m_axi_gmem2_AWID", "direction": "out", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "gmem2", "role": "AWID" }} , 
 	{ "name": "m_axi_gmem2_AWLEN", "direction": "out", "datatype": "sc_lv", "bitwidth":8, "type": "signal", "bundle":{"name": "gmem2", "role": "AWLEN" }} , 
 	{ "name": "m_axi_gmem2_AWSIZE", "direction": "out", "datatype": "sc_lv", "bitwidth":3, "type": "signal", "bundle":{"name": "gmem2", "role": "AWSIZE" }} , 
 	{ "name": "m_axi_gmem2_AWBURST", "direction": "out", "datatype": "sc_lv", "bitwidth":2, "type": "signal", "bundle":{"name": "gmem2", "role": "AWBURST" }} , 
 	{ "name": "m_axi_gmem2_AWLOCK", "direction": "out", "datatype": "sc_lv", "bitwidth":2, "type": "signal", "bundle":{"name": "gmem2", "role": "AWLOCK" }} , 
 	{ "name": "m_axi_gmem2_AWCACHE", "direction": "out", "datatype": "sc_lv", "bitwidth":4, "type": "signal", "bundle":{"name": "gmem2", "role": "AWCACHE" }} , 
 	{ "name": "m_axi_gmem2_AWPROT", "direction": "out", "datatype": "sc_lv", "bitwidth":3, "type": "signal", "bundle":{"name": "gmem2", "role": "AWPROT" }} , 
 	{ "name": "m_axi_gmem2_AWQOS", "direction": "out", "datatype": "sc_lv", "bitwidth":4, "type": "signal", "bundle":{"name": "gmem2", "role": "AWQOS" }} , 
 	{ "name": "m_axi_gmem2_AWREGION", "direction": "out", "datatype": "sc_lv", "bitwidth":4, "type": "signal", "bundle":{"name": "gmem2", "role": "AWREGION" }} , 
 	{ "name": "m_axi_gmem2_AWUSER", "direction": "out", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "gmem2", "role": "AWUSER" }} , 
 	{ "name": "m_axi_gmem2_WVALID", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "gmem2", "role": "WVALID" }} , 
 	{ "name": "m_axi_gmem2_WREADY", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "gmem2", "role": "WREADY" }} , 
 	{ "name": "m_axi_gmem2_WDATA", "direction": "out", "datatype": "sc_lv", "bitwidth":64, "type": "signal", "bundle":{"name": "gmem2", "role": "WDATA" }} , 
 	{ "name": "m_axi_gmem2_WSTRB", "direction": "out", "datatype": "sc_lv", "bitwidth":8, "type": "signal", "bundle":{"name": "gmem2", "role": "WSTRB" }} , 
 	{ "name": "m_axi_gmem2_WLAST", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "gmem2", "role": "WLAST" }} , 
 	{ "name": "m_axi_gmem2_WID", "direction": "out", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "gmem2", "role": "WID" }} , 
 	{ "name": "m_axi_gmem2_WUSER", "direction": "out", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "gmem2", "role": "WUSER" }} , 
 	{ "name": "m_axi_gmem2_ARVALID", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "gmem2", "role": "ARVALID" }} , 
 	{ "name": "m_axi_gmem2_ARREADY", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "gmem2", "role": "ARREADY" }} , 
 	{ "name": "m_axi_gmem2_ARADDR", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "gmem2", "role": "ARADDR" }} , 
 	{ "name": "m_axi_gmem2_ARID", "direction": "out", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "gmem2", "role": "ARID" }} , 
 	{ "name": "m_axi_gmem2_ARLEN", "direction": "out", "datatype": "sc_lv", "bitwidth":8, "type": "signal", "bundle":{"name": "gmem2", "role": "ARLEN" }} , 
 	{ "name": "m_axi_gmem2_ARSIZE", "direction": "out", "datatype": "sc_lv", "bitwidth":3, "type": "signal", "bundle":{"name": "gmem2", "role": "ARSIZE" }} , 
 	{ "name": "m_axi_gmem2_ARBURST", "direction": "out", "datatype": "sc_lv", "bitwidth":2, "type": "signal", "bundle":{"name": "gmem2", "role": "ARBURST" }} , 
 	{ "name": "m_axi_gmem2_ARLOCK", "direction": "out", "datatype": "sc_lv", "bitwidth":2, "type": "signal", "bundle":{"name": "gmem2", "role": "ARLOCK" }} , 
 	{ "name": "m_axi_gmem2_ARCACHE", "direction": "out", "datatype": "sc_lv", "bitwidth":4, "type": "signal", "bundle":{"name": "gmem2", "role": "ARCACHE" }} , 
 	{ "name": "m_axi_gmem2_ARPROT", "direction": "out", "datatype": "sc_lv", "bitwidth":3, "type": "signal", "bundle":{"name": "gmem2", "role": "ARPROT" }} , 
 	{ "name": "m_axi_gmem2_ARQOS", "direction": "out", "datatype": "sc_lv", "bitwidth":4, "type": "signal", "bundle":{"name": "gmem2", "role": "ARQOS" }} , 
 	{ "name": "m_axi_gmem2_ARREGION", "direction": "out", "datatype": "sc_lv", "bitwidth":4, "type": "signal", "bundle":{"name": "gmem2", "role": "ARREGION" }} , 
 	{ "name": "m_axi_gmem2_ARUSER", "direction": "out", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "gmem2", "role": "ARUSER" }} , 
 	{ "name": "m_axi_gmem2_RVALID", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "gmem2", "role": "RVALID" }} , 
 	{ "name": "m_axi_gmem2_RREADY", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "gmem2", "role": "RREADY" }} , 
 	{ "name": "m_axi_gmem2_RDATA", "direction": "in", "datatype": "sc_lv", "bitwidth":64, "type": "signal", "bundle":{"name": "gmem2", "role": "RDATA" }} , 
 	{ "name": "m_axi_gmem2_RLAST", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "gmem2", "role": "RLAST" }} , 
 	{ "name": "m_axi_gmem2_RID", "direction": "in", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "gmem2", "role": "RID" }} , 
 	{ "name": "m_axi_gmem2_RUSER", "direction": "in", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "gmem2", "role": "RUSER" }} , 
 	{ "name": "m_axi_gmem2_RRESP", "direction": "in", "datatype": "sc_lv", "bitwidth":2, "type": "signal", "bundle":{"name": "gmem2", "role": "RRESP" }} , 
 	{ "name": "m_axi_gmem2_BVALID", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "gmem2", "role": "BVALID" }} , 
 	{ "name": "m_axi_gmem2_BREADY", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "gmem2", "role": "BREADY" }} , 
 	{ "name": "m_axi_gmem2_BRESP", "direction": "in", "datatype": "sc_lv", "bitwidth":2, "type": "signal", "bundle":{"name": "gmem2", "role": "BRESP" }} , 
 	{ "name": "m_axi_gmem2_BID", "direction": "in", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "gmem2", "role": "BID" }} , 
 	{ "name": "m_axi_gmem2_BUSER", "direction": "in", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "gmem2", "role": "BUSER" }}  ]}

set RtlHierarchyInfo {[
	{"ID" : "0", "Level" : "0", "Path" : "`AUTOTB_DUT_INST", "Parent" : "", "Child" : ["1", "2", "3", "4", "5"],
		"CDFG" : "topkernel",
		"Protocol" : "ap_ctrl_hs",
		"ControlExist" : "1", "ap_start" : "1", "ap_ready" : "1", "ap_done" : "1", "ap_continue" : "0", "ap_idle" : "1",
		"Pipeline" : "None", "UnalignedPipeline" : "0", "RewindPipeline" : "0", "ProcessNetwork" : "0",
		"II" : "0",
		"VariableLatency" : "1", "ExactLatency" : "-1", "EstimateLatencyMin" : "57", "EstimateLatencyMax" : "197850356",
		"Combinational" : "0",
		"Datapath" : "0",
		"ClockEnable" : "0",
		"HasSubDataflow" : "0",
		"InDataflowNetwork" : "0",
		"HasNonBlockingOperation" : "0",
		"WaitState" : [
			{"State" : "ap_ST_fsm_state17", "FSM" : "ap_CS_fsm", "SubInstance" : "grp_partitionandreduce0_fu_178"}],
		"Port" : [
			{"Name" : "gmem0", "Type" : "MAXI", "Direction" : "IO",
				"SubConnect" : [
					{"ID" : "5", "SubInstance" : "grp_partitionandreduce0_fu_178", "Port" : "kvsourcedram_V"}]},
			{"Name" : "gmem1", "Type" : "MAXI", "Direction" : "IO",
				"SubConnect" : [
					{"ID" : "5", "SubInstance" : "grp_partitionandreduce0_fu_178", "Port" : "kvdestdram_V"}]},
			{"Name" : "gmem2", "Type" : "MAXI", "Direction" : "IO",
				"BlockSignal" : [
					{"Name" : "gmem2_blk_n_AR", "Type" : "RtlSignal"},
					{"Name" : "gmem2_blk_n_R", "Type" : "RtlSignal"}],
				"SubConnect" : [
					{"ID" : "5", "SubInstance" : "grp_partitionandreduce0_fu_178", "Port" : "kvstats"}]},
			{"Name" : "kvsourcedramA_V", "Type" : "None", "Direction" : "I"},
			{"Name" : "kvdestdramA_V", "Type" : "None", "Direction" : "I"},
			{"Name" : "kvstatsA", "Type" : "None", "Direction" : "I"}]},
	{"ID" : "1", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.topkernel_control_s_axi_U", "Parent" : "0"},
	{"ID" : "2", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.topkernel_gmem0_m_axi_U", "Parent" : "0"},
	{"ID" : "3", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.topkernel_gmem1_m_axi_U", "Parent" : "0"},
	{"ID" : "4", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.topkernel_gmem2_m_axi_U", "Parent" : "0"},
	{"ID" : "5", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178", "Parent" : "0", "Child" : ["6", "7", "8", "9", "10", "11", "12", "13", "14", "15", "16", "17", "18", "19", "20", "21", "22", "23", "24", "25", "26", "27", "28", "29", "30", "31", "32", "161", "174", "183", "232", "234", "236", "238", "240", "241", "242", "243", "244"],
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
					{"ID" : "234", "SubInstance" : "grp_savecapsules0_fu_1599", "Port" : "kvdram_V"},
					{"ID" : "238", "SubInstance" : "grp_readkeyvalues0_1_fu_1650", "Port" : "kvdram_V"},
					{"ID" : "174", "SubInstance" : "grp_savepartitions0_fu_1529", "Port" : "kvdram_V"},
					{"ID" : "236", "SubInstance" : "grp_readkeyvalues0_2_fu_1628", "Port" : "kvdram_V"},
					{"ID" : "232", "SubInstance" : "grp_readcapsules0_fu_1575", "Port" : "kvdram_V"}]},
			{"Name" : "kvsourcedram_V_offset", "Type" : "None", "Direction" : "I"},
			{"Name" : "kvdestdram_V", "Type" : "MAXI", "Direction" : "IO",
				"SubConnect" : [
					{"ID" : "241", "SubInstance" : "grp_readkeyvalues0_157_fu_1676", "Port" : "kvdram_V"},
					{"ID" : "240", "SubInstance" : "grp_savekeyvalues0_153_fu_1667", "Port" : "kvdram_V"}]},
			{"Name" : "kvdestdram_V_offset", "Type" : "None", "Direction" : "I"},
			{"Name" : "kvstats", "Type" : "MAXI", "Direction" : "IO",
				"BlockSignal" : [
					{"Name" : "kvstats_blk_n_AW", "Type" : "RtlSignal"},
					{"Name" : "kvstats_blk_n_W", "Type" : "RtlSignal"},
					{"Name" : "kvstats_blk_n_B", "Type" : "RtlSignal"},
					{"Name" : "kvstats_blk_n_AR", "Type" : "RtlSignal"},
					{"Name" : "kvstats_blk_n_R", "Type" : "RtlSignal"}],
				"SubConnect" : [
					{"ID" : "243", "SubInstance" : "grp_savekeyvalues0_1_fu_1695", "Port" : "dram"},
					{"ID" : "242", "SubInstance" : "grp_savekeyvalues0_2_fu_1685", "Port" : "dram"}]},
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
	{"ID" : "6", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.sourcebuffer00_0_V_U", "Parent" : "5"},
	{"ID" : "7", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.sourcebuffer00_1_V_U", "Parent" : "5"},
	{"ID" : "8", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.destbuffer00_V_U", "Parent" : "5"},
	{"ID" : "9", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.capsule00_key_U", "Parent" : "5"},
	{"ID" : "10", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.capsule00_value_U", "Parent" : "5"},
	{"ID" : "11", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.BIGcapsule00_V_U", "Parent" : "5"},
	{"ID" : "12", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.sourcebuffer01_0_V_U", "Parent" : "5"},
	{"ID" : "13", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.sourcebuffer01_1_V_U", "Parent" : "5"},
	{"ID" : "14", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.destbuffer01_V_U", "Parent" : "5"},
	{"ID" : "15", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.capsule01_key_U", "Parent" : "5"},
	{"ID" : "16", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.capsule01_value_U", "Parent" : "5"},
	{"ID" : "17", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.BIGcapsule01_V_U", "Parent" : "5"},
	{"ID" : "18", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.sourcebuffer02_0_V_U", "Parent" : "5"},
	{"ID" : "19", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.sourcebuffer02_1_V_U", "Parent" : "5"},
	{"ID" : "20", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.destbuffer02_V_U", "Parent" : "5"},
	{"ID" : "21", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.capsule02_key_U", "Parent" : "5"},
	{"ID" : "22", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.capsule02_value_U", "Parent" : "5"},
	{"ID" : "23", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.BIGcapsule02_V_U", "Parent" : "5"},
	{"ID" : "24", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.sourcebuffer03_0_V_U", "Parent" : "5"},
	{"ID" : "25", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.sourcebuffer03_1_V_U", "Parent" : "5"},
	{"ID" : "26", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.destbuffer03_V_U", "Parent" : "5"},
	{"ID" : "27", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.capsule03_key_U", "Parent" : "5"},
	{"ID" : "28", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.capsule03_value_U", "Parent" : "5"},
	{"ID" : "29", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.BIGcapsule03_V_U", "Parent" : "5"},
	{"ID" : "30", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.kvdeststats_tmp_key_U", "Parent" : "5"},
	{"ID" : "31", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.kvdeststats_tmp_valu_U", "Parent" : "5"},
	{"ID" : "32", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1486", "Parent" : "5", "Child" : ["33", "34", "35", "36", "37", "38", "39", "40", "41", "42", "43", "44", "45", "46", "47", "48", "49", "50", "51", "52", "53", "54", "55", "56", "57", "58", "59", "60", "61", "62", "63", "64", "65", "66", "67", "68", "69", "70", "71", "72", "73", "74", "75", "76", "77", "78", "79", "80", "81", "82", "83", "84", "85", "86", "87", "88", "89", "90", "91", "92", "93", "94", "95", "96", "97", "98", "99", "100", "101", "102", "103", "104", "105", "106", "107", "108", "109", "110", "111", "112", "113", "114", "115", "116", "117", "118", "119", "120", "121", "122", "123", "124", "125", "126", "127", "128", "129", "130", "131", "132", "133", "134", "135", "136", "137", "138", "139", "140", "141", "142", "143", "144", "145", "146", "147", "148", "149", "150", "151", "152", "153", "154", "155", "156", "157", "158", "159", "160"],
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
	{"ID" : "33", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1486.destbuffer00_key_U", "Parent" : "32"},
	{"ID" : "34", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1486.destbuffer00_value_U", "Parent" : "32"},
	{"ID" : "35", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1486.destbuffer01_key_U", "Parent" : "32"},
	{"ID" : "36", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1486.destbuffer01_value_U", "Parent" : "32"},
	{"ID" : "37", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1486.destbuffer02_key_U", "Parent" : "32"},
	{"ID" : "38", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1486.destbuffer02_value_U", "Parent" : "32"},
	{"ID" : "39", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1486.destbuffer03_key_U", "Parent" : "32"},
	{"ID" : "40", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1486.destbuffer03_value_U", "Parent" : "32"},
	{"ID" : "41", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1486.destbuffer04_key_U", "Parent" : "32"},
	{"ID" : "42", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1486.destbuffer04_value_U", "Parent" : "32"},
	{"ID" : "43", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1486.destbuffer05_key_U", "Parent" : "32"},
	{"ID" : "44", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1486.destbuffer05_value_U", "Parent" : "32"},
	{"ID" : "45", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1486.destbuffer06_key_U", "Parent" : "32"},
	{"ID" : "46", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1486.destbuffer06_value_U", "Parent" : "32"},
	{"ID" : "47", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1486.destbuffer07_key_U", "Parent" : "32"},
	{"ID" : "48", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1486.destbuffer07_value_U", "Parent" : "32"},
	{"ID" : "49", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1486.destbuffer10_key_U", "Parent" : "32"},
	{"ID" : "50", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1486.destbuffer10_value_U", "Parent" : "32"},
	{"ID" : "51", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1486.destbuffer11_key_U", "Parent" : "32"},
	{"ID" : "52", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1486.destbuffer11_value_U", "Parent" : "32"},
	{"ID" : "53", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1486.destbuffer12_key_U", "Parent" : "32"},
	{"ID" : "54", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1486.destbuffer12_value_U", "Parent" : "32"},
	{"ID" : "55", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1486.destbuffer13_key_U", "Parent" : "32"},
	{"ID" : "56", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1486.destbuffer13_value_U", "Parent" : "32"},
	{"ID" : "57", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1486.destbuffer14_key_U", "Parent" : "32"},
	{"ID" : "58", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1486.destbuffer14_value_U", "Parent" : "32"},
	{"ID" : "59", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1486.destbuffer15_key_U", "Parent" : "32"},
	{"ID" : "60", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1486.destbuffer15_value_U", "Parent" : "32"},
	{"ID" : "61", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1486.destbuffer16_key_U", "Parent" : "32"},
	{"ID" : "62", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1486.destbuffer16_value_U", "Parent" : "32"},
	{"ID" : "63", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1486.destbuffer17_key_U", "Parent" : "32"},
	{"ID" : "64", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1486.destbuffer17_value_U", "Parent" : "32"},
	{"ID" : "65", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1486.destbuffer20_key_U", "Parent" : "32"},
	{"ID" : "66", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1486.destbuffer20_value_U", "Parent" : "32"},
	{"ID" : "67", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1486.destbuffer21_key_U", "Parent" : "32"},
	{"ID" : "68", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1486.destbuffer21_value_U", "Parent" : "32"},
	{"ID" : "69", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1486.destbuffer22_key_U", "Parent" : "32"},
	{"ID" : "70", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1486.destbuffer22_value_U", "Parent" : "32"},
	{"ID" : "71", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1486.destbuffer23_key_U", "Parent" : "32"},
	{"ID" : "72", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1486.destbuffer23_value_U", "Parent" : "32"},
	{"ID" : "73", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1486.destbuffer24_key_U", "Parent" : "32"},
	{"ID" : "74", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1486.destbuffer24_value_U", "Parent" : "32"},
	{"ID" : "75", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1486.destbuffer25_key_U", "Parent" : "32"},
	{"ID" : "76", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1486.destbuffer25_value_U", "Parent" : "32"},
	{"ID" : "77", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1486.destbuffer26_key_U", "Parent" : "32"},
	{"ID" : "78", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1486.destbuffer26_value_U", "Parent" : "32"},
	{"ID" : "79", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1486.destbuffer27_key_U", "Parent" : "32"},
	{"ID" : "80", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1486.destbuffer27_value_U", "Parent" : "32"},
	{"ID" : "81", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1486.destbuffer30_key_U", "Parent" : "32"},
	{"ID" : "82", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1486.destbuffer30_value_U", "Parent" : "32"},
	{"ID" : "83", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1486.destbuffer31_key_U", "Parent" : "32"},
	{"ID" : "84", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1486.destbuffer31_value_U", "Parent" : "32"},
	{"ID" : "85", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1486.destbuffer32_key_U", "Parent" : "32"},
	{"ID" : "86", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1486.destbuffer32_value_U", "Parent" : "32"},
	{"ID" : "87", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1486.destbuffer33_key_U", "Parent" : "32"},
	{"ID" : "88", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1486.destbuffer33_value_U", "Parent" : "32"},
	{"ID" : "89", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1486.destbuffer34_key_U", "Parent" : "32"},
	{"ID" : "90", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1486.destbuffer34_value_U", "Parent" : "32"},
	{"ID" : "91", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1486.destbuffer35_key_U", "Parent" : "32"},
	{"ID" : "92", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1486.destbuffer35_value_U", "Parent" : "32"},
	{"ID" : "93", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1486.destbuffer36_key_U", "Parent" : "32"},
	{"ID" : "94", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1486.destbuffer36_value_U", "Parent" : "32"},
	{"ID" : "95", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1486.destbuffer37_key_U", "Parent" : "32"},
	{"ID" : "96", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1486.destbuffer37_value_U", "Parent" : "32"},
	{"ID" : "97", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1486.topkernel_mux_83_32_1_1_U165", "Parent" : "32"},
	{"ID" : "98", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1486.topkernel_mux_83_32_1_1_U166", "Parent" : "32"},
	{"ID" : "99", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1486.topkernel_mux_83_32_1_1_U167", "Parent" : "32"},
	{"ID" : "100", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1486.topkernel_mux_83_32_1_1_U168", "Parent" : "32"},
	{"ID" : "101", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1486.topkernel_mux_83_32_1_1_U169", "Parent" : "32"},
	{"ID" : "102", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1486.topkernel_mux_83_32_1_1_U170", "Parent" : "32"},
	{"ID" : "103", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1486.topkernel_mux_83_32_1_1_U171", "Parent" : "32"},
	{"ID" : "104", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1486.topkernel_mux_83_32_1_1_U172", "Parent" : "32"},
	{"ID" : "105", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1486.topkernel_mux_83_32_1_1_U173", "Parent" : "32"},
	{"ID" : "106", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1486.topkernel_mux_83_32_1_1_U174", "Parent" : "32"},
	{"ID" : "107", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1486.topkernel_mux_83_32_1_1_U175", "Parent" : "32"},
	{"ID" : "108", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1486.topkernel_mux_83_32_1_1_U176", "Parent" : "32"},
	{"ID" : "109", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1486.topkernel_mux_83_32_1_1_U177", "Parent" : "32"},
	{"ID" : "110", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1486.topkernel_mux_83_32_1_1_U178", "Parent" : "32"},
	{"ID" : "111", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1486.topkernel_mux_83_32_1_1_U179", "Parent" : "32"},
	{"ID" : "112", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1486.topkernel_mux_83_32_1_1_U180", "Parent" : "32"},
	{"ID" : "113", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1486.topkernel_mux_83_32_1_1_U181", "Parent" : "32"},
	{"ID" : "114", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1486.topkernel_mux_83_32_1_1_U182", "Parent" : "32"},
	{"ID" : "115", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1486.topkernel_mux_83_32_1_1_U183", "Parent" : "32"},
	{"ID" : "116", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1486.topkernel_mux_83_32_1_1_U184", "Parent" : "32"},
	{"ID" : "117", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1486.topkernel_mux_83_32_1_1_U185", "Parent" : "32"},
	{"ID" : "118", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1486.topkernel_mux_83_32_1_1_U186", "Parent" : "32"},
	{"ID" : "119", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1486.topkernel_mux_83_32_1_1_U187", "Parent" : "32"},
	{"ID" : "120", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1486.topkernel_mux_83_32_1_1_U188", "Parent" : "32"},
	{"ID" : "121", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1486.topkernel_mux_83_32_1_1_U189", "Parent" : "32"},
	{"ID" : "122", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1486.topkernel_mux_83_32_1_1_U190", "Parent" : "32"},
	{"ID" : "123", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1486.topkernel_mux_83_32_1_1_U191", "Parent" : "32"},
	{"ID" : "124", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1486.topkernel_mux_83_32_1_1_U192", "Parent" : "32"},
	{"ID" : "125", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1486.topkernel_mux_83_32_1_1_U193", "Parent" : "32"},
	{"ID" : "126", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1486.topkernel_mux_83_32_1_1_U194", "Parent" : "32"},
	{"ID" : "127", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1486.topkernel_mux_83_32_1_1_U195", "Parent" : "32"},
	{"ID" : "128", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1486.topkernel_mux_83_32_1_1_U196", "Parent" : "32"},
	{"ID" : "129", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1486.topkernel_mux_83_32_1_1_U197", "Parent" : "32"},
	{"ID" : "130", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1486.topkernel_mux_83_32_1_1_U198", "Parent" : "32"},
	{"ID" : "131", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1486.topkernel_mux_83_32_1_1_U199", "Parent" : "32"},
	{"ID" : "132", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1486.topkernel_mux_83_32_1_1_U200", "Parent" : "32"},
	{"ID" : "133", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1486.topkernel_mux_83_32_1_1_U201", "Parent" : "32"},
	{"ID" : "134", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1486.topkernel_mux_83_32_1_1_U202", "Parent" : "32"},
	{"ID" : "135", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1486.topkernel_mux_83_32_1_1_U203", "Parent" : "32"},
	{"ID" : "136", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1486.topkernel_mux_83_32_1_1_U204", "Parent" : "32"},
	{"ID" : "137", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1486.topkernel_mux_83_32_1_1_U205", "Parent" : "32"},
	{"ID" : "138", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1486.topkernel_mux_83_32_1_1_U206", "Parent" : "32"},
	{"ID" : "139", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1486.topkernel_mux_83_32_1_1_U207", "Parent" : "32"},
	{"ID" : "140", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1486.topkernel_mux_83_32_1_1_U208", "Parent" : "32"},
	{"ID" : "141", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1486.topkernel_mux_83_32_1_1_U209", "Parent" : "32"},
	{"ID" : "142", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1486.topkernel_mux_83_32_1_1_U210", "Parent" : "32"},
	{"ID" : "143", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1486.topkernel_mux_83_32_1_1_U211", "Parent" : "32"},
	{"ID" : "144", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1486.topkernel_mux_83_32_1_1_U212", "Parent" : "32"},
	{"ID" : "145", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1486.topkernel_mux_83_32_1_1_U213", "Parent" : "32"},
	{"ID" : "146", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1486.topkernel_mux_83_32_1_1_U214", "Parent" : "32"},
	{"ID" : "147", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1486.topkernel_mux_83_32_1_1_U215", "Parent" : "32"},
	{"ID" : "148", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1486.topkernel_mux_83_32_1_1_U216", "Parent" : "32"},
	{"ID" : "149", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1486.topkernel_mux_83_32_1_1_U217", "Parent" : "32"},
	{"ID" : "150", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1486.topkernel_mux_83_32_1_1_U218", "Parent" : "32"},
	{"ID" : "151", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1486.topkernel_mux_83_32_1_1_U219", "Parent" : "32"},
	{"ID" : "152", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1486.topkernel_mux_83_32_1_1_U220", "Parent" : "32"},
	{"ID" : "153", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1486.topkernel_mux_83_32_1_1_U221", "Parent" : "32"},
	{"ID" : "154", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1486.topkernel_mux_83_32_1_1_U222", "Parent" : "32"},
	{"ID" : "155", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1486.topkernel_mux_83_32_1_1_U223", "Parent" : "32"},
	{"ID" : "156", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1486.topkernel_mux_83_32_1_1_U224", "Parent" : "32"},
	{"ID" : "157", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1486.topkernel_mux_83_32_1_1_U225", "Parent" : "32"},
	{"ID" : "158", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1486.topkernel_mux_83_32_1_1_U226", "Parent" : "32"},
	{"ID" : "159", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1486.topkernel_mux_83_32_1_1_U227", "Parent" : "32"},
	{"ID" : "160", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1486.topkernel_mux_83_32_1_1_U228", "Parent" : "32"},
	{"ID" : "161", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_partitionkeyvalues00_fu_1501", "Parent" : "5", "Child" : ["162", "163", "164", "165", "166", "167", "168", "169", "170", "171", "172", "173"],
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
	{"ID" : "162", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_partitionkeyvalues00_fu_1501.topkernel_mux_325_32_1_1_U85", "Parent" : "161"},
	{"ID" : "163", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_partitionkeyvalues00_fu_1501.topkernel_mux_325_32_1_1_U86", "Parent" : "161"},
	{"ID" : "164", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_partitionkeyvalues00_fu_1501.topkernel_mux_325_32_1_1_U87", "Parent" : "161"},
	{"ID" : "165", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_partitionkeyvalues00_fu_1501.topkernel_mux_325_32_1_1_U88", "Parent" : "161"},
	{"ID" : "166", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_partitionkeyvalues00_fu_1501.topkernel_mux_325_32_1_1_U89", "Parent" : "161"},
	{"ID" : "167", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_partitionkeyvalues00_fu_1501.topkernel_mux_325_32_1_1_U90", "Parent" : "161"},
	{"ID" : "168", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_partitionkeyvalues00_fu_1501.topkernel_mux_325_32_1_1_U91", "Parent" : "161"},
	{"ID" : "169", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_partitionkeyvalues00_fu_1501.topkernel_mux_325_32_1_1_U92", "Parent" : "161"},
	{"ID" : "170", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_partitionkeyvalues00_fu_1501.topkernel_mux_325_32_1_1_U93", "Parent" : "161"},
	{"ID" : "171", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_partitionkeyvalues00_fu_1501.topkernel_mux_325_32_1_1_U94", "Parent" : "161"},
	{"ID" : "172", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_partitionkeyvalues00_fu_1501.topkernel_mux_325_32_1_1_U95", "Parent" : "161"},
	{"ID" : "173", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_partitionkeyvalues00_fu_1501.topkernel_mux_325_32_1_1_U96", "Parent" : "161"},
	{"ID" : "174", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_savepartitions0_fu_1529", "Parent" : "5", "Child" : ["175", "176", "177", "178", "179", "180", "181", "182"],
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
	{"ID" : "175", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_savepartitions0_fu_1529.topkernel_urem_12ns_8ns_12_16_1_U119", "Parent" : "174"},
	{"ID" : "176", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_savepartitions0_fu_1529.topkernel_urem_12ns_8ns_12_16_1_U120", "Parent" : "174"},
	{"ID" : "177", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_savepartitions0_fu_1529.topkernel_urem_12ns_8ns_12_16_1_U121", "Parent" : "174"},
	{"ID" : "178", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_savepartitions0_fu_1529.topkernel_urem_12ns_8ns_12_16_1_U122", "Parent" : "174"},
	{"ID" : "179", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_savepartitions0_fu_1529.topkernel_urem_12ns_8ns_12_16_1_U123", "Parent" : "174"},
	{"ID" : "180", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_savepartitions0_fu_1529.topkernel_urem_12ns_8ns_12_16_1_U124", "Parent" : "174"},
	{"ID" : "181", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_savepartitions0_fu_1529.topkernel_urem_12ns_8ns_12_16_1_U125", "Parent" : "174"},
	{"ID" : "182", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_savepartitions0_fu_1529.topkernel_urem_12ns_8ns_12_16_1_U126", "Parent" : "174"},
	{"ID" : "183", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_collectstats00_fu_1551", "Parent" : "5", "Child" : ["184", "185", "186", "187", "188", "189", "190", "191", "192", "193", "194", "195", "196", "197", "198", "199", "200", "201", "202", "203", "204", "205", "206", "207", "208", "209", "210", "211", "212", "213", "214", "215", "216", "217", "218", "219", "220", "221", "222", "223", "224", "225", "226", "227", "228", "229", "230", "231"],
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
	{"ID" : "184", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_collectstats00_fu_1551.sizes00_U", "Parent" : "183"},
	{"ID" : "185", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_collectstats00_fu_1551.sizes01_U", "Parent" : "183"},
	{"ID" : "186", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_collectstats00_fu_1551.sizes02_U", "Parent" : "183"},
	{"ID" : "187", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_collectstats00_fu_1551.sizes03_U", "Parent" : "183"},
	{"ID" : "188", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_collectstats00_fu_1551.sizes04_U", "Parent" : "183"},
	{"ID" : "189", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_collectstats00_fu_1551.sizes05_U", "Parent" : "183"},
	{"ID" : "190", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_collectstats00_fu_1551.sizes06_U", "Parent" : "183"},
	{"ID" : "191", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_collectstats00_fu_1551.sizes07_U", "Parent" : "183"},
	{"ID" : "192", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_collectstats00_fu_1551.sizes10_U", "Parent" : "183"},
	{"ID" : "193", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_collectstats00_fu_1551.sizes11_U", "Parent" : "183"},
	{"ID" : "194", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_collectstats00_fu_1551.sizes12_U", "Parent" : "183"},
	{"ID" : "195", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_collectstats00_fu_1551.sizes13_U", "Parent" : "183"},
	{"ID" : "196", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_collectstats00_fu_1551.sizes14_U", "Parent" : "183"},
	{"ID" : "197", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_collectstats00_fu_1551.sizes15_U", "Parent" : "183"},
	{"ID" : "198", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_collectstats00_fu_1551.sizes16_U", "Parent" : "183"},
	{"ID" : "199", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_collectstats00_fu_1551.sizes17_U", "Parent" : "183"},
	{"ID" : "200", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_collectstats00_fu_1551.sizes20_U", "Parent" : "183"},
	{"ID" : "201", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_collectstats00_fu_1551.sizes21_U", "Parent" : "183"},
	{"ID" : "202", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_collectstats00_fu_1551.sizes22_U", "Parent" : "183"},
	{"ID" : "203", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_collectstats00_fu_1551.sizes23_U", "Parent" : "183"},
	{"ID" : "204", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_collectstats00_fu_1551.sizes24_U", "Parent" : "183"},
	{"ID" : "205", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_collectstats00_fu_1551.sizes25_U", "Parent" : "183"},
	{"ID" : "206", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_collectstats00_fu_1551.sizes26_U", "Parent" : "183"},
	{"ID" : "207", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_collectstats00_fu_1551.sizes27_U", "Parent" : "183"},
	{"ID" : "208", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_collectstats00_fu_1551.sizes30_U", "Parent" : "183"},
	{"ID" : "209", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_collectstats00_fu_1551.sizes31_U", "Parent" : "183"},
	{"ID" : "210", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_collectstats00_fu_1551.sizes32_U", "Parent" : "183"},
	{"ID" : "211", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_collectstats00_fu_1551.sizes33_U", "Parent" : "183"},
	{"ID" : "212", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_collectstats00_fu_1551.sizes34_U", "Parent" : "183"},
	{"ID" : "213", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_collectstats00_fu_1551.sizes35_U", "Parent" : "183"},
	{"ID" : "214", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_collectstats00_fu_1551.sizes36_U", "Parent" : "183"},
	{"ID" : "215", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_collectstats00_fu_1551.sizes37_U", "Parent" : "183"},
	{"ID" : "216", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_collectstats00_fu_1551.grp_getpartition_fu_1471", "Parent" : "183",
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
	{"ID" : "217", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_collectstats00_fu_1551.grp_getpartition_fu_1478", "Parent" : "183",
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
	{"ID" : "218", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_collectstats00_fu_1551.grp_getpartition_fu_1485", "Parent" : "183",
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
	{"ID" : "219", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_collectstats00_fu_1551.grp_getpartition_fu_1492", "Parent" : "183",
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
	{"ID" : "220", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_collectstats00_fu_1551.grp_getpartition_fu_1499", "Parent" : "183",
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
	{"ID" : "221", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_collectstats00_fu_1551.grp_getpartition_fu_1506", "Parent" : "183",
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
	{"ID" : "222", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_collectstats00_fu_1551.grp_getpartition_fu_1513", "Parent" : "183",
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
	{"ID" : "223", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_collectstats00_fu_1551.grp_getpartition_fu_1520", "Parent" : "183",
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
	{"ID" : "224", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_collectstats00_fu_1551.grp_getpartition_fu_1527", "Parent" : "183",
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
	{"ID" : "225", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_collectstats00_fu_1551.grp_getpartition_fu_1534", "Parent" : "183",
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
	{"ID" : "226", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_collectstats00_fu_1551.grp_getpartition_fu_1541", "Parent" : "183",
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
	{"ID" : "227", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_collectstats00_fu_1551.grp_getpartition_fu_1548", "Parent" : "183",
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
	{"ID" : "228", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_collectstats00_fu_1551.grp_getpartition_fu_1555", "Parent" : "183",
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
	{"ID" : "229", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_collectstats00_fu_1551.grp_getpartition_fu_1562", "Parent" : "183",
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
	{"ID" : "230", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_collectstats00_fu_1551.grp_getpartition_fu_1569", "Parent" : "183",
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
	{"ID" : "231", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_collectstats00_fu_1551.grp_getpartition_fu_1576", "Parent" : "183",
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
	{"ID" : "232", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_readcapsules0_fu_1575", "Parent" : "5", "Child" : ["233"],
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
					{"ID" : "233", "SubInstance" : "grp_readkeyvalues0_fu_418", "Port" : "kvdram_V"}]},
			{"Name" : "kvdram_V_offset", "Type" : "None", "Direction" : "I"},
			{"Name" : "BIGcapsule0_V", "Type" : "Memory", "Direction" : "IO",
				"SubConnect" : [
					{"ID" : "233", "SubInstance" : "grp_readkeyvalues0_fu_418", "Port" : "buffer_V"}]},
			{"Name" : "BIGcapsule1_V", "Type" : "Memory", "Direction" : "IO",
				"SubConnect" : [
					{"ID" : "233", "SubInstance" : "grp_readkeyvalues0_fu_418", "Port" : "buffer_V"}]},
			{"Name" : "BIGcapsule2_V", "Type" : "Memory", "Direction" : "IO",
				"SubConnect" : [
					{"ID" : "233", "SubInstance" : "grp_readkeyvalues0_fu_418", "Port" : "buffer_V"}]},
			{"Name" : "BIGcapsule3_V", "Type" : "Memory", "Direction" : "IO",
				"SubConnect" : [
					{"ID" : "233", "SubInstance" : "grp_readkeyvalues0_fu_418", "Port" : "buffer_V"}]},
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
	{"ID" : "233", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_readcapsules0_fu_1575.grp_readkeyvalues0_fu_418", "Parent" : "232",
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
	{"ID" : "234", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_savecapsules0_fu_1599", "Parent" : "5", "Child" : ["235"],
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
					{"ID" : "235", "SubInstance" : "grp_savekeyvalues0_fu_887", "Port" : "kvdram_V"}]},
			{"Name" : "kvdram_V_offset", "Type" : "None", "Direction" : "I"},
			{"Name" : "BIGcapsule0_V", "Type" : "Memory", "Direction" : "IO",
				"SubConnect" : [
					{"ID" : "235", "SubInstance" : "grp_savekeyvalues0_fu_887", "Port" : "buffer_V"}]},
			{"Name" : "BIGcapsule1_V", "Type" : "Memory", "Direction" : "IO",
				"SubConnect" : [
					{"ID" : "235", "SubInstance" : "grp_savekeyvalues0_fu_887", "Port" : "buffer_V"}]},
			{"Name" : "BIGcapsule2_V", "Type" : "Memory", "Direction" : "IO",
				"SubConnect" : [
					{"ID" : "235", "SubInstance" : "grp_savekeyvalues0_fu_887", "Port" : "buffer_V"}]},
			{"Name" : "BIGcapsule3_V", "Type" : "Memory", "Direction" : "IO",
				"SubConnect" : [
					{"ID" : "235", "SubInstance" : "grp_savekeyvalues0_fu_887", "Port" : "buffer_V"}]},
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
	{"ID" : "235", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_savecapsules0_fu_1599.grp_savekeyvalues0_fu_887", "Parent" : "234",
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
	{"ID" : "236", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_readkeyvalues0_2_fu_1628", "Parent" : "5", "Child" : ["237"],
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
					{"ID" : "237", "SubInstance" : "grp_readkeyvalues0_158_fu_136", "Port" : "kvdram_V"}]},
			{"Name" : "kvdram_V_offset", "Type" : "None", "Direction" : "I"},
			{"Name" : "buffer0_V", "Type" : "Memory", "Direction" : "O",
				"SubConnect" : [
					{"ID" : "237", "SubInstance" : "grp_readkeyvalues0_158_fu_136", "Port" : "buffer_V"}]},
			{"Name" : "buffer1_V", "Type" : "Memory", "Direction" : "O",
				"SubConnect" : [
					{"ID" : "237", "SubInstance" : "grp_readkeyvalues0_158_fu_136", "Port" : "buffer_V"}]},
			{"Name" : "buffer2_V", "Type" : "Memory", "Direction" : "O",
				"SubConnect" : [
					{"ID" : "237", "SubInstance" : "grp_readkeyvalues0_158_fu_136", "Port" : "buffer_V"}]},
			{"Name" : "buffer3_V", "Type" : "Memory", "Direction" : "O",
				"SubConnect" : [
					{"ID" : "237", "SubInstance" : "grp_readkeyvalues0_158_fu_136", "Port" : "buffer_V"}]},
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
	{"ID" : "237", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_readkeyvalues0_2_fu_1628.grp_readkeyvalues0_158_fu_136", "Parent" : "236",
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
	{"ID" : "238", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_readkeyvalues0_1_fu_1650", "Parent" : "5", "Child" : ["239"],
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
					{"ID" : "239", "SubInstance" : "grp_readkeyvalues0_158_fu_94", "Port" : "kvdram_V"}]},
			{"Name" : "kvdram_V_offset", "Type" : "None", "Direction" : "I"},
			{"Name" : "buffer0_V", "Type" : "Memory", "Direction" : "O",
				"SubConnect" : [
					{"ID" : "239", "SubInstance" : "grp_readkeyvalues0_158_fu_94", "Port" : "buffer_V"}]},
			{"Name" : "buffer1_V", "Type" : "Memory", "Direction" : "O",
				"SubConnect" : [
					{"ID" : "239", "SubInstance" : "grp_readkeyvalues0_158_fu_94", "Port" : "buffer_V"}]},
			{"Name" : "buffer2_V", "Type" : "Memory", "Direction" : "O",
				"SubConnect" : [
					{"ID" : "239", "SubInstance" : "grp_readkeyvalues0_158_fu_94", "Port" : "buffer_V"}]},
			{"Name" : "buffer3_V", "Type" : "Memory", "Direction" : "O",
				"SubConnect" : [
					{"ID" : "239", "SubInstance" : "grp_readkeyvalues0_158_fu_94", "Port" : "buffer_V"}]},
			{"Name" : "baseaddress", "Type" : "None", "Direction" : "I"},
			{"Name" : "offset_kvs", "Type" : "None", "Direction" : "I"},
			{"Name" : "travstate_i_kvs", "Type" : "None", "Direction" : "I"},
			{"Name" : "travstate_end_kvs", "Type" : "None", "Direction" : "I"}]},
	{"ID" : "239", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_readkeyvalues0_1_fu_1650.grp_readkeyvalues0_158_fu_94", "Parent" : "238",
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
	{"ID" : "240", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_savekeyvalues0_153_fu_1667", "Parent" : "5",
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
	{"ID" : "241", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_readkeyvalues0_157_fu_1676", "Parent" : "5",
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
	{"ID" : "242", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_savekeyvalues0_2_fu_1685", "Parent" : "5",
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
	{"ID" : "243", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_savekeyvalues0_1_fu_1695", "Parent" : "5",
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
	{"ID" : "244", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.topkernel_mux_42_32_1_1_U255", "Parent" : "5"}]}


set ArgLastReadFirstWriteLatency {
	topkernel {
		gmem0 {Type IO LastRead 73 FirstWrite -1}
		gmem1 {Type IO LastRead 9 FirstWrite -1}
		gmem2 {Type IO LastRead 44 FirstWrite 2}
		kvsourcedramA_V {Type I LastRead 0 FirstWrite -1}
		kvdestdramA_V {Type I LastRead 0 FirstWrite -1}
		kvstatsA {Type I LastRead 0 FirstWrite -1}}
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
	{"Name" : "Latency", "Min" : "57", "Max" : "197850356"}
	, {"Name" : "Interval", "Min" : "58", "Max" : "197850357"}
]}

set PipelineEnableSignalInfo {[
]}

set Spec2ImplPortList { 
	gmem0 { m_axi {  { m_axi_gmem0_AWVALID VALID 1 1 }  { m_axi_gmem0_AWREADY READY 0 1 }  { m_axi_gmem0_AWADDR ADDR 1 32 }  { m_axi_gmem0_AWID ID 1 1 }  { m_axi_gmem0_AWLEN LEN 1 8 }  { m_axi_gmem0_AWSIZE SIZE 1 3 }  { m_axi_gmem0_AWBURST BURST 1 2 }  { m_axi_gmem0_AWLOCK LOCK 1 2 }  { m_axi_gmem0_AWCACHE CACHE 1 4 }  { m_axi_gmem0_AWPROT PROT 1 3 }  { m_axi_gmem0_AWQOS QOS 1 4 }  { m_axi_gmem0_AWREGION REGION 1 4 }  { m_axi_gmem0_AWUSER USER 1 1 }  { m_axi_gmem0_WVALID VALID 1 1 }  { m_axi_gmem0_WREADY READY 0 1 }  { m_axi_gmem0_WDATA DATA 1 512 }  { m_axi_gmem0_WSTRB STRB 1 64 }  { m_axi_gmem0_WLAST LAST 1 1 }  { m_axi_gmem0_WID ID 1 1 }  { m_axi_gmem0_WUSER USER 1 1 }  { m_axi_gmem0_ARVALID VALID 1 1 }  { m_axi_gmem0_ARREADY READY 0 1 }  { m_axi_gmem0_ARADDR ADDR 1 32 }  { m_axi_gmem0_ARID ID 1 1 }  { m_axi_gmem0_ARLEN LEN 1 8 }  { m_axi_gmem0_ARSIZE SIZE 1 3 }  { m_axi_gmem0_ARBURST BURST 1 2 }  { m_axi_gmem0_ARLOCK LOCK 1 2 }  { m_axi_gmem0_ARCACHE CACHE 1 4 }  { m_axi_gmem0_ARPROT PROT 1 3 }  { m_axi_gmem0_ARQOS QOS 1 4 }  { m_axi_gmem0_ARREGION REGION 1 4 }  { m_axi_gmem0_ARUSER USER 1 1 }  { m_axi_gmem0_RVALID VALID 0 1 }  { m_axi_gmem0_RREADY READY 1 1 }  { m_axi_gmem0_RDATA DATA 0 512 }  { m_axi_gmem0_RLAST LAST 0 1 }  { m_axi_gmem0_RID ID 0 1 }  { m_axi_gmem0_RUSER USER 0 1 }  { m_axi_gmem0_RRESP RESP 0 2 }  { m_axi_gmem0_BVALID VALID 0 1 }  { m_axi_gmem0_BREADY READY 1 1 }  { m_axi_gmem0_BRESP RESP 0 2 }  { m_axi_gmem0_BID ID 0 1 }  { m_axi_gmem0_BUSER USER 0 1 } } }
	gmem1 { m_axi {  { m_axi_gmem1_AWVALID VALID 1 1 }  { m_axi_gmem1_AWREADY READY 0 1 }  { m_axi_gmem1_AWADDR ADDR 1 32 }  { m_axi_gmem1_AWID ID 1 1 }  { m_axi_gmem1_AWLEN LEN 1 8 }  { m_axi_gmem1_AWSIZE SIZE 1 3 }  { m_axi_gmem1_AWBURST BURST 1 2 }  { m_axi_gmem1_AWLOCK LOCK 1 2 }  { m_axi_gmem1_AWCACHE CACHE 1 4 }  { m_axi_gmem1_AWPROT PROT 1 3 }  { m_axi_gmem1_AWQOS QOS 1 4 }  { m_axi_gmem1_AWREGION REGION 1 4 }  { m_axi_gmem1_AWUSER USER 1 1 }  { m_axi_gmem1_WVALID VALID 1 1 }  { m_axi_gmem1_WREADY READY 0 1 }  { m_axi_gmem1_WDATA DATA 1 512 }  { m_axi_gmem1_WSTRB STRB 1 64 }  { m_axi_gmem1_WLAST LAST 1 1 }  { m_axi_gmem1_WID ID 1 1 }  { m_axi_gmem1_WUSER USER 1 1 }  { m_axi_gmem1_ARVALID VALID 1 1 }  { m_axi_gmem1_ARREADY READY 0 1 }  { m_axi_gmem1_ARADDR ADDR 1 32 }  { m_axi_gmem1_ARID ID 1 1 }  { m_axi_gmem1_ARLEN LEN 1 8 }  { m_axi_gmem1_ARSIZE SIZE 1 3 }  { m_axi_gmem1_ARBURST BURST 1 2 }  { m_axi_gmem1_ARLOCK LOCK 1 2 }  { m_axi_gmem1_ARCACHE CACHE 1 4 }  { m_axi_gmem1_ARPROT PROT 1 3 }  { m_axi_gmem1_ARQOS QOS 1 4 }  { m_axi_gmem1_ARREGION REGION 1 4 }  { m_axi_gmem1_ARUSER USER 1 1 }  { m_axi_gmem1_RVALID VALID 0 1 }  { m_axi_gmem1_RREADY READY 1 1 }  { m_axi_gmem1_RDATA DATA 0 512 }  { m_axi_gmem1_RLAST LAST 0 1 }  { m_axi_gmem1_RID ID 0 1 }  { m_axi_gmem1_RUSER USER 0 1 }  { m_axi_gmem1_RRESP RESP 0 2 }  { m_axi_gmem1_BVALID VALID 0 1 }  { m_axi_gmem1_BREADY READY 1 1 }  { m_axi_gmem1_BRESP RESP 0 2 }  { m_axi_gmem1_BID ID 0 1 }  { m_axi_gmem1_BUSER USER 0 1 } } }
	gmem2 { m_axi {  { m_axi_gmem2_AWVALID VALID 1 1 }  { m_axi_gmem2_AWREADY READY 0 1 }  { m_axi_gmem2_AWADDR ADDR 1 32 }  { m_axi_gmem2_AWID ID 1 1 }  { m_axi_gmem2_AWLEN LEN 1 8 }  { m_axi_gmem2_AWSIZE SIZE 1 3 }  { m_axi_gmem2_AWBURST BURST 1 2 }  { m_axi_gmem2_AWLOCK LOCK 1 2 }  { m_axi_gmem2_AWCACHE CACHE 1 4 }  { m_axi_gmem2_AWPROT PROT 1 3 }  { m_axi_gmem2_AWQOS QOS 1 4 }  { m_axi_gmem2_AWREGION REGION 1 4 }  { m_axi_gmem2_AWUSER USER 1 1 }  { m_axi_gmem2_WVALID VALID 1 1 }  { m_axi_gmem2_WREADY READY 0 1 }  { m_axi_gmem2_WDATA DATA 1 64 }  { m_axi_gmem2_WSTRB STRB 1 8 }  { m_axi_gmem2_WLAST LAST 1 1 }  { m_axi_gmem2_WID ID 1 1 }  { m_axi_gmem2_WUSER USER 1 1 }  { m_axi_gmem2_ARVALID VALID 1 1 }  { m_axi_gmem2_ARREADY READY 0 1 }  { m_axi_gmem2_ARADDR ADDR 1 32 }  { m_axi_gmem2_ARID ID 1 1 }  { m_axi_gmem2_ARLEN LEN 1 8 }  { m_axi_gmem2_ARSIZE SIZE 1 3 }  { m_axi_gmem2_ARBURST BURST 1 2 }  { m_axi_gmem2_ARLOCK LOCK 1 2 }  { m_axi_gmem2_ARCACHE CACHE 1 4 }  { m_axi_gmem2_ARPROT PROT 1 3 }  { m_axi_gmem2_ARQOS QOS 1 4 }  { m_axi_gmem2_ARREGION REGION 1 4 }  { m_axi_gmem2_ARUSER USER 1 1 }  { m_axi_gmem2_RVALID VALID 0 1 }  { m_axi_gmem2_RREADY READY 1 1 }  { m_axi_gmem2_RDATA DATA 0 64 }  { m_axi_gmem2_RLAST LAST 0 1 }  { m_axi_gmem2_RID ID 0 1 }  { m_axi_gmem2_RUSER USER 0 1 }  { m_axi_gmem2_RRESP RESP 0 2 }  { m_axi_gmem2_BVALID VALID 0 1 }  { m_axi_gmem2_BREADY READY 1 1 }  { m_axi_gmem2_BRESP RESP 0 2 }  { m_axi_gmem2_BID ID 0 1 }  { m_axi_gmem2_BUSER USER 0 1 } } }
}

set busDeadlockParameterList { 
	{ gmem0 { NUM_READ_OUTSTANDING 16 NUM_WRITE_OUTSTANDING 16 MAX_READ_BURST_LENGTH 64 MAX_WRITE_BURST_LENGTH 64 } } \
	{ gmem1 { NUM_READ_OUTSTANDING 16 NUM_WRITE_OUTSTANDING 16 MAX_READ_BURST_LENGTH 64 MAX_WRITE_BURST_LENGTH 64 } } \
	{ gmem2 { NUM_READ_OUTSTANDING 16 NUM_WRITE_OUTSTANDING 16 MAX_READ_BURST_LENGTH 16 MAX_WRITE_BURST_LENGTH 16 } } \
}

# RTL port scheduling information:
set fifoSchedulingInfoList { 
}

# RTL bus port read request latency information:
set busReadReqLatencyList { 
	{ gmem0 1 }
	{ gmem1 1 }
	{ gmem2 1 }
}

# RTL bus port write response latency information:
set busWriteResLatencyList { 
	{ gmem0 1 }
	{ gmem1 1 }
	{ gmem2 1 }
}

# RTL array port load latency information:
set memoryLoadLatencyList { 
}
set moduleName topkernel
set isTopModule 1
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
set C_modelName {topkernel}
set C_modelType { void 0 }
set C_modelArgList {
	{ gmem0 int 512 regular {axi_master 2}  }
	{ gmem1 int 512 regular {axi_master 2}  }
	{ gmem2 int 64 regular {axi_master 2}  }
	{ kvsourcedramA_V int 32 regular {axi_slave 0}  }
	{ kvdestdramA_V int 32 regular {axi_slave 0}  }
	{ kvstatsA int 32 regular {axi_slave 0}  }
}
set C_modelArgMapList {[ 
	{ "Name" : "gmem0", "interface" : "axi_master", "bitwidth" : 512, "direction" : "READWRITE", "bitSlice":[{"low":0,"up":511,"cElement": [{"cName": "kvsourcedramA.V","cData": "uint512","bit_use": { "low": 0,"up": 511},"offset": { "type": "dynamic","port_name": "kvsourcedramA_V","bundle": "control"},"direction": "READWRITE","cArray": [{"low" : 0,"up" : 0,"step" : 1}]}]}]} , 
 	{ "Name" : "gmem1", "interface" : "axi_master", "bitwidth" : 512, "direction" : "READWRITE", "bitSlice":[{"low":0,"up":511,"cElement": [{"cName": "kvdestdramA.V","cData": "uint512","bit_use": { "low": 0,"up": 511},"offset": { "type": "dynamic","port_name": "kvdestdramA_V","bundle": "control"},"direction": "READWRITE","cArray": [{"low" : 0,"up" : 0,"step" : 1}]}]}]} , 
 	{ "Name" : "gmem2", "interface" : "axi_master", "bitwidth" : 64, "direction" : "READWRITE", "bitSlice":[{"low":0,"up":31,"cElement": [{"cName": "kvstatsA.key","cData": "unsigned int","bit_use": { "low": 0,"up": 31},"offset": { "type": "dynamic","port_name": "kvstatsA"},"direction": "READWRITE","cArray": [{"low" : 0,"up" : 0,"step" : 1}]}]},{"low":32,"up":63,"cElement": [{"cName": "kvstatsA.value","cData": "unsigned int","bit_use": { "low": 0,"up": 31},"offset": { "type": "dynamic","port_name": "kvstatsA"},"direction": "READWRITE","cArray": [{"low" : 0,"up" : 0,"step" : 1}]}]}]} , 
 	{ "Name" : "kvsourcedramA_V", "interface" : "axi_slave", "bundle":"control","type":"ap_none","bitwidth" : 32, "direction" : "READONLY", "offset" : {"in":16}, "offset_end" : {"in":23}} , 
 	{ "Name" : "kvdestdramA_V", "interface" : "axi_slave", "bundle":"control","type":"ap_none","bitwidth" : 32, "direction" : "READONLY", "offset" : {"in":24}, "offset_end" : {"in":31}} , 
 	{ "Name" : "kvstatsA", "interface" : "axi_slave", "bundle":"control","type":"ap_none","bitwidth" : 32, "direction" : "READONLY", "offset" : {"in":32}, "offset_end" : {"in":39}} ]}
# RTL Port declarations: 
set portNum 155
set portList { 
	{ ap_clk sc_in sc_logic 1 clock -1 } 
	{ ap_rst_n sc_in sc_logic 1 reset -1 active_low_sync } 
	{ m_axi_gmem0_AWVALID sc_out sc_logic 1 signal 0 } 
	{ m_axi_gmem0_AWREADY sc_in sc_logic 1 signal 0 } 
	{ m_axi_gmem0_AWADDR sc_out sc_lv 32 signal 0 } 
	{ m_axi_gmem0_AWID sc_out sc_lv 1 signal 0 } 
	{ m_axi_gmem0_AWLEN sc_out sc_lv 8 signal 0 } 
	{ m_axi_gmem0_AWSIZE sc_out sc_lv 3 signal 0 } 
	{ m_axi_gmem0_AWBURST sc_out sc_lv 2 signal 0 } 
	{ m_axi_gmem0_AWLOCK sc_out sc_lv 2 signal 0 } 
	{ m_axi_gmem0_AWCACHE sc_out sc_lv 4 signal 0 } 
	{ m_axi_gmem0_AWPROT sc_out sc_lv 3 signal 0 } 
	{ m_axi_gmem0_AWQOS sc_out sc_lv 4 signal 0 } 
	{ m_axi_gmem0_AWREGION sc_out sc_lv 4 signal 0 } 
	{ m_axi_gmem0_AWUSER sc_out sc_lv 1 signal 0 } 
	{ m_axi_gmem0_WVALID sc_out sc_logic 1 signal 0 } 
	{ m_axi_gmem0_WREADY sc_in sc_logic 1 signal 0 } 
	{ m_axi_gmem0_WDATA sc_out sc_lv 512 signal 0 } 
	{ m_axi_gmem0_WSTRB sc_out sc_lv 64 signal 0 } 
	{ m_axi_gmem0_WLAST sc_out sc_logic 1 signal 0 } 
	{ m_axi_gmem0_WID sc_out sc_lv 1 signal 0 } 
	{ m_axi_gmem0_WUSER sc_out sc_lv 1 signal 0 } 
	{ m_axi_gmem0_ARVALID sc_out sc_logic 1 signal 0 } 
	{ m_axi_gmem0_ARREADY sc_in sc_logic 1 signal 0 } 
	{ m_axi_gmem0_ARADDR sc_out sc_lv 32 signal 0 } 
	{ m_axi_gmem0_ARID sc_out sc_lv 1 signal 0 } 
	{ m_axi_gmem0_ARLEN sc_out sc_lv 8 signal 0 } 
	{ m_axi_gmem0_ARSIZE sc_out sc_lv 3 signal 0 } 
	{ m_axi_gmem0_ARBURST sc_out sc_lv 2 signal 0 } 
	{ m_axi_gmem0_ARLOCK sc_out sc_lv 2 signal 0 } 
	{ m_axi_gmem0_ARCACHE sc_out sc_lv 4 signal 0 } 
	{ m_axi_gmem0_ARPROT sc_out sc_lv 3 signal 0 } 
	{ m_axi_gmem0_ARQOS sc_out sc_lv 4 signal 0 } 
	{ m_axi_gmem0_ARREGION sc_out sc_lv 4 signal 0 } 
	{ m_axi_gmem0_ARUSER sc_out sc_lv 1 signal 0 } 
	{ m_axi_gmem0_RVALID sc_in sc_logic 1 signal 0 } 
	{ m_axi_gmem0_RREADY sc_out sc_logic 1 signal 0 } 
	{ m_axi_gmem0_RDATA sc_in sc_lv 512 signal 0 } 
	{ m_axi_gmem0_RLAST sc_in sc_logic 1 signal 0 } 
	{ m_axi_gmem0_RID sc_in sc_lv 1 signal 0 } 
	{ m_axi_gmem0_RUSER sc_in sc_lv 1 signal 0 } 
	{ m_axi_gmem0_RRESP sc_in sc_lv 2 signal 0 } 
	{ m_axi_gmem0_BVALID sc_in sc_logic 1 signal 0 } 
	{ m_axi_gmem0_BREADY sc_out sc_logic 1 signal 0 } 
	{ m_axi_gmem0_BRESP sc_in sc_lv 2 signal 0 } 
	{ m_axi_gmem0_BID sc_in sc_lv 1 signal 0 } 
	{ m_axi_gmem0_BUSER sc_in sc_lv 1 signal 0 } 
	{ m_axi_gmem1_AWVALID sc_out sc_logic 1 signal 1 } 
	{ m_axi_gmem1_AWREADY sc_in sc_logic 1 signal 1 } 
	{ m_axi_gmem1_AWADDR sc_out sc_lv 32 signal 1 } 
	{ m_axi_gmem1_AWID sc_out sc_lv 1 signal 1 } 
	{ m_axi_gmem1_AWLEN sc_out sc_lv 8 signal 1 } 
	{ m_axi_gmem1_AWSIZE sc_out sc_lv 3 signal 1 } 
	{ m_axi_gmem1_AWBURST sc_out sc_lv 2 signal 1 } 
	{ m_axi_gmem1_AWLOCK sc_out sc_lv 2 signal 1 } 
	{ m_axi_gmem1_AWCACHE sc_out sc_lv 4 signal 1 } 
	{ m_axi_gmem1_AWPROT sc_out sc_lv 3 signal 1 } 
	{ m_axi_gmem1_AWQOS sc_out sc_lv 4 signal 1 } 
	{ m_axi_gmem1_AWREGION sc_out sc_lv 4 signal 1 } 
	{ m_axi_gmem1_AWUSER sc_out sc_lv 1 signal 1 } 
	{ m_axi_gmem1_WVALID sc_out sc_logic 1 signal 1 } 
	{ m_axi_gmem1_WREADY sc_in sc_logic 1 signal 1 } 
	{ m_axi_gmem1_WDATA sc_out sc_lv 512 signal 1 } 
	{ m_axi_gmem1_WSTRB sc_out sc_lv 64 signal 1 } 
	{ m_axi_gmem1_WLAST sc_out sc_logic 1 signal 1 } 
	{ m_axi_gmem1_WID sc_out sc_lv 1 signal 1 } 
	{ m_axi_gmem1_WUSER sc_out sc_lv 1 signal 1 } 
	{ m_axi_gmem1_ARVALID sc_out sc_logic 1 signal 1 } 
	{ m_axi_gmem1_ARREADY sc_in sc_logic 1 signal 1 } 
	{ m_axi_gmem1_ARADDR sc_out sc_lv 32 signal 1 } 
	{ m_axi_gmem1_ARID sc_out sc_lv 1 signal 1 } 
	{ m_axi_gmem1_ARLEN sc_out sc_lv 8 signal 1 } 
	{ m_axi_gmem1_ARSIZE sc_out sc_lv 3 signal 1 } 
	{ m_axi_gmem1_ARBURST sc_out sc_lv 2 signal 1 } 
	{ m_axi_gmem1_ARLOCK sc_out sc_lv 2 signal 1 } 
	{ m_axi_gmem1_ARCACHE sc_out sc_lv 4 signal 1 } 
	{ m_axi_gmem1_ARPROT sc_out sc_lv 3 signal 1 } 
	{ m_axi_gmem1_ARQOS sc_out sc_lv 4 signal 1 } 
	{ m_axi_gmem1_ARREGION sc_out sc_lv 4 signal 1 } 
	{ m_axi_gmem1_ARUSER sc_out sc_lv 1 signal 1 } 
	{ m_axi_gmem1_RVALID sc_in sc_logic 1 signal 1 } 
	{ m_axi_gmem1_RREADY sc_out sc_logic 1 signal 1 } 
	{ m_axi_gmem1_RDATA sc_in sc_lv 512 signal 1 } 
	{ m_axi_gmem1_RLAST sc_in sc_logic 1 signal 1 } 
	{ m_axi_gmem1_RID sc_in sc_lv 1 signal 1 } 
	{ m_axi_gmem1_RUSER sc_in sc_lv 1 signal 1 } 
	{ m_axi_gmem1_RRESP sc_in sc_lv 2 signal 1 } 
	{ m_axi_gmem1_BVALID sc_in sc_logic 1 signal 1 } 
	{ m_axi_gmem1_BREADY sc_out sc_logic 1 signal 1 } 
	{ m_axi_gmem1_BRESP sc_in sc_lv 2 signal 1 } 
	{ m_axi_gmem1_BID sc_in sc_lv 1 signal 1 } 
	{ m_axi_gmem1_BUSER sc_in sc_lv 1 signal 1 } 
	{ m_axi_gmem2_AWVALID sc_out sc_logic 1 signal 2 } 
	{ m_axi_gmem2_AWREADY sc_in sc_logic 1 signal 2 } 
	{ m_axi_gmem2_AWADDR sc_out sc_lv 32 signal 2 } 
	{ m_axi_gmem2_AWID sc_out sc_lv 1 signal 2 } 
	{ m_axi_gmem2_AWLEN sc_out sc_lv 8 signal 2 } 
	{ m_axi_gmem2_AWSIZE sc_out sc_lv 3 signal 2 } 
	{ m_axi_gmem2_AWBURST sc_out sc_lv 2 signal 2 } 
	{ m_axi_gmem2_AWLOCK sc_out sc_lv 2 signal 2 } 
	{ m_axi_gmem2_AWCACHE sc_out sc_lv 4 signal 2 } 
	{ m_axi_gmem2_AWPROT sc_out sc_lv 3 signal 2 } 
	{ m_axi_gmem2_AWQOS sc_out sc_lv 4 signal 2 } 
	{ m_axi_gmem2_AWREGION sc_out sc_lv 4 signal 2 } 
	{ m_axi_gmem2_AWUSER sc_out sc_lv 1 signal 2 } 
	{ m_axi_gmem2_WVALID sc_out sc_logic 1 signal 2 } 
	{ m_axi_gmem2_WREADY sc_in sc_logic 1 signal 2 } 
	{ m_axi_gmem2_WDATA sc_out sc_lv 64 signal 2 } 
	{ m_axi_gmem2_WSTRB sc_out sc_lv 8 signal 2 } 
	{ m_axi_gmem2_WLAST sc_out sc_logic 1 signal 2 } 
	{ m_axi_gmem2_WID sc_out sc_lv 1 signal 2 } 
	{ m_axi_gmem2_WUSER sc_out sc_lv 1 signal 2 } 
	{ m_axi_gmem2_ARVALID sc_out sc_logic 1 signal 2 } 
	{ m_axi_gmem2_ARREADY sc_in sc_logic 1 signal 2 } 
	{ m_axi_gmem2_ARADDR sc_out sc_lv 32 signal 2 } 
	{ m_axi_gmem2_ARID sc_out sc_lv 1 signal 2 } 
	{ m_axi_gmem2_ARLEN sc_out sc_lv 8 signal 2 } 
	{ m_axi_gmem2_ARSIZE sc_out sc_lv 3 signal 2 } 
	{ m_axi_gmem2_ARBURST sc_out sc_lv 2 signal 2 } 
	{ m_axi_gmem2_ARLOCK sc_out sc_lv 2 signal 2 } 
	{ m_axi_gmem2_ARCACHE sc_out sc_lv 4 signal 2 } 
	{ m_axi_gmem2_ARPROT sc_out sc_lv 3 signal 2 } 
	{ m_axi_gmem2_ARQOS sc_out sc_lv 4 signal 2 } 
	{ m_axi_gmem2_ARREGION sc_out sc_lv 4 signal 2 } 
	{ m_axi_gmem2_ARUSER sc_out sc_lv 1 signal 2 } 
	{ m_axi_gmem2_RVALID sc_in sc_logic 1 signal 2 } 
	{ m_axi_gmem2_RREADY sc_out sc_logic 1 signal 2 } 
	{ m_axi_gmem2_RDATA sc_in sc_lv 64 signal 2 } 
	{ m_axi_gmem2_RLAST sc_in sc_logic 1 signal 2 } 
	{ m_axi_gmem2_RID sc_in sc_lv 1 signal 2 } 
	{ m_axi_gmem2_RUSER sc_in sc_lv 1 signal 2 } 
	{ m_axi_gmem2_RRESP sc_in sc_lv 2 signal 2 } 
	{ m_axi_gmem2_BVALID sc_in sc_logic 1 signal 2 } 
	{ m_axi_gmem2_BREADY sc_out sc_logic 1 signal 2 } 
	{ m_axi_gmem2_BRESP sc_in sc_lv 2 signal 2 } 
	{ m_axi_gmem2_BID sc_in sc_lv 1 signal 2 } 
	{ m_axi_gmem2_BUSER sc_in sc_lv 1 signal 2 } 
	{ s_axi_control_AWVALID sc_in sc_logic 1 signal -1 } 
	{ s_axi_control_AWREADY sc_out sc_logic 1 signal -1 } 
	{ s_axi_control_AWADDR sc_in sc_lv 6 signal -1 } 
	{ s_axi_control_WVALID sc_in sc_logic 1 signal -1 } 
	{ s_axi_control_WREADY sc_out sc_logic 1 signal -1 } 
	{ s_axi_control_WDATA sc_in sc_lv 32 signal -1 } 
	{ s_axi_control_WSTRB sc_in sc_lv 4 signal -1 } 
	{ s_axi_control_ARVALID sc_in sc_logic 1 signal -1 } 
	{ s_axi_control_ARREADY sc_out sc_logic 1 signal -1 } 
	{ s_axi_control_ARADDR sc_in sc_lv 6 signal -1 } 
	{ s_axi_control_RVALID sc_out sc_logic 1 signal -1 } 
	{ s_axi_control_RREADY sc_in sc_logic 1 signal -1 } 
	{ s_axi_control_RDATA sc_out sc_lv 32 signal -1 } 
	{ s_axi_control_RRESP sc_out sc_lv 2 signal -1 } 
	{ s_axi_control_BVALID sc_out sc_logic 1 signal -1 } 
	{ s_axi_control_BREADY sc_in sc_logic 1 signal -1 } 
	{ s_axi_control_BRESP sc_out sc_lv 2 signal -1 } 
	{ interrupt sc_out sc_logic 1 signal -1 } 
}
set NewPortList {[ 
	{ "name": "s_axi_control_AWADDR", "direction": "in", "datatype": "sc_lv", "bitwidth":6, "type": "signal", "bundle":{"name": "control", "role": "AWADDR" },"address":[{"name":"topkernel","role":"start","value":"0","valid_bit":"0"},{"name":"topkernel","role":"continue","value":"0","valid_bit":"4"},{"name":"topkernel","role":"auto_start","value":"0","valid_bit":"7"},{"name":"kvsourcedramA_V","role":"data","value":"16"},{"name":"kvdestdramA_V","role":"data","value":"24"},{"name":"kvstatsA","role":"data","value":"32"}] },
	{ "name": "s_axi_control_AWVALID", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "control", "role": "AWVALID" } },
	{ "name": "s_axi_control_AWREADY", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "control", "role": "AWREADY" } },
	{ "name": "s_axi_control_WVALID", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "control", "role": "WVALID" } },
	{ "name": "s_axi_control_WREADY", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "control", "role": "WREADY" } },
	{ "name": "s_axi_control_WDATA", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "control", "role": "WDATA" } },
	{ "name": "s_axi_control_WSTRB", "direction": "in", "datatype": "sc_lv", "bitwidth":4, "type": "signal", "bundle":{"name": "control", "role": "WSTRB" } },
	{ "name": "s_axi_control_ARADDR", "direction": "in", "datatype": "sc_lv", "bitwidth":6, "type": "signal", "bundle":{"name": "control", "role": "ARADDR" },"address":[{"name":"topkernel","role":"start","value":"0","valid_bit":"0"},{"name":"topkernel","role":"done","value":"0","valid_bit":"1"},{"name":"topkernel","role":"idle","value":"0","valid_bit":"2"},{"name":"topkernel","role":"ready","value":"0","valid_bit":"3"},{"name":"topkernel","role":"auto_start","value":"0","valid_bit":"7"}] },
	{ "name": "s_axi_control_ARVALID", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "control", "role": "ARVALID" } },
	{ "name": "s_axi_control_ARREADY", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "control", "role": "ARREADY" } },
	{ "name": "s_axi_control_RVALID", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "control", "role": "RVALID" } },
	{ "name": "s_axi_control_RREADY", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "control", "role": "RREADY" } },
	{ "name": "s_axi_control_RDATA", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "control", "role": "RDATA" } },
	{ "name": "s_axi_control_RRESP", "direction": "out", "datatype": "sc_lv", "bitwidth":2, "type": "signal", "bundle":{"name": "control", "role": "RRESP" } },
	{ "name": "s_axi_control_BVALID", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "control", "role": "BVALID" } },
	{ "name": "s_axi_control_BREADY", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "control", "role": "BREADY" } },
	{ "name": "s_axi_control_BRESP", "direction": "out", "datatype": "sc_lv", "bitwidth":2, "type": "signal", "bundle":{"name": "control", "role": "BRESP" } },
	{ "name": "interrupt", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "control", "role": "interrupt" } }, 
 	{ "name": "ap_clk", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "clock", "bundle":{"name": "ap_clk", "role": "default" }} , 
 	{ "name": "ap_rst_n", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "reset", "bundle":{"name": "ap_rst_n", "role": "default" }} , 
 	{ "name": "m_axi_gmem0_AWVALID", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "gmem0", "role": "AWVALID" }} , 
 	{ "name": "m_axi_gmem0_AWREADY", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "gmem0", "role": "AWREADY" }} , 
 	{ "name": "m_axi_gmem0_AWADDR", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "gmem0", "role": "AWADDR" }} , 
 	{ "name": "m_axi_gmem0_AWID", "direction": "out", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "gmem0", "role": "AWID" }} , 
 	{ "name": "m_axi_gmem0_AWLEN", "direction": "out", "datatype": "sc_lv", "bitwidth":8, "type": "signal", "bundle":{"name": "gmem0", "role": "AWLEN" }} , 
 	{ "name": "m_axi_gmem0_AWSIZE", "direction": "out", "datatype": "sc_lv", "bitwidth":3, "type": "signal", "bundle":{"name": "gmem0", "role": "AWSIZE" }} , 
 	{ "name": "m_axi_gmem0_AWBURST", "direction": "out", "datatype": "sc_lv", "bitwidth":2, "type": "signal", "bundle":{"name": "gmem0", "role": "AWBURST" }} , 
 	{ "name": "m_axi_gmem0_AWLOCK", "direction": "out", "datatype": "sc_lv", "bitwidth":2, "type": "signal", "bundle":{"name": "gmem0", "role": "AWLOCK" }} , 
 	{ "name": "m_axi_gmem0_AWCACHE", "direction": "out", "datatype": "sc_lv", "bitwidth":4, "type": "signal", "bundle":{"name": "gmem0", "role": "AWCACHE" }} , 
 	{ "name": "m_axi_gmem0_AWPROT", "direction": "out", "datatype": "sc_lv", "bitwidth":3, "type": "signal", "bundle":{"name": "gmem0", "role": "AWPROT" }} , 
 	{ "name": "m_axi_gmem0_AWQOS", "direction": "out", "datatype": "sc_lv", "bitwidth":4, "type": "signal", "bundle":{"name": "gmem0", "role": "AWQOS" }} , 
 	{ "name": "m_axi_gmem0_AWREGION", "direction": "out", "datatype": "sc_lv", "bitwidth":4, "type": "signal", "bundle":{"name": "gmem0", "role": "AWREGION" }} , 
 	{ "name": "m_axi_gmem0_AWUSER", "direction": "out", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "gmem0", "role": "AWUSER" }} , 
 	{ "name": "m_axi_gmem0_WVALID", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "gmem0", "role": "WVALID" }} , 
 	{ "name": "m_axi_gmem0_WREADY", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "gmem0", "role": "WREADY" }} , 
 	{ "name": "m_axi_gmem0_WDATA", "direction": "out", "datatype": "sc_lv", "bitwidth":512, "type": "signal", "bundle":{"name": "gmem0", "role": "WDATA" }} , 
 	{ "name": "m_axi_gmem0_WSTRB", "direction": "out", "datatype": "sc_lv", "bitwidth":64, "type": "signal", "bundle":{"name": "gmem0", "role": "WSTRB" }} , 
 	{ "name": "m_axi_gmem0_WLAST", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "gmem0", "role": "WLAST" }} , 
 	{ "name": "m_axi_gmem0_WID", "direction": "out", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "gmem0", "role": "WID" }} , 
 	{ "name": "m_axi_gmem0_WUSER", "direction": "out", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "gmem0", "role": "WUSER" }} , 
 	{ "name": "m_axi_gmem0_ARVALID", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "gmem0", "role": "ARVALID" }} , 
 	{ "name": "m_axi_gmem0_ARREADY", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "gmem0", "role": "ARREADY" }} , 
 	{ "name": "m_axi_gmem0_ARADDR", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "gmem0", "role": "ARADDR" }} , 
 	{ "name": "m_axi_gmem0_ARID", "direction": "out", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "gmem0", "role": "ARID" }} , 
 	{ "name": "m_axi_gmem0_ARLEN", "direction": "out", "datatype": "sc_lv", "bitwidth":8, "type": "signal", "bundle":{"name": "gmem0", "role": "ARLEN" }} , 
 	{ "name": "m_axi_gmem0_ARSIZE", "direction": "out", "datatype": "sc_lv", "bitwidth":3, "type": "signal", "bundle":{"name": "gmem0", "role": "ARSIZE" }} , 
 	{ "name": "m_axi_gmem0_ARBURST", "direction": "out", "datatype": "sc_lv", "bitwidth":2, "type": "signal", "bundle":{"name": "gmem0", "role": "ARBURST" }} , 
 	{ "name": "m_axi_gmem0_ARLOCK", "direction": "out", "datatype": "sc_lv", "bitwidth":2, "type": "signal", "bundle":{"name": "gmem0", "role": "ARLOCK" }} , 
 	{ "name": "m_axi_gmem0_ARCACHE", "direction": "out", "datatype": "sc_lv", "bitwidth":4, "type": "signal", "bundle":{"name": "gmem0", "role": "ARCACHE" }} , 
 	{ "name": "m_axi_gmem0_ARPROT", "direction": "out", "datatype": "sc_lv", "bitwidth":3, "type": "signal", "bundle":{"name": "gmem0", "role": "ARPROT" }} , 
 	{ "name": "m_axi_gmem0_ARQOS", "direction": "out", "datatype": "sc_lv", "bitwidth":4, "type": "signal", "bundle":{"name": "gmem0", "role": "ARQOS" }} , 
 	{ "name": "m_axi_gmem0_ARREGION", "direction": "out", "datatype": "sc_lv", "bitwidth":4, "type": "signal", "bundle":{"name": "gmem0", "role": "ARREGION" }} , 
 	{ "name": "m_axi_gmem0_ARUSER", "direction": "out", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "gmem0", "role": "ARUSER" }} , 
 	{ "name": "m_axi_gmem0_RVALID", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "gmem0", "role": "RVALID" }} , 
 	{ "name": "m_axi_gmem0_RREADY", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "gmem0", "role": "RREADY" }} , 
 	{ "name": "m_axi_gmem0_RDATA", "direction": "in", "datatype": "sc_lv", "bitwidth":512, "type": "signal", "bundle":{"name": "gmem0", "role": "RDATA" }} , 
 	{ "name": "m_axi_gmem0_RLAST", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "gmem0", "role": "RLAST" }} , 
 	{ "name": "m_axi_gmem0_RID", "direction": "in", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "gmem0", "role": "RID" }} , 
 	{ "name": "m_axi_gmem0_RUSER", "direction": "in", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "gmem0", "role": "RUSER" }} , 
 	{ "name": "m_axi_gmem0_RRESP", "direction": "in", "datatype": "sc_lv", "bitwidth":2, "type": "signal", "bundle":{"name": "gmem0", "role": "RRESP" }} , 
 	{ "name": "m_axi_gmem0_BVALID", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "gmem0", "role": "BVALID" }} , 
 	{ "name": "m_axi_gmem0_BREADY", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "gmem0", "role": "BREADY" }} , 
 	{ "name": "m_axi_gmem0_BRESP", "direction": "in", "datatype": "sc_lv", "bitwidth":2, "type": "signal", "bundle":{"name": "gmem0", "role": "BRESP" }} , 
 	{ "name": "m_axi_gmem0_BID", "direction": "in", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "gmem0", "role": "BID" }} , 
 	{ "name": "m_axi_gmem0_BUSER", "direction": "in", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "gmem0", "role": "BUSER" }} , 
 	{ "name": "m_axi_gmem1_AWVALID", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "gmem1", "role": "AWVALID" }} , 
 	{ "name": "m_axi_gmem1_AWREADY", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "gmem1", "role": "AWREADY" }} , 
 	{ "name": "m_axi_gmem1_AWADDR", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "gmem1", "role": "AWADDR" }} , 
 	{ "name": "m_axi_gmem1_AWID", "direction": "out", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "gmem1", "role": "AWID" }} , 
 	{ "name": "m_axi_gmem1_AWLEN", "direction": "out", "datatype": "sc_lv", "bitwidth":8, "type": "signal", "bundle":{"name": "gmem1", "role": "AWLEN" }} , 
 	{ "name": "m_axi_gmem1_AWSIZE", "direction": "out", "datatype": "sc_lv", "bitwidth":3, "type": "signal", "bundle":{"name": "gmem1", "role": "AWSIZE" }} , 
 	{ "name": "m_axi_gmem1_AWBURST", "direction": "out", "datatype": "sc_lv", "bitwidth":2, "type": "signal", "bundle":{"name": "gmem1", "role": "AWBURST" }} , 
 	{ "name": "m_axi_gmem1_AWLOCK", "direction": "out", "datatype": "sc_lv", "bitwidth":2, "type": "signal", "bundle":{"name": "gmem1", "role": "AWLOCK" }} , 
 	{ "name": "m_axi_gmem1_AWCACHE", "direction": "out", "datatype": "sc_lv", "bitwidth":4, "type": "signal", "bundle":{"name": "gmem1", "role": "AWCACHE" }} , 
 	{ "name": "m_axi_gmem1_AWPROT", "direction": "out", "datatype": "sc_lv", "bitwidth":3, "type": "signal", "bundle":{"name": "gmem1", "role": "AWPROT" }} , 
 	{ "name": "m_axi_gmem1_AWQOS", "direction": "out", "datatype": "sc_lv", "bitwidth":4, "type": "signal", "bundle":{"name": "gmem1", "role": "AWQOS" }} , 
 	{ "name": "m_axi_gmem1_AWREGION", "direction": "out", "datatype": "sc_lv", "bitwidth":4, "type": "signal", "bundle":{"name": "gmem1", "role": "AWREGION" }} , 
 	{ "name": "m_axi_gmem1_AWUSER", "direction": "out", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "gmem1", "role": "AWUSER" }} , 
 	{ "name": "m_axi_gmem1_WVALID", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "gmem1", "role": "WVALID" }} , 
 	{ "name": "m_axi_gmem1_WREADY", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "gmem1", "role": "WREADY" }} , 
 	{ "name": "m_axi_gmem1_WDATA", "direction": "out", "datatype": "sc_lv", "bitwidth":512, "type": "signal", "bundle":{"name": "gmem1", "role": "WDATA" }} , 
 	{ "name": "m_axi_gmem1_WSTRB", "direction": "out", "datatype": "sc_lv", "bitwidth":64, "type": "signal", "bundle":{"name": "gmem1", "role": "WSTRB" }} , 
 	{ "name": "m_axi_gmem1_WLAST", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "gmem1", "role": "WLAST" }} , 
 	{ "name": "m_axi_gmem1_WID", "direction": "out", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "gmem1", "role": "WID" }} , 
 	{ "name": "m_axi_gmem1_WUSER", "direction": "out", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "gmem1", "role": "WUSER" }} , 
 	{ "name": "m_axi_gmem1_ARVALID", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "gmem1", "role": "ARVALID" }} , 
 	{ "name": "m_axi_gmem1_ARREADY", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "gmem1", "role": "ARREADY" }} , 
 	{ "name": "m_axi_gmem1_ARADDR", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "gmem1", "role": "ARADDR" }} , 
 	{ "name": "m_axi_gmem1_ARID", "direction": "out", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "gmem1", "role": "ARID" }} , 
 	{ "name": "m_axi_gmem1_ARLEN", "direction": "out", "datatype": "sc_lv", "bitwidth":8, "type": "signal", "bundle":{"name": "gmem1", "role": "ARLEN" }} , 
 	{ "name": "m_axi_gmem1_ARSIZE", "direction": "out", "datatype": "sc_lv", "bitwidth":3, "type": "signal", "bundle":{"name": "gmem1", "role": "ARSIZE" }} , 
 	{ "name": "m_axi_gmem1_ARBURST", "direction": "out", "datatype": "sc_lv", "bitwidth":2, "type": "signal", "bundle":{"name": "gmem1", "role": "ARBURST" }} , 
 	{ "name": "m_axi_gmem1_ARLOCK", "direction": "out", "datatype": "sc_lv", "bitwidth":2, "type": "signal", "bundle":{"name": "gmem1", "role": "ARLOCK" }} , 
 	{ "name": "m_axi_gmem1_ARCACHE", "direction": "out", "datatype": "sc_lv", "bitwidth":4, "type": "signal", "bundle":{"name": "gmem1", "role": "ARCACHE" }} , 
 	{ "name": "m_axi_gmem1_ARPROT", "direction": "out", "datatype": "sc_lv", "bitwidth":3, "type": "signal", "bundle":{"name": "gmem1", "role": "ARPROT" }} , 
 	{ "name": "m_axi_gmem1_ARQOS", "direction": "out", "datatype": "sc_lv", "bitwidth":4, "type": "signal", "bundle":{"name": "gmem1", "role": "ARQOS" }} , 
 	{ "name": "m_axi_gmem1_ARREGION", "direction": "out", "datatype": "sc_lv", "bitwidth":4, "type": "signal", "bundle":{"name": "gmem1", "role": "ARREGION" }} , 
 	{ "name": "m_axi_gmem1_ARUSER", "direction": "out", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "gmem1", "role": "ARUSER" }} , 
 	{ "name": "m_axi_gmem1_RVALID", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "gmem1", "role": "RVALID" }} , 
 	{ "name": "m_axi_gmem1_RREADY", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "gmem1", "role": "RREADY" }} , 
 	{ "name": "m_axi_gmem1_RDATA", "direction": "in", "datatype": "sc_lv", "bitwidth":512, "type": "signal", "bundle":{"name": "gmem1", "role": "RDATA" }} , 
 	{ "name": "m_axi_gmem1_RLAST", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "gmem1", "role": "RLAST" }} , 
 	{ "name": "m_axi_gmem1_RID", "direction": "in", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "gmem1", "role": "RID" }} , 
 	{ "name": "m_axi_gmem1_RUSER", "direction": "in", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "gmem1", "role": "RUSER" }} , 
 	{ "name": "m_axi_gmem1_RRESP", "direction": "in", "datatype": "sc_lv", "bitwidth":2, "type": "signal", "bundle":{"name": "gmem1", "role": "RRESP" }} , 
 	{ "name": "m_axi_gmem1_BVALID", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "gmem1", "role": "BVALID" }} , 
 	{ "name": "m_axi_gmem1_BREADY", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "gmem1", "role": "BREADY" }} , 
 	{ "name": "m_axi_gmem1_BRESP", "direction": "in", "datatype": "sc_lv", "bitwidth":2, "type": "signal", "bundle":{"name": "gmem1", "role": "BRESP" }} , 
 	{ "name": "m_axi_gmem1_BID", "direction": "in", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "gmem1", "role": "BID" }} , 
 	{ "name": "m_axi_gmem1_BUSER", "direction": "in", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "gmem1", "role": "BUSER" }} , 
 	{ "name": "m_axi_gmem2_AWVALID", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "gmem2", "role": "AWVALID" }} , 
 	{ "name": "m_axi_gmem2_AWREADY", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "gmem2", "role": "AWREADY" }} , 
 	{ "name": "m_axi_gmem2_AWADDR", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "gmem2", "role": "AWADDR" }} , 
 	{ "name": "m_axi_gmem2_AWID", "direction": "out", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "gmem2", "role": "AWID" }} , 
 	{ "name": "m_axi_gmem2_AWLEN", "direction": "out", "datatype": "sc_lv", "bitwidth":8, "type": "signal", "bundle":{"name": "gmem2", "role": "AWLEN" }} , 
 	{ "name": "m_axi_gmem2_AWSIZE", "direction": "out", "datatype": "sc_lv", "bitwidth":3, "type": "signal", "bundle":{"name": "gmem2", "role": "AWSIZE" }} , 
 	{ "name": "m_axi_gmem2_AWBURST", "direction": "out", "datatype": "sc_lv", "bitwidth":2, "type": "signal", "bundle":{"name": "gmem2", "role": "AWBURST" }} , 
 	{ "name": "m_axi_gmem2_AWLOCK", "direction": "out", "datatype": "sc_lv", "bitwidth":2, "type": "signal", "bundle":{"name": "gmem2", "role": "AWLOCK" }} , 
 	{ "name": "m_axi_gmem2_AWCACHE", "direction": "out", "datatype": "sc_lv", "bitwidth":4, "type": "signal", "bundle":{"name": "gmem2", "role": "AWCACHE" }} , 
 	{ "name": "m_axi_gmem2_AWPROT", "direction": "out", "datatype": "sc_lv", "bitwidth":3, "type": "signal", "bundle":{"name": "gmem2", "role": "AWPROT" }} , 
 	{ "name": "m_axi_gmem2_AWQOS", "direction": "out", "datatype": "sc_lv", "bitwidth":4, "type": "signal", "bundle":{"name": "gmem2", "role": "AWQOS" }} , 
 	{ "name": "m_axi_gmem2_AWREGION", "direction": "out", "datatype": "sc_lv", "bitwidth":4, "type": "signal", "bundle":{"name": "gmem2", "role": "AWREGION" }} , 
 	{ "name": "m_axi_gmem2_AWUSER", "direction": "out", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "gmem2", "role": "AWUSER" }} , 
 	{ "name": "m_axi_gmem2_WVALID", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "gmem2", "role": "WVALID" }} , 
 	{ "name": "m_axi_gmem2_WREADY", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "gmem2", "role": "WREADY" }} , 
 	{ "name": "m_axi_gmem2_WDATA", "direction": "out", "datatype": "sc_lv", "bitwidth":64, "type": "signal", "bundle":{"name": "gmem2", "role": "WDATA" }} , 
 	{ "name": "m_axi_gmem2_WSTRB", "direction": "out", "datatype": "sc_lv", "bitwidth":8, "type": "signal", "bundle":{"name": "gmem2", "role": "WSTRB" }} , 
 	{ "name": "m_axi_gmem2_WLAST", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "gmem2", "role": "WLAST" }} , 
 	{ "name": "m_axi_gmem2_WID", "direction": "out", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "gmem2", "role": "WID" }} , 
 	{ "name": "m_axi_gmem2_WUSER", "direction": "out", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "gmem2", "role": "WUSER" }} , 
 	{ "name": "m_axi_gmem2_ARVALID", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "gmem2", "role": "ARVALID" }} , 
 	{ "name": "m_axi_gmem2_ARREADY", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "gmem2", "role": "ARREADY" }} , 
 	{ "name": "m_axi_gmem2_ARADDR", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "gmem2", "role": "ARADDR" }} , 
 	{ "name": "m_axi_gmem2_ARID", "direction": "out", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "gmem2", "role": "ARID" }} , 
 	{ "name": "m_axi_gmem2_ARLEN", "direction": "out", "datatype": "sc_lv", "bitwidth":8, "type": "signal", "bundle":{"name": "gmem2", "role": "ARLEN" }} , 
 	{ "name": "m_axi_gmem2_ARSIZE", "direction": "out", "datatype": "sc_lv", "bitwidth":3, "type": "signal", "bundle":{"name": "gmem2", "role": "ARSIZE" }} , 
 	{ "name": "m_axi_gmem2_ARBURST", "direction": "out", "datatype": "sc_lv", "bitwidth":2, "type": "signal", "bundle":{"name": "gmem2", "role": "ARBURST" }} , 
 	{ "name": "m_axi_gmem2_ARLOCK", "direction": "out", "datatype": "sc_lv", "bitwidth":2, "type": "signal", "bundle":{"name": "gmem2", "role": "ARLOCK" }} , 
 	{ "name": "m_axi_gmem2_ARCACHE", "direction": "out", "datatype": "sc_lv", "bitwidth":4, "type": "signal", "bundle":{"name": "gmem2", "role": "ARCACHE" }} , 
 	{ "name": "m_axi_gmem2_ARPROT", "direction": "out", "datatype": "sc_lv", "bitwidth":3, "type": "signal", "bundle":{"name": "gmem2", "role": "ARPROT" }} , 
 	{ "name": "m_axi_gmem2_ARQOS", "direction": "out", "datatype": "sc_lv", "bitwidth":4, "type": "signal", "bundle":{"name": "gmem2", "role": "ARQOS" }} , 
 	{ "name": "m_axi_gmem2_ARREGION", "direction": "out", "datatype": "sc_lv", "bitwidth":4, "type": "signal", "bundle":{"name": "gmem2", "role": "ARREGION" }} , 
 	{ "name": "m_axi_gmem2_ARUSER", "direction": "out", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "gmem2", "role": "ARUSER" }} , 
 	{ "name": "m_axi_gmem2_RVALID", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "gmem2", "role": "RVALID" }} , 
 	{ "name": "m_axi_gmem2_RREADY", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "gmem2", "role": "RREADY" }} , 
 	{ "name": "m_axi_gmem2_RDATA", "direction": "in", "datatype": "sc_lv", "bitwidth":64, "type": "signal", "bundle":{"name": "gmem2", "role": "RDATA" }} , 
 	{ "name": "m_axi_gmem2_RLAST", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "gmem2", "role": "RLAST" }} , 
 	{ "name": "m_axi_gmem2_RID", "direction": "in", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "gmem2", "role": "RID" }} , 
 	{ "name": "m_axi_gmem2_RUSER", "direction": "in", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "gmem2", "role": "RUSER" }} , 
 	{ "name": "m_axi_gmem2_RRESP", "direction": "in", "datatype": "sc_lv", "bitwidth":2, "type": "signal", "bundle":{"name": "gmem2", "role": "RRESP" }} , 
 	{ "name": "m_axi_gmem2_BVALID", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "gmem2", "role": "BVALID" }} , 
 	{ "name": "m_axi_gmem2_BREADY", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "gmem2", "role": "BREADY" }} , 
 	{ "name": "m_axi_gmem2_BRESP", "direction": "in", "datatype": "sc_lv", "bitwidth":2, "type": "signal", "bundle":{"name": "gmem2", "role": "BRESP" }} , 
 	{ "name": "m_axi_gmem2_BID", "direction": "in", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "gmem2", "role": "BID" }} , 
 	{ "name": "m_axi_gmem2_BUSER", "direction": "in", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "gmem2", "role": "BUSER" }}  ]}

set RtlHierarchyInfo {[
	{"ID" : "0", "Level" : "0", "Path" : "`AUTOTB_DUT_INST", "Parent" : "", "Child" : ["1", "2", "3", "4", "5"],
		"CDFG" : "topkernel",
		"Protocol" : "ap_ctrl_hs",
		"ControlExist" : "1", "ap_start" : "1", "ap_ready" : "1", "ap_done" : "1", "ap_continue" : "0", "ap_idle" : "1",
		"Pipeline" : "None", "UnalignedPipeline" : "0", "RewindPipeline" : "0", "ProcessNetwork" : "0",
		"II" : "0",
		"VariableLatency" : "1", "ExactLatency" : "-1", "EstimateLatencyMin" : "57", "EstimateLatencyMax" : "197844234",
		"Combinational" : "0",
		"Datapath" : "0",
		"ClockEnable" : "0",
		"HasSubDataflow" : "0",
		"InDataflowNetwork" : "0",
		"HasNonBlockingOperation" : "0",
		"WaitState" : [
			{"State" : "ap_ST_fsm_state17", "FSM" : "ap_CS_fsm", "SubInstance" : "grp_partitionandreduce0_fu_178"}],
		"Port" : [
			{"Name" : "gmem0", "Type" : "MAXI", "Direction" : "IO",
				"SubConnect" : [
					{"ID" : "5", "SubInstance" : "grp_partitionandreduce0_fu_178", "Port" : "kvsourcedram_V"}]},
			{"Name" : "gmem1", "Type" : "MAXI", "Direction" : "IO",
				"SubConnect" : [
					{"ID" : "5", "SubInstance" : "grp_partitionandreduce0_fu_178", "Port" : "kvdestdram_V"}]},
			{"Name" : "gmem2", "Type" : "MAXI", "Direction" : "IO",
				"BlockSignal" : [
					{"Name" : "gmem2_blk_n_AR", "Type" : "RtlSignal"},
					{"Name" : "gmem2_blk_n_R", "Type" : "RtlSignal"}],
				"SubConnect" : [
					{"ID" : "5", "SubInstance" : "grp_partitionandreduce0_fu_178", "Port" : "kvstats"}]},
			{"Name" : "kvsourcedramA_V", "Type" : "None", "Direction" : "I"},
			{"Name" : "kvdestdramA_V", "Type" : "None", "Direction" : "I"},
			{"Name" : "kvstatsA", "Type" : "None", "Direction" : "I"}]},
	{"ID" : "1", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.topkernel_control_s_axi_U", "Parent" : "0"},
	{"ID" : "2", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.topkernel_gmem0_m_axi_U", "Parent" : "0"},
	{"ID" : "3", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.topkernel_gmem1_m_axi_U", "Parent" : "0"},
	{"ID" : "4", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.topkernel_gmem2_m_axi_U", "Parent" : "0"},
	{"ID" : "5", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178", "Parent" : "0", "Child" : ["6", "7", "8", "9", "10", "11", "12", "13", "14", "15", "16", "17", "18", "19", "20", "21", "22", "23", "24", "25", "26", "27", "28", "29", "30", "31", "32", "33", "34", "35", "36", "37", "38", "39", "40", "41", "42", "43", "44", "45", "46", "47", "48", "49", "50", "51", "52", "53", "54", "55", "56", "57", "58", "59", "60", "61", "62", "63", "64", "65", "66", "67", "68", "69", "70", "71", "72", "73", "74", "75", "76", "77", "78", "79", "80", "81", "82", "83", "84", "85", "86", "87", "88", "89", "90", "91", "92", "93", "94", "95", "96", "161", "174", "183", "232", "234", "236", "238", "240", "241", "242", "243", "244"],
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
					{"ID" : "236", "SubInstance" : "grp_readkeyvalues0_3_fu_1912", "Port" : "kvdram_V"},
					{"ID" : "238", "SubInstance" : "grp_readkeyvalues0_2_fu_1934", "Port" : "kvdram_V"},
					{"ID" : "234", "SubInstance" : "grp_savecapsules0_fu_1883", "Port" : "kvdram_V"},
					{"ID" : "232", "SubInstance" : "grp_readcapsules0_fu_1859", "Port" : "kvdram_V"},
					{"ID" : "174", "SubInstance" : "grp_savepartitions0_fu_1813", "Port" : "kvdram_V"}]},
			{"Name" : "kvsourcedram_V_offset", "Type" : "None", "Direction" : "I"},
			{"Name" : "kvdestdram_V", "Type" : "MAXI", "Direction" : "IO",
				"SubConnect" : [
					{"ID" : "240", "SubInstance" : "grp_savekeyvalues0_2_fu_1951", "Port" : "kvdram_V"},
					{"ID" : "241", "SubInstance" : "grp_readkeyvalues0_1_fu_1975", "Port" : "kvdram_V"}]},
			{"Name" : "kvdestdram_V_offset", "Type" : "None", "Direction" : "I"},
			{"Name" : "kvstats", "Type" : "MAXI", "Direction" : "IO",
				"BlockSignal" : [
					{"Name" : "kvstats_blk_n_AW", "Type" : "RtlSignal"},
					{"Name" : "kvstats_blk_n_W", "Type" : "RtlSignal"},
					{"Name" : "kvstats_blk_n_B", "Type" : "RtlSignal"},
					{"Name" : "kvstats_blk_n_AR", "Type" : "RtlSignal"},
					{"Name" : "kvstats_blk_n_R", "Type" : "RtlSignal"}],
				"SubConnect" : [
					{"ID" : "243", "SubInstance" : "grp_savekeyvalues0_1_fu_2057", "Port" : "dram"},
					{"ID" : "242", "SubInstance" : "grp_savekeyvalues0_251_fu_2047", "Port" : "dram"}]},
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
	{"ID" : "6", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.sourcebuffer00_0_V_U", "Parent" : "5"},
	{"ID" : "7", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.sourcebuffer00_1_V_U", "Parent" : "5"},
	{"ID" : "8", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.destbuffer00_V_U", "Parent" : "5"},
	{"ID" : "9", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.capsule00_key_U", "Parent" : "5"},
	{"ID" : "10", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.capsule00_value_U", "Parent" : "5"},
	{"ID" : "11", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.BIGcapsule00_V_U", "Parent" : "5"},
	{"ID" : "12", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.sourcebuffer01_0_V_U", "Parent" : "5"},
	{"ID" : "13", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.sourcebuffer01_1_V_U", "Parent" : "5"},
	{"ID" : "14", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.destbuffer01_V_U", "Parent" : "5"},
	{"ID" : "15", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.capsule01_key_U", "Parent" : "5"},
	{"ID" : "16", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.capsule01_value_U", "Parent" : "5"},
	{"ID" : "17", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.BIGcapsule01_V_U", "Parent" : "5"},
	{"ID" : "18", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.sourcebuffer02_0_V_U", "Parent" : "5"},
	{"ID" : "19", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.sourcebuffer02_1_V_U", "Parent" : "5"},
	{"ID" : "20", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.destbuffer02_V_U", "Parent" : "5"},
	{"ID" : "21", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.capsule02_key_U", "Parent" : "5"},
	{"ID" : "22", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.capsule02_value_U", "Parent" : "5"},
	{"ID" : "23", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.BIGcapsule02_V_U", "Parent" : "5"},
	{"ID" : "24", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.sourcebuffer03_0_V_U", "Parent" : "5"},
	{"ID" : "25", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.sourcebuffer03_1_V_U", "Parent" : "5"},
	{"ID" : "26", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.destbuffer03_V_U", "Parent" : "5"},
	{"ID" : "27", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.capsule03_key_U", "Parent" : "5"},
	{"ID" : "28", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.capsule03_value_U", "Parent" : "5"},
	{"ID" : "29", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.BIGcapsule03_V_U", "Parent" : "5"},
	{"ID" : "30", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.destbuffer000_key_U", "Parent" : "5"},
	{"ID" : "31", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.destbuffer000_value_U", "Parent" : "5"},
	{"ID" : "32", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.destbuffer001_key_U", "Parent" : "5"},
	{"ID" : "33", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.destbuffer001_value_U", "Parent" : "5"},
	{"ID" : "34", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.destbuffer002_key_U", "Parent" : "5"},
	{"ID" : "35", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.destbuffer002_value_U", "Parent" : "5"},
	{"ID" : "36", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.destbuffer003_key_U", "Parent" : "5"},
	{"ID" : "37", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.destbuffer003_value_U", "Parent" : "5"},
	{"ID" : "38", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.destbuffer004_key_U", "Parent" : "5"},
	{"ID" : "39", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.destbuffer004_value_U", "Parent" : "5"},
	{"ID" : "40", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.destbuffer005_key_U", "Parent" : "5"},
	{"ID" : "41", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.destbuffer005_value_U", "Parent" : "5"},
	{"ID" : "42", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.destbuffer006_key_U", "Parent" : "5"},
	{"ID" : "43", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.destbuffer006_value_U", "Parent" : "5"},
	{"ID" : "44", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.destbuffer007_key_U", "Parent" : "5"},
	{"ID" : "45", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.destbuffer007_value_U", "Parent" : "5"},
	{"ID" : "46", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.destbuffer010_key_U", "Parent" : "5"},
	{"ID" : "47", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.destbuffer010_value_U", "Parent" : "5"},
	{"ID" : "48", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.destbuffer011_key_U", "Parent" : "5"},
	{"ID" : "49", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.destbuffer011_value_U", "Parent" : "5"},
	{"ID" : "50", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.destbuffer012_key_U", "Parent" : "5"},
	{"ID" : "51", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.destbuffer012_value_U", "Parent" : "5"},
	{"ID" : "52", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.destbuffer013_key_U", "Parent" : "5"},
	{"ID" : "53", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.destbuffer013_value_U", "Parent" : "5"},
	{"ID" : "54", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.destbuffer014_key_U", "Parent" : "5"},
	{"ID" : "55", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.destbuffer014_value_U", "Parent" : "5"},
	{"ID" : "56", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.destbuffer015_key_U", "Parent" : "5"},
	{"ID" : "57", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.destbuffer015_value_U", "Parent" : "5"},
	{"ID" : "58", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.destbuffer016_key_U", "Parent" : "5"},
	{"ID" : "59", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.destbuffer016_value_U", "Parent" : "5"},
	{"ID" : "60", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.destbuffer017_key_U", "Parent" : "5"},
	{"ID" : "61", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.destbuffer017_value_U", "Parent" : "5"},
	{"ID" : "62", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.destbuffer020_key_U", "Parent" : "5"},
	{"ID" : "63", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.destbuffer020_value_U", "Parent" : "5"},
	{"ID" : "64", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.destbuffer021_key_U", "Parent" : "5"},
	{"ID" : "65", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.destbuffer021_value_U", "Parent" : "5"},
	{"ID" : "66", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.destbuffer022_key_U", "Parent" : "5"},
	{"ID" : "67", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.destbuffer022_value_U", "Parent" : "5"},
	{"ID" : "68", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.destbuffer023_key_U", "Parent" : "5"},
	{"ID" : "69", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.destbuffer023_value_U", "Parent" : "5"},
	{"ID" : "70", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.destbuffer024_key_U", "Parent" : "5"},
	{"ID" : "71", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.destbuffer024_value_U", "Parent" : "5"},
	{"ID" : "72", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.destbuffer025_key_U", "Parent" : "5"},
	{"ID" : "73", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.destbuffer025_value_U", "Parent" : "5"},
	{"ID" : "74", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.destbuffer026_key_U", "Parent" : "5"},
	{"ID" : "75", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.destbuffer026_value_U", "Parent" : "5"},
	{"ID" : "76", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.destbuffer027_key_U", "Parent" : "5"},
	{"ID" : "77", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.destbuffer027_value_U", "Parent" : "5"},
	{"ID" : "78", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.destbuffer030_key_U", "Parent" : "5"},
	{"ID" : "79", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.destbuffer030_value_U", "Parent" : "5"},
	{"ID" : "80", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.destbuffer031_key_U", "Parent" : "5"},
	{"ID" : "81", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.destbuffer031_value_U", "Parent" : "5"},
	{"ID" : "82", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.destbuffer032_key_U", "Parent" : "5"},
	{"ID" : "83", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.destbuffer032_value_U", "Parent" : "5"},
	{"ID" : "84", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.destbuffer033_key_U", "Parent" : "5"},
	{"ID" : "85", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.destbuffer033_value_U", "Parent" : "5"},
	{"ID" : "86", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.destbuffer034_key_U", "Parent" : "5"},
	{"ID" : "87", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.destbuffer034_value_U", "Parent" : "5"},
	{"ID" : "88", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.destbuffer035_key_U", "Parent" : "5"},
	{"ID" : "89", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.destbuffer035_value_U", "Parent" : "5"},
	{"ID" : "90", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.destbuffer036_key_U", "Parent" : "5"},
	{"ID" : "91", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.destbuffer036_value_U", "Parent" : "5"},
	{"ID" : "92", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.destbuffer037_key_U", "Parent" : "5"},
	{"ID" : "93", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.destbuffer037_value_U", "Parent" : "5"},
	{"ID" : "94", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.kvdeststats_tmp_key_U", "Parent" : "5"},
	{"ID" : "95", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.kvdeststats_tmp_valu_U", "Parent" : "5"},
	{"ID" : "96", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1742", "Parent" : "5", "Child" : ["97", "98", "99", "100", "101", "102", "103", "104", "105", "106", "107", "108", "109", "110", "111", "112", "113", "114", "115", "116", "117", "118", "119", "120", "121", "122", "123", "124", "125", "126", "127", "128", "129", "130", "131", "132", "133", "134", "135", "136", "137", "138", "139", "140", "141", "142", "143", "144", "145", "146", "147", "148", "149", "150", "151", "152", "153", "154", "155", "156", "157", "158", "159", "160"],
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
	{"ID" : "97", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1742.topkernel_mux_83_32_1_1_U228", "Parent" : "96"},
	{"ID" : "98", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1742.topkernel_mux_83_32_1_1_U229", "Parent" : "96"},
	{"ID" : "99", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1742.topkernel_mux_83_32_1_1_U230", "Parent" : "96"},
	{"ID" : "100", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1742.topkernel_mux_83_32_1_1_U231", "Parent" : "96"},
	{"ID" : "101", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1742.topkernel_mux_83_32_1_1_U232", "Parent" : "96"},
	{"ID" : "102", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1742.topkernel_mux_83_32_1_1_U233", "Parent" : "96"},
	{"ID" : "103", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1742.topkernel_mux_83_32_1_1_U234", "Parent" : "96"},
	{"ID" : "104", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1742.topkernel_mux_83_32_1_1_U235", "Parent" : "96"},
	{"ID" : "105", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1742.topkernel_mux_83_32_1_1_U236", "Parent" : "96"},
	{"ID" : "106", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1742.topkernel_mux_83_32_1_1_U237", "Parent" : "96"},
	{"ID" : "107", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1742.topkernel_mux_83_32_1_1_U238", "Parent" : "96"},
	{"ID" : "108", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1742.topkernel_mux_83_32_1_1_U239", "Parent" : "96"},
	{"ID" : "109", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1742.topkernel_mux_83_32_1_1_U240", "Parent" : "96"},
	{"ID" : "110", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1742.topkernel_mux_83_32_1_1_U241", "Parent" : "96"},
	{"ID" : "111", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1742.topkernel_mux_83_32_1_1_U242", "Parent" : "96"},
	{"ID" : "112", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1742.topkernel_mux_83_32_1_1_U243", "Parent" : "96"},
	{"ID" : "113", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1742.topkernel_mux_83_32_1_1_U244", "Parent" : "96"},
	{"ID" : "114", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1742.topkernel_mux_83_32_1_1_U245", "Parent" : "96"},
	{"ID" : "115", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1742.topkernel_mux_83_32_1_1_U246", "Parent" : "96"},
	{"ID" : "116", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1742.topkernel_mux_83_32_1_1_U247", "Parent" : "96"},
	{"ID" : "117", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1742.topkernel_mux_83_32_1_1_U248", "Parent" : "96"},
	{"ID" : "118", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1742.topkernel_mux_83_32_1_1_U249", "Parent" : "96"},
	{"ID" : "119", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1742.topkernel_mux_83_32_1_1_U250", "Parent" : "96"},
	{"ID" : "120", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1742.topkernel_mux_83_32_1_1_U251", "Parent" : "96"},
	{"ID" : "121", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1742.topkernel_mux_83_32_1_1_U252", "Parent" : "96"},
	{"ID" : "122", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1742.topkernel_mux_83_32_1_1_U253", "Parent" : "96"},
	{"ID" : "123", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1742.topkernel_mux_83_32_1_1_U254", "Parent" : "96"},
	{"ID" : "124", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1742.topkernel_mux_83_32_1_1_U255", "Parent" : "96"},
	{"ID" : "125", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1742.topkernel_mux_83_32_1_1_U256", "Parent" : "96"},
	{"ID" : "126", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1742.topkernel_mux_83_32_1_1_U257", "Parent" : "96"},
	{"ID" : "127", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1742.topkernel_mux_83_32_1_1_U258", "Parent" : "96"},
	{"ID" : "128", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1742.topkernel_mux_83_32_1_1_U259", "Parent" : "96"},
	{"ID" : "129", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1742.topkernel_mux_83_32_1_1_U260", "Parent" : "96"},
	{"ID" : "130", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1742.topkernel_mux_83_32_1_1_U261", "Parent" : "96"},
	{"ID" : "131", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1742.topkernel_mux_83_32_1_1_U262", "Parent" : "96"},
	{"ID" : "132", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1742.topkernel_mux_83_32_1_1_U263", "Parent" : "96"},
	{"ID" : "133", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1742.topkernel_mux_83_32_1_1_U264", "Parent" : "96"},
	{"ID" : "134", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1742.topkernel_mux_83_32_1_1_U265", "Parent" : "96"},
	{"ID" : "135", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1742.topkernel_mux_83_32_1_1_U266", "Parent" : "96"},
	{"ID" : "136", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1742.topkernel_mux_83_32_1_1_U267", "Parent" : "96"},
	{"ID" : "137", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1742.topkernel_mux_83_32_1_1_U268", "Parent" : "96"},
	{"ID" : "138", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1742.topkernel_mux_83_32_1_1_U269", "Parent" : "96"},
	{"ID" : "139", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1742.topkernel_mux_83_32_1_1_U270", "Parent" : "96"},
	{"ID" : "140", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1742.topkernel_mux_83_32_1_1_U271", "Parent" : "96"},
	{"ID" : "141", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1742.topkernel_mux_83_32_1_1_U272", "Parent" : "96"},
	{"ID" : "142", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1742.topkernel_mux_83_32_1_1_U273", "Parent" : "96"},
	{"ID" : "143", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1742.topkernel_mux_83_32_1_1_U274", "Parent" : "96"},
	{"ID" : "144", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1742.topkernel_mux_83_32_1_1_U275", "Parent" : "96"},
	{"ID" : "145", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1742.topkernel_mux_83_32_1_1_U276", "Parent" : "96"},
	{"ID" : "146", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1742.topkernel_mux_83_32_1_1_U277", "Parent" : "96"},
	{"ID" : "147", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1742.topkernel_mux_83_32_1_1_U278", "Parent" : "96"},
	{"ID" : "148", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1742.topkernel_mux_83_32_1_1_U279", "Parent" : "96"},
	{"ID" : "149", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1742.topkernel_mux_83_32_1_1_U280", "Parent" : "96"},
	{"ID" : "150", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1742.topkernel_mux_83_32_1_1_U281", "Parent" : "96"},
	{"ID" : "151", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1742.topkernel_mux_83_32_1_1_U282", "Parent" : "96"},
	{"ID" : "152", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1742.topkernel_mux_83_32_1_1_U283", "Parent" : "96"},
	{"ID" : "153", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1742.topkernel_mux_83_32_1_1_U284", "Parent" : "96"},
	{"ID" : "154", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1742.topkernel_mux_83_32_1_1_U285", "Parent" : "96"},
	{"ID" : "155", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1742.topkernel_mux_83_32_1_1_U286", "Parent" : "96"},
	{"ID" : "156", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1742.topkernel_mux_83_32_1_1_U287", "Parent" : "96"},
	{"ID" : "157", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1742.topkernel_mux_83_32_1_1_U288", "Parent" : "96"},
	{"ID" : "158", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1742.topkernel_mux_83_32_1_1_U289", "Parent" : "96"},
	{"ID" : "159", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1742.topkernel_mux_83_32_1_1_U290", "Parent" : "96"},
	{"ID" : "160", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_reducepartitions0_fu_1742.topkernel_mux_83_32_1_1_U291", "Parent" : "96"},
	{"ID" : "161", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_partitionkeyvalues00_fu_1785", "Parent" : "5", "Child" : ["162", "163", "164", "165", "166", "167", "168", "169", "170", "171", "172", "173"],
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
	{"ID" : "162", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_partitionkeyvalues00_fu_1785.topkernel_mux_325_32_1_1_U85", "Parent" : "161"},
	{"ID" : "163", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_partitionkeyvalues00_fu_1785.topkernel_mux_325_32_1_1_U86", "Parent" : "161"},
	{"ID" : "164", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_partitionkeyvalues00_fu_1785.topkernel_mux_325_32_1_1_U87", "Parent" : "161"},
	{"ID" : "165", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_partitionkeyvalues00_fu_1785.topkernel_mux_325_32_1_1_U88", "Parent" : "161"},
	{"ID" : "166", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_partitionkeyvalues00_fu_1785.topkernel_mux_325_32_1_1_U89", "Parent" : "161"},
	{"ID" : "167", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_partitionkeyvalues00_fu_1785.topkernel_mux_325_32_1_1_U90", "Parent" : "161"},
	{"ID" : "168", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_partitionkeyvalues00_fu_1785.topkernel_mux_325_32_1_1_U91", "Parent" : "161"},
	{"ID" : "169", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_partitionkeyvalues00_fu_1785.topkernel_mux_325_32_1_1_U92", "Parent" : "161"},
	{"ID" : "170", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_partitionkeyvalues00_fu_1785.topkernel_mux_325_32_1_1_U93", "Parent" : "161"},
	{"ID" : "171", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_partitionkeyvalues00_fu_1785.topkernel_mux_325_32_1_1_U94", "Parent" : "161"},
	{"ID" : "172", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_partitionkeyvalues00_fu_1785.topkernel_mux_325_32_1_1_U95", "Parent" : "161"},
	{"ID" : "173", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_partitionkeyvalues00_fu_1785.topkernel_mux_325_32_1_1_U96", "Parent" : "161"},
	{"ID" : "174", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_savepartitions0_fu_1813", "Parent" : "5", "Child" : ["175", "176", "177", "178", "179", "180", "181", "182"],
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
	{"ID" : "175", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_savepartitions0_fu_1813.topkernel_urem_12ns_8ns_12_16_1_U119", "Parent" : "174"},
	{"ID" : "176", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_savepartitions0_fu_1813.topkernel_urem_12ns_8ns_12_16_1_U120", "Parent" : "174"},
	{"ID" : "177", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_savepartitions0_fu_1813.topkernel_urem_12ns_8ns_12_16_1_U121", "Parent" : "174"},
	{"ID" : "178", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_savepartitions0_fu_1813.topkernel_urem_12ns_8ns_12_16_1_U122", "Parent" : "174"},
	{"ID" : "179", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_savepartitions0_fu_1813.topkernel_urem_12ns_8ns_12_16_1_U123", "Parent" : "174"},
	{"ID" : "180", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_savepartitions0_fu_1813.topkernel_urem_12ns_8ns_12_16_1_U124", "Parent" : "174"},
	{"ID" : "181", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_savepartitions0_fu_1813.topkernel_urem_12ns_8ns_12_16_1_U125", "Parent" : "174"},
	{"ID" : "182", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_savepartitions0_fu_1813.topkernel_urem_12ns_8ns_12_16_1_U126", "Parent" : "174"},
	{"ID" : "183", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_collectstats00_fu_1835", "Parent" : "5", "Child" : ["184", "185", "186", "187", "188", "189", "190", "191", "192", "193", "194", "195", "196", "197", "198", "199", "200", "201", "202", "203", "204", "205", "206", "207", "208", "209", "210", "211", "212", "213", "214", "215", "216", "217", "218", "219", "220", "221", "222", "223", "224", "225", "226", "227", "228", "229", "230", "231"],
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
	{"ID" : "184", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_collectstats00_fu_1835.sizes00_U", "Parent" : "183"},
	{"ID" : "185", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_collectstats00_fu_1835.sizes01_U", "Parent" : "183"},
	{"ID" : "186", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_collectstats00_fu_1835.sizes02_U", "Parent" : "183"},
	{"ID" : "187", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_collectstats00_fu_1835.sizes03_U", "Parent" : "183"},
	{"ID" : "188", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_collectstats00_fu_1835.sizes04_U", "Parent" : "183"},
	{"ID" : "189", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_collectstats00_fu_1835.sizes05_U", "Parent" : "183"},
	{"ID" : "190", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_collectstats00_fu_1835.sizes06_U", "Parent" : "183"},
	{"ID" : "191", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_collectstats00_fu_1835.sizes07_U", "Parent" : "183"},
	{"ID" : "192", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_collectstats00_fu_1835.sizes10_U", "Parent" : "183"},
	{"ID" : "193", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_collectstats00_fu_1835.sizes11_U", "Parent" : "183"},
	{"ID" : "194", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_collectstats00_fu_1835.sizes12_U", "Parent" : "183"},
	{"ID" : "195", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_collectstats00_fu_1835.sizes13_U", "Parent" : "183"},
	{"ID" : "196", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_collectstats00_fu_1835.sizes14_U", "Parent" : "183"},
	{"ID" : "197", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_collectstats00_fu_1835.sizes15_U", "Parent" : "183"},
	{"ID" : "198", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_collectstats00_fu_1835.sizes16_U", "Parent" : "183"},
	{"ID" : "199", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_collectstats00_fu_1835.sizes17_U", "Parent" : "183"},
	{"ID" : "200", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_collectstats00_fu_1835.sizes20_U", "Parent" : "183"},
	{"ID" : "201", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_collectstats00_fu_1835.sizes21_U", "Parent" : "183"},
	{"ID" : "202", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_collectstats00_fu_1835.sizes22_U", "Parent" : "183"},
	{"ID" : "203", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_collectstats00_fu_1835.sizes23_U", "Parent" : "183"},
	{"ID" : "204", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_collectstats00_fu_1835.sizes24_U", "Parent" : "183"},
	{"ID" : "205", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_collectstats00_fu_1835.sizes25_U", "Parent" : "183"},
	{"ID" : "206", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_collectstats00_fu_1835.sizes26_U", "Parent" : "183"},
	{"ID" : "207", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_collectstats00_fu_1835.sizes27_U", "Parent" : "183"},
	{"ID" : "208", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_collectstats00_fu_1835.sizes30_U", "Parent" : "183"},
	{"ID" : "209", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_collectstats00_fu_1835.sizes31_U", "Parent" : "183"},
	{"ID" : "210", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_collectstats00_fu_1835.sizes32_U", "Parent" : "183"},
	{"ID" : "211", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_collectstats00_fu_1835.sizes33_U", "Parent" : "183"},
	{"ID" : "212", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_collectstats00_fu_1835.sizes34_U", "Parent" : "183"},
	{"ID" : "213", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_collectstats00_fu_1835.sizes35_U", "Parent" : "183"},
	{"ID" : "214", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_collectstats00_fu_1835.sizes36_U", "Parent" : "183"},
	{"ID" : "215", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_collectstats00_fu_1835.sizes37_U", "Parent" : "183"},
	{"ID" : "216", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_collectstats00_fu_1835.grp_getpartition_fu_1471", "Parent" : "183",
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
	{"ID" : "217", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_collectstats00_fu_1835.grp_getpartition_fu_1478", "Parent" : "183",
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
	{"ID" : "218", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_collectstats00_fu_1835.grp_getpartition_fu_1485", "Parent" : "183",
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
	{"ID" : "219", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_collectstats00_fu_1835.grp_getpartition_fu_1492", "Parent" : "183",
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
	{"ID" : "220", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_collectstats00_fu_1835.grp_getpartition_fu_1499", "Parent" : "183",
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
	{"ID" : "221", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_collectstats00_fu_1835.grp_getpartition_fu_1506", "Parent" : "183",
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
	{"ID" : "222", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_collectstats00_fu_1835.grp_getpartition_fu_1513", "Parent" : "183",
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
	{"ID" : "223", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_collectstats00_fu_1835.grp_getpartition_fu_1520", "Parent" : "183",
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
	{"ID" : "224", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_collectstats00_fu_1835.grp_getpartition_fu_1527", "Parent" : "183",
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
	{"ID" : "225", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_collectstats00_fu_1835.grp_getpartition_fu_1534", "Parent" : "183",
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
	{"ID" : "226", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_collectstats00_fu_1835.grp_getpartition_fu_1541", "Parent" : "183",
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
	{"ID" : "227", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_collectstats00_fu_1835.grp_getpartition_fu_1548", "Parent" : "183",
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
	{"ID" : "228", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_collectstats00_fu_1835.grp_getpartition_fu_1555", "Parent" : "183",
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
	{"ID" : "229", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_collectstats00_fu_1835.grp_getpartition_fu_1562", "Parent" : "183",
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
	{"ID" : "230", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_collectstats00_fu_1835.grp_getpartition_fu_1569", "Parent" : "183",
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
	{"ID" : "231", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_collectstats00_fu_1835.grp_getpartition_fu_1576", "Parent" : "183",
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
	{"ID" : "232", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_readcapsules0_fu_1859", "Parent" : "5", "Child" : ["233"],
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
					{"ID" : "233", "SubInstance" : "grp_readkeyvalues0_fu_418", "Port" : "kvdram_V"}]},
			{"Name" : "kvdram_V_offset", "Type" : "None", "Direction" : "I"},
			{"Name" : "BIGcapsule0_V", "Type" : "Memory", "Direction" : "IO",
				"SubConnect" : [
					{"ID" : "233", "SubInstance" : "grp_readkeyvalues0_fu_418", "Port" : "buffer_V"}]},
			{"Name" : "BIGcapsule1_V", "Type" : "Memory", "Direction" : "IO",
				"SubConnect" : [
					{"ID" : "233", "SubInstance" : "grp_readkeyvalues0_fu_418", "Port" : "buffer_V"}]},
			{"Name" : "BIGcapsule2_V", "Type" : "Memory", "Direction" : "IO",
				"SubConnect" : [
					{"ID" : "233", "SubInstance" : "grp_readkeyvalues0_fu_418", "Port" : "buffer_V"}]},
			{"Name" : "BIGcapsule3_V", "Type" : "Memory", "Direction" : "IO",
				"SubConnect" : [
					{"ID" : "233", "SubInstance" : "grp_readkeyvalues0_fu_418", "Port" : "buffer_V"}]},
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
	{"ID" : "233", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_readcapsules0_fu_1859.grp_readkeyvalues0_fu_418", "Parent" : "232",
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
	{"ID" : "234", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_savecapsules0_fu_1883", "Parent" : "5", "Child" : ["235"],
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
					{"ID" : "235", "SubInstance" : "grp_savekeyvalues0_fu_887", "Port" : "kvdram_V"}]},
			{"Name" : "kvdram_V_offset", "Type" : "None", "Direction" : "I"},
			{"Name" : "BIGcapsule0_V", "Type" : "Memory", "Direction" : "IO",
				"SubConnect" : [
					{"ID" : "235", "SubInstance" : "grp_savekeyvalues0_fu_887", "Port" : "buffer_V"}]},
			{"Name" : "BIGcapsule1_V", "Type" : "Memory", "Direction" : "IO",
				"SubConnect" : [
					{"ID" : "235", "SubInstance" : "grp_savekeyvalues0_fu_887", "Port" : "buffer_V"}]},
			{"Name" : "BIGcapsule2_V", "Type" : "Memory", "Direction" : "IO",
				"SubConnect" : [
					{"ID" : "235", "SubInstance" : "grp_savekeyvalues0_fu_887", "Port" : "buffer_V"}]},
			{"Name" : "BIGcapsule3_V", "Type" : "Memory", "Direction" : "IO",
				"SubConnect" : [
					{"ID" : "235", "SubInstance" : "grp_savekeyvalues0_fu_887", "Port" : "buffer_V"}]},
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
	{"ID" : "235", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_savecapsules0_fu_1883.grp_savekeyvalues0_fu_887", "Parent" : "234",
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
	{"ID" : "236", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_readkeyvalues0_3_fu_1912", "Parent" : "5", "Child" : ["237"],
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
					{"ID" : "237", "SubInstance" : "grp_readkeyvalues0_166_fu_136", "Port" : "kvdram_V"}]},
			{"Name" : "kvdram_V_offset", "Type" : "None", "Direction" : "I"},
			{"Name" : "buffer0_V", "Type" : "Memory", "Direction" : "O",
				"SubConnect" : [
					{"ID" : "237", "SubInstance" : "grp_readkeyvalues0_166_fu_136", "Port" : "buffer_V"}]},
			{"Name" : "buffer1_V", "Type" : "Memory", "Direction" : "O",
				"SubConnect" : [
					{"ID" : "237", "SubInstance" : "grp_readkeyvalues0_166_fu_136", "Port" : "buffer_V"}]},
			{"Name" : "buffer2_V", "Type" : "Memory", "Direction" : "O",
				"SubConnect" : [
					{"ID" : "237", "SubInstance" : "grp_readkeyvalues0_166_fu_136", "Port" : "buffer_V"}]},
			{"Name" : "buffer3_V", "Type" : "Memory", "Direction" : "O",
				"SubConnect" : [
					{"ID" : "237", "SubInstance" : "grp_readkeyvalues0_166_fu_136", "Port" : "buffer_V"}]},
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
	{"ID" : "237", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_readkeyvalues0_3_fu_1912.grp_readkeyvalues0_166_fu_136", "Parent" : "236",
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
	{"ID" : "238", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_readkeyvalues0_2_fu_1934", "Parent" : "5", "Child" : ["239"],
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
					{"ID" : "239", "SubInstance" : "grp_readkeyvalues0_166_fu_94", "Port" : "kvdram_V"}]},
			{"Name" : "kvdram_V_offset", "Type" : "None", "Direction" : "I"},
			{"Name" : "buffer0_V", "Type" : "Memory", "Direction" : "O",
				"SubConnect" : [
					{"ID" : "239", "SubInstance" : "grp_readkeyvalues0_166_fu_94", "Port" : "buffer_V"}]},
			{"Name" : "buffer1_V", "Type" : "Memory", "Direction" : "O",
				"SubConnect" : [
					{"ID" : "239", "SubInstance" : "grp_readkeyvalues0_166_fu_94", "Port" : "buffer_V"}]},
			{"Name" : "buffer2_V", "Type" : "Memory", "Direction" : "O",
				"SubConnect" : [
					{"ID" : "239", "SubInstance" : "grp_readkeyvalues0_166_fu_94", "Port" : "buffer_V"}]},
			{"Name" : "buffer3_V", "Type" : "Memory", "Direction" : "O",
				"SubConnect" : [
					{"ID" : "239", "SubInstance" : "grp_readkeyvalues0_166_fu_94", "Port" : "buffer_V"}]},
			{"Name" : "baseaddress", "Type" : "None", "Direction" : "I"},
			{"Name" : "offset_kvs", "Type" : "None", "Direction" : "I"},
			{"Name" : "travstate_i_kvs", "Type" : "None", "Direction" : "I"},
			{"Name" : "travstate_end_kvs", "Type" : "None", "Direction" : "I"}]},
	{"ID" : "239", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_readkeyvalues0_2_fu_1934.grp_readkeyvalues0_166_fu_94", "Parent" : "238",
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
	{"ID" : "240", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_savekeyvalues0_2_fu_1951", "Parent" : "5",
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
	{"ID" : "241", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_readkeyvalues0_1_fu_1975", "Parent" : "5",
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
	{"ID" : "242", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_savekeyvalues0_251_fu_2047", "Parent" : "5",
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
	{"ID" : "243", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.grp_savekeyvalues0_1_fu_2057", "Parent" : "5",
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
	{"ID" : "244", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_partitionandreduce0_fu_178.topkernel_mux_42_32_1_1_U360", "Parent" : "5"}]}


set ArgLastReadFirstWriteLatency {
	topkernel {
		gmem0 {Type IO LastRead 73 FirstWrite -1}
		gmem1 {Type IO LastRead 9 FirstWrite -1}
		gmem2 {Type IO LastRead 44 FirstWrite 2}
		kvsourcedramA_V {Type I LastRead 0 FirstWrite -1}
		kvdestdramA_V {Type I LastRead 0 FirstWrite -1}
		kvstatsA {Type I LastRead 0 FirstWrite -1}}
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
	{"Name" : "Latency", "Min" : "57", "Max" : "197844234"}
	, {"Name" : "Interval", "Min" : "58", "Max" : "197844235"}
]}

set PipelineEnableSignalInfo {[
]}

set Spec2ImplPortList { 
	gmem0 { m_axi {  { m_axi_gmem0_AWVALID VALID 1 1 }  { m_axi_gmem0_AWREADY READY 0 1 }  { m_axi_gmem0_AWADDR ADDR 1 32 }  { m_axi_gmem0_AWID ID 1 1 }  { m_axi_gmem0_AWLEN LEN 1 8 }  { m_axi_gmem0_AWSIZE SIZE 1 3 }  { m_axi_gmem0_AWBURST BURST 1 2 }  { m_axi_gmem0_AWLOCK LOCK 1 2 }  { m_axi_gmem0_AWCACHE CACHE 1 4 }  { m_axi_gmem0_AWPROT PROT 1 3 }  { m_axi_gmem0_AWQOS QOS 1 4 }  { m_axi_gmem0_AWREGION REGION 1 4 }  { m_axi_gmem0_AWUSER USER 1 1 }  { m_axi_gmem0_WVALID VALID 1 1 }  { m_axi_gmem0_WREADY READY 0 1 }  { m_axi_gmem0_WDATA DATA 1 512 }  { m_axi_gmem0_WSTRB STRB 1 64 }  { m_axi_gmem0_WLAST LAST 1 1 }  { m_axi_gmem0_WID ID 1 1 }  { m_axi_gmem0_WUSER USER 1 1 }  { m_axi_gmem0_ARVALID VALID 1 1 }  { m_axi_gmem0_ARREADY READY 0 1 }  { m_axi_gmem0_ARADDR ADDR 1 32 }  { m_axi_gmem0_ARID ID 1 1 }  { m_axi_gmem0_ARLEN LEN 1 8 }  { m_axi_gmem0_ARSIZE SIZE 1 3 }  { m_axi_gmem0_ARBURST BURST 1 2 }  { m_axi_gmem0_ARLOCK LOCK 1 2 }  { m_axi_gmem0_ARCACHE CACHE 1 4 }  { m_axi_gmem0_ARPROT PROT 1 3 }  { m_axi_gmem0_ARQOS QOS 1 4 }  { m_axi_gmem0_ARREGION REGION 1 4 }  { m_axi_gmem0_ARUSER USER 1 1 }  { m_axi_gmem0_RVALID VALID 0 1 }  { m_axi_gmem0_RREADY READY 1 1 }  { m_axi_gmem0_RDATA DATA 0 512 }  { m_axi_gmem0_RLAST LAST 0 1 }  { m_axi_gmem0_RID ID 0 1 }  { m_axi_gmem0_RUSER USER 0 1 }  { m_axi_gmem0_RRESP RESP 0 2 }  { m_axi_gmem0_BVALID VALID 0 1 }  { m_axi_gmem0_BREADY READY 1 1 }  { m_axi_gmem0_BRESP RESP 0 2 }  { m_axi_gmem0_BID ID 0 1 }  { m_axi_gmem0_BUSER USER 0 1 } } }
	gmem1 { m_axi {  { m_axi_gmem1_AWVALID VALID 1 1 }  { m_axi_gmem1_AWREADY READY 0 1 }  { m_axi_gmem1_AWADDR ADDR 1 32 }  { m_axi_gmem1_AWID ID 1 1 }  { m_axi_gmem1_AWLEN LEN 1 8 }  { m_axi_gmem1_AWSIZE SIZE 1 3 }  { m_axi_gmem1_AWBURST BURST 1 2 }  { m_axi_gmem1_AWLOCK LOCK 1 2 }  { m_axi_gmem1_AWCACHE CACHE 1 4 }  { m_axi_gmem1_AWPROT PROT 1 3 }  { m_axi_gmem1_AWQOS QOS 1 4 }  { m_axi_gmem1_AWREGION REGION 1 4 }  { m_axi_gmem1_AWUSER USER 1 1 }  { m_axi_gmem1_WVALID VALID 1 1 }  { m_axi_gmem1_WREADY READY 0 1 }  { m_axi_gmem1_WDATA DATA 1 512 }  { m_axi_gmem1_WSTRB STRB 1 64 }  { m_axi_gmem1_WLAST LAST 1 1 }  { m_axi_gmem1_WID ID 1 1 }  { m_axi_gmem1_WUSER USER 1 1 }  { m_axi_gmem1_ARVALID VALID 1 1 }  { m_axi_gmem1_ARREADY READY 0 1 }  { m_axi_gmem1_ARADDR ADDR 1 32 }  { m_axi_gmem1_ARID ID 1 1 }  { m_axi_gmem1_ARLEN LEN 1 8 }  { m_axi_gmem1_ARSIZE SIZE 1 3 }  { m_axi_gmem1_ARBURST BURST 1 2 }  { m_axi_gmem1_ARLOCK LOCK 1 2 }  { m_axi_gmem1_ARCACHE CACHE 1 4 }  { m_axi_gmem1_ARPROT PROT 1 3 }  { m_axi_gmem1_ARQOS QOS 1 4 }  { m_axi_gmem1_ARREGION REGION 1 4 }  { m_axi_gmem1_ARUSER USER 1 1 }  { m_axi_gmem1_RVALID VALID 0 1 }  { m_axi_gmem1_RREADY READY 1 1 }  { m_axi_gmem1_RDATA DATA 0 512 }  { m_axi_gmem1_RLAST LAST 0 1 }  { m_axi_gmem1_RID ID 0 1 }  { m_axi_gmem1_RUSER USER 0 1 }  { m_axi_gmem1_RRESP RESP 0 2 }  { m_axi_gmem1_BVALID VALID 0 1 }  { m_axi_gmem1_BREADY READY 1 1 }  { m_axi_gmem1_BRESP RESP 0 2 }  { m_axi_gmem1_BID ID 0 1 }  { m_axi_gmem1_BUSER USER 0 1 } } }
	gmem2 { m_axi {  { m_axi_gmem2_AWVALID VALID 1 1 }  { m_axi_gmem2_AWREADY READY 0 1 }  { m_axi_gmem2_AWADDR ADDR 1 32 }  { m_axi_gmem2_AWID ID 1 1 }  { m_axi_gmem2_AWLEN LEN 1 8 }  { m_axi_gmem2_AWSIZE SIZE 1 3 }  { m_axi_gmem2_AWBURST BURST 1 2 }  { m_axi_gmem2_AWLOCK LOCK 1 2 }  { m_axi_gmem2_AWCACHE CACHE 1 4 }  { m_axi_gmem2_AWPROT PROT 1 3 }  { m_axi_gmem2_AWQOS QOS 1 4 }  { m_axi_gmem2_AWREGION REGION 1 4 }  { m_axi_gmem2_AWUSER USER 1 1 }  { m_axi_gmem2_WVALID VALID 1 1 }  { m_axi_gmem2_WREADY READY 0 1 }  { m_axi_gmem2_WDATA DATA 1 64 }  { m_axi_gmem2_WSTRB STRB 1 8 }  { m_axi_gmem2_WLAST LAST 1 1 }  { m_axi_gmem2_WID ID 1 1 }  { m_axi_gmem2_WUSER USER 1 1 }  { m_axi_gmem2_ARVALID VALID 1 1 }  { m_axi_gmem2_ARREADY READY 0 1 }  { m_axi_gmem2_ARADDR ADDR 1 32 }  { m_axi_gmem2_ARID ID 1 1 }  { m_axi_gmem2_ARLEN LEN 1 8 }  { m_axi_gmem2_ARSIZE SIZE 1 3 }  { m_axi_gmem2_ARBURST BURST 1 2 }  { m_axi_gmem2_ARLOCK LOCK 1 2 }  { m_axi_gmem2_ARCACHE CACHE 1 4 }  { m_axi_gmem2_ARPROT PROT 1 3 }  { m_axi_gmem2_ARQOS QOS 1 4 }  { m_axi_gmem2_ARREGION REGION 1 4 }  { m_axi_gmem2_ARUSER USER 1 1 }  { m_axi_gmem2_RVALID VALID 0 1 }  { m_axi_gmem2_RREADY READY 1 1 }  { m_axi_gmem2_RDATA DATA 0 64 }  { m_axi_gmem2_RLAST LAST 0 1 }  { m_axi_gmem2_RID ID 0 1 }  { m_axi_gmem2_RUSER USER 0 1 }  { m_axi_gmem2_RRESP RESP 0 2 }  { m_axi_gmem2_BVALID VALID 0 1 }  { m_axi_gmem2_BREADY READY 1 1 }  { m_axi_gmem2_BRESP RESP 0 2 }  { m_axi_gmem2_BID ID 0 1 }  { m_axi_gmem2_BUSER USER 0 1 } } }
}

set busDeadlockParameterList { 
	{ gmem0 { NUM_READ_OUTSTANDING 16 NUM_WRITE_OUTSTANDING 16 MAX_READ_BURST_LENGTH 64 MAX_WRITE_BURST_LENGTH 64 } } \
	{ gmem1 { NUM_READ_OUTSTANDING 16 NUM_WRITE_OUTSTANDING 16 MAX_READ_BURST_LENGTH 64 MAX_WRITE_BURST_LENGTH 64 } } \
	{ gmem2 { NUM_READ_OUTSTANDING 16 NUM_WRITE_OUTSTANDING 16 MAX_READ_BURST_LENGTH 16 MAX_WRITE_BURST_LENGTH 16 } } \
}

# RTL port scheduling information:
set fifoSchedulingInfoList { 
}

# RTL bus port read request latency information:
set busReadReqLatencyList { 
	{ gmem0 1 }
	{ gmem1 1 }
	{ gmem2 1 }
}

# RTL bus port write response latency information:
set busWriteResLatencyList { 
	{ gmem0 1 }
	{ gmem1 1 }
	{ gmem2 1 }
}

# RTL array port load latency information:
set memoryLoadLatencyList { 
}
