#!/bin/bash

# Read parent pid, child pid
path=false
while [ "$#" -gt 0 ]; do
  case "$1" in
    --parent)
      parent="$2"
      shift 2
      ;;
    --child)
      child="$2"
      shift 2
      ;;
    --path)
      path=true
      shift
      ;;
    *)
      echo "Unknown parameter passed: $1"
      exit 1
      ;;
  esac
done

# Check if parent or child is empty
if [ -z "$parent" ] || [ -z "$child" ]; then
  echo "Usage: $0 --parent PARENT_PID --child CHILD_PID [--path]"
  exit 1
fi

########
# TODO #
########

# The code below is only for demonstration purposes, please remove it before submitting.
# echo "parent pid: $parent, child pid: $child"

proc_path=($child)

function find_path() {
  local parent="$1"
  local child="$2"
  
  for i in $(ps --ppid $parent -o pid --no-headers);
  do   
    if [ $i = $child ]; then
      proc_path+=($parent) 
      return 0
    fi

    if find_path $i $child; then
      proc_path+=($parent) 
      return 0
    fi
  done 

  return 1
}

if find_path "$parent" "$child"; then
  echo "Yes"
  if $path; then 
    echo ${proc_path[@]} | \
      awk -F' ' '{for (i=NF; i>0; --i) printf "%s%s", (i<NF ? " -> " : ""), $i; printf "\n"}'
  fi
else
  echo "No"
fi

