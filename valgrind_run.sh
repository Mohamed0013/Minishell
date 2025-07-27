#!/bin/bash
# Convenience script to run minishell with valgrind and proper suppression

valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --suppressions=readline.supp ./minishell "$@"
