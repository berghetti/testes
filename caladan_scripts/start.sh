#!/bin/bash

proc_killall()
{
  echo "killing processes..."
  sudo killall iokerneld 2> /dev/null
  sudo killall synthetic 2> /dev/null
  sudo killall memcached 2> /dev/null
  sudo killall stress_shm_query 2> /dev/null
  sudo killall swaptions 2> /dev/null
}

MY_PATH="/home/users/mberghetti"
HOST=$(hostname)

do_server()
{
  proc_killall
  echo "Starting IOkernel..."
  sudo ${MY_PATH}/caladan/iokerneld ias noht 2-18:2,22-38:2 &> /dev/null &

  #wait iokernel start
  sleep 10
  echo "Starting memcached..."
  ${MY_PATH}/memcached/memcached ${MY_PATH}/memcached/memcached.config -U 5000 -p 5000 -c 8192 -m 8192 -b 8192 \
   -o hashpower=25,no_slab_reassign,no_lru_maintainer,no_lru_crawler,no_hashexpand &> /dev/null &

  echo "Starting memory monitor..."
  sudo numactl -N1 ${MY_PATH}/caladan/apps/netbench/stress_shm_query membw:1000 > ${MY_PATH}/mem.txt 2> /dev/null &

  echo "Starting swaptions..."
  ${MY_PATH}/caladan-all/parsec/pkgs/apps/swaptions/inst/amd64-linux.gcc-shenango-gc/bin/swaptions \
  ${MY_PATH}/caladan-all/parsec/swaptions.config -ns 5000000 -sm 400 -nt 64 &> /dev/null

  echo "Server stared, run client now"
}

stop_server()
{
  echo "Stoping server..."
  ssh mberghetti@10.90.0.20 "${MY_PATH}/start.sh stop"
}

start_server_from_client()
{
  echo "Starting server..."
  ssh mberghetti@10.90.0.20 "${MY_PATH}/start.sh server" &
}

PPS=0.2

do_client()
{
  proc_killall

  start_server_from_client

  echo "Starting IOkernel...($HOST)"
  sudo ${MY_PATH}/caladan/iokerneld ias 3-19:2,23-39:2 &>/dev/null &
  #sudo ${MY_PATH}/caladan/iokerneld ias noht 3-19:2 &>/dev/null &
  
  sleep 15;
  echo "Starting synthetic client...($HOST)"
  ${MY_PATH}/caladan/apps/synthetic/target/release/synthetic 192.168.50.1:5000 \
  -c ${MY_PATH}/caladan/client.config --mode runtime-client --runtime 60 --samples 1 \
  --start_mpps ${PPS} --mpps ${PPS} --threads 1024  --transport udp \
  -d bimodal2 --protocol memcached --nvalues 1000000 --value_size 1024 --key_size 64 --pctset 500 --warmup -o trace > ${MY_PATH}/trace.txt && \
  stop_server;

  proc_killall

  echo "Copyng file from server..."
  scp mberghetti@10.90.0.20:${MY_PATH}/\{mem.txt,swaptions.txt\} ${MY_PATH}/.;

  echo "Plotting chart..."
  python3 ${MY_PATH}/plot_window.py ${MY_PATH}/trace.txt ${MY_PATH}/mem.txt ${MY_PATH}/swaptions.txt ${MY_PATH}/out.png

  echo "Plotted chart ${MY_PATH}/out.png"
}

if [[ $1 == "server" ]]; then
  do_server
elif [[ $1 == "client" ]]; then
  do_client
elif [[ $1 == "stop" ]]; then
  proc_killall
else
  echo "Usage: $0 server|client|stop"
fi

