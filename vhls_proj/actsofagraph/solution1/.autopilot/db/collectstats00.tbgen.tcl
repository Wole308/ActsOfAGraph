set moduleName collectstats00
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
set C_modelName {collectstats00}
set C_modelType { void 0 }
set C_modelArgList {
	{ enable int 1 regular  }
	{ sourcebuffer0_V int 512 regular {array 512 { 1 3 } 1 1 }  }
	{ sourcebuffer1_V int 512 regular {array 512 { 1 3 } 1 1 }  }
	{ sourcebuffer2_V int 512 regular {array 512 { 1 3 } 1 1 }  }
	{ sourcebuffer3_V int 512 regular {array 512 { 1 3 } 1 1 }  }
	{ capsule0_key int 32 regular {array 32 { 0 3 } 0 1 }  }
	{ capsule0_value int 32 regular {array 32 { 2 3 } 1 1 }  }
	{ capsule1_key int 32 regular {array 32 { 0 3 } 0 1 }  }
	{ capsule1_value int 32 regular {array 32 { 2 3 } 1 1 }  }
	{ capsule2_key int 32 regular {array 32 { 0 3 } 0 1 }  }
	{ capsule2_value int 32 regular {array 32 { 2 3 } 1 1 }  }
	{ capsule3_key int 32 regular {array 32 { 0 3 } 0 1 }  }
	{ capsule3_value int 32 regular {array 32 { 2 3 } 1 1 }  }
	{ llopparams_currentLOP int 32 regular  }
	{ llopparams_upperlimit int 32 regular  }
	{ travstate_i_kvs int 32 regular  }
	{ travstate_end_kvs int 30 regular  }
}
set C_modelArgMapList {[ 
	{ "Name" : "enable", "interface" : "wire", "bitwidth" : 1, "direction" : "READONLY"} , 
 	{ "Name" : "sourcebuffer0_V", "interface" : "memory", "bitwidth" : 512, "direction" : "READONLY"} , 
 	{ "Name" : "sourcebuffer1_V", "interface" : "memory", "bitwidth" : 512, "direction" : "READONLY"} , 
 	{ "Name" : "sourcebuffer2_V", "interface" : "memory", "bitwidth" : 512, "direction" : "READONLY"} , 
 	{ "Name" : "sourcebuffer3_V", "interface" : "memory", "bitwidth" : 512, "direction" : "READONLY"} , 
 	{ "Name" : "capsule0_key", "interface" : "memory", "bitwidth" : 32, "direction" : "WRITEONLY"} , 
 	{ "Name" : "capsule0_value", "interface" : "memory", "bitwidth" : 32, "direction" : "READWRITE"} , 
 	{ "Name" : "capsule1_key", "interface" : "memory", "bitwidth" : 32, "direction" : "WRITEONLY"} , 
 	{ "Name" : "capsule1_value", "interface" : "memory", "bitwidth" : 32, "direction" : "READWRITE"} , 
 	{ "Name" : "capsule2_key", "interface" : "memory", "bitwidth" : 32, "direction" : "WRITEONLY"} , 
 	{ "Name" : "capsule2_value", "interface" : "memory", "bitwidth" : 32, "direction" : "READWRITE"} , 
 	{ "Name" : "capsule3_key", "interface" : "memory", "bitwidth" : 32, "direction" : "WRITEONLY"} , 
 	{ "Name" : "capsule3_value", "interface" : "memory", "bitwidth" : 32, "direction" : "READWRITE"} , 
 	{ "Name" : "llopparams_currentLOP", "interface" : "wire", "bitwidth" : 32, "direction" : "READONLY"} , 
 	{ "Name" : "llopparams_upperlimit", "interface" : "wire", "bitwidth" : 32, "direction" : "READONLY"} , 
 	{ "Name" : "travstate_i_kvs", "interface" : "wire", "bitwidth" : 32, "direction" : "READONLY"} , 
 	{ "Name" : "travstate_end_kvs", "interface" : "wire", "bitwidth" : 30, "direction" : "READONLY"} ]}
# RTL Port declarations: 
set portNum 59
set portList { 
	{ ap_clk sc_in sc_logic 1 clock -1 } 
	{ ap_rst sc_in sc_logic 1 reset -1 active_high_sync } 
	{ ap_start sc_in sc_logic 1 start -1 } 
	{ ap_done sc_out sc_logic 1 predone -1 } 
	{ ap_idle sc_out sc_logic 1 done -1 } 
	{ ap_ready sc_out sc_logic 1 ready -1 } 
	{ enable sc_in sc_lv 1 signal 0 } 
	{ sourcebuffer0_V_address0 sc_out sc_lv 9 signal 1 } 
	{ sourcebuffer0_V_ce0 sc_out sc_logic 1 signal 1 } 
	{ sourcebuffer0_V_q0 sc_in sc_lv 512 signal 1 } 
	{ sourcebuffer1_V_address0 sc_out sc_lv 9 signal 2 } 
	{ sourcebuffer1_V_ce0 sc_out sc_logic 1 signal 2 } 
	{ sourcebuffer1_V_q0 sc_in sc_lv 512 signal 2 } 
	{ sourcebuffer2_V_address0 sc_out sc_lv 9 signal 3 } 
	{ sourcebuffer2_V_ce0 sc_out sc_logic 1 signal 3 } 
	{ sourcebuffer2_V_q0 sc_in sc_lv 512 signal 3 } 
	{ sourcebuffer3_V_address0 sc_out sc_lv 9 signal 4 } 
	{ sourcebuffer3_V_ce0 sc_out sc_logic 1 signal 4 } 
	{ sourcebuffer3_V_q0 sc_in sc_lv 512 signal 4 } 
	{ capsule0_key_address0 sc_out sc_lv 5 signal 5 } 
	{ capsule0_key_ce0 sc_out sc_logic 1 signal 5 } 
	{ capsule0_key_we0 sc_out sc_logic 1 signal 5 } 
	{ capsule0_key_d0 sc_out sc_lv 32 signal 5 } 
	{ capsule0_value_address0 sc_out sc_lv 5 signal 6 } 
	{ capsule0_value_ce0 sc_out sc_logic 1 signal 6 } 
	{ capsule0_value_we0 sc_out sc_logic 1 signal 6 } 
	{ capsule0_value_d0 sc_out sc_lv 32 signal 6 } 
	{ capsule0_value_q0 sc_in sc_lv 32 signal 6 } 
	{ capsule1_key_address0 sc_out sc_lv 5 signal 7 } 
	{ capsule1_key_ce0 sc_out sc_logic 1 signal 7 } 
	{ capsule1_key_we0 sc_out sc_logic 1 signal 7 } 
	{ capsule1_key_d0 sc_out sc_lv 32 signal 7 } 
	{ capsule1_value_address0 sc_out sc_lv 5 signal 8 } 
	{ capsule1_value_ce0 sc_out sc_logic 1 signal 8 } 
	{ capsule1_value_we0 sc_out sc_logic 1 signal 8 } 
	{ capsule1_value_d0 sc_out sc_lv 32 signal 8 } 
	{ capsule1_value_q0 sc_in sc_lv 32 signal 8 } 
	{ capsule2_key_address0 sc_out sc_lv 5 signal 9 } 
	{ capsule2_key_ce0 sc_out sc_logic 1 signal 9 } 
	{ capsule2_key_we0 sc_out sc_logic 1 signal 9 } 
	{ capsule2_key_d0 sc_out sc_lv 32 signal 9 } 
	{ capsule2_value_address0 sc_out sc_lv 5 signal 10 } 
	{ capsule2_value_ce0 sc_out sc_logic 1 signal 10 } 
	{ capsule2_value_we0 sc_out sc_logic 1 signal 10 } 
	{ capsule2_value_d0 sc_out sc_lv 32 signal 10 } 
	{ capsule2_value_q0 sc_in sc_lv 32 signal 10 } 
	{ capsule3_key_address0 sc_out sc_lv 5 signal 11 } 
	{ capsule3_key_ce0 sc_out sc_logic 1 signal 11 } 
	{ capsule3_key_we0 sc_out sc_logic 1 signal 11 } 
	{ capsule3_key_d0 sc_out sc_lv 32 signal 11 } 
	{ capsule3_value_address0 sc_out sc_lv 5 signal 12 } 
	{ capsule3_value_ce0 sc_out sc_logic 1 signal 12 } 
	{ capsule3_value_we0 sc_out sc_logic 1 signal 12 } 
	{ capsule3_value_d0 sc_out sc_lv 32 signal 12 } 
	{ capsule3_value_q0 sc_in sc_lv 32 signal 12 } 
	{ llopparams_currentLOP sc_in sc_lv 32 signal 13 } 
	{ llopparams_upperlimit sc_in sc_lv 32 signal 14 } 
	{ travstate_i_kvs sc_in sc_lv 32 signal 15 } 
	{ travstate_end_kvs sc_in sc_lv 30 signal 16 } 
}
set NewPortList {[ 
	{ "name": "ap_clk", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "clock", "bundle":{"name": "ap_clk", "role": "default" }} , 
 	{ "name": "ap_rst", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "reset", "bundle":{"name": "ap_rst", "role": "default" }} , 
 	{ "name": "ap_start", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "start", "bundle":{"name": "ap_start", "role": "default" }} , 
 	{ "name": "ap_done", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "predone", "bundle":{"name": "ap_done", "role": "default" }} , 
 	{ "name": "ap_idle", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "done", "bundle":{"name": "ap_idle", "role": "default" }} , 
 	{ "name": "ap_ready", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "ready", "bundle":{"name": "ap_ready", "role": "default" }} , 
 	{ "name": "enable", "direction": "in", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "enable", "role": "default" }} , 
 	{ "name": "sourcebuffer0_V_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":9, "type": "signal", "bundle":{"name": "sourcebuffer0_V", "role": "address0" }} , 
 	{ "name": "sourcebuffer0_V_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "sourcebuffer0_V", "role": "ce0" }} , 
 	{ "name": "sourcebuffer0_V_q0", "direction": "in", "datatype": "sc_lv", "bitwidth":512, "type": "signal", "bundle":{"name": "sourcebuffer0_V", "role": "q0" }} , 
 	{ "name": "sourcebuffer1_V_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":9, "type": "signal", "bundle":{"name": "sourcebuffer1_V", "role": "address0" }} , 
 	{ "name": "sourcebuffer1_V_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "sourcebuffer1_V", "role": "ce0" }} , 
 	{ "name": "sourcebuffer1_V_q0", "direction": "in", "datatype": "sc_lv", "bitwidth":512, "type": "signal", "bundle":{"name": "sourcebuffer1_V", "role": "q0" }} , 
 	{ "name": "sourcebuffer2_V_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":9, "type": "signal", "bundle":{"name": "sourcebuffer2_V", "role": "address0" }} , 
 	{ "name": "sourcebuffer2_V_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "sourcebuffer2_V", "role": "ce0" }} , 
 	{ "name": "sourcebuffer2_V_q0", "direction": "in", "datatype": "sc_lv", "bitwidth":512, "type": "signal", "bundle":{"name": "sourcebuffer2_V", "role": "q0" }} , 
 	{ "name": "sourcebuffer3_V_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":9, "type": "signal", "bundle":{"name": "sourcebuffer3_V", "role": "address0" }} , 
 	{ "name": "sourcebuffer3_V_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "sourcebuffer3_V", "role": "ce0" }} , 
 	{ "name": "sourcebuffer3_V_q0", "direction": "in", "datatype": "sc_lv", "bitwidth":512, "type": "signal", "bundle":{"name": "sourcebuffer3_V", "role": "q0" }} , 
 	{ "name": "capsule0_key_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":5, "type": "signal", "bundle":{"name": "capsule0_key", "role": "address0" }} , 
 	{ "name": "capsule0_key_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "capsule0_key", "role": "ce0" }} , 
 	{ "name": "capsule0_key_we0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "capsule0_key", "role": "we0" }} , 
 	{ "name": "capsule0_key_d0", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "capsule0_key", "role": "d0" }} , 
 	{ "name": "capsule0_value_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":5, "type": "signal", "bundle":{"name": "capsule0_value", "role": "address0" }} , 
 	{ "name": "capsule0_value_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "capsule0_value", "role": "ce0" }} , 
 	{ "name": "capsule0_value_we0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "capsule0_value", "role": "we0" }} , 
 	{ "name": "capsule0_value_d0", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "capsule0_value", "role": "d0" }} , 
 	{ "name": "capsule0_value_q0", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "capsule0_value", "role": "q0" }} , 
 	{ "name": "capsule1_key_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":5, "type": "signal", "bundle":{"name": "capsule1_key", "role": "address0" }} , 
 	{ "name": "capsule1_key_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "capsule1_key", "role": "ce0" }} , 
 	{ "name": "capsule1_key_we0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "capsule1_key", "role": "we0" }} , 
 	{ "name": "capsule1_key_d0", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "capsule1_key", "role": "d0" }} , 
 	{ "name": "capsule1_value_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":5, "type": "signal", "bundle":{"name": "capsule1_value", "role": "address0" }} , 
 	{ "name": "capsule1_value_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "capsule1_value", "role": "ce0" }} , 
 	{ "name": "capsule1_value_we0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "capsule1_value", "role": "we0" }} , 
 	{ "name": "capsule1_value_d0", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "capsule1_value", "role": "d0" }} , 
 	{ "name": "capsule1_value_q0", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "capsule1_value", "role": "q0" }} , 
 	{ "name": "capsule2_key_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":5, "type": "signal", "bundle":{"name": "capsule2_key", "role": "address0" }} , 
 	{ "name": "capsule2_key_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "capsule2_key", "role": "ce0" }} , 
 	{ "name": "capsule2_key_we0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "capsule2_key", "role": "we0" }} , 
 	{ "name": "capsule2_key_d0", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "capsule2_key", "role": "d0" }} , 
 	{ "name": "capsule2_value_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":5, "type": "signal", "bundle":{"name": "capsule2_value", "role": "address0" }} , 
 	{ "name": "capsule2_value_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "capsule2_value", "role": "ce0" }} , 
 	{ "name": "capsule2_value_we0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "capsule2_value", "role": "we0" }} , 
 	{ "name": "capsule2_value_d0", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "capsule2_value", "role": "d0" }} , 
 	{ "name": "capsule2_value_q0", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "capsule2_value", "role": "q0" }} , 
 	{ "name": "capsule3_key_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":5, "type": "signal", "bundle":{"name": "capsule3_key", "role": "address0" }} , 
 	{ "name": "capsule3_key_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "capsule3_key", "role": "ce0" }} , 
 	{ "name": "capsule3_key_we0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "capsule3_key", "role": "we0" }} , 
 	{ "name": "capsule3_key_d0", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "capsule3_key", "role": "d0" }} , 
 	{ "name": "capsule3_value_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":5, "type": "signal", "bundle":{"name": "capsule3_value", "role": "address0" }} , 
 	{ "name": "capsule3_value_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "capsule3_value", "role": "ce0" }} , 
 	{ "name": "capsule3_value_we0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "capsule3_value", "role": "we0" }} , 
 	{ "name": "capsule3_value_d0", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "capsule3_value", "role": "d0" }} , 
 	{ "name": "capsule3_value_q0", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "capsule3_value", "role": "q0" }} , 
 	{ "name": "llopparams_currentLOP", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "llopparams_currentLOP", "role": "default" }} , 
 	{ "name": "llopparams_upperlimit", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "llopparams_upperlimit", "role": "default" }} , 
 	{ "name": "travstate_i_kvs", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "travstate_i_kvs", "role": "default" }} , 
 	{ "name": "travstate_end_kvs", "direction": "in", "datatype": "sc_lv", "bitwidth":30, "type": "signal", "bundle":{"name": "travstate_end_kvs", "role": "default" }}  ]}

set RtlHierarchyInfo {[
	{"ID" : "0", "Level" : "0", "Path" : "`AUTOTB_DUT_INST", "Parent" : "", "Child" : ["1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13", "14", "15", "16", "17", "18", "19", "20", "21", "22", "23", "24", "25", "26", "27", "28", "29", "30", "31", "32", "33", "34", "35", "36", "37", "38", "39", "40", "41", "42", "43", "44", "45", "46", "47", "48"],
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
	{"ID" : "1", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.sizes00_U", "Parent" : "0"},
	{"ID" : "2", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.sizes01_U", "Parent" : "0"},
	{"ID" : "3", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.sizes02_U", "Parent" : "0"},
	{"ID" : "4", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.sizes03_U", "Parent" : "0"},
	{"ID" : "5", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.sizes04_U", "Parent" : "0"},
	{"ID" : "6", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.sizes05_U", "Parent" : "0"},
	{"ID" : "7", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.sizes06_U", "Parent" : "0"},
	{"ID" : "8", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.sizes07_U", "Parent" : "0"},
	{"ID" : "9", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.sizes10_U", "Parent" : "0"},
	{"ID" : "10", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.sizes11_U", "Parent" : "0"},
	{"ID" : "11", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.sizes12_U", "Parent" : "0"},
	{"ID" : "12", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.sizes13_U", "Parent" : "0"},
	{"ID" : "13", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.sizes14_U", "Parent" : "0"},
	{"ID" : "14", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.sizes15_U", "Parent" : "0"},
	{"ID" : "15", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.sizes16_U", "Parent" : "0"},
	{"ID" : "16", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.sizes17_U", "Parent" : "0"},
	{"ID" : "17", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.sizes20_U", "Parent" : "0"},
	{"ID" : "18", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.sizes21_U", "Parent" : "0"},
	{"ID" : "19", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.sizes22_U", "Parent" : "0"},
	{"ID" : "20", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.sizes23_U", "Parent" : "0"},
	{"ID" : "21", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.sizes24_U", "Parent" : "0"},
	{"ID" : "22", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.sizes25_U", "Parent" : "0"},
	{"ID" : "23", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.sizes26_U", "Parent" : "0"},
	{"ID" : "24", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.sizes27_U", "Parent" : "0"},
	{"ID" : "25", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.sizes30_U", "Parent" : "0"},
	{"ID" : "26", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.sizes31_U", "Parent" : "0"},
	{"ID" : "27", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.sizes32_U", "Parent" : "0"},
	{"ID" : "28", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.sizes33_U", "Parent" : "0"},
	{"ID" : "29", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.sizes34_U", "Parent" : "0"},
	{"ID" : "30", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.sizes35_U", "Parent" : "0"},
	{"ID" : "31", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.sizes36_U", "Parent" : "0"},
	{"ID" : "32", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.sizes37_U", "Parent" : "0"},
	{"ID" : "33", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.grp_getpartition_fu_1471", "Parent" : "0",
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
	{"ID" : "34", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.grp_getpartition_fu_1478", "Parent" : "0",
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
	{"ID" : "35", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.grp_getpartition_fu_1485", "Parent" : "0",
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
	{"ID" : "36", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.grp_getpartition_fu_1492", "Parent" : "0",
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
	{"ID" : "37", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.grp_getpartition_fu_1499", "Parent" : "0",
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
	{"ID" : "38", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.grp_getpartition_fu_1506", "Parent" : "0",
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
	{"ID" : "39", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.grp_getpartition_fu_1513", "Parent" : "0",
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
	{"ID" : "40", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.grp_getpartition_fu_1520", "Parent" : "0",
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
	{"ID" : "41", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.grp_getpartition_fu_1527", "Parent" : "0",
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
	{"ID" : "42", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.grp_getpartition_fu_1534", "Parent" : "0",
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
	{"ID" : "43", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.grp_getpartition_fu_1541", "Parent" : "0",
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
	{"ID" : "44", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.grp_getpartition_fu_1548", "Parent" : "0",
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
	{"ID" : "45", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.grp_getpartition_fu_1555", "Parent" : "0",
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
	{"ID" : "46", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.grp_getpartition_fu_1562", "Parent" : "0",
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
	{"ID" : "47", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.grp_getpartition_fu_1569", "Parent" : "0",
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
	{"ID" : "48", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.grp_getpartition_fu_1576", "Parent" : "0",
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
			{"Name" : "upperlimit", "Type" : "None", "Direction" : "I"}]}]}


set ArgLastReadFirstWriteLatency {
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
		upperlimit {Type I LastRead 0 FirstWrite -1}}}

set hasDtUnsupportedChannel 0

set PerformanceInfo {[
	{"Name" : "Latency", "Min" : "1", "Max" : "1126"}
	, {"Name" : "Interval", "Min" : "1", "Max" : "1126"}
]}

set PipelineEnableSignalInfo {[
	{"Pipeline" : "0", "EnableSignal" : "ap_enable_pp0"}
]}

set Spec2ImplPortList { 
	enable { ap_none {  { enable in_data 0 1 } } }
	sourcebuffer0_V { ap_memory {  { sourcebuffer0_V_address0 mem_address 1 9 }  { sourcebuffer0_V_ce0 mem_ce 1 1 }  { sourcebuffer0_V_q0 mem_dout 0 512 } } }
	sourcebuffer1_V { ap_memory {  { sourcebuffer1_V_address0 mem_address 1 9 }  { sourcebuffer1_V_ce0 mem_ce 1 1 }  { sourcebuffer1_V_q0 mem_dout 0 512 } } }
	sourcebuffer2_V { ap_memory {  { sourcebuffer2_V_address0 mem_address 1 9 }  { sourcebuffer2_V_ce0 mem_ce 1 1 }  { sourcebuffer2_V_q0 mem_dout 0 512 } } }
	sourcebuffer3_V { ap_memory {  { sourcebuffer3_V_address0 mem_address 1 9 }  { sourcebuffer3_V_ce0 mem_ce 1 1 }  { sourcebuffer3_V_q0 mem_dout 0 512 } } }
	capsule0_key { ap_memory {  { capsule0_key_address0 mem_address 1 5 }  { capsule0_key_ce0 mem_ce 1 1 }  { capsule0_key_we0 mem_we 1 1 }  { capsule0_key_d0 mem_din 1 32 } } }
	capsule0_value { ap_memory {  { capsule0_value_address0 mem_address 1 5 }  { capsule0_value_ce0 mem_ce 1 1 }  { capsule0_value_we0 mem_we 1 1 }  { capsule0_value_d0 mem_din 1 32 }  { capsule0_value_q0 mem_dout 0 32 } } }
	capsule1_key { ap_memory {  { capsule1_key_address0 mem_address 1 5 }  { capsule1_key_ce0 mem_ce 1 1 }  { capsule1_key_we0 mem_we 1 1 }  { capsule1_key_d0 mem_din 1 32 } } }
	capsule1_value { ap_memory {  { capsule1_value_address0 mem_address 1 5 }  { capsule1_value_ce0 mem_ce 1 1 }  { capsule1_value_we0 mem_we 1 1 }  { capsule1_value_d0 mem_din 1 32 }  { capsule1_value_q0 mem_dout 0 32 } } }
	capsule2_key { ap_memory {  { capsule2_key_address0 mem_address 1 5 }  { capsule2_key_ce0 mem_ce 1 1 }  { capsule2_key_we0 mem_we 1 1 }  { capsule2_key_d0 mem_din 1 32 } } }
	capsule2_value { ap_memory {  { capsule2_value_address0 mem_address 1 5 }  { capsule2_value_ce0 mem_ce 1 1 }  { capsule2_value_we0 mem_we 1 1 }  { capsule2_value_d0 mem_din 1 32 }  { capsule2_value_q0 mem_dout 0 32 } } }
	capsule3_key { ap_memory {  { capsule3_key_address0 mem_address 1 5 }  { capsule3_key_ce0 mem_ce 1 1 }  { capsule3_key_we0 mem_we 1 1 }  { capsule3_key_d0 mem_din 1 32 } } }
	capsule3_value { ap_memory {  { capsule3_value_address0 mem_address 1 5 }  { capsule3_value_ce0 mem_ce 1 1 }  { capsule3_value_we0 mem_we 1 1 }  { capsule3_value_d0 mem_din 1 32 }  { capsule3_value_q0 mem_dout 0 32 } } }
	llopparams_currentLOP { ap_none {  { llopparams_currentLOP in_data 0 32 } } }
	llopparams_upperlimit { ap_none {  { llopparams_upperlimit in_data 0 32 } } }
	travstate_i_kvs { ap_none {  { travstate_i_kvs in_data 0 32 } } }
	travstate_end_kvs { ap_none {  { travstate_end_kvs in_data 0 30 } } }
}
set moduleName collectstats00
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
set C_modelName {collectstats00}
set C_modelType { void 0 }
set C_modelArgList {
	{ enable int 1 regular  }
	{ sourcebuffer0_V int 512 regular {array 512 { 1 3 } 1 1 }  }
	{ sourcebuffer1_V int 512 regular {array 512 { 1 3 } 1 1 }  }
	{ sourcebuffer2_V int 512 regular {array 512 { 1 3 } 1 1 }  }
	{ sourcebuffer3_V int 512 regular {array 512 { 1 3 } 1 1 }  }
	{ capsule0_key int 32 regular {array 32 { 0 3 } 0 1 }  }
	{ capsule0_value int 32 regular {array 32 { 2 3 } 1 1 }  }
	{ capsule1_key int 32 regular {array 32 { 0 3 } 0 1 }  }
	{ capsule1_value int 32 regular {array 32 { 2 3 } 1 1 }  }
	{ capsule2_key int 32 regular {array 32 { 0 3 } 0 1 }  }
	{ capsule2_value int 32 regular {array 32 { 2 3 } 1 1 }  }
	{ capsule3_key int 32 regular {array 32 { 0 3 } 0 1 }  }
	{ capsule3_value int 32 regular {array 32 { 2 3 } 1 1 }  }
	{ llopparams_currentLOP int 32 regular  }
	{ llopparams_upperlimit int 32 regular  }
	{ travstate_i_kvs int 32 regular  }
	{ travstate_end_kvs int 30 regular  }
}
set C_modelArgMapList {[ 
	{ "Name" : "enable", "interface" : "wire", "bitwidth" : 1, "direction" : "READONLY"} , 
 	{ "Name" : "sourcebuffer0_V", "interface" : "memory", "bitwidth" : 512, "direction" : "READONLY"} , 
 	{ "Name" : "sourcebuffer1_V", "interface" : "memory", "bitwidth" : 512, "direction" : "READONLY"} , 
 	{ "Name" : "sourcebuffer2_V", "interface" : "memory", "bitwidth" : 512, "direction" : "READONLY"} , 
 	{ "Name" : "sourcebuffer3_V", "interface" : "memory", "bitwidth" : 512, "direction" : "READONLY"} , 
 	{ "Name" : "capsule0_key", "interface" : "memory", "bitwidth" : 32, "direction" : "WRITEONLY"} , 
 	{ "Name" : "capsule0_value", "interface" : "memory", "bitwidth" : 32, "direction" : "READWRITE"} , 
 	{ "Name" : "capsule1_key", "interface" : "memory", "bitwidth" : 32, "direction" : "WRITEONLY"} , 
 	{ "Name" : "capsule1_value", "interface" : "memory", "bitwidth" : 32, "direction" : "READWRITE"} , 
 	{ "Name" : "capsule2_key", "interface" : "memory", "bitwidth" : 32, "direction" : "WRITEONLY"} , 
 	{ "Name" : "capsule2_value", "interface" : "memory", "bitwidth" : 32, "direction" : "READWRITE"} , 
 	{ "Name" : "capsule3_key", "interface" : "memory", "bitwidth" : 32, "direction" : "WRITEONLY"} , 
 	{ "Name" : "capsule3_value", "interface" : "memory", "bitwidth" : 32, "direction" : "READWRITE"} , 
 	{ "Name" : "llopparams_currentLOP", "interface" : "wire", "bitwidth" : 32, "direction" : "READONLY"} , 
 	{ "Name" : "llopparams_upperlimit", "interface" : "wire", "bitwidth" : 32, "direction" : "READONLY"} , 
 	{ "Name" : "travstate_i_kvs", "interface" : "wire", "bitwidth" : 32, "direction" : "READONLY"} , 
 	{ "Name" : "travstate_end_kvs", "interface" : "wire", "bitwidth" : 30, "direction" : "READONLY"} ]}
# RTL Port declarations: 
set portNum 59
set portList { 
	{ ap_clk sc_in sc_logic 1 clock -1 } 
	{ ap_rst sc_in sc_logic 1 reset -1 active_high_sync } 
	{ ap_start sc_in sc_logic 1 start -1 } 
	{ ap_done sc_out sc_logic 1 predone -1 } 
	{ ap_idle sc_out sc_logic 1 done -1 } 
	{ ap_ready sc_out sc_logic 1 ready -1 } 
	{ enable sc_in sc_lv 1 signal 0 } 
	{ sourcebuffer0_V_address0 sc_out sc_lv 9 signal 1 } 
	{ sourcebuffer0_V_ce0 sc_out sc_logic 1 signal 1 } 
	{ sourcebuffer0_V_q0 sc_in sc_lv 512 signal 1 } 
	{ sourcebuffer1_V_address0 sc_out sc_lv 9 signal 2 } 
	{ sourcebuffer1_V_ce0 sc_out sc_logic 1 signal 2 } 
	{ sourcebuffer1_V_q0 sc_in sc_lv 512 signal 2 } 
	{ sourcebuffer2_V_address0 sc_out sc_lv 9 signal 3 } 
	{ sourcebuffer2_V_ce0 sc_out sc_logic 1 signal 3 } 
	{ sourcebuffer2_V_q0 sc_in sc_lv 512 signal 3 } 
	{ sourcebuffer3_V_address0 sc_out sc_lv 9 signal 4 } 
	{ sourcebuffer3_V_ce0 sc_out sc_logic 1 signal 4 } 
	{ sourcebuffer3_V_q0 sc_in sc_lv 512 signal 4 } 
	{ capsule0_key_address0 sc_out sc_lv 5 signal 5 } 
	{ capsule0_key_ce0 sc_out sc_logic 1 signal 5 } 
	{ capsule0_key_we0 sc_out sc_logic 1 signal 5 } 
	{ capsule0_key_d0 sc_out sc_lv 32 signal 5 } 
	{ capsule0_value_address0 sc_out sc_lv 5 signal 6 } 
	{ capsule0_value_ce0 sc_out sc_logic 1 signal 6 } 
	{ capsule0_value_we0 sc_out sc_logic 1 signal 6 } 
	{ capsule0_value_d0 sc_out sc_lv 32 signal 6 } 
	{ capsule0_value_q0 sc_in sc_lv 32 signal 6 } 
	{ capsule1_key_address0 sc_out sc_lv 5 signal 7 } 
	{ capsule1_key_ce0 sc_out sc_logic 1 signal 7 } 
	{ capsule1_key_we0 sc_out sc_logic 1 signal 7 } 
	{ capsule1_key_d0 sc_out sc_lv 32 signal 7 } 
	{ capsule1_value_address0 sc_out sc_lv 5 signal 8 } 
	{ capsule1_value_ce0 sc_out sc_logic 1 signal 8 } 
	{ capsule1_value_we0 sc_out sc_logic 1 signal 8 } 
	{ capsule1_value_d0 sc_out sc_lv 32 signal 8 } 
	{ capsule1_value_q0 sc_in sc_lv 32 signal 8 } 
	{ capsule2_key_address0 sc_out sc_lv 5 signal 9 } 
	{ capsule2_key_ce0 sc_out sc_logic 1 signal 9 } 
	{ capsule2_key_we0 sc_out sc_logic 1 signal 9 } 
	{ capsule2_key_d0 sc_out sc_lv 32 signal 9 } 
	{ capsule2_value_address0 sc_out sc_lv 5 signal 10 } 
	{ capsule2_value_ce0 sc_out sc_logic 1 signal 10 } 
	{ capsule2_value_we0 sc_out sc_logic 1 signal 10 } 
	{ capsule2_value_d0 sc_out sc_lv 32 signal 10 } 
	{ capsule2_value_q0 sc_in sc_lv 32 signal 10 } 
	{ capsule3_key_address0 sc_out sc_lv 5 signal 11 } 
	{ capsule3_key_ce0 sc_out sc_logic 1 signal 11 } 
	{ capsule3_key_we0 sc_out sc_logic 1 signal 11 } 
	{ capsule3_key_d0 sc_out sc_lv 32 signal 11 } 
	{ capsule3_value_address0 sc_out sc_lv 5 signal 12 } 
	{ capsule3_value_ce0 sc_out sc_logic 1 signal 12 } 
	{ capsule3_value_we0 sc_out sc_logic 1 signal 12 } 
	{ capsule3_value_d0 sc_out sc_lv 32 signal 12 } 
	{ capsule3_value_q0 sc_in sc_lv 32 signal 12 } 
	{ llopparams_currentLOP sc_in sc_lv 32 signal 13 } 
	{ llopparams_upperlimit sc_in sc_lv 32 signal 14 } 
	{ travstate_i_kvs sc_in sc_lv 32 signal 15 } 
	{ travstate_end_kvs sc_in sc_lv 30 signal 16 } 
}
set NewPortList {[ 
	{ "name": "ap_clk", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "clock", "bundle":{"name": "ap_clk", "role": "default" }} , 
 	{ "name": "ap_rst", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "reset", "bundle":{"name": "ap_rst", "role": "default" }} , 
 	{ "name": "ap_start", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "start", "bundle":{"name": "ap_start", "role": "default" }} , 
 	{ "name": "ap_done", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "predone", "bundle":{"name": "ap_done", "role": "default" }} , 
 	{ "name": "ap_idle", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "done", "bundle":{"name": "ap_idle", "role": "default" }} , 
 	{ "name": "ap_ready", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "ready", "bundle":{"name": "ap_ready", "role": "default" }} , 
 	{ "name": "enable", "direction": "in", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "enable", "role": "default" }} , 
 	{ "name": "sourcebuffer0_V_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":9, "type": "signal", "bundle":{"name": "sourcebuffer0_V", "role": "address0" }} , 
 	{ "name": "sourcebuffer0_V_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "sourcebuffer0_V", "role": "ce0" }} , 
 	{ "name": "sourcebuffer0_V_q0", "direction": "in", "datatype": "sc_lv", "bitwidth":512, "type": "signal", "bundle":{"name": "sourcebuffer0_V", "role": "q0" }} , 
 	{ "name": "sourcebuffer1_V_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":9, "type": "signal", "bundle":{"name": "sourcebuffer1_V", "role": "address0" }} , 
 	{ "name": "sourcebuffer1_V_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "sourcebuffer1_V", "role": "ce0" }} , 
 	{ "name": "sourcebuffer1_V_q0", "direction": "in", "datatype": "sc_lv", "bitwidth":512, "type": "signal", "bundle":{"name": "sourcebuffer1_V", "role": "q0" }} , 
 	{ "name": "sourcebuffer2_V_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":9, "type": "signal", "bundle":{"name": "sourcebuffer2_V", "role": "address0" }} , 
 	{ "name": "sourcebuffer2_V_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "sourcebuffer2_V", "role": "ce0" }} , 
 	{ "name": "sourcebuffer2_V_q0", "direction": "in", "datatype": "sc_lv", "bitwidth":512, "type": "signal", "bundle":{"name": "sourcebuffer2_V", "role": "q0" }} , 
 	{ "name": "sourcebuffer3_V_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":9, "type": "signal", "bundle":{"name": "sourcebuffer3_V", "role": "address0" }} , 
 	{ "name": "sourcebuffer3_V_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "sourcebuffer3_V", "role": "ce0" }} , 
 	{ "name": "sourcebuffer3_V_q0", "direction": "in", "datatype": "sc_lv", "bitwidth":512, "type": "signal", "bundle":{"name": "sourcebuffer3_V", "role": "q0" }} , 
 	{ "name": "capsule0_key_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":5, "type": "signal", "bundle":{"name": "capsule0_key", "role": "address0" }} , 
 	{ "name": "capsule0_key_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "capsule0_key", "role": "ce0" }} , 
 	{ "name": "capsule0_key_we0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "capsule0_key", "role": "we0" }} , 
 	{ "name": "capsule0_key_d0", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "capsule0_key", "role": "d0" }} , 
 	{ "name": "capsule0_value_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":5, "type": "signal", "bundle":{"name": "capsule0_value", "role": "address0" }} , 
 	{ "name": "capsule0_value_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "capsule0_value", "role": "ce0" }} , 
 	{ "name": "capsule0_value_we0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "capsule0_value", "role": "we0" }} , 
 	{ "name": "capsule0_value_d0", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "capsule0_value", "role": "d0" }} , 
 	{ "name": "capsule0_value_q0", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "capsule0_value", "role": "q0" }} , 
 	{ "name": "capsule1_key_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":5, "type": "signal", "bundle":{"name": "capsule1_key", "role": "address0" }} , 
 	{ "name": "capsule1_key_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "capsule1_key", "role": "ce0" }} , 
 	{ "name": "capsule1_key_we0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "capsule1_key", "role": "we0" }} , 
 	{ "name": "capsule1_key_d0", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "capsule1_key", "role": "d0" }} , 
 	{ "name": "capsule1_value_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":5, "type": "signal", "bundle":{"name": "capsule1_value", "role": "address0" }} , 
 	{ "name": "capsule1_value_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "capsule1_value", "role": "ce0" }} , 
 	{ "name": "capsule1_value_we0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "capsule1_value", "role": "we0" }} , 
 	{ "name": "capsule1_value_d0", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "capsule1_value", "role": "d0" }} , 
 	{ "name": "capsule1_value_q0", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "capsule1_value", "role": "q0" }} , 
 	{ "name": "capsule2_key_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":5, "type": "signal", "bundle":{"name": "capsule2_key", "role": "address0" }} , 
 	{ "name": "capsule2_key_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "capsule2_key", "role": "ce0" }} , 
 	{ "name": "capsule2_key_we0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "capsule2_key", "role": "we0" }} , 
 	{ "name": "capsule2_key_d0", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "capsule2_key", "role": "d0" }} , 
 	{ "name": "capsule2_value_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":5, "type": "signal", "bundle":{"name": "capsule2_value", "role": "address0" }} , 
 	{ "name": "capsule2_value_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "capsule2_value", "role": "ce0" }} , 
 	{ "name": "capsule2_value_we0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "capsule2_value", "role": "we0" }} , 
 	{ "name": "capsule2_value_d0", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "capsule2_value", "role": "d0" }} , 
 	{ "name": "capsule2_value_q0", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "capsule2_value", "role": "q0" }} , 
 	{ "name": "capsule3_key_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":5, "type": "signal", "bundle":{"name": "capsule3_key", "role": "address0" }} , 
 	{ "name": "capsule3_key_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "capsule3_key", "role": "ce0" }} , 
 	{ "name": "capsule3_key_we0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "capsule3_key", "role": "we0" }} , 
 	{ "name": "capsule3_key_d0", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "capsule3_key", "role": "d0" }} , 
 	{ "name": "capsule3_value_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":5, "type": "signal", "bundle":{"name": "capsule3_value", "role": "address0" }} , 
 	{ "name": "capsule3_value_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "capsule3_value", "role": "ce0" }} , 
 	{ "name": "capsule3_value_we0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "capsule3_value", "role": "we0" }} , 
 	{ "name": "capsule3_value_d0", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "capsule3_value", "role": "d0" }} , 
 	{ "name": "capsule3_value_q0", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "capsule3_value", "role": "q0" }} , 
 	{ "name": "llopparams_currentLOP", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "llopparams_currentLOP", "role": "default" }} , 
 	{ "name": "llopparams_upperlimit", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "llopparams_upperlimit", "role": "default" }} , 
 	{ "name": "travstate_i_kvs", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "travstate_i_kvs", "role": "default" }} , 
 	{ "name": "travstate_end_kvs", "direction": "in", "datatype": "sc_lv", "bitwidth":30, "type": "signal", "bundle":{"name": "travstate_end_kvs", "role": "default" }}  ]}

set RtlHierarchyInfo {[
	{"ID" : "0", "Level" : "0", "Path" : "`AUTOTB_DUT_INST", "Parent" : "", "Child" : ["1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13", "14", "15", "16", "17", "18", "19", "20", "21", "22", "23", "24", "25", "26", "27", "28", "29", "30", "31", "32", "33", "34", "35", "36", "37", "38", "39", "40", "41", "42", "43", "44", "45", "46", "47", "48"],
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
	{"ID" : "1", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.sizes00_U", "Parent" : "0"},
	{"ID" : "2", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.sizes01_U", "Parent" : "0"},
	{"ID" : "3", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.sizes02_U", "Parent" : "0"},
	{"ID" : "4", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.sizes03_U", "Parent" : "0"},
	{"ID" : "5", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.sizes04_U", "Parent" : "0"},
	{"ID" : "6", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.sizes05_U", "Parent" : "0"},
	{"ID" : "7", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.sizes06_U", "Parent" : "0"},
	{"ID" : "8", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.sizes07_U", "Parent" : "0"},
	{"ID" : "9", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.sizes10_U", "Parent" : "0"},
	{"ID" : "10", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.sizes11_U", "Parent" : "0"},
	{"ID" : "11", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.sizes12_U", "Parent" : "0"},
	{"ID" : "12", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.sizes13_U", "Parent" : "0"},
	{"ID" : "13", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.sizes14_U", "Parent" : "0"},
	{"ID" : "14", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.sizes15_U", "Parent" : "0"},
	{"ID" : "15", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.sizes16_U", "Parent" : "0"},
	{"ID" : "16", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.sizes17_U", "Parent" : "0"},
	{"ID" : "17", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.sizes20_U", "Parent" : "0"},
	{"ID" : "18", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.sizes21_U", "Parent" : "0"},
	{"ID" : "19", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.sizes22_U", "Parent" : "0"},
	{"ID" : "20", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.sizes23_U", "Parent" : "0"},
	{"ID" : "21", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.sizes24_U", "Parent" : "0"},
	{"ID" : "22", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.sizes25_U", "Parent" : "0"},
	{"ID" : "23", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.sizes26_U", "Parent" : "0"},
	{"ID" : "24", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.sizes27_U", "Parent" : "0"},
	{"ID" : "25", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.sizes30_U", "Parent" : "0"},
	{"ID" : "26", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.sizes31_U", "Parent" : "0"},
	{"ID" : "27", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.sizes32_U", "Parent" : "0"},
	{"ID" : "28", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.sizes33_U", "Parent" : "0"},
	{"ID" : "29", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.sizes34_U", "Parent" : "0"},
	{"ID" : "30", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.sizes35_U", "Parent" : "0"},
	{"ID" : "31", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.sizes36_U", "Parent" : "0"},
	{"ID" : "32", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.sizes37_U", "Parent" : "0"},
	{"ID" : "33", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.grp_getpartition_fu_1471", "Parent" : "0",
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
	{"ID" : "34", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.grp_getpartition_fu_1478", "Parent" : "0",
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
	{"ID" : "35", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.grp_getpartition_fu_1485", "Parent" : "0",
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
	{"ID" : "36", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.grp_getpartition_fu_1492", "Parent" : "0",
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
	{"ID" : "37", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.grp_getpartition_fu_1499", "Parent" : "0",
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
	{"ID" : "38", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.grp_getpartition_fu_1506", "Parent" : "0",
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
	{"ID" : "39", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.grp_getpartition_fu_1513", "Parent" : "0",
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
	{"ID" : "40", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.grp_getpartition_fu_1520", "Parent" : "0",
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
	{"ID" : "41", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.grp_getpartition_fu_1527", "Parent" : "0",
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
	{"ID" : "42", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.grp_getpartition_fu_1534", "Parent" : "0",
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
	{"ID" : "43", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.grp_getpartition_fu_1541", "Parent" : "0",
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
	{"ID" : "44", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.grp_getpartition_fu_1548", "Parent" : "0",
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
	{"ID" : "45", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.grp_getpartition_fu_1555", "Parent" : "0",
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
	{"ID" : "46", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.grp_getpartition_fu_1562", "Parent" : "0",
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
	{"ID" : "47", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.grp_getpartition_fu_1569", "Parent" : "0",
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
	{"ID" : "48", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.grp_getpartition_fu_1576", "Parent" : "0",
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
			{"Name" : "upperlimit", "Type" : "None", "Direction" : "I"}]}]}


set ArgLastReadFirstWriteLatency {
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
		upperlimit {Type I LastRead 0 FirstWrite -1}}}

set hasDtUnsupportedChannel 0

set PerformanceInfo {[
	{"Name" : "Latency", "Min" : "1", "Max" : "1126"}
	, {"Name" : "Interval", "Min" : "1", "Max" : "1126"}
]}

set PipelineEnableSignalInfo {[
	{"Pipeline" : "0", "EnableSignal" : "ap_enable_pp0"}
]}

set Spec2ImplPortList { 
	enable { ap_none {  { enable in_data 0 1 } } }
	sourcebuffer0_V { ap_memory {  { sourcebuffer0_V_address0 mem_address 1 9 }  { sourcebuffer0_V_ce0 mem_ce 1 1 }  { sourcebuffer0_V_q0 mem_dout 0 512 } } }
	sourcebuffer1_V { ap_memory {  { sourcebuffer1_V_address0 mem_address 1 9 }  { sourcebuffer1_V_ce0 mem_ce 1 1 }  { sourcebuffer1_V_q0 mem_dout 0 512 } } }
	sourcebuffer2_V { ap_memory {  { sourcebuffer2_V_address0 mem_address 1 9 }  { sourcebuffer2_V_ce0 mem_ce 1 1 }  { sourcebuffer2_V_q0 mem_dout 0 512 } } }
	sourcebuffer3_V { ap_memory {  { sourcebuffer3_V_address0 mem_address 1 9 }  { sourcebuffer3_V_ce0 mem_ce 1 1 }  { sourcebuffer3_V_q0 mem_dout 0 512 } } }
	capsule0_key { ap_memory {  { capsule0_key_address0 mem_address 1 5 }  { capsule0_key_ce0 mem_ce 1 1 }  { capsule0_key_we0 mem_we 1 1 }  { capsule0_key_d0 mem_din 1 32 } } }
	capsule0_value { ap_memory {  { capsule0_value_address0 mem_address 1 5 }  { capsule0_value_ce0 mem_ce 1 1 }  { capsule0_value_we0 mem_we 1 1 }  { capsule0_value_d0 mem_din 1 32 }  { capsule0_value_q0 mem_dout 0 32 } } }
	capsule1_key { ap_memory {  { capsule1_key_address0 mem_address 1 5 }  { capsule1_key_ce0 mem_ce 1 1 }  { capsule1_key_we0 mem_we 1 1 }  { capsule1_key_d0 mem_din 1 32 } } }
	capsule1_value { ap_memory {  { capsule1_value_address0 mem_address 1 5 }  { capsule1_value_ce0 mem_ce 1 1 }  { capsule1_value_we0 mem_we 1 1 }  { capsule1_value_d0 mem_din 1 32 }  { capsule1_value_q0 mem_dout 0 32 } } }
	capsule2_key { ap_memory {  { capsule2_key_address0 mem_address 1 5 }  { capsule2_key_ce0 mem_ce 1 1 }  { capsule2_key_we0 mem_we 1 1 }  { capsule2_key_d0 mem_din 1 32 } } }
	capsule2_value { ap_memory {  { capsule2_value_address0 mem_address 1 5 }  { capsule2_value_ce0 mem_ce 1 1 }  { capsule2_value_we0 mem_we 1 1 }  { capsule2_value_d0 mem_din 1 32 }  { capsule2_value_q0 mem_dout 0 32 } } }
	capsule3_key { ap_memory {  { capsule3_key_address0 mem_address 1 5 }  { capsule3_key_ce0 mem_ce 1 1 }  { capsule3_key_we0 mem_we 1 1 }  { capsule3_key_d0 mem_din 1 32 } } }
	capsule3_value { ap_memory {  { capsule3_value_address0 mem_address 1 5 }  { capsule3_value_ce0 mem_ce 1 1 }  { capsule3_value_we0 mem_we 1 1 }  { capsule3_value_d0 mem_din 1 32 }  { capsule3_value_q0 mem_dout 0 32 } } }
	llopparams_currentLOP { ap_none {  { llopparams_currentLOP in_data 0 32 } } }
	llopparams_upperlimit { ap_none {  { llopparams_upperlimit in_data 0 32 } } }
	travstate_i_kvs { ap_none {  { travstate_i_kvs in_data 0 32 } } }
	travstate_end_kvs { ap_none {  { travstate_end_kvs in_data 0 30 } } }
}
set moduleName collectstats00
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
set C_modelName {collectstats00}
set C_modelType { void 0 }
set C_modelArgList {
	{ enable int 1 regular  }
	{ sourcebuffer0_V int 512 regular {array 512 { 1 3 } 1 1 }  }
	{ sourcebuffer1_V int 512 regular {array 512 { 1 3 } 1 1 }  }
	{ sourcebuffer2_V int 512 regular {array 512 { 1 3 } 1 1 }  }
	{ sourcebuffer3_V int 512 regular {array 512 { 1 3 } 1 1 }  }
	{ capsule0_key int 32 regular {array 32 { 0 3 } 0 1 }  }
	{ capsule0_value int 32 regular {array 32 { 2 3 } 1 1 }  }
	{ capsule1_key int 32 regular {array 32 { 0 3 } 0 1 }  }
	{ capsule1_value int 32 regular {array 32 { 2 3 } 1 1 }  }
	{ capsule2_key int 32 regular {array 32 { 0 3 } 0 1 }  }
	{ capsule2_value int 32 regular {array 32 { 2 3 } 1 1 }  }
	{ capsule3_key int 32 regular {array 32 { 0 3 } 0 1 }  }
	{ capsule3_value int 32 regular {array 32 { 2 3 } 1 1 }  }
	{ llopparams_currentLOP int 32 regular  }
	{ llopparams_upperlimit int 32 regular  }
	{ travstate_i_kvs int 32 regular  }
	{ travstate_end_kvs int 30 regular  }
}
set C_modelArgMapList {[ 
	{ "Name" : "enable", "interface" : "wire", "bitwidth" : 1, "direction" : "READONLY"} , 
 	{ "Name" : "sourcebuffer0_V", "interface" : "memory", "bitwidth" : 512, "direction" : "READONLY"} , 
 	{ "Name" : "sourcebuffer1_V", "interface" : "memory", "bitwidth" : 512, "direction" : "READONLY"} , 
 	{ "Name" : "sourcebuffer2_V", "interface" : "memory", "bitwidth" : 512, "direction" : "READONLY"} , 
 	{ "Name" : "sourcebuffer3_V", "interface" : "memory", "bitwidth" : 512, "direction" : "READONLY"} , 
 	{ "Name" : "capsule0_key", "interface" : "memory", "bitwidth" : 32, "direction" : "WRITEONLY"} , 
 	{ "Name" : "capsule0_value", "interface" : "memory", "bitwidth" : 32, "direction" : "READWRITE"} , 
 	{ "Name" : "capsule1_key", "interface" : "memory", "bitwidth" : 32, "direction" : "WRITEONLY"} , 
 	{ "Name" : "capsule1_value", "interface" : "memory", "bitwidth" : 32, "direction" : "READWRITE"} , 
 	{ "Name" : "capsule2_key", "interface" : "memory", "bitwidth" : 32, "direction" : "WRITEONLY"} , 
 	{ "Name" : "capsule2_value", "interface" : "memory", "bitwidth" : 32, "direction" : "READWRITE"} , 
 	{ "Name" : "capsule3_key", "interface" : "memory", "bitwidth" : 32, "direction" : "WRITEONLY"} , 
 	{ "Name" : "capsule3_value", "interface" : "memory", "bitwidth" : 32, "direction" : "READWRITE"} , 
 	{ "Name" : "llopparams_currentLOP", "interface" : "wire", "bitwidth" : 32, "direction" : "READONLY"} , 
 	{ "Name" : "llopparams_upperlimit", "interface" : "wire", "bitwidth" : 32, "direction" : "READONLY"} , 
 	{ "Name" : "travstate_i_kvs", "interface" : "wire", "bitwidth" : 32, "direction" : "READONLY"} , 
 	{ "Name" : "travstate_end_kvs", "interface" : "wire", "bitwidth" : 30, "direction" : "READONLY"} ]}
# RTL Port declarations: 
set portNum 59
set portList { 
	{ ap_clk sc_in sc_logic 1 clock -1 } 
	{ ap_rst sc_in sc_logic 1 reset -1 active_high_sync } 
	{ ap_start sc_in sc_logic 1 start -1 } 
	{ ap_done sc_out sc_logic 1 predone -1 } 
	{ ap_idle sc_out sc_logic 1 done -1 } 
	{ ap_ready sc_out sc_logic 1 ready -1 } 
	{ enable sc_in sc_lv 1 signal 0 } 
	{ sourcebuffer0_V_address0 sc_out sc_lv 9 signal 1 } 
	{ sourcebuffer0_V_ce0 sc_out sc_logic 1 signal 1 } 
	{ sourcebuffer0_V_q0 sc_in sc_lv 512 signal 1 } 
	{ sourcebuffer1_V_address0 sc_out sc_lv 9 signal 2 } 
	{ sourcebuffer1_V_ce0 sc_out sc_logic 1 signal 2 } 
	{ sourcebuffer1_V_q0 sc_in sc_lv 512 signal 2 } 
	{ sourcebuffer2_V_address0 sc_out sc_lv 9 signal 3 } 
	{ sourcebuffer2_V_ce0 sc_out sc_logic 1 signal 3 } 
	{ sourcebuffer2_V_q0 sc_in sc_lv 512 signal 3 } 
	{ sourcebuffer3_V_address0 sc_out sc_lv 9 signal 4 } 
	{ sourcebuffer3_V_ce0 sc_out sc_logic 1 signal 4 } 
	{ sourcebuffer3_V_q0 sc_in sc_lv 512 signal 4 } 
	{ capsule0_key_address0 sc_out sc_lv 5 signal 5 } 
	{ capsule0_key_ce0 sc_out sc_logic 1 signal 5 } 
	{ capsule0_key_we0 sc_out sc_logic 1 signal 5 } 
	{ capsule0_key_d0 sc_out sc_lv 32 signal 5 } 
	{ capsule0_value_address0 sc_out sc_lv 5 signal 6 } 
	{ capsule0_value_ce0 sc_out sc_logic 1 signal 6 } 
	{ capsule0_value_we0 sc_out sc_logic 1 signal 6 } 
	{ capsule0_value_d0 sc_out sc_lv 32 signal 6 } 
	{ capsule0_value_q0 sc_in sc_lv 32 signal 6 } 
	{ capsule1_key_address0 sc_out sc_lv 5 signal 7 } 
	{ capsule1_key_ce0 sc_out sc_logic 1 signal 7 } 
	{ capsule1_key_we0 sc_out sc_logic 1 signal 7 } 
	{ capsule1_key_d0 sc_out sc_lv 32 signal 7 } 
	{ capsule1_value_address0 sc_out sc_lv 5 signal 8 } 
	{ capsule1_value_ce0 sc_out sc_logic 1 signal 8 } 
	{ capsule1_value_we0 sc_out sc_logic 1 signal 8 } 
	{ capsule1_value_d0 sc_out sc_lv 32 signal 8 } 
	{ capsule1_value_q0 sc_in sc_lv 32 signal 8 } 
	{ capsule2_key_address0 sc_out sc_lv 5 signal 9 } 
	{ capsule2_key_ce0 sc_out sc_logic 1 signal 9 } 
	{ capsule2_key_we0 sc_out sc_logic 1 signal 9 } 
	{ capsule2_key_d0 sc_out sc_lv 32 signal 9 } 
	{ capsule2_value_address0 sc_out sc_lv 5 signal 10 } 
	{ capsule2_value_ce0 sc_out sc_logic 1 signal 10 } 
	{ capsule2_value_we0 sc_out sc_logic 1 signal 10 } 
	{ capsule2_value_d0 sc_out sc_lv 32 signal 10 } 
	{ capsule2_value_q0 sc_in sc_lv 32 signal 10 } 
	{ capsule3_key_address0 sc_out sc_lv 5 signal 11 } 
	{ capsule3_key_ce0 sc_out sc_logic 1 signal 11 } 
	{ capsule3_key_we0 sc_out sc_logic 1 signal 11 } 
	{ capsule3_key_d0 sc_out sc_lv 32 signal 11 } 
	{ capsule3_value_address0 sc_out sc_lv 5 signal 12 } 
	{ capsule3_value_ce0 sc_out sc_logic 1 signal 12 } 
	{ capsule3_value_we0 sc_out sc_logic 1 signal 12 } 
	{ capsule3_value_d0 sc_out sc_lv 32 signal 12 } 
	{ capsule3_value_q0 sc_in sc_lv 32 signal 12 } 
	{ llopparams_currentLOP sc_in sc_lv 32 signal 13 } 
	{ llopparams_upperlimit sc_in sc_lv 32 signal 14 } 
	{ travstate_i_kvs sc_in sc_lv 32 signal 15 } 
	{ travstate_end_kvs sc_in sc_lv 30 signal 16 } 
}
set NewPortList {[ 
	{ "name": "ap_clk", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "clock", "bundle":{"name": "ap_clk", "role": "default" }} , 
 	{ "name": "ap_rst", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "reset", "bundle":{"name": "ap_rst", "role": "default" }} , 
 	{ "name": "ap_start", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "start", "bundle":{"name": "ap_start", "role": "default" }} , 
 	{ "name": "ap_done", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "predone", "bundle":{"name": "ap_done", "role": "default" }} , 
 	{ "name": "ap_idle", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "done", "bundle":{"name": "ap_idle", "role": "default" }} , 
 	{ "name": "ap_ready", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "ready", "bundle":{"name": "ap_ready", "role": "default" }} , 
 	{ "name": "enable", "direction": "in", "datatype": "sc_lv", "bitwidth":1, "type": "signal", "bundle":{"name": "enable", "role": "default" }} , 
 	{ "name": "sourcebuffer0_V_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":9, "type": "signal", "bundle":{"name": "sourcebuffer0_V", "role": "address0" }} , 
 	{ "name": "sourcebuffer0_V_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "sourcebuffer0_V", "role": "ce0" }} , 
 	{ "name": "sourcebuffer0_V_q0", "direction": "in", "datatype": "sc_lv", "bitwidth":512, "type": "signal", "bundle":{"name": "sourcebuffer0_V", "role": "q0" }} , 
 	{ "name": "sourcebuffer1_V_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":9, "type": "signal", "bundle":{"name": "sourcebuffer1_V", "role": "address0" }} , 
 	{ "name": "sourcebuffer1_V_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "sourcebuffer1_V", "role": "ce0" }} , 
 	{ "name": "sourcebuffer1_V_q0", "direction": "in", "datatype": "sc_lv", "bitwidth":512, "type": "signal", "bundle":{"name": "sourcebuffer1_V", "role": "q0" }} , 
 	{ "name": "sourcebuffer2_V_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":9, "type": "signal", "bundle":{"name": "sourcebuffer2_V", "role": "address0" }} , 
 	{ "name": "sourcebuffer2_V_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "sourcebuffer2_V", "role": "ce0" }} , 
 	{ "name": "sourcebuffer2_V_q0", "direction": "in", "datatype": "sc_lv", "bitwidth":512, "type": "signal", "bundle":{"name": "sourcebuffer2_V", "role": "q0" }} , 
 	{ "name": "sourcebuffer3_V_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":9, "type": "signal", "bundle":{"name": "sourcebuffer3_V", "role": "address0" }} , 
 	{ "name": "sourcebuffer3_V_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "sourcebuffer3_V", "role": "ce0" }} , 
 	{ "name": "sourcebuffer3_V_q0", "direction": "in", "datatype": "sc_lv", "bitwidth":512, "type": "signal", "bundle":{"name": "sourcebuffer3_V", "role": "q0" }} , 
 	{ "name": "capsule0_key_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":5, "type": "signal", "bundle":{"name": "capsule0_key", "role": "address0" }} , 
 	{ "name": "capsule0_key_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "capsule0_key", "role": "ce0" }} , 
 	{ "name": "capsule0_key_we0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "capsule0_key", "role": "we0" }} , 
 	{ "name": "capsule0_key_d0", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "capsule0_key", "role": "d0" }} , 
 	{ "name": "capsule0_value_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":5, "type": "signal", "bundle":{"name": "capsule0_value", "role": "address0" }} , 
 	{ "name": "capsule0_value_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "capsule0_value", "role": "ce0" }} , 
 	{ "name": "capsule0_value_we0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "capsule0_value", "role": "we0" }} , 
 	{ "name": "capsule0_value_d0", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "capsule0_value", "role": "d0" }} , 
 	{ "name": "capsule0_value_q0", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "capsule0_value", "role": "q0" }} , 
 	{ "name": "capsule1_key_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":5, "type": "signal", "bundle":{"name": "capsule1_key", "role": "address0" }} , 
 	{ "name": "capsule1_key_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "capsule1_key", "role": "ce0" }} , 
 	{ "name": "capsule1_key_we0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "capsule1_key", "role": "we0" }} , 
 	{ "name": "capsule1_key_d0", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "capsule1_key", "role": "d0" }} , 
 	{ "name": "capsule1_value_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":5, "type": "signal", "bundle":{"name": "capsule1_value", "role": "address0" }} , 
 	{ "name": "capsule1_value_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "capsule1_value", "role": "ce0" }} , 
 	{ "name": "capsule1_value_we0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "capsule1_value", "role": "we0" }} , 
 	{ "name": "capsule1_value_d0", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "capsule1_value", "role": "d0" }} , 
 	{ "name": "capsule1_value_q0", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "capsule1_value", "role": "q0" }} , 
 	{ "name": "capsule2_key_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":5, "type": "signal", "bundle":{"name": "capsule2_key", "role": "address0" }} , 
 	{ "name": "capsule2_key_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "capsule2_key", "role": "ce0" }} , 
 	{ "name": "capsule2_key_we0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "capsule2_key", "role": "we0" }} , 
 	{ "name": "capsule2_key_d0", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "capsule2_key", "role": "d0" }} , 
 	{ "name": "capsule2_value_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":5, "type": "signal", "bundle":{"name": "capsule2_value", "role": "address0" }} , 
 	{ "name": "capsule2_value_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "capsule2_value", "role": "ce0" }} , 
 	{ "name": "capsule2_value_we0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "capsule2_value", "role": "we0" }} , 
 	{ "name": "capsule2_value_d0", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "capsule2_value", "role": "d0" }} , 
 	{ "name": "capsule2_value_q0", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "capsule2_value", "role": "q0" }} , 
 	{ "name": "capsule3_key_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":5, "type": "signal", "bundle":{"name": "capsule3_key", "role": "address0" }} , 
 	{ "name": "capsule3_key_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "capsule3_key", "role": "ce0" }} , 
 	{ "name": "capsule3_key_we0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "capsule3_key", "role": "we0" }} , 
 	{ "name": "capsule3_key_d0", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "capsule3_key", "role": "d0" }} , 
 	{ "name": "capsule3_value_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":5, "type": "signal", "bundle":{"name": "capsule3_value", "role": "address0" }} , 
 	{ "name": "capsule3_value_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "capsule3_value", "role": "ce0" }} , 
 	{ "name": "capsule3_value_we0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "capsule3_value", "role": "we0" }} , 
 	{ "name": "capsule3_value_d0", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "capsule3_value", "role": "d0" }} , 
 	{ "name": "capsule3_value_q0", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "capsule3_value", "role": "q0" }} , 
 	{ "name": "llopparams_currentLOP", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "llopparams_currentLOP", "role": "default" }} , 
 	{ "name": "llopparams_upperlimit", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "llopparams_upperlimit", "role": "default" }} , 
 	{ "name": "travstate_i_kvs", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "travstate_i_kvs", "role": "default" }} , 
 	{ "name": "travstate_end_kvs", "direction": "in", "datatype": "sc_lv", "bitwidth":30, "type": "signal", "bundle":{"name": "travstate_end_kvs", "role": "default" }}  ]}

set RtlHierarchyInfo {[
	{"ID" : "0", "Level" : "0", "Path" : "`AUTOTB_DUT_INST", "Parent" : "", "Child" : ["1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13", "14", "15", "16", "17", "18", "19", "20", "21", "22", "23", "24", "25", "26", "27", "28", "29", "30", "31", "32", "33", "34", "35", "36", "37", "38", "39", "40", "41", "42", "43", "44", "45", "46", "47", "48"],
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
	{"ID" : "1", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.sizes00_U", "Parent" : "0"},
	{"ID" : "2", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.sizes01_U", "Parent" : "0"},
	{"ID" : "3", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.sizes02_U", "Parent" : "0"},
	{"ID" : "4", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.sizes03_U", "Parent" : "0"},
	{"ID" : "5", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.sizes04_U", "Parent" : "0"},
	{"ID" : "6", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.sizes05_U", "Parent" : "0"},
	{"ID" : "7", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.sizes06_U", "Parent" : "0"},
	{"ID" : "8", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.sizes07_U", "Parent" : "0"},
	{"ID" : "9", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.sizes10_U", "Parent" : "0"},
	{"ID" : "10", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.sizes11_U", "Parent" : "0"},
	{"ID" : "11", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.sizes12_U", "Parent" : "0"},
	{"ID" : "12", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.sizes13_U", "Parent" : "0"},
	{"ID" : "13", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.sizes14_U", "Parent" : "0"},
	{"ID" : "14", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.sizes15_U", "Parent" : "0"},
	{"ID" : "15", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.sizes16_U", "Parent" : "0"},
	{"ID" : "16", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.sizes17_U", "Parent" : "0"},
	{"ID" : "17", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.sizes20_U", "Parent" : "0"},
	{"ID" : "18", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.sizes21_U", "Parent" : "0"},
	{"ID" : "19", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.sizes22_U", "Parent" : "0"},
	{"ID" : "20", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.sizes23_U", "Parent" : "0"},
	{"ID" : "21", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.sizes24_U", "Parent" : "0"},
	{"ID" : "22", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.sizes25_U", "Parent" : "0"},
	{"ID" : "23", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.sizes26_U", "Parent" : "0"},
	{"ID" : "24", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.sizes27_U", "Parent" : "0"},
	{"ID" : "25", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.sizes30_U", "Parent" : "0"},
	{"ID" : "26", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.sizes31_U", "Parent" : "0"},
	{"ID" : "27", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.sizes32_U", "Parent" : "0"},
	{"ID" : "28", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.sizes33_U", "Parent" : "0"},
	{"ID" : "29", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.sizes34_U", "Parent" : "0"},
	{"ID" : "30", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.sizes35_U", "Parent" : "0"},
	{"ID" : "31", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.sizes36_U", "Parent" : "0"},
	{"ID" : "32", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.sizes37_U", "Parent" : "0"},
	{"ID" : "33", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.grp_getpartition_fu_1471", "Parent" : "0",
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
	{"ID" : "34", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.grp_getpartition_fu_1478", "Parent" : "0",
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
	{"ID" : "35", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.grp_getpartition_fu_1485", "Parent" : "0",
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
	{"ID" : "36", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.grp_getpartition_fu_1492", "Parent" : "0",
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
	{"ID" : "37", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.grp_getpartition_fu_1499", "Parent" : "0",
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
	{"ID" : "38", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.grp_getpartition_fu_1506", "Parent" : "0",
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
	{"ID" : "39", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.grp_getpartition_fu_1513", "Parent" : "0",
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
	{"ID" : "40", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.grp_getpartition_fu_1520", "Parent" : "0",
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
	{"ID" : "41", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.grp_getpartition_fu_1527", "Parent" : "0",
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
	{"ID" : "42", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.grp_getpartition_fu_1534", "Parent" : "0",
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
	{"ID" : "43", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.grp_getpartition_fu_1541", "Parent" : "0",
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
	{"ID" : "44", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.grp_getpartition_fu_1548", "Parent" : "0",
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
	{"ID" : "45", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.grp_getpartition_fu_1555", "Parent" : "0",
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
	{"ID" : "46", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.grp_getpartition_fu_1562", "Parent" : "0",
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
	{"ID" : "47", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.grp_getpartition_fu_1569", "Parent" : "0",
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
	{"ID" : "48", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.grp_getpartition_fu_1576", "Parent" : "0",
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
			{"Name" : "upperlimit", "Type" : "None", "Direction" : "I"}]}]}


set ArgLastReadFirstWriteLatency {
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
		upperlimit {Type I LastRead 0 FirstWrite -1}}}

set hasDtUnsupportedChannel 0

set PerformanceInfo {[
	{"Name" : "Latency", "Min" : "1", "Max" : "1126"}
	, {"Name" : "Interval", "Min" : "1", "Max" : "1126"}
]}

set PipelineEnableSignalInfo {[
	{"Pipeline" : "0", "EnableSignal" : "ap_enable_pp0"}
]}

set Spec2ImplPortList { 
	enable { ap_none {  { enable in_data 0 1 } } }
	sourcebuffer0_V { ap_memory {  { sourcebuffer0_V_address0 mem_address 1 9 }  { sourcebuffer0_V_ce0 mem_ce 1 1 }  { sourcebuffer0_V_q0 mem_dout 0 512 } } }
	sourcebuffer1_V { ap_memory {  { sourcebuffer1_V_address0 mem_address 1 9 }  { sourcebuffer1_V_ce0 mem_ce 1 1 }  { sourcebuffer1_V_q0 mem_dout 0 512 } } }
	sourcebuffer2_V { ap_memory {  { sourcebuffer2_V_address0 mem_address 1 9 }  { sourcebuffer2_V_ce0 mem_ce 1 1 }  { sourcebuffer2_V_q0 mem_dout 0 512 } } }
	sourcebuffer3_V { ap_memory {  { sourcebuffer3_V_address0 mem_address 1 9 }  { sourcebuffer3_V_ce0 mem_ce 1 1 }  { sourcebuffer3_V_q0 mem_dout 0 512 } } }
	capsule0_key { ap_memory {  { capsule0_key_address0 mem_address 1 5 }  { capsule0_key_ce0 mem_ce 1 1 }  { capsule0_key_we0 mem_we 1 1 }  { capsule0_key_d0 mem_din 1 32 } } }
	capsule0_value { ap_memory {  { capsule0_value_address0 mem_address 1 5 }  { capsule0_value_ce0 mem_ce 1 1 }  { capsule0_value_we0 mem_we 1 1 }  { capsule0_value_d0 mem_din 1 32 }  { capsule0_value_q0 mem_dout 0 32 } } }
	capsule1_key { ap_memory {  { capsule1_key_address0 mem_address 1 5 }  { capsule1_key_ce0 mem_ce 1 1 }  { capsule1_key_we0 mem_we 1 1 }  { capsule1_key_d0 mem_din 1 32 } } }
	capsule1_value { ap_memory {  { capsule1_value_address0 mem_address 1 5 }  { capsule1_value_ce0 mem_ce 1 1 }  { capsule1_value_we0 mem_we 1 1 }  { capsule1_value_d0 mem_din 1 32 }  { capsule1_value_q0 mem_dout 0 32 } } }
	capsule2_key { ap_memory {  { capsule2_key_address0 mem_address 1 5 }  { capsule2_key_ce0 mem_ce 1 1 }  { capsule2_key_we0 mem_we 1 1 }  { capsule2_key_d0 mem_din 1 32 } } }
	capsule2_value { ap_memory {  { capsule2_value_address0 mem_address 1 5 }  { capsule2_value_ce0 mem_ce 1 1 }  { capsule2_value_we0 mem_we 1 1 }  { capsule2_value_d0 mem_din 1 32 }  { capsule2_value_q0 mem_dout 0 32 } } }
	capsule3_key { ap_memory {  { capsule3_key_address0 mem_address 1 5 }  { capsule3_key_ce0 mem_ce 1 1 }  { capsule3_key_we0 mem_we 1 1 }  { capsule3_key_d0 mem_din 1 32 } } }
	capsule3_value { ap_memory {  { capsule3_value_address0 mem_address 1 5 }  { capsule3_value_ce0 mem_ce 1 1 }  { capsule3_value_we0 mem_we 1 1 }  { capsule3_value_d0 mem_din 1 32 }  { capsule3_value_q0 mem_dout 0 32 } } }
	llopparams_currentLOP { ap_none {  { llopparams_currentLOP in_data 0 32 } } }
	llopparams_upperlimit { ap_none {  { llopparams_upperlimit in_data 0 32 } } }
	travstate_i_kvs { ap_none {  { travstate_i_kvs in_data 0 32 } } }
	travstate_end_kvs { ap_none {  { travstate_end_kvs in_data 0 30 } } }
}
