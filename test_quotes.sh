#!/bin/bash

echo "Testing quoted cases with minishell"
export TEST_VAR="file1 file2"

echo "Test 1: Quoted variable (should work)"
echo 'echo test > "$TEST_VAR"' | ./minishell

echo -e "\nTest 2: Unquoted variable (should fail with ambiguous redirect)"
echo 'echo test > $TEST_VAR' | ./minishell

echo -e "\nTest 3: Quoted literal with spaces (should work)"
echo 'echo test > "literal with spaces"' | ./minishell

echo -e "\nTest 4: Unquoted literal with spaces (should fail)"
echo 'echo test > literal with spaces' | ./minishell
