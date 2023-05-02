#!/bin/bash

VM=AFP

YELLOW='\033[0;33m'
RESET='\033[0m' # reset color


shutdown_and_exit()
{
  echo -e "\nStopping ${VM}"
  virsh shutdown AFP;
  exit
}

trap shutdown_and_exit SIGINT SIGHUP SIGTERM

echo "Starting ${VM}"
virsh start ${VM}; 
echo -e "${YELLOW}Keep this terminal open while using the virtual machine ${VM}.\n"\
        "\rClose this terminal or typing CTRL+C will poweroff the virtual machine ${VM}${RESET}.";
sleep infinity
