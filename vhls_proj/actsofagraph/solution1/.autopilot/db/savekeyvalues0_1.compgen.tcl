# This script segment is generated automatically by AutoPilot

# clear list
if {${::AESL::PGuard_autoexp_gen}} {
    cg_default_interface_gen_dc_begin
    cg_default_interface_gen_bundle_begin
    AESL_LIB_XILADAPTER::native_axis_begin
}

# Direct connection:
if {${::AESL::PGuard_autoexp_gen}} {
eval "cg_default_interface_gen_dc { \
    id 350 \
    name dram \
    type other \
    dir O \
    reset_level 1 \
    sync_rst true \
    corename dc_dram \
    op interface \
    ports { m_axi_dram_AWVALID { O 1 bit } m_axi_dram_AWREADY { I 1 bit } m_axi_dram_AWADDR { O 32 vector } m_axi_dram_AWID { O 1 vector } m_axi_dram_AWLEN { O 32 vector } m_axi_dram_AWSIZE { O 3 vector } m_axi_dram_AWBURST { O 2 vector } m_axi_dram_AWLOCK { O 2 vector } m_axi_dram_AWCACHE { O 4 vector } m_axi_dram_AWPROT { O 3 vector } m_axi_dram_AWQOS { O 4 vector } m_axi_dram_AWREGION { O 4 vector } m_axi_dram_AWUSER { O 1 vector } m_axi_dram_WVALID { O 1 bit } m_axi_dram_WREADY { I 1 bit } m_axi_dram_WDATA { O 64 vector } m_axi_dram_WSTRB { O 8 vector } m_axi_dram_WLAST { O 1 bit } m_axi_dram_WID { O 1 vector } m_axi_dram_WUSER { O 1 vector } m_axi_dram_ARVALID { O 1 bit } m_axi_dram_ARREADY { I 1 bit } m_axi_dram_ARADDR { O 32 vector } m_axi_dram_ARID { O 1 vector } m_axi_dram_ARLEN { O 32 vector } m_axi_dram_ARSIZE { O 3 vector } m_axi_dram_ARBURST { O 2 vector } m_axi_dram_ARLOCK { O 2 vector } m_axi_dram_ARCACHE { O 4 vector } m_axi_dram_ARPROT { O 3 vector } m_axi_dram_ARQOS { O 4 vector } m_axi_dram_ARREGION { O 4 vector } m_axi_dram_ARUSER { O 1 vector } m_axi_dram_RVALID { I 1 bit } m_axi_dram_RREADY { O 1 bit } m_axi_dram_RDATA { I 64 vector } m_axi_dram_RLAST { I 1 bit } m_axi_dram_RID { I 1 vector } m_axi_dram_RUSER { I 1 vector } m_axi_dram_RRESP { I 2 vector } m_axi_dram_BVALID { I 1 bit } m_axi_dram_BREADY { O 1 bit } m_axi_dram_BRESP { I 2 vector } m_axi_dram_BID { I 1 vector } m_axi_dram_BUSER { I 1 vector } } \
} "
}

# Direct connection:
if {${::AESL::PGuard_autoexp_gen}} {
eval "cg_default_interface_gen_dc { \
    id 351 \
    name dram_offset \
    type other \
    dir I \
    reset_level 1 \
    sync_rst true \
    corename dc_dram_offset \
    op interface \
    ports { dram_offset { I 29 vector } } \
} "
}

# Direct connection:
if {${::AESL::PGuard_autoexp_gen}} {
eval "cg_default_interface_gen_dc { \
    id 352 \
    name buffer_0_key_read \
    type other \
    dir I \
    reset_level 1 \
    sync_rst true \
    corename dc_buffer_0_key_read \
    op interface \
    ports { buffer_0_key_read { I 32 vector } } \
} "
}

# Direct connection:
if {${::AESL::PGuard_autoexp_gen}} {
eval "cg_default_interface_gen_dc { \
    id 353 \
    name buffer_0_value_read \
    type other \
    dir I \
    reset_level 1 \
    sync_rst true \
    corename dc_buffer_0_value_read \
    op interface \
    ports { buffer_0_value_read { I 32 vector } } \
} "
}

# Direct connection:
if {${::AESL::PGuard_autoexp_gen}} {
eval "cg_default_interface_gen_dc { \
    id 354 \
    name offset_kvs \
    type other \
    dir I \
    reset_level 1 \
    sync_rst true \
    corename dc_offset_kvs \
    op interface \
    ports { offset_kvs { I 32 vector } } \
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


