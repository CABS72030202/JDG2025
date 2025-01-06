#!/bin/bash

ip_address1="10.40.82.231"
ip_address2="10.40.82.73"

# Display usage instructions if no parameter is provided
if [ $# -eq 0 ]; then
  echo "Usage: $0 <1|2>"
  echo "1: Transfer files to Raspberry Pi 3 Model B"
  echo "2: Transfer files to Raspberry Pi Zero 2"
  exit 1
fi

# Define paths for each machine
if [ "$1" -eq 1 ]; then
  path="admin@"$ip_address1":~"
elif [ "$1" -eq 2 ]; then
  path="admin@"$ip_address2":~"
else
  echo "Invalid option. Please use 1 or 2."
  exit 1
fi

# Transfer files
echo "Transferring files to $path..."
scp -r "/mnt/c/Users/sebas/OneDrive - Université du Québec à Trois-Rivières/Implication/JDG/Machine 2025/JDG2025/lib" ${path}
scp -r "/mnt/c/Users/sebas/OneDrive - Université du Québec à Trois-Rivières/Implication/JDG/Machine 2025/JDG2025/src" ${path}
//scp -r "/mnt/c/Users/sebas/OneDrive - Université du Québec à Trois-Rivières/Implication/JDG/Machine 2025/JDG2025/test" ${path}
scp -r "/mnt/c/Users/sebas/OneDrive - Université du Québec à Trois-Rivières/Implication/JDG/Machine 2025/JDG2025/Makefile" ${path}

echo "File transfer completed."

