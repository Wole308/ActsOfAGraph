# This script segment is generated automatically by AutoPilot

set id 360
set name topkernel_mux_42_32_1_1
set corename simcore_mux
set op mux
set stage_num 1
set max_latency -1
set registered_input 1
set din0_width 32
set din0_signed 0
set din1_width 32
set din1_signed 0
set din2_width 32
set din2_signed 0
set din3_width 32
set din3_signed 0
set din4_width 2
set din4_signed 0
set dout_width 32
if {${::AESL::PGuard_simmodel_gen}} {
if {[info proc ap_gen_simcore_mux] == "ap_gen_simcore_mux"} {
eval "ap_gen_simcore_mux { \
    id ${id} \
    name ${name} \
    corename ${corename} \
    op ${op} \
    reset_level 1 \
    sync_rst true \
    stage_num ${stage_num} \
    max_latency ${max_latency} \
    registered_input ${registered_input} \
    din0_width ${din0_width} \
    din0_signed ${din0_signed} \
    din1_width ${din1_width} \
    din1_signed ${din1_signed} \
    din2_width ${din2_width} \
    din2_signed ${din2_signed} \
    din3_width ${din3_width} \
    din3_signed ${din3_signed} \
    din4_width ${din4_width} \
    din4_signed ${din4_signed} \
    dout_width ${dout_width} \
}"
} else {
puts "@W \[IMPL-100\] Cannot find ap_gen_simcore_mux, check your AutoPilot builtin lib"
}
}


if {${::AESL::PGuard_rtl_comp_handler}} {
	::AP::rtl_comp_handler ${name}
}


set op mux
set corename MuxnS
if {${::AESL::PGuard_autocg_gen} && ${::AESL::PGuard_autocg_ipmgen}} {
if {[info proc ::AESL_LIB_VIRTEX::xil_gen_pipemux] == "::AESL_LIB_VIRTEX::xil_gen_pipemux"} {
eval "::AESL_LIB_VIRTEX::xil_gen_pipemux { \
    id ${id} \
    name ${name} \
    corename ${corename} \
    op ${op} \
    reset_level 1 \
    sync_rst true \
    stage_num ${stage_num} \
    max_latency ${max_latency} \
    registered_input ${registered_input} \
    din0_width ${din0_width} \
    din0_signed ${din0_signed} \
    din1_width ${din1_width} \
    din1_signed ${din1_signed} \
    din2_width ${din2_width} \
    din2_signed ${din2_signed} \
    din3_width ${din3_width} \
    din3_signed ${din3_signed} \
    din4_width ${din4_width} \
    din4_signed ${din4_signed} \
    dout_width ${dout_width} \
}"
} else {
puts "@W \[IMPL-101\] Cannot find ::AESL_LIB_VIRTEX::xil_gen_pipemux, check your platform lib"
}
}


# Memory (RAM/ROM)  definition:
set ID 362
set hasByteEnable 0
set MemName partitionandreduce0_sourcebuffer00_0_V
set CoreName ap_simcore_mem
set PortList { 2 3 }
set DataWd 512
set AddrRange 512
set AddrWd 9
set impl_style block
set TrueReset 0
set HasInitializer 0
set IsROM 0
set ROMData {}
set NumOfStage 2
set MaxLatency -1
set DelayBudget 1.158
set ClkPeriod 10
set RegisteredInput 0
if {${::AESL::PGuard_simmodel_gen}} {
if {[info proc ap_gen_simcore_mem] == "ap_gen_simcore_mem"} {
    eval "ap_gen_simcore_mem { \
    id ${ID} \
    name ${MemName} \
    corename ${CoreName}  \
    op mem \
    hasByteEnable ${hasByteEnable} \
    reset_level 1 \
    sync_rst true \
    stage_num ${NumOfStage}  \
    registered_input ${RegisteredInput} \
    port_num 2 \
    port_list \{${PortList}\} \
    data_wd ${DataWd} \
    addr_wd ${AddrWd} \
    addr_range ${AddrRange} \
    style ${impl_style} \
    true_reset ${TrueReset} \
    delay_budget ${DelayBudget} \
    clk_period ${ClkPeriod} \
    HasInitializer ${HasInitializer} \
    rom_data \{${ROMData}\} \
 } "
} else {
    puts "@W \[IMPL-102\] Cannot find ap_gen_simcore_mem, check your platform lib"
}
}


if {${::AESL::PGuard_rtl_comp_handler}} {
  ::AP::rtl_comp_handler $MemName
}


set CoreName RAM
if {${::AESL::PGuard_autocg_gen} && ${::AESL::PGuard_autocg_ipmgen}} {
if {[info proc ::AESL_LIB_VIRTEX::xil_gen_RAM] == "::AESL_LIB_VIRTEX::xil_gen_RAM"} {
    eval "::AESL_LIB_VIRTEX::xil_gen_RAM { \
    id ${ID} \
    name ${MemName} \
    corename ${CoreName}  \
    op mem \
    hasByteEnable ${hasByteEnable} \
    reset_level 1 \
    sync_rst true \
    stage_num ${NumOfStage}  \
    registered_input ${RegisteredInput} \
    port_num 2 \
    port_list \{${PortList}\} \
    data_wd ${DataWd} \
    addr_wd ${AddrWd} \
    addr_range ${AddrRange} \
    style ${impl_style} \
    true_reset ${TrueReset} \
    delay_budget ${DelayBudget} \
    clk_period ${ClkPeriod} \
    HasInitializer ${HasInitializer} \
    rom_data \{${ROMData}\} \
 } "
  } else {
    puts "@W \[IMPL-104\] Cannot find ::AESL_LIB_VIRTEX::xil_gen_RAM, check your platform lib"
  }
}


# Memory (RAM/ROM)  definition:
set ID 363
set hasByteEnable 1
set MemName partitionandreduce0_destbuffer00_V
set CoreName ap_simcore_mem
set PortList { 2 3 }
set DataWd 512
set AddrRange 2112
set AddrWd 12
set impl_style block
set TrueReset 0
set HasInitializer 0
set IsROM 0
set ROMData {}
set NumOfStage 2
set MaxLatency -1
set DelayBudget 1.158
set ClkPeriod 10
set RegisteredInput 0
if {${::AESL::PGuard_simmodel_gen}} {
if {[info proc ap_gen_simcore_mem] == "ap_gen_simcore_mem"} {
    eval "ap_gen_simcore_mem { \
    id ${ID} \
    name ${MemName} \
    corename ${CoreName}  \
    op mem \
    hasByteEnable ${hasByteEnable} \
    reset_level 1 \
    sync_rst true \
    stage_num ${NumOfStage}  \
    registered_input ${RegisteredInput} \
    port_num 2 \
    port_list \{${PortList}\} \
    data_wd ${DataWd} \
    addr_wd ${AddrWd} \
    addr_range ${AddrRange} \
    style ${impl_style} \
    true_reset ${TrueReset} \
    delay_budget ${DelayBudget} \
    clk_period ${ClkPeriod} \
    HasInitializer ${HasInitializer} \
    rom_data \{${ROMData}\} \
 } "
} else {
    puts "@W \[IMPL-102\] Cannot find ap_gen_simcore_mem, check your platform lib"
}
}


if {${::AESL::PGuard_rtl_comp_handler}} {
  ::AP::rtl_comp_handler $MemName
}


set CoreName RAM
if {${::AESL::PGuard_autocg_gen} && ${::AESL::PGuard_autocg_ipmgen}} {
if {[info proc ::AESL_LIB_VIRTEX::xil_gen_RAM] == "::AESL_LIB_VIRTEX::xil_gen_RAM"} {
    eval "::AESL_LIB_VIRTEX::xil_gen_RAM { \
    id ${ID} \
    name ${MemName} \
    corename ${CoreName}  \
    op mem \
    hasByteEnable ${hasByteEnable} \
    reset_level 1 \
    sync_rst true \
    stage_num ${NumOfStage}  \
    registered_input ${RegisteredInput} \
    port_num 2 \
    port_list \{${PortList}\} \
    data_wd ${DataWd} \
    addr_wd ${AddrWd} \
    addr_range ${AddrRange} \
    style ${impl_style} \
    true_reset ${TrueReset} \
    delay_budget ${DelayBudget} \
    clk_period ${ClkPeriod} \
    HasInitializer ${HasInitializer} \
    rom_data \{${ROMData}\} \
 } "
  } else {
    puts "@W \[IMPL-104\] Cannot find ::AESL_LIB_VIRTEX::xil_gen_RAM, check your platform lib"
  }
}


# Memory (RAM/ROM)  definition:
set ID 364
set hasByteEnable 0
set MemName partitionandreduce0_capsule00_key
set CoreName ap_simcore_mem
set PortList { 2 1 }
set DataWd 32
set AddrRange 32
set AddrWd 5
set impl_style block
set TrueReset 0
set HasInitializer 0
set IsROM 0
set ROMData {}
set NumOfStage 2
set MaxLatency -1
set DelayBudget 1.158
set ClkPeriod 10
set RegisteredInput 0
if {${::AESL::PGuard_simmodel_gen}} {
if {[info proc ap_gen_simcore_mem] == "ap_gen_simcore_mem"} {
    eval "ap_gen_simcore_mem { \
    id ${ID} \
    name ${MemName} \
    corename ${CoreName}  \
    op mem \
    hasByteEnable ${hasByteEnable} \
    reset_level 1 \
    sync_rst true \
    stage_num ${NumOfStage}  \
    registered_input ${RegisteredInput} \
    port_num 2 \
    port_list \{${PortList}\} \
    data_wd ${DataWd} \
    addr_wd ${AddrWd} \
    addr_range ${AddrRange} \
    style ${impl_style} \
    true_reset ${TrueReset} \
    delay_budget ${DelayBudget} \
    clk_period ${ClkPeriod} \
    HasInitializer ${HasInitializer} \
    rom_data \{${ROMData}\} \
 } "
} else {
    puts "@W \[IMPL-102\] Cannot find ap_gen_simcore_mem, check your platform lib"
}
}


if {${::AESL::PGuard_rtl_comp_handler}} {
  ::AP::rtl_comp_handler $MemName
}


set CoreName RAM
if {${::AESL::PGuard_autocg_gen} && ${::AESL::PGuard_autocg_ipmgen}} {
if {[info proc ::AESL_LIB_VIRTEX::xil_gen_RAM] == "::AESL_LIB_VIRTEX::xil_gen_RAM"} {
    eval "::AESL_LIB_VIRTEX::xil_gen_RAM { \
    id ${ID} \
    name ${MemName} \
    corename ${CoreName}  \
    op mem \
    hasByteEnable ${hasByteEnable} \
    reset_level 1 \
    sync_rst true \
    stage_num ${NumOfStage}  \
    registered_input ${RegisteredInput} \
    port_num 2 \
    port_list \{${PortList}\} \
    data_wd ${DataWd} \
    addr_wd ${AddrWd} \
    addr_range ${AddrRange} \
    style ${impl_style} \
    true_reset ${TrueReset} \
    delay_budget ${DelayBudget} \
    clk_period ${ClkPeriod} \
    HasInitializer ${HasInitializer} \
    rom_data \{${ROMData}\} \
 } "
  } else {
    puts "@W \[IMPL-104\] Cannot find ::AESL_LIB_VIRTEX::xil_gen_RAM, check your platform lib"
  }
}


# Memory (RAM/ROM)  definition:
set ID 365
set hasByteEnable 1
set MemName partitionandreduce0_BIGcapsule00_V
set CoreName ap_simcore_mem
set PortList { 2 3 }
set DataWd 512
set AddrRange 512
set AddrWd 9
set impl_style block
set TrueReset 0
set HasInitializer 0
set IsROM 0
set ROMData {}
set NumOfStage 2
set MaxLatency -1
set DelayBudget 1.158
set ClkPeriod 10
set RegisteredInput 0
if {${::AESL::PGuard_simmodel_gen}} {
if {[info proc ap_gen_simcore_mem] == "ap_gen_simcore_mem"} {
    eval "ap_gen_simcore_mem { \
    id ${ID} \
    name ${MemName} \
    corename ${CoreName}  \
    op mem \
    hasByteEnable ${hasByteEnable} \
    reset_level 1 \
    sync_rst true \
    stage_num ${NumOfStage}  \
    registered_input ${RegisteredInput} \
    port_num 2 \
    port_list \{${PortList}\} \
    data_wd ${DataWd} \
    addr_wd ${AddrWd} \
    addr_range ${AddrRange} \
    style ${impl_style} \
    true_reset ${TrueReset} \
    delay_budget ${DelayBudget} \
    clk_period ${ClkPeriod} \
    HasInitializer ${HasInitializer} \
    rom_data \{${ROMData}\} \
 } "
} else {
    puts "@W \[IMPL-102\] Cannot find ap_gen_simcore_mem, check your platform lib"
}
}


if {${::AESL::PGuard_rtl_comp_handler}} {
  ::AP::rtl_comp_handler $MemName
}


set CoreName RAM
if {${::AESL::PGuard_autocg_gen} && ${::AESL::PGuard_autocg_ipmgen}} {
if {[info proc ::AESL_LIB_VIRTEX::xil_gen_RAM] == "::AESL_LIB_VIRTEX::xil_gen_RAM"} {
    eval "::AESL_LIB_VIRTEX::xil_gen_RAM { \
    id ${ID} \
    name ${MemName} \
    corename ${CoreName}  \
    op mem \
    hasByteEnable ${hasByteEnable} \
    reset_level 1 \
    sync_rst true \
    stage_num ${NumOfStage}  \
    registered_input ${RegisteredInput} \
    port_num 2 \
    port_list \{${PortList}\} \
    data_wd ${DataWd} \
    addr_wd ${AddrWd} \
    addr_range ${AddrRange} \
    style ${impl_style} \
    true_reset ${TrueReset} \
    delay_budget ${DelayBudget} \
    clk_period ${ClkPeriod} \
    HasInitializer ${HasInitializer} \
    rom_data \{${ROMData}\} \
 } "
  } else {
    puts "@W \[IMPL-104\] Cannot find ::AESL_LIB_VIRTEX::xil_gen_RAM, check your platform lib"
  }
}


# Memory (RAM/ROM)  definition:
set ID 366
set hasByteEnable 0
set MemName partitionandreduce0_destbuffer000_key
set CoreName ap_simcore_mem
set PortList { 0 3 }
set DataWd 32
set AddrRange 2112
set AddrWd 12
set impl_style block
set TrueReset 0
set HasInitializer 0
set IsROM 0
set ROMData {}
set NumOfStage 2
set MaxLatency -1
set DelayBudget 1.158
set ClkPeriod 10
set RegisteredInput 0
if {${::AESL::PGuard_simmodel_gen}} {
if {[info proc ap_gen_simcore_mem] == "ap_gen_simcore_mem"} {
    eval "ap_gen_simcore_mem { \
    id ${ID} \
    name ${MemName} \
    corename ${CoreName}  \
    op mem \
    hasByteEnable ${hasByteEnable} \
    reset_level 1 \
    sync_rst true \
    stage_num ${NumOfStage}  \
    registered_input ${RegisteredInput} \
    port_num 2 \
    port_list \{${PortList}\} \
    data_wd ${DataWd} \
    addr_wd ${AddrWd} \
    addr_range ${AddrRange} \
    style ${impl_style} \
    true_reset ${TrueReset} \
    delay_budget ${DelayBudget} \
    clk_period ${ClkPeriod} \
    HasInitializer ${HasInitializer} \
    rom_data \{${ROMData}\} \
 } "
} else {
    puts "@W \[IMPL-102\] Cannot find ap_gen_simcore_mem, check your platform lib"
}
}


if {${::AESL::PGuard_rtl_comp_handler}} {
  ::AP::rtl_comp_handler $MemName
}


set CoreName RAM
if {${::AESL::PGuard_autocg_gen} && ${::AESL::PGuard_autocg_ipmgen}} {
if {[info proc ::AESL_LIB_VIRTEX::xil_gen_RAM] == "::AESL_LIB_VIRTEX::xil_gen_RAM"} {
    eval "::AESL_LIB_VIRTEX::xil_gen_RAM { \
    id ${ID} \
    name ${MemName} \
    corename ${CoreName}  \
    op mem \
    hasByteEnable ${hasByteEnable} \
    reset_level 1 \
    sync_rst true \
    stage_num ${NumOfStage}  \
    registered_input ${RegisteredInput} \
    port_num 2 \
    port_list \{${PortList}\} \
    data_wd ${DataWd} \
    addr_wd ${AddrWd} \
    addr_range ${AddrRange} \
    style ${impl_style} \
    true_reset ${TrueReset} \
    delay_budget ${DelayBudget} \
    clk_period ${ClkPeriod} \
    HasInitializer ${HasInitializer} \
    rom_data \{${ROMData}\} \
 } "
  } else {
    puts "@W \[IMPL-104\] Cannot find ::AESL_LIB_VIRTEX::xil_gen_RAM, check your platform lib"
  }
}


# Memory (RAM/ROM)  definition:
set ID 367
set hasByteEnable 0
set MemName partitionandreduce0_destbuffer000_value
set CoreName ap_simcore_mem
set PortList { 2 3 }
set DataWd 32
set AddrRange 2112
set AddrWd 12
set impl_style block
set TrueReset 0
set HasInitializer 0
set IsROM 0
set ROMData {}
set NumOfStage 2
set MaxLatency -1
set DelayBudget 1.158
set ClkPeriod 10
set RegisteredInput 0
if {${::AESL::PGuard_simmodel_gen}} {
if {[info proc ap_gen_simcore_mem] == "ap_gen_simcore_mem"} {
    eval "ap_gen_simcore_mem { \
    id ${ID} \
    name ${MemName} \
    corename ${CoreName}  \
    op mem \
    hasByteEnable ${hasByteEnable} \
    reset_level 1 \
    sync_rst true \
    stage_num ${NumOfStage}  \
    registered_input ${RegisteredInput} \
    port_num 2 \
    port_list \{${PortList}\} \
    data_wd ${DataWd} \
    addr_wd ${AddrWd} \
    addr_range ${AddrRange} \
    style ${impl_style} \
    true_reset ${TrueReset} \
    delay_budget ${DelayBudget} \
    clk_period ${ClkPeriod} \
    HasInitializer ${HasInitializer} \
    rom_data \{${ROMData}\} \
 } "
} else {
    puts "@W \[IMPL-102\] Cannot find ap_gen_simcore_mem, check your platform lib"
}
}


if {${::AESL::PGuard_rtl_comp_handler}} {
  ::AP::rtl_comp_handler $MemName
}


set CoreName RAM
if {${::AESL::PGuard_autocg_gen} && ${::AESL::PGuard_autocg_ipmgen}} {
if {[info proc ::AESL_LIB_VIRTEX::xil_gen_RAM] == "::AESL_LIB_VIRTEX::xil_gen_RAM"} {
    eval "::AESL_LIB_VIRTEX::xil_gen_RAM { \
    id ${ID} \
    name ${MemName} \
    corename ${CoreName}  \
    op mem \
    hasByteEnable ${hasByteEnable} \
    reset_level 1 \
    sync_rst true \
    stage_num ${NumOfStage}  \
    registered_input ${RegisteredInput} \
    port_num 2 \
    port_list \{${PortList}\} \
    data_wd ${DataWd} \
    addr_wd ${AddrWd} \
    addr_range ${AddrRange} \
    style ${impl_style} \
    true_reset ${TrueReset} \
    delay_budget ${DelayBudget} \
    clk_period ${ClkPeriod} \
    HasInitializer ${HasInitializer} \
    rom_data \{${ROMData}\} \
 } "
  } else {
    puts "@W \[IMPL-104\] Cannot find ::AESL_LIB_VIRTEX::xil_gen_RAM, check your platform lib"
  }
}


# Memory (RAM/ROM)  definition:
set ID 368
set hasByteEnable 0
set MemName partitionandreduce0_kvdeststats_tmp_valu
set CoreName ap_simcore_mem
set PortList { 2 2 }
set DataWd 32
set AddrRange 32
set AddrWd 5
set impl_style block
set TrueReset 0
set HasInitializer 0
set IsROM 0
set ROMData {}
set NumOfStage 2
set MaxLatency -1
set DelayBudget 1.158
set ClkPeriod 10
set RegisteredInput 0
if {${::AESL::PGuard_simmodel_gen}} {
if {[info proc ap_gen_simcore_mem] == "ap_gen_simcore_mem"} {
    eval "ap_gen_simcore_mem { \
    id ${ID} \
    name ${MemName} \
    corename ${CoreName}  \
    op mem \
    hasByteEnable ${hasByteEnable} \
    reset_level 1 \
    sync_rst true \
    stage_num ${NumOfStage}  \
    registered_input ${RegisteredInput} \
    port_num 2 \
    port_list \{${PortList}\} \
    data_wd ${DataWd} \
    addr_wd ${AddrWd} \
    addr_range ${AddrRange} \
    style ${impl_style} \
    true_reset ${TrueReset} \
    delay_budget ${DelayBudget} \
    clk_period ${ClkPeriod} \
    HasInitializer ${HasInitializer} \
    rom_data \{${ROMData}\} \
 } "
} else {
    puts "@W \[IMPL-102\] Cannot find ap_gen_simcore_mem, check your platform lib"
}
}


if {${::AESL::PGuard_rtl_comp_handler}} {
  ::AP::rtl_comp_handler $MemName
}


set CoreName RAM
if {${::AESL::PGuard_autocg_gen} && ${::AESL::PGuard_autocg_ipmgen}} {
if {[info proc ::AESL_LIB_VIRTEX::xil_gen_RAM] == "::AESL_LIB_VIRTEX::xil_gen_RAM"} {
    eval "::AESL_LIB_VIRTEX::xil_gen_RAM { \
    id ${ID} \
    name ${MemName} \
    corename ${CoreName}  \
    op mem \
    hasByteEnable ${hasByteEnable} \
    reset_level 1 \
    sync_rst true \
    stage_num ${NumOfStage}  \
    registered_input ${RegisteredInput} \
    port_num 2 \
    port_list \{${PortList}\} \
    data_wd ${DataWd} \
    addr_wd ${AddrWd} \
    addr_range ${AddrRange} \
    style ${impl_style} \
    true_reset ${TrueReset} \
    delay_budget ${DelayBudget} \
    clk_period ${ClkPeriod} \
    HasInitializer ${HasInitializer} \
    rom_data \{${ROMData}\} \
 } "
  } else {
    puts "@W \[IMPL-104\] Cannot find ::AESL_LIB_VIRTEX::xil_gen_RAM, check your platform lib"
  }
}


# clear list
if {${::AESL::PGuard_autoexp_gen}} {
    cg_default_interface_gen_dc_begin
    cg_default_interface_gen_bundle_begin
    AESL_LIB_XILADAPTER::native_axis_begin
}

# Direct connection:
if {${::AESL::PGuard_autoexp_gen}} {
eval "cg_default_interface_gen_dc { \
    id 369 \
    name kvsourcedram_V \
    type other \
    dir IO \
    reset_level 1 \
    sync_rst true \
    corename dc_kvsourcedram_V \
    op interface \
    ports { m_axi_kvsourcedram_V_AWVALID { O 1 bit } m_axi_kvsourcedram_V_AWREADY { I 1 bit } m_axi_kvsourcedram_V_AWADDR { O 32 vector } m_axi_kvsourcedram_V_AWID { O 1 vector } m_axi_kvsourcedram_V_AWLEN { O 32 vector } m_axi_kvsourcedram_V_AWSIZE { O 3 vector } m_axi_kvsourcedram_V_AWBURST { O 2 vector } m_axi_kvsourcedram_V_AWLOCK { O 2 vector } m_axi_kvsourcedram_V_AWCACHE { O 4 vector } m_axi_kvsourcedram_V_AWPROT { O 3 vector } m_axi_kvsourcedram_V_AWQOS { O 4 vector } m_axi_kvsourcedram_V_AWREGION { O 4 vector } m_axi_kvsourcedram_V_AWUSER { O 1 vector } m_axi_kvsourcedram_V_WVALID { O 1 bit } m_axi_kvsourcedram_V_WREADY { I 1 bit } m_axi_kvsourcedram_V_WDATA { O 512 vector } m_axi_kvsourcedram_V_WSTRB { O 64 vector } m_axi_kvsourcedram_V_WLAST { O 1 bit } m_axi_kvsourcedram_V_WID { O 1 vector } m_axi_kvsourcedram_V_WUSER { O 1 vector } m_axi_kvsourcedram_V_ARVALID { O 1 bit } m_axi_kvsourcedram_V_ARREADY { I 1 bit } m_axi_kvsourcedram_V_ARADDR { O 32 vector } m_axi_kvsourcedram_V_ARID { O 1 vector } m_axi_kvsourcedram_V_ARLEN { O 32 vector } m_axi_kvsourcedram_V_ARSIZE { O 3 vector } m_axi_kvsourcedram_V_ARBURST { O 2 vector } m_axi_kvsourcedram_V_ARLOCK { O 2 vector } m_axi_kvsourcedram_V_ARCACHE { O 4 vector } m_axi_kvsourcedram_V_ARPROT { O 3 vector } m_axi_kvsourcedram_V_ARQOS { O 4 vector } m_axi_kvsourcedram_V_ARREGION { O 4 vector } m_axi_kvsourcedram_V_ARUSER { O 1 vector } m_axi_kvsourcedram_V_RVALID { I 1 bit } m_axi_kvsourcedram_V_RREADY { O 1 bit } m_axi_kvsourcedram_V_RDATA { I 512 vector } m_axi_kvsourcedram_V_RLAST { I 1 bit } m_axi_kvsourcedram_V_RID { I 1 vector } m_axi_kvsourcedram_V_RUSER { I 1 vector } m_axi_kvsourcedram_V_RRESP { I 2 vector } m_axi_kvsourcedram_V_BVALID { I 1 bit } m_axi_kvsourcedram_V_BREADY { O 1 bit } m_axi_kvsourcedram_V_BRESP { I 2 vector } m_axi_kvsourcedram_V_BID { I 1 vector } m_axi_kvsourcedram_V_BUSER { I 1 vector } } \
} "
}

# Direct connection:
if {${::AESL::PGuard_autoexp_gen}} {
eval "cg_default_interface_gen_dc { \
    id 370 \
    name kvsourcedram_V_offset \
    type other \
    dir I \
    reset_level 1 \
    sync_rst true \
    corename dc_kvsourcedram_V_offset \
    op interface \
    ports { kvsourcedram_V_offset { I 26 vector } } \
} "
}

# Direct connection:
if {${::AESL::PGuard_autoexp_gen}} {
eval "cg_default_interface_gen_dc { \
    id 371 \
    name kvdestdram_V \
    type other \
    dir IO \
    reset_level 1 \
    sync_rst true \
    corename dc_kvdestdram_V \
    op interface \
    ports { m_axi_kvdestdram_V_AWVALID { O 1 bit } m_axi_kvdestdram_V_AWREADY { I 1 bit } m_axi_kvdestdram_V_AWADDR { O 32 vector } m_axi_kvdestdram_V_AWID { O 1 vector } m_axi_kvdestdram_V_AWLEN { O 32 vector } m_axi_kvdestdram_V_AWSIZE { O 3 vector } m_axi_kvdestdram_V_AWBURST { O 2 vector } m_axi_kvdestdram_V_AWLOCK { O 2 vector } m_axi_kvdestdram_V_AWCACHE { O 4 vector } m_axi_kvdestdram_V_AWPROT { O 3 vector } m_axi_kvdestdram_V_AWQOS { O 4 vector } m_axi_kvdestdram_V_AWREGION { O 4 vector } m_axi_kvdestdram_V_AWUSER { O 1 vector } m_axi_kvdestdram_V_WVALID { O 1 bit } m_axi_kvdestdram_V_WREADY { I 1 bit } m_axi_kvdestdram_V_WDATA { O 512 vector } m_axi_kvdestdram_V_WSTRB { O 64 vector } m_axi_kvdestdram_V_WLAST { O 1 bit } m_axi_kvdestdram_V_WID { O 1 vector } m_axi_kvdestdram_V_WUSER { O 1 vector } m_axi_kvdestdram_V_ARVALID { O 1 bit } m_axi_kvdestdram_V_ARREADY { I 1 bit } m_axi_kvdestdram_V_ARADDR { O 32 vector } m_axi_kvdestdram_V_ARID { O 1 vector } m_axi_kvdestdram_V_ARLEN { O 32 vector } m_axi_kvdestdram_V_ARSIZE { O 3 vector } m_axi_kvdestdram_V_ARBURST { O 2 vector } m_axi_kvdestdram_V_ARLOCK { O 2 vector } m_axi_kvdestdram_V_ARCACHE { O 4 vector } m_axi_kvdestdram_V_ARPROT { O 3 vector } m_axi_kvdestdram_V_ARQOS { O 4 vector } m_axi_kvdestdram_V_ARREGION { O 4 vector } m_axi_kvdestdram_V_ARUSER { O 1 vector } m_axi_kvdestdram_V_RVALID { I 1 bit } m_axi_kvdestdram_V_RREADY { O 1 bit } m_axi_kvdestdram_V_RDATA { I 512 vector } m_axi_kvdestdram_V_RLAST { I 1 bit } m_axi_kvdestdram_V_RID { I 1 vector } m_axi_kvdestdram_V_RUSER { I 1 vector } m_axi_kvdestdram_V_RRESP { I 2 vector } m_axi_kvdestdram_V_BVALID { I 1 bit } m_axi_kvdestdram_V_BREADY { O 1 bit } m_axi_kvdestdram_V_BRESP { I 2 vector } m_axi_kvdestdram_V_BID { I 1 vector } m_axi_kvdestdram_V_BUSER { I 1 vector } } \
} "
}

# Direct connection:
if {${::AESL::PGuard_autoexp_gen}} {
eval "cg_default_interface_gen_dc { \
    id 372 \
    name kvdestdram_V_offset \
    type other \
    dir I \
    reset_level 1 \
    sync_rst true \
    corename dc_kvdestdram_V_offset \
    op interface \
    ports { kvdestdram_V_offset { I 26 vector } } \
} "
}

# Direct connection:
if {${::AESL::PGuard_autoexp_gen}} {
eval "cg_default_interface_gen_dc { \
    id 373 \
    name kvstats \
    type other \
    dir IO \
    reset_level 1 \
    sync_rst true \
    corename dc_kvstats \
    op interface \
    ports { m_axi_kvstats_AWVALID { O 1 bit } m_axi_kvstats_AWREADY { I 1 bit } m_axi_kvstats_AWADDR { O 32 vector } m_axi_kvstats_AWID { O 1 vector } m_axi_kvstats_AWLEN { O 32 vector } m_axi_kvstats_AWSIZE { O 3 vector } m_axi_kvstats_AWBURST { O 2 vector } m_axi_kvstats_AWLOCK { O 2 vector } m_axi_kvstats_AWCACHE { O 4 vector } m_axi_kvstats_AWPROT { O 3 vector } m_axi_kvstats_AWQOS { O 4 vector } m_axi_kvstats_AWREGION { O 4 vector } m_axi_kvstats_AWUSER { O 1 vector } m_axi_kvstats_WVALID { O 1 bit } m_axi_kvstats_WREADY { I 1 bit } m_axi_kvstats_WDATA { O 64 vector } m_axi_kvstats_WSTRB { O 8 vector } m_axi_kvstats_WLAST { O 1 bit } m_axi_kvstats_WID { O 1 vector } m_axi_kvstats_WUSER { O 1 vector } m_axi_kvstats_ARVALID { O 1 bit } m_axi_kvstats_ARREADY { I 1 bit } m_axi_kvstats_ARADDR { O 32 vector } m_axi_kvstats_ARID { O 1 vector } m_axi_kvstats_ARLEN { O 32 vector } m_axi_kvstats_ARSIZE { O 3 vector } m_axi_kvstats_ARBURST { O 2 vector } m_axi_kvstats_ARLOCK { O 2 vector } m_axi_kvstats_ARCACHE { O 4 vector } m_axi_kvstats_ARPROT { O 3 vector } m_axi_kvstats_ARQOS { O 4 vector } m_axi_kvstats_ARREGION { O 4 vector } m_axi_kvstats_ARUSER { O 1 vector } m_axi_kvstats_RVALID { I 1 bit } m_axi_kvstats_RREADY { O 1 bit } m_axi_kvstats_RDATA { I 64 vector } m_axi_kvstats_RLAST { I 1 bit } m_axi_kvstats_RID { I 1 vector } m_axi_kvstats_RUSER { I 1 vector } m_axi_kvstats_RRESP { I 2 vector } m_axi_kvstats_BVALID { I 1 bit } m_axi_kvstats_BREADY { O 1 bit } m_axi_kvstats_BRESP { I 2 vector } m_axi_kvstats_BID { I 1 vector } m_axi_kvstats_BUSER { I 1 vector } } \
} "
}

# Direct connection:
if {${::AESL::PGuard_autoexp_gen}} {
eval "cg_default_interface_gen_dc { \
    id 374 \
    name kvstats_offset \
    type other \
    dir I \
    reset_level 1 \
    sync_rst true \
    corename dc_kvstats_offset \
    op interface \
    ports { kvstats_offset { I 29 vector } } \
} "
}

# Direct connection:
if {${::AESL::PGuard_autoexp_gen}} {
eval "cg_default_interface_gen_dc { \
    id 375 \
    name globalparams_runkernelcommand \
    type other \
    dir I \
    reset_level 1 \
    sync_rst true \
    corename dc_globalparams_runkernelcommand \
    op interface \
    ports { globalparams_runkernelcommand { I 32 vector } } \
} "
}

# Direct connection:
if {${::AESL::PGuard_autoexp_gen}} {
eval "cg_default_interface_gen_dc { \
    id 376 \
    name globalparams_partitioncommand \
    type other \
    dir I \
    reset_level 1 \
    sync_rst true \
    corename dc_globalparams_partitioncommand \
    op interface \
    ports { globalparams_partitioncommand { I 32 vector } } \
} "
}

# Direct connection:
if {${::AESL::PGuard_autoexp_gen}} {
eval "cg_default_interface_gen_dc { \
    id 377 \
    name globalparams_reducecommand \
    type other \
    dir I \
    reset_level 1 \
    sync_rst true \
    corename dc_globalparams_reducecommand \
    op interface \
    ports { globalparams_reducecommand { I 32 vector } } \
} "
}

# Direct connection:
if {${::AESL::PGuard_autoexp_gen}} {
eval "cg_default_interface_gen_dc { \
    id 378 \
    name globalparams_treedepth \
    type other \
    dir I \
    reset_level 1 \
    sync_rst true \
    corename dc_globalparams_treedepth \
    op interface \
    ports { globalparams_treedepth { I 32 vector } } \
} "
}

# Direct connection:
if {${::AESL::PGuard_autoexp_gen}} {
eval "cg_default_interface_gen_dc { \
    id 379 \
    name globalparams_runsize \
    type other \
    dir I \
    reset_level 1 \
    sync_rst true \
    corename dc_globalparams_runsize \
    op interface \
    ports { globalparams_runsize { I 32 vector } } \
} "
}

# Direct connection:
if {${::AESL::PGuard_autoexp_gen}} {
eval "cg_default_interface_gen_dc { \
    id 380 \
    name globalparams_vbegin \
    type other \
    dir I \
    reset_level 1 \
    sync_rst true \
    corename dc_globalparams_vbegin \
    op interface \
    ports { globalparams_vbegin { I 32 vector } } \
} "
}

# Direct connection:
if {${::AESL::PGuard_autoexp_gen}} {
eval "cg_default_interface_gen_dc { \
    id 381 \
    name globalparams_statsalreadycollected \
    type other \
    dir I \
    reset_level 1 \
    sync_rst true \
    corename dc_globalparams_statsalreadycollected \
    op interface \
    ports { globalparams_statsalreadycollected { I 32 vector } } \
} "
}

# Direct connection:
if {${::AESL::PGuard_autoexp_gen}} {
eval "cg_default_interface_gen_dc { \
    id -1 \
    name ap_ctrl \
    type ap_ctrl \
    reset_level 1 \
    sync_rst true \
    corename ap_ctrl \
    op interface \
    ports { ap_start { I 1 bit } ap_ready { O 1 bit } ap_done { O 1 bit } ap_idle { O 1 bit } } \
} "
}


# Adapter definition:
set PortName ap_clk
set DataWd 1 
if {${::AESL::PGuard_autoexp_gen}} {
if {[info proc cg_default_interface_gen_clock] == "cg_default_interface_gen_clock"} {
eval "cg_default_interface_gen_clock { \
    id -2 \
    name ${PortName} \
    reset_level 1 \
    sync_rst true \
    corename apif_ap_clk \
    data_wd ${DataWd} \
    op interface \
}"
} else {
puts "@W \[IMPL-113\] Cannot find bus interface model in the library. Ignored generation of bus interface for '${PortName}'"
}
}


# Adapter definition:
set PortName ap_rst
set DataWd 1 
if {${::AESL::PGuard_autoexp_gen}} {
if {[info proc cg_default_interface_gen_reset] == "cg_default_interface_gen_reset"} {
eval "cg_default_interface_gen_reset { \
    id -3 \
    name ${PortName} \
    reset_level 1 \
    sync_rst true \
    corename apif_ap_rst \
    data_wd ${DataWd} \
    op interface \
}"
} else {
puts "@W \[IMPL-114\] Cannot find bus interface model in the library. Ignored generation of bus interface for '${PortName}'"
}
}



# merge
if {${::AESL::PGuard_autoexp_gen}} {
    cg_default_interface_gen_dc_end
    cg_default_interface_gen_bundle_end
    AESL_LIB_XILADAPTER::native_axis_end
}


