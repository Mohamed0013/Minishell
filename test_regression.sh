#!/bin/bash

echo "=== Testing for regressions ==="

echo "Test 1: Normal variable expansion with word splitting"
echo -e 'export files="file1.txt file2.txt"\nls $files' | ./minishell

echo -e "\nTest 2: Echo with word splitting"
echo -e 'export words="hello world test"\necho $words' | ./minishell

echo -e "\nTest 3: Command substitution scenarios"
echo 'echo hello world | wc' | ./minishell

echo -e "\nTest 4: Export assignment vs variable expansion"
echo -e 'export cmd="echo test"\n$cmd' | ./minishell

echo -e "\nRegression tests completed"
