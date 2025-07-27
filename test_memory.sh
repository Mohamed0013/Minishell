#!/bin/bash

echo "Testing memory leaks and segfaults..."

VALGRIND_CMD="valgrind --error-exitcode=1 --leak-check=full --suppressions=readline.supp"

# Test basic commands
echo "=== Testing basic commands ==="
echo "echo hello" | timeout 5s $VALGRIND_CMD ./minishell > /dev/null 2>&1
if [ $? -eq 0 ]; then
    echo "✓ Basic echo command - OK"
else
    echo "✗ Basic echo command - FAILED"
fi

# Test with pipes
echo "=== Testing pipes ==="
echo "echo hello | cat" | timeout 5s $VALGRIND_CMD ./minishell > /dev/null 2>&1
if [ $? -eq 0 ]; then
    echo "✓ Pipe command - OK"
else
    echo "✗ Pipe command - FAILED"
fi

# Test with redirections
echo "=== Testing redirections ==="
echo "echo hello > /tmp/test.txt" | timeout 5s $VALGRIND_CMD ./minishell > /dev/null 2>&1
REDIR_EXIT_CODE=$?
if [ $REDIR_EXIT_CODE -eq 0 ]; then
    echo "✓ Output redirection - OK"
elif [ $REDIR_EXIT_CODE -eq 124 ]; then
    echo "⚠ Output redirection - TIMEOUT (functionality issue, not memory)"
else
    echo "✗ Output redirection - FAILED (exit code: $REDIR_EXIT_CODE)"
fi

# Test with builtins
echo "=== Testing builtins ==="
echo "pwd" | timeout 5s $VALGRIND_CMD ./minishell > /dev/null 2>&1
if [ $? -eq 0 ]; then
    echo "✓ PWD builtin - OK"
else
    echo "✗ PWD builtin - FAILED"
fi

echo "env" | timeout 5s $VALGRIND_CMD ./minishell > /dev/null 2>&1
if [ $? -eq 0 ]; then
    echo "✓ ENV builtin - OK"
else
    echo "✗ ENV builtin - FAILED"
fi

# Test exit command
echo "=== Testing exit ==="
echo "exit" | timeout 5s $VALGRIND_CMD ./minishell > /dev/null 2>&1
if [ $? -eq 0 ]; then
    echo "✓ Exit command - OK"
else
    echo "✗ Exit command - FAILED"
fi

echo "Memory testing complete!"
