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
    id 333 \
    name buffer30_key \
    reset_level 1 \
    sync_rst true \
    dir I \
    corename buffer30_key \
    op interface \
    ports { buffer30_key_address0 { O 12 vector } buffer30_key_ce0 { O 1 bit } buffer30_key_q0 { I 32 vector } } \
} "
} else {
puts "@W \[IMPL-110\] Cannot find bus interface model in the library. Ignored generation of bus interface for 'buffer30_key'"
}
}


# XIL_BRAM:
if {${::AESL::PGuard_autoexp_gen}} {
if {[info proc ::AESL_LIB_XILADAPTER::xil_bram_gen] == "::AESL_LIB_XILADAPTER::xil_bram_gen"} {
eval "::AESL_LIB_XILADAPTER::xil_bram_gen { \
    id 334 \
    name buffer30_value \
    reset_level 1 \
    sync_rst true \
    dir I \
    corename buffer30_value \
    op interface \
    ports { buffer30_value_address0 { O 12 vector } buffer30_value_ce0 { O 1 bit } buffer30_value_q0 { I 32 vector } } \
} "
} else {
puts "@W \[IMPL-110\] Cannot find bus interface model in the library. Ignored generation of bus interface for 'buffer30_value'"
}
}


# XIL_BRAM:
if {${::AESL::PGuard_autoexp_gen}} {
if {[info proc ::AESL_LIB_XILADAPTER::xil_bram_gen] == "::AESL_LIB_XILADAPTER::xil_bram_gen"} {
eval "::AESL_LIB_XILADAPTER::xil_bram_gen { \
    id 335 \
    name buffer31_key \
    reset_level 1 \
    sync_rst true \
    dir I \
    corename buffer31_key \
    op interface \
    ports { buffer31_key_address0 { O 12 vector } buffer31_key_ce0 { O 1 bit } buffer31_key_q0 { I 32 vector } } \
} "
} else {
puts "@W \[IMPL-110\] Cannot find bus interface model in the library. Ignored generation of bus interface for 'buffer31_key'"
}
}


# XIL_BRAM:
if {${::AESL::PGuard_autoexp_gen}} {
if {[info proc ::AESL_LIB_XILADAPTER::xil_bram_gen] == "::AESL_LIB_XILADAPTER::xil_bram_gen"} {
eval "::AESL_LIB_XILADAPTER::xil_bram_gen { \
    id 336 \
    name buffer31_value \
    reset_level 1 \
    sync_rst true \
    dir I \
    corename buffer31_value \
    op interface \
    ports { buffer31_value_address0 { O 12 vector } buffer31_value_ce0 { O 1 bit } buffer31_value_q0 { I 32 vector } } \
} "
} else {
puts "@W \[IMPL-110\] Cannot find bus interface model in the library. Ignored generation of bus interface for 'buffer31_value'"
}
}


# XIL_BRAM:
if {${::AESL::PGuard_autoexp_gen}} {
if {[info proc ::AESL_LIB_XILADAPTER::xil_bram_gen] == "::AESL_LIB_XILADAPTER::xil_bram_gen"} {
eval "::AESL_LIB_XILADAPTER::xil_bram_gen { \
    id 337 \
    name buffer32_key \
    reset_level 1 \
    sync_rst true \
    dir I \
    corename buffer32_key \
    op interface \
    ports { buffer32_key_address0 { O 12 vector } buffer32_key_ce0 { O 1 bit } buffer32_key_q0 { I 32 vector } } \
} "
} else {
puts "@W \[IMPL-110\] Cannot find bus interface model in the library. Ignored generation of bus interface for 'buffer32_key'"
}
}


# XIL_BRAM:
if {${::AESL::PGuard_autoexp_gen}} {
if {[info proc ::AESL_LIB_XILADAPTER::xil_bram_gen] == "::AESL_LIB_XILADAPTER::xil_bram_gen"} {
eval "::AESL_LIB_XILADAPTER::xil_bram_gen { \
    id 338 \
    name buffer32_value \
    reset_level 1 \
    sync_rst true \
    dir I \
    corename buffer32_value \
    op interface \
    ports { buffer32_value_address0 { O 12 vector } buffer32_value_ce0 { O 1 bit } buffer32_value_q0 { I 32 vector } } \
} "
} else {
puts "@W \[IMPL-110\] Cannot find bus interface model in the library. Ignored generation of bus interface for 'buffer32_value'"
}
}


# XIL_BRAM:
if {${::AESL::PGuard_autoexp_gen}} {
if {[info proc ::AESL_LIB_XILADAPTER::xil_bram_gen] == "::AESL_LIB_XILADAPTER::xil_bram_gen"} {
eval "::AESL_LIB_XILADAPTER::xil_bram_gen { \
    id 339 \
    name buffer33_key \
    reset_level 1 \
    sync_rst true \
    dir I \
    corename buffer33_key \
    op interface \
    ports { buffer33_key_address0 { O 12 vector } buffer33_key_ce0 { O 1 bit } buffer33_key_q0 { I 32 vector } } \
} "
} else {
puts "@W \[IMPL-110\] Cannot find bus interface model in the library. Ignored generation of bus interface for 'buffer33_key'"
}
}


# XIL_BRAM:
if {${::AESL::PGuard_autoexp_gen}} {
if {[info proc ::AESL_LIB_XILADAPTER::xil_bram_gen] == "::AESL_LIB_XILADAPTER::xil_bram_gen"} {
eval "::AESL_LIB_XILADAPTER::xil_bram_gen { \
    id 340 \
    name buffer33_value \
    reset_level 1 \
    sync_rst true \
    dir I \
    corename buffer33_value \
    op interface \
    ports { buffer33_value_address0 { O 12 vector } buffer33_value_ce0 { O 1 bit } buffer33_value_q0 { I 32 vector } } \
} "
} else {
puts "@W \[IMPL-110\] Cannot find bus interface model in the library. Ignored generation of bus interface for 'buffer33_value'"
}
}


# XIL_BRAM:
if {${::AESL::PGuard_autoexp_gen}} {
if {[info proc ::AESL_LIB_XILADAPTER::xil_bram_gen] == "::AESL_LIB_XILADAPTER::xil_bram_gen"} {
eval "::AESL_LIB_XILADAPTER::xil_bram_gen { \
    id 341 \
    name buffer34_key \
    reset_level 1 \
    sync_rst true \
    dir I \
    corename buffer34_key \
    op interface \
    ports { buffer34_key_address0 { O 12 vector } buffer34_key_ce0 { O 1 bit } buffer34_key_q0 { I 32 vector } } \
} "
} else {
puts "@W \[IMPL-110\] Cannot find bus interface model in the library. Ignored generation of bus interface for 'buffer34_key'"
}
}


# XIL_BRAM:
if {${::AESL::PGuard_autoexp_gen}} {
if {[info proc ::AESL_LIB_XILADAPTER::xil_bram_gen] == "::AESL_LIB_XILADAPTER::xil_bram_gen"} {
eval "::AESL_LIB_XILADAPTER::xil_bram_gen { \
    id 342 \
    name buffer34_value \
    reset_level 1 \
    sync_rst true \
    dir I \
    corename buffer34_value \
    op interface \
    ports { buffer34_value_address0 { O 12 vector } buffer34_value_ce0 { O 1 bit } buffer34_value_q0 { I 32 vector } } \
} "
} else {
puts "@W \[IMPL-110\] Cannot find bus interface model in the library. Ignored generation of bus interface for 'buffer34_value'"
}
}


# XIL_BRAM:
if {${::AESL::PGuard_autoexp_gen}} {
if {[info proc ::AESL_LIB_XILADAPTER::xil_bram_gen] == "::AESL_LIB_XILADAPTER::xil_bram_gen"} {
eval "::AESL_LIB_XILADAPTER::xil_bram_gen { \
    id 343 \
    name buffer35_key \
    reset_level 1 \
    sync_rst true \
    dir I \
    corename buffer35_key \
    op interface \
    ports { buffer35_key_address0 { O 12 vector } buffer35_key_ce0 { O 1 bit } buffer35_key_q0 { I 32 vector } } \
} "
} else {
puts "@W \[IMPL-110\] Cannot find bus interface model in the library. Ignored generation of bus interface for 'buffer35_key'"
}
}


# XIL_BRAM:
if {${::AESL::PGuard_autoexp_gen}} {
if {[info proc ::AESL_LIB_XILADAPTER::xil_bram_gen] == "::AESL_LIB_XILADAPTER::xil_bram_gen"} {
eval "::AESL_LIB_XILADAPTER::xil_bram_gen { \
    id 344 \
    name buffer35_value \
    reset_level 1 \
    sync_rst true \
    dir I \
    corename buffer35_value \
    op interface \
    ports { buffer35_value_address0 { O 12 vector } buffer35_value_ce0 { O 1 bit } buffer35_value_q0 { I 32 vector } } \
} "
} else {
puts "@W \[IMPL-110\] Cannot find bus interface model in the library. Ignored generation of bus interface for 'buffer35_value'"
}
}


# XIL_BRAM:
if {${::AESL::PGuard_autoexp_gen}} {
if {[info proc ::AESL_LIB_XILADAPTER::xil_bram_gen] == "::AESL_LIB_XILADAPTER::xil_bram_gen"} {
eval "::AESL_LIB_XILADAPTER::xil_bram_gen { \
    id 345 \
    name buffer36_key \
    reset_level 1 \
    sync_rst true \
    dir I \
    corename buffer36_key \
    op interface \
    ports { buffer36_key_address0 { O 12 vector } buffer36_key_ce0 { O 1 bit } buffer36_key_q0 { I 32 vector } } \
} "
} else {
puts "@W \[IMPL-110\] Cannot find bus interface model in the library. Ignored generation of bus interface for 'buffer36_key'"
}
}


# XIL_BRAM:
if {${::AESL::PGuard_autoexp_gen}} {
if {[info proc ::AESL_LIB_XILADAPTER::xil_bram_gen] == "::AESL_LIB_XILADAPTER::xil_bram_gen"} {
eval "::AESL_LIB_XILADAPTER::xil_bram_gen { \
    id 346 \
    name buffer36_value \
    reset_level 1 \
    sync_rst true \
    dir I \
    corename buffer36_value \
    op interface \
    ports { buffer36_value_address0 { O 12 vector } buffer36_value_ce0 { O 1 bit } buffer36_value_q0 { I 32 vector } } \
} "
} else {
puts "@W \[IMPL-110\] Cannot find bus interface model in the library. Ignored generation of bus interface for 'buffer36_value'"
}
}


# XIL_BRAM:
if {${::AESL::PGuard_autoexp_gen}} {
if {[info proc ::AESL_LIB_XILADAPTER::xil_bram_gen] == "::AESL_LIB_XILADAPTER::xil_bram_gen"} {
eval "::AESL_LIB_XILADAPTER::xil_bram_gen { \
    id 347 \
    name buffer37_key \
    reset_level 1 \
    sync_rst true \
    dir I \
    corename buffer37_key \
    op interface \
    ports { buffer37_key_address0 { O 12 vector } buffer37_key_ce0 { O 1 bit } buffer37_key_q0 { I 32 vector } } \
} "
} else {
puts "@W \[IMPL-110\] Cannot find bus interface model in the library. Ignored generation of bus interface for 'buffer37_key'"
}
}


# XIL_BRAM:
if {${::AESL::PGuard_autoexp_gen}} {
if {[info proc ::AESL_LIB_XILADAPTER::xil_bram_gen] == "::AESL_LIB_XILADAPTER::xil_bram_gen"} {
eval "::AESL_LIB_XILADAPTER::xil_bram_gen { \
    id 348 \
    name buffer37_value \
    reset_level 1 \
    sync_rst true \
    dir I \
    corename buffer37_value \
    op interface \
    ports { buffer37_value_address0 { O 12 vector } buffer37_value_ce0 { O 1 bit } buffer37_value_q0 { I 32 vector } } \
} "
} else {
puts "@W \[IMPL-110\] Cannot find bus interface model in the library. Ignored generation of bus interface for 'buffer37_value'"
}
}


# Direct connection:
if {${::AESL::PGuard_autoexp_gen}} {
eval "cg_default_interface_gen_dc { \
    id 331 \
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
    id 332 \
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
    id 349 \
    name offset_kvs \
    type other \
    dir I \
    reset_level 1 \
    sync_rst true \
    corename dc_offset_kvs \
    op interface \
    ports { offset_kvs { I 30 vector } } \
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


