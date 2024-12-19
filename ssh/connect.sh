#!/bin/bash

# Display usage instructions if no parameter is provided
if [ $# -eq 0 ]; then
  echo "Usage: $0 <1|2>"
  echo "1: Connect to Raspberry Pi 3 Model B"
  echo "2: Connect to Raspberry Pi Zero 2"
  exit 1
fi

# Set connection details based on the parameter
if [ "$1" -eq 1 ]; then
  echo -e "To connect to the Raspberry Pi, please turn on the mobile hotspot named 'Pixel Seb' and ensure that the user is also connected to this network.\n"
  ssh admin@192.168.193.231
elif [ "$1" -eq 2 ]; then
  echo -e "To connect to Raspberry Pi, please ensure you are connected to the appropriate network.\n"
  ssh admin@192.168.193.73 
else
  echo "Invalid option. Please use 1 or 2."
  exit 1
fi

