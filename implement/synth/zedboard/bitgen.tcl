set_msg_config -id {HDL 9-1061} -limit 100000
set_msg_config -id {HDL 9-1654} -limit 100000

set PROJ_NAME polyphony
set PROJ_DIR .
set SDK_DIR ./sdk
set TOP_NAME zedboard

open_checkpoint ${TOP_NAME}_routed.dcp
write_bitstream -verbose -force ${TOP_NAME}.bit 

write_hw_platform -fixed -force  -include_bit -file ${TOP_NAME}.xsa

#write_sysdef -force -hwdef ${TOP_NAME}.hwdef -bitfile ${TOP_NAME}.bit -meminfo ${TOP_NAME}.mmi -file ${TOP_NAME}.sysdef


#file copy -force ${TOP_NAME}.sysdef ${SDK_DIR}.2015.4/${TOP_NAME}.hdf
#file copy -force ${TOP_NAME}.sysdef ${SDK_DIR}.2016.x/${TOP_NAME}.hdf
