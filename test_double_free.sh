#!/bin/bash

echo "=== Test pour les double free dans minishell ==="

# Test 1: Export avec variables valides et invalides
echo "Test 1: Export avec variables"
echo 'export VAR1=value1 VAR2=value2 INVALID-VAR=value3' | valgrind --tool=memcheck --leak-check=full --show-leak-kinds=all --track-origins=yes ./minishell

# Test 2: Export sans arguments (affichage des variables)
echo "Test 2: Export sans arguments"
echo 'export' | valgrind --tool=memcheck --leak-check=full ./minishell

# Test 3: Test de l'expansion de variables
echo "Test 3: Expansion de variables"
echo 'export TEST=hello; echo $TEST $NONEXISTENT' | valgrind --tool=memcheck --leak-check=full ./minishell

# Test 4: Test de exit
echo "Test 4: Exit"
echo 'exit 0' | valgrind --tool=memcheck --leak-check=full ./minishell

echo "=== Tests terminés ==="
