set moduleName getpartition
set isTopModule 0
set isTaskLevelControl 1
set isCombinational 1
set isDatapathOnly 0
set isFreeRunPipelineModule 0
set isPipelined 0
set pipeline_type function
set FunctionProtocol ap_ctrl_hs
set isOneStateSeq 0
set ProfileFlag 0
set StallSigGenFlag 0
set isEnableWaveformDebug 1
set C_modelName {getpartition}
set C_modelType { int 5 }
set C_modelArgList {
	{ keyvalue_key int 32 regular  }
	{ currentLOP int 32 regular  }
	{ upperlimit int 32 regular  }
}
set C_modelArgMapList {[ 
	{ "Name" : "keyvalue_key", "interface" : "wire", "bitwidth" : 32, "direction" : "READONLY"} , 
 	{ "Name" : "currentLOP", "interface" : "wire", "bitwidth" : 32, "direction" : "READONLY"} , 
 	{ "Name" : "upperlimit", "interface" : "wire", "bitwidth" : 32, "direction" : "READONLY"} , 
 	{ "Name" : "ap_return", "interface" : "wire", "bitwidth" : 5} ]}
# RTL Port declarations: 
set portNum 5
set portList { 
	{ ap_ready sc_out sc_logic 1 ready -1 } 
	{ keyvalue_key sc_in sc_lv 32 signal 0 } 
	{ currentLOP sc_in sc_lv 32 signal 1 } 
	{ upperlimit sc_in sc_lv 32 signal 2 } 
	{ ap_return sc_out sc_lv 5 signal -1 } 
}
set NewPortList {[ 
	{ "name": "ap_ready", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "ready", "bundle":{"name": "ap_ready", "role": "default" }} , 
 	{ "name": "keyvalue_key", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "keyvalue_key", "role": "default" }} , 
 	{ "name": "currentLOP", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "currentLOP", "role": "default" }} , 
 	{ "name": "upperlimit", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "upperlimit", "role": "default" }} , 
 	{ "name": "ap_return", "direction": "out", "datatype": "sc_lv", "bitwidth":5, "type": "signal", "bundle":{"name": "ap_return", "role": "default" }}  ]}

set RtlHierarchyInfo {[
	{"ID" : "0", "Level" : "0", "Path" : "`AUTOTB_DUT_INST", "Parent" : "",
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
	getpartition {
		keyvalue_key {Type I LastRead 0 FirstWrite -1}
		currentLOP {Type I LastRead 0 FirstWrite -1}
		upperlimit {Type I LastRead 0 FirstWrite -1}}}

set hasDtUnsupportedChannel 0

set PerformanceInfo {[
	{"Name" : "Latency", "Min" : "0", "Max" : "0"}
	, {"Name" : "Interval", "Min" : "1", "Max" : "1"}
]}

set PipelineEnableSignalInfo {[
]}

set Spec2ImplPortList { 
	keyvalue_key { ap_none {  { keyvalue_key in_data 0 32 } } }
	currentLOP { ap_none {  { currentLOP in_data 0 32 } } }
	upperlimit { ap_none {  { upperlimit in_data 0 32 } } }
}
set moduleName getpartition
set isTopModule 0
set isTaskLevelControl 1
set isCombinational 1
set isDatapathOnly 0
set isFreeRunPipelineModule 0
set isPipelined 0
set pipeline_type function
set FunctionProtocol ap_ctrl_hs
set isOneStateSeq 0
set ProfileFlag 0
set StallSigGenFlag 0
set isEnableWaveformDebug 1
set C_modelName {getpartition}
set C_modelType { int 5 }
set C_modelArgList {
	{ keyvalue_key int 32 regular  }
	{ currentLOP int 32 regular  }
	{ upperlimit int 32 regular  }
}
set C_modelArgMapList {[ 
	{ "Name" : "keyvalue_key", "interface" : "wire", "bitwidth" : 32, "direction" : "READONLY"} , 
 	{ "Name" : "currentLOP", "interface" : "wire", "bitwidth" : 32, "direction" : "READONLY"} , 
 	{ "Name" : "upperlimit", "interface" : "wire", "bitwidth" : 32, "direction" : "READONLY"} , 
 	{ "Name" : "ap_return", "interface" : "wire", "bitwidth" : 5} ]}
# RTL Port declarations: 
set portNum 5
set portList { 
	{ ap_ready sc_out sc_logic 1 ready -1 } 
	{ keyvalue_key sc_in sc_lv 32 signal 0 } 
	{ currentLOP sc_in sc_lv 32 signal 1 } 
	{ upperlimit sc_in sc_lv 32 signal 2 } 
	{ ap_return sc_out sc_lv 5 signal -1 } 
}
set NewPortList {[ 
	{ "name": "ap_ready", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "ready", "bundle":{"name": "ap_ready", "role": "default" }} , 
 	{ "name": "keyvalue_key", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "keyvalue_key", "role": "default" }} , 
 	{ "name": "currentLOP", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "currentLOP", "role": "default" }} , 
 	{ "name": "upperlimit", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "upperlimit", "role": "default" }} , 
 	{ "name": "ap_return", "direction": "out", "datatype": "sc_lv", "bitwidth":5, "type": "signal", "bundle":{"name": "ap_return", "role": "default" }}  ]}

set RtlHierarchyInfo {[
	{"ID" : "0", "Level" : "0", "Path" : "`AUTOTB_DUT_INST", "Parent" : "",
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
	getpartition {
		keyvalue_key {Type I LastRead 0 FirstWrite -1}
		currentLOP {Type I LastRead 0 FirstWrite -1}
		upperlimit {Type I LastRead 0 FirstWrite -1}}}

set hasDtUnsupportedChannel 0

set PerformanceInfo {[
	{"Name" : "Latency", "Min" : "0", "Max" : "0"}
	, {"Name" : "Interval", "Min" : "1", "Max" : "1"}
]}

set PipelineEnableSignalInfo {[
]}

set Spec2ImplPortList { 
	keyvalue_key { ap_none {  { keyvalue_key in_data 0 32 } } }
	currentLOP { ap_none {  { currentLOP in_data 0 32 } } }
	upperlimit { ap_none {  { upperlimit in_data 0 32 } } }
}
set moduleName getpartition
set isTopModule 0
set isTaskLevelControl 1
set isCombinational 1
set isDatapathOnly 0
set isFreeRunPipelineModule 0
set isPipelined 0
set pipeline_type function
set FunctionProtocol ap_ctrl_hs
set isOneStateSeq 0
set ProfileFlag 0
set StallSigGenFlag 0
set isEnableWaveformDebug 1
set C_modelName {getpartition}
set C_modelType { int 5 }
set C_modelArgList {
	{ keyvalue_key int 32 regular  }
	{ currentLOP int 32 regular  }
	{ upperlimit int 32 regular  }
}
set C_modelArgMapList {[ 
	{ "Name" : "keyvalue_key", "interface" : "wire", "bitwidth" : 32, "direction" : "READONLY"} , 
 	{ "Name" : "currentLOP", "interface" : "wire", "bitwidth" : 32, "direction" : "READONLY"} , 
 	{ "Name" : "upperlimit", "interface" : "wire", "bitwidth" : 32, "direction" : "READONLY"} , 
 	{ "Name" : "ap_return", "interface" : "wire", "bitwidth" : 5} ]}
# RTL Port declarations: 
set portNum 5
set portList { 
	{ ap_ready sc_out sc_logic 1 ready -1 } 
	{ keyvalue_key sc_in sc_lv 32 signal 0 } 
	{ currentLOP sc_in sc_lv 32 signal 1 } 
	{ upperlimit sc_in sc_lv 32 signal 2 } 
	{ ap_return sc_out sc_lv 5 signal -1 } 
}
set NewPortList {[ 
	{ "name": "ap_ready", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "ready", "bundle":{"name": "ap_ready", "role": "default" }} , 
 	{ "name": "keyvalue_key", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "keyvalue_key", "role": "default" }} , 
 	{ "name": "currentLOP", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "currentLOP", "role": "default" }} , 
 	{ "name": "upperlimit", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "upperlimit", "role": "default" }} , 
 	{ "name": "ap_return", "direction": "out", "datatype": "sc_lv", "bitwidth":5, "type": "signal", "bundle":{"name": "ap_return", "role": "default" }}  ]}

set RtlHierarchyInfo {[
	{"ID" : "0", "Level" : "0", "Path" : "`AUTOTB_DUT_INST", "Parent" : "",
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
	getpartition {
		keyvalue_key {Type I LastRead 0 FirstWrite -1}
		currentLOP {Type I LastRead 0 FirstWrite -1}
		upperlimit {Type I LastRead 0 FirstWrite -1}}}

set hasDtUnsupportedChannel 0

set PerformanceInfo {[
	{"Name" : "Latency", "Min" : "0", "Max" : "0"}
	, {"Name" : "Interval", "Min" : "1", "Max" : "1"}
]}

set PipelineEnableSignalInfo {[
]}

set Spec2ImplPortList { 
	keyvalue_key { ap_none {  { keyvalue_key in_data 0 32 } } }
	currentLOP { ap_none {  { currentLOP in_data 0 32 } } }
	upperlimit { ap_none {  { upperlimit in_data 0 32 } } }
}
