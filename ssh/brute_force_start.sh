#!/bin/bash

# Configuration
ip_address="10.40.82.231"
host_user="sebas"
host_repo_path="/mnt/c/Users/sebas/OneDrive - Université du Québec à Trois-Rivières/Implication/JDG/2025/Machine 2025/JDG2025/doc/secret_command/brute_force_output"
remote_user="admin"
remote_code_path="/home/admin/brute_force.c"
remote_script_path="/home/admin/remote_task.sh"
local_code_path="../doc/secret_command/brute_force.c"
local_text_path="../doc/secret_command/test_msg.txt"

# Validate numeric input
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

# Display usage information
usage() {
  echo "Usage: $0"
  echo "This script configures and runs the brute force program on a remote RPI 3b."
  echo ""
  echo "DOUBLE TEST | Test every word with and without ':' at the end"
  echo "CAPS TEST   | Test every word with full caps"
  exit 1
}

# Validate command-line arguments
if [[ $# -ne 0 ]]; then
  usage
fi

# Prompt user for parameters
read -p "Enable double test? (Y/N): " double_test
while [[ ! $double_test =~ ^[YyNn]$ ]]; do
  echo "Invalid input. Please enter Y or N."
  read -p "Enable double test? (Y/N): " double_test
done

double_test=$(echo "$double_test" | tr '[:lower:]' '[:upper:]')
double_test_flag=$([[ "$double_test" == "Y" ]] && echo 1 || echo 0)

read -p "Enable caps test? (Y/N): " caps_test
while [[ ! $caps_test =~ ^[YyNn]$ ]]; do
  echo "Invalid input. Please enter Y or N."
  read -p "Enable caps test? (Y/N): " caps_test
done

caps_test=$(echo "$caps_test" | tr '[:lower:]' '[:upper:]')
caps_test_flag=$([[ "$caps_test" == "Y" ]] && echo 1 || echo 0)

read -p "Enable combinaison generation? (Y/N): " combinaison_gen
while [[ ! $combinaison_gen =~ ^[YyNn]$ ]]; do
  echo "Invalid input. Please enter Y or N."
  read -p "Enable combinaison generation? (Y/N): " combinaison_gen
done

combinaison_gen=$(echo "$combinaison_gen" | tr '[:lower:]' '[:upper:]')
combinaison_gen_flag=$([[ "$combinaison_gen" == "Y" ]] && echo 1 || echo 0)

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

#read -p "Enter the starting string: " start_string
#if [[ -z $start_string ]]; then
#  start_string=" "
#fi

# Update source code with parameters
sudo sed -i "s/^#define DOUBLE_TEST.*/#define DOUBLE_TEST $double_test_flag/" "$local_code_path"
sudo sed -i "s/^#define CAPS_TEST.*/#define CAPS_TEST $caps_test_flag/" "$local_code_path"
sudo sed -i "s/^#define MIN_LENGTH.*/#define MIN_LENGTH $min_length/" "$local_code_path"
sudo sed -i "s/^#define MAX_LENGTH.*/#define MAX_LENGTH $max_length/" "$local_code_path"
#sudo sed -i "s/^#define START_STR.*/#define START_STR \"$start_string\"/" "$local_code_path"
sudo sed -i "s/^#define COMBINAISON_GEN.*/#define COMBINAISON_GEN $combinaison_gen_flag/" "$local_code_path"

# Prepare remote task script
cat <<'EOF' > remote_task.sh
#!/bin/bash

# Compile and run the brute force program
gcc -o brute_force brute_force.c -lwiringPi
if [[ $? -eq 0 ]]; then
  echo "Compilation successful. Running the brute force program..."
  ./brute_force
else
  echo "Compilation failed. Please check the C code for errors."
fi
EOF
sudo chmod +x remote_task.sh

# Transfer files to remote
scp "$local_code_path" "$remote_user@$ip_address:$remote_code_path"
scp "$local_text_path" "$remote_user@$ip_address:/home/admin/input.txt"
scp remote_task.sh "$remote_user@$ip_address:$remote_script_path"

if [[ $? -ne 0 ]]; then
  echo "Failed to transfer files to the remote Raspberry Pi. Exiting."
  exit 1
fi

# Save previous output and transfer to host repository
output_file="$(date +%Y-%m-%d_%H-%M-%S)_output.txt"
scp -r "$remote_user@$ip_address":~/output.txt "$host_repo_path"/"$output_file"

# Execute remote task
ssh "$remote_user@$ip_address" "bash $remote_script_path"
sudo rm "/mnt/c/Users/sebas/OneDrive - Université du Québec à Trois-Rivières/Implication/JDG/Machine 2025/JDG2025/ssh/remote_task.sh"