#!/bin/bash

echo "=== Testing export fixes ==="

echo "Test 1: export with spaces in quotes"
echo 'export t="ls -la"' | ./minishell

echo -e "\nTest 2: Check if variable is set correctly"
echo -e 'export t="ls -la"\necho $t' | ./minishell

echo -e "\nTest 3: export without quotes"
echo 'export test=hello' | ./minishell

echo -e "\nTest 4: Multiple exports"
echo -e 'export var1="hello world"\nexport var2=test\necho $var1 $var2' | ./minishell

echo -e "\nTest 5: Export with single quotes"
echo "export single='hello world'" | ./minishell

echo -e "\nTest 6: Normal word splitting should still work"
echo 'echo "one two three"' | ./minishell

echo -e "\nAll tests completed"
