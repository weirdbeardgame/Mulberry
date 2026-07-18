# Disable printing "Entering/Leaving directory ..."
MAKEFLAGS += --no-print-directory

NUMPROC ?= $(shell nproc)

CHECK_ENV_CMDS := \
	python3 \
	ninja \
	stat \
	iconv \
	mips-ps2-decompals-as \
	mips-ps2-decompals-strip \
	mips-ps2-decompals-ld \
	mips-ps2-decompals-objcopy

GREEN  := \033[32m
RED    := \033[31m
YELLOW := \033[33m
RESET  := \033[0m

.check-env:
	@missing=0; \
	output=""; \
	for cmd in $(CHECK_ENV_CMDS); do \
		if command -v $$cmd >/dev/null 2>&1; then \
			output="$$output  ${GREEN}[OK]${RESET}      $$cmd\n"; \
		else \
			output="$$output  ${RED}[MISSING]${RESET} $$cmd\n"; \
			missing=1; \
		fi; \
	done; \
	if [ $$missing -eq 1 ]; then \
		printf "%-11s %s\n" "STATUS" "COMMAND"; \
		printf "%-11s %s\n" "------" "-------"; \
		printf "$$output"; \
		printf "\n${RED}Environment check failed${RESET}\n\n"; \
		exit 1; \
	fi

WITH_ENV := .check-env

.PHONY: help \
		.build-only .check-files-on-error configure build extract-data make-asm map-mismatch report clean \

.DEFAULT_GOAL := help

# python script that parses makefiles and pretty-prints help messages
define PY_HELP_SCRIPT
import os
import re
import sys

CYAN = "\033[36m"
CRST = "\033[0m"

makefiles = sys.argv[1:]
target_max_len = 0
target_help_list = []

for file in makefiles:
	with open(file, mode="r") as fh:
		for line in fh:
			if match := re.match(r"^([a-zA-Z0-9_-]+):\s*##\s*(.*)$$", line):
				target, help_str = match.groups()
				target_max_len = max(target_max_len, len(target))
				target_help_list.append((target, help_str))

			if match := re.match(r"^##\s*(.*)$$", line):
				comment = match.group(1)
				target_help_list.append((None, comment))

all_targets = [target for target, help_or_comment in target_help_list if target]

print(f"{CYAN}usage:{CRST} make <{CYAN}{f'{CRST}|{CYAN}'.join(all_targets)}{CRST}>")
print()

for target, help_or_comment in target_help_list:
	if target:
		print(f"  {CYAN}{target:{target_max_len + 2}s}{CRST} {help_or_comment}")
	else:
		print(help_or_comment)

print()
endef

export PY_HELP_SCRIPT
PYHELP := python3 -c "$$PY_HELP_SCRIPT"

help: $(WITH_ENV) ## Show this help
	@$(PYHELP) $(MAKEFILE_LIST)

##
##  Commands:
configure: ## Configure  project (needs SLUS_210.07)
	@python3 configure.py config/kuon.yaml -c

.build-only:
	@cd config/; \
	ninja -t clean; \
	ninja -j$(NUMPROC)

.check-files-on-error:
	ls -l config/build/SLUS_210.07 config/SLUS_210.07

build: ## Build project
	@$(MAKE) .build-only; status=$$?; [ $$status -eq 0 ] || { $(MAKE) .check-files-on-error; }; exit $$status

extract-data:  ## Extract variables from .data in config directory
	@python3 tools/python/parse_data.py 

map-mismatch:  ## Check for mismatches in mapfile
	@python3 tools/python/map_mismatch.py --language 

report:  ## Create progress report in config directory
	@(stat config/expected/obj/ >/dev/null 2>&1 || (echo "Target objects do not exist, please run \`make make-asm\`"; false));
	@(stat config/build/src/ >/dev/null 2>&1 || (echo "Base objects do not exist, please run \`make build\`"; false));
	@tools/objdiff/objdiff-cli report generate -p config/ -o config/report.json -f json
	@python3 tools/python/fix_report.py config/report.json
	@python3 -c "import json;from pathlib import Path;report=json.loads(Path('config/report.json').read_text());print(f\"Progress: {report['measures']['fuzzy_match_percent']:.2f}%\")"

clean:  ## Clean artifact in config directory
	@cd config/; \
	ninja -t clean

reset:  ## Resets the  config directory to its original state
	@python3 configure.py config/kuon.yaml --reset

# Include custom makefile for user-defined commands
-include Makefile.custom
