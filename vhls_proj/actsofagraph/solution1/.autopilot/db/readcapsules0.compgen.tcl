# This script segment is generated automatically by AutoPilot

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
    id 70 \
    name BIGcapsule0_V \
    reset_level 1 \
    sync_rst true \
    dir IO \
    corename BIGcapsule0_V \
    op interface \
    ports { BIGcapsule0_V_address0 { O 9 vector } BIGcapsule0_V_ce0 { O 1 bit } BIGcapsule0_V_we0 { O 64 vector } BIGcapsule0_V_d0 { O 512 vector } BIGcapsule0_V_q0 { I 512 vector } } \
} "
} else {
puts "@W \[IMPL-110\] Cannot find bus interface model in the library. Ignored generation of bus interface for 'BIGcapsule0_V'"
}
}


# XIL_BRAM:
if {${::AESL::PGuard_autoexp_gen}} {
if {[info proc ::AESL_LIB_XILADAPTER::xil_bram_gen] == "::AESL_LIB_XILADAPTER::xil_bram_gen"} {
eval "::AESL_LIB_XILADAPTER::xil_bram_gen { \
    id 71 \
    name BIGcapsule1_V \
    reset_level 1 \
    sync_rst true \
    dir IO \
    corename BIGcapsule1_V \
    op interface \
    ports { BIGcapsule1_V_address0 { O 9 vector } BIGcapsule1_V_ce0 { O 1 bit } BIGcapsule1_V_we0 { O 64 vector } BIGcapsule1_V_d0 { O 512 vector } BIGcapsule1_V_q0 { I 512 vector } } \
} "
} else {
puts "@W \[IMPL-110\] Cannot find bus interface model in the library. Ignored generation of bus interface for 'BIGcapsule1_V'"
}
}


# XIL_BRAM:
if {${::AESL::PGuard_autoexp_gen}} {
if {[info proc ::AESL_LIB_XILADAPTER::xil_bram_gen] == "::AESL_LIB_XILADAPTER::xil_bram_gen"} {
eval "::AESL_LIB_XILADAPTER::xil_bram_gen { \
    id 72 \
    name BIGcapsule2_V \
    reset_level 1 \
    sync_rst true \
    dir IO \
    corename BIGcapsule2_V \
    op interface \
    ports { BIGcapsule2_V_address0 { O 9 vector } BIGcapsule2_V_ce0 { O 1 bit } BIGcapsule2_V_we0 { O 64 vector } BIGcapsule2_V_d0 { O 512 vector } BIGcapsule2_V_q0 { I 512 vector } } \
} "
} else {
puts "@W \[IMPL-110\] Cannot find bus interface model in the library. Ignored generation of bus interface for 'BIGcapsule2_V'"
}
}


# XIL_BRAM:
if {${::AESL::PGuard_autoexp_gen}} {
if {[info proc ::AESL_LIB_XILADAPTER::xil_bram_gen] == "::AESL_LIB_XILADAPTER::xil_bram_gen"} {
eval "::AESL_LIB_XILADAPTER::xil_bram_gen { \
    id 73 \
    name BIGcapsule3_V \
    reset_level 1 \
    sync_rst true \
    dir IO \
    corename BIGcapsule3_V \
    op interface \
    ports { BIGcapsule3_V_address0 { O 9 vector } BIGcapsule3_V_ce0 { O 1 bit } BIGcapsule3_V_we0 { O 64 vector } BIGcapsule3_V_d0 { O 512 vector } BIGcapsule3_V_q0 { I 512 vector } } \
} "
} else {
puts "@W \[IMPL-110\] Cannot find bus interface model in the library. Ignored generation of bus interface for 'BIGcapsule3_V'"
}
}


# XIL_BRAM:
if {${::AESL::PGuard_autoexp_gen}} {
if {[info proc ::AESL_LIB_XILADAPTER::xil_bram_gen] == "::AESL_LIB_XILADAPTER::xil_bram_gen"} {
eval "::AESL_LIB_XILADAPTER::xil_bram_gen { \
    id 74 \
    name capsule0_key \
    reset_level 1 \
    sync_rst true \
    dir O \
    corename capsule0_key \
    op interface \
    ports { capsule0_key_address0 { O 5 vector } capsule0_key_ce0 { O 1 bit } capsule0_key_we0 { O 1 bit } capsule0_key_d0 { O 32 vector } } \
} "
} else {
puts "@W \[IMPL-110\] Cannot find bus interface model in the library. Ignored generation of bus interface for 'capsule0_key'"
}
}


# XIL_BRAM:
if {${::AESL::PGuard_autoexp_gen}} {
if {[info proc ::AESL_LIB_XILADAPTER::xil_bram_gen] == "::AESL_LIB_XILADAPTER::xil_bram_gen"} {
eval "::AESL_LIB_XILADAPTER::xil_bram_gen { \
    id 75 \
    name capsule0_value \
    reset_level 1 \
    sync_rst true \
    dir O \
    corename capsule0_value \
    op interface \
    ports { capsule0_value_address0 { O 5 vector } capsule0_value_ce0 { O 1 bit } capsule0_value_we0 { O 1 bit } capsule0_value_d0 { O 32 vector } } \
} "
} else {
puts "@W \[IMPL-110\] Cannot find bus interface model in the library. Ignored generation of bus interface for 'capsule0_value'"
}
}


# XIL_BRAM:
if {${::AESL::PGuard_autoexp_gen}} {
if {[info proc ::AESL_LIB_XILADAPTER::xil_bram_gen] == "::AESL_LIB_XILADAPTER::xil_bram_gen"} {
eval "::AESL_LIB_XILADAPTER::xil_bram_gen { \
    id 76 \
    name capsule1_key \
    reset_level 1 \
    sync_rst true \
    dir O \
    corename capsule1_key \
    op interface \
    ports { capsule1_key_address0 { O 5 vector } capsule1_key_ce0 { O 1 bit } capsule1_key_we0 { O 1 bit } capsule1_key_d0 { O 32 vector } } \
} "
} else {
puts "@W \[IMPL-110\] Cannot find bus interface model in the library. Ignored generation of bus interface for 'capsule1_key'"
}
}


# XIL_BRAM:
if {${::AESL::PGuard_autoexp_gen}} {
if {[info proc ::AESL_LIB_XILADAPTER::xil_bram_gen] == "::AESL_LIB_XILADAPTER::xil_bram_gen"} {
eval "::AESL_LIB_XILADAPTER::xil_bram_gen { \
    id 77 \
    name capsule1_value \
    reset_level 1 \
    sync_rst true \
    dir O \
    corename capsule1_value \
    op interface \
    ports { capsule1_value_address0 { O 5 vector } capsule1_value_ce0 { O 1 bit } capsule1_value_we0 { O 1 bit } capsule1_value_d0 { O 32 vector } } \
} "
} else {
puts "@W \[IMPL-110\] Cannot find bus interface model in the library. Ignored generation of bus interface for 'capsule1_value'"
}
}


# XIL_BRAM:
if {${::AESL::PGuard_autoexp_gen}} {
if {[info proc ::AESL_LIB_XILADAPTER::xil_bram_gen] == "::AESL_LIB_XILADAPTER::xil_bram_gen"} {
eval "::AESL_LIB_XILADAPTER::xil_bram_gen { \
    id 78 \
    name capsule2_key \
    reset_level 1 \
    sync_rst true \
    dir O \
    corename capsule2_key \
    op interface \
    ports { capsule2_key_address0 { O 5 vector } capsule2_key_ce0 { O 1 bit } capsule2_key_we0 { O 1 bit } capsule2_key_d0 { O 32 vector } } \
} "
} else {
puts "@W \[IMPL-110\] Cannot find bus interface model in the library. Ignored generation of bus interface for 'capsule2_key'"
}
}


# XIL_BRAM:
if {${::AESL::PGuard_autoexp_gen}} {
if {[info proc ::AESL_LIB_XILADAPTER::xil_bram_gen] == "::AESL_LIB_XILADAPTER::xil_bram_gen"} {
eval "::AESL_LIB_XILADAPTER::xil_bram_gen { \
    id 79 \
    name capsule2_value \
    reset_level 1 \
    sync_rst true \
    dir O \
    corename capsule2_value \
    op interface \
    ports { capsule2_value_address0 { O 5 vector } capsule2_value_ce0 { O 1 bit } capsule2_value_we0 { O 1 bit } capsule2_value_d0 { O 32 vector } } \
} "
} else {
puts "@W \[IMPL-110\] Cannot find bus interface model in the library. Ignored generation of bus interface for 'capsule2_value'"
}
}


# XIL_BRAM:
if {${::AESL::PGuard_autoexp_gen}} {
if {[info proc ::AESL_LIB_XILADAPTER::xil_bram_gen] == "::AESL_LIB_XILADAPTER::xil_bram_gen"} {
eval "::AESL_LIB_XILADAPTER::xil_bram_gen { \
    id 80 \
    name capsule3_key \
    reset_level 1 \
    sync_rst true \
    dir O \
    corename capsule3_key \
    op interface \
    ports { capsule3_key_address0 { O 5 vector } capsule3_key_ce0 { O 1 bit } capsule3_key_we0 { O 1 bit } capsule3_key_d0 { O 32 vector } } \
} "
} else {
puts "@W \[IMPL-110\] Cannot find bus interface model in the library. Ignored generation of bus interface for 'capsule3_key'"
}
}


# XIL_BRAM:
if {${::AESL::PGuard_autoexp_gen}} {
if {[info proc ::AESL_LIB_XILADAPTER::xil_bram_gen] == "::AESL_LIB_XILADAPTER::xil_bram_gen"} {
eval "::AESL_LIB_XILADAPTER::xil_bram_gen { \
    id 81 \
    name capsule3_value \
    reset_level 1 \
    sync_rst true \
    dir O \
    corename capsule3_value \
    op interface \
    ports { capsule3_value_address0 { O 5 vector } capsule3_value_ce0 { O 1 bit } capsule3_value_we0 { O 1 bit } capsule3_value_d0 { O 32 vector } } \
} "
} else {
puts "@W \[IMPL-110\] Cannot find bus interface model in the library. Ignored generation of bus interface for 'capsule3_value'"
}
}


# Direct connection:
if {${::AESL::PGuard_autoexp_gen}} {
eval "cg_default_interface_gen_dc { \
    id 68 \
    name kvdram_V \
    type other \
    dir I \
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
    id 69 \
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
    id 82 \
    name offset \
    type other \
    dir I \
    reset_level 1 \
    sync_rst true \
    corename dc_offset \
    op interface \
    ports { offset { I 24 vector } } \
} "
}

# Direct connection:
if {${::AESL::PGuard_autoexp_gen}} {
eval "cg_default_interface_gen_dc { \
    id 83 \
    name capsulemetadata_globaloffset \
    type other \
    dir I \
    reset_level 1 \
    sync_rst true \
    corename dc_capsulemetadata_globaloffset \
    op interface \
    ports { capsulemetadata_globaloffset { I 32 vector } } \
} "
}

# Direct connection:
if {${::AESL::PGuard_autoexp_gen}} {
eval "cg_default_interface_gen_dc { \
    id 84 \
    name capsulemetadata_localoffset \
    type other \
    dir I \
    reset_level 1 \
    sync_rst true \
    corename dc_capsulemetadata_localoffset \
    op interface \
    ports { capsulemetadata_localoffset { I 32 vector } } \
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

# Direct connection:
if {${::AESL::PGuard_autoexp_gen}} {
eval "cg_default_interface_gen_dc { \
    id -2 \
    name ap_return \
    type ap_return \
    reset_level 1 \
    sync_rst true \
    corename ap_return \
    op interface \
    ports { ap_return { O 1 vector } } \
} "
}


# Adapter definition:
set PortName ap_clk
set DataWd 1 
if {${::AESL::PGuard_autoexp_gen}} {
if {[info proc cg_default_interface_gen_clock] == "cg_default_interface_gen_clock"} {
eval "cg_default_interface_gen_clock { \
    id -3 \
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
    id -4 \
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


