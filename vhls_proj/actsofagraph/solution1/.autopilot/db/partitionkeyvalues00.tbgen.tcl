set moduleName partitionkeyvalues00
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
set C_modelName {partitionkeyvalues00}
set C_modelType { void 0 }
set C_modelArgList {
	{ enable int 1 regular  }
	{ sourcebuffer0_V int 512 regular {array 512 { 1 3 } 1 1 }  }
	{ sourcebuffer1_V int 512 regular {array 512 { 1 3 } 1 1 }  }
	{ sourcebuffer2_V int 512 regular {array 512 { 1 3 } 1 1 }  }
	{ sourcebuffer3_V int 512 regular {array 512 { 1 3 } 1 1 }  }
	{ destbuffer0_V int 512 regular {array 2112 { 0 3 } 0 1 }  }
	{ destbuffer1_V int 512 regular {array 2112 { 0 3 } 0 1 }  }
	{ destbuffer2_V int 512 regular {array 2112 { 0 3 } 0 1 }  }
	{ destbuffer3_V int 512 regular {array 2112 { 0 3 } 0 1 }  }
	{ capsule0_key int 32 regular {array 32 { 1 3 } 1 1 }  }
	{ capsule0_value int 32 regular {array 32 { 2 3 } 1 1 }  }
	{ capsule1_key int 32 regular {array 32 { 1 3 } 1 1 }  }
	{ capsule1_value int 32 regular {array 32 { 2 3 } 1 1 }  }
	{ capsule2_key int 32 regular {array 32 { 1 3 } 1 1 }  }
	{ capsule2_value int 32 regular {array 32 { 2 3 } 1 1 }  }
	{ capsule3_key int 32 regular {array 32 { 1 3 } 1 1 }  }
	{ capsule3_value int 32 regular {array 32 { 2 3 } 1 1 }  }
	{ travstate_i_kvs int 32 regular  }
	{ travstate_end_kvs int 30 regular  }
	{ llopparams_currentLOP int 32 regular  }
	{ llopparams_upperlimit int 32 regular  }
}
set C_modelArgMapList {[ 
	{ "Name" : "enable", "interface" : "wire", "bitwidth" : 1, "direction" : "READONLY"} , 
 	{ "Name" : "sourcebuffer0_V", "interface" : "memory", "bitwidth" : 512, "direction" : "READONLY"} , 
 	{ "Name" : "sourcebuffer1_V", "interface" : "memory", "bitwidth" : 512, "direction" : "READONLY"} , 
 	{ "Name" : "sourcebuffer2_V", "interface" : "memory", "bitwidth" : 512, "direction" : "READONLY"} , 
 	{ "Name" : "sourcebuffer3_V", "interface" : "memory", "bitwidth" : 512, "direction" : "READONLY"} , 
 	{ "Name" : "destbuffer0_V", "interface" : "memory", "bitwidth" : 512, "direction" : "WRITEONLY"} , 
 	{ "Name" : "destbuffer1_V", "interface" : "memory", "bitwidth" : 512, "direction" : "WRITEONLY"} , 
 	{ "Name" : "destbuffer2_V", "interface" : "memory", "bitwidth" : 512, "direction" : "WRITEONLY"} , 
 	{ "Name" : "destbuffer3_V", "interface" : "memory", "bitwidth" : 512, "direction" : "WRITEONLY"} , 
 	{ "Name" : "capsule0_key", "interface" : "memory", "bitwidth" : 32, "direction" : "READONLY"} , 
 	{ "Name" : "capsule0_value", "interface" : "memory", "bitwidth" : 32, "direction" : "READWRITE"} , 
 	{ "Name" : "capsule1_key", "interface" : "memory", "bitwidth" : 32, "direction" : "READONLY"} , 
 	{ "Name" : "capsule1_value", "interface" : "memory", "bitwidth" : 32, "direction" : "READWRITE"} , 
 	{ "Name" : "capsule2_key", "interface" : "memory", "bitwidth" : 32, "direction" : "READONLY"} , 
 	{ "Name" : "capsule2_value", "interface" : "memory", "bitwidth" : 32, "direction" : "READWRITE"} , 
 	{ "Name" : "capsule3_key", "interface" : "memory", "bitwidth" : 32, "direction" : "READONLY"} , 
 	{ "Name" : "capsule3_value", "interface" : "memory", "bitwidth" : 32, "direction" : "READWRITE"} , 
 	{ "Name" : "travstate_i_kvs", "interface" : "wire", "bitwidth" : 32, "direction" : "READONLY"} , 
 	{ "Name" : "travstate_end_kvs", "interface" : "wire", "bitwidth" : 30, "direction" : "READONLY"} , 
 	{ "Name" : "llopparams_currentLOP", "interface" : "wire", "bitwidth" : 32, "direction" : "READONLY"} , 
 	{ "Name" : "llopparams_upperlimit", "interface" : "wire", "bitwidth" : 32, "direction" : "READONLY"} ]}
# RTL Port declarations: 
set portNum 71
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
	{ destbuffer1_V_address0 sc_out sc_lv 12 signal 6 } 
	{ destbuffer1_V_ce0 sc_out sc_logic 1 signal 6 } 
	{ destbuffer1_V_we0 sc_out sc_lv 64 signal 6 } 
	{ destbuffer1_V_d0 sc_out sc_lv 512 signal 6 } 
	{ destbuffer2_V_address0 sc_out sc_lv 12 signal 7 } 
	{ destbuffer2_V_ce0 sc_out sc_logic 1 signal 7 } 
	{ destbuffer2_V_we0 sc_out sc_lv 64 signal 7 } 
	{ destbuffer2_V_d0 sc_out sc_lv 512 signal 7 } 
	{ destbuffer3_V_address0 sc_out sc_lv 12 signal 8 } 
	{ destbuffer3_V_ce0 sc_out sc_logic 1 signal 8 } 
	{ destbuffer3_V_we0 sc_out sc_lv 64 signal 8 } 
	{ destbuffer3_V_d0 sc_out sc_lv 512 signal 8 } 
	{ capsule0_key_address0 sc_out sc_lv 5 signal 9 } 
	{ capsule0_key_ce0 sc_out sc_logic 1 signal 9 } 
	{ capsule0_key_q0 sc_in sc_lv 32 signal 9 } 
	{ capsule0_value_address0 sc_out sc_lv 5 signal 10 } 
	{ capsule0_value_ce0 sc_out sc_logic 1 signal 10 } 
	{ capsule0_value_we0 sc_out sc_logic 1 signal 10 } 
	{ capsule0_value_d0 sc_out sc_lv 32 signal 10 } 
	{ capsule0_value_q0 sc_in sc_lv 32 signal 10 } 
	{ capsule1_key_address0 sc_out sc_lv 5 signal 11 } 
	{ capsule1_key_ce0 sc_out sc_logic 1 signal 11 } 
	{ capsule1_key_q0 sc_in sc_lv 32 signal 11 } 
	{ capsule1_value_address0 sc_out sc_lv 5 signal 12 } 
	{ capsule1_value_ce0 sc_out sc_logic 1 signal 12 } 
	{ capsule1_value_we0 sc_out sc_logic 1 signal 12 } 
	{ capsule1_value_d0 sc_out sc_lv 32 signal 12 } 
	{ capsule1_value_q0 sc_in sc_lv 32 signal 12 } 
	{ capsule2_key_address0 sc_out sc_lv 5 signal 13 } 
	{ capsule2_key_ce0 sc_out sc_logic 1 signal 13 } 
	{ capsule2_key_q0 sc_in sc_lv 32 signal 13 } 
	{ capsule2_value_address0 sc_out sc_lv 5 signal 14 } 
	{ capsule2_value_ce0 sc_out sc_logic 1 signal 14 } 
	{ capsule2_value_we0 sc_out sc_logic 1 signal 14 } 
	{ capsule2_value_d0 sc_out sc_lv 32 signal 14 } 
	{ capsule2_value_q0 sc_in sc_lv 32 signal 14 } 
	{ capsule3_key_address0 sc_out sc_lv 5 signal 15 } 
	{ capsule3_key_ce0 sc_out sc_logic 1 signal 15 } 
	{ capsule3_key_q0 sc_in sc_lv 32 signal 15 } 
	{ capsule3_value_address0 sc_out sc_lv 5 signal 16 } 
	{ capsule3_value_ce0 sc_out sc_logic 1 signal 16 } 
	{ capsule3_value_we0 sc_out sc_logic 1 signal 16 } 
	{ capsule3_value_d0 sc_out sc_lv 32 signal 16 } 
	{ capsule3_value_q0 sc_in sc_lv 32 signal 16 } 
	{ travstate_i_kvs sc_in sc_lv 32 signal 17 } 
	{ travstate_end_kvs sc_in sc_lv 30 signal 18 } 
	{ llopparams_currentLOP sc_in sc_lv 32 signal 19 } 
	{ llopparams_upperlimit sc_in sc_lv 32 signal 20 } 
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
 	{ "name": "destbuffer1_V_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":12, "type": "signal", "bundle":{"name": "destbuffer1_V", "role": "address0" }} , 
 	{ "name": "destbuffer1_V_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "destbuffer1_V", "role": "ce0" }} , 
 	{ "name": "destbuffer1_V_we0", "direction": "out", "datatype": "sc_lv", "bitwidth":64, "type": "signal", "bundle":{"name": "destbuffer1_V", "role": "we0" }} , 
 	{ "name": "destbuffer1_V_d0", "direction": "out", "datatype": "sc_lv", "bitwidth":512, "type": "signal", "bundle":{"name": "destbuffer1_V", "role": "d0" }} , 
 	{ "name": "destbuffer2_V_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":12, "type": "signal", "bundle":{"name": "destbuffer2_V", "role": "address0" }} , 
 	{ "name": "destbuffer2_V_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "destbuffer2_V", "role": "ce0" }} , 
 	{ "name": "destbuffer2_V_we0", "direction": "out", "datatype": "sc_lv", "bitwidth":64, "type": "signal", "bundle":{"name": "destbuffer2_V", "role": "we0" }} , 
 	{ "name": "destbuffer2_V_d0", "direction": "out", "datatype": "sc_lv", "bitwidth":512, "type": "signal", "bundle":{"name": "destbuffer2_V", "role": "d0" }} , 
 	{ "name": "destbuffer3_V_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":12, "type": "signal", "bundle":{"name": "destbuffer3_V", "role": "address0" }} , 
 	{ "name": "destbuffer3_V_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "destbuffer3_V", "role": "ce0" }} , 
 	{ "name": "destbuffer3_V_we0", "direction": "out", "datatype": "sc_lv", "bitwidth":64, "type": "signal", "bundle":{"name": "destbuffer3_V", "role": "we0" }} , 
 	{ "name": "destbuffer3_V_d0", "direction": "out", "datatype": "sc_lv", "bitwidth":512, "type": "signal", "bundle":{"name": "destbuffer3_V", "role": "d0" }} , 
 	{ "name": "capsule0_key_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":5, "type": "signal", "bundle":{"name": "capsule0_key", "role": "address0" }} , 
 	{ "name": "capsule0_key_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "capsule0_key", "role": "ce0" }} , 
 	{ "name": "capsule0_key_q0", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "capsule0_key", "role": "q0" }} , 
 	{ "name": "capsule0_value_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":5, "type": "signal", "bundle":{"name": "capsule0_value", "role": "address0" }} , 
 	{ "name": "capsule0_value_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "capsule0_value", "role": "ce0" }} , 
 	{ "name": "capsule0_value_we0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "capsule0_value", "role": "we0" }} , 
 	{ "name": "capsule0_value_d0", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "capsule0_value", "role": "d0" }} , 
 	{ "name": "capsule0_value_q0", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "capsule0_value", "role": "q0" }} , 
 	{ "name": "capsule1_key_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":5, "type": "signal", "bundle":{"name": "capsule1_key", "role": "address0" }} , 
 	{ "name": "capsule1_key_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "capsule1_key", "role": "ce0" }} , 
 	{ "name": "capsule1_key_q0", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "capsule1_key", "role": "q0" }} , 
 	{ "name": "capsule1_value_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":5, "type": "signal", "bundle":{"name": "capsule1_value", "role": "address0" }} , 
 	{ "name": "capsule1_value_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "capsule1_value", "role": "ce0" }} , 
 	{ "name": "capsule1_value_we0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "capsule1_value", "role": "we0" }} , 
 	{ "name": "capsule1_value_d0", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "capsule1_value", "role": "d0" }} , 
 	{ "name": "capsule1_value_q0", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "capsule1_value", "role": "q0" }} , 
 	{ "name": "capsule2_key_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":5, "type": "signal", "bundle":{"name": "capsule2_key", "role": "address0" }} , 
 	{ "name": "capsule2_key_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "capsule2_key", "role": "ce0" }} , 
 	{ "name": "capsule2_key_q0", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "capsule2_key", "role": "q0" }} , 
 	{ "name": "capsule2_value_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":5, "type": "signal", "bundle":{"name": "capsule2_value", "role": "address0" }} , 
 	{ "name": "capsule2_value_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "capsule2_value", "role": "ce0" }} , 
 	{ "name": "capsule2_value_we0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "capsule2_value", "role": "we0" }} , 
 	{ "name": "capsule2_value_d0", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "capsule2_value", "role": "d0" }} , 
 	{ "name": "capsule2_value_q0", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "capsule2_value", "role": "q0" }} , 
 	{ "name": "capsule3_key_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":5, "type": "signal", "bundle":{"name": "capsule3_key", "role": "address0" }} , 
 	{ "name": "capsule3_key_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "capsule3_key", "role": "ce0" }} , 
 	{ "name": "capsule3_key_q0", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "capsule3_key", "role": "q0" }} , 
 	{ "name": "capsule3_value_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":5, "type": "signal", "bundle":{"name": "capsule3_value", "role": "address0" }} , 
 	{ "name": "capsule3_value_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "capsule3_value", "role": "ce0" }} , 
 	{ "name": "capsule3_value_we0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "capsule3_value", "role": "we0" }} , 
 	{ "name": "capsule3_value_d0", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "capsule3_value", "role": "d0" }} , 
 	{ "name": "capsule3_value_q0", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "capsule3_value", "role": "q0" }} , 
 	{ "name": "travstate_i_kvs", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "travstate_i_kvs", "role": "default" }} , 
 	{ "name": "travstate_end_kvs", "direction": "in", "datatype": "sc_lv", "bitwidth":30, "type": "signal", "bundle":{"name": "travstate_end_kvs", "role": "default" }} , 
 	{ "name": "llopparams_currentLOP", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "llopparams_currentLOP", "role": "default" }} , 
 	{ "name": "llopparams_upperlimit", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "llopparams_upperlimit", "role": "default" }}  ]}

set RtlHierarchyInfo {[
	{"ID" : "0", "Level" : "0", "Path" : "`AUTOTB_DUT_INST", "Parent" : "", "Child" : ["1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12"],
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
	{"ID" : "1", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.topkernel_mux_325_32_1_1_U85", "Parent" : "0"},
	{"ID" : "2", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.topkernel_mux_325_32_1_1_U86", "Parent" : "0"},
	{"ID" : "3", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.topkernel_mux_325_32_1_1_U87", "Parent" : "0"},
	{"ID" : "4", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.topkernel_mux_325_32_1_1_U88", "Parent" : "0"},
	{"ID" : "5", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.topkernel_mux_325_32_1_1_U89", "Parent" : "0"},
	{"ID" : "6", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.topkernel_mux_325_32_1_1_U90", "Parent" : "0"},
	{"ID" : "7", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.topkernel_mux_325_32_1_1_U91", "Parent" : "0"},
	{"ID" : "8", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.topkernel_mux_325_32_1_1_U92", "Parent" : "0"},
	{"ID" : "9", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.topkernel_mux_325_32_1_1_U93", "Parent" : "0"},
	{"ID" : "10", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.topkernel_mux_325_32_1_1_U94", "Parent" : "0"},
	{"ID" : "11", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.topkernel_mux_325_32_1_1_U95", "Parent" : "0"},
	{"ID" : "12", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.topkernel_mux_325_32_1_1_U96", "Parent" : "0"}]}


set ArgLastReadFirstWriteLatency {
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
		llopparams_upperlimit {Type I LastRead 0 FirstWrite -1}}}

set hasDtUnsupportedChannel 0

set PerformanceInfo {[
	{"Name" : "Latency", "Min" : "1", "Max" : "4228"}
	, {"Name" : "Interval", "Min" : "1", "Max" : "4228"}
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
	destbuffer0_V { ap_memory {  { destbuffer0_V_address0 mem_address 1 12 }  { destbuffer0_V_ce0 mem_ce 1 1 }  { destbuffer0_V_we0 mem_we 1 64 }  { destbuffer0_V_d0 mem_din 1 512 } } }
	destbuffer1_V { ap_memory {  { destbuffer1_V_address0 mem_address 1 12 }  { destbuffer1_V_ce0 mem_ce 1 1 }  { destbuffer1_V_we0 mem_we 1 64 }  { destbuffer1_V_d0 mem_din 1 512 } } }
	destbuffer2_V { ap_memory {  { destbuffer2_V_address0 mem_address 1 12 }  { destbuffer2_V_ce0 mem_ce 1 1 }  { destbuffer2_V_we0 mem_we 1 64 }  { destbuffer2_V_d0 mem_din 1 512 } } }
	destbuffer3_V { ap_memory {  { destbuffer3_V_address0 mem_address 1 12 }  { destbuffer3_V_ce0 mem_ce 1 1 }  { destbuffer3_V_we0 mem_we 1 64 }  { destbuffer3_V_d0 mem_din 1 512 } } }
	capsule0_key { ap_memory {  { capsule0_key_address0 mem_address 1 5 }  { capsule0_key_ce0 mem_ce 1 1 }  { capsule0_key_q0 mem_dout 0 32 } } }
	capsule0_value { ap_memory {  { capsule0_value_address0 mem_address 1 5 }  { capsule0_value_ce0 mem_ce 1 1 }  { capsule0_value_we0 mem_we 1 1 }  { capsule0_value_d0 mem_din 1 32 }  { capsule0_value_q0 mem_dout 0 32 } } }
	capsule1_key { ap_memory {  { capsule1_key_address0 mem_address 1 5 }  { capsule1_key_ce0 mem_ce 1 1 }  { capsule1_key_q0 mem_dout 0 32 } } }
	capsule1_value { ap_memory {  { capsule1_value_address0 mem_address 1 5 }  { capsule1_value_ce0 mem_ce 1 1 }  { capsule1_value_we0 mem_we 1 1 }  { capsule1_value_d0 mem_din 1 32 }  { capsule1_value_q0 mem_dout 0 32 } } }
	capsule2_key { ap_memory {  { capsule2_key_address0 mem_address 1 5 }  { capsule2_key_ce0 mem_ce 1 1 }  { capsule2_key_q0 mem_dout 0 32 } } }
	capsule2_value { ap_memory {  { capsule2_value_address0 mem_address 1 5 }  { capsule2_value_ce0 mem_ce 1 1 }  { capsule2_value_we0 mem_we 1 1 }  { capsule2_value_d0 mem_din 1 32 }  { capsule2_value_q0 mem_dout 0 32 } } }
	capsule3_key { ap_memory {  { capsule3_key_address0 mem_address 1 5 }  { capsule3_key_ce0 mem_ce 1 1 }  { capsule3_key_q0 mem_dout 0 32 } } }
	capsule3_value { ap_memory {  { capsule3_value_address0 mem_address 1 5 }  { capsule3_value_ce0 mem_ce 1 1 }  { capsule3_value_we0 mem_we 1 1 }  { capsule3_value_d0 mem_din 1 32 }  { capsule3_value_q0 mem_dout 0 32 } } }
	travstate_i_kvs { ap_none {  { travstate_i_kvs in_data 0 32 } } }
	travstate_end_kvs { ap_none {  { travstate_end_kvs in_data 0 30 } } }
	llopparams_currentLOP { ap_none {  { llopparams_currentLOP in_data 0 32 } } }
	llopparams_upperlimit { ap_none {  { llopparams_upperlimit in_data 0 32 } } }
}
set moduleName partitionkeyvalues00
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
set C_modelName {partitionkeyvalues00}
set C_modelType { void 0 }
set C_modelArgList {
	{ enable int 1 regular  }
	{ sourcebuffer0_V int 512 regular {array 512 { 1 3 } 1 1 }  }
	{ sourcebuffer1_V int 512 regular {array 512 { 1 3 } 1 1 }  }
	{ sourcebuffer2_V int 512 regular {array 512 { 1 3 } 1 1 }  }
	{ sourcebuffer3_V int 512 regular {array 512 { 1 3 } 1 1 }  }
	{ destbuffer0_V int 512 regular {array 2112 { 0 3 } 0 1 }  }
	{ destbuffer1_V int 512 regular {array 2112 { 0 3 } 0 1 }  }
	{ destbuffer2_V int 512 regular {array 2112 { 0 3 } 0 1 }  }
	{ destbuffer3_V int 512 regular {array 2112 { 0 3 } 0 1 }  }
	{ capsule0_key int 32 regular {array 32 { 1 3 } 1 1 }  }
	{ capsule0_value int 32 regular {array 32 { 2 3 } 1 1 }  }
	{ capsule1_key int 32 regular {array 32 { 1 3 } 1 1 }  }
	{ capsule1_value int 32 regular {array 32 { 2 3 } 1 1 }  }
	{ capsule2_key int 32 regular {array 32 { 1 3 } 1 1 }  }
	{ capsule2_value int 32 regular {array 32 { 2 3 } 1 1 }  }
	{ capsule3_key int 32 regular {array 32 { 1 3 } 1 1 }  }
	{ capsule3_value int 32 regular {array 32 { 2 3 } 1 1 }  }
	{ travstate_i_kvs int 32 regular  }
	{ travstate_end_kvs int 30 regular  }
	{ llopparams_currentLOP int 32 regular  }
	{ llopparams_upperlimit int 32 regular  }
}
set C_modelArgMapList {[ 
	{ "Name" : "enable", "interface" : "wire", "bitwidth" : 1, "direction" : "READONLY"} , 
 	{ "Name" : "sourcebuffer0_V", "interface" : "memory", "bitwidth" : 512, "direction" : "READONLY"} , 
 	{ "Name" : "sourcebuffer1_V", "interface" : "memory", "bitwidth" : 512, "direction" : "READONLY"} , 
 	{ "Name" : "sourcebuffer2_V", "interface" : "memory", "bitwidth" : 512, "direction" : "READONLY"} , 
 	{ "Name" : "sourcebuffer3_V", "interface" : "memory", "bitwidth" : 512, "direction" : "READONLY"} , 
 	{ "Name" : "destbuffer0_V", "interface" : "memory", "bitwidth" : 512, "direction" : "WRITEONLY"} , 
 	{ "Name" : "destbuffer1_V", "interface" : "memory", "bitwidth" : 512, "direction" : "WRITEONLY"} , 
 	{ "Name" : "destbuffer2_V", "interface" : "memory", "bitwidth" : 512, "direction" : "WRITEONLY"} , 
 	{ "Name" : "destbuffer3_V", "interface" : "memory", "bitwidth" : 512, "direction" : "WRITEONLY"} , 
 	{ "Name" : "capsule0_key", "interface" : "memory", "bitwidth" : 32, "direction" : "READONLY"} , 
 	{ "Name" : "capsule0_value", "interface" : "memory", "bitwidth" : 32, "direction" : "READWRITE"} , 
 	{ "Name" : "capsule1_key", "interface" : "memory", "bitwidth" : 32, "direction" : "READONLY"} , 
 	{ "Name" : "capsule1_value", "interface" : "memory", "bitwidth" : 32, "direction" : "READWRITE"} , 
 	{ "Name" : "capsule2_key", "interface" : "memory", "bitwidth" : 32, "direction" : "READONLY"} , 
 	{ "Name" : "capsule2_value", "interface" : "memory", "bitwidth" : 32, "direction" : "READWRITE"} , 
 	{ "Name" : "capsule3_key", "interface" : "memory", "bitwidth" : 32, "direction" : "READONLY"} , 
 	{ "Name" : "capsule3_value", "interface" : "memory", "bitwidth" : 32, "direction" : "READWRITE"} , 
 	{ "Name" : "travstate_i_kvs", "interface" : "wire", "bitwidth" : 32, "direction" : "READONLY"} , 
 	{ "Name" : "travstate_end_kvs", "interface" : "wire", "bitwidth" : 30, "direction" : "READONLY"} , 
 	{ "Name" : "llopparams_currentLOP", "interface" : "wire", "bitwidth" : 32, "direction" : "READONLY"} , 
 	{ "Name" : "llopparams_upperlimit", "interface" : "wire", "bitwidth" : 32, "direction" : "READONLY"} ]}
# RTL Port declarations: 
set portNum 71
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
	{ destbuffer1_V_address0 sc_out sc_lv 12 signal 6 } 
	{ destbuffer1_V_ce0 sc_out sc_logic 1 signal 6 } 
	{ destbuffer1_V_we0 sc_out sc_lv 64 signal 6 } 
	{ destbuffer1_V_d0 sc_out sc_lv 512 signal 6 } 
	{ destbuffer2_V_address0 sc_out sc_lv 12 signal 7 } 
	{ destbuffer2_V_ce0 sc_out sc_logic 1 signal 7 } 
	{ destbuffer2_V_we0 sc_out sc_lv 64 signal 7 } 
	{ destbuffer2_V_d0 sc_out sc_lv 512 signal 7 } 
	{ destbuffer3_V_address0 sc_out sc_lv 12 signal 8 } 
	{ destbuffer3_V_ce0 sc_out sc_logic 1 signal 8 } 
	{ destbuffer3_V_we0 sc_out sc_lv 64 signal 8 } 
	{ destbuffer3_V_d0 sc_out sc_lv 512 signal 8 } 
	{ capsule0_key_address0 sc_out sc_lv 5 signal 9 } 
	{ capsule0_key_ce0 sc_out sc_logic 1 signal 9 } 
	{ capsule0_key_q0 sc_in sc_lv 32 signal 9 } 
	{ capsule0_value_address0 sc_out sc_lv 5 signal 10 } 
	{ capsule0_value_ce0 sc_out sc_logic 1 signal 10 } 
	{ capsule0_value_we0 sc_out sc_logic 1 signal 10 } 
	{ capsule0_value_d0 sc_out sc_lv 32 signal 10 } 
	{ capsule0_value_q0 sc_in sc_lv 32 signal 10 } 
	{ capsule1_key_address0 sc_out sc_lv 5 signal 11 } 
	{ capsule1_key_ce0 sc_out sc_logic 1 signal 11 } 
	{ capsule1_key_q0 sc_in sc_lv 32 signal 11 } 
	{ capsule1_value_address0 sc_out sc_lv 5 signal 12 } 
	{ capsule1_value_ce0 sc_out sc_logic 1 signal 12 } 
	{ capsule1_value_we0 sc_out sc_logic 1 signal 12 } 
	{ capsule1_value_d0 sc_out sc_lv 32 signal 12 } 
	{ capsule1_value_q0 sc_in sc_lv 32 signal 12 } 
	{ capsule2_key_address0 sc_out sc_lv 5 signal 13 } 
	{ capsule2_key_ce0 sc_out sc_logic 1 signal 13 } 
	{ capsule2_key_q0 sc_in sc_lv 32 signal 13 } 
	{ capsule2_value_address0 sc_out sc_lv 5 signal 14 } 
	{ capsule2_value_ce0 sc_out sc_logic 1 signal 14 } 
	{ capsule2_value_we0 sc_out sc_logic 1 signal 14 } 
	{ capsule2_value_d0 sc_out sc_lv 32 signal 14 } 
	{ capsule2_value_q0 sc_in sc_lv 32 signal 14 } 
	{ capsule3_key_address0 sc_out sc_lv 5 signal 15 } 
	{ capsule3_key_ce0 sc_out sc_logic 1 signal 15 } 
	{ capsule3_key_q0 sc_in sc_lv 32 signal 15 } 
	{ capsule3_value_address0 sc_out sc_lv 5 signal 16 } 
	{ capsule3_value_ce0 sc_out sc_logic 1 signal 16 } 
	{ capsule3_value_we0 sc_out sc_logic 1 signal 16 } 
	{ capsule3_value_d0 sc_out sc_lv 32 signal 16 } 
	{ capsule3_value_q0 sc_in sc_lv 32 signal 16 } 
	{ travstate_i_kvs sc_in sc_lv 32 signal 17 } 
	{ travstate_end_kvs sc_in sc_lv 30 signal 18 } 
	{ llopparams_currentLOP sc_in sc_lv 32 signal 19 } 
	{ llopparams_upperlimit sc_in sc_lv 32 signal 20 } 
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
 	{ "name": "destbuffer1_V_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":12, "type": "signal", "bundle":{"name": "destbuffer1_V", "role": "address0" }} , 
 	{ "name": "destbuffer1_V_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "destbuffer1_V", "role": "ce0" }} , 
 	{ "name": "destbuffer1_V_we0", "direction": "out", "datatype": "sc_lv", "bitwidth":64, "type": "signal", "bundle":{"name": "destbuffer1_V", "role": "we0" }} , 
 	{ "name": "destbuffer1_V_d0", "direction": "out", "datatype": "sc_lv", "bitwidth":512, "type": "signal", "bundle":{"name": "destbuffer1_V", "role": "d0" }} , 
 	{ "name": "destbuffer2_V_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":12, "type": "signal", "bundle":{"name": "destbuffer2_V", "role": "address0" }} , 
 	{ "name": "destbuffer2_V_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "destbuffer2_V", "role": "ce0" }} , 
 	{ "name": "destbuffer2_V_we0", "direction": "out", "datatype": "sc_lv", "bitwidth":64, "type": "signal", "bundle":{"name": "destbuffer2_V", "role": "we0" }} , 
 	{ "name": "destbuffer2_V_d0", "direction": "out", "datatype": "sc_lv", "bitwidth":512, "type": "signal", "bundle":{"name": "destbuffer2_V", "role": "d0" }} , 
 	{ "name": "destbuffer3_V_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":12, "type": "signal", "bundle":{"name": "destbuffer3_V", "role": "address0" }} , 
 	{ "name": "destbuffer3_V_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "destbuffer3_V", "role": "ce0" }} , 
 	{ "name": "destbuffer3_V_we0", "direction": "out", "datatype": "sc_lv", "bitwidth":64, "type": "signal", "bundle":{"name": "destbuffer3_V", "role": "we0" }} , 
 	{ "name": "destbuffer3_V_d0", "direction": "out", "datatype": "sc_lv", "bitwidth":512, "type": "signal", "bundle":{"name": "destbuffer3_V", "role": "d0" }} , 
 	{ "name": "capsule0_key_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":5, "type": "signal", "bundle":{"name": "capsule0_key", "role": "address0" }} , 
 	{ "name": "capsule0_key_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "capsule0_key", "role": "ce0" }} , 
 	{ "name": "capsule0_key_q0", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "capsule0_key", "role": "q0" }} , 
 	{ "name": "capsule0_value_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":5, "type": "signal", "bundle":{"name": "capsule0_value", "role": "address0" }} , 
 	{ "name": "capsule0_value_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "capsule0_value", "role": "ce0" }} , 
 	{ "name": "capsule0_value_we0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "capsule0_value", "role": "we0" }} , 
 	{ "name": "capsule0_value_d0", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "capsule0_value", "role": "d0" }} , 
 	{ "name": "capsule0_value_q0", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "capsule0_value", "role": "q0" }} , 
 	{ "name": "capsule1_key_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":5, "type": "signal", "bundle":{"name": "capsule1_key", "role": "address0" }} , 
 	{ "name": "capsule1_key_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "capsule1_key", "role": "ce0" }} , 
 	{ "name": "capsule1_key_q0", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "capsule1_key", "role": "q0" }} , 
 	{ "name": "capsule1_value_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":5, "type": "signal", "bundle":{"name": "capsule1_value", "role": "address0" }} , 
 	{ "name": "capsule1_value_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "capsule1_value", "role": "ce0" }} , 
 	{ "name": "capsule1_value_we0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "capsule1_value", "role": "we0" }} , 
 	{ "name": "capsule1_value_d0", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "capsule1_value", "role": "d0" }} , 
 	{ "name": "capsule1_value_q0", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "capsule1_value", "role": "q0" }} , 
 	{ "name": "capsule2_key_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":5, "type": "signal", "bundle":{"name": "capsule2_key", "role": "address0" }} , 
 	{ "name": "capsule2_key_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "capsule2_key", "role": "ce0" }} , 
 	{ "name": "capsule2_key_q0", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "capsule2_key", "role": "q0" }} , 
 	{ "name": "capsule2_value_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":5, "type": "signal", "bundle":{"name": "capsule2_value", "role": "address0" }} , 
 	{ "name": "capsule2_value_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "capsule2_value", "role": "ce0" }} , 
 	{ "name": "capsule2_value_we0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "capsule2_value", "role": "we0" }} , 
 	{ "name": "capsule2_value_d0", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "capsule2_value", "role": "d0" }} , 
 	{ "name": "capsule2_value_q0", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "capsule2_value", "role": "q0" }} , 
 	{ "name": "capsule3_key_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":5, "type": "signal", "bundle":{"name": "capsule3_key", "role": "address0" }} , 
 	{ "name": "capsule3_key_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "capsule3_key", "role": "ce0" }} , 
 	{ "name": "capsule3_key_q0", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "capsule3_key", "role": "q0" }} , 
 	{ "name": "capsule3_value_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":5, "type": "signal", "bundle":{"name": "capsule3_value", "role": "address0" }} , 
 	{ "name": "capsule3_value_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "capsule3_value", "role": "ce0" }} , 
 	{ "name": "capsule3_value_we0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "capsule3_value", "role": "we0" }} , 
 	{ "name": "capsule3_value_d0", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "capsule3_value", "role": "d0" }} , 
 	{ "name": "capsule3_value_q0", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "capsule3_value", "role": "q0" }} , 
 	{ "name": "travstate_i_kvs", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "travstate_i_kvs", "role": "default" }} , 
 	{ "name": "travstate_end_kvs", "direction": "in", "datatype": "sc_lv", "bitwidth":30, "type": "signal", "bundle":{"name": "travstate_end_kvs", "role": "default" }} , 
 	{ "name": "llopparams_currentLOP", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "llopparams_currentLOP", "role": "default" }} , 
 	{ "name": "llopparams_upperlimit", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "llopparams_upperlimit", "role": "default" }}  ]}

set RtlHierarchyInfo {[
	{"ID" : "0", "Level" : "0", "Path" : "`AUTOTB_DUT_INST", "Parent" : "", "Child" : ["1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12"],
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
	{"ID" : "1", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.topkernel_mux_325_32_1_1_U85", "Parent" : "0"},
	{"ID" : "2", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.topkernel_mux_325_32_1_1_U86", "Parent" : "0"},
	{"ID" : "3", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.topkernel_mux_325_32_1_1_U87", "Parent" : "0"},
	{"ID" : "4", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.topkernel_mux_325_32_1_1_U88", "Parent" : "0"},
	{"ID" : "5", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.topkernel_mux_325_32_1_1_U89", "Parent" : "0"},
	{"ID" : "6", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.topkernel_mux_325_32_1_1_U90", "Parent" : "0"},
	{"ID" : "7", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.topkernel_mux_325_32_1_1_U91", "Parent" : "0"},
	{"ID" : "8", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.topkernel_mux_325_32_1_1_U92", "Parent" : "0"},
	{"ID" : "9", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.topkernel_mux_325_32_1_1_U93", "Parent" : "0"},
	{"ID" : "10", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.topkernel_mux_325_32_1_1_U94", "Parent" : "0"},
	{"ID" : "11", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.topkernel_mux_325_32_1_1_U95", "Parent" : "0"},
	{"ID" : "12", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.topkernel_mux_325_32_1_1_U96", "Parent" : "0"}]}


set ArgLastReadFirstWriteLatency {
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
		llopparams_upperlimit {Type I LastRead 0 FirstWrite -1}}}

set hasDtUnsupportedChannel 0

set PerformanceInfo {[
	{"Name" : "Latency", "Min" : "1", "Max" : "4228"}
	, {"Name" : "Interval", "Min" : "1", "Max" : "4228"}
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
	destbuffer0_V { ap_memory {  { destbuffer0_V_address0 mem_address 1 12 }  { destbuffer0_V_ce0 mem_ce 1 1 }  { destbuffer0_V_we0 mem_we 1 64 }  { destbuffer0_V_d0 mem_din 1 512 } } }
	destbuffer1_V { ap_memory {  { destbuffer1_V_address0 mem_address 1 12 }  { destbuffer1_V_ce0 mem_ce 1 1 }  { destbuffer1_V_we0 mem_we 1 64 }  { destbuffer1_V_d0 mem_din 1 512 } } }
	destbuffer2_V { ap_memory {  { destbuffer2_V_address0 mem_address 1 12 }  { destbuffer2_V_ce0 mem_ce 1 1 }  { destbuffer2_V_we0 mem_we 1 64 }  { destbuffer2_V_d0 mem_din 1 512 } } }
	destbuffer3_V { ap_memory {  { destbuffer3_V_address0 mem_address 1 12 }  { destbuffer3_V_ce0 mem_ce 1 1 }  { destbuffer3_V_we0 mem_we 1 64 }  { destbuffer3_V_d0 mem_din 1 512 } } }
	capsule0_key { ap_memory {  { capsule0_key_address0 mem_address 1 5 }  { capsule0_key_ce0 mem_ce 1 1 }  { capsule0_key_q0 mem_dout 0 32 } } }
	capsule0_value { ap_memory {  { capsule0_value_address0 mem_address 1 5 }  { capsule0_value_ce0 mem_ce 1 1 }  { capsule0_value_we0 mem_we 1 1 }  { capsule0_value_d0 mem_din 1 32 }  { capsule0_value_q0 mem_dout 0 32 } } }
	capsule1_key { ap_memory {  { capsule1_key_address0 mem_address 1 5 }  { capsule1_key_ce0 mem_ce 1 1 }  { capsule1_key_q0 mem_dout 0 32 } } }
	capsule1_value { ap_memory {  { capsule1_value_address0 mem_address 1 5 }  { capsule1_value_ce0 mem_ce 1 1 }  { capsule1_value_we0 mem_we 1 1 }  { capsule1_value_d0 mem_din 1 32 }  { capsule1_value_q0 mem_dout 0 32 } } }
	capsule2_key { ap_memory {  { capsule2_key_address0 mem_address 1 5 }  { capsule2_key_ce0 mem_ce 1 1 }  { capsule2_key_q0 mem_dout 0 32 } } }
	capsule2_value { ap_memory {  { capsule2_value_address0 mem_address 1 5 }  { capsule2_value_ce0 mem_ce 1 1 }  { capsule2_value_we0 mem_we 1 1 }  { capsule2_value_d0 mem_din 1 32 }  { capsule2_value_q0 mem_dout 0 32 } } }
	capsule3_key { ap_memory {  { capsule3_key_address0 mem_address 1 5 }  { capsule3_key_ce0 mem_ce 1 1 }  { capsule3_key_q0 mem_dout 0 32 } } }
	capsule3_value { ap_memory {  { capsule3_value_address0 mem_address 1 5 }  { capsule3_value_ce0 mem_ce 1 1 }  { capsule3_value_we0 mem_we 1 1 }  { capsule3_value_d0 mem_din 1 32 }  { capsule3_value_q0 mem_dout 0 32 } } }
	travstate_i_kvs { ap_none {  { travstate_i_kvs in_data 0 32 } } }
	travstate_end_kvs { ap_none {  { travstate_end_kvs in_data 0 30 } } }
	llopparams_currentLOP { ap_none {  { llopparams_currentLOP in_data 0 32 } } }
	llopparams_upperlimit { ap_none {  { llopparams_upperlimit in_data 0 32 } } }
}
set moduleName partitionkeyvalues00
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
set C_modelName {partitionkeyvalues00}
set C_modelType { void 0 }
set C_modelArgList {
	{ enable int 1 regular  }
	{ sourcebuffer0_V int 512 regular {array 512 { 1 3 } 1 1 }  }
	{ sourcebuffer1_V int 512 regular {array 512 { 1 3 } 1 1 }  }
	{ sourcebuffer2_V int 512 regular {array 512 { 1 3 } 1 1 }  }
	{ sourcebuffer3_V int 512 regular {array 512 { 1 3 } 1 1 }  }
	{ destbuffer0_V int 512 regular {array 2112 { 0 3 } 0 1 }  }
	{ destbuffer1_V int 512 regular {array 2112 { 0 3 } 0 1 }  }
	{ destbuffer2_V int 512 regular {array 2112 { 0 3 } 0 1 }  }
	{ destbuffer3_V int 512 regular {array 2112 { 0 3 } 0 1 }  }
	{ capsule0_key int 32 regular {array 32 { 1 3 } 1 1 }  }
	{ capsule0_value int 32 regular {array 32 { 2 3 } 1 1 }  }
	{ capsule1_key int 32 regular {array 32 { 1 3 } 1 1 }  }
	{ capsule1_value int 32 regular {array 32 { 2 3 } 1 1 }  }
	{ capsule2_key int 32 regular {array 32 { 1 3 } 1 1 }  }
	{ capsule2_value int 32 regular {array 32 { 2 3 } 1 1 }  }
	{ capsule3_key int 32 regular {array 32 { 1 3 } 1 1 }  }
	{ capsule3_value int 32 regular {array 32 { 2 3 } 1 1 }  }
	{ travstate_i_kvs int 32 regular  }
	{ travstate_end_kvs int 30 regular  }
	{ llopparams_currentLOP int 32 regular  }
	{ llopparams_upperlimit int 32 regular  }
}
set C_modelArgMapList {[ 
	{ "Name" : "enable", "interface" : "wire", "bitwidth" : 1, "direction" : "READONLY"} , 
 	{ "Name" : "sourcebuffer0_V", "interface" : "memory", "bitwidth" : 512, "direction" : "READONLY"} , 
 	{ "Name" : "sourcebuffer1_V", "interface" : "memory", "bitwidth" : 512, "direction" : "READONLY"} , 
 	{ "Name" : "sourcebuffer2_V", "interface" : "memory", "bitwidth" : 512, "direction" : "READONLY"} , 
 	{ "Name" : "sourcebuffer3_V", "interface" : "memory", "bitwidth" : 512, "direction" : "READONLY"} , 
 	{ "Name" : "destbuffer0_V", "interface" : "memory", "bitwidth" : 512, "direction" : "WRITEONLY"} , 
 	{ "Name" : "destbuffer1_V", "interface" : "memory", "bitwidth" : 512, "direction" : "WRITEONLY"} , 
 	{ "Name" : "destbuffer2_V", "interface" : "memory", "bitwidth" : 512, "direction" : "WRITEONLY"} , 
 	{ "Name" : "destbuffer3_V", "interface" : "memory", "bitwidth" : 512, "direction" : "WRITEONLY"} , 
 	{ "Name" : "capsule0_key", "interface" : "memory", "bitwidth" : 32, "direction" : "READONLY"} , 
 	{ "Name" : "capsule0_value", "interface" : "memory", "bitwidth" : 32, "direction" : "READWRITE"} , 
 	{ "Name" : "capsule1_key", "interface" : "memory", "bitwidth" : 32, "direction" : "READONLY"} , 
 	{ "Name" : "capsule1_value", "interface" : "memory", "bitwidth" : 32, "direction" : "READWRITE"} , 
 	{ "Name" : "capsule2_key", "interface" : "memory", "bitwidth" : 32, "direction" : "READONLY"} , 
 	{ "Name" : "capsule2_value", "interface" : "memory", "bitwidth" : 32, "direction" : "READWRITE"} , 
 	{ "Name" : "capsule3_key", "interface" : "memory", "bitwidth" : 32, "direction" : "READONLY"} , 
 	{ "Name" : "capsule3_value", "interface" : "memory", "bitwidth" : 32, "direction" : "READWRITE"} , 
 	{ "Name" : "travstate_i_kvs", "interface" : "wire", "bitwidth" : 32, "direction" : "READONLY"} , 
 	{ "Name" : "travstate_end_kvs", "interface" : "wire", "bitwidth" : 30, "direction" : "READONLY"} , 
 	{ "Name" : "llopparams_currentLOP", "interface" : "wire", "bitwidth" : 32, "direction" : "READONLY"} , 
 	{ "Name" : "llopparams_upperlimit", "interface" : "wire", "bitwidth" : 32, "direction" : "READONLY"} ]}
# RTL Port declarations: 
set portNum 71
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
	{ destbuffer1_V_address0 sc_out sc_lv 12 signal 6 } 
	{ destbuffer1_V_ce0 sc_out sc_logic 1 signal 6 } 
	{ destbuffer1_V_we0 sc_out sc_lv 64 signal 6 } 
	{ destbuffer1_V_d0 sc_out sc_lv 512 signal 6 } 
	{ destbuffer2_V_address0 sc_out sc_lv 12 signal 7 } 
	{ destbuffer2_V_ce0 sc_out sc_logic 1 signal 7 } 
	{ destbuffer2_V_we0 sc_out sc_lv 64 signal 7 } 
	{ destbuffer2_V_d0 sc_out sc_lv 512 signal 7 } 
	{ destbuffer3_V_address0 sc_out sc_lv 12 signal 8 } 
	{ destbuffer3_V_ce0 sc_out sc_logic 1 signal 8 } 
	{ destbuffer3_V_we0 sc_out sc_lv 64 signal 8 } 
	{ destbuffer3_V_d0 sc_out sc_lv 512 signal 8 } 
	{ capsule0_key_address0 sc_out sc_lv 5 signal 9 } 
	{ capsule0_key_ce0 sc_out sc_logic 1 signal 9 } 
	{ capsule0_key_q0 sc_in sc_lv 32 signal 9 } 
	{ capsule0_value_address0 sc_out sc_lv 5 signal 10 } 
	{ capsule0_value_ce0 sc_out sc_logic 1 signal 10 } 
	{ capsule0_value_we0 sc_out sc_logic 1 signal 10 } 
	{ capsule0_value_d0 sc_out sc_lv 32 signal 10 } 
	{ capsule0_value_q0 sc_in sc_lv 32 signal 10 } 
	{ capsule1_key_address0 sc_out sc_lv 5 signal 11 } 
	{ capsule1_key_ce0 sc_out sc_logic 1 signal 11 } 
	{ capsule1_key_q0 sc_in sc_lv 32 signal 11 } 
	{ capsule1_value_address0 sc_out sc_lv 5 signal 12 } 
	{ capsule1_value_ce0 sc_out sc_logic 1 signal 12 } 
	{ capsule1_value_we0 sc_out sc_logic 1 signal 12 } 
	{ capsule1_value_d0 sc_out sc_lv 32 signal 12 } 
	{ capsule1_value_q0 sc_in sc_lv 32 signal 12 } 
	{ capsule2_key_address0 sc_out sc_lv 5 signal 13 } 
	{ capsule2_key_ce0 sc_out sc_logic 1 signal 13 } 
	{ capsule2_key_q0 sc_in sc_lv 32 signal 13 } 
	{ capsule2_value_address0 sc_out sc_lv 5 signal 14 } 
	{ capsule2_value_ce0 sc_out sc_logic 1 signal 14 } 
	{ capsule2_value_we0 sc_out sc_logic 1 signal 14 } 
	{ capsule2_value_d0 sc_out sc_lv 32 signal 14 } 
	{ capsule2_value_q0 sc_in sc_lv 32 signal 14 } 
	{ capsule3_key_address0 sc_out sc_lv 5 signal 15 } 
	{ capsule3_key_ce0 sc_out sc_logic 1 signal 15 } 
	{ capsule3_key_q0 sc_in sc_lv 32 signal 15 } 
	{ capsule3_value_address0 sc_out sc_lv 5 signal 16 } 
	{ capsule3_value_ce0 sc_out sc_logic 1 signal 16 } 
	{ capsule3_value_we0 sc_out sc_logic 1 signal 16 } 
	{ capsule3_value_d0 sc_out sc_lv 32 signal 16 } 
	{ capsule3_value_q0 sc_in sc_lv 32 signal 16 } 
	{ travstate_i_kvs sc_in sc_lv 32 signal 17 } 
	{ travstate_end_kvs sc_in sc_lv 30 signal 18 } 
	{ llopparams_currentLOP sc_in sc_lv 32 signal 19 } 
	{ llopparams_upperlimit sc_in sc_lv 32 signal 20 } 
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
 	{ "name": "destbuffer1_V_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":12, "type": "signal", "bundle":{"name": "destbuffer1_V", "role": "address0" }} , 
 	{ "name": "destbuffer1_V_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "destbuffer1_V", "role": "ce0" }} , 
 	{ "name": "destbuffer1_V_we0", "direction": "out", "datatype": "sc_lv", "bitwidth":64, "type": "signal", "bundle":{"name": "destbuffer1_V", "role": "we0" }} , 
 	{ "name": "destbuffer1_V_d0", "direction": "out", "datatype": "sc_lv", "bitwidth":512, "type": "signal", "bundle":{"name": "destbuffer1_V", "role": "d0" }} , 
 	{ "name": "destbuffer2_V_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":12, "type": "signal", "bundle":{"name": "destbuffer2_V", "role": "address0" }} , 
 	{ "name": "destbuffer2_V_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "destbuffer2_V", "role": "ce0" }} , 
 	{ "name": "destbuffer2_V_we0", "direction": "out", "datatype": "sc_lv", "bitwidth":64, "type": "signal", "bundle":{"name": "destbuffer2_V", "role": "we0" }} , 
 	{ "name": "destbuffer2_V_d0", "direction": "out", "datatype": "sc_lv", "bitwidth":512, "type": "signal", "bundle":{"name": "destbuffer2_V", "role": "d0" }} , 
 	{ "name": "destbuffer3_V_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":12, "type": "signal", "bundle":{"name": "destbuffer3_V", "role": "address0" }} , 
 	{ "name": "destbuffer3_V_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "destbuffer3_V", "role": "ce0" }} , 
 	{ "name": "destbuffer3_V_we0", "direction": "out", "datatype": "sc_lv", "bitwidth":64, "type": "signal", "bundle":{"name": "destbuffer3_V", "role": "we0" }} , 
 	{ "name": "destbuffer3_V_d0", "direction": "out", "datatype": "sc_lv", "bitwidth":512, "type": "signal", "bundle":{"name": "destbuffer3_V", "role": "d0" }} , 
 	{ "name": "capsule0_key_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":5, "type": "signal", "bundle":{"name": "capsule0_key", "role": "address0" }} , 
 	{ "name": "capsule0_key_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "capsule0_key", "role": "ce0" }} , 
 	{ "name": "capsule0_key_q0", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "capsule0_key", "role": "q0" }} , 
 	{ "name": "capsule0_value_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":5, "type": "signal", "bundle":{"name": "capsule0_value", "role": "address0" }} , 
 	{ "name": "capsule0_value_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "capsule0_value", "role": "ce0" }} , 
 	{ "name": "capsule0_value_we0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "capsule0_value", "role": "we0" }} , 
 	{ "name": "capsule0_value_d0", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "capsule0_value", "role": "d0" }} , 
 	{ "name": "capsule0_value_q0", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "capsule0_value", "role": "q0" }} , 
 	{ "name": "capsule1_key_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":5, "type": "signal", "bundle":{"name": "capsule1_key", "role": "address0" }} , 
 	{ "name": "capsule1_key_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "capsule1_key", "role": "ce0" }} , 
 	{ "name": "capsule1_key_q0", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "capsule1_key", "role": "q0" }} , 
 	{ "name": "capsule1_value_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":5, "type": "signal", "bundle":{"name": "capsule1_value", "role": "address0" }} , 
 	{ "name": "capsule1_value_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "capsule1_value", "role": "ce0" }} , 
 	{ "name": "capsule1_value_we0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "capsule1_value", "role": "we0" }} , 
 	{ "name": "capsule1_value_d0", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "capsule1_value", "role": "d0" }} , 
 	{ "name": "capsule1_value_q0", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "capsule1_value", "role": "q0" }} , 
 	{ "name": "capsule2_key_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":5, "type": "signal", "bundle":{"name": "capsule2_key", "role": "address0" }} , 
 	{ "name": "capsule2_key_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "capsule2_key", "role": "ce0" }} , 
 	{ "name": "capsule2_key_q0", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "capsule2_key", "role": "q0" }} , 
 	{ "name": "capsule2_value_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":5, "type": "signal", "bundle":{"name": "capsule2_value", "role": "address0" }} , 
 	{ "name": "capsule2_value_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "capsule2_value", "role": "ce0" }} , 
 	{ "name": "capsule2_value_we0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "capsule2_value", "role": "we0" }} , 
 	{ "name": "capsule2_value_d0", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "capsule2_value", "role": "d0" }} , 
 	{ "name": "capsule2_value_q0", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "capsule2_value", "role": "q0" }} , 
 	{ "name": "capsule3_key_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":5, "type": "signal", "bundle":{"name": "capsule3_key", "role": "address0" }} , 
 	{ "name": "capsule3_key_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "capsule3_key", "role": "ce0" }} , 
 	{ "name": "capsule3_key_q0", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "capsule3_key", "role": "q0" }} , 
 	{ "name": "capsule3_value_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":5, "type": "signal", "bundle":{"name": "capsule3_value", "role": "address0" }} , 
 	{ "name": "capsule3_value_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "capsule3_value", "role": "ce0" }} , 
 	{ "name": "capsule3_value_we0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "capsule3_value", "role": "we0" }} , 
 	{ "name": "capsule3_value_d0", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "capsule3_value", "role": "d0" }} , 
 	{ "name": "capsule3_value_q0", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "capsule3_value", "role": "q0" }} , 
 	{ "name": "travstate_i_kvs", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "travstate_i_kvs", "role": "default" }} , 
 	{ "name": "travstate_end_kvs", "direction": "in", "datatype": "sc_lv", "bitwidth":30, "type": "signal", "bundle":{"name": "travstate_end_kvs", "role": "default" }} , 
 	{ "name": "llopparams_currentLOP", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "llopparams_currentLOP", "role": "default" }} , 
 	{ "name": "llopparams_upperlimit", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "llopparams_upperlimit", "role": "default" }}  ]}

set RtlHierarchyInfo {[
	{"ID" : "0", "Level" : "0", "Path" : "`AUTOTB_DUT_INST", "Parent" : "", "Child" : ["1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12"],
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
	{"ID" : "1", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.topkernel_mux_325_32_1_1_U85", "Parent" : "0"},
	{"ID" : "2", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.topkernel_mux_325_32_1_1_U86", "Parent" : "0"},
	{"ID" : "3", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.topkernel_mux_325_32_1_1_U87", "Parent" : "0"},
	{"ID" : "4", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.topkernel_mux_325_32_1_1_U88", "Parent" : "0"},
	{"ID" : "5", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.topkernel_mux_325_32_1_1_U89", "Parent" : "0"},
	{"ID" : "6", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.topkernel_mux_325_32_1_1_U90", "Parent" : "0"},
	{"ID" : "7", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.topkernel_mux_325_32_1_1_U91", "Parent" : "0"},
	{"ID" : "8", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.topkernel_mux_325_32_1_1_U92", "Parent" : "0"},
	{"ID" : "9", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.topkernel_mux_325_32_1_1_U93", "Parent" : "0"},
	{"ID" : "10", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.topkernel_mux_325_32_1_1_U94", "Parent" : "0"},
	{"ID" : "11", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.topkernel_mux_325_32_1_1_U95", "Parent" : "0"},
	{"ID" : "12", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.topkernel_mux_325_32_1_1_U96", "Parent" : "0"}]}


set ArgLastReadFirstWriteLatency {
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
		llopparams_upperlimit {Type I LastRead 0 FirstWrite -1}}}

set hasDtUnsupportedChannel 0

set PerformanceInfo {[
	{"Name" : "Latency", "Min" : "1", "Max" : "4228"}
	, {"Name" : "Interval", "Min" : "1", "Max" : "4228"}
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
	destbuffer0_V { ap_memory {  { destbuffer0_V_address0 mem_address 1 12 }  { destbuffer0_V_ce0 mem_ce 1 1 }  { destbuffer0_V_we0 mem_we 1 64 }  { destbuffer0_V_d0 mem_din 1 512 } } }
	destbuffer1_V { ap_memory {  { destbuffer1_V_address0 mem_address 1 12 }  { destbuffer1_V_ce0 mem_ce 1 1 }  { destbuffer1_V_we0 mem_we 1 64 }  { destbuffer1_V_d0 mem_din 1 512 } } }
	destbuffer2_V { ap_memory {  { destbuffer2_V_address0 mem_address 1 12 }  { destbuffer2_V_ce0 mem_ce 1 1 }  { destbuffer2_V_we0 mem_we 1 64 }  { destbuffer2_V_d0 mem_din 1 512 } } }
	destbuffer3_V { ap_memory {  { destbuffer3_V_address0 mem_address 1 12 }  { destbuffer3_V_ce0 mem_ce 1 1 }  { destbuffer3_V_we0 mem_we 1 64 }  { destbuffer3_V_d0 mem_din 1 512 } } }
	capsule0_key { ap_memory {  { capsule0_key_address0 mem_address 1 5 }  { capsule0_key_ce0 mem_ce 1 1 }  { capsule0_key_q0 mem_dout 0 32 } } }
	capsule0_value { ap_memory {  { capsule0_value_address0 mem_address 1 5 }  { capsule0_value_ce0 mem_ce 1 1 }  { capsule0_value_we0 mem_we 1 1 }  { capsule0_value_d0 mem_din 1 32 }  { capsule0_value_q0 mem_dout 0 32 } } }
	capsule1_key { ap_memory {  { capsule1_key_address0 mem_address 1 5 }  { capsule1_key_ce0 mem_ce 1 1 }  { capsule1_key_q0 mem_dout 0 32 } } }
	capsule1_value { ap_memory {  { capsule1_value_address0 mem_address 1 5 }  { capsule1_value_ce0 mem_ce 1 1 }  { capsule1_value_we0 mem_we 1 1 }  { capsule1_value_d0 mem_din 1 32 }  { capsule1_value_q0 mem_dout 0 32 } } }
	capsule2_key { ap_memory {  { capsule2_key_address0 mem_address 1 5 }  { capsule2_key_ce0 mem_ce 1 1 }  { capsule2_key_q0 mem_dout 0 32 } } }
	capsule2_value { ap_memory {  { capsule2_value_address0 mem_address 1 5 }  { capsule2_value_ce0 mem_ce 1 1 }  { capsule2_value_we0 mem_we 1 1 }  { capsule2_value_d0 mem_din 1 32 }  { capsule2_value_q0 mem_dout 0 32 } } }
	capsule3_key { ap_memory {  { capsule3_key_address0 mem_address 1 5 }  { capsule3_key_ce0 mem_ce 1 1 }  { capsule3_key_q0 mem_dout 0 32 } } }
	capsule3_value { ap_memory {  { capsule3_value_address0 mem_address 1 5 }  { capsule3_value_ce0 mem_ce 1 1 }  { capsule3_value_we0 mem_we 1 1 }  { capsule3_value_d0 mem_din 1 32 }  { capsule3_value_q0 mem_dout 0 32 } } }
	travstate_i_kvs { ap_none {  { travstate_i_kvs in_data 0 32 } } }
	travstate_end_kvs { ap_none {  { travstate_end_kvs in_data 0 30 } } }
	llopparams_currentLOP { ap_none {  { llopparams_currentLOP in_data 0 32 } } }
	llopparams_upperlimit { ap_none {  { llopparams_upperlimit in_data 0 32 } } }
}
