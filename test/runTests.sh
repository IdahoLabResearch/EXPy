#!/bin/bash

script_dir="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

echo "Searching for test*.py files in $script_dir..."

test_files=$(find "$script_dir" -maxdepth 1 -name "test*.py" -type f)

if [ -z "$test_files" ]; then
    echo "No test*.py files found in $script_dir."
    exit 0
fi

echo "Found test files:"
echo "$test_files"
echo ""

for test_file in $test_files; do
    echo "Running $test_file..."
    echo "============================================================================================================================================================"
    cd "$script_dir"
    python3 "$test_file"
    echo "============================================================================================================================================================"
    echo ""
    
done

echo "All tests completed."