# Shell-Interface-Development

## Custom Shell Interface for xv6  

A simple shell implementation for the **xv6 operating system**, supporting command execution, **I/O redirection**, and **piping**.

## Features  

- **Basic Command Execution**: Runs user commands with a `>>>` prompt.  
- **Built-in `cd` Command**: Changes directories within the shell.  
- **Input/Output Redirection**: Supports `>` and `<` operators.  
- **Pipelines**: Handles single and multi-stage command pipelines (`|`).  
- **Sequential Execution**: Supports the `;` operator for running multiple commands.  

## How to Compile & Run  

1. **Navigate to the xv6 user directory:**  
   ```bash
   cd xv6-riscv/user
   ```
2. **Compile the shell:**  
   ```bash
   make qemu
   ```
3. **Run the shell inside xv6:**  
   ```bash
   myshell
   ```

## Example Commands  

```bash
>>> echo "Hello world" > temp
>>> cat < temp
Hello world

>>> ls | grep test | cat
file1_test.txt  
file2_test.txt

>>> ls | grep test | cat > myoutput
>>> cat myoutput
file1_test.txt  
file2_test.txt

>>> ls; echo "Done"
file1.txt  
file2.txt  
Done
```
