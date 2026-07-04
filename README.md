# Mulbery (桑)
## What is Mulberry?
Mulberry is a project that aims to have a matching source decompilation of the PS2 game Kuon. Once we have a matching decompiled rom we want to port the game to modern hardware.


## Disclaimer
No game assets are hosted on this repository. You must provide them yourself, copying them from a legal copy.


## Getting Started & Building
Linux is required in order to build this project. It requires a compiler that is only available for Linux. You'll also need at least `python 3.8` to run any command. Your linux distribution should come with `python3` already installed. If not, please follow your distribution's instructions to install it. Python is a mandatory dependency, as many core tools are written in python. On Ubuntu, if necessary, python3 can be installed with the following command:
```bash
sudo apt install python3-full
```

### Add i386 architecture
The original GCC compiler is a 32-bit executable, so on a 64-bit system the `i386` architecture must be added in order for the system to run it. On Ubuntu you can use the following commands:
```bash
sudo dpkg --add-architecture i386
sudo apt update
sudo apt install libc6:i386 libstdc++6:i386
```

### Install dependencies
```bash
sudo apt install make python3-venv
```

### Install decompals binutils

This project requires a custom PS2-specific version of binutils to assemble the extracted MIPS assembly. The system-provided `binutils-mips-linux-gnu` package may not work correctly with PS2 assembly, so manual installation is required.

Download the [binutils-mips-ps2-decompals-linux-x86-64.tar.gz (v0.10)](https://github.com/decompals/binutils-mips-ps2-decompals/releases/download/v0.10/binutils-mips-ps2-decompals-linux-x86-64.tar.gz) package.

Extract the archive to a location of your choice, for example:

```bash
sudo mkdir -p /opt/binutils-mips-ps2-decompals
sudo tar -xzf binutils-mips-ps2-decompals-linux-x86-64.tar.gz -C /opt/binutils-mips-ps2-decompals
```

Then add the bin directory to your `PATH` environment variable:

```bash
export PATH="/opt/binutils-mips-ps2-decompals/:$PATH"
```

To make this change persistent, add the line above to your shell configuration file (for example `~/.bashrc`, `~/.zshrc`, or equivalent), then restart your terminal or reload your shell.

Verify the installation by running:

```bash
mips-ps2-decompals-as --version
```

If the installation was successful, the command should print the assembler version information.


### Setup a Virtual Environment for Python
Python's virtual environments are the preferred way to use this software, as you may not be allowed to install packages globally. 
```bash
python3 -m venv .venv
source .venv/bin/activate
python3 -m pip install -r requirements.txt
```

### Copy assets from game DVD/ISO
The main executable is needed in order to perform the decompilation.

#### JP
```bash
make jp-clean
```

### Command Help
To have a list all available commands, run `make` without targets:
```bash
make
```

## Decompiling a TU
1. Add the decompiled code to the TU's `c` file in `src/`
2. Update `config/kuon.yaml`:
   1. Replace `asm` with `c` for the TU you are decompiling
   2. Add a leading dot (`.`) to the type (e.g., `rodata` -> `.rodata`) of each subsection that belongs to the TU
3. Reconfigure the project
4. Build the project

