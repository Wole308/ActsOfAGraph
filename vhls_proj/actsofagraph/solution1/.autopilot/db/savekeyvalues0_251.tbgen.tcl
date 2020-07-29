set moduleName savekeyvalues0_251
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
set C_modelName {savekeyvalues0.251}
set C_modelType { void 0 }
set C_modelArgList {
	{ dram int 64 regular {axi_master 1}  }
	{ dram_offset int 29 regular  }
	{ buffer_key int 32 regular {array 32 { 1 3 } 1 1 }  }
	{ buffer_value int 32 regular {array 32 { 1 3 } 1 1 }  }
	{ offset_kvs int 32 regular  }
}
set C_modelArgMapList {[ 
	{ "Name" : "dram", "interface" : "axi_master", "bitwidth" : 64, "direction" : "WRITEONLY"} , 
 	{ "Name" : "dram_offset", "interface" : "wire", "bitwidth" : 29, "direction" : "READONLY"} , 
 	{ "Name" : "buffer_key", "interface" : "memory", "bitwidth" : 32, "direction" : "READONLY"} , 
 	{ "Name" : "buffer_value", "interface" : "memory", "bitwidth" : 32, "direction" : "READONLY"} , 
 	{ "Name" : "offset_kvs", "interface" : "wire", "bitwidth" : 32, "direction" : "READONLY"} ]}
# RTL Port declarations: 
set portNum 59
set portList { 
	{ ap_clk sc_in sc_logic 1 clock -1 } 
	{ ap_rst sc_in sc_logic 1 reset -1 active_high_sync } 
	{ ap_start sc_in sc_logic 1 start -1 } 
	{ ap_done sc_out sc_logic 1 predone -1 } 
	{ ap_idle sc_out sc_logic 1 done -1 } 
	{ ap_ready sc_out sc_logic 1 ready -1 } 
	{ m_axi_dram_AWVALID sc_out sc_logic 1 signal 0 } 
	{ m_axi_dram_AWREADY sc_in sc_logic 1 signal 0 } 
	{ m_axi_dram_AWADDR sc_out sc_lv 32 signal 0 } 
	{ m_axi_dram_AWID sc_out sc_lv 1 signal 0 } 
	{ m_axi_dram_AWLEN sc_out sc_lv 32 signal 0 } 
	{ m_axi_dram_AWSIZE sc_out sc_lv 3 signal 0 } 
	{ m_axi_dram_AWBURST sc_out sc_lv 2 signal 0 } 
	{ m_axi_dram_AWLOCK sc_out sc_lv 2 signal 0 } 
	{ m_axi_dram_AWCACHE sc_out sc_lv 4 signal 0 } 
	{ m_axi_dram_AWPROT sc_out sc_lv 3 signal 0 } 
	{ m_axi_dram_AWQOS sc_out sc_lv 4 signal 0 } 
	{ m_axi_dram_AWREGION sc_out sc_lv 4 signal 0 } 
	{ m_axi_dram_AWUSER sc_out sc_lv 1 signal 0 } 
	{ m_axi_dram_WVALID sc_out sc_logic 1 signal 0 } 
	{ m_axi_dram_WREADY sc_in sc_logic 1 signal 0 } 
	{ m_axi_dram_WDATA sc_out sc_lv 64 signal 0 } 
	{ m_axi_dram_WSTRB sc_out sc_lv 8 signal 0 } 
	{ m_axi_dram_WLAST sc_out sc_logic 1 signal 0 } 
	{ m_axi_dram_WID sc_out sc_lv 1 signal 0 } 
	{ m_axi_dram_WUSER sc_out sc_lv 1 signal 0 } 
	{ m_axi_dram_ARVALID sc_out sc_logic 1 signal 0 } 
	{ m_axi_dram_ARREADY sc_in sc_logic 1 signal 0 } 
	{ m_axi_dram_ARADDR sc_out sc_lv 32 signal 0 } 
	{ m_axi_dram_ARID sc_out sc_lv 1 signal 0 } 
	{ m_axi_dram_ARLEN sc_out sc_lv 32 signal 0 } 
	{ m_axi_dram_ARSIZE sc_out sc_lv 3 signal 0 } 
	{ m_axi_dram_ARBURST sc_out sc_lv 2 signal 0 } 
	{ m_axi_dram_ARLOCK sc_out sc_lv 2 signal 0 } 
	{ m_axi_dram_ARCACHE sc_out sc_lv 4 signal 0 } 
	{ m_axi_dram_ARPROT sc_out sc_lv 3 signal 0 } 
	{ m_axi_dram_ARQOS sc_out sc_lv 4 signal 0 } 
	{ m_axi_dram_ARREGION sc_out sc_lv 4 signal 0 } 
	{ m_axi_dram_ARUSER sc_out sc_lv 1 signal 0 } 
	{ m_axi_dram_RVALID sc_in sc_logic 1 signal 0 } 
	{ m_axi_dram_RREADY sc_out sc_logic 1 signal 0 } 
	{ m_axi_dram_RDATA sc_in sc_lv 64 signal 0 } 
	{ m_axi_dram_RLAST sc_in sc_logic 1 signal 0 } 
	{ m_axi_dram_RID sc_in sc_lv 1 signal 0 } 
	{ m_axi_dram_RUSER sc_in sc_lv 1 signal 0 } 
	{ m_axi_dram_RRESP sc_in sc_lv 2 signal 0 } 
	{ m_axi_dram_BVALID sc_in sc_logic 1 signal 0 } 
	{ m_axi_dram_BREADY sc_out sc_logic 1 signal 0 } 
	{ m_axi_dram_BRESP sc_in sc_lv 2 signal 0 } 
	{ m_axi_dram_BID sc_in sc_lv 1 signal 0 } 
	{ m_axi_dram_BUSER sc_in sc_lv 1 signal 0 } 
	{ dram_offset sc_in sc_lv 29 signal 1 } 
	{ buffer_key_address0 sc_out sc_lv 5 signal 2 } 
	{ buffer_key_ce0 sc_out sc_logic 1 signal 2 } 
	{ buffer_key_q0 sc_in sc_lv 32 signal 2 } 
	{ buffer_value_address0 sc_out sc_lv 5 signal 3 } 
	{ buffer_value_ce0 sc_out sc_logic 1 signal 3 } 
	{ buffer_value_q0 sc_in sc_lv 32 signal 3 } 
	{ offset_kvs sc_in sc_lv 32 signal 4 } 
}
set NewPortList {[ 
	{ "name": "ap_clk", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "clock", "bundle":{"name": "ap_clk", "role": "default" }} , 
 	{ "name": "ap_rst", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "reset", "bundle":{"name": "ap_rst", "role": "default" }} , 
 	{ "name": "ap_start", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "start", "bundle":{"name": "ap_start", "role": "default" }} , 
 	{ "name": "ap_done", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "predone", "bundle":{"name": "ap_done", "role": "default" }} , 
 	{ "name": "ap_idle", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "done", "bundle":{"name": "ap_idle", "role": "default" }} , 
 	{ "name": "ap_ready", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "ready", "bundle":{"name": "ap_ready", "role": "default" }} , 
 	{ "name": "m_axi_dram_AWVALID", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "dram", "role": "AWVALID" }} , 
 	{ "name": "m_axi_dram_AWREADY", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "dram", "role": "AWREADY" }} , 
 	{ "name": "m_axi_dram_AWADDR", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "dram", "role": "AWADDR" }} , 
 	{ "name": "m_axi_dram_AWID", "direction": "out", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "dram", "role": "AWID" }} , 
 	{ "name": "m_axi_dram_AWLEN", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "dram", "role": "AWLEN" }} , 
 	{ "name": "m_axi_dram_AWSIZE", "direction": "out", "datatype": "sc_lv", "bitwidth":3, "type": "signal", "bundle":{"name": "dram", "role": "AWSIZE" }} , 
 	{ "name": "m_axi_dram_AWBURST", "direction": "out", "datatype": "sc_lv", "bitwidth":2, "type": "signal", "bundle":{"name": "dram", "role": "AWBURST" }} , 
 	{ "name": "m_axi_dram_AWLOCK", "direction": "out", "datatype": "sc_lv", "bitwidth":2, "type": "signal", "bundle":{"name": "dram", "role": "AWLOCK" }} , 
 	{ "name": "m_axi_dram_AWCACHE", "direction": "out", "datatype": "sc_lv", "bitwidth":4, "type": "signal", "bundle":{"name": "dram", "role": "AWCACHE" }} , 
 	{ "name": "m_axi_dram_AWPROT", "direction": "out", "datatype": "sc_lv", "bitwidth":3, "type": "signal", "bundle":{"name": "dram", "role": "AWPROT" }} , 
 	{ "name": "m_axi_dram_AWQOS", "direction": "out", "datatype": "sc_lv", "bitwidth":4, "type": "signal", "bundle":{"name": "dram", "role": "AWQOS" }} , 
 	{ "name": "m_axi_dram_AWREGION", "direction": "out", "datatype": "sc_lv", "bitwidth":4, "type": "signal", "bundle":{"name": "dram", "role": "AWREGION" }} , 
 	{ "name": "m_axi_dram_AWUSER", "direction": "out", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "dram", "role": "AWUSER" }} , 
 	{ "name": "m_axi_dram_WVALID", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "dram", "role": "WVALID" }} , 
 	{ "name": "m_axi_dram_WREADY", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "dram", "role": "WREADY" }} , 
 	{ "name": "m_axi_dram_WDATA", "direction": "out", "datatype": "sc_lv", "bitwidth":64, "type": "signal", "bundle":{"name": "dram", "role": "WDATA" }} , 
 	{ "name": "m_axi_dram_WSTRB", "direction": "out", "datatype": "sc_lv", "bitwidth":8, "type": "signal", "bundle":{"name": "dram", "role": "WSTRB" }} , 
 	{ "name": "m_axi_dram_WLAST", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "dram", "role": "WLAST" }} , 
 	{ "name": "m_axi_dram_WID", "direction": "out", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "dram", "role": "WID" }} , 
 	{ "name": "m_axi_dram_WUSER", "direction": "out", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "dram", "role": "WUSER" }} , 
 	{ "name": "m_axi_dram_ARVALID", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "dram", "role": "ARVALID" }} , 
 	{ "name": "m_axi_dram_ARREADY", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "dram", "role": "ARREADY" }} , 
 	{ "name": "m_axi_dram_ARADDR", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "dram", "role": "ARADDR" }} , 
 	{ "name": "m_axi_dram_ARID", "direction": "out", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "dram", "role": "ARID" }} , 
 	{ "name": "m_axi_dram_ARLEN", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "dram", "role": "ARLEN" }} , 
 	{ "name": "m_axi_dram_ARSIZE", "direction": "out", "datatype": "sc_lv", "bitwidth":3, "type": "signal", "bundle":{"name": "dram", "role": "ARSIZE" }} , 
 	{ "name": "m_axi_dram_ARBURST", "direction": "out", "datatype": "sc_lv", "bitwidth":2, "type": "signal", "bundle":{"name": "dram", "role": "ARBURST" }} , 
 	{ "name": "m_axi_dram_ARLOCK", "direction": "out", "datatype": "sc_lv", "bitwidth":2, "type": "signal", "bundle":{"name": "dram", "role": "ARLOCK" }} , 
 	{ "name": "m_axi_dram_ARCACHE", "direction": "out", "datatype": "sc_lv", "bitwidth":4, "type": "signal", "bundle":{"name": "dram", "role": "ARCACHE" }} , 
 	{ "name": "m_axi_dram_ARPROT", "direction": "out", "datatype": "sc_lv", "bitwidth":3, "type": "signal", "bundle":{"name": "dram", "role": "ARPROT" }} , 
 	{ "name": "m_axi_dram_ARQOS", "direction": "out", "datatype": "sc_lv", "bitwidth":4, "type": "signal", "bundle":{"name": "dram", "role": "ARQOS" }} , 
 	{ "name": "m_axi_dram_ARREGION", "direction": "out", "datatype": "sc_lv", "bitwidth":4, "type": "signal", "bundle":{"name": "dram", "role": "ARREGION" }} , 
 	{ "name": "m_axi_dram_ARUSER", "direction": "out", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "dram", "role": "ARUSER" }} , 
 	{ "name": "m_axi_dram_RVALID", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "dram", "role": "RVALID" }} , 
 	{ "name": "m_axi_dram_RREADY", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "dram", "role": "RREADY" }} , 
 	{ "name": "m_axi_dram_RDATA", "direction": "in", "datatype": "sc_lv", "bitwidth":64, "type": "signal", "bundle":{"name": "dram", "role": "RDATA" }} , 
 	{ "name": "m_axi_dram_RLAST", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "dram", "role": "RLAST" }} , 
 	{ "name": "m_axi_dram_RID", "direction": "in", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "dram", "role": "RID" }} , 
 	{ "name": "m_axi_dram_RUSER", "direction": "in", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "dram", "role": "RUSER" }} , 
 	{ "name": "m_axi_dram_RRESP", "direction": "in", "datatype": "sc_lv", "bitwidth":2, "type": "signal", "bundle":{"name": "dram", "role": "RRESP" }} , 
 	{ "name": "m_axi_dram_BVALID", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "dram", "role": "BVALID" }} , 
 	{ "name": "m_axi_dram_BREADY", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "dram", "role": "BREADY" }} , 
 	{ "name": "m_axi_dram_BRESP", "direction": "in", "datatype": "sc_lv", "bitwidth":2, "type": "signal", "bundle":{"name": "dram", "role": "BRESP" }} , 
 	{ "name": "m_axi_dram_BID", "direction": "in", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "dram", "role": "BID" }} , 
 	{ "name": "m_axi_dram_BUSER", "direction": "in", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "dram", "role": "BUSER" }} , 
 	{ "name": "dram_offset", "direction": "in", "datatype": "sc_lv", "bitwidth":29, "type": "signal", "bundle":{"name": "dram_offset", "role": "default" }} , 
 	{ "name": "buffer_key_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":5, "type": "signal", "bundle":{"name": "buffer_key", "role": "address0" }} , 
 	{ "name": "buffer_key_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "buffer_key", "role": "ce0" }} , 
 	{ "name": "buffer_key_q0", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "buffer_key", "role": "q0" }} , 
 	{ "name": "buffer_value_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":5, "type": "signal", "bundle":{"name": "buffer_value", "role": "address0" }} , 
 	{ "name": "buffer_value_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "buffer_value", "role": "ce0" }} , 
 	{ "name": "buffer_value_q0", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "buffer_value", "role": "q0" }} , 
 	{ "name": "offset_kvs", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "offset_kvs", "role": "default" }}  ]}

set RtlHierarchyInfo {[
	{"ID" : "0", "Level" : "0", "Path" : "`AUTOTB_DUT_INST", "Parent" : "",
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
			{"Name" : "offset_kvs", "Type" : "None", "Direction" : "I"}]}]}


set ArgLastReadFirstWriteLatency {
	savekeyvalues0_251 {
		dram {Type O LastRead 3 FirstWrite 4}
		dram_offset {Type I LastRead 0 FirstWrite -1}
		buffer_key {Type I LastRead 2 FirstWrite -1}
		buffer_value {Type I LastRead 2 FirstWrite -1}
		offset_kvs {Type I LastRead 0 FirstWrite -1}}}

set hasDtUnsupportedChannel 0

set PerformanceInfo {[
	{"Name" : "Latency", "Min" : "40", "Max" : "40"}
	, {"Name" : "Interval", "Min" : "40", "Max" : "40"}
]}

set PipelineEnableSignalInfo {[
	{"Pipeline" : "0", "EnableSignal" : "ap_enable_pp0"}
]}

set Spec2ImplPortList { 
	dram { m_axi {  { m_axi_dram_AWVALID VALID 1 1 }  { m_axi_dram_AWREADY READY 0 1 }  { m_axi_dram_AWADDR ADDR 1 32 }  { m_axi_dram_AWID ID 1 1 }  { m_axi_dram_AWLEN LEN 1 32 }  { m_axi_dram_AWSIZE SIZE 1 3 }  { m_axi_dram_AWBURST BURST 1 2 }  { m_axi_dram_AWLOCK LOCK 1 2 }  { m_axi_dram_AWCACHE CACHE 1 4 }  { m_axi_dram_AWPROT PROT 1 3 }  { m_axi_dram_AWQOS QOS 1 4 }  { m_axi_dram_AWREGION REGION 1 4 }  { m_axi_dram_AWUSER USER 1 1 }  { m_axi_dram_WVALID VALID 1 1 }  { m_axi_dram_WREADY READY 0 1 }  { m_axi_dram_WDATA DATA 1 64 }  { m_axi_dram_WSTRB STRB 1 8 }  { m_axi_dram_WLAST LAST 1 1 }  { m_axi_dram_WID ID 1 1 }  { m_axi_dram_WUSER USER 1 1 }  { m_axi_dram_ARVALID VALID 1 1 }  { m_axi_dram_ARREADY READY 0 1 }  { m_axi_dram_ARADDR ADDR 1 32 }  { m_axi_dram_ARID ID 1 1 }  { m_axi_dram_ARLEN LEN 1 32 }  { m_axi_dram_ARSIZE SIZE 1 3 }  { m_axi_dram_ARBURST BURST 1 2 }  { m_axi_dram_ARLOCK LOCK 1 2 }  { m_axi_dram_ARCACHE CACHE 1 4 }  { m_axi_dram_ARPROT PROT 1 3 }  { m_axi_dram_ARQOS QOS 1 4 }  { m_axi_dram_ARREGION REGION 1 4 }  { m_axi_dram_ARUSER USER 1 1 }  { m_axi_dram_RVALID VALID 0 1 }  { m_axi_dram_RREADY READY 1 1 }  { m_axi_dram_RDATA DATA 0 64 }  { m_axi_dram_RLAST LAST 0 1 }  { m_axi_dram_RID ID 0 1 }  { m_axi_dram_RUSER USER 0 1 }  { m_axi_dram_RRESP RESP 0 2 }  { m_axi_dram_BVALID VALID 0 1 }  { m_axi_dram_BREADY READY 1 1 }  { m_axi_dram_BRESP RESP 0 2 }  { m_axi_dram_BID ID 0 1 }  { m_axi_dram_BUSER USER 0 1 } } }
	dram_offset { ap_none {  { dram_offset in_data 0 29 } } }
	buffer_key { ap_memory {  { buffer_key_address0 mem_address 1 5 }  { buffer_key_ce0 mem_ce 1 1 }  { buffer_key_q0 mem_dout 0 32 } } }
	buffer_value { ap_memory {  { buffer_value_address0 mem_address 1 5 }  { buffer_value_ce0 mem_ce 1 1 }  { buffer_value_q0 mem_dout 0 32 } } }
	offset_kvs { ap_none {  { offset_kvs in_data 0 32 } } }
}
