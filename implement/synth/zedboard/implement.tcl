set PROJ_NAME polyphony
set PROJ_DIR .
set TOP_NAME zedboard
set BD_IP_DIR ./${PROJ_NAME}.srcs/sources_1/bd/zed_base/ip

create_project -in_memory -part xc7z020clg484-1
set_property board_part em.avnet.com:zed:part0:1.3 [current_project]
set_property design_mode GateLvl [current_fileset]
set_property parent.project_path ${PROJ_DIR}/${PROJ_NAME}.xpr [current_project]
set_property ip_output_repo ${PROJ_DIR}/${PROJ_NAME}.cache/ip [current_project]
set_property ip_cache_permissions {read write} [current_project]
set_property XPM_LIBRARIES {XPM_CDC XPM_MEMORY} [current_project]
add_files -quiet ${TOP_NAME}.dcp

# version check
if {[expr [version -short]] >= 2016.3} {
  set PS7 ps7
} else {
  set PS7 processing_system7
}

set aaa [file normalize ${PROJ_DIR}/${PROJ_NAME}.srcs/sources_1/bd/zed_base/zed_base.bd]
# read .bd, instead of reading *.xdc
read_bd ${aaa}
#read_xdc dont_touch.xdc
read_xdc user_const.xdc
# Link
link_design -top ${TOP_NAME} -part xc7z020clg484-1
write_hwdef -force -file ${TOP_NAME}.hwdef
# Opt
opt_design 
write_checkpoint -force ${TOP_NAME}_opt.dcp
report_drc -file ${TOP_NAME}_drc_opted.rpt
# Place
place_design 
write_checkpoint -force ${TOP_NAME}_placed.dcp
report_io -file ${TOP_NAME}_io_placed.rpt
report_utilization -file ${TOP_NAME}_utilization_placed.rpt -pb ${TOP_NAME}_utilization_placed.pb
report_control_sets -verbose -file ${TOP_NAME}_control_sets_placed.rpt
# Phys Opt
phys_opt_design 
write_checkpoint -force ${TOP_NAME}_physopt.dcp
# Route
route_design 
write_checkpoint -force ${TOP_NAME}_routed.dcp
report_drc -file ${TOP_NAME}_drc_routed.rpt -pb ${TOP_NAME}_drc_routed.pb
report_timing_summary -warn_on_violation -max_paths 10 -file ${TOP_NAME}_timing_summary_routed.rpt -rpx ${TOP_NAME}_timing_summary_routed.rpx
report_power -file ${TOP_NAME}_power_routed.rpt -pb ${TOP_NAME}_power_summary_routed.pb
report_route_status -file ${TOP_NAME}_route_status.rpt -pb ${TOP_NAME}_route_status.pb
report_clock_utilization -file ${TOP_NAME}_clock_utilization_routed.rpt
