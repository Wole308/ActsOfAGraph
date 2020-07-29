set moduleName reducepartitions0
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
set C_modelName {reducepartitions0}
set C_modelType { void 0 }
set C_modelArgList {
	{ enable int 1 regular  }
	{ sourcebuffer0_V int 512 regular {array 512 { 1 3 } 1 1 }  }
	{ sourcebuffer1_V int 512 regular {array 512 { 1 3 } 1 1 }  }
	{ sourcebuffer2_V int 512 regular {array 512 { 1 3 } 1 1 }  }
	{ sourcebuffer3_V int 512 regular {array 512 { 1 3 } 1 1 }  }
	{ destbuffer0_V int 512 regular {array 2112 { 2 3 } 1 1 }  }
	{ destbuffer1_V int 512 regular {array 2112 { 2 3 } 1 1 }  }
	{ destbuffer2_V int 512 regular {array 2112 { 2 3 } 1 1 }  }
	{ destbuffer3_V int 512 regular {array 2112 { 2 3 } 1 1 }  }
	{ globalparams_vbegin int 32 regular  }
}
set C_modelArgMapList {[ 
	{ "Name" : "enable", "interface" : "wire", "bitwidth" : 1, "direction" : "READONLY"} , 
 	{ "Name" : "sourcebuffer0_V", "interface" : "memory", "bitwidth" : 512, "direction" : "READONLY"} , 
 	{ "Name" : "sourcebuffer1_V", "interface" : "memory", "bitwidth" : 512, "direction" : "READONLY"} , 
 	{ "Name" : "sourcebuffer2_V", "interface" : "memory", "bitwidth" : 512, "direction" : "READONLY"} , 
 	{ "Name" : "sourcebuffer3_V", "interface" : "memory", "bitwidth" : 512, "direction" : "READONLY"} , 
 	{ "Name" : "destbuffer0_V", "interface" : "memory", "bitwidth" : 512, "direction" : "READWRITE"} , 
 	{ "Name" : "destbuffer1_V", "interface" : "memory", "bitwidth" : 512, "direction" : "READWRITE"} , 
 	{ "Name" : "destbuffer2_V", "interface" : "memory", "bitwidth" : 512, "direction" : "READWRITE"} , 
 	{ "Name" : "destbuffer3_V", "interface" : "memory", "bitwidth" : 512, "direction" : "READWRITE"} , 
 	{ "Name" : "globalparams_vbegin", "interface" : "wire", "bitwidth" : 32, "direction" : "READONLY"} ]}
# RTL Port declarations: 
set portNum 40
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
	{ destbuffer0_V_address0 sc_out sc_lv 12 signal 5 } 
	{ destbuffer0_V_ce0 sc_out sc_logic 1 signal 5 } 
	{ destbuffer0_V_we0 sc_out sc_lv 64 signal 5 } 
	{ destbuffer0_V_d0 sc_out sc_lv 512 signal 5 } 
	{ destbuffer0_V_q0 sc_in sc_lv 512 signal 5 } 
	{ destbuffer1_V_address0 sc_out sc_lv 12 signal 6 } 
	{ destbuffer1_V_ce0 sc_out sc_logic 1 signal 6 } 
	{ destbuffer1_V_we0 sc_out sc_lv 64 signal 6 } 
	{ destbuffer1_V_d0 sc_out sc_lv 512 signal 6 } 
	{ destbuffer1_V_q0 sc_in sc_lv 512 signal 6 } 
	{ destbuffer2_V_address0 sc_out sc_lv 12 signal 7 } 
	{ destbuffer2_V_ce0 sc_out sc_logic 1 signal 7 } 
	{ destbuffer2_V_we0 sc_out sc_lv 64 signal 7 } 
	{ destbuffer2_V_d0 sc_out sc_lv 512 signal 7 } 
	{ destbuffer2_V_q0 sc_in sc_lv 512 signal 7 } 
	{ destbuffer3_V_address0 sc_out sc_lv 12 signal 8 } 
	{ destbuffer3_V_ce0 sc_out sc_logic 1 signal 8 } 
	{ destbuffer3_V_we0 sc_out sc_lv 64 signal 8 } 
	{ destbuffer3_V_d0 sc_out sc_lv 512 signal 8 } 
	{ destbuffer3_V_q0 sc_in sc_lv 512 signal 8 } 
	{ globalparams_vbegin sc_in sc_lv 32 signal 9 } 
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
 	{ "name": "destbuffer0_V_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":12, "type": "signal", "bundle":{"name": "destbuffer0_V", "role": "address0" }} , 
 	{ "name": "destbuffer0_V_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "destbuffer0_V", "role": "ce0" }} , 
 	{ "name": "destbuffer0_V_we0", "direction": "out", "datatype": "sc_lv", "bitwidth":64, "type": "signal", "bundle":{"name": "destbuffer0_V", "role": "we0" }} , 
 	{ "name": "destbuffer0_V_d0", "direction": "out", "datatype": "sc_lv", "bitwidth":512, "type": "signal", "bundle":{"name": "destbuffer0_V", "role": "d0" }} , 
 	{ "name": "destbuffer0_V_q0", "direction": "in", "datatype": "sc_lv", "bitwidth":512, "type": "signal", "bundle":{"name": "destbuffer0_V", "role": "q0" }} , 
 	{ "name": "destbuffer1_V_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":12, "type": "signal", "bundle":{"name": "destbuffer1_V", "role": "address0" }} , 
 	{ "name": "destbuffer1_V_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "destbuffer1_V", "role": "ce0" }} , 
 	{ "name": "destbuffer1_V_we0", "direction": "out", "datatype": "sc_lv", "bitwidth":64, "type": "signal", "bundle":{"name": "destbuffer1_V", "role": "we0" }} , 
 	{ "name": "destbuffer1_V_d0", "direction": "out", "datatype": "sc_lv", "bitwidth":512, "type": "signal", "bundle":{"name": "destbuffer1_V", "role": "d0" }} , 
 	{ "name": "destbuffer1_V_q0", "direction": "in", "datatype": "sc_lv", "bitwidth":512, "type": "signal", "bundle":{"name": "destbuffer1_V", "role": "q0" }} , 
 	{ "name": "destbuffer2_V_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":12, "type": "signal", "bundle":{"name": "destbuffer2_V", "role": "address0" }} , 
 	{ "name": "destbuffer2_V_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "destbuffer2_V", "role": "ce0" }} , 
 	{ "name": "destbuffer2_V_we0", "direction": "out", "datatype": "sc_lv", "bitwidth":64, "type": "signal", "bundle":{"name": "destbuffer2_V", "role": "we0" }} , 
 	{ "name": "destbuffer2_V_d0", "direction": "out", "datatype": "sc_lv", "bitwidth":512, "type": "signal", "bundle":{"name": "destbuffer2_V", "role": "d0" }} , 
 	{ "name": "destbuffer2_V_q0", "direction": "in", "datatype": "sc_lv", "bitwidth":512, "type": "signal", "bundle":{"name": "destbuffer2_V", "role": "q0" }} , 
 	{ "name": "destbuffer3_V_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":12, "type": "signal", "bundle":{"name": "destbuffer3_V", "role": "address0" }} , 
 	{ "name": "destbuffer3_V_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "destbuffer3_V", "role": "ce0" }} , 
 	{ "name": "destbuffer3_V_we0", "direction": "out", "datatype": "sc_lv", "bitwidth":64, "type": "signal", "bundle":{"name": "destbuffer3_V", "role": "we0" }} , 
 	{ "name": "destbuffer3_V_d0", "direction": "out", "datatype": "sc_lv", "bitwidth":512, "type": "signal", "bundle":{"name": "destbuffer3_V", "role": "d0" }} , 
 	{ "name": "destbuffer3_V_q0", "direction": "in", "datatype": "sc_lv", "bitwidth":512, "type": "signal", "bundle":{"name": "destbuffer3_V", "role": "q0" }} , 
 	{ "name": "globalparams_vbegin", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "globalparams_vbegin", "role": "default" }}  ]}

set RtlHierarchyInfo {[
	{"ID" : "0", "Level" : "0", "Path" : "`AUTOTB_DUT_INST", "Parent" : "", "Child" : ["1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13", "14", "15", "16", "17", "18", "19", "20", "21", "22", "23", "24", "25", "26", "27", "28", "29", "30", "31", "32", "33", "34", "35", "36", "37", "38", "39", "40", "41", "42", "43", "44", "45", "46", "47", "48", "49", "50", "51", "52", "53", "54", "55", "56", "57", "58", "59", "60", "61", "62", "63", "64", "65", "66", "67", "68", "69", "70", "71", "72", "73", "74", "75", "76", "77", "78", "79", "80", "81", "82", "83", "84", "85", "86", "87", "88", "89", "90", "91", "92", "93", "94", "95", "96", "97", "98", "99", "100", "101", "102", "103", "104", "105", "106", "107", "108", "109", "110", "111", "112", "113", "114", "115", "116", "117", "118", "119", "120", "121", "122", "123", "124", "125", "126", "127", "128"],
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
	{"ID" : "1", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.destbuffer00_key_U", "Parent" : "0"},
	{"ID" : "2", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.destbuffer00_value_U", "Parent" : "0"},
	{"ID" : "3", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.destbuffer01_key_U", "Parent" : "0"},
	{"ID" : "4", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.destbuffer01_value_U", "Parent" : "0"},
	{"ID" : "5", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.destbuffer02_key_U", "Parent" : "0"},
	{"ID" : "6", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.destbuffer02_value_U", "Parent" : "0"},
	{"ID" : "7", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.destbuffer03_key_U", "Parent" : "0"},
	{"ID" : "8", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.destbuffer03_value_U", "Parent" : "0"},
	{"ID" : "9", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.destbuffer04_key_U", "Parent" : "0"},
	{"ID" : "10", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.destbuffer04_value_U", "Parent" : "0"},
	{"ID" : "11", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.destbuffer05_key_U", "Parent" : "0"},
	{"ID" : "12", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.destbuffer05_value_U", "Parent" : "0"},
	{"ID" : "13", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.destbuffer06_key_U", "Parent" : "0"},
	{"ID" : "14", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.destbuffer06_value_U", "Parent" : "0"},
	{"ID" : "15", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.destbuffer07_key_U", "Parent" : "0"},
	{"ID" : "16", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.destbuffer07_value_U", "Parent" : "0"},
	{"ID" : "17", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.destbuffer10_key_U", "Parent" : "0"},
	{"ID" : "18", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.destbuffer10_value_U", "Parent" : "0"},
	{"ID" : "19", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.destbuffer11_key_U", "Parent" : "0"},
	{"ID" : "20", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.destbuffer11_value_U", "Parent" : "0"},
	{"ID" : "21", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.destbuffer12_key_U", "Parent" : "0"},
	{"ID" : "22", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.destbuffer12_value_U", "Parent" : "0"},
	{"ID" : "23", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.destbuffer13_key_U", "Parent" : "0"},
	{"ID" : "24", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.destbuffer13_value_U", "Parent" : "0"},
	{"ID" : "25", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.destbuffer14_key_U", "Parent" : "0"},
	{"ID" : "26", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.destbuffer14_value_U", "Parent" : "0"},
	{"ID" : "27", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.destbuffer15_key_U", "Parent" : "0"},
	{"ID" : "28", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.destbuffer15_value_U", "Parent" : "0"},
	{"ID" : "29", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.destbuffer16_key_U", "Parent" : "0"},
	{"ID" : "30", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.destbuffer16_value_U", "Parent" : "0"},
	{"ID" : "31", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.destbuffer17_key_U", "Parent" : "0"},
	{"ID" : "32", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.destbuffer17_value_U", "Parent" : "0"},
	{"ID" : "33", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.destbuffer20_key_U", "Parent" : "0"},
	{"ID" : "34", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.destbuffer20_value_U", "Parent" : "0"},
	{"ID" : "35", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.destbuffer21_key_U", "Parent" : "0"},
	{"ID" : "36", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.destbuffer21_value_U", "Parent" : "0"},
	{"ID" : "37", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.destbuffer22_key_U", "Parent" : "0"},
	{"ID" : "38", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.destbuffer22_value_U", "Parent" : "0"},
	{"ID" : "39", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.destbuffer23_key_U", "Parent" : "0"},
	{"ID" : "40", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.destbuffer23_value_U", "Parent" : "0"},
	{"ID" : "41", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.destbuffer24_key_U", "Parent" : "0"},
	{"ID" : "42", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.destbuffer24_value_U", "Parent" : "0"},
	{"ID" : "43", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.destbuffer25_key_U", "Parent" : "0"},
	{"ID" : "44", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.destbuffer25_value_U", "Parent" : "0"},
	{"ID" : "45", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.destbuffer26_key_U", "Parent" : "0"},
	{"ID" : "46", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.destbuffer26_value_U", "Parent" : "0"},
	{"ID" : "47", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.destbuffer27_key_U", "Parent" : "0"},
	{"ID" : "48", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.destbuffer27_value_U", "Parent" : "0"},
	{"ID" : "49", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.destbuffer30_key_U", "Parent" : "0"},
	{"ID" : "50", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.destbuffer30_value_U", "Parent" : "0"},
	{"ID" : "51", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.destbuffer31_key_U", "Parent" : "0"},
	{"ID" : "52", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.destbuffer31_value_U", "Parent" : "0"},
	{"ID" : "53", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.destbuffer32_key_U", "Parent" : "0"},
	{"ID" : "54", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.destbuffer32_value_U", "Parent" : "0"},
	{"ID" : "55", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.destbuffer33_key_U", "Parent" : "0"},
	{"ID" : "56", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.destbuffer33_value_U", "Parent" : "0"},
	{"ID" : "57", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.destbuffer34_key_U", "Parent" : "0"},
	{"ID" : "58", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.destbuffer34_value_U", "Parent" : "0"},
	{"ID" : "59", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.destbuffer35_key_U", "Parent" : "0"},
	{"ID" : "60", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.destbuffer35_value_U", "Parent" : "0"},
	{"ID" : "61", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.destbuffer36_key_U", "Parent" : "0"},
	{"ID" : "62", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.destbuffer36_value_U", "Parent" : "0"},
	{"ID" : "63", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.destbuffer37_key_U", "Parent" : "0"},
	{"ID" : "64", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.destbuffer37_value_U", "Parent" : "0"},
	{"ID" : "65", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.Vtemp0_0_key_U", "Parent" : "0"},
	{"ID" : "66", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.Vtemp0_1_key_U", "Parent" : "0"},
	{"ID" : "67", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.Vtemp0_2_key_U", "Parent" : "0"},
	{"ID" : "68", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.Vtemp0_3_key_U", "Parent" : "0"},
	{"ID" : "69", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.Vtemp0_4_key_U", "Parent" : "0"},
	{"ID" : "70", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.Vtemp0_5_key_U", "Parent" : "0"},
	{"ID" : "71", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.Vtemp0_6_key_U", "Parent" : "0"},
	{"ID" : "72", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.Vtemp0_7_key_U", "Parent" : "0"},
	{"ID" : "73", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.Vtemp0_0_value_U", "Parent" : "0"},
	{"ID" : "74", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.Vtemp0_1_value_U", "Parent" : "0"},
	{"ID" : "75", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.Vtemp0_2_value_U", "Parent" : "0"},
	{"ID" : "76", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.Vtemp0_3_value_U", "Parent" : "0"},
	{"ID" : "77", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.Vtemp0_4_value_U", "Parent" : "0"},
	{"ID" : "78", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.Vtemp0_5_value_U", "Parent" : "0"},
	{"ID" : "79", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.Vtemp0_6_value_U", "Parent" : "0"},
	{"ID" : "80", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.Vtemp0_7_value_U", "Parent" : "0"},
	{"ID" : "81", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.Vtemp1_0_key_U", "Parent" : "0"},
	{"ID" : "82", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.Vtemp1_1_key_U", "Parent" : "0"},
	{"ID" : "83", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.Vtemp1_2_key_U", "Parent" : "0"},
	{"ID" : "84", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.Vtemp1_3_key_U", "Parent" : "0"},
	{"ID" : "85", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.Vtemp1_4_key_U", "Parent" : "0"},
	{"ID" : "86", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.Vtemp1_5_key_U", "Parent" : "0"},
	{"ID" : "87", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.Vtemp1_6_key_U", "Parent" : "0"},
	{"ID" : "88", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.Vtemp1_7_key_U", "Parent" : "0"},
	{"ID" : "89", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.Vtemp1_0_value_U", "Parent" : "0"},
	{"ID" : "90", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.Vtemp1_1_value_U", "Parent" : "0"},
	{"ID" : "91", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.Vtemp1_2_value_U", "Parent" : "0"},
	{"ID" : "92", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.Vtemp1_3_value_U", "Parent" : "0"},
	{"ID" : "93", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.Vtemp1_4_value_U", "Parent" : "0"},
	{"ID" : "94", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.Vtemp1_5_value_U", "Parent" : "0"},
	{"ID" : "95", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.Vtemp1_6_value_U", "Parent" : "0"},
	{"ID" : "96", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.Vtemp1_7_value_U", "Parent" : "0"},
	{"ID" : "97", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.Vtemp2_0_key_U", "Parent" : "0"},
	{"ID" : "98", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.Vtemp2_1_key_U", "Parent" : "0"},
	{"ID" : "99", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.Vtemp2_2_key_U", "Parent" : "0"},
	{"ID" : "100", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.Vtemp2_3_key_U", "Parent" : "0"},
	{"ID" : "101", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.Vtemp2_4_key_U", "Parent" : "0"},
	{"ID" : "102", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.Vtemp2_5_key_U", "Parent" : "0"},
	{"ID" : "103", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.Vtemp2_6_key_U", "Parent" : "0"},
	{"ID" : "104", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.Vtemp2_7_key_U", "Parent" : "0"},
	{"ID" : "105", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.Vtemp2_0_value_U", "Parent" : "0"},
	{"ID" : "106", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.Vtemp2_1_value_U", "Parent" : "0"},
	{"ID" : "107", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.Vtemp2_2_value_U", "Parent" : "0"},
	{"ID" : "108", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.Vtemp2_3_value_U", "Parent" : "0"},
	{"ID" : "109", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.Vtemp2_4_value_U", "Parent" : "0"},
	{"ID" : "110", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.Vtemp2_5_value_U", "Parent" : "0"},
	{"ID" : "111", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.Vtemp2_6_value_U", "Parent" : "0"},
	{"ID" : "112", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.Vtemp2_7_value_U", "Parent" : "0"},
	{"ID" : "113", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.Vtemp3_0_key_U", "Parent" : "0"},
	{"ID" : "114", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.Vtemp3_1_key_U", "Parent" : "0"},
	{"ID" : "115", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.Vtemp3_2_key_U", "Parent" : "0"},
	{"ID" : "116", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.Vtemp3_3_key_U", "Parent" : "0"},
	{"ID" : "117", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.Vtemp3_4_key_U", "Parent" : "0"},
	{"ID" : "118", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.Vtemp3_5_key_U", "Parent" : "0"},
	{"ID" : "119", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.Vtemp3_6_key_U", "Parent" : "0"},
	{"ID" : "120", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.Vtemp3_7_key_U", "Parent" : "0"},
	{"ID" : "121", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.Vtemp3_0_value_U", "Parent" : "0"},
	{"ID" : "122", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.Vtemp3_1_value_U", "Parent" : "0"},
	{"ID" : "123", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.Vtemp3_2_value_U", "Parent" : "0"},
	{"ID" : "124", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.Vtemp3_3_value_U", "Parent" : "0"},
	{"ID" : "125", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.Vtemp3_4_value_U", "Parent" : "0"},
	{"ID" : "126", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.Vtemp3_5_value_U", "Parent" : "0"},
	{"ID" : "127", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.Vtemp3_6_value_U", "Parent" : "0"},
	{"ID" : "128", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.Vtemp3_7_value_U", "Parent" : "0"}]}


set ArgLastReadFirstWriteLatency {
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
		globalparams_vbegin {Type I LastRead 0 FirstWrite -1}}}

set hasDtUnsupportedChannel 0

set PerformanceInfo {[
	{"Name" : "Latency", "Min" : "1", "Max" : "6181"}
	, {"Name" : "Interval", "Min" : "1", "Max" : "6181"}
]}

set PipelineEnableSignalInfo {[
	{"Pipeline" : "0", "EnableSignal" : "ap_enable_pp0"}
	{"Pipeline" : "1", "EnableSignal" : "ap_enable_pp1"}
	{"Pipeline" : "3", "EnableSignal" : "ap_enable_pp3"}
]}

set Spec2ImplPortList { 
	enable { ap_none {  { enable in_data 0 1 } } }
	sourcebuffer0_V { ap_memory {  { sourcebuffer0_V_address0 mem_address 1 9 }  { sourcebuffer0_V_ce0 mem_ce 1 1 }  { sourcebuffer0_V_q0 mem_dout 0 512 } } }
	sourcebuffer1_V { ap_memory {  { sourcebuffer1_V_address0 mem_address 1 9 }  { sourcebuffer1_V_ce0 mem_ce 1 1 }  { sourcebuffer1_V_q0 mem_dout 0 512 } } }
	sourcebuffer2_V { ap_memory {  { sourcebuffer2_V_address0 mem_address 1 9 }  { sourcebuffer2_V_ce0 mem_ce 1 1 }  { sourcebuffer2_V_q0 mem_dout 0 512 } } }
	sourcebuffer3_V { ap_memory {  { sourcebuffer3_V_address0 mem_address 1 9 }  { sourcebuffer3_V_ce0 mem_ce 1 1 }  { sourcebuffer3_V_q0 mem_dout 0 512 } } }
	destbuffer0_V { ap_memory {  { destbuffer0_V_address0 mem_address 1 12 }  { destbuffer0_V_ce0 mem_ce 1 1 }  { destbuffer0_V_we0 mem_we 1 64 }  { destbuffer0_V_d0 mem_din 1 512 }  { destbuffer0_V_q0 mem_dout 0 512 } } }
	destbuffer1_V { ap_memory {  { destbuffer1_V_address0 mem_address 1 12 }  { destbuffer1_V_ce0 mem_ce 1 1 }  { destbuffer1_V_we0 mem_we 1 64 }  { destbuffer1_V_d0 mem_din 1 512 }  { destbuffer1_V_q0 mem_dout 0 512 } } }
	destbuffer2_V { ap_memory {  { destbuffer2_V_address0 mem_address 1 12 }  { destbuffer2_V_ce0 mem_ce 1 1 }  { destbuffer2_V_we0 mem_we 1 64 }  { destbuffer2_V_d0 mem_din 1 512 }  { destbuffer2_V_q0 mem_dout 0 512 } } }
	destbuffer3_V { ap_memory {  { destbuffer3_V_address0 mem_address 1 12 }  { destbuffer3_V_ce0 mem_ce 1 1 }  { destbuffer3_V_we0 mem_we 1 64 }  { destbuffer3_V_d0 mem_din 1 512 }  { destbuffer3_V_q0 mem_dout 0 512 } } }
	globalparams_vbegin { ap_none {  { globalparams_vbegin in_data 0 32 } } }
}
set moduleName reducepartitions0
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
set C_modelName {reducepartitions0}
set C_modelType { void 0 }
set C_modelArgList {
	{ enable int 1 regular  }
	{ sourcebuffer0_V int 512 regular {array 512 { 1 3 } 1 1 }  }
	{ sourcebuffer1_V int 512 regular {array 512 { 1 3 } 1 1 }  }
	{ sourcebuffer2_V int 512 regular {array 512 { 1 3 } 1 1 }  }
	{ sourcebuffer3_V int 512 regular {array 512 { 1 3 } 1 1 }  }
	{ destbuffer0_V int 512 regular {array 2112 { 2 3 } 1 1 }  }
	{ destbuffer1_V int 512 regular {array 2112 { 2 3 } 1 1 }  }
	{ destbuffer2_V int 512 regular {array 2112 { 2 3 } 1 1 }  }
	{ destbuffer3_V int 512 regular {array 2112 { 2 3 } 1 1 }  }
	{ globalparams_vbegin int 32 regular  }
}
set C_modelArgMapList {[ 
	{ "Name" : "enable", "interface" : "wire", "bitwidth" : 1, "direction" : "READONLY"} , 
 	{ "Name" : "sourcebuffer0_V", "interface" : "memory", "bitwidth" : 512, "direction" : "READONLY"} , 
 	{ "Name" : "sourcebuffer1_V", "interface" : "memory", "bitwidth" : 512, "direction" : "READONLY"} , 
 	{ "Name" : "sourcebuffer2_V", "interface" : "memory", "bitwidth" : 512, "direction" : "READONLY"} , 
 	{ "Name" : "sourcebuffer3_V", "interface" : "memory", "bitwidth" : 512, "direction" : "READONLY"} , 
 	{ "Name" : "destbuffer0_V", "interface" : "memory", "bitwidth" : 512, "direction" : "READWRITE"} , 
 	{ "Name" : "destbuffer1_V", "interface" : "memory", "bitwidth" : 512, "direction" : "READWRITE"} , 
 	{ "Name" : "destbuffer2_V", "interface" : "memory", "bitwidth" : 512, "direction" : "READWRITE"} , 
 	{ "Name" : "destbuffer3_V", "interface" : "memory", "bitwidth" : 512, "direction" : "READWRITE"} , 
 	{ "Name" : "globalparams_vbegin", "interface" : "wire", "bitwidth" : 32, "direction" : "READONLY"} ]}
# RTL Port declarations: 
set portNum 40
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
	{ destbuffer0_V_address0 sc_out sc_lv 12 signal 5 } 
	{ destbuffer0_V_ce0 sc_out sc_logic 1 signal 5 } 
	{ destbuffer0_V_we0 sc_out sc_lv 64 signal 5 } 
	{ destbuffer0_V_d0 sc_out sc_lv 512 signal 5 } 
	{ destbuffer0_V_q0 sc_in sc_lv 512 signal 5 } 
	{ destbuffer1_V_address0 sc_out sc_lv 12 signal 6 } 
	{ destbuffer1_V_ce0 sc_out sc_logic 1 signal 6 } 
	{ destbuffer1_V_we0 sc_out sc_lv 64 signal 6 } 
	{ destbuffer1_V_d0 sc_out sc_lv 512 signal 6 } 
	{ destbuffer1_V_q0 sc_in sc_lv 512 signal 6 } 
	{ destbuffer2_V_address0 sc_out sc_lv 12 signal 7 } 
	{ destbuffer2_V_ce0 sc_out sc_logic 1 signal 7 } 
	{ destbuffer2_V_we0 sc_out sc_lv 64 signal 7 } 
	{ destbuffer2_V_d0 sc_out sc_lv 512 signal 7 } 
	{ destbuffer2_V_q0 sc_in sc_lv 512 signal 7 } 
	{ destbuffer3_V_address0 sc_out sc_lv 12 signal 8 } 
	{ destbuffer3_V_ce0 sc_out sc_logic 1 signal 8 } 
	{ destbuffer3_V_we0 sc_out sc_lv 64 signal 8 } 
	{ destbuffer3_V_d0 sc_out sc_lv 512 signal 8 } 
	{ destbuffer3_V_q0 sc_in sc_lv 512 signal 8 } 
	{ globalparams_vbegin sc_in sc_lv 32 signal 9 } 
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
 	{ "name": "destbuffer0_V_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":12, "type": "signal", "bundle":{"name": "destbuffer0_V", "role": "address0" }} , 
 	{ "name": "destbuffer0_V_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "destbuffer0_V", "role": "ce0" }} , 
 	{ "name": "destbuffer0_V_we0", "direction": "out", "datatype": "sc_lv", "bitwidth":64, "type": "signal", "bundle":{"name": "destbuffer0_V", "role": "we0" }} , 
 	{ "name": "destbuffer0_V_d0", "direction": "out", "datatype": "sc_lv", "bitwidth":512, "type": "signal", "bundle":{"name": "destbuffer0_V", "role": "d0" }} , 
 	{ "name": "destbuffer0_V_q0", "direction": "in", "datatype": "sc_lv", "bitwidth":512, "type": "signal", "bundle":{"name": "destbuffer0_V", "role": "q0" }} , 
 	{ "name": "destbuffer1_V_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":12, "type": "signal", "bundle":{"name": "destbuffer1_V", "role": "address0" }} , 
 	{ "name": "destbuffer1_V_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "destbuffer1_V", "role": "ce0" }} , 
 	{ "name": "destbuffer1_V_we0", "direction": "out", "datatype": "sc_lv", "bitwidth":64, "type": "signal", "bundle":{"name": "destbuffer1_V", "role": "we0" }} , 
 	{ "name": "destbuffer1_V_d0", "direction": "out", "datatype": "sc_lv", "bitwidth":512, "type": "signal", "bundle":{"name": "destbuffer1_V", "role": "d0" }} , 
 	{ "name": "destbuffer1_V_q0", "direction": "in", "datatype": "sc_lv", "bitwidth":512, "type": "signal", "bundle":{"name": "destbuffer1_V", "role": "q0" }} , 
 	{ "name": "destbuffer2_V_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":12, "type": "signal", "bundle":{"name": "destbuffer2_V", "role": "address0" }} , 
 	{ "name": "destbuffer2_V_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "destbuffer2_V", "role": "ce0" }} , 
 	{ "name": "destbuffer2_V_we0", "direction": "out", "datatype": "sc_lv", "bitwidth":64, "type": "signal", "bundle":{"name": "destbuffer2_V", "role": "we0" }} , 
 	{ "name": "destbuffer2_V_d0", "direction": "out", "datatype": "sc_lv", "bitwidth":512, "type": "signal", "bundle":{"name": "destbuffer2_V", "role": "d0" }} , 
 	{ "name": "destbuffer2_V_q0", "direction": "in", "datatype": "sc_lv", "bitwidth":512, "type": "signal", "bundle":{"name": "destbuffer2_V", "role": "q0" }} , 
 	{ "name": "destbuffer3_V_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":12, "type": "signal", "bundle":{"name": "destbuffer3_V", "role": "address0" }} , 
 	{ "name": "destbuffer3_V_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "destbuffer3_V", "role": "ce0" }} , 
 	{ "name": "destbuffer3_V_we0", "direction": "out", "datatype": "sc_lv", "bitwidth":64, "type": "signal", "bundle":{"name": "destbuffer3_V", "role": "we0" }} , 
 	{ "name": "destbuffer3_V_d0", "direction": "out", "datatype": "sc_lv", "bitwidth":512, "type": "signal", "bundle":{"name": "destbuffer3_V", "role": "d0" }} , 
 	{ "name": "destbuffer3_V_q0", "direction": "in", "datatype": "sc_lv", "bitwidth":512, "type": "signal", "bundle":{"name": "destbuffer3_V", "role": "q0" }} , 
 	{ "name": "globalparams_vbegin", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "globalparams_vbegin", "role": "default" }}  ]}

set RtlHierarchyInfo {[
	{"ID" : "0", "Level" : "0", "Path" : "`AUTOTB_DUT_INST", "Parent" : "", "Child" : ["1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13", "14", "15", "16", "17", "18", "19", "20", "21", "22", "23", "24", "25", "26", "27", "28", "29", "30", "31", "32", "33", "34", "35", "36", "37", "38", "39", "40", "41", "42", "43", "44", "45", "46", "47", "48", "49", "50", "51", "52", "53", "54", "55", "56", "57", "58", "59", "60", "61", "62", "63", "64", "65", "66", "67", "68", "69", "70", "71", "72", "73", "74", "75", "76", "77", "78", "79", "80", "81", "82", "83", "84", "85", "86", "87", "88", "89", "90", "91", "92", "93", "94", "95", "96", "97", "98", "99", "100", "101", "102", "103", "104", "105", "106", "107", "108", "109", "110", "111", "112", "113", "114", "115", "116", "117", "118", "119", "120", "121", "122", "123", "124", "125", "126", "127", "128"],
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
	{"ID" : "1", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.destbuffer00_key_U", "Parent" : "0"},
	{"ID" : "2", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.destbuffer00_value_U", "Parent" : "0"},
	{"ID" : "3", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.destbuffer01_key_U", "Parent" : "0"},
	{"ID" : "4", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.destbuffer01_value_U", "Parent" : "0"},
	{"ID" : "5", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.destbuffer02_key_U", "Parent" : "0"},
	{"ID" : "6", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.destbuffer02_value_U", "Parent" : "0"},
	{"ID" : "7", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.destbuffer03_key_U", "Parent" : "0"},
	{"ID" : "8", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.destbuffer03_value_U", "Parent" : "0"},
	{"ID" : "9", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.destbuffer04_key_U", "Parent" : "0"},
	{"ID" : "10", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.destbuffer04_value_U", "Parent" : "0"},
	{"ID" : "11", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.destbuffer05_key_U", "Parent" : "0"},
	{"ID" : "12", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.destbuffer05_value_U", "Parent" : "0"},
	{"ID" : "13", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.destbuffer06_key_U", "Parent" : "0"},
	{"ID" : "14", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.destbuffer06_value_U", "Parent" : "0"},
	{"ID" : "15", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.destbuffer07_key_U", "Parent" : "0"},
	{"ID" : "16", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.destbuffer07_value_U", "Parent" : "0"},
	{"ID" : "17", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.destbuffer10_key_U", "Parent" : "0"},
	{"ID" : "18", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.destbuffer10_value_U", "Parent" : "0"},
	{"ID" : "19", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.destbuffer11_key_U", "Parent" : "0"},
	{"ID" : "20", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.destbuffer11_value_U", "Parent" : "0"},
	{"ID" : "21", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.destbuffer12_key_U", "Parent" : "0"},
	{"ID" : "22", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.destbuffer12_value_U", "Parent" : "0"},
	{"ID" : "23", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.destbuffer13_key_U", "Parent" : "0"},
	{"ID" : "24", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.destbuffer13_value_U", "Parent" : "0"},
	{"ID" : "25", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.destbuffer14_key_U", "Parent" : "0"},
	{"ID" : "26", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.destbuffer14_value_U", "Parent" : "0"},
	{"ID" : "27", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.destbuffer15_key_U", "Parent" : "0"},
	{"ID" : "28", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.destbuffer15_value_U", "Parent" : "0"},
	{"ID" : "29", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.destbuffer16_key_U", "Parent" : "0"},
	{"ID" : "30", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.destbuffer16_value_U", "Parent" : "0"},
	{"ID" : "31", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.destbuffer17_key_U", "Parent" : "0"},
	{"ID" : "32", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.destbuffer17_value_U", "Parent" : "0"},
	{"ID" : "33", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.destbuffer20_key_U", "Parent" : "0"},
	{"ID" : "34", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.destbuffer20_value_U", "Parent" : "0"},
	{"ID" : "35", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.destbuffer21_key_U", "Parent" : "0"},
	{"ID" : "36", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.destbuffer21_value_U", "Parent" : "0"},
	{"ID" : "37", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.destbuffer22_key_U", "Parent" : "0"},
	{"ID" : "38", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.destbuffer22_value_U", "Parent" : "0"},
	{"ID" : "39", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.destbuffer23_key_U", "Parent" : "0"},
	{"ID" : "40", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.destbuffer23_value_U", "Parent" : "0"},
	{"ID" : "41", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.destbuffer24_key_U", "Parent" : "0"},
	{"ID" : "42", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.destbuffer24_value_U", "Parent" : "0"},
	{"ID" : "43", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.destbuffer25_key_U", "Parent" : "0"},
	{"ID" : "44", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.destbuffer25_value_U", "Parent" : "0"},
	{"ID" : "45", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.destbuffer26_key_U", "Parent" : "0"},
	{"ID" : "46", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.destbuffer26_value_U", "Parent" : "0"},
	{"ID" : "47", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.destbuffer27_key_U", "Parent" : "0"},
	{"ID" : "48", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.destbuffer27_value_U", "Parent" : "0"},
	{"ID" : "49", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.destbuffer30_key_U", "Parent" : "0"},
	{"ID" : "50", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.destbuffer30_value_U", "Parent" : "0"},
	{"ID" : "51", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.destbuffer31_key_U", "Parent" : "0"},
	{"ID" : "52", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.destbuffer31_value_U", "Parent" : "0"},
	{"ID" : "53", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.destbuffer32_key_U", "Parent" : "0"},
	{"ID" : "54", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.destbuffer32_value_U", "Parent" : "0"},
	{"ID" : "55", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.destbuffer33_key_U", "Parent" : "0"},
	{"ID" : "56", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.destbuffer33_value_U", "Parent" : "0"},
	{"ID" : "57", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.destbuffer34_key_U", "Parent" : "0"},
	{"ID" : "58", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.destbuffer34_value_U", "Parent" : "0"},
	{"ID" : "59", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.destbuffer35_key_U", "Parent" : "0"},
	{"ID" : "60", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.destbuffer35_value_U", "Parent" : "0"},
	{"ID" : "61", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.destbuffer36_key_U", "Parent" : "0"},
	{"ID" : "62", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.destbuffer36_value_U", "Parent" : "0"},
	{"ID" : "63", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.destbuffer37_key_U", "Parent" : "0"},
	{"ID" : "64", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.destbuffer37_value_U", "Parent" : "0"},
	{"ID" : "65", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.topkernel_mux_83_32_1_1_U165", "Parent" : "0"},
	{"ID" : "66", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.topkernel_mux_83_32_1_1_U166", "Parent" : "0"},
	{"ID" : "67", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.topkernel_mux_83_32_1_1_U167", "Parent" : "0"},
	{"ID" : "68", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.topkernel_mux_83_32_1_1_U168", "Parent" : "0"},
	{"ID" : "69", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.topkernel_mux_83_32_1_1_U169", "Parent" : "0"},
	{"ID" : "70", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.topkernel_mux_83_32_1_1_U170", "Parent" : "0"},
	{"ID" : "71", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.topkernel_mux_83_32_1_1_U171", "Parent" : "0"},
	{"ID" : "72", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.topkernel_mux_83_32_1_1_U172", "Parent" : "0"},
	{"ID" : "73", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.topkernel_mux_83_32_1_1_U173", "Parent" : "0"},
	{"ID" : "74", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.topkernel_mux_83_32_1_1_U174", "Parent" : "0"},
	{"ID" : "75", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.topkernel_mux_83_32_1_1_U175", "Parent" : "0"},
	{"ID" : "76", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.topkernel_mux_83_32_1_1_U176", "Parent" : "0"},
	{"ID" : "77", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.topkernel_mux_83_32_1_1_U177", "Parent" : "0"},
	{"ID" : "78", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.topkernel_mux_83_32_1_1_U178", "Parent" : "0"},
	{"ID" : "79", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.topkernel_mux_83_32_1_1_U179", "Parent" : "0"},
	{"ID" : "80", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.topkernel_mux_83_32_1_1_U180", "Parent" : "0"},
	{"ID" : "81", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.topkernel_mux_83_32_1_1_U181", "Parent" : "0"},
	{"ID" : "82", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.topkernel_mux_83_32_1_1_U182", "Parent" : "0"},
	{"ID" : "83", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.topkernel_mux_83_32_1_1_U183", "Parent" : "0"},
	{"ID" : "84", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.topkernel_mux_83_32_1_1_U184", "Parent" : "0"},
	{"ID" : "85", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.topkernel_mux_83_32_1_1_U185", "Parent" : "0"},
	{"ID" : "86", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.topkernel_mux_83_32_1_1_U186", "Parent" : "0"},
	{"ID" : "87", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.topkernel_mux_83_32_1_1_U187", "Parent" : "0"},
	{"ID" : "88", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.topkernel_mux_83_32_1_1_U188", "Parent" : "0"},
	{"ID" : "89", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.topkernel_mux_83_32_1_1_U189", "Parent" : "0"},
	{"ID" : "90", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.topkernel_mux_83_32_1_1_U190", "Parent" : "0"},
	{"ID" : "91", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.topkernel_mux_83_32_1_1_U191", "Parent" : "0"},
	{"ID" : "92", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.topkernel_mux_83_32_1_1_U192", "Parent" : "0"},
	{"ID" : "93", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.topkernel_mux_83_32_1_1_U193", "Parent" : "0"},
	{"ID" : "94", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.topkernel_mux_83_32_1_1_U194", "Parent" : "0"},
	{"ID" : "95", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.topkernel_mux_83_32_1_1_U195", "Parent" : "0"},
	{"ID" : "96", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.topkernel_mux_83_32_1_1_U196", "Parent" : "0"},
	{"ID" : "97", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.topkernel_mux_83_32_1_1_U197", "Parent" : "0"},
	{"ID" : "98", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.topkernel_mux_83_32_1_1_U198", "Parent" : "0"},
	{"ID" : "99", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.topkernel_mux_83_32_1_1_U199", "Parent" : "0"},
	{"ID" : "100", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.topkernel_mux_83_32_1_1_U200", "Parent" : "0"},
	{"ID" : "101", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.topkernel_mux_83_32_1_1_U201", "Parent" : "0"},
	{"ID" : "102", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.topkernel_mux_83_32_1_1_U202", "Parent" : "0"},
	{"ID" : "103", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.topkernel_mux_83_32_1_1_U203", "Parent" : "0"},
	{"ID" : "104", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.topkernel_mux_83_32_1_1_U204", "Parent" : "0"},
	{"ID" : "105", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.topkernel_mux_83_32_1_1_U205", "Parent" : "0"},
	{"ID" : "106", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.topkernel_mux_83_32_1_1_U206", "Parent" : "0"},
	{"ID" : "107", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.topkernel_mux_83_32_1_1_U207", "Parent" : "0"},
	{"ID" : "108", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.topkernel_mux_83_32_1_1_U208", "Parent" : "0"},
	{"ID" : "109", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.topkernel_mux_83_32_1_1_U209", "Parent" : "0"},
	{"ID" : "110", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.topkernel_mux_83_32_1_1_U210", "Parent" : "0"},
	{"ID" : "111", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.topkernel_mux_83_32_1_1_U211", "Parent" : "0"},
	{"ID" : "112", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.topkernel_mux_83_32_1_1_U212", "Parent" : "0"},
	{"ID" : "113", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.topkernel_mux_83_32_1_1_U213", "Parent" : "0"},
	{"ID" : "114", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.topkernel_mux_83_32_1_1_U214", "Parent" : "0"},
	{"ID" : "115", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.topkernel_mux_83_32_1_1_U215", "Parent" : "0"},
	{"ID" : "116", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.topkernel_mux_83_32_1_1_U216", "Parent" : "0"},
	{"ID" : "117", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.topkernel_mux_83_32_1_1_U217", "Parent" : "0"},
	{"ID" : "118", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.topkernel_mux_83_32_1_1_U218", "Parent" : "0"},
	{"ID" : "119", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.topkernel_mux_83_32_1_1_U219", "Parent" : "0"},
	{"ID" : "120", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.topkernel_mux_83_32_1_1_U220", "Parent" : "0"},
	{"ID" : "121", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.topkernel_mux_83_32_1_1_U221", "Parent" : "0"},
	{"ID" : "122", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.topkernel_mux_83_32_1_1_U222", "Parent" : "0"},
	{"ID" : "123", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.topkernel_mux_83_32_1_1_U223", "Parent" : "0"},
	{"ID" : "124", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.topkernel_mux_83_32_1_1_U224", "Parent" : "0"},
	{"ID" : "125", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.topkernel_mux_83_32_1_1_U225", "Parent" : "0"},
	{"ID" : "126", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.topkernel_mux_83_32_1_1_U226", "Parent" : "0"},
	{"ID" : "127", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.topkernel_mux_83_32_1_1_U227", "Parent" : "0"},
	{"ID" : "128", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.topkernel_mux_83_32_1_1_U228", "Parent" : "0"}]}


set ArgLastReadFirstWriteLatency {
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
		globalparams_vbegin {Type I LastRead 0 FirstWrite -1}}}

set hasDtUnsupportedChannel 0

set PerformanceInfo {[
	{"Name" : "Latency", "Min" : "1", "Max" : "5157"}
	, {"Name" : "Interval", "Min" : "1", "Max" : "5157"}
]}

set PipelineEnableSignalInfo {[
	{"Pipeline" : "0", "EnableSignal" : "ap_enable_pp0"}
	{"Pipeline" : "1", "EnableSignal" : "ap_enable_pp1"}
	{"Pipeline" : "3", "EnableSignal" : "ap_enable_pp3"}
]}

set Spec2ImplPortList { 
	enable { ap_none {  { enable in_data 0 1 } } }
	sourcebuffer0_V { ap_memory {  { sourcebuffer0_V_address0 mem_address 1 9 }  { sourcebuffer0_V_ce0 mem_ce 1 1 }  { sourcebuffer0_V_q0 mem_dout 0 512 } } }
	sourcebuffer1_V { ap_memory {  { sourcebuffer1_V_address0 mem_address 1 9 }  { sourcebuffer1_V_ce0 mem_ce 1 1 }  { sourcebuffer1_V_q0 mem_dout 0 512 } } }
	sourcebuffer2_V { ap_memory {  { sourcebuffer2_V_address0 mem_address 1 9 }  { sourcebuffer2_V_ce0 mem_ce 1 1 }  { sourcebuffer2_V_q0 mem_dout 0 512 } } }
	sourcebuffer3_V { ap_memory {  { sourcebuffer3_V_address0 mem_address 1 9 }  { sourcebuffer3_V_ce0 mem_ce 1 1 }  { sourcebuffer3_V_q0 mem_dout 0 512 } } }
	destbuffer0_V { ap_memory {  { destbuffer0_V_address0 mem_address 1 12 }  { destbuffer0_V_ce0 mem_ce 1 1 }  { destbuffer0_V_we0 mem_we 1 64 }  { destbuffer0_V_d0 mem_din 1 512 }  { destbuffer0_V_q0 mem_dout 0 512 } } }
	destbuffer1_V { ap_memory {  { destbuffer1_V_address0 mem_address 1 12 }  { destbuffer1_V_ce0 mem_ce 1 1 }  { destbuffer1_V_we0 mem_we 1 64 }  { destbuffer1_V_d0 mem_din 1 512 }  { destbuffer1_V_q0 mem_dout 0 512 } } }
	destbuffer2_V { ap_memory {  { destbuffer2_V_address0 mem_address 1 12 }  { destbuffer2_V_ce0 mem_ce 1 1 }  { destbuffer2_V_we0 mem_we 1 64 }  { destbuffer2_V_d0 mem_din 1 512 }  { destbuffer2_V_q0 mem_dout 0 512 } } }
	destbuffer3_V { ap_memory {  { destbuffer3_V_address0 mem_address 1 12 }  { destbuffer3_V_ce0 mem_ce 1 1 }  { destbuffer3_V_we0 mem_we 1 64 }  { destbuffer3_V_d0 mem_din 1 512 }  { destbuffer3_V_q0 mem_dout 0 512 } } }
	globalparams_vbegin { ap_none {  { globalparams_vbegin in_data 0 32 } } }
}
set moduleName reducepartitions0
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
set C_modelName {reducepartitions0}
set C_modelType { void 0 }
set C_modelArgList {
	{ enable int 1 regular  }
	{ sourcebuffer0_V int 512 regular {array 512 { 1 3 } 1 1 }  }
	{ sourcebuffer1_V int 512 regular {array 512 { 1 3 } 1 1 }  }
	{ sourcebuffer2_V int 512 regular {array 512 { 1 3 } 1 1 }  }
	{ sourcebuffer3_V int 512 regular {array 512 { 1 3 } 1 1 }  }
	{ destbuffer00_value int 32 regular {array 2112 { 2 3 } 1 1 }  }
	{ destbuffer01_value int 32 regular {array 2112 { 2 3 } 1 1 }  }
	{ destbuffer02_value int 32 regular {array 2112 { 2 3 } 1 1 }  }
	{ destbuffer03_value int 32 regular {array 2112 { 2 3 } 1 1 }  }
	{ destbuffer04_value int 32 regular {array 2112 { 2 3 } 1 1 }  }
	{ destbuffer05_value int 32 regular {array 2112 { 2 3 } 1 1 }  }
	{ destbuffer06_value int 32 regular {array 2112 { 2 3 } 1 1 }  }
	{ destbuffer07_value int 32 regular {array 2112 { 2 3 } 1 1 }  }
	{ destbuffer10_value int 32 regular {array 2112 { 2 3 } 1 1 }  }
	{ destbuffer11_value int 32 regular {array 2112 { 2 3 } 1 1 }  }
	{ destbuffer12_value int 32 regular {array 2112 { 2 3 } 1 1 }  }
	{ destbuffer13_value int 32 regular {array 2112 { 2 3 } 1 1 }  }
	{ destbuffer14_value int 32 regular {array 2112 { 2 3 } 1 1 }  }
	{ destbuffer15_value int 32 regular {array 2112 { 2 3 } 1 1 }  }
	{ destbuffer16_value int 32 regular {array 2112 { 2 3 } 1 1 }  }
	{ destbuffer17_value int 32 regular {array 2112 { 2 3 } 1 1 }  }
	{ destbuffer20_value int 32 regular {array 2112 { 2 3 } 1 1 }  }
	{ destbuffer21_value int 32 regular {array 2112 { 2 3 } 1 1 }  }
	{ destbuffer22_value int 32 regular {array 2112 { 2 3 } 1 1 }  }
	{ destbuffer23_value int 32 regular {array 2112 { 2 3 } 1 1 }  }
	{ destbuffer24_value int 32 regular {array 2112 { 2 3 } 1 1 }  }
	{ destbuffer25_value int 32 regular {array 2112 { 2 3 } 1 1 }  }
	{ destbuffer26_value int 32 regular {array 2112 { 2 3 } 1 1 }  }
	{ destbuffer27_value int 32 regular {array 2112 { 2 3 } 1 1 }  }
	{ destbuffer30_value int 32 regular {array 2112 { 2 3 } 1 1 }  }
	{ destbuffer31_value int 32 regular {array 2112 { 2 3 } 1 1 }  }
	{ destbuffer32_value int 32 regular {array 2112 { 2 3 } 1 1 }  }
	{ destbuffer33_value int 32 regular {array 2112 { 2 3 } 1 1 }  }
	{ destbuffer34_value int 32 regular {array 2112 { 2 3 } 1 1 }  }
	{ destbuffer35_value int 32 regular {array 2112 { 2 3 } 1 1 }  }
	{ destbuffer36_value int 32 regular {array 2112 { 2 3 } 1 1 }  }
	{ destbuffer37_value int 32 regular {array 2112 { 2 3 } 1 1 }  }
	{ globalparams_vbegin int 32 regular  }
}
set C_modelArgMapList {[ 
	{ "Name" : "enable", "interface" : "wire", "bitwidth" : 1, "direction" : "READONLY"} , 
 	{ "Name" : "sourcebuffer0_V", "interface" : "memory", "bitwidth" : 512, "direction" : "READONLY"} , 
 	{ "Name" : "sourcebuffer1_V", "interface" : "memory", "bitwidth" : 512, "direction" : "READONLY"} , 
 	{ "Name" : "sourcebuffer2_V", "interface" : "memory", "bitwidth" : 512, "direction" : "READONLY"} , 
 	{ "Name" : "sourcebuffer3_V", "interface" : "memory", "bitwidth" : 512, "direction" : "READONLY"} , 
 	{ "Name" : "destbuffer00_value", "interface" : "memory", "bitwidth" : 32, "direction" : "READWRITE"} , 
 	{ "Name" : "destbuffer01_value", "interface" : "memory", "bitwidth" : 32, "direction" : "READWRITE"} , 
 	{ "Name" : "destbuffer02_value", "interface" : "memory", "bitwidth" : 32, "direction" : "READWRITE"} , 
 	{ "Name" : "destbuffer03_value", "interface" : "memory", "bitwidth" : 32, "direction" : "READWRITE"} , 
 	{ "Name" : "destbuffer04_value", "interface" : "memory", "bitwidth" : 32, "direction" : "READWRITE"} , 
 	{ "Name" : "destbuffer05_value", "interface" : "memory", "bitwidth" : 32, "direction" : "READWRITE"} , 
 	{ "Name" : "destbuffer06_value", "interface" : "memory", "bitwidth" : 32, "direction" : "READWRITE"} , 
 	{ "Name" : "destbuffer07_value", "interface" : "memory", "bitwidth" : 32, "direction" : "READWRITE"} , 
 	{ "Name" : "destbuffer10_value", "interface" : "memory", "bitwidth" : 32, "direction" : "READWRITE"} , 
 	{ "Name" : "destbuffer11_value", "interface" : "memory", "bitwidth" : 32, "direction" : "READWRITE"} , 
 	{ "Name" : "destbuffer12_value", "interface" : "memory", "bitwidth" : 32, "direction" : "READWRITE"} , 
 	{ "Name" : "destbuffer13_value", "interface" : "memory", "bitwidth" : 32, "direction" : "READWRITE"} , 
 	{ "Name" : "destbuffer14_value", "interface" : "memory", "bitwidth" : 32, "direction" : "READWRITE"} , 
 	{ "Name" : "destbuffer15_value", "interface" : "memory", "bitwidth" : 32, "direction" : "READWRITE"} , 
 	{ "Name" : "destbuffer16_value", "interface" : "memory", "bitwidth" : 32, "direction" : "READWRITE"} , 
 	{ "Name" : "destbuffer17_value", "interface" : "memory", "bitwidth" : 32, "direction" : "READWRITE"} , 
 	{ "Name" : "destbuffer20_value", "interface" : "memory", "bitwidth" : 32, "direction" : "READWRITE"} , 
 	{ "Name" : "destbuffer21_value", "interface" : "memory", "bitwidth" : 32, "direction" : "READWRITE"} , 
 	{ "Name" : "destbuffer22_value", "interface" : "memory", "bitwidth" : 32, "direction" : "READWRITE"} , 
 	{ "Name" : "destbuffer23_value", "interface" : "memory", "bitwidth" : 32, "direction" : "READWRITE"} , 
 	{ "Name" : "destbuffer24_value", "interface" : "memory", "bitwidth" : 32, "direction" : "READWRITE"} , 
 	{ "Name" : "destbuffer25_value", "interface" : "memory", "bitwidth" : 32, "direction" : "READWRITE"} , 
 	{ "Name" : "destbuffer26_value", "interface" : "memory", "bitwidth" : 32, "direction" : "READWRITE"} , 
 	{ "Name" : "destbuffer27_value", "interface" : "memory", "bitwidth" : 32, "direction" : "READWRITE"} , 
 	{ "Name" : "destbuffer30_value", "interface" : "memory", "bitwidth" : 32, "direction" : "READWRITE"} , 
 	{ "Name" : "destbuffer31_value", "interface" : "memory", "bitwidth" : 32, "direction" : "READWRITE"} , 
 	{ "Name" : "destbuffer32_value", "interface" : "memory", "bitwidth" : 32, "direction" : "READWRITE"} , 
 	{ "Name" : "destbuffer33_value", "interface" : "memory", "bitwidth" : 32, "direction" : "READWRITE"} , 
 	{ "Name" : "destbuffer34_value", "interface" : "memory", "bitwidth" : 32, "direction" : "READWRITE"} , 
 	{ "Name" : "destbuffer35_value", "interface" : "memory", "bitwidth" : 32, "direction" : "READWRITE"} , 
 	{ "Name" : "destbuffer36_value", "interface" : "memory", "bitwidth" : 32, "direction" : "READWRITE"} , 
 	{ "Name" : "destbuffer37_value", "interface" : "memory", "bitwidth" : 32, "direction" : "READWRITE"} , 
 	{ "Name" : "globalparams_vbegin", "interface" : "wire", "bitwidth" : 32, "direction" : "READONLY"} ]}
# RTL Port declarations: 
set portNum 180
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
	{ destbuffer00_value_address0 sc_out sc_lv 12 signal 5 } 
	{ destbuffer00_value_ce0 sc_out sc_logic 1 signal 5 } 
	{ destbuffer00_value_we0 sc_out sc_logic 1 signal 5 } 
	{ destbuffer00_value_d0 sc_out sc_lv 32 signal 5 } 
	{ destbuffer00_value_q0 sc_in sc_lv 32 signal 5 } 
	{ destbuffer01_value_address0 sc_out sc_lv 12 signal 6 } 
	{ destbuffer01_value_ce0 sc_out sc_logic 1 signal 6 } 
	{ destbuffer01_value_we0 sc_out sc_logic 1 signal 6 } 
	{ destbuffer01_value_d0 sc_out sc_lv 32 signal 6 } 
	{ destbuffer01_value_q0 sc_in sc_lv 32 signal 6 } 
	{ destbuffer02_value_address0 sc_out sc_lv 12 signal 7 } 
	{ destbuffer02_value_ce0 sc_out sc_logic 1 signal 7 } 
	{ destbuffer02_value_we0 sc_out sc_logic 1 signal 7 } 
	{ destbuffer02_value_d0 sc_out sc_lv 32 signal 7 } 
	{ destbuffer02_value_q0 sc_in sc_lv 32 signal 7 } 
	{ destbuffer03_value_address0 sc_out sc_lv 12 signal 8 } 
	{ destbuffer03_value_ce0 sc_out sc_logic 1 signal 8 } 
	{ destbuffer03_value_we0 sc_out sc_logic 1 signal 8 } 
	{ destbuffer03_value_d0 sc_out sc_lv 32 signal 8 } 
	{ destbuffer03_value_q0 sc_in sc_lv 32 signal 8 } 
	{ destbuffer04_value_address0 sc_out sc_lv 12 signal 9 } 
	{ destbuffer04_value_ce0 sc_out sc_logic 1 signal 9 } 
	{ destbuffer04_value_we0 sc_out sc_logic 1 signal 9 } 
	{ destbuffer04_value_d0 sc_out sc_lv 32 signal 9 } 
	{ destbuffer04_value_q0 sc_in sc_lv 32 signal 9 } 
	{ destbuffer05_value_address0 sc_out sc_lv 12 signal 10 } 
	{ destbuffer05_value_ce0 sc_out sc_logic 1 signal 10 } 
	{ destbuffer05_value_we0 sc_out sc_logic 1 signal 10 } 
	{ destbuffer05_value_d0 sc_out sc_lv 32 signal 10 } 
	{ destbuffer05_value_q0 sc_in sc_lv 32 signal 10 } 
	{ destbuffer06_value_address0 sc_out sc_lv 12 signal 11 } 
	{ destbuffer06_value_ce0 sc_out sc_logic 1 signal 11 } 
	{ destbuffer06_value_we0 sc_out sc_logic 1 signal 11 } 
	{ destbuffer06_value_d0 sc_out sc_lv 32 signal 11 } 
	{ destbuffer06_value_q0 sc_in sc_lv 32 signal 11 } 
	{ destbuffer07_value_address0 sc_out sc_lv 12 signal 12 } 
	{ destbuffer07_value_ce0 sc_out sc_logic 1 signal 12 } 
	{ destbuffer07_value_we0 sc_out sc_logic 1 signal 12 } 
	{ destbuffer07_value_d0 sc_out sc_lv 32 signal 12 } 
	{ destbuffer07_value_q0 sc_in sc_lv 32 signal 12 } 
	{ destbuffer10_value_address0 sc_out sc_lv 12 signal 13 } 
	{ destbuffer10_value_ce0 sc_out sc_logic 1 signal 13 } 
	{ destbuffer10_value_we0 sc_out sc_logic 1 signal 13 } 
	{ destbuffer10_value_d0 sc_out sc_lv 32 signal 13 } 
	{ destbuffer10_value_q0 sc_in sc_lv 32 signal 13 } 
	{ destbuffer11_value_address0 sc_out sc_lv 12 signal 14 } 
	{ destbuffer11_value_ce0 sc_out sc_logic 1 signal 14 } 
	{ destbuffer11_value_we0 sc_out sc_logic 1 signal 14 } 
	{ destbuffer11_value_d0 sc_out sc_lv 32 signal 14 } 
	{ destbuffer11_value_q0 sc_in sc_lv 32 signal 14 } 
	{ destbuffer12_value_address0 sc_out sc_lv 12 signal 15 } 
	{ destbuffer12_value_ce0 sc_out sc_logic 1 signal 15 } 
	{ destbuffer12_value_we0 sc_out sc_logic 1 signal 15 } 
	{ destbuffer12_value_d0 sc_out sc_lv 32 signal 15 } 
	{ destbuffer12_value_q0 sc_in sc_lv 32 signal 15 } 
	{ destbuffer13_value_address0 sc_out sc_lv 12 signal 16 } 
	{ destbuffer13_value_ce0 sc_out sc_logic 1 signal 16 } 
	{ destbuffer13_value_we0 sc_out sc_logic 1 signal 16 } 
	{ destbuffer13_value_d0 sc_out sc_lv 32 signal 16 } 
	{ destbuffer13_value_q0 sc_in sc_lv 32 signal 16 } 
	{ destbuffer14_value_address0 sc_out sc_lv 12 signal 17 } 
	{ destbuffer14_value_ce0 sc_out sc_logic 1 signal 17 } 
	{ destbuffer14_value_we0 sc_out sc_logic 1 signal 17 } 
	{ destbuffer14_value_d0 sc_out sc_lv 32 signal 17 } 
	{ destbuffer14_value_q0 sc_in sc_lv 32 signal 17 } 
	{ destbuffer15_value_address0 sc_out sc_lv 12 signal 18 } 
	{ destbuffer15_value_ce0 sc_out sc_logic 1 signal 18 } 
	{ destbuffer15_value_we0 sc_out sc_logic 1 signal 18 } 
	{ destbuffer15_value_d0 sc_out sc_lv 32 signal 18 } 
	{ destbuffer15_value_q0 sc_in sc_lv 32 signal 18 } 
	{ destbuffer16_value_address0 sc_out sc_lv 12 signal 19 } 
	{ destbuffer16_value_ce0 sc_out sc_logic 1 signal 19 } 
	{ destbuffer16_value_we0 sc_out sc_logic 1 signal 19 } 
	{ destbuffer16_value_d0 sc_out sc_lv 32 signal 19 } 
	{ destbuffer16_value_q0 sc_in sc_lv 32 signal 19 } 
	{ destbuffer17_value_address0 sc_out sc_lv 12 signal 20 } 
	{ destbuffer17_value_ce0 sc_out sc_logic 1 signal 20 } 
	{ destbuffer17_value_we0 sc_out sc_logic 1 signal 20 } 
	{ destbuffer17_value_d0 sc_out sc_lv 32 signal 20 } 
	{ destbuffer17_value_q0 sc_in sc_lv 32 signal 20 } 
	{ destbuffer20_value_address0 sc_out sc_lv 12 signal 21 } 
	{ destbuffer20_value_ce0 sc_out sc_logic 1 signal 21 } 
	{ destbuffer20_value_we0 sc_out sc_logic 1 signal 21 } 
	{ destbuffer20_value_d0 sc_out sc_lv 32 signal 21 } 
	{ destbuffer20_value_q0 sc_in sc_lv 32 signal 21 } 
	{ destbuffer21_value_address0 sc_out sc_lv 12 signal 22 } 
	{ destbuffer21_value_ce0 sc_out sc_logic 1 signal 22 } 
	{ destbuffer21_value_we0 sc_out sc_logic 1 signal 22 } 
	{ destbuffer21_value_d0 sc_out sc_lv 32 signal 22 } 
	{ destbuffer21_value_q0 sc_in sc_lv 32 signal 22 } 
	{ destbuffer22_value_address0 sc_out sc_lv 12 signal 23 } 
	{ destbuffer22_value_ce0 sc_out sc_logic 1 signal 23 } 
	{ destbuffer22_value_we0 sc_out sc_logic 1 signal 23 } 
	{ destbuffer22_value_d0 sc_out sc_lv 32 signal 23 } 
	{ destbuffer22_value_q0 sc_in sc_lv 32 signal 23 } 
	{ destbuffer23_value_address0 sc_out sc_lv 12 signal 24 } 
	{ destbuffer23_value_ce0 sc_out sc_logic 1 signal 24 } 
	{ destbuffer23_value_we0 sc_out sc_logic 1 signal 24 } 
	{ destbuffer23_value_d0 sc_out sc_lv 32 signal 24 } 
	{ destbuffer23_value_q0 sc_in sc_lv 32 signal 24 } 
	{ destbuffer24_value_address0 sc_out sc_lv 12 signal 25 } 
	{ destbuffer24_value_ce0 sc_out sc_logic 1 signal 25 } 
	{ destbuffer24_value_we0 sc_out sc_logic 1 signal 25 } 
	{ destbuffer24_value_d0 sc_out sc_lv 32 signal 25 } 
	{ destbuffer24_value_q0 sc_in sc_lv 32 signal 25 } 
	{ destbuffer25_value_address0 sc_out sc_lv 12 signal 26 } 
	{ destbuffer25_value_ce0 sc_out sc_logic 1 signal 26 } 
	{ destbuffer25_value_we0 sc_out sc_logic 1 signal 26 } 
	{ destbuffer25_value_d0 sc_out sc_lv 32 signal 26 } 
	{ destbuffer25_value_q0 sc_in sc_lv 32 signal 26 } 
	{ destbuffer26_value_address0 sc_out sc_lv 12 signal 27 } 
	{ destbuffer26_value_ce0 sc_out sc_logic 1 signal 27 } 
	{ destbuffer26_value_we0 sc_out sc_logic 1 signal 27 } 
	{ destbuffer26_value_d0 sc_out sc_lv 32 signal 27 } 
	{ destbuffer26_value_q0 sc_in sc_lv 32 signal 27 } 
	{ destbuffer27_value_address0 sc_out sc_lv 12 signal 28 } 
	{ destbuffer27_value_ce0 sc_out sc_logic 1 signal 28 } 
	{ destbuffer27_value_we0 sc_out sc_logic 1 signal 28 } 
	{ destbuffer27_value_d0 sc_out sc_lv 32 signal 28 } 
	{ destbuffer27_value_q0 sc_in sc_lv 32 signal 28 } 
	{ destbuffer30_value_address0 sc_out sc_lv 12 signal 29 } 
	{ destbuffer30_value_ce0 sc_out sc_logic 1 signal 29 } 
	{ destbuffer30_value_we0 sc_out sc_logic 1 signal 29 } 
	{ destbuffer30_value_d0 sc_out sc_lv 32 signal 29 } 
	{ destbuffer30_value_q0 sc_in sc_lv 32 signal 29 } 
	{ destbuffer31_value_address0 sc_out sc_lv 12 signal 30 } 
	{ destbuffer31_value_ce0 sc_out sc_logic 1 signal 30 } 
	{ destbuffer31_value_we0 sc_out sc_logic 1 signal 30 } 
	{ destbuffer31_value_d0 sc_out sc_lv 32 signal 30 } 
	{ destbuffer31_value_q0 sc_in sc_lv 32 signal 30 } 
	{ destbuffer32_value_address0 sc_out sc_lv 12 signal 31 } 
	{ destbuffer32_value_ce0 sc_out sc_logic 1 signal 31 } 
	{ destbuffer32_value_we0 sc_out sc_logic 1 signal 31 } 
	{ destbuffer32_value_d0 sc_out sc_lv 32 signal 31 } 
	{ destbuffer32_value_q0 sc_in sc_lv 32 signal 31 } 
	{ destbuffer33_value_address0 sc_out sc_lv 12 signal 32 } 
	{ destbuffer33_value_ce0 sc_out sc_logic 1 signal 32 } 
	{ destbuffer33_value_we0 sc_out sc_logic 1 signal 32 } 
	{ destbuffer33_value_d0 sc_out sc_lv 32 signal 32 } 
	{ destbuffer33_value_q0 sc_in sc_lv 32 signal 32 } 
	{ destbuffer34_value_address0 sc_out sc_lv 12 signal 33 } 
	{ destbuffer34_value_ce0 sc_out sc_logic 1 signal 33 } 
	{ destbuffer34_value_we0 sc_out sc_logic 1 signal 33 } 
	{ destbuffer34_value_d0 sc_out sc_lv 32 signal 33 } 
	{ destbuffer34_value_q0 sc_in sc_lv 32 signal 33 } 
	{ destbuffer35_value_address0 sc_out sc_lv 12 signal 34 } 
	{ destbuffer35_value_ce0 sc_out sc_logic 1 signal 34 } 
	{ destbuffer35_value_we0 sc_out sc_logic 1 signal 34 } 
	{ destbuffer35_value_d0 sc_out sc_lv 32 signal 34 } 
	{ destbuffer35_value_q0 sc_in sc_lv 32 signal 34 } 
	{ destbuffer36_value_address0 sc_out sc_lv 12 signal 35 } 
	{ destbuffer36_value_ce0 sc_out sc_logic 1 signal 35 } 
	{ destbuffer36_value_we0 sc_out sc_logic 1 signal 35 } 
	{ destbuffer36_value_d0 sc_out sc_lv 32 signal 35 } 
	{ destbuffer36_value_q0 sc_in sc_lv 32 signal 35 } 
	{ destbuffer37_value_address0 sc_out sc_lv 12 signal 36 } 
	{ destbuffer37_value_ce0 sc_out sc_logic 1 signal 36 } 
	{ destbuffer37_value_we0 sc_out sc_logic 1 signal 36 } 
	{ destbuffer37_value_d0 sc_out sc_lv 32 signal 36 } 
	{ destbuffer37_value_q0 sc_in sc_lv 32 signal 36 } 
	{ globalparams_vbegin sc_in sc_lv 32 signal 37 } 
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
 	{ "name": "destbuffer00_value_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":12, "type": "signal", "bundle":{"name": "destbuffer00_value", "role": "address0" }} , 
 	{ "name": "destbuffer00_value_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "destbuffer00_value", "role": "ce0" }} , 
 	{ "name": "destbuffer00_value_we0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "destbuffer00_value", "role": "we0" }} , 
 	{ "name": "destbuffer00_value_d0", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "destbuffer00_value", "role": "d0" }} , 
 	{ "name": "destbuffer00_value_q0", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "destbuffer00_value", "role": "q0" }} , 
 	{ "name": "destbuffer01_value_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":12, "type": "signal", "bundle":{"name": "destbuffer01_value", "role": "address0" }} , 
 	{ "name": "destbuffer01_value_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "destbuffer01_value", "role": "ce0" }} , 
 	{ "name": "destbuffer01_value_we0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "destbuffer01_value", "role": "we0" }} , 
 	{ "name": "destbuffer01_value_d0", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "destbuffer01_value", "role": "d0" }} , 
 	{ "name": "destbuffer01_value_q0", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "destbuffer01_value", "role": "q0" }} , 
 	{ "name": "destbuffer02_value_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":12, "type": "signal", "bundle":{"name": "destbuffer02_value", "role": "address0" }} , 
 	{ "name": "destbuffer02_value_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "destbuffer02_value", "role": "ce0" }} , 
 	{ "name": "destbuffer02_value_we0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "destbuffer02_value", "role": "we0" }} , 
 	{ "name": "destbuffer02_value_d0", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "destbuffer02_value", "role": "d0" }} , 
 	{ "name": "destbuffer02_value_q0", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "destbuffer02_value", "role": "q0" }} , 
 	{ "name": "destbuffer03_value_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":12, "type": "signal", "bundle":{"name": "destbuffer03_value", "role": "address0" }} , 
 	{ "name": "destbuffer03_value_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "destbuffer03_value", "role": "ce0" }} , 
 	{ "name": "destbuffer03_value_we0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "destbuffer03_value", "role": "we0" }} , 
 	{ "name": "destbuffer03_value_d0", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "destbuffer03_value", "role": "d0" }} , 
 	{ "name": "destbuffer03_value_q0", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "destbuffer03_value", "role": "q0" }} , 
 	{ "name": "destbuffer04_value_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":12, "type": "signal", "bundle":{"name": "destbuffer04_value", "role": "address0" }} , 
 	{ "name": "destbuffer04_value_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "destbuffer04_value", "role": "ce0" }} , 
 	{ "name": "destbuffer04_value_we0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "destbuffer04_value", "role": "we0" }} , 
 	{ "name": "destbuffer04_value_d0", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "destbuffer04_value", "role": "d0" }} , 
 	{ "name": "destbuffer04_value_q0", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "destbuffer04_value", "role": "q0" }} , 
 	{ "name": "destbuffer05_value_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":12, "type": "signal", "bundle":{"name": "destbuffer05_value", "role": "address0" }} , 
 	{ "name": "destbuffer05_value_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "destbuffer05_value", "role": "ce0" }} , 
 	{ "name": "destbuffer05_value_we0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "destbuffer05_value", "role": "we0" }} , 
 	{ "name": "destbuffer05_value_d0", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "destbuffer05_value", "role": "d0" }} , 
 	{ "name": "destbuffer05_value_q0", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "destbuffer05_value", "role": "q0" }} , 
 	{ "name": "destbuffer06_value_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":12, "type": "signal", "bundle":{"name": "destbuffer06_value", "role": "address0" }} , 
 	{ "name": "destbuffer06_value_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "destbuffer06_value", "role": "ce0" }} , 
 	{ "name": "destbuffer06_value_we0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "destbuffer06_value", "role": "we0" }} , 
 	{ "name": "destbuffer06_value_d0", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "destbuffer06_value", "role": "d0" }} , 
 	{ "name": "destbuffer06_value_q0", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "destbuffer06_value", "role": "q0" }} , 
 	{ "name": "destbuffer07_value_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":12, "type": "signal", "bundle":{"name": "destbuffer07_value", "role": "address0" }} , 
 	{ "name": "destbuffer07_value_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "destbuffer07_value", "role": "ce0" }} , 
 	{ "name": "destbuffer07_value_we0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "destbuffer07_value", "role": "we0" }} , 
 	{ "name": "destbuffer07_value_d0", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "destbuffer07_value", "role": "d0" }} , 
 	{ "name": "destbuffer07_value_q0", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "destbuffer07_value", "role": "q0" }} , 
 	{ "name": "destbuffer10_value_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":12, "type": "signal", "bundle":{"name": "destbuffer10_value", "role": "address0" }} , 
 	{ "name": "destbuffer10_value_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "destbuffer10_value", "role": "ce0" }} , 
 	{ "name": "destbuffer10_value_we0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "destbuffer10_value", "role": "we0" }} , 
 	{ "name": "destbuffer10_value_d0", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "destbuffer10_value", "role": "d0" }} , 
 	{ "name": "destbuffer10_value_q0", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "destbuffer10_value", "role": "q0" }} , 
 	{ "name": "destbuffer11_value_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":12, "type": "signal", "bundle":{"name": "destbuffer11_value", "role": "address0" }} , 
 	{ "name": "destbuffer11_value_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "destbuffer11_value", "role": "ce0" }} , 
 	{ "name": "destbuffer11_value_we0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "destbuffer11_value", "role": "we0" }} , 
 	{ "name": "destbuffer11_value_d0", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "destbuffer11_value", "role": "d0" }} , 
 	{ "name": "destbuffer11_value_q0", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "destbuffer11_value", "role": "q0" }} , 
 	{ "name": "destbuffer12_value_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":12, "type": "signal", "bundle":{"name": "destbuffer12_value", "role": "address0" }} , 
 	{ "name": "destbuffer12_value_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "destbuffer12_value", "role": "ce0" }} , 
 	{ "name": "destbuffer12_value_we0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "destbuffer12_value", "role": "we0" }} , 
 	{ "name": "destbuffer12_value_d0", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "destbuffer12_value", "role": "d0" }} , 
 	{ "name": "destbuffer12_value_q0", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "destbuffer12_value", "role": "q0" }} , 
 	{ "name": "destbuffer13_value_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":12, "type": "signal", "bundle":{"name": "destbuffer13_value", "role": "address0" }} , 
 	{ "name": "destbuffer13_value_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "destbuffer13_value", "role": "ce0" }} , 
 	{ "name": "destbuffer13_value_we0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "destbuffer13_value", "role": "we0" }} , 
 	{ "name": "destbuffer13_value_d0", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "destbuffer13_value", "role": "d0" }} , 
 	{ "name": "destbuffer13_value_q0", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "destbuffer13_value", "role": "q0" }} , 
 	{ "name": "destbuffer14_value_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":12, "type": "signal", "bundle":{"name": "destbuffer14_value", "role": "address0" }} , 
 	{ "name": "destbuffer14_value_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "destbuffer14_value", "role": "ce0" }} , 
 	{ "name": "destbuffer14_value_we0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "destbuffer14_value", "role": "we0" }} , 
 	{ "name": "destbuffer14_value_d0", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "destbuffer14_value", "role": "d0" }} , 
 	{ "name": "destbuffer14_value_q0", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "destbuffer14_value", "role": "q0" }} , 
 	{ "name": "destbuffer15_value_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":12, "type": "signal", "bundle":{"name": "destbuffer15_value", "role": "address0" }} , 
 	{ "name": "destbuffer15_value_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "destbuffer15_value", "role": "ce0" }} , 
 	{ "name": "destbuffer15_value_we0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "destbuffer15_value", "role": "we0" }} , 
 	{ "name": "destbuffer15_value_d0", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "destbuffer15_value", "role": "d0" }} , 
 	{ "name": "destbuffer15_value_q0", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "destbuffer15_value", "role": "q0" }} , 
 	{ "name": "destbuffer16_value_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":12, "type": "signal", "bundle":{"name": "destbuffer16_value", "role": "address0" }} , 
 	{ "name": "destbuffer16_value_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "destbuffer16_value", "role": "ce0" }} , 
 	{ "name": "destbuffer16_value_we0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "destbuffer16_value", "role": "we0" }} , 
 	{ "name": "destbuffer16_value_d0", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "destbuffer16_value", "role": "d0" }} , 
 	{ "name": "destbuffer16_value_q0", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "destbuffer16_value", "role": "q0" }} , 
 	{ "name": "destbuffer17_value_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":12, "type": "signal", "bundle":{"name": "destbuffer17_value", "role": "address0" }} , 
 	{ "name": "destbuffer17_value_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "destbuffer17_value", "role": "ce0" }} , 
 	{ "name": "destbuffer17_value_we0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "destbuffer17_value", "role": "we0" }} , 
 	{ "name": "destbuffer17_value_d0", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "destbuffer17_value", "role": "d0" }} , 
 	{ "name": "destbuffer17_value_q0", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "destbuffer17_value", "role": "q0" }} , 
 	{ "name": "destbuffer20_value_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":12, "type": "signal", "bundle":{"name": "destbuffer20_value", "role": "address0" }} , 
 	{ "name": "destbuffer20_value_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "destbuffer20_value", "role": "ce0" }} , 
 	{ "name": "destbuffer20_value_we0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "destbuffer20_value", "role": "we0" }} , 
 	{ "name": "destbuffer20_value_d0", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "destbuffer20_value", "role": "d0" }} , 
 	{ "name": "destbuffer20_value_q0", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "destbuffer20_value", "role": "q0" }} , 
 	{ "name": "destbuffer21_value_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":12, "type": "signal", "bundle":{"name": "destbuffer21_value", "role": "address0" }} , 
 	{ "name": "destbuffer21_value_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "destbuffer21_value", "role": "ce0" }} , 
 	{ "name": "destbuffer21_value_we0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "destbuffer21_value", "role": "we0" }} , 
 	{ "name": "destbuffer21_value_d0", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "destbuffer21_value", "role": "d0" }} , 
 	{ "name": "destbuffer21_value_q0", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "destbuffer21_value", "role": "q0" }} , 
 	{ "name": "destbuffer22_value_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":12, "type": "signal", "bundle":{"name": "destbuffer22_value", "role": "address0" }} , 
 	{ "name": "destbuffer22_value_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "destbuffer22_value", "role": "ce0" }} , 
 	{ "name": "destbuffer22_value_we0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "destbuffer22_value", "role": "we0" }} , 
 	{ "name": "destbuffer22_value_d0", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "destbuffer22_value", "role": "d0" }} , 
 	{ "name": "destbuffer22_value_q0", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "destbuffer22_value", "role": "q0" }} , 
 	{ "name": "destbuffer23_value_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":12, "type": "signal", "bundle":{"name": "destbuffer23_value", "role": "address0" }} , 
 	{ "name": "destbuffer23_value_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "destbuffer23_value", "role": "ce0" }} , 
 	{ "name": "destbuffer23_value_we0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "destbuffer23_value", "role": "we0" }} , 
 	{ "name": "destbuffer23_value_d0", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "destbuffer23_value", "role": "d0" }} , 
 	{ "name": "destbuffer23_value_q0", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "destbuffer23_value", "role": "q0" }} , 
 	{ "name": "destbuffer24_value_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":12, "type": "signal", "bundle":{"name": "destbuffer24_value", "role": "address0" }} , 
 	{ "name": "destbuffer24_value_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "destbuffer24_value", "role": "ce0" }} , 
 	{ "name": "destbuffer24_value_we0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "destbuffer24_value", "role": "we0" }} , 
 	{ "name": "destbuffer24_value_d0", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "destbuffer24_value", "role": "d0" }} , 
 	{ "name": "destbuffer24_value_q0", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "destbuffer24_value", "role": "q0" }} , 
 	{ "name": "destbuffer25_value_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":12, "type": "signal", "bundle":{"name": "destbuffer25_value", "role": "address0" }} , 
 	{ "name": "destbuffer25_value_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "destbuffer25_value", "role": "ce0" }} , 
 	{ "name": "destbuffer25_value_we0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "destbuffer25_value", "role": "we0" }} , 
 	{ "name": "destbuffer25_value_d0", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "destbuffer25_value", "role": "d0" }} , 
 	{ "name": "destbuffer25_value_q0", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "destbuffer25_value", "role": "q0" }} , 
 	{ "name": "destbuffer26_value_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":12, "type": "signal", "bundle":{"name": "destbuffer26_value", "role": "address0" }} , 
 	{ "name": "destbuffer26_value_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "destbuffer26_value", "role": "ce0" }} , 
 	{ "name": "destbuffer26_value_we0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "destbuffer26_value", "role": "we0" }} , 
 	{ "name": "destbuffer26_value_d0", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "destbuffer26_value", "role": "d0" }} , 
 	{ "name": "destbuffer26_value_q0", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "destbuffer26_value", "role": "q0" }} , 
 	{ "name": "destbuffer27_value_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":12, "type": "signal", "bundle":{"name": "destbuffer27_value", "role": "address0" }} , 
 	{ "name": "destbuffer27_value_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "destbuffer27_value", "role": "ce0" }} , 
 	{ "name": "destbuffer27_value_we0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "destbuffer27_value", "role": "we0" }} , 
 	{ "name": "destbuffer27_value_d0", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "destbuffer27_value", "role": "d0" }} , 
 	{ "name": "destbuffer27_value_q0", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "destbuffer27_value", "role": "q0" }} , 
 	{ "name": "destbuffer30_value_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":12, "type": "signal", "bundle":{"name": "destbuffer30_value", "role": "address0" }} , 
 	{ "name": "destbuffer30_value_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "destbuffer30_value", "role": "ce0" }} , 
 	{ "name": "destbuffer30_value_we0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "destbuffer30_value", "role": "we0" }} , 
 	{ "name": "destbuffer30_value_d0", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "destbuffer30_value", "role": "d0" }} , 
 	{ "name": "destbuffer30_value_q0", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "destbuffer30_value", "role": "q0" }} , 
 	{ "name": "destbuffer31_value_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":12, "type": "signal", "bundle":{"name": "destbuffer31_value", "role": "address0" }} , 
 	{ "name": "destbuffer31_value_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "destbuffer31_value", "role": "ce0" }} , 
 	{ "name": "destbuffer31_value_we0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "destbuffer31_value", "role": "we0" }} , 
 	{ "name": "destbuffer31_value_d0", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "destbuffer31_value", "role": "d0" }} , 
 	{ "name": "destbuffer31_value_q0", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "destbuffer31_value", "role": "q0" }} , 
 	{ "name": "destbuffer32_value_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":12, "type": "signal", "bundle":{"name": "destbuffer32_value", "role": "address0" }} , 
 	{ "name": "destbuffer32_value_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "destbuffer32_value", "role": "ce0" }} , 
 	{ "name": "destbuffer32_value_we0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "destbuffer32_value", "role": "we0" }} , 
 	{ "name": "destbuffer32_value_d0", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "destbuffer32_value", "role": "d0" }} , 
 	{ "name": "destbuffer32_value_q0", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "destbuffer32_value", "role": "q0" }} , 
 	{ "name": "destbuffer33_value_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":12, "type": "signal", "bundle":{"name": "destbuffer33_value", "role": "address0" }} , 
 	{ "name": "destbuffer33_value_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "destbuffer33_value", "role": "ce0" }} , 
 	{ "name": "destbuffer33_value_we0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "destbuffer33_value", "role": "we0" }} , 
 	{ "name": "destbuffer33_value_d0", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "destbuffer33_value", "role": "d0" }} , 
 	{ "name": "destbuffer33_value_q0", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "destbuffer33_value", "role": "q0" }} , 
 	{ "name": "destbuffer34_value_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":12, "type": "signal", "bundle":{"name": "destbuffer34_value", "role": "address0" }} , 
 	{ "name": "destbuffer34_value_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "destbuffer34_value", "role": "ce0" }} , 
 	{ "name": "destbuffer34_value_we0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "destbuffer34_value", "role": "we0" }} , 
 	{ "name": "destbuffer34_value_d0", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "destbuffer34_value", "role": "d0" }} , 
 	{ "name": "destbuffer34_value_q0", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "destbuffer34_value", "role": "q0" }} , 
 	{ "name": "destbuffer35_value_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":12, "type": "signal", "bundle":{"name": "destbuffer35_value", "role": "address0" }} , 
 	{ "name": "destbuffer35_value_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "destbuffer35_value", "role": "ce0" }} , 
 	{ "name": "destbuffer35_value_we0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "destbuffer35_value", "role": "we0" }} , 
 	{ "name": "destbuffer35_value_d0", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "destbuffer35_value", "role": "d0" }} , 
 	{ "name": "destbuffer35_value_q0", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "destbuffer35_value", "role": "q0" }} , 
 	{ "name": "destbuffer36_value_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":12, "type": "signal", "bundle":{"name": "destbuffer36_value", "role": "address0" }} , 
 	{ "name": "destbuffer36_value_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "destbuffer36_value", "role": "ce0" }} , 
 	{ "name": "destbuffer36_value_we0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "destbuffer36_value", "role": "we0" }} , 
 	{ "name": "destbuffer36_value_d0", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "destbuffer36_value", "role": "d0" }} , 
 	{ "name": "destbuffer36_value_q0", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "destbuffer36_value", "role": "q0" }} , 
 	{ "name": "destbuffer37_value_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":12, "type": "signal", "bundle":{"name": "destbuffer37_value", "role": "address0" }} , 
 	{ "name": "destbuffer37_value_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "destbuffer37_value", "role": "ce0" }} , 
 	{ "name": "destbuffer37_value_we0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "destbuffer37_value", "role": "we0" }} , 
 	{ "name": "destbuffer37_value_d0", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "destbuffer37_value", "role": "d0" }} , 
 	{ "name": "destbuffer37_value_q0", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "destbuffer37_value", "role": "q0" }} , 
 	{ "name": "globalparams_vbegin", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "globalparams_vbegin", "role": "default" }}  ]}

set RtlHierarchyInfo {[
	{"ID" : "0", "Level" : "0", "Path" : "`AUTOTB_DUT_INST", "Parent" : "", "Child" : ["1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13", "14", "15", "16", "17", "18", "19", "20", "21", "22", "23", "24", "25", "26", "27", "28", "29", "30", "31", "32", "33", "34", "35", "36", "37", "38", "39", "40", "41", "42", "43", "44", "45", "46", "47", "48", "49", "50", "51", "52", "53", "54", "55", "56", "57", "58", "59", "60", "61", "62", "63", "64"],
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
	{"ID" : "1", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.topkernel_mux_83_32_1_1_U228", "Parent" : "0"},
	{"ID" : "2", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.topkernel_mux_83_32_1_1_U229", "Parent" : "0"},
	{"ID" : "3", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.topkernel_mux_83_32_1_1_U230", "Parent" : "0"},
	{"ID" : "4", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.topkernel_mux_83_32_1_1_U231", "Parent" : "0"},
	{"ID" : "5", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.topkernel_mux_83_32_1_1_U232", "Parent" : "0"},
	{"ID" : "6", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.topkernel_mux_83_32_1_1_U233", "Parent" : "0"},
	{"ID" : "7", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.topkernel_mux_83_32_1_1_U234", "Parent" : "0"},
	{"ID" : "8", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.topkernel_mux_83_32_1_1_U235", "Parent" : "0"},
	{"ID" : "9", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.topkernel_mux_83_32_1_1_U236", "Parent" : "0"},
	{"ID" : "10", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.topkernel_mux_83_32_1_1_U237", "Parent" : "0"},
	{"ID" : "11", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.topkernel_mux_83_32_1_1_U238", "Parent" : "0"},
	{"ID" : "12", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.topkernel_mux_83_32_1_1_U239", "Parent" : "0"},
	{"ID" : "13", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.topkernel_mux_83_32_1_1_U240", "Parent" : "0"},
	{"ID" : "14", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.topkernel_mux_83_32_1_1_U241", "Parent" : "0"},
	{"ID" : "15", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.topkernel_mux_83_32_1_1_U242", "Parent" : "0"},
	{"ID" : "16", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.topkernel_mux_83_32_1_1_U243", "Parent" : "0"},
	{"ID" : "17", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.topkernel_mux_83_32_1_1_U244", "Parent" : "0"},
	{"ID" : "18", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.topkernel_mux_83_32_1_1_U245", "Parent" : "0"},
	{"ID" : "19", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.topkernel_mux_83_32_1_1_U246", "Parent" : "0"},
	{"ID" : "20", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.topkernel_mux_83_32_1_1_U247", "Parent" : "0"},
	{"ID" : "21", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.topkernel_mux_83_32_1_1_U248", "Parent" : "0"},
	{"ID" : "22", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.topkernel_mux_83_32_1_1_U249", "Parent" : "0"},
	{"ID" : "23", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.topkernel_mux_83_32_1_1_U250", "Parent" : "0"},
	{"ID" : "24", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.topkernel_mux_83_32_1_1_U251", "Parent" : "0"},
	{"ID" : "25", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.topkernel_mux_83_32_1_1_U252", "Parent" : "0"},
	{"ID" : "26", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.topkernel_mux_83_32_1_1_U253", "Parent" : "0"},
	{"ID" : "27", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.topkernel_mux_83_32_1_1_U254", "Parent" : "0"},
	{"ID" : "28", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.topkernel_mux_83_32_1_1_U255", "Parent" : "0"},
	{"ID" : "29", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.topkernel_mux_83_32_1_1_U256", "Parent" : "0"},
	{"ID" : "30", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.topkernel_mux_83_32_1_1_U257", "Parent" : "0"},
	{"ID" : "31", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.topkernel_mux_83_32_1_1_U258", "Parent" : "0"},
	{"ID" : "32", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.topkernel_mux_83_32_1_1_U259", "Parent" : "0"},
	{"ID" : "33", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.topkernel_mux_83_32_1_1_U260", "Parent" : "0"},
	{"ID" : "34", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.topkernel_mux_83_32_1_1_U261", "Parent" : "0"},
	{"ID" : "35", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.topkernel_mux_83_32_1_1_U262", "Parent" : "0"},
	{"ID" : "36", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.topkernel_mux_83_32_1_1_U263", "Parent" : "0"},
	{"ID" : "37", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.topkernel_mux_83_32_1_1_U264", "Parent" : "0"},
	{"ID" : "38", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.topkernel_mux_83_32_1_1_U265", "Parent" : "0"},
	{"ID" : "39", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.topkernel_mux_83_32_1_1_U266", "Parent" : "0"},
	{"ID" : "40", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.topkernel_mux_83_32_1_1_U267", "Parent" : "0"},
	{"ID" : "41", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.topkernel_mux_83_32_1_1_U268", "Parent" : "0"},
	{"ID" : "42", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.topkernel_mux_83_32_1_1_U269", "Parent" : "0"},
	{"ID" : "43", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.topkernel_mux_83_32_1_1_U270", "Parent" : "0"},
	{"ID" : "44", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.topkernel_mux_83_32_1_1_U271", "Parent" : "0"},
	{"ID" : "45", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.topkernel_mux_83_32_1_1_U272", "Parent" : "0"},
	{"ID" : "46", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.topkernel_mux_83_32_1_1_U273", "Parent" : "0"},
	{"ID" : "47", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.topkernel_mux_83_32_1_1_U274", "Parent" : "0"},
	{"ID" : "48", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.topkernel_mux_83_32_1_1_U275", "Parent" : "0"},
	{"ID" : "49", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.topkernel_mux_83_32_1_1_U276", "Parent" : "0"},
	{"ID" : "50", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.topkernel_mux_83_32_1_1_U277", "Parent" : "0"},
	{"ID" : "51", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.topkernel_mux_83_32_1_1_U278", "Parent" : "0"},
	{"ID" : "52", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.topkernel_mux_83_32_1_1_U279", "Parent" : "0"},
	{"ID" : "53", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.topkernel_mux_83_32_1_1_U280", "Parent" : "0"},
	{"ID" : "54", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.topkernel_mux_83_32_1_1_U281", "Parent" : "0"},
	{"ID" : "55", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.topkernel_mux_83_32_1_1_U282", "Parent" : "0"},
	{"ID" : "56", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.topkernel_mux_83_32_1_1_U283", "Parent" : "0"},
	{"ID" : "57", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.topkernel_mux_83_32_1_1_U284", "Parent" : "0"},
	{"ID" : "58", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.topkernel_mux_83_32_1_1_U285", "Parent" : "0"},
	{"ID" : "59", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.topkernel_mux_83_32_1_1_U286", "Parent" : "0"},
	{"ID" : "60", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.topkernel_mux_83_32_1_1_U287", "Parent" : "0"},
	{"ID" : "61", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.topkernel_mux_83_32_1_1_U288", "Parent" : "0"},
	{"ID" : "62", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.topkernel_mux_83_32_1_1_U289", "Parent" : "0"},
	{"ID" : "63", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.topkernel_mux_83_32_1_1_U290", "Parent" : "0"},
	{"ID" : "64", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.topkernel_mux_83_32_1_1_U291", "Parent" : "0"}]}


set ArgLastReadFirstWriteLatency {
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
		globalparams_vbegin {Type I LastRead 0 FirstWrite -1}}}

set hasDtUnsupportedChannel 0

set PerformanceInfo {[
	{"Name" : "Latency", "Min" : "1", "Max" : "929"}
	, {"Name" : "Interval", "Min" : "1", "Max" : "929"}
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
	destbuffer00_value { ap_memory {  { destbuffer00_value_address0 mem_address 1 12 }  { destbuffer00_value_ce0 mem_ce 1 1 }  { destbuffer00_value_we0 mem_we 1 1 }  { destbuffer00_value_d0 mem_din 1 32 }  { destbuffer00_value_q0 mem_dout 0 32 } } }
	destbuffer01_value { ap_memory {  { destbuffer01_value_address0 mem_address 1 12 }  { destbuffer01_value_ce0 mem_ce 1 1 }  { destbuffer01_value_we0 mem_we 1 1 }  { destbuffer01_value_d0 mem_din 1 32 }  { destbuffer01_value_q0 mem_dout 0 32 } } }
	destbuffer02_value { ap_memory {  { destbuffer02_value_address0 mem_address 1 12 }  { destbuffer02_value_ce0 mem_ce 1 1 }  { destbuffer02_value_we0 mem_we 1 1 }  { destbuffer02_value_d0 mem_din 1 32 }  { destbuffer02_value_q0 mem_dout 0 32 } } }
	destbuffer03_value { ap_memory {  { destbuffer03_value_address0 mem_address 1 12 }  { destbuffer03_value_ce0 mem_ce 1 1 }  { destbuffer03_value_we0 mem_we 1 1 }  { destbuffer03_value_d0 mem_din 1 32 }  { destbuffer03_value_q0 mem_dout 0 32 } } }
	destbuffer04_value { ap_memory {  { destbuffer04_value_address0 mem_address 1 12 }  { destbuffer04_value_ce0 mem_ce 1 1 }  { destbuffer04_value_we0 mem_we 1 1 }  { destbuffer04_value_d0 mem_din 1 32 }  { destbuffer04_value_q0 mem_dout 0 32 } } }
	destbuffer05_value { ap_memory {  { destbuffer05_value_address0 mem_address 1 12 }  { destbuffer05_value_ce0 mem_ce 1 1 }  { destbuffer05_value_we0 mem_we 1 1 }  { destbuffer05_value_d0 mem_din 1 32 }  { destbuffer05_value_q0 mem_dout 0 32 } } }
	destbuffer06_value { ap_memory {  { destbuffer06_value_address0 mem_address 1 12 }  { destbuffer06_value_ce0 mem_ce 1 1 }  { destbuffer06_value_we0 mem_we 1 1 }  { destbuffer06_value_d0 mem_din 1 32 }  { destbuffer06_value_q0 mem_dout 0 32 } } }
	destbuffer07_value { ap_memory {  { destbuffer07_value_address0 mem_address 1 12 }  { destbuffer07_value_ce0 mem_ce 1 1 }  { destbuffer07_value_we0 mem_we 1 1 }  { destbuffer07_value_d0 mem_din 1 32 }  { destbuffer07_value_q0 mem_dout 0 32 } } }
	destbuffer10_value { ap_memory {  { destbuffer10_value_address0 mem_address 1 12 }  { destbuffer10_value_ce0 mem_ce 1 1 }  { destbuffer10_value_we0 mem_we 1 1 }  { destbuffer10_value_d0 mem_din 1 32 }  { destbuffer10_value_q0 mem_dout 0 32 } } }
	destbuffer11_value { ap_memory {  { destbuffer11_value_address0 mem_address 1 12 }  { destbuffer11_value_ce0 mem_ce 1 1 }  { destbuffer11_value_we0 mem_we 1 1 }  { destbuffer11_value_d0 mem_din 1 32 }  { destbuffer11_value_q0 mem_dout 0 32 } } }
	destbuffer12_value { ap_memory {  { destbuffer12_value_address0 mem_address 1 12 }  { destbuffer12_value_ce0 mem_ce 1 1 }  { destbuffer12_value_we0 mem_we 1 1 }  { destbuffer12_value_d0 mem_din 1 32 }  { destbuffer12_value_q0 mem_dout 0 32 } } }
	destbuffer13_value { ap_memory {  { destbuffer13_value_address0 mem_address 1 12 }  { destbuffer13_value_ce0 mem_ce 1 1 }  { destbuffer13_value_we0 mem_we 1 1 }  { destbuffer13_value_d0 mem_din 1 32 }  { destbuffer13_value_q0 mem_dout 0 32 } } }
	destbuffer14_value { ap_memory {  { destbuffer14_value_address0 mem_address 1 12 }  { destbuffer14_value_ce0 mem_ce 1 1 }  { destbuffer14_value_we0 mem_we 1 1 }  { destbuffer14_value_d0 mem_din 1 32 }  { destbuffer14_value_q0 mem_dout 0 32 } } }
	destbuffer15_value { ap_memory {  { destbuffer15_value_address0 mem_address 1 12 }  { destbuffer15_value_ce0 mem_ce 1 1 }  { destbuffer15_value_we0 mem_we 1 1 }  { destbuffer15_value_d0 mem_din 1 32 }  { destbuffer15_value_q0 mem_dout 0 32 } } }
	destbuffer16_value { ap_memory {  { destbuffer16_value_address0 mem_address 1 12 }  { destbuffer16_value_ce0 mem_ce 1 1 }  { destbuffer16_value_we0 mem_we 1 1 }  { destbuffer16_value_d0 mem_din 1 32 }  { destbuffer16_value_q0 mem_dout 0 32 } } }
	destbuffer17_value { ap_memory {  { destbuffer17_value_address0 mem_address 1 12 }  { destbuffer17_value_ce0 mem_ce 1 1 }  { destbuffer17_value_we0 mem_we 1 1 }  { destbuffer17_value_d0 mem_din 1 32 }  { destbuffer17_value_q0 mem_dout 0 32 } } }
	destbuffer20_value { ap_memory {  { destbuffer20_value_address0 mem_address 1 12 }  { destbuffer20_value_ce0 mem_ce 1 1 }  { destbuffer20_value_we0 mem_we 1 1 }  { destbuffer20_value_d0 mem_din 1 32 }  { destbuffer20_value_q0 mem_dout 0 32 } } }
	destbuffer21_value { ap_memory {  { destbuffer21_value_address0 mem_address 1 12 }  { destbuffer21_value_ce0 mem_ce 1 1 }  { destbuffer21_value_we0 mem_we 1 1 }  { destbuffer21_value_d0 mem_din 1 32 }  { destbuffer21_value_q0 mem_dout 0 32 } } }
	destbuffer22_value { ap_memory {  { destbuffer22_value_address0 mem_address 1 12 }  { destbuffer22_value_ce0 mem_ce 1 1 }  { destbuffer22_value_we0 mem_we 1 1 }  { destbuffer22_value_d0 mem_din 1 32 }  { destbuffer22_value_q0 mem_dout 0 32 } } }
	destbuffer23_value { ap_memory {  { destbuffer23_value_address0 mem_address 1 12 }  { destbuffer23_value_ce0 mem_ce 1 1 }  { destbuffer23_value_we0 mem_we 1 1 }  { destbuffer23_value_d0 mem_din 1 32 }  { destbuffer23_value_q0 mem_dout 0 32 } } }
	destbuffer24_value { ap_memory {  { destbuffer24_value_address0 mem_address 1 12 }  { destbuffer24_value_ce0 mem_ce 1 1 }  { destbuffer24_value_we0 mem_we 1 1 }  { destbuffer24_value_d0 mem_din 1 32 }  { destbuffer24_value_q0 mem_dout 0 32 } } }
	destbuffer25_value { ap_memory {  { destbuffer25_value_address0 mem_address 1 12 }  { destbuffer25_value_ce0 mem_ce 1 1 }  { destbuffer25_value_we0 mem_we 1 1 }  { destbuffer25_value_d0 mem_din 1 32 }  { destbuffer25_value_q0 mem_dout 0 32 } } }
	destbuffer26_value { ap_memory {  { destbuffer26_value_address0 mem_address 1 12 }  { destbuffer26_value_ce0 mem_ce 1 1 }  { destbuffer26_value_we0 mem_we 1 1 }  { destbuffer26_value_d0 mem_din 1 32 }  { destbuffer26_value_q0 mem_dout 0 32 } } }
	destbuffer27_value { ap_memory {  { destbuffer27_value_address0 mem_address 1 12 }  { destbuffer27_value_ce0 mem_ce 1 1 }  { destbuffer27_value_we0 mem_we 1 1 }  { destbuffer27_value_d0 mem_din 1 32 }  { destbuffer27_value_q0 mem_dout 0 32 } } }
	destbuffer30_value { ap_memory {  { destbuffer30_value_address0 mem_address 1 12 }  { destbuffer30_value_ce0 mem_ce 1 1 }  { destbuffer30_value_we0 mem_we 1 1 }  { destbuffer30_value_d0 mem_din 1 32 }  { destbuffer30_value_q0 mem_dout 0 32 } } }
	destbuffer31_value { ap_memory {  { destbuffer31_value_address0 mem_address 1 12 }  { destbuffer31_value_ce0 mem_ce 1 1 }  { destbuffer31_value_we0 mem_we 1 1 }  { destbuffer31_value_d0 mem_din 1 32 }  { destbuffer31_value_q0 mem_dout 0 32 } } }
	destbuffer32_value { ap_memory {  { destbuffer32_value_address0 mem_address 1 12 }  { destbuffer32_value_ce0 mem_ce 1 1 }  { destbuffer32_value_we0 mem_we 1 1 }  { destbuffer32_value_d0 mem_din 1 32 }  { destbuffer32_value_q0 mem_dout 0 32 } } }
	destbuffer33_value { ap_memory {  { destbuffer33_value_address0 mem_address 1 12 }  { destbuffer33_value_ce0 mem_ce 1 1 }  { destbuffer33_value_we0 mem_we 1 1 }  { destbuffer33_value_d0 mem_din 1 32 }  { destbuffer33_value_q0 mem_dout 0 32 } } }
	destbuffer34_value { ap_memory {  { destbuffer34_value_address0 mem_address 1 12 }  { destbuffer34_value_ce0 mem_ce 1 1 }  { destbuffer34_value_we0 mem_we 1 1 }  { destbuffer34_value_d0 mem_din 1 32 }  { destbuffer34_value_q0 mem_dout 0 32 } } }
	destbuffer35_value { ap_memory {  { destbuffer35_value_address0 mem_address 1 12 }  { destbuffer35_value_ce0 mem_ce 1 1 }  { destbuffer35_value_we0 mem_we 1 1 }  { destbuffer35_value_d0 mem_din 1 32 }  { destbuffer35_value_q0 mem_dout 0 32 } } }
	destbuffer36_value { ap_memory {  { destbuffer36_value_address0 mem_address 1 12 }  { destbuffer36_value_ce0 mem_ce 1 1 }  { destbuffer36_value_we0 mem_we 1 1 }  { destbuffer36_value_d0 mem_din 1 32 }  { destbuffer36_value_q0 mem_dout 0 32 } } }
	destbuffer37_value { ap_memory {  { destbuffer37_value_address0 mem_address 1 12 }  { destbuffer37_value_ce0 mem_ce 1 1 }  { destbuffer37_value_we0 mem_we 1 1 }  { destbuffer37_value_d0 mem_din 1 32 }  { destbuffer37_value_q0 mem_dout 0 32 } } }
	globalparams_vbegin { ap_none {  { globalparams_vbegin in_data 0 32 } } }
}
