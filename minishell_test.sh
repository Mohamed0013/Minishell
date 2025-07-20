#!/bin/bash

# Path to your minishell binary
MINISHELL=./minishell

# Helper to run a command in bash and minishell, and compare outputs
test_case() {
    DESC="$1"
    CMD="$2"
    echo -e "\n===== $DESC ====="
    echo "> $CMD"
    echo "--- bash output ---"
    bash -c "$CMD"
    echo "--- minishell output ---"
    echo "$CMD" | $MINISHELL
}

# 1. Simple command
test_case "Simple command: /bin/ls" "/bin/ls"

test_case "Simple command with arguments: /bin/ls -l /" "/bin/ls -l /"

test_case "Empty command" ""

test_case "Only spaces" "   "

# 2. echo
test_case "echo with arguments" "echo Hello World"
test_case "echo -n option" "echo -n Hello"
test_case "echo with multiple arguments" "echo arg1 arg2 arg3"

test_case "echo with quotes" "echo 'single quoted' \"double quoted\""

test_case "echo with environment variable" "export TESTVAR=42; echo $TESTVAR"

test_case "echo with $?" "ls /notfound; echo $?"

# 3. exit
test_case "exit command" "exit"

test_case "exit with argument" "exit 42"

# 4. env
test_case "env command" "env"

# 5. export/unset
test_case "export new variable" "export FOO=bar; echo $FOO"
test_case "unset variable" "export FOO=bar; unset FOO; echo $FOO"

test_case "export overwrite variable" "export FOO=bar; export FOO=baz; echo $FOO"

# 6. cd/pwd
test_case "cd to /tmp and pwd" "cd /tmp; pwd"
test_case "cd to .. and pwd" "cd ..; pwd"

test_case "cd to non-existent dir" "cd /doesnotexist"

test_case "pwd command" "pwd"

# 7. Redirections
test_case "output redirection" "echo Hello > testfile && cat testfile && rm testfile"
test_case "input redirection" "echo Hello > testfile && cat < testfile && rm testfile"
test_case "append redirection" "echo First > testfile; echo Second >> testfile; cat testfile; rm testfile"

test_case "heredoc (manual)" "cat << EOF\nHello\nEOF"

# 8. Pipes
test_case "simple pipe" "echo Hello | cat"
test_case "multiple pipes" "echo Hello | cat | grep H"

test_case "pipe with redirection" "echo Hello | tee testfile; cat testfile; rm testfile"

test_case "invalid command in pipe" "notacommand | cat"

# 9. Quotes
test_case "single quotes" "echo 'This is $HOME'"
test_case "double quotes" "echo \"This is $HOME\""

test_case "mixed quotes" "echo 'A'\"B\"'C'"

# 10. Relative path
test_case "relative path command" "./minishell_test.sh"

test_case "complex relative path" "cd /tmp; mkdir -p a/b/c; cd a/b/c; pwd; cd ../../../; rm -rf a"

# 11. PATH variable
test_case "unset PATH and try ls" "unset PATH; ls"
test_case "set PATH and try ls" "export PATH=/bin; ls"

# 12. Error handling
test_case "invalid command" "dsbksdgbksdghsd"

echo -e "\n===== Manual tests required for: Ctrl-C, Ctrl-D, Ctrl-\\, heredoc, and history navigation =====\n"
echo "Please test these interactively in your minishell." 