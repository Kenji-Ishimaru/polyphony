#/bin/bash

remove_files_ext () {
  for file in *.$1
  do
    if [ ! -e "$file" ]; then
      break
    fi

    rm -f "$file"
    echo "$file has been removed" 
  done
}

remove_files_ext xpr
remove_files_ext dcp
remove_files_ext pb
remove_files_ext rpt
remove_files_ext rptx
remove_files_ext xml
remove_files_ext html
remove_files_ext txt
remove_files_ext bit
remove_files_ext hwdef
remove_files_ext mmi
remove_files_ext xsa

if [ -d ./polyphony.srcs ]; then
  rm -rf polyphony.srcs
fi

if [ -d ./polyphony.ip_user_files ]; then
  rm -rf polyphony.ip_user_files
fi

if [ -d ./polyphony.cache ]; then
  rm -rf polyphony.cache
fi

if [ -d ./polyphony.hw ]; then
  rm -rf polyphony.hw
fi

if [ -d ./polyphony.sim ]; then
  rm -rf polyphony.sim
fi

if [ -d ./NA ]; then
  rm -rf NA
fi

if [ -d ./Packages ]; then
  rm -rf Packages
fi
