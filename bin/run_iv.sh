#!/bin/sh

# RTL directory
RTL_DIR="../rtl"
BEHAVIOR_DIR="../behavior"
TOP_MODULE=top

sim_file=$1
echo "Test Scenario=" $sim_file
BENCH_DIR=$(dirname "${sim_file}")

# delete previous data
if [ -e ${TOP_MODULE} ]; then
  rm -f ${TOP_MODULE}
fi

# compile
iverilog -c ../bin/cmd.txt \
-v \
-y ${RTL_DIR} \
-y ${RTL_DIR}/fm_axi_m \
-y ${RTL_DIR}/fm_axi_s \
-y ${RTL_DIR}/fm_sys \
-y ${RTL_DIR}/fm_3d \
-y ${RTL_DIR}/fm_rd \
-y ${RTL_DIR}/fm_cmn \
-y ${RTL_DIR}/fm_mic \
-y ${RTL_DIR}/fm_hvc \
-y ${RTL_DIR}/fm_hdmi \
-y ${BEHAVIOR_DIR} \
-I ${BENCH_DIR} \
-I ${RTL_DIR}/include_32 \
-I ${RTL_DIR}/fm_3d \
-o ${TOP_MODULE} \
${sim_file}

# PLI
if [ ! -e ../bin/pli/pli_boot.so ]; then
  (
    cd ../bin/pli
    make
  )
fi

# simulation
vvp -mcadpli top -cadpli=../bin/pli/pli_boot.so:my_bootstrap

# sim result -> bmp converter
if [ ! -e ../bin/a2bmp/a2bmp ]; then
  (
    cd ../bin/a2bmp
    make
  )
fi

if [ -e frame_buffer.dat ]; then
  echo "generating result bmp ..."
  ../bin/a2bmp/a2bmp frame_buffer.dat result.bmp 640 480
  echo "done"
fi
