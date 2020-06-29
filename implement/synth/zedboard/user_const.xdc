####################################################################################
# Constraints from file : 'user_const.xdc'
####################################################################################

# analog-vga
# "VGA-B1"
set_property PACKAGE_PIN Y21 [get_ports {o_vb[0]}]
# The conversion of 'IOSTANDARD' constraint on 'net' object 'o_vb[0]' has been applied to the port object 'o_vb[0]'.
set_property IOSTANDARD LVCMOS33 [get_ports {o_vb[0]}]
# "VGA-B2"
set_property PACKAGE_PIN Y20 [get_ports {o_vb[1]}]
# The conversion of 'IOSTANDARD' constraint on 'net' object 'o_vb[1]' has been applied to the port object 'o_vb[1]'.
set_property IOSTANDARD LVCMOS33 [get_ports {o_vb[1]}]
# "VGA-B3"
set_property PACKAGE_PIN AB20 [get_ports {o_vb[2]}]
# The conversion of 'IOSTANDARD' constraint on 'net' object 'o_vb[2]' has been applied to the port object 'o_vb[2]'.
set_property IOSTANDARD LVCMOS33 [get_ports {o_vb[2]}]
# "VGA-B4"
set_property PACKAGE_PIN AB19 [get_ports {o_vb[3]}]
# The conversion of 'IOSTANDARD' constraint on 'net' object 'o_vb[3]' has been applied to the port object 'o_vb[3]'.
set_property IOSTANDARD LVCMOS33 [get_ports {o_vb[3]}]
# "VGA-G1"
set_property PACKAGE_PIN AB22 [get_ports {o_vg[0]}]
# The conversion of 'IOSTANDARD' constraint on 'net' object 'o_vg[0]' has been applied to the port object 'o_vg[0]'.
set_property IOSTANDARD LVCMOS33 [get_ports {o_vg[0]}]
# "VGA-G2"
set_property PACKAGE_PIN AA22 [get_ports {o_vg[1]}]
# The conversion of 'IOSTANDARD' constraint on 'net' object 'o_vg[1]' has been applied to the port object 'o_vg[1]'.
set_property IOSTANDARD LVCMOS33 [get_ports {o_vg[1]}]
# "VGA-G3"
set_property PACKAGE_PIN AB21 [get_ports {o_vg[2]}]
# The conversion of 'IOSTANDARD' constraint on 'net' object 'o_vg[2]' has been applied to the port object 'o_vg[2]'.
set_property IOSTANDARD LVCMOS33 [get_ports {o_vg[2]}]
# "VGA-G4"
set_property PACKAGE_PIN AA21 [get_ports {o_vg[3]}]
# The conversion of 'IOSTANDARD' constraint on 'net' object 'o_vg[3]' has been applied to the port object 'o_vg[3]'.
set_property IOSTANDARD LVCMOS33 [get_ports {o_vg[3]}]
# "VGA-HS"
set_property PACKAGE_PIN AA19 [get_ports o_hsync_x]
# The conversion of 'IOSTANDARD' constraint on 'net' object 'o_hsync_x' has been applied to the port object 'o_hsync_x'.
set_property IOSTANDARD LVCMOS33 [get_ports o_hsync_x]
# "VGA-R1"
set_property PACKAGE_PIN V20 [get_ports {o_vr[0]}]
# The conversion of 'IOSTANDARD' constraint on 'net' object 'o_vr[0]' has been applied to the port object 'o_vr[0]'.
set_property IOSTANDARD LVCMOS33 [get_ports {o_vr[0]}]
# "VGA-R2"
set_property PACKAGE_PIN U20 [get_ports {o_vr[1]}]
# The conversion of 'IOSTANDARD' constraint on 'net' object 'o_vr[1]' has been applied to the port object 'o_vr[1]'.
set_property IOSTANDARD LVCMOS33 [get_ports {o_vr[1]}]
# "VGA-R3"
set_property PACKAGE_PIN V19 [get_ports {o_vr[2]}]
# The conversion of 'IOSTANDARD' constraint on 'net' object 'o_vr[2]' has been applied to the port object 'o_vr[2]'.
set_property IOSTANDARD LVCMOS33 [get_ports {o_vr[2]}]
# "VGA-R4"
set_property PACKAGE_PIN V18 [get_ports {o_vr[3]}]
# The conversion of 'IOSTANDARD' constraint on 'net' object 'o_vr[3]' has been applied to the port object 'o_vr[3]'.
set_property IOSTANDARD LVCMOS33 [get_ports {o_vr[3]}]
# "VGA-VS"
set_property PACKAGE_PIN Y19 [get_ports o_vsync_x]
# The conversion of 'IOSTANDARD' constraint on 'net' object 'o_vsync_x' has been applied to the port object 'o_vsync_x'.
set_property IOSTANDARD LVCMOS33 [get_ports o_vsync_x]
# hdmi
# "HD-CLK"
#set_property PACKAGE_PIN W18 [get_ports clk_vo]
# The conversion of 'IOSTANDARD' constraint on 'net' object 'clk_vo' has been applied to the port object 'clk_vo'.
set_property IOSTANDARD LVCMOS33 [get_ports clk_vo]
# "HD-D0"
#set_property PACKAGE_PIN Y13 [get_ports {o_hd_d[0]}]
# The conversion of 'IOSTANDARD' constraint on 'net' object 'o_hd_d[0]' has been applied to the port object 'o_hd_d[0]'.
set_property IOSTANDARD LVCMOS33 [get_ports {o_hd_d[0]}]
# "HD-D1"
#set_property PACKAGE_PIN AA13 [get_ports {o_hd_d[1]}]
# The conversion of 'IOSTANDARD' constraint on 'net' object 'o_hd_d[1]' has been applied to the port object 'o_hd_d[1]'.
#set_property IOSTANDARD LVCMOS33 [get_ports {o_hd_d[1]}]
# "HD-D10"
#set_property PACKAGE_PIN W13 [get_ports {o_hd_d[10]}]
# The conversion of 'IOSTANDARD' constraint on 'net' object 'o_hd_d[10]' has been applied to the port object 'o_hd_d[10]'.
#set_property IOSTANDARD LVCMOS33 [get_ports {o_hd_d[10]}]
# "HD-D11"
set_property PACKAGE_PIN W15 [get_ports {o_hd_d[11]}]
# The conversion of 'IOSTANDARD' constraint on 'net' object 'o_hd_d[11]' has been applied to the port object 'o_hd_d[11]'.
set_property IOSTANDARD LVCMOS33 [get_ports {o_hd_d[11]}]
# "HD-D12"
#set_property PACKAGE_PIN V15 [get_ports {o_hd_d[12]}]
# The conversion of 'IOSTANDARD' constraint on 'net' object 'o_hd_d[12]' has been applied to the port object 'o_hd_d[12]'.
#set_property IOSTANDARD LVCMOS33 [get_ports {o_hd_d[12]}]
# "HD-D13"
set_property PACKAGE_PIN U17 [get_ports {o_hd_d[13]}]
# The conversion of 'IOSTANDARD' constraint on 'net' object 'o_hd_d[13]' has been applied to the port object 'o_hd_d[13]'.
#set_property IOSTANDARD LVCMOS33 [get_ports {o_hd_d[13]}]
# "HD-D14"
set_property PACKAGE_PIN V14 [get_ports {o_hd_d[14]}]
# The conversion of 'IOSTANDARD' constraint on 'net' object 'o_hd_d[14]' has been applied to the port object 'o_hd_d[14]'.
#set_property IOSTANDARD LVCMOS33 [get_ports {o_hd_d[14]}]
# "HD-D15"
#set_property PACKAGE_PIN V13 [get_ports {o_hd_d[15]}]
# The conversion of 'IOSTANDARD' constraint on 'net' object 'o_hd_d[15]' has been applied to the port object 'o_hd_d[15]'.
set_property IOSTANDARD LVCMOS33 [get_ports {o_hd_d[15]}]
# "HD-D2"
set_property PACKAGE_PIN AA14 [get_ports {o_hd_d[2]}]
# The conversion of 'IOSTANDARD' constraint on 'net' object 'o_hd_d[2]' has been applied to the port object 'o_hd_d[2]'.
#set_property IOSTANDARD LVCMOS33 [get_ports {o_hd_d[2]}]
# "HD-D3"
set_property PACKAGE_PIN Y14 [get_ports {o_hd_d[3]}]
# The conversion of 'IOSTANDARD' constraint on 'net' object 'o_hd_d[3]' has been applied to the port object 'o_hd_d[3]'.
#set_property IOSTANDARD LVCMOS33 [get_ports {o_hd_d[3]}]
# "HD-D4"
set_property PACKAGE_PIN AB15 [get_ports {o_hd_d[4]}]
# The conversion of 'IOSTANDARD' constraint on 'net' object 'o_hd_d[4]' has been applied to the port object 'o_hd_d[4]'.
#set_property IOSTANDARD LVCMOS33 [get_ports {o_hd_d[4]}]
# "HD-D5"
set_property PACKAGE_PIN AB16 [get_ports {o_hd_d[5]}]
# The conversion of 'IOSTANDARD' constraint on 'net' object 'o_hd_d[5]' has been applied to the port object 'o_hd_d[5]'.
#set_property IOSTANDARD LVCMOS33 [get_ports {o_hd_d[5]}]
# "HD-D6"
set_property PACKAGE_PIN AA16 [get_ports {o_hd_d[6]}]
# The conversion of 'IOSTANDARD' constraint on 'net' object 'o_hd_d[6]' has been applied to the port object 'o_hd_d[6]'.
#set_property IOSTANDARD LVCMOS33 [get_ports {o_hd_d[6]}]
# "HD-D7"
set_property PACKAGE_PIN AB17 [get_ports {o_hd_d[7]}]
# The conversion of 'IOSTANDARD' constraint on 'net' object 'o_hd_d[7]' has been applied to the port object 'o_hd_d[7]'.
#set_property IOSTANDARD LVCMOS33 [get_ports {o_hd_d[7]}]
# "HD-D8"
set_property PACKAGE_PIN AA17 [get_ports {o_hd_d[8]}]
# The conversion of 'IOSTANDARD' constraint on 'net' object 'o_hd_d[8]' has been applied to the port object 'o_hd_d[8]'.
#set_property IOSTANDARD LVCMOS33 [get_ports {o_hd_d[8]}]
# "HD-D9"
#set_property PACKAGE_PIN Y15 [get_ports {o_hd_d[9]}]
# The conversion of 'IOSTANDARD' constraint on 'net' object 'o_hd_d[9]' has been applied to the port object 'o_hd_d[9]'.
#set_property IOSTANDARD LVCMOS33 [get_ports {o_hd_d[9]}]
# "HD-DE"
set_property PACKAGE_PIN U16 [get_ports o_hd_de]
# The conversion of 'IOSTANDARD' constraint on 'net' object 'o_hd_de' has been applied to the port object 'o_hd_de'.
#set_property IOSTANDARD LVCMOS33 [get_ports o_hd_de]
# "HD-HSYNC"
#set_property PACKAGE_PIN V17 [get_ports o_hd_hsync]
# The conversion of 'IOSTANDARD' constraint on 'net' object 'o_hd_hsync' has been applied to the port object 'o_hd_hsync'.
#set_property IOSTANDARD LVCMOS33 [get_ports o_hd_hsync]
# "HD-SCL"
set_property PACKAGE_PIN AA18 [get_ports io_scl]
# The conversion of 'IOSTANDARD' constraint on 'net' object 'io_scl' has been applied to the port object 'io_scl'.
#set_property IOSTANDARD LVCMOS33 [get_ports io_scl]
# "HD-SDA"
set_property PACKAGE_PIN Y16 [get_ports io_sda]
# The conversion of 'IOSTANDARD' constraint on 'net' object 'io_sda' has been applied to the port object 'io_sda'.
#set_property IOSTANDARD LVCMOS33 [get_ports io_sda]
# "HD-VSYNC"
set_property PACKAGE_PIN W17 [get_ports o_hd_vsync]
# The conversion of 'IOSTANDARD' constraint on 'net' object 'o_hd_vsync' has been applied to the port object 'o_hd_vsync'.
#set_property IOSTANDARD LVCMOS33 [get_ports o_hd_vsync]

# "GCLK"
set_property PACKAGE_PIN Y9 [get_ports CLK_100]
# The conversion of 'IOSTANDARD' constraint on 'net' object 'CLK_100' has been applied to the port object 'CLK_100'.
set_property IOSTANDARD LVCMOS33 [get_ports CLK_100]
create_clock -period 10.000 -name CLK_100 [get_ports CLK_100]

# The following cross clock domain false path constraints can be uncommented in order to mimic ucf constraints behavior (see message at the beginning of this file)
# set_false_path -from [get_clocks CLK_100] -to [get_clocks [list FCLK_CLK1 FCLK_CLK0 FCLK_CLK3]]
# set_false_path -from [get_clocks FCLK_CLK1] -to [get_clocks [list CLK_100 FCLK_CLK0 FCLK_CLK3]]
# set_false_path -from [get_clocks FCLK_CLK0] -to [get_clocks [list CLK_100 FCLK_CLK1 FCLK_CLK3]]
# set_false_path -from [get_clocks FCLK_CLK3] -to [get_clocks [list CLK_100 FCLK_CLK1 FCLK_CLK0]]


# falth path bw main clock and video clock
set_false_path -from [get_clocks clk_fpga_0] -to [get_clocks clk_v_pll]
set_false_path -from [get_clocks clk_v_pll] -to [get_clocks clk_fpga_0]


