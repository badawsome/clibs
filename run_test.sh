#!/usr/bin/env bash
set -ex

rm -rf build
build_mod=debug
build_mod_list=('debug' 'release')

if [ ! -n "$1" ]; then
  echo "you may specific debug/release mode by \`./run_test.sh debug\` or \`./run_test.sh release\`"
else
  build_mod=$1
  flag=false
  for mod in "${build_mod_list[@]}"
  do
    [[ "${1}" = "${mod}" ]] && flag=true
  done
  [[ "${flag}" = "false" ]] && echo "fail to judge your mode, please check your command." && exit 1
fi

cmake_build_mod=$(echo "${1:0:1}" | tr '[:lower:]' '[:upper:]')${1:1}

#echo "cmake -B build/debug -DCMAKE_BUILD_TYPE=${build_mod} -G Ninja"
cmake -B build/"${build_mod}" -DCMAKE_BUILD_TYPE="${cmake_build_mod}" -G Ninja
cmake --build build/"${build_mod}"
ninja test -C build/"${build_mod}"