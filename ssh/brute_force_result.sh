#!/bin/bash

# Default values
file_path="/mnt/c/Users/sebas/OneDrive - Université du Québec à Trois-Rivières/Implication/JDG/Machine 2025/JDG2025/doc/secret_command/brute_force_output"
file_name="output.txt"

# Function to display usage information
usage() {
  echo "Usage: $0 [OPTIONS]"
  echo ""
  echo "Options:"
  echo "  -h, --help          Show the help message"
  echo "  -a, --all           Show all values"
  echo "  -r, --result        Show all values that could be a valid secret word"
  echo "  -c, --connect       Show the station connection values"
  echo "  -t, --timeout       Show the timeout values"
  echo "  -o, --other         Show all unexpected values"
  echo "  -C, --config        Show brute force config"
  echo "  -f, --file <path>   Specify a custom directory path (default: ../doc/secret_command/brute_force_output/)"
  echo "  -n, --name <name>   Specify a custom file name (default: output.txt)"
  echo ""
}

# Validate the full file path
validate_file() {
  local full_path="$file_path/$file_name"
  if [[ ! -f $full_path ]]; then
    echo "Error: File '$full_path' not found."
    exit 1
  fi
}

# Check for no arguments
if [ $# -eq 0 ]; then
  usage
  exit 1
fi

# Process the command-line arguments
while [[ "$1" != "" ]]; do
  case $1 in
    -h | --help)
      usage
      exit 0
      ;;
    -a | --all)
      validate_file
      cat "$file_path/$file_name"
      exit 0
      ;;
    -r | --result)
      validate_file
      grep -v '^[-#]' "$file_path/$file_name"
      shift
      ;;
    -c | --connect)
      validate_file
      grep '^#' "$file_path/$file_name"
      shift
      ;;
    -t | --timeout)
      validate_file
      grep '^!' "$file_path/$file_name"
      shift
      ;;
    -o | --other)
      validate_file
      grep -v '^[-#!]' "$file_path/$file_name"
      shift
      ;;
    -C | --config)
      validate_file
      grep '^-' "$file_path/$file_name"
      shift
      ;;
    -f | --file)
      shift
      if [[ -z $1 ]]; then
        echo "Error: No directory path specified."
        usage
        exit 1
      fi
      file_path="$1"
      shift
      ;;
    -n | --name)
      shift
      if [[ -z $1 ]]; then
        echo "Error: No file name specified."
        usage
        exit 1
      fi
      file_name="$1"
      shift
      ;;
    *)
      echo "Error: Invalid option '$1'"
      usage
      exit 1
      ;;
  esac
done
