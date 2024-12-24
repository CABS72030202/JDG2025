#!/bin/bash

# Connect to remote RPI 3b
echo -e "To connect to the Raspberry Pi, please turn on the mobile hotspot named 'Pixel Seb' and ensure that the user is also connected to this network.\n"

# Send test_msg.txt to the remote Raspberry Pi and rename it to input.txt
local_file_path="../doc/secret_command/test_msg.txt"
remote_file_path="/home/admin/input.txt"
scp "$local_file_path" admin@192.168.193.231:"$remote_file_path"
if [[ $? -ne 0 ]]; then
  echo "Failed to transfer test_msg.txt to the remote Raspberry Pi. Exiting."
  exit 1
fi

# Attempt to connect to the remote Raspberry Pi
retry_limit=3
retry_count=0
while true; do
  ssh admin@192.168.193.231
  if [[ $? -eq 0 ]]; then
    echo "Connected to Raspberry Pi successfully."
    break
  else
    echo "Failed to connect to Raspberry Pi."
    retry_count=$((retry_count + 1))
    if [[ $retry_count -ge $retry_limit ]]; then
      echo "Reached maximum retry attempts. Exiting."
      exit 1
    fi
    read -p "Do you want to retry? (Y/N): " retry_choice
    if [[ $retry_choice =~ ^[Nn]$ ]]; then
      echo "Exiting."
      exit 1
    fi
  fi
done

# Function to validate numeric input
validate_number() {
  local input=$1
  local min=$2
  local max=$3
  if [[ $input =~ ^[0-9]+$ ]] && [ $input -ge $min ] && [ $input -le $max ]; then
    echo 1
  else
    echo 0
  fi
}

# Function to display usage information
usage() {
  echo "Usage: $0"
  echo "This script configures and runs the brute force program on a remote RPI 3b."
  echo "It also automatically transfers the previous output to this repository."
  echo "Options are entered interactively."
  exit 1
}

# Validate command-line arguments
if [[ $# -ne 0 ]]; then
  usage
fi

# Ask the user for parameters
read -p "Enable double test? (Y/N): " double_test
while [[ ! $double_test =~ ^[YyNn]$ ]]; do
  echo "Invalid input. Please enter Y or N."
  read -p "Enable double test? (Y/N): " double_test
done

read -p "Enable caps test? (Y/N): " caps_test
while [[ ! $caps_test =~ ^[YyNn]$ ]]; do
  echo "Invalid input. Please enter Y or N."
  read -p "Enable caps test? (Y/N): " caps_test
done

# Validate min and max length inputs
while true; do
  read -p "Enter minimum word length (1 to 13): " min_length
  if [[ $(validate_number $min_length 1 13) -eq 1 ]]; then
    break
  else
    echo "Invalid input. Please enter a number between 1 and 13."
  fi
done

while true; do
  read -p "Enter maximum word length (2 to 14): " max_length
  if [[ $(validate_number $max_length 2 14) -eq 1 && $max_length -ge $min_length ]]; then
    break
  else
    echo "Invalid input. Please ensure the maximum length is between 2 and 14 and greater than or equal to the minimum length."
  fi
done

read -p "Enter the starting string: " start_string
if [[ -z $start_string ]]; then
  start_string="\0"
fi

# Convert Y/N responses to 1/0
double_test=$(echo "$double_test" | tr '[:lower:]' '[:upper:]')
caps_test=$(echo "$caps_test" | tr '[:lower:]' '[:upper:]')
double_test_flag=$([[ "$double_test" == "Y" ]] && echo 1 || echo 0)
caps_test_flag=$([[ "$caps_test" == "Y" ]] && echo 1 || echo 0)

# Update the C code with the user inputs
sed -i "s/^#define DOUBLE_TEST.*/#define DOUBLE_TEST $double_test_flag/" brute_force.c
sed -i "s/^#define CAPS_TEST.*/#define CAPS_TEST $caps_test_flag/" brute_force.c
sed -i "s/^#define MIN_LENGTH.*/#define MIN_LENGTH $min_length/" brute_force.c
sed -i "s/^#define MAX_LENGTH.*/#define MAX_LENGTH $max_length/" brute_force.c
sed -i "s/^#define START_STR.*/#define START_STR \"$start_string\"/" brute_force.c

# Save previous output and transfer to repository
output_file="./log/$(date +%Y-%m-%d_%H:%M:%S)_output.txt"
mv output.txt "$output_file"
path="sebas@SCStudio:/mnt/c/Users/sebas/OneDrive - Université du Québec à Trois-Rivières/Implication/JDG/Machine 2025/JDG2025/doc/secret_command/brute_force_output/"

# Check if the remote is accessible before transferring files
if ssh -q sebas@SCStudio exit; then
  echo "Transferring files to $path..."
  scp -r "$output_file" ${path}
else
  echo "Remote repository is not accessible. Skipping file transfer."
fi

# Compile the C code
gcc -o brute_force brute_force.c -lwiringPi

# Check if compilation was successful
if [[ $? -eq 0 ]]; then
  echo "Compilation successful. Running the brute force program..."
  ./brute_force
else
  echo "Compilation failed. Please check the C code for errors."
fi