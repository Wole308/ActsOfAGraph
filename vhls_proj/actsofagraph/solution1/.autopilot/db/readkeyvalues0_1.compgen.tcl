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
    id 147 \
    name buffer00_key \
    reset_level 1 \
    sync_rst true \
    dir O \
    corename buffer00_key \
    op interface \
    ports { buffer00_key_address0 { O 12 vector } buffer00_key_ce0 { O 1 bit } buffer00_key_we0 { O 1 bit } buffer00_key_d0 { O 32 vector } } \
} "
} else {
puts "@W \[IMPL-110\] Cannot find bus interface model in the library. Ignored generation of bus interface for 'buffer00_key'"
}
}


# XIL_BRAM:
if {${::AESL::PGuard_autoexp_gen}} {
if {[info proc ::AESL_LIB_XILADAPTER::xil_bram_gen] == "::AESL_LIB_XILADAPTER::xil_bram_gen"} {
eval "::AESL_LIB_XILADAPTER::xil_bram_gen { \
    id 148 \
    name buffer00_value \
    reset_level 1 \
    sync_rst true \
    dir O \
    corename buffer00_value \
    op interface \
    ports { buffer00_value_address0 { O 12 vector } buffer00_value_ce0 { O 1 bit } buffer00_value_we0 { O 1 bit } buffer00_value_d0 { O 32 vector } } \
} "
} else {
puts "@W \[IMPL-110\] Cannot find bus interface model in the library. Ignored generation of bus interface for 'buffer00_value'"
}
}


# XIL_BRAM:
if {${::AESL::PGuard_autoexp_gen}} {
if {[info proc ::AESL_LIB_XILADAPTER::xil_bram_gen] == "::AESL_LIB_XILADAPTER::xil_bram_gen"} {
eval "::AESL_LIB_XILADAPTER::xil_bram_gen { \
    id 149 \
    name buffer01_key \
    reset_level 1 \
    sync_rst true \
    dir O \
    corename buffer01_key \
    op interface \
    ports { buffer01_key_address0 { O 12 vector } buffer01_key_ce0 { O 1 bit } buffer01_key_we0 { O 1 bit } buffer01_key_d0 { O 32 vector } } \
} "
} else {
puts "@W \[IMPL-110\] Cannot find bus interface model in the library. Ignored generation of bus interface for 'buffer01_key'"
}
}


# XIL_BRAM:
if {${::AESL::PGuard_autoexp_gen}} {
if {[info proc ::AESL_LIB_XILADAPTER::xil_bram_gen] == "::AESL_LIB_XILADAPTER::xil_bram_gen"} {
eval "::AESL_LIB_XILADAPTER::xil_bram_gen { \
    id 150 \
    name buffer01_value \
    reset_level 1 \
    sync_rst true \
    dir O \
    corename buffer01_value \
    op interface \
    ports { buffer01_value_address0 { O 12 vector } buffer01_value_ce0 { O 1 bit } buffer01_value_we0 { O 1 bit } buffer01_value_d0 { O 32 vector } } \
} "
} else {
puts "@W \[IMPL-110\] Cannot find bus interface model in the library. Ignored generation of bus interface for 'buffer01_value'"
}
}


# XIL_BRAM:
if {${::AESL::PGuard_autoexp_gen}} {
if {[info proc ::AESL_LIB_XILADAPTER::xil_bram_gen] == "::AESL_LIB_XILADAPTER::xil_bram_gen"} {
eval "::AESL_LIB_XILADAPTER::xil_bram_gen { \
    id 151 \
    name buffer02_key \
    reset_level 1 \
    sync_rst true \
    dir O \
    corename buffer02_key \
    op interface \
    ports { buffer02_key_address0 { O 12 vector } buffer02_key_ce0 { O 1 bit } buffer02_key_we0 { O 1 bit } buffer02_key_d0 { O 32 vector } } \
} "
} else {
puts "@W \[IMPL-110\] Cannot find bus interface model in the library. Ignored generation of bus interface for 'buffer02_key'"
}
}


# XIL_BRAM:
if {${::AESL::PGuard_autoexp_gen}} {
if {[info proc ::AESL_LIB_XILADAPTER::xil_bram_gen] == "::AESL_LIB_XILADAPTER::xil_bram_gen"} {
eval "::AESL_LIB_XILADAPTER::xil_bram_gen { \
    id 152 \
    name buffer02_value \
    reset_level 1 \
    sync_rst true \
    dir O \
    corename buffer02_value \
    op interface \
    ports { buffer02_value_address0 { O 12 vector } buffer02_value_ce0 { O 1 bit } buffer02_value_we0 { O 1 bit } buffer02_value_d0 { O 32 vector } } \
} "
} else {
puts "@W \[IMPL-110\] Cannot find bus interface model in the library. Ignored generation of bus interface for 'buffer02_value'"
}
}


# XIL_BRAM:
if {${::AESL::PGuard_autoexp_gen}} {
if {[info proc ::AESL_LIB_XILADAPTER::xil_bram_gen] == "::AESL_LIB_XILADAPTER::xil_bram_gen"} {
eval "::AESL_LIB_XILADAPTER::xil_bram_gen { \
    id 153 \
    name buffer03_key \
    reset_level 1 \
    sync_rst true \
    dir O \
    corename buffer03_key \
    op interface \
    ports { buffer03_key_address0 { O 12 vector } buffer03_key_ce0 { O 1 bit } buffer03_key_we0 { O 1 bit } buffer03_key_d0 { O 32 vector } } \
} "
} else {
puts "@W \[IMPL-110\] Cannot find bus interface model in the library. Ignored generation of bus interface for 'buffer03_key'"
}
}


# XIL_BRAM:
if {${::AESL::PGuard_autoexp_gen}} {
if {[info proc ::AESL_LIB_XILADAPTER::xil_bram_gen] == "::AESL_LIB_XILADAPTER::xil_bram_gen"} {
eval "::AESL_LIB_XILADAPTER::xil_bram_gen { \
    id 154 \
    name buffer03_value \
    reset_level 1 \
    sync_rst true \
    dir O \
    corename buffer03_value \
    op interface \
    ports { buffer03_value_address0 { O 12 vector } buffer03_value_ce0 { O 1 bit } buffer03_value_we0 { O 1 bit } buffer03_value_d0 { O 32 vector } } \
} "
} else {
puts "@W \[IMPL-110\] Cannot find bus interface model in the library. Ignored generation of bus interface for 'buffer03_value'"
}
}


# XIL_BRAM:
if {${::AESL::PGuard_autoexp_gen}} {
if {[info proc ::AESL_LIB_XILADAPTER::xil_bram_gen] == "::AESL_LIB_XILADAPTER::xil_bram_gen"} {
eval "::AESL_LIB_XILADAPTER::xil_bram_gen { \
    id 155 \
    name buffer04_key \
    reset_level 1 \
    sync_rst true \
    dir O \
    corename buffer04_key \
    op interface \
    ports { buffer04_key_address0 { O 12 vector } buffer04_key_ce0 { O 1 bit } buffer04_key_we0 { O 1 bit } buffer04_key_d0 { O 32 vector } } \
} "
} else {
puts "@W \[IMPL-110\] Cannot find bus interface model in the library. Ignored generation of bus interface for 'buffer04_key'"
}
}


# XIL_BRAM:
if {${::AESL::PGuard_autoexp_gen}} {
if {[info proc ::AESL_LIB_XILADAPTER::xil_bram_gen] == "::AESL_LIB_XILADAPTER::xil_bram_gen"} {
eval "::AESL_LIB_XILADAPTER::xil_bram_gen { \
    id 156 \
    name buffer04_value \
    reset_level 1 \
    sync_rst true \
    dir O \
    corename buffer04_value \
    op interface \
    ports { buffer04_value_address0 { O 12 vector } buffer04_value_ce0 { O 1 bit } buffer04_value_we0 { O 1 bit } buffer04_value_d0 { O 32 vector } } \
} "
} else {
puts "@W \[IMPL-110\] Cannot find bus interface model in the library. Ignored generation of bus interface for 'buffer04_value'"
}
}


# XIL_BRAM:
if {${::AESL::PGuard_autoexp_gen}} {
if {[info proc ::AESL_LIB_XILADAPTER::xil_bram_gen] == "::AESL_LIB_XILADAPTER::xil_bram_gen"} {
eval "::AESL_LIB_XILADAPTER::xil_bram_gen { \
    id 157 \
    name buffer05_key \
    reset_level 1 \
    sync_rst true \
    dir O \
    corename buffer05_key \
    op interface \
    ports { buffer05_key_address0 { O 12 vector } buffer05_key_ce0 { O 1 bit } buffer05_key_we0 { O 1 bit } buffer05_key_d0 { O 32 vector } } \
} "
} else {
puts "@W \[IMPL-110\] Cannot find bus interface model in the library. Ignored generation of bus interface for 'buffer05_key'"
}
}


# XIL_BRAM:
if {${::AESL::PGuard_autoexp_gen}} {
if {[info proc ::AESL_LIB_XILADAPTER::xil_bram_gen] == "::AESL_LIB_XILADAPTER::xil_bram_gen"} {
eval "::AESL_LIB_XILADAPTER::xil_bram_gen { \
    id 158 \
    name buffer05_value \
    reset_level 1 \
    sync_rst true \
    dir O \
    corename buffer05_value \
    op interface \
    ports { buffer05_value_address0 { O 12 vector } buffer05_value_ce0 { O 1 bit } buffer05_value_we0 { O 1 bit } buffer05_value_d0 { O 32 vector } } \
} "
} else {
puts "@W \[IMPL-110\] Cannot find bus interface model in the library. Ignored generation of bus interface for 'buffer05_value'"
}
}


# XIL_BRAM:
if {${::AESL::PGuard_autoexp_gen}} {
if {[info proc ::AESL_LIB_XILADAPTER::xil_bram_gen] == "::AESL_LIB_XILADAPTER::xil_bram_gen"} {
eval "::AESL_LIB_XILADAPTER::xil_bram_gen { \
    id 159 \
    name buffer06_key \
    reset_level 1 \
    sync_rst true \
    dir O \
    corename buffer06_key \
    op interface \
    ports { buffer06_key_address0 { O 12 vector } buffer06_key_ce0 { O 1 bit } buffer06_key_we0 { O 1 bit } buffer06_key_d0 { O 32 vector } } \
} "
} else {
puts "@W \[IMPL-110\] Cannot find bus interface model in the library. Ignored generation of bus interface for 'buffer06_key'"
}
}


# XIL_BRAM:
if {${::AESL::PGuard_autoexp_gen}} {
if {[info proc ::AESL_LIB_XILADAPTER::xil_bram_gen] == "::AESL_LIB_XILADAPTER::xil_bram_gen"} {
eval "::AESL_LIB_XILADAPTER::xil_bram_gen { \
    id 160 \
    name buffer06_value \
    reset_level 1 \
    sync_rst true \
    dir O \
    corename buffer06_value \
    op interface \
    ports { buffer06_value_address0 { O 12 vector } buffer06_value_ce0 { O 1 bit } buffer06_value_we0 { O 1 bit } buffer06_value_d0 { O 32 vector } } \
} "
} else {
puts "@W \[IMPL-110\] Cannot find bus interface model in the library. Ignored generation of bus interface for 'buffer06_value'"
}
}


# XIL_BRAM:
if {${::AESL::PGuard_autoexp_gen}} {
if {[info proc ::AESL_LIB_XILADAPTER::xil_bram_gen] == "::AESL_LIB_XILADAPTER::xil_bram_gen"} {
eval "::AESL_LIB_XILADAPTER::xil_bram_gen { \
    id 161 \
    name buffer07_key \
    reset_level 1 \
    sync_rst true \
    dir O \
    corename buffer07_key \
    op interface \
    ports { buffer07_key_address0 { O 12 vector } buffer07_key_ce0 { O 1 bit } buffer07_key_we0 { O 1 bit } buffer07_key_d0 { O 32 vector } } \
} "
} else {
puts "@W \[IMPL-110\] Cannot find bus interface model in the library. Ignored generation of bus interface for 'buffer07_key'"
}
}


# XIL_BRAM:
if {${::AESL::PGuard_autoexp_gen}} {
if {[info proc ::AESL_LIB_XILADAPTER::xil_bram_gen] == "::AESL_LIB_XILADAPTER::xil_bram_gen"} {
eval "::AESL_LIB_XILADAPTER::xil_bram_gen { \
    id 162 \
    name buffer07_value \
    reset_level 1 \
    sync_rst true \
    dir O \
    corename buffer07_value \
    op interface \
    ports { buffer07_value_address0 { O 12 vector } buffer07_value_ce0 { O 1 bit } buffer07_value_we0 { O 1 bit } buffer07_value_d0 { O 32 vector } } \
} "
} else {
puts "@W \[IMPL-110\] Cannot find bus interface model in the library. Ignored generation of bus interface for 'buffer07_value'"
}
}


# XIL_BRAM:
if {${::AESL::PGuard_autoexp_gen}} {
if {[info proc ::AESL_LIB_XILADAPTER::xil_bram_gen] == "::AESL_LIB_XILADAPTER::xil_bram_gen"} {
eval "::AESL_LIB_XILADAPTER::xil_bram_gen { \
    id 163 \
    name buffer10_key \
    reset_level 1 \
    sync_rst true \
    dir O \
    corename buffer10_key \
    op interface \
    ports { buffer10_key_address0 { O 12 vector } buffer10_key_ce0 { O 1 bit } buffer10_key_we0 { O 1 bit } buffer10_key_d0 { O 32 vector } } \
} "
} else {
puts "@W \[IMPL-110\] Cannot find bus interface model in the library. Ignored generation of bus interface for 'buffer10_key'"
}
}


# XIL_BRAM:
if {${::AESL::PGuard_autoexp_gen}} {
if {[info proc ::AESL_LIB_XILADAPTER::xil_bram_gen] == "::AESL_LIB_XILADAPTER::xil_bram_gen"} {
eval "::AESL_LIB_XILADAPTER::xil_bram_gen { \
    id 164 \
    name buffer10_value \
    reset_level 1 \
    sync_rst true \
    dir O \
    corename buffer10_value \
    op interface \
    ports { buffer10_value_address0 { O 12 vector } buffer10_value_ce0 { O 1 bit } buffer10_value_we0 { O 1 bit } buffer10_value_d0 { O 32 vector } } \
} "
} else {
puts "@W \[IMPL-110\] Cannot find bus interface model in the library. Ignored generation of bus interface for 'buffer10_value'"
}
}


# XIL_BRAM:
if {${::AESL::PGuard_autoexp_gen}} {
if {[info proc ::AESL_LIB_XILADAPTER::xil_bram_gen] == "::AESL_LIB_XILADAPTER::xil_bram_gen"} {
eval "::AESL_LIB_XILADAPTER::xil_bram_gen { \
    id 165 \
    name buffer11_key \
    reset_level 1 \
    sync_rst true \
    dir O \
    corename buffer11_key \
    op interface \
    ports { buffer11_key_address0 { O 12 vector } buffer11_key_ce0 { O 1 bit } buffer11_key_we0 { O 1 bit } buffer11_key_d0 { O 32 vector } } \
} "
} else {
puts "@W \[IMPL-110\] Cannot find bus interface model in the library. Ignored generation of bus interface for 'buffer11_key'"
}
}


# XIL_BRAM:
if {${::AESL::PGuard_autoexp_gen}} {
if {[info proc ::AESL_LIB_XILADAPTER::xil_bram_gen] == "::AESL_LIB_XILADAPTER::xil_bram_gen"} {
eval "::AESL_LIB_XILADAPTER::xil_bram_gen { \
    id 166 \
    name buffer11_value \
    reset_level 1 \
    sync_rst true \
    dir O \
    corename buffer11_value \
    op interface \
    ports { buffer11_value_address0 { O 12 vector } buffer11_value_ce0 { O 1 bit } buffer11_value_we0 { O 1 bit } buffer11_value_d0 { O 32 vector } } \
} "
} else {
puts "@W \[IMPL-110\] Cannot find bus interface model in the library. Ignored generation of bus interface for 'buffer11_value'"
}
}


# XIL_BRAM:
if {${::AESL::PGuard_autoexp_gen}} {
if {[info proc ::AESL_LIB_XILADAPTER::xil_bram_gen] == "::AESL_LIB_XILADAPTER::xil_bram_gen"} {
eval "::AESL_LIB_XILADAPTER::xil_bram_gen { \
    id 167 \
    name buffer12_key \
    reset_level 1 \
    sync_rst true \
    dir O \
    corename buffer12_key \
    op interface \
    ports { buffer12_key_address0 { O 12 vector } buffer12_key_ce0 { O 1 bit } buffer12_key_we0 { O 1 bit } buffer12_key_d0 { O 32 vector } } \
} "
} else {
puts "@W \[IMPL-110\] Cannot find bus interface model in the library. Ignored generation of bus interface for 'buffer12_key'"
}
}


# XIL_BRAM:
if {${::AESL::PGuard_autoexp_gen}} {
if {[info proc ::AESL_LIB_XILADAPTER::xil_bram_gen] == "::AESL_LIB_XILADAPTER::xil_bram_gen"} {
eval "::AESL_LIB_XILADAPTER::xil_bram_gen { \
    id 168 \
    name buffer12_value \
    reset_level 1 \
    sync_rst true \
    dir O \
    corename buffer12_value \
    op interface \
    ports { buffer12_value_address0 { O 12 vector } buffer12_value_ce0 { O 1 bit } buffer12_value_we0 { O 1 bit } buffer12_value_d0 { O 32 vector } } \
} "
} else {
puts "@W \[IMPL-110\] Cannot find bus interface model in the library. Ignored generation of bus interface for 'buffer12_value'"
}
}


# XIL_BRAM:
if {${::AESL::PGuard_autoexp_gen}} {
if {[info proc ::AESL_LIB_XILADAPTER::xil_bram_gen] == "::AESL_LIB_XILADAPTER::xil_bram_gen"} {
eval "::AESL_LIB_XILADAPTER::xil_bram_gen { \
    id 169 \
    name buffer13_key \
    reset_level 1 \
    sync_rst true \
    dir O \
    corename buffer13_key \
    op interface \
    ports { buffer13_key_address0 { O 12 vector } buffer13_key_ce0 { O 1 bit } buffer13_key_we0 { O 1 bit } buffer13_key_d0 { O 32 vector } } \
} "
} else {
puts "@W \[IMPL-110\] Cannot find bus interface model in the library. Ignored generation of bus interface for 'buffer13_key'"
}
}


# XIL_BRAM:
if {${::AESL::PGuard_autoexp_gen}} {
if {[info proc ::AESL_LIB_XILADAPTER::xil_bram_gen] == "::AESL_LIB_XILADAPTER::xil_bram_gen"} {
eval "::AESL_LIB_XILADAPTER::xil_bram_gen { \
    id 170 \
    name buffer13_value \
    reset_level 1 \
    sync_rst true \
    dir O \
    corename buffer13_value \
    op interface \
    ports { buffer13_value_address0 { O 12 vector } buffer13_value_ce0 { O 1 bit } buffer13_value_we0 { O 1 bit } buffer13_value_d0 { O 32 vector } } \
} "
} else {
puts "@W \[IMPL-110\] Cannot find bus interface model in the library. Ignored generation of bus interface for 'buffer13_value'"
}
}


# XIL_BRAM:
if {${::AESL::PGuard_autoexp_gen}} {
if {[info proc ::AESL_LIB_XILADAPTER::xil_bram_gen] == "::AESL_LIB_XILADAPTER::xil_bram_gen"} {
eval "::AESL_LIB_XILADAPTER::xil_bram_gen { \
    id 171 \
    name buffer14_key \
    reset_level 1 \
    sync_rst true \
    dir O \
    corename buffer14_key \
    op interface \
    ports { buffer14_key_address0 { O 12 vector } buffer14_key_ce0 { O 1 bit } buffer14_key_we0 { O 1 bit } buffer14_key_d0 { O 32 vector } } \
} "
} else {
puts "@W \[IMPL-110\] Cannot find bus interface model in the library. Ignored generation of bus interface for 'buffer14_key'"
}
}


# XIL_BRAM:
if {${::AESL::PGuard_autoexp_gen}} {
if {[info proc ::AESL_LIB_XILADAPTER::xil_bram_gen] == "::AESL_LIB_XILADAPTER::xil_bram_gen"} {
eval "::AESL_LIB_XILADAPTER::xil_bram_gen { \
    id 172 \
    name buffer14_value \
    reset_level 1 \
    sync_rst true \
    dir O \
    corename buffer14_value \
    op interface \
    ports { buffer14_value_address0 { O 12 vector } buffer14_value_ce0 { O 1 bit } buffer14_value_we0 { O 1 bit } buffer14_value_d0 { O 32 vector } } \
} "
} else {
puts "@W \[IMPL-110\] Cannot find bus interface model in the library. Ignored generation of bus interface for 'buffer14_value'"
}
}


# XIL_BRAM:
if {${::AESL::PGuard_autoexp_gen}} {
if {[info proc ::AESL_LIB_XILADAPTER::xil_bram_gen] == "::AESL_LIB_XILADAPTER::xil_bram_gen"} {
eval "::AESL_LIB_XILADAPTER::xil_bram_gen { \
    id 173 \
    name buffer15_key \
    reset_level 1 \
    sync_rst true \
    dir O \
    corename buffer15_key \
    op interface \
    ports { buffer15_key_address0 { O 12 vector } buffer15_key_ce0 { O 1 bit } buffer15_key_we0 { O 1 bit } buffer15_key_d0 { O 32 vector } } \
} "
} else {
puts "@W \[IMPL-110\] Cannot find bus interface model in the library. Ignored generation of bus interface for 'buffer15_key'"
}
}


# XIL_BRAM:
if {${::AESL::PGuard_autoexp_gen}} {
if {[info proc ::AESL_LIB_XILADAPTER::xil_bram_gen] == "::AESL_LIB_XILADAPTER::xil_bram_gen"} {
eval "::AESL_LIB_XILADAPTER::xil_bram_gen { \
    id 174 \
    name buffer15_value \
    reset_level 1 \
    sync_rst true \
    dir O \
    corename buffer15_value \
    op interface \
    ports { buffer15_value_address0 { O 12 vector } buffer15_value_ce0 { O 1 bit } buffer15_value_we0 { O 1 bit } buffer15_value_d0 { O 32 vector } } \
} "
} else {
puts "@W \[IMPL-110\] Cannot find bus interface model in the library. Ignored generation of bus interface for 'buffer15_value'"
}
}


# XIL_BRAM:
if {${::AESL::PGuard_autoexp_gen}} {
if {[info proc ::AESL_LIB_XILADAPTER::xil_bram_gen] == "::AESL_LIB_XILADAPTER::xil_bram_gen"} {
eval "::AESL_LIB_XILADAPTER::xil_bram_gen { \
    id 175 \
    name buffer16_key \
    reset_level 1 \
    sync_rst true \
    dir O \
    corename buffer16_key \
    op interface \
    ports { buffer16_key_address0 { O 12 vector } buffer16_key_ce0 { O 1 bit } buffer16_key_we0 { O 1 bit } buffer16_key_d0 { O 32 vector } } \
} "
} else {
puts "@W \[IMPL-110\] Cannot find bus interface model in the library. Ignored generation of bus interface for 'buffer16_key'"
}
}


# XIL_BRAM:
if {${::AESL::PGuard_autoexp_gen}} {
if {[info proc ::AESL_LIB_XILADAPTER::xil_bram_gen] == "::AESL_LIB_XILADAPTER::xil_bram_gen"} {
eval "::AESL_LIB_XILADAPTER::xil_bram_gen { \
    id 176 \
    name buffer16_value \
    reset_level 1 \
    sync_rst true \
    dir O \
    corename buffer16_value \
    op interface \
    ports { buffer16_value_address0 { O 12 vector } buffer16_value_ce0 { O 1 bit } buffer16_value_we0 { O 1 bit } buffer16_value_d0 { O 32 vector } } \
} "
} else {
puts "@W \[IMPL-110\] Cannot find bus interface model in the library. Ignored generation of bus interface for 'buffer16_value'"
}
}


# XIL_BRAM:
if {${::AESL::PGuard_autoexp_gen}} {
if {[info proc ::AESL_LIB_XILADAPTER::xil_bram_gen] == "::AESL_LIB_XILADAPTER::xil_bram_gen"} {
eval "::AESL_LIB_XILADAPTER::xil_bram_gen { \
    id 177 \
    name buffer17_key \
    reset_level 1 \
    sync_rst true \
    dir O \
    corename buffer17_key \
    op interface \
    ports { buffer17_key_address0 { O 12 vector } buffer17_key_ce0 { O 1 bit } buffer17_key_we0 { O 1 bit } buffer17_key_d0 { O 32 vector } } \
} "
} else {
puts "@W \[IMPL-110\] Cannot find bus interface model in the library. Ignored generation of bus interface for 'buffer17_key'"
}
}


# XIL_BRAM:
if {${::AESL::PGuard_autoexp_gen}} {
if {[info proc ::AESL_LIB_XILADAPTER::xil_bram_gen] == "::AESL_LIB_XILADAPTER::xil_bram_gen"} {
eval "::AESL_LIB_XILADAPTER::xil_bram_gen { \
    id 178 \
    name buffer17_value \
    reset_level 1 \
    sync_rst true \
    dir O \
    corename buffer17_value \
    op interface \
    ports { buffer17_value_address0 { O 12 vector } buffer17_value_ce0 { O 1 bit } buffer17_value_we0 { O 1 bit } buffer17_value_d0 { O 32 vector } } \
} "
} else {
puts "@W \[IMPL-110\] Cannot find bus interface model in the library. Ignored generation of bus interface for 'buffer17_value'"
}
}


# XIL_BRAM:
if {${::AESL::PGuard_autoexp_gen}} {
if {[info proc ::AESL_LIB_XILADAPTER::xil_bram_gen] == "::AESL_LIB_XILADAPTER::xil_bram_gen"} {
eval "::AESL_LIB_XILADAPTER::xil_bram_gen { \
    id 179 \
    name buffer20_key \
    reset_level 1 \
    sync_rst true \
    dir O \
    corename buffer20_key \
    op interface \
    ports { buffer20_key_address0 { O 12 vector } buffer20_key_ce0 { O 1 bit } buffer20_key_we0 { O 1 bit } buffer20_key_d0 { O 32 vector } } \
} "
} else {
puts "@W \[IMPL-110\] Cannot find bus interface model in the library. Ignored generation of bus interface for 'buffer20_key'"
}
}


# XIL_BRAM:
if {${::AESL::PGuard_autoexp_gen}} {
if {[info proc ::AESL_LIB_XILADAPTER::xil_bram_gen] == "::AESL_LIB_XILADAPTER::xil_bram_gen"} {
eval "::AESL_LIB_XILADAPTER::xil_bram_gen { \
    id 180 \
    name buffer20_value \
    reset_level 1 \
    sync_rst true \
    dir O \
    corename buffer20_value \
    op interface \
    ports { buffer20_value_address0 { O 12 vector } buffer20_value_ce0 { O 1 bit } buffer20_value_we0 { O 1 bit } buffer20_value_d0 { O 32 vector } } \
} "
} else {
puts "@W \[IMPL-110\] Cannot find bus interface model in the library. Ignored generation of bus interface for 'buffer20_value'"
}
}


# XIL_BRAM:
if {${::AESL::PGuard_autoexp_gen}} {
if {[info proc ::AESL_LIB_XILADAPTER::xil_bram_gen] == "::AESL_LIB_XILADAPTER::xil_bram_gen"} {
eval "::AESL_LIB_XILADAPTER::xil_bram_gen { \
    id 181 \
    name buffer21_key \
    reset_level 1 \
    sync_rst true \
    dir O \
    corename buffer21_key \
    op interface \
    ports { buffer21_key_address0 { O 12 vector } buffer21_key_ce0 { O 1 bit } buffer21_key_we0 { O 1 bit } buffer21_key_d0 { O 32 vector } } \
} "
} else {
puts "@W \[IMPL-110\] Cannot find bus interface model in the library. Ignored generation of bus interface for 'buffer21_key'"
}
}


# XIL_BRAM:
if {${::AESL::PGuard_autoexp_gen}} {
if {[info proc ::AESL_LIB_XILADAPTER::xil_bram_gen] == "::AESL_LIB_XILADAPTER::xil_bram_gen"} {
eval "::AESL_LIB_XILADAPTER::xil_bram_gen { \
    id 182 \
    name buffer21_value \
    reset_level 1 \
    sync_rst true \
    dir O \
    corename buffer21_value \
    op interface \
    ports { buffer21_value_address0 { O 12 vector } buffer21_value_ce0 { O 1 bit } buffer21_value_we0 { O 1 bit } buffer21_value_d0 { O 32 vector } } \
} "
} else {
puts "@W \[IMPL-110\] Cannot find bus interface model in the library. Ignored generation of bus interface for 'buffer21_value'"
}
}


# XIL_BRAM:
if {${::AESL::PGuard_autoexp_gen}} {
if {[info proc ::AESL_LIB_XILADAPTER::xil_bram_gen] == "::AESL_LIB_XILADAPTER::xil_bram_gen"} {
eval "::AESL_LIB_XILADAPTER::xil_bram_gen { \
    id 183 \
    name buffer22_key \
    reset_level 1 \
    sync_rst true \
    dir O \
    corename buffer22_key \
    op interface \
    ports { buffer22_key_address0 { O 12 vector } buffer22_key_ce0 { O 1 bit } buffer22_key_we0 { O 1 bit } buffer22_key_d0 { O 32 vector } } \
} "
} else {
puts "@W \[IMPL-110\] Cannot find bus interface model in the library. Ignored generation of bus interface for 'buffer22_key'"
}
}


# XIL_BRAM:
if {${::AESL::PGuard_autoexp_gen}} {
if {[info proc ::AESL_LIB_XILADAPTER::xil_bram_gen] == "::AESL_LIB_XILADAPTER::xil_bram_gen"} {
eval "::AESL_LIB_XILADAPTER::xil_bram_gen { \
    id 184 \
    name buffer22_value \
    reset_level 1 \
    sync_rst true \
    dir O \
    corename buffer22_value \
    op interface \
    ports { buffer22_value_address0 { O 12 vector } buffer22_value_ce0 { O 1 bit } buffer22_value_we0 { O 1 bit } buffer22_value_d0 { O 32 vector } } \
} "
} else {
puts "@W \[IMPL-110\] Cannot find bus interface model in the library. Ignored generation of bus interface for 'buffer22_value'"
}
}


# XIL_BRAM:
if {${::AESL::PGuard_autoexp_gen}} {
if {[info proc ::AESL_LIB_XILADAPTER::xil_bram_gen] == "::AESL_LIB_XILADAPTER::xil_bram_gen"} {
eval "::AESL_LIB_XILADAPTER::xil_bram_gen { \
    id 185 \
    name buffer23_key \
    reset_level 1 \
    sync_rst true \
    dir O \
    corename buffer23_key \
    op interface \
    ports { buffer23_key_address0 { O 12 vector } buffer23_key_ce0 { O 1 bit } buffer23_key_we0 { O 1 bit } buffer23_key_d0 { O 32 vector } } \
} "
} else {
puts "@W \[IMPL-110\] Cannot find bus interface model in the library. Ignored generation of bus interface for 'buffer23_key'"
}
}


# XIL_BRAM:
if {${::AESL::PGuard_autoexp_gen}} {
if {[info proc ::AESL_LIB_XILADAPTER::xil_bram_gen] == "::AESL_LIB_XILADAPTER::xil_bram_gen"} {
eval "::AESL_LIB_XILADAPTER::xil_bram_gen { \
    id 186 \
    name buffer23_value \
    reset_level 1 \
    sync_rst true \
    dir O \
    corename buffer23_value \
    op interface \
    ports { buffer23_value_address0 { O 12 vector } buffer23_value_ce0 { O 1 bit } buffer23_value_we0 { O 1 bit } buffer23_value_d0 { O 32 vector } } \
} "
} else {
puts "@W \[IMPL-110\] Cannot find bus interface model in the library. Ignored generation of bus interface for 'buffer23_value'"
}
}


# XIL_BRAM:
if {${::AESL::PGuard_autoexp_gen}} {
if {[info proc ::AESL_LIB_XILADAPTER::xil_bram_gen] == "::AESL_LIB_XILADAPTER::xil_bram_gen"} {
eval "::AESL_LIB_XILADAPTER::xil_bram_gen { \
    id 187 \
    name buffer24_key \
    reset_level 1 \
    sync_rst true \
    dir O \
    corename buffer24_key \
    op interface \
    ports { buffer24_key_address0 { O 12 vector } buffer24_key_ce0 { O 1 bit } buffer24_key_we0 { O 1 bit } buffer24_key_d0 { O 32 vector } } \
} "
} else {
puts "@W \[IMPL-110\] Cannot find bus interface model in the library. Ignored generation of bus interface for 'buffer24_key'"
}
}


# XIL_BRAM:
if {${::AESL::PGuard_autoexp_gen}} {
if {[info proc ::AESL_LIB_XILADAPTER::xil_bram_gen] == "::AESL_LIB_XILADAPTER::xil_bram_gen"} {
eval "::AESL_LIB_XILADAPTER::xil_bram_gen { \
    id 188 \
    name buffer24_value \
    reset_level 1 \
    sync_rst true \
    dir O \
    corename buffer24_value \
    op interface \
    ports { buffer24_value_address0 { O 12 vector } buffer24_value_ce0 { O 1 bit } buffer24_value_we0 { O 1 bit } buffer24_value_d0 { O 32 vector } } \
} "
} else {
puts "@W \[IMPL-110\] Cannot find bus interface model in the library. Ignored generation of bus interface for 'buffer24_value'"
}
}


# XIL_BRAM:
if {${::AESL::PGuard_autoexp_gen}} {
if {[info proc ::AESL_LIB_XILADAPTER::xil_bram_gen] == "::AESL_LIB_XILADAPTER::xil_bram_gen"} {
eval "::AESL_LIB_XILADAPTER::xil_bram_gen { \
    id 189 \
    name buffer25_key \
    reset_level 1 \
    sync_rst true \
    dir O \
    corename buffer25_key \
    op interface \
    ports { buffer25_key_address0 { O 12 vector } buffer25_key_ce0 { O 1 bit } buffer25_key_we0 { O 1 bit } buffer25_key_d0 { O 32 vector } } \
} "
} else {
puts "@W \[IMPL-110\] Cannot find bus interface model in the library. Ignored generation of bus interface for 'buffer25_key'"
}
}


# XIL_BRAM:
if {${::AESL::PGuard_autoexp_gen}} {
if {[info proc ::AESL_LIB_XILADAPTER::xil_bram_gen] == "::AESL_LIB_XILADAPTER::xil_bram_gen"} {
eval "::AESL_LIB_XILADAPTER::xil_bram_gen { \
    id 190 \
    name buffer25_value \
    reset_level 1 \
    sync_rst true \
    dir O \
    corename buffer25_value \
    op interface \
    ports { buffer25_value_address0 { O 12 vector } buffer25_value_ce0 { O 1 bit } buffer25_value_we0 { O 1 bit } buffer25_value_d0 { O 32 vector } } \
} "
} else {
puts "@W \[IMPL-110\] Cannot find bus interface model in the library. Ignored generation of bus interface for 'buffer25_value'"
}
}


# XIL_BRAM:
if {${::AESL::PGuard_autoexp_gen}} {
if {[info proc ::AESL_LIB_XILADAPTER::xil_bram_gen] == "::AESL_LIB_XILADAPTER::xil_bram_gen"} {
eval "::AESL_LIB_XILADAPTER::xil_bram_gen { \
    id 191 \
    name buffer26_key \
    reset_level 1 \
    sync_rst true \
    dir O \
    corename buffer26_key \
    op interface \
    ports { buffer26_key_address0 { O 12 vector } buffer26_key_ce0 { O 1 bit } buffer26_key_we0 { O 1 bit } buffer26_key_d0 { O 32 vector } } \
} "
} else {
puts "@W \[IMPL-110\] Cannot find bus interface model in the library. Ignored generation of bus interface for 'buffer26_key'"
}
}


# XIL_BRAM:
if {${::AESL::PGuard_autoexp_gen}} {
if {[info proc ::AESL_LIB_XILADAPTER::xil_bram_gen] == "::AESL_LIB_XILADAPTER::xil_bram_gen"} {
eval "::AESL_LIB_XILADAPTER::xil_bram_gen { \
    id 192 \
    name buffer26_value \
    reset_level 1 \
    sync_rst true \
    dir O \
    corename buffer26_value \
    op interface \
    ports { buffer26_value_address0 { O 12 vector } buffer26_value_ce0 { O 1 bit } buffer26_value_we0 { O 1 bit } buffer26_value_d0 { O 32 vector } } \
} "
} else {
puts "@W \[IMPL-110\] Cannot find bus interface model in the library. Ignored generation of bus interface for 'buffer26_value'"
}
}


# XIL_BRAM:
if {${::AESL::PGuard_autoexp_gen}} {
if {[info proc ::AESL_LIB_XILADAPTER::xil_bram_gen] == "::AESL_LIB_XILADAPTER::xil_bram_gen"} {
eval "::AESL_LIB_XILADAPTER::xil_bram_gen { \
    id 193 \
    name buffer27_key \
    reset_level 1 \
    sync_rst true \
    dir O \
    corename buffer27_key \
    op interface \
    ports { buffer27_key_address0 { O 12 vector } buffer27_key_ce0 { O 1 bit } buffer27_key_we0 { O 1 bit } buffer27_key_d0 { O 32 vector } } \
} "
} else {
puts "@W \[IMPL-110\] Cannot find bus interface model in the library. Ignored generation of bus interface for 'buffer27_key'"
}
}


# XIL_BRAM:
if {${::AESL::PGuard_autoexp_gen}} {
if {[info proc ::AESL_LIB_XILADAPTER::xil_bram_gen] == "::AESL_LIB_XILADAPTER::xil_bram_gen"} {
eval "::AESL_LIB_XILADAPTER::xil_bram_gen { \
    id 194 \
    name buffer27_value \
    reset_level 1 \
    sync_rst true \
    dir O \
    corename buffer27_value \
    op interface \
    ports { buffer27_value_address0 { O 12 vector } buffer27_value_ce0 { O 1 bit } buffer27_value_we0 { O 1 bit } buffer27_value_d0 { O 32 vector } } \
} "
} else {
puts "@W \[IMPL-110\] Cannot find bus interface model in the library. Ignored generation of bus interface for 'buffer27_value'"
}
}


# XIL_BRAM:
if {${::AESL::PGuard_autoexp_gen}} {
if {[info proc ::AESL_LIB_XILADAPTER::xil_bram_gen] == "::AESL_LIB_XILADAPTER::xil_bram_gen"} {
eval "::AESL_LIB_XILADAPTER::xil_bram_gen { \
    id 195 \
    name buffer30_key \
    reset_level 1 \
    sync_rst true \
    dir O \
    corename buffer30_key \
    op interface \
    ports { buffer30_key_address0 { O 12 vector } buffer30_key_ce0 { O 1 bit } buffer30_key_we0 { O 1 bit } buffer30_key_d0 { O 32 vector } } \
} "
} else {
puts "@W \[IMPL-110\] Cannot find bus interface model in the library. Ignored generation of bus interface for 'buffer30_key'"
}
}


# XIL_BRAM:
if {${::AESL::PGuard_autoexp_gen}} {
if {[info proc ::AESL_LIB_XILADAPTER::xil_bram_gen] == "::AESL_LIB_XILADAPTER::xil_bram_gen"} {
eval "::AESL_LIB_XILADAPTER::xil_bram_gen { \
    id 196 \
    name buffer30_value \
    reset_level 1 \
    sync_rst true \
    dir O \
    corename buffer30_value \
    op interface \
    ports { buffer30_value_address0 { O 12 vector } buffer30_value_ce0 { O 1 bit } buffer30_value_we0 { O 1 bit } buffer30_value_d0 { O 32 vector } } \
} "
} else {
puts "@W \[IMPL-110\] Cannot find bus interface model in the library. Ignored generation of bus interface for 'buffer30_value'"
}
}


# XIL_BRAM:
if {${::AESL::PGuard_autoexp_gen}} {
if {[info proc ::AESL_LIB_XILADAPTER::xil_bram_gen] == "::AESL_LIB_XILADAPTER::xil_bram_gen"} {
eval "::AESL_LIB_XILADAPTER::xil_bram_gen { \
    id 197 \
    name buffer31_key \
    reset_level 1 \
    sync_rst true \
    dir O \
    corename buffer31_key \
    op interface \
    ports { buffer31_key_address0 { O 12 vector } buffer31_key_ce0 { O 1 bit } buffer31_key_we0 { O 1 bit } buffer31_key_d0 { O 32 vector } } \
} "
} else {
puts "@W \[IMPL-110\] Cannot find bus interface model in the library. Ignored generation of bus interface for 'buffer31_key'"
}
}


# XIL_BRAM:
if {${::AESL::PGuard_autoexp_gen}} {
if {[info proc ::AESL_LIB_XILADAPTER::xil_bram_gen] == "::AESL_LIB_XILADAPTER::xil_bram_gen"} {
eval "::AESL_LIB_XILADAPTER::xil_bram_gen { \
    id 198 \
    name buffer31_value \
    reset_level 1 \
    sync_rst true \
    dir O \
    corename buffer31_value \
    op interface \
    ports { buffer31_value_address0 { O 12 vector } buffer31_value_ce0 { O 1 bit } buffer31_value_we0 { O 1 bit } buffer31_value_d0 { O 32 vector } } \
} "
} else {
puts "@W \[IMPL-110\] Cannot find bus interface model in the library. Ignored generation of bus interface for 'buffer31_value'"
}
}


# XIL_BRAM:
if {${::AESL::PGuard_autoexp_gen}} {
if {[info proc ::AESL_LIB_XILADAPTER::xil_bram_gen] == "::AESL_LIB_XILADAPTER::xil_bram_gen"} {
eval "::AESL_LIB_XILADAPTER::xil_bram_gen { \
    id 199 \
    name buffer32_key \
    reset_level 1 \
    sync_rst true \
    dir O \
    corename buffer32_key \
    op interface \
    ports { buffer32_key_address0 { O 12 vector } buffer32_key_ce0 { O 1 bit } buffer32_key_we0 { O 1 bit } buffer32_key_d0 { O 32 vector } } \
} "
} else {
puts "@W \[IMPL-110\] Cannot find bus interface model in the library. Ignored generation of bus interface for 'buffer32_key'"
}
}


# XIL_BRAM:
if {${::AESL::PGuard_autoexp_gen}} {
if {[info proc ::AESL_LIB_XILADAPTER::xil_bram_gen] == "::AESL_LIB_XILADAPTER::xil_bram_gen"} {
eval "::AESL_LIB_XILADAPTER::xil_bram_gen { \
    id 200 \
    name buffer32_value \
    reset_level 1 \
    sync_rst true \
    dir O \
    corename buffer32_value \
    op interface \
    ports { buffer32_value_address0 { O 12 vector } buffer32_value_ce0 { O 1 bit } buffer32_value_we0 { O 1 bit } buffer32_value_d0 { O 32 vector } } \
} "
} else {
puts "@W \[IMPL-110\] Cannot find bus interface model in the library. Ignored generation of bus interface for 'buffer32_value'"
}
}


# XIL_BRAM:
if {${::AESL::PGuard_autoexp_gen}} {
if {[info proc ::AESL_LIB_XILADAPTER::xil_bram_gen] == "::AESL_LIB_XILADAPTER::xil_bram_gen"} {
eval "::AESL_LIB_XILADAPTER::xil_bram_gen { \
    id 201 \
    name buffer33_key \
    reset_level 1 \
    sync_rst true \
    dir O \
    corename buffer33_key \
    op interface \
    ports { buffer33_key_address0 { O 12 vector } buffer33_key_ce0 { O 1 bit } buffer33_key_we0 { O 1 bit } buffer33_key_d0 { O 32 vector } } \
} "
} else {
puts "@W \[IMPL-110\] Cannot find bus interface model in the library. Ignored generation of bus interface for 'buffer33_key'"
}
}


# XIL_BRAM:
if {${::AESL::PGuard_autoexp_gen}} {
if {[info proc ::AESL_LIB_XILADAPTER::xil_bram_gen] == "::AESL_LIB_XILADAPTER::xil_bram_gen"} {
eval "::AESL_LIB_XILADAPTER::xil_bram_gen { \
    id 202 \
    name buffer33_value \
    reset_level 1 \
    sync_rst true \
    dir O \
    corename buffer33_value \
    op interface \
    ports { buffer33_value_address0 { O 12 vector } buffer33_value_ce0 { O 1 bit } buffer33_value_we0 { O 1 bit } buffer33_value_d0 { O 32 vector } } \
} "
} else {
puts "@W \[IMPL-110\] Cannot find bus interface model in the library. Ignored generation of bus interface for 'buffer33_value'"
}
}


# XIL_BRAM:
if {${::AESL::PGuard_autoexp_gen}} {
if {[info proc ::AESL_LIB_XILADAPTER::xil_bram_gen] == "::AESL_LIB_XILADAPTER::xil_bram_gen"} {
eval "::AESL_LIB_XILADAPTER::xil_bram_gen { \
    id 203 \
    name buffer34_key \
    reset_level 1 \
    sync_rst true \
    dir O \
    corename buffer34_key \
    op interface \
    ports { buffer34_key_address0 { O 12 vector } buffer34_key_ce0 { O 1 bit } buffer34_key_we0 { O 1 bit } buffer34_key_d0 { O 32 vector } } \
} "
} else {
puts "@W \[IMPL-110\] Cannot find bus interface model in the library. Ignored generation of bus interface for 'buffer34_key'"
}
}


# XIL_BRAM:
if {${::AESL::PGuard_autoexp_gen}} {
if {[info proc ::AESL_LIB_XILADAPTER::xil_bram_gen] == "::AESL_LIB_XILADAPTER::xil_bram_gen"} {
eval "::AESL_LIB_XILADAPTER::xil_bram_gen { \
    id 204 \
    name buffer34_value \
    reset_level 1 \
    sync_rst true \
    dir O \
    corename buffer34_value \
    op interface \
    ports { buffer34_value_address0 { O 12 vector } buffer34_value_ce0 { O 1 bit } buffer34_value_we0 { O 1 bit } buffer34_value_d0 { O 32 vector } } \
} "
} else {
puts "@W \[IMPL-110\] Cannot find bus interface model in the library. Ignored generation of bus interface for 'buffer34_value'"
}
}


# XIL_BRAM:
if {${::AESL::PGuard_autoexp_gen}} {
if {[info proc ::AESL_LIB_XILADAPTER::xil_bram_gen] == "::AESL_LIB_XILADAPTER::xil_bram_gen"} {
eval "::AESL_LIB_XILADAPTER::xil_bram_gen { \
    id 205 \
    name buffer35_key \
    reset_level 1 \
    sync_rst true \
    dir O \
    corename buffer35_key \
    op interface \
    ports { buffer35_key_address0 { O 12 vector } buffer35_key_ce0 { O 1 bit } buffer35_key_we0 { O 1 bit } buffer35_key_d0 { O 32 vector } } \
} "
} else {
puts "@W \[IMPL-110\] Cannot find bus interface model in the library. Ignored generation of bus interface for 'buffer35_key'"
}
}


# XIL_BRAM:
if {${::AESL::PGuard_autoexp_gen}} {
if {[info proc ::AESL_LIB_XILADAPTER::xil_bram_gen] == "::AESL_LIB_XILADAPTER::xil_bram_gen"} {
eval "::AESL_LIB_XILADAPTER::xil_bram_gen { \
    id 206 \
    name buffer35_value \
    reset_level 1 \
    sync_rst true \
    dir O \
    corename buffer35_value \
    op interface \
    ports { buffer35_value_address0 { O 12 vector } buffer35_value_ce0 { O 1 bit } buffer35_value_we0 { O 1 bit } buffer35_value_d0 { O 32 vector } } \
} "
} else {
puts "@W \[IMPL-110\] Cannot find bus interface model in the library. Ignored generation of bus interface for 'buffer35_value'"
}
}


# XIL_BRAM:
if {${::AESL::PGuard_autoexp_gen}} {
if {[info proc ::AESL_LIB_XILADAPTER::xil_bram_gen] == "::AESL_LIB_XILADAPTER::xil_bram_gen"} {
eval "::AESL_LIB_XILADAPTER::xil_bram_gen { \
    id 207 \
    name buffer36_key \
    reset_level 1 \
    sync_rst true \
    dir O \
    corename buffer36_key \
    op interface \
    ports { buffer36_key_address0 { O 12 vector } buffer36_key_ce0 { O 1 bit } buffer36_key_we0 { O 1 bit } buffer36_key_d0 { O 32 vector } } \
} "
} else {
puts "@W \[IMPL-110\] Cannot find bus interface model in the library. Ignored generation of bus interface for 'buffer36_key'"
}
}


# XIL_BRAM:
if {${::AESL::PGuard_autoexp_gen}} {
if {[info proc ::AESL_LIB_XILADAPTER::xil_bram_gen] == "::AESL_LIB_XILADAPTER::xil_bram_gen"} {
eval "::AESL_LIB_XILADAPTER::xil_bram_gen { \
    id 208 \
    name buffer36_value \
    reset_level 1 \
    sync_rst true \
    dir O \
    corename buffer36_value \
    op interface \
    ports { buffer36_value_address0 { O 12 vector } buffer36_value_ce0 { O 1 bit } buffer36_value_we0 { O 1 bit } buffer36_value_d0 { O 32 vector } } \
} "
} else {
puts "@W \[IMPL-110\] Cannot find bus interface model in the library. Ignored generation of bus interface for 'buffer36_value'"
}
}


# XIL_BRAM:
if {${::AESL::PGuard_autoexp_gen}} {
if {[info proc ::AESL_LIB_XILADAPTER::xil_bram_gen] == "::AESL_LIB_XILADAPTER::xil_bram_gen"} {
eval "::AESL_LIB_XILADAPTER::xil_bram_gen { \
    id 209 \
    name buffer37_key \
    reset_level 1 \
    sync_rst true \
    dir O \
    corename buffer37_key \
    op interface \
    ports { buffer37_key_address0 { O 12 vector } buffer37_key_ce0 { O 1 bit } buffer37_key_we0 { O 1 bit } buffer37_key_d0 { O 32 vector } } \
} "
} else {
puts "@W \[IMPL-110\] Cannot find bus interface model in the library. Ignored generation of bus interface for 'buffer37_key'"
}
}


# XIL_BRAM:
if {${::AESL::PGuard_autoexp_gen}} {
if {[info proc ::AESL_LIB_XILADAPTER::xil_bram_gen] == "::AESL_LIB_XILADAPTER::xil_bram_gen"} {
eval "::AESL_LIB_XILADAPTER::xil_bram_gen { \
    id 210 \
    name buffer37_value \
    reset_level 1 \
    sync_rst true \
    dir O \
    corename buffer37_value \
    op interface \
    ports { buffer37_value_address0 { O 12 vector } buffer37_value_ce0 { O 1 bit } buffer37_value_we0 { O 1 bit } buffer37_value_d0 { O 32 vector } } \
} "
} else {
puts "@W \[IMPL-110\] Cannot find bus interface model in the library. Ignored generation of bus interface for 'buffer37_value'"
}
}


# Direct connection:
if {${::AESL::PGuard_autoexp_gen}} {
eval "cg_default_interface_gen_dc { \
    id 145 \
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
    id 146 \
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
    id 211 \
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


