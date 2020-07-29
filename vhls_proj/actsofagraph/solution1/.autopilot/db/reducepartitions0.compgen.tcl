# This script segment is generated automatically by AutoPilot

set id 228
set name topkernel_mux_83_32_1_1
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
set din4_width 32
set din4_signed 0
set din5_width 32
set din5_signed 0
set din6_width 32
set din6_signed 0
set din7_width 32
set din7_signed 0
set din8_width 3
set din8_signed 0
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
    din5_width ${din5_width} \
    din5_signed ${din5_signed} \
    din6_width ${din6_width} \
    din6_signed ${din6_signed} \
    din7_width ${din7_width} \
    din7_signed ${din7_signed} \
    din8_width ${din8_width} \
    din8_signed ${din8_signed} \
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
    din5_width ${din5_width} \
    din5_signed ${din5_signed} \
    din6_width ${din6_width} \
    din6_signed ${din6_signed} \
    din7_width ${din7_width} \
    din7_signed ${din7_signed} \
    din8_width ${din8_width} \
    din8_signed ${din8_signed} \
    dout_width ${dout_width} \
}"
} else {
puts "@W \[IMPL-101\] Cannot find ::AESL_LIB_VIRTEX::xil_gen_pipemux, check your platform lib"
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
    id 294 \
    name sourcebuffer0_V \
    reset_level 1 \
    sync_rst true \
    dir I \
    corename sourcebuffer0_V \
    op interface \
    ports { sourcebuffer0_V_address0 { O 9 vector } sourcebuffer0_V_ce0 { O 1 bit } sourcebuffer0_V_q0 { I 512 vector } } \
} "
} else {
puts "@W \[IMPL-110\] Cannot find bus interface model in the library. Ignored generation of bus interface for 'sourcebuffer0_V'"
}
}


# XIL_BRAM:
if {${::AESL::PGuard_autoexp_gen}} {
if {[info proc ::AESL_LIB_XILADAPTER::xil_bram_gen] == "::AESL_LIB_XILADAPTER::xil_bram_gen"} {
eval "::AESL_LIB_XILADAPTER::xil_bram_gen { \
    id 295 \
    name sourcebuffer1_V \
    reset_level 1 \
    sync_rst true \
    dir I \
    corename sourcebuffer1_V \
    op interface \
    ports { sourcebuffer1_V_address0 { O 9 vector } sourcebuffer1_V_ce0 { O 1 bit } sourcebuffer1_V_q0 { I 512 vector } } \
} "
} else {
puts "@W \[IMPL-110\] Cannot find bus interface model in the library. Ignored generation of bus interface for 'sourcebuffer1_V'"
}
}


# XIL_BRAM:
if {${::AESL::PGuard_autoexp_gen}} {
if {[info proc ::AESL_LIB_XILADAPTER::xil_bram_gen] == "::AESL_LIB_XILADAPTER::xil_bram_gen"} {
eval "::AESL_LIB_XILADAPTER::xil_bram_gen { \
    id 296 \
    name sourcebuffer2_V \
    reset_level 1 \
    sync_rst true \
    dir I \
    corename sourcebuffer2_V \
    op interface \
    ports { sourcebuffer2_V_address0 { O 9 vector } sourcebuffer2_V_ce0 { O 1 bit } sourcebuffer2_V_q0 { I 512 vector } } \
} "
} else {
puts "@W \[IMPL-110\] Cannot find bus interface model in the library. Ignored generation of bus interface for 'sourcebuffer2_V'"
}
}


# XIL_BRAM:
if {${::AESL::PGuard_autoexp_gen}} {
if {[info proc ::AESL_LIB_XILADAPTER::xil_bram_gen] == "::AESL_LIB_XILADAPTER::xil_bram_gen"} {
eval "::AESL_LIB_XILADAPTER::xil_bram_gen { \
    id 297 \
    name sourcebuffer3_V \
    reset_level 1 \
    sync_rst true \
    dir I \
    corename sourcebuffer3_V \
    op interface \
    ports { sourcebuffer3_V_address0 { O 9 vector } sourcebuffer3_V_ce0 { O 1 bit } sourcebuffer3_V_q0 { I 512 vector } } \
} "
} else {
puts "@W \[IMPL-110\] Cannot find bus interface model in the library. Ignored generation of bus interface for 'sourcebuffer3_V'"
}
}


# XIL_BRAM:
if {${::AESL::PGuard_autoexp_gen}} {
if {[info proc ::AESL_LIB_XILADAPTER::xil_bram_gen] == "::AESL_LIB_XILADAPTER::xil_bram_gen"} {
eval "::AESL_LIB_XILADAPTER::xil_bram_gen { \
    id 298 \
    name destbuffer00_value \
    reset_level 1 \
    sync_rst true \
    dir IO \
    corename destbuffer00_value \
    op interface \
    ports { destbuffer00_value_address0 { O 12 vector } destbuffer00_value_ce0 { O 1 bit } destbuffer00_value_we0 { O 1 bit } destbuffer00_value_d0 { O 32 vector } destbuffer00_value_q0 { I 32 vector } } \
} "
} else {
puts "@W \[IMPL-110\] Cannot find bus interface model in the library. Ignored generation of bus interface for 'destbuffer00_value'"
}
}


# XIL_BRAM:
if {${::AESL::PGuard_autoexp_gen}} {
if {[info proc ::AESL_LIB_XILADAPTER::xil_bram_gen] == "::AESL_LIB_XILADAPTER::xil_bram_gen"} {
eval "::AESL_LIB_XILADAPTER::xil_bram_gen { \
    id 299 \
    name destbuffer01_value \
    reset_level 1 \
    sync_rst true \
    dir IO \
    corename destbuffer01_value \
    op interface \
    ports { destbuffer01_value_address0 { O 12 vector } destbuffer01_value_ce0 { O 1 bit } destbuffer01_value_we0 { O 1 bit } destbuffer01_value_d0 { O 32 vector } destbuffer01_value_q0 { I 32 vector } } \
} "
} else {
puts "@W \[IMPL-110\] Cannot find bus interface model in the library. Ignored generation of bus interface for 'destbuffer01_value'"
}
}


# XIL_BRAM:
if {${::AESL::PGuard_autoexp_gen}} {
if {[info proc ::AESL_LIB_XILADAPTER::xil_bram_gen] == "::AESL_LIB_XILADAPTER::xil_bram_gen"} {
eval "::AESL_LIB_XILADAPTER::xil_bram_gen { \
    id 300 \
    name destbuffer02_value \
    reset_level 1 \
    sync_rst true \
    dir IO \
    corename destbuffer02_value \
    op interface \
    ports { destbuffer02_value_address0 { O 12 vector } destbuffer02_value_ce0 { O 1 bit } destbuffer02_value_we0 { O 1 bit } destbuffer02_value_d0 { O 32 vector } destbuffer02_value_q0 { I 32 vector } } \
} "
} else {
puts "@W \[IMPL-110\] Cannot find bus interface model in the library. Ignored generation of bus interface for 'destbuffer02_value'"
}
}


# XIL_BRAM:
if {${::AESL::PGuard_autoexp_gen}} {
if {[info proc ::AESL_LIB_XILADAPTER::xil_bram_gen] == "::AESL_LIB_XILADAPTER::xil_bram_gen"} {
eval "::AESL_LIB_XILADAPTER::xil_bram_gen { \
    id 301 \
    name destbuffer03_value \
    reset_level 1 \
    sync_rst true \
    dir IO \
    corename destbuffer03_value \
    op interface \
    ports { destbuffer03_value_address0 { O 12 vector } destbuffer03_value_ce0 { O 1 bit } destbuffer03_value_we0 { O 1 bit } destbuffer03_value_d0 { O 32 vector } destbuffer03_value_q0 { I 32 vector } } \
} "
} else {
puts "@W \[IMPL-110\] Cannot find bus interface model in the library. Ignored generation of bus interface for 'destbuffer03_value'"
}
}


# XIL_BRAM:
if {${::AESL::PGuard_autoexp_gen}} {
if {[info proc ::AESL_LIB_XILADAPTER::xil_bram_gen] == "::AESL_LIB_XILADAPTER::xil_bram_gen"} {
eval "::AESL_LIB_XILADAPTER::xil_bram_gen { \
    id 302 \
    name destbuffer04_value \
    reset_level 1 \
    sync_rst true \
    dir IO \
    corename destbuffer04_value \
    op interface \
    ports { destbuffer04_value_address0 { O 12 vector } destbuffer04_value_ce0 { O 1 bit } destbuffer04_value_we0 { O 1 bit } destbuffer04_value_d0 { O 32 vector } destbuffer04_value_q0 { I 32 vector } } \
} "
} else {
puts "@W \[IMPL-110\] Cannot find bus interface model in the library. Ignored generation of bus interface for 'destbuffer04_value'"
}
}


# XIL_BRAM:
if {${::AESL::PGuard_autoexp_gen}} {
if {[info proc ::AESL_LIB_XILADAPTER::xil_bram_gen] == "::AESL_LIB_XILADAPTER::xil_bram_gen"} {
eval "::AESL_LIB_XILADAPTER::xil_bram_gen { \
    id 303 \
    name destbuffer05_value \
    reset_level 1 \
    sync_rst true \
    dir IO \
    corename destbuffer05_value \
    op interface \
    ports { destbuffer05_value_address0 { O 12 vector } destbuffer05_value_ce0 { O 1 bit } destbuffer05_value_we0 { O 1 bit } destbuffer05_value_d0 { O 32 vector } destbuffer05_value_q0 { I 32 vector } } \
} "
} else {
puts "@W \[IMPL-110\] Cannot find bus interface model in the library. Ignored generation of bus interface for 'destbuffer05_value'"
}
}


# XIL_BRAM:
if {${::AESL::PGuard_autoexp_gen}} {
if {[info proc ::AESL_LIB_XILADAPTER::xil_bram_gen] == "::AESL_LIB_XILADAPTER::xil_bram_gen"} {
eval "::AESL_LIB_XILADAPTER::xil_bram_gen { \
    id 304 \
    name destbuffer06_value \
    reset_level 1 \
    sync_rst true \
    dir IO \
    corename destbuffer06_value \
    op interface \
    ports { destbuffer06_value_address0 { O 12 vector } destbuffer06_value_ce0 { O 1 bit } destbuffer06_value_we0 { O 1 bit } destbuffer06_value_d0 { O 32 vector } destbuffer06_value_q0 { I 32 vector } } \
} "
} else {
puts "@W \[IMPL-110\] Cannot find bus interface model in the library. Ignored generation of bus interface for 'destbuffer06_value'"
}
}


# XIL_BRAM:
if {${::AESL::PGuard_autoexp_gen}} {
if {[info proc ::AESL_LIB_XILADAPTER::xil_bram_gen] == "::AESL_LIB_XILADAPTER::xil_bram_gen"} {
eval "::AESL_LIB_XILADAPTER::xil_bram_gen { \
    id 305 \
    name destbuffer07_value \
    reset_level 1 \
    sync_rst true \
    dir IO \
    corename destbuffer07_value \
    op interface \
    ports { destbuffer07_value_address0 { O 12 vector } destbuffer07_value_ce0 { O 1 bit } destbuffer07_value_we0 { O 1 bit } destbuffer07_value_d0 { O 32 vector } destbuffer07_value_q0 { I 32 vector } } \
} "
} else {
puts "@W \[IMPL-110\] Cannot find bus interface model in the library. Ignored generation of bus interface for 'destbuffer07_value'"
}
}


# XIL_BRAM:
if {${::AESL::PGuard_autoexp_gen}} {
if {[info proc ::AESL_LIB_XILADAPTER::xil_bram_gen] == "::AESL_LIB_XILADAPTER::xil_bram_gen"} {
eval "::AESL_LIB_XILADAPTER::xil_bram_gen { \
    id 306 \
    name destbuffer10_value \
    reset_level 1 \
    sync_rst true \
    dir IO \
    corename destbuffer10_value \
    op interface \
    ports { destbuffer10_value_address0 { O 12 vector } destbuffer10_value_ce0 { O 1 bit } destbuffer10_value_we0 { O 1 bit } destbuffer10_value_d0 { O 32 vector } destbuffer10_value_q0 { I 32 vector } } \
} "
} else {
puts "@W \[IMPL-110\] Cannot find bus interface model in the library. Ignored generation of bus interface for 'destbuffer10_value'"
}
}


# XIL_BRAM:
if {${::AESL::PGuard_autoexp_gen}} {
if {[info proc ::AESL_LIB_XILADAPTER::xil_bram_gen] == "::AESL_LIB_XILADAPTER::xil_bram_gen"} {
eval "::AESL_LIB_XILADAPTER::xil_bram_gen { \
    id 307 \
    name destbuffer11_value \
    reset_level 1 \
    sync_rst true \
    dir IO \
    corename destbuffer11_value \
    op interface \
    ports { destbuffer11_value_address0 { O 12 vector } destbuffer11_value_ce0 { O 1 bit } destbuffer11_value_we0 { O 1 bit } destbuffer11_value_d0 { O 32 vector } destbuffer11_value_q0 { I 32 vector } } \
} "
} else {
puts "@W \[IMPL-110\] Cannot find bus interface model in the library. Ignored generation of bus interface for 'destbuffer11_value'"
}
}


# XIL_BRAM:
if {${::AESL::PGuard_autoexp_gen}} {
if {[info proc ::AESL_LIB_XILADAPTER::xil_bram_gen] == "::AESL_LIB_XILADAPTER::xil_bram_gen"} {
eval "::AESL_LIB_XILADAPTER::xil_bram_gen { \
    id 308 \
    name destbuffer12_value \
    reset_level 1 \
    sync_rst true \
    dir IO \
    corename destbuffer12_value \
    op interface \
    ports { destbuffer12_value_address0 { O 12 vector } destbuffer12_value_ce0 { O 1 bit } destbuffer12_value_we0 { O 1 bit } destbuffer12_value_d0 { O 32 vector } destbuffer12_value_q0 { I 32 vector } } \
} "
} else {
puts "@W \[IMPL-110\] Cannot find bus interface model in the library. Ignored generation of bus interface for 'destbuffer12_value'"
}
}


# XIL_BRAM:
if {${::AESL::PGuard_autoexp_gen}} {
if {[info proc ::AESL_LIB_XILADAPTER::xil_bram_gen] == "::AESL_LIB_XILADAPTER::xil_bram_gen"} {
eval "::AESL_LIB_XILADAPTER::xil_bram_gen { \
    id 309 \
    name destbuffer13_value \
    reset_level 1 \
    sync_rst true \
    dir IO \
    corename destbuffer13_value \
    op interface \
    ports { destbuffer13_value_address0 { O 12 vector } destbuffer13_value_ce0 { O 1 bit } destbuffer13_value_we0 { O 1 bit } destbuffer13_value_d0 { O 32 vector } destbuffer13_value_q0 { I 32 vector } } \
} "
} else {
puts "@W \[IMPL-110\] Cannot find bus interface model in the library. Ignored generation of bus interface for 'destbuffer13_value'"
}
}


# XIL_BRAM:
if {${::AESL::PGuard_autoexp_gen}} {
if {[info proc ::AESL_LIB_XILADAPTER::xil_bram_gen] == "::AESL_LIB_XILADAPTER::xil_bram_gen"} {
eval "::AESL_LIB_XILADAPTER::xil_bram_gen { \
    id 310 \
    name destbuffer14_value \
    reset_level 1 \
    sync_rst true \
    dir IO \
    corename destbuffer14_value \
    op interface \
    ports { destbuffer14_value_address0 { O 12 vector } destbuffer14_value_ce0 { O 1 bit } destbuffer14_value_we0 { O 1 bit } destbuffer14_value_d0 { O 32 vector } destbuffer14_value_q0 { I 32 vector } } \
} "
} else {
puts "@W \[IMPL-110\] Cannot find bus interface model in the library. Ignored generation of bus interface for 'destbuffer14_value'"
}
}


# XIL_BRAM:
if {${::AESL::PGuard_autoexp_gen}} {
if {[info proc ::AESL_LIB_XILADAPTER::xil_bram_gen] == "::AESL_LIB_XILADAPTER::xil_bram_gen"} {
eval "::AESL_LIB_XILADAPTER::xil_bram_gen { \
    id 311 \
    name destbuffer15_value \
    reset_level 1 \
    sync_rst true \
    dir IO \
    corename destbuffer15_value \
    op interface \
    ports { destbuffer15_value_address0 { O 12 vector } destbuffer15_value_ce0 { O 1 bit } destbuffer15_value_we0 { O 1 bit } destbuffer15_value_d0 { O 32 vector } destbuffer15_value_q0 { I 32 vector } } \
} "
} else {
puts "@W \[IMPL-110\] Cannot find bus interface model in the library. Ignored generation of bus interface for 'destbuffer15_value'"
}
}


# XIL_BRAM:
if {${::AESL::PGuard_autoexp_gen}} {
if {[info proc ::AESL_LIB_XILADAPTER::xil_bram_gen] == "::AESL_LIB_XILADAPTER::xil_bram_gen"} {
eval "::AESL_LIB_XILADAPTER::xil_bram_gen { \
    id 312 \
    name destbuffer16_value \
    reset_level 1 \
    sync_rst true \
    dir IO \
    corename destbuffer16_value \
    op interface \
    ports { destbuffer16_value_address0 { O 12 vector } destbuffer16_value_ce0 { O 1 bit } destbuffer16_value_we0 { O 1 bit } destbuffer16_value_d0 { O 32 vector } destbuffer16_value_q0 { I 32 vector } } \
} "
} else {
puts "@W \[IMPL-110\] Cannot find bus interface model in the library. Ignored generation of bus interface for 'destbuffer16_value'"
}
}


# XIL_BRAM:
if {${::AESL::PGuard_autoexp_gen}} {
if {[info proc ::AESL_LIB_XILADAPTER::xil_bram_gen] == "::AESL_LIB_XILADAPTER::xil_bram_gen"} {
eval "::AESL_LIB_XILADAPTER::xil_bram_gen { \
    id 313 \
    name destbuffer17_value \
    reset_level 1 \
    sync_rst true \
    dir IO \
    corename destbuffer17_value \
    op interface \
    ports { destbuffer17_value_address0 { O 12 vector } destbuffer17_value_ce0 { O 1 bit } destbuffer17_value_we0 { O 1 bit } destbuffer17_value_d0 { O 32 vector } destbuffer17_value_q0 { I 32 vector } } \
} "
} else {
puts "@W \[IMPL-110\] Cannot find bus interface model in the library. Ignored generation of bus interface for 'destbuffer17_value'"
}
}


# XIL_BRAM:
if {${::AESL::PGuard_autoexp_gen}} {
if {[info proc ::AESL_LIB_XILADAPTER::xil_bram_gen] == "::AESL_LIB_XILADAPTER::xil_bram_gen"} {
eval "::AESL_LIB_XILADAPTER::xil_bram_gen { \
    id 314 \
    name destbuffer20_value \
    reset_level 1 \
    sync_rst true \
    dir IO \
    corename destbuffer20_value \
    op interface \
    ports { destbuffer20_value_address0 { O 12 vector } destbuffer20_value_ce0 { O 1 bit } destbuffer20_value_we0 { O 1 bit } destbuffer20_value_d0 { O 32 vector } destbuffer20_value_q0 { I 32 vector } } \
} "
} else {
puts "@W \[IMPL-110\] Cannot find bus interface model in the library. Ignored generation of bus interface for 'destbuffer20_value'"
}
}


# XIL_BRAM:
if {${::AESL::PGuard_autoexp_gen}} {
if {[info proc ::AESL_LIB_XILADAPTER::xil_bram_gen] == "::AESL_LIB_XILADAPTER::xil_bram_gen"} {
eval "::AESL_LIB_XILADAPTER::xil_bram_gen { \
    id 315 \
    name destbuffer21_value \
    reset_level 1 \
    sync_rst true \
    dir IO \
    corename destbuffer21_value \
    op interface \
    ports { destbuffer21_value_address0 { O 12 vector } destbuffer21_value_ce0 { O 1 bit } destbuffer21_value_we0 { O 1 bit } destbuffer21_value_d0 { O 32 vector } destbuffer21_value_q0 { I 32 vector } } \
} "
} else {
puts "@W \[IMPL-110\] Cannot find bus interface model in the library. Ignored generation of bus interface for 'destbuffer21_value'"
}
}


# XIL_BRAM:
if {${::AESL::PGuard_autoexp_gen}} {
if {[info proc ::AESL_LIB_XILADAPTER::xil_bram_gen] == "::AESL_LIB_XILADAPTER::xil_bram_gen"} {
eval "::AESL_LIB_XILADAPTER::xil_bram_gen { \
    id 316 \
    name destbuffer22_value \
    reset_level 1 \
    sync_rst true \
    dir IO \
    corename destbuffer22_value \
    op interface \
    ports { destbuffer22_value_address0 { O 12 vector } destbuffer22_value_ce0 { O 1 bit } destbuffer22_value_we0 { O 1 bit } destbuffer22_value_d0 { O 32 vector } destbuffer22_value_q0 { I 32 vector } } \
} "
} else {
puts "@W \[IMPL-110\] Cannot find bus interface model in the library. Ignored generation of bus interface for 'destbuffer22_value'"
}
}


# XIL_BRAM:
if {${::AESL::PGuard_autoexp_gen}} {
if {[info proc ::AESL_LIB_XILADAPTER::xil_bram_gen] == "::AESL_LIB_XILADAPTER::xil_bram_gen"} {
eval "::AESL_LIB_XILADAPTER::xil_bram_gen { \
    id 317 \
    name destbuffer23_value \
    reset_level 1 \
    sync_rst true \
    dir IO \
    corename destbuffer23_value \
    op interface \
    ports { destbuffer23_value_address0 { O 12 vector } destbuffer23_value_ce0 { O 1 bit } destbuffer23_value_we0 { O 1 bit } destbuffer23_value_d0 { O 32 vector } destbuffer23_value_q0 { I 32 vector } } \
} "
} else {
puts "@W \[IMPL-110\] Cannot find bus interface model in the library. Ignored generation of bus interface for 'destbuffer23_value'"
}
}


# XIL_BRAM:
if {${::AESL::PGuard_autoexp_gen}} {
if {[info proc ::AESL_LIB_XILADAPTER::xil_bram_gen] == "::AESL_LIB_XILADAPTER::xil_bram_gen"} {
eval "::AESL_LIB_XILADAPTER::xil_bram_gen { \
    id 318 \
    name destbuffer24_value \
    reset_level 1 \
    sync_rst true \
    dir IO \
    corename destbuffer24_value \
    op interface \
    ports { destbuffer24_value_address0 { O 12 vector } destbuffer24_value_ce0 { O 1 bit } destbuffer24_value_we0 { O 1 bit } destbuffer24_value_d0 { O 32 vector } destbuffer24_value_q0 { I 32 vector } } \
} "
} else {
puts "@W \[IMPL-110\] Cannot find bus interface model in the library. Ignored generation of bus interface for 'destbuffer24_value'"
}
}


# XIL_BRAM:
if {${::AESL::PGuard_autoexp_gen}} {
if {[info proc ::AESL_LIB_XILADAPTER::xil_bram_gen] == "::AESL_LIB_XILADAPTER::xil_bram_gen"} {
eval "::AESL_LIB_XILADAPTER::xil_bram_gen { \
    id 319 \
    name destbuffer25_value \
    reset_level 1 \
    sync_rst true \
    dir IO \
    corename destbuffer25_value \
    op interface \
    ports { destbuffer25_value_address0 { O 12 vector } destbuffer25_value_ce0 { O 1 bit } destbuffer25_value_we0 { O 1 bit } destbuffer25_value_d0 { O 32 vector } destbuffer25_value_q0 { I 32 vector } } \
} "
} else {
puts "@W \[IMPL-110\] Cannot find bus interface model in the library. Ignored generation of bus interface for 'destbuffer25_value'"
}
}


# XIL_BRAM:
if {${::AESL::PGuard_autoexp_gen}} {
if {[info proc ::AESL_LIB_XILADAPTER::xil_bram_gen] == "::AESL_LIB_XILADAPTER::xil_bram_gen"} {
eval "::AESL_LIB_XILADAPTER::xil_bram_gen { \
    id 320 \
    name destbuffer26_value \
    reset_level 1 \
    sync_rst true \
    dir IO \
    corename destbuffer26_value \
    op interface \
    ports { destbuffer26_value_address0 { O 12 vector } destbuffer26_value_ce0 { O 1 bit } destbuffer26_value_we0 { O 1 bit } destbuffer26_value_d0 { O 32 vector } destbuffer26_value_q0 { I 32 vector } } \
} "
} else {
puts "@W \[IMPL-110\] Cannot find bus interface model in the library. Ignored generation of bus interface for 'destbuffer26_value'"
}
}


# XIL_BRAM:
if {${::AESL::PGuard_autoexp_gen}} {
if {[info proc ::AESL_LIB_XILADAPTER::xil_bram_gen] == "::AESL_LIB_XILADAPTER::xil_bram_gen"} {
eval "::AESL_LIB_XILADAPTER::xil_bram_gen { \
    id 321 \
    name destbuffer27_value \
    reset_level 1 \
    sync_rst true \
    dir IO \
    corename destbuffer27_value \
    op interface \
    ports { destbuffer27_value_address0 { O 12 vector } destbuffer27_value_ce0 { O 1 bit } destbuffer27_value_we0 { O 1 bit } destbuffer27_value_d0 { O 32 vector } destbuffer27_value_q0 { I 32 vector } } \
} "
} else {
puts "@W \[IMPL-110\] Cannot find bus interface model in the library. Ignored generation of bus interface for 'destbuffer27_value'"
}
}


# XIL_BRAM:
if {${::AESL::PGuard_autoexp_gen}} {
if {[info proc ::AESL_LIB_XILADAPTER::xil_bram_gen] == "::AESL_LIB_XILADAPTER::xil_bram_gen"} {
eval "::AESL_LIB_XILADAPTER::xil_bram_gen { \
    id 322 \
    name destbuffer30_value \
    reset_level 1 \
    sync_rst true \
    dir IO \
    corename destbuffer30_value \
    op interface \
    ports { destbuffer30_value_address0 { O 12 vector } destbuffer30_value_ce0 { O 1 bit } destbuffer30_value_we0 { O 1 bit } destbuffer30_value_d0 { O 32 vector } destbuffer30_value_q0 { I 32 vector } } \
} "
} else {
puts "@W \[IMPL-110\] Cannot find bus interface model in the library. Ignored generation of bus interface for 'destbuffer30_value'"
}
}


# XIL_BRAM:
if {${::AESL::PGuard_autoexp_gen}} {
if {[info proc ::AESL_LIB_XILADAPTER::xil_bram_gen] == "::AESL_LIB_XILADAPTER::xil_bram_gen"} {
eval "::AESL_LIB_XILADAPTER::xil_bram_gen { \
    id 323 \
    name destbuffer31_value \
    reset_level 1 \
    sync_rst true \
    dir IO \
    corename destbuffer31_value \
    op interface \
    ports { destbuffer31_value_address0 { O 12 vector } destbuffer31_value_ce0 { O 1 bit } destbuffer31_value_we0 { O 1 bit } destbuffer31_value_d0 { O 32 vector } destbuffer31_value_q0 { I 32 vector } } \
} "
} else {
puts "@W \[IMPL-110\] Cannot find bus interface model in the library. Ignored generation of bus interface for 'destbuffer31_value'"
}
}


# XIL_BRAM:
if {${::AESL::PGuard_autoexp_gen}} {
if {[info proc ::AESL_LIB_XILADAPTER::xil_bram_gen] == "::AESL_LIB_XILADAPTER::xil_bram_gen"} {
eval "::AESL_LIB_XILADAPTER::xil_bram_gen { \
    id 324 \
    name destbuffer32_value \
    reset_level 1 \
    sync_rst true \
    dir IO \
    corename destbuffer32_value \
    op interface \
    ports { destbuffer32_value_address0 { O 12 vector } destbuffer32_value_ce0 { O 1 bit } destbuffer32_value_we0 { O 1 bit } destbuffer32_value_d0 { O 32 vector } destbuffer32_value_q0 { I 32 vector } } \
} "
} else {
puts "@W \[IMPL-110\] Cannot find bus interface model in the library. Ignored generation of bus interface for 'destbuffer32_value'"
}
}


# XIL_BRAM:
if {${::AESL::PGuard_autoexp_gen}} {
if {[info proc ::AESL_LIB_XILADAPTER::xil_bram_gen] == "::AESL_LIB_XILADAPTER::xil_bram_gen"} {
eval "::AESL_LIB_XILADAPTER::xil_bram_gen { \
    id 325 \
    name destbuffer33_value \
    reset_level 1 \
    sync_rst true \
    dir IO \
    corename destbuffer33_value \
    op interface \
    ports { destbuffer33_value_address0 { O 12 vector } destbuffer33_value_ce0 { O 1 bit } destbuffer33_value_we0 { O 1 bit } destbuffer33_value_d0 { O 32 vector } destbuffer33_value_q0 { I 32 vector } } \
} "
} else {
puts "@W \[IMPL-110\] Cannot find bus interface model in the library. Ignored generation of bus interface for 'destbuffer33_value'"
}
}


# XIL_BRAM:
if {${::AESL::PGuard_autoexp_gen}} {
if {[info proc ::AESL_LIB_XILADAPTER::xil_bram_gen] == "::AESL_LIB_XILADAPTER::xil_bram_gen"} {
eval "::AESL_LIB_XILADAPTER::xil_bram_gen { \
    id 326 \
    name destbuffer34_value \
    reset_level 1 \
    sync_rst true \
    dir IO \
    corename destbuffer34_value \
    op interface \
    ports { destbuffer34_value_address0 { O 12 vector } destbuffer34_value_ce0 { O 1 bit } destbuffer34_value_we0 { O 1 bit } destbuffer34_value_d0 { O 32 vector } destbuffer34_value_q0 { I 32 vector } } \
} "
} else {
puts "@W \[IMPL-110\] Cannot find bus interface model in the library. Ignored generation of bus interface for 'destbuffer34_value'"
}
}


# XIL_BRAM:
if {${::AESL::PGuard_autoexp_gen}} {
if {[info proc ::AESL_LIB_XILADAPTER::xil_bram_gen] == "::AESL_LIB_XILADAPTER::xil_bram_gen"} {
eval "::AESL_LIB_XILADAPTER::xil_bram_gen { \
    id 327 \
    name destbuffer35_value \
    reset_level 1 \
    sync_rst true \
    dir IO \
    corename destbuffer35_value \
    op interface \
    ports { destbuffer35_value_address0 { O 12 vector } destbuffer35_value_ce0 { O 1 bit } destbuffer35_value_we0 { O 1 bit } destbuffer35_value_d0 { O 32 vector } destbuffer35_value_q0 { I 32 vector } } \
} "
} else {
puts "@W \[IMPL-110\] Cannot find bus interface model in the library. Ignored generation of bus interface for 'destbuffer35_value'"
}
}


# XIL_BRAM:
if {${::AESL::PGuard_autoexp_gen}} {
if {[info proc ::AESL_LIB_XILADAPTER::xil_bram_gen] == "::AESL_LIB_XILADAPTER::xil_bram_gen"} {
eval "::AESL_LIB_XILADAPTER::xil_bram_gen { \
    id 328 \
    name destbuffer36_value \
    reset_level 1 \
    sync_rst true \
    dir IO \
    corename destbuffer36_value \
    op interface \
    ports { destbuffer36_value_address0 { O 12 vector } destbuffer36_value_ce0 { O 1 bit } destbuffer36_value_we0 { O 1 bit } destbuffer36_value_d0 { O 32 vector } destbuffer36_value_q0 { I 32 vector } } \
} "
} else {
puts "@W \[IMPL-110\] Cannot find bus interface model in the library. Ignored generation of bus interface for 'destbuffer36_value'"
}
}


# XIL_BRAM:
if {${::AESL::PGuard_autoexp_gen}} {
if {[info proc ::AESL_LIB_XILADAPTER::xil_bram_gen] == "::AESL_LIB_XILADAPTER::xil_bram_gen"} {
eval "::AESL_LIB_XILADAPTER::xil_bram_gen { \
    id 329 \
    name destbuffer37_value \
    reset_level 1 \
    sync_rst true \
    dir IO \
    corename destbuffer37_value \
    op interface \
    ports { destbuffer37_value_address0 { O 12 vector } destbuffer37_value_ce0 { O 1 bit } destbuffer37_value_we0 { O 1 bit } destbuffer37_value_d0 { O 32 vector } destbuffer37_value_q0 { I 32 vector } } \
} "
} else {
puts "@W \[IMPL-110\] Cannot find bus interface model in the library. Ignored generation of bus interface for 'destbuffer37_value'"
}
}


# Direct connection:
if {${::AESL::PGuard_autoexp_gen}} {
eval "cg_default_interface_gen_dc { \
    id 293 \
    name enable \
    type other \
    dir I \
    reset_level 1 \
    sync_rst true \
    corename dc_enable \
    op interface \
    ports { enable { I 1 vector } } \
} "
}

# Direct connection:
if {${::AESL::PGuard_autoexp_gen}} {
eval "cg_default_interface_gen_dc { \
    id 330 \
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


