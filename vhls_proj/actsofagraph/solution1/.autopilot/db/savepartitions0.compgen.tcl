# This script segment is generated automatically by AutoPilot

set id 119
set name topkernel_urem_12ns_8ns_12_16_1
set corename simcore_urem
set op urem
set stage_num 16
set max_latency -1
set registered_input 1
set clk_width 1
set clk_signed 0
set reset_width 1
set reset_signed 0
set in0_width 12
set in0_signed 0
set in1_width 8
set in1_signed 0
set ce_width 1
set ce_signed 0
set out_width 12
if {${::AESL::PGuard_simmodel_gen}} {
if {[info proc ap_gen_simcore_urem] == "ap_gen_simcore_urem"} {
eval "ap_gen_simcore_urem { \
    id ${id} \
    name ${name} \
    corename ${corename} \
    op ${op} \
    reset_level 1 \
    sync_rst true \
    stage_num ${stage_num} \
    max_latency ${max_latency} \
    registered_input ${registered_input} \
    clk_width ${clk_width} \
    clk_signed ${clk_signed} \
    reset_width ${reset_width} \
    reset_signed ${reset_signed} \
    in0_width ${in0_width} \
    in0_signed ${in0_signed} \
    in1_width ${in1_width} \
    in1_signed ${in1_signed} \
    ce_width ${ce_width} \
    ce_signed ${ce_signed} \
    out_width ${out_width} \
}"
} else {
puts "@W \[IMPL-100\] Cannot find ap_gen_simcore_urem, check your AutoPilot builtin lib"
}
}


if {${::AESL::PGuard_rtl_comp_handler}} {
	::AP::rtl_comp_handler ${name}
}


set op urem
set corename DivnS
if {${::AESL::PGuard_autocg_gen} && ${::AESL::PGuard_autocg_ipmgen}} {
if {[info proc ::AESL_LIB_VIRTEX::xil_gen_div] == "::AESL_LIB_VIRTEX::xil_gen_div"} {
eval "::AESL_LIB_VIRTEX::xil_gen_div { \
    id ${id} \
    name ${name} \
    corename ${corename} \
    op ${op} \
    reset_level 1 \
    sync_rst true \
    stage_num ${stage_num} \
    max_latency ${max_latency} \
    registered_input ${registered_input} \
    clk_width ${clk_width} \
    clk_signed ${clk_signed} \
    reset_width ${reset_width} \
    reset_signed ${reset_signed} \
    in0_width ${in0_width} \
    in0_signed ${in0_signed} \
    in1_width ${in1_width} \
    in1_signed ${in1_signed} \
    ce_width ${ce_width} \
    ce_signed ${ce_signed} \
    out_width ${out_width} \
}"
} else {
puts "@W \[IMPL-101\] Cannot find ::AESL_LIB_VIRTEX::xil_gen_div, check your platform lib"
}
}


# clear list
if {${::AESL::PGuard_autoexp_gen}} {
    cg_default_interface_gen_dc_begin
    cg_default_interface_gen_bundle_begin
    AESL_LIB_XILADAPTER::native_axis_begin
}

# XIL_BRAM:
if {${::AESL::PGuard_autoexp_gen}} {
if {[info proc ::AESL_LIB_XILADAPTER::xil_bram_gen] == "::AESL_LIB_XILADAPTER::xil_bram_gen"} {
eval "::AESL_LIB_XILADAPTER::xil_bram_gen { \
    id 130 \
    name destbuffer0_V \
    reset_level 1 \
    sync_rst true \
    dir I \
    corename destbuffer0_V \
    op interface \
    ports { destbuffer0_V_address0 { O 12 vector } destbuffer0_V_ce0 { O 1 bit } destbuffer0_V_q0 { I 512 vector } } \
} "
} else {
puts "@W \[IMPL-110\] Cannot find bus interface model in the library. Ignored generation of bus interface for 'destbuffer0_V'"
}
}


# XIL_BRAM:
if {${::AESL::PGuard_autoexp_gen}} {
if {[info proc ::AESL_LIB_XILADAPTER::xil_bram_gen] == "::AESL_LIB_XILADAPTER::xil_bram_gen"} {
eval "::AESL_LIB_XILADAPTER::xil_bram_gen { \
    id 131 \
    name destbuffer1_V \
    reset_level 1 \
    sync_rst true \
    dir I \
    corename destbuffer1_V \
    op interface \
    ports { destbuffer1_V_address0 { O 12 vector } destbuffer1_V_ce0 { O 1 bit } destbuffer1_V_q0 { I 512 vector } } \
} "
} else {
puts "@W \[IMPL-110\] Cannot find bus interface model in the library. Ignored generation of bus interface for 'destbuffer1_V'"
}
}


# XIL_BRAM:
if {${::AESL::PGuard_autoexp_gen}} {
if {[info proc ::AESL_LIB_XILADAPTER::xil_bram_gen] == "::AESL_LIB_XILADAPTER::xil_bram_gen"} {
eval "::AESL_LIB_XILADAPTER::xil_bram_gen { \
    id 132 \
    name destbuffer2_V \
    reset_level 1 \
    sync_rst true \
    dir I \
    corename destbuffer2_V \
    op interface \
    ports { destbuffer2_V_address0 { O 12 vector } destbuffer2_V_ce0 { O 1 bit } destbuffer2_V_q0 { I 512 vector } } \
} "
} else {
puts "@W \[IMPL-110\] Cannot find bus interface model in the library. Ignored generation of bus interface for 'destbuffer2_V'"
}
}


# XIL_BRAM:
if {${::AESL::PGuard_autoexp_gen}} {
if {[info proc ::AESL_LIB_XILADAPTER::xil_bram_gen] == "::AESL_LIB_XILADAPTER::xil_bram_gen"} {
eval "::AESL_LIB_XILADAPTER::xil_bram_gen { \
    id 133 \
    name destbuffer3_V \
    reset_level 1 \
    sync_rst true \
    dir I \
    corename destbuffer3_V \
    op interface \
    ports { destbuffer3_V_address0 { O 12 vector } destbuffer3_V_ce0 { O 1 bit } destbuffer3_V_q0 { I 512 vector } } \
} "
} else {
puts "@W \[IMPL-110\] Cannot find bus interface model in the library. Ignored generation of bus interface for 'destbuffer3_V'"
}
}


# XIL_BRAM:
if {${::AESL::PGuard_autoexp_gen}} {
if {[info proc ::AESL_LIB_XILADAPTER::xil_bram_gen] == "::AESL_LIB_XILADAPTER::xil_bram_gen"} {
eval "::AESL_LIB_XILADAPTER::xil_bram_gen { \
    id 134 \
    name capsule0_key \
    reset_level 1 \
    sync_rst true \
    dir I \
    corename capsule0_key \
    op interface \
    ports { capsule0_key_address0 { O 5 vector } capsule0_key_ce0 { O 1 bit } capsule0_key_q0 { I 32 vector } capsule0_key_address1 { O 5 vector } capsule0_key_ce1 { O 1 bit } capsule0_key_q1 { I 32 vector } } \
} "
} else {
puts "@W \[IMPL-110\] Cannot find bus interface model in the library. Ignored generation of bus interface for 'capsule0_key'"
}
}


# XIL_BRAM:
if {${::AESL::PGuard_autoexp_gen}} {
if {[info proc ::AESL_LIB_XILADAPTER::xil_bram_gen] == "::AESL_LIB_XILADAPTER::xil_bram_gen"} {
eval "::AESL_LIB_XILADAPTER::xil_bram_gen { \
    id 135 \
    name capsule0_value \
    reset_level 1 \
    sync_rst true \
    dir I \
    corename capsule0_value \
    op interface \
    ports { capsule0_value_address0 { O 5 vector } capsule0_value_ce0 { O 1 bit } capsule0_value_q0 { I 32 vector } } \
} "
} else {
puts "@W \[IMPL-110\] Cannot find bus interface model in the library. Ignored generation of bus interface for 'capsule0_value'"
}
}


# XIL_BRAM:
if {${::AESL::PGuard_autoexp_gen}} {
if {[info proc ::AESL_LIB_XILADAPTER::xil_bram_gen] == "::AESL_LIB_XILADAPTER::xil_bram_gen"} {
eval "::AESL_LIB_XILADAPTER::xil_bram_gen { \
    id 136 \
    name capsule1_key \
    reset_level 1 \
    sync_rst true \
    dir I \
    corename capsule1_key \
    op interface \
    ports { capsule1_key_address0 { O 5 vector } capsule1_key_ce0 { O 1 bit } capsule1_key_q0 { I 32 vector } capsule1_key_address1 { O 5 vector } capsule1_key_ce1 { O 1 bit } capsule1_key_q1 { I 32 vector } } \
} "
} else {
puts "@W \[IMPL-110\] Cannot find bus interface model in the library. Ignored generation of bus interface for 'capsule1_key'"
}
}


# XIL_BRAM:
if {${::AESL::PGuard_autoexp_gen}} {
if {[info proc ::AESL_LIB_XILADAPTER::xil_bram_gen] == "::AESL_LIB_XILADAPTER::xil_bram_gen"} {
eval "::AESL_LIB_XILADAPTER::xil_bram_gen { \
    id 137 \
    name capsule1_value \
    reset_level 1 \
    sync_rst true \
    dir I \
    corename capsule1_value \
    op interface \
    ports { capsule1_value_address0 { O 5 vector } capsule1_value_ce0 { O 1 bit } capsule1_value_q0 { I 32 vector } } \
} "
} else {
puts "@W \[IMPL-110\] Cannot find bus interface model in the library. Ignored generation of bus interface for 'capsule1_value'"
}
}


# XIL_BRAM:
if {${::AESL::PGuard_autoexp_gen}} {
if {[info proc ::AESL_LIB_XILADAPTER::xil_bram_gen] == "::AESL_LIB_XILADAPTER::xil_bram_gen"} {
eval "::AESL_LIB_XILADAPTER::xil_bram_gen { \
    id 138 \
    name capsule2_key \
    reset_level 1 \
    sync_rst true \
    dir I \
    corename capsule2_key \
    op interface \
    ports { capsule2_key_address0 { O 5 vector } capsule2_key_ce0 { O 1 bit } capsule2_key_q0 { I 32 vector } capsule2_key_address1 { O 5 vector } capsule2_key_ce1 { O 1 bit } capsule2_key_q1 { I 32 vector } } \
} "
} else {
puts "@W \[IMPL-110\] Cannot find bus interface model in the library. Ignored generation of bus interface for 'capsule2_key'"
}
}


# XIL_BRAM:
if {${::AESL::PGuard_autoexp_gen}} {
if {[info proc ::AESL_LIB_XILADAPTER::xil_bram_gen] == "::AESL_LIB_XILADAPTER::xil_bram_gen"} {
eval "::AESL_LIB_XILADAPTER::xil_bram_gen { \
    id 139 \
    name capsule2_value \
    reset_level 1 \
    sync_rst true \
    dir I \
    corename capsule2_value \
    op interface \
    ports { capsule2_value_address0 { O 5 vector } capsule2_value_ce0 { O 1 bit } capsule2_value_q0 { I 32 vector } } \
} "
} else {
puts "@W \[IMPL-110\] Cannot find bus interface model in the library. Ignored generation of bus interface for 'capsule2_value'"
}
}


# XIL_BRAM:
if {${::AESL::PGuard_autoexp_gen}} {
if {[info proc ::AESL_LIB_XILADAPTER::xil_bram_gen] == "::AESL_LIB_XILADAPTER::xil_bram_gen"} {
eval "::AESL_LIB_XILADAPTER::xil_bram_gen { \
    id 140 \
    name capsule3_key \
    reset_level 1 \
    sync_rst true \
    dir I \
    corename capsule3_key \
    op interface \
    ports { capsule3_key_address0 { O 5 vector } capsule3_key_ce0 { O 1 bit } capsule3_key_q0 { I 32 vector } capsule3_key_address1 { O 5 vector } capsule3_key_ce1 { O 1 bit } capsule3_key_q1 { I 32 vector } } \
} "
} else {
puts "@W \[IMPL-110\] Cannot find bus interface model in the library. Ignored generation of bus interface for 'capsule3_key'"
}
}


# XIL_BRAM:
if {${::AESL::PGuard_autoexp_gen}} {
if {[info proc ::AESL_LIB_XILADAPTER::xil_bram_gen] == "::AESL_LIB_XILADAPTER::xil_bram_gen"} {
eval "::AESL_LIB_XILADAPTER::xil_bram_gen { \
    id 141 \
    name capsule3_value \
    reset_level 1 \
    sync_rst true \
    dir I \
    corename capsule3_value \
    op interface \
    ports { capsule3_value_address0 { O 5 vector } capsule3_value_ce0 { O 1 bit } capsule3_value_q0 { I 32 vector } } \
} "
} else {
puts "@W \[IMPL-110\] Cannot find bus interface model in the library. Ignored generation of bus interface for 'capsule3_value'"
}
}


# XIL_BRAM:
if {${::AESL::PGuard_autoexp_gen}} {
if {[info proc ::AESL_LIB_XILADAPTER::xil_bram_gen] == "::AESL_LIB_XILADAPTER::xil_bram_gen"} {
eval "::AESL_LIB_XILADAPTER::xil_bram_gen { \
    id 142 \
    name kvdeststats_tmp_key \
    reset_level 1 \
    sync_rst true \
    dir I \
    corename kvdeststats_tmp_key \
    op interface \
    ports { kvdeststats_tmp_key_address0 { O 5 vector } kvdeststats_tmp_key_ce0 { O 1 bit } kvdeststats_tmp_key_q0 { I 32 vector } kvdeststats_tmp_key_address1 { O 5 vector } kvdeststats_tmp_key_ce1 { O 1 bit } kvdeststats_tmp_key_q1 { I 32 vector } } \
} "
} else {
puts "@W \[IMPL-110\] Cannot find bus interface model in the library. Ignored generation of bus interface for 'kvdeststats_tmp_key'"
}
}


# XIL_BRAM:
if {${::AESL::PGuard_autoexp_gen}} {
if {[info proc ::AESL_LIB_XILADAPTER::xil_bram_gen] == "::AESL_LIB_XILADAPTER::xil_bram_gen"} {
eval "::AESL_LIB_XILADAPTER::xil_bram_gen { \
    id 143 \
    name kvdeststats_tmp_valu \
    reset_level 1 \
    sync_rst true \
    dir IO \
    corename kvdeststats_tmp_valu \
    op interface \
    ports { kvdeststats_tmp_valu_address0 { O 5 vector } kvdeststats_tmp_valu_ce0 { O 1 bit } kvdeststats_tmp_valu_q0 { I 32 vector } kvdeststats_tmp_valu_address1 { O 5 vector } kvdeststats_tmp_valu_ce1 { O 1 bit } kvdeststats_tmp_valu_we1 { O 1 bit } kvdeststats_tmp_valu_d1 { O 32 vector } kvdeststats_tmp_valu_q1 { I 32 vector } } \
} "
} else {
puts "@W \[IMPL-110\] Cannot find bus interface model in the library. Ignored generation of bus interface for 'kvdeststats_tmp_valu'"
}
}


# Direct connection:
if {${::AESL::PGuard_autoexp_gen}} {
eval "cg_default_interface_gen_dc { \
    id 128 \
    name kvdram_V \
    type other \
    dir O \
    reset_level 1 \
    sync_rst true \
    corename dc_kvdram_V \
    op interface \
    ports { m_axi_kvdram_V_AWVALID { O 1 bit } m_axi_kvdram_V_AWREADY { I 1 bit } m_axi_kvdram_V_AWADDR { O 32 vector } m_axi_kvdram_V_AWID { O 1 vector } m_axi_kvdram_V_AWLEN { O 32 vector } m_axi_kvdram_V_AWSIZE { O 3 vector } m_axi_kvdram_V_AWBURST { O 2 vector } m_axi_kvdram_V_AWLOCK { O 2 vector } m_axi_kvdram_V_AWCACHE { O 4 vector } m_axi_kvdram_V_AWPROT { O 3 vector } m_axi_kvdram_V_AWQOS { O 4 vector } m_axi_kvdram_V_AWREGION { O 4 vector } m_axi_kvdram_V_AWUSER { O 1 vector } m_axi_kvdram_V_WVALID { O 1 bit } m_axi_kvdram_V_WREADY { I 1 bit } m_axi_kvdram_V_WDATA { O 512 vector } m_axi_kvdram_V_WSTRB { O 64 vector } m_axi_kvdram_V_WLAST { O 1 bit } m_axi_kvdram_V_WID { O 1 vector } m_axi_kvdram_V_WUSER { O 1 vector } m_axi_kvdram_V_ARVALID { O 1 bit } m_axi_kvdram_V_ARREADY { I 1 bit } m_axi_kvdram_V_ARADDR { O 32 vector } m_axi_kvdram_V_ARID { O 1 vector } m_axi_kvdram_V_ARLEN { O 32 vector } m_axi_kvdram_V_ARSIZE { O 3 vector } m_axi_kvdram_V_ARBURST { O 2 vector } m_axi_kvdram_V_ARLOCK { O 2 vector } m_axi_kvdram_V_ARCACHE { O 4 vector } m_axi_kvdram_V_ARPROT { O 3 vector } m_axi_kvdram_V_ARQOS { O 4 vector } m_axi_kvdram_V_ARREGION { O 4 vector } m_axi_kvdram_V_ARUSER { O 1 vector } m_axi_kvdram_V_RVALID { I 1 bit } m_axi_kvdram_V_RREADY { O 1 bit } m_axi_kvdram_V_RDATA { I 512 vector } m_axi_kvdram_V_RLAST { I 1 bit } m_axi_kvdram_V_RID { I 1 vector } m_axi_kvdram_V_RUSER { I 1 vector } m_axi_kvdram_V_RRESP { I 2 vector } m_axi_kvdram_V_BVALID { I 1 bit } m_axi_kvdram_V_BREADY { O 1 bit } m_axi_kvdram_V_BRESP { I 2 vector } m_axi_kvdram_V_BID { I 1 vector } m_axi_kvdram_V_BUSER { I 1 vector } } \
} "
}

# Direct connection:
if {${::AESL::PGuard_autoexp_gen}} {
eval "cg_default_interface_gen_dc { \
    id 129 \
    name kvdram_V_offset \
    type other \
    dir I \
    reset_level 1 \
    sync_rst true \
    corename dc_kvdram_V_offset \
    op interface \
    ports { kvdram_V_offset { I 26 vector } } \
} "
}

# Direct connection:
if {${::AESL::PGuard_autoexp_gen}} {
eval "cg_default_interface_gen_dc { \
    id 144 \
    name kvdrambaseaddress \
    type other \
    dir I \
    reset_level 1 \
    sync_rst true \
    corename dc_kvdrambaseaddress \
    op interface \
    ports { kvdrambaseaddress { I 25 vector } } \
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


