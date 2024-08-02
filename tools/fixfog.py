#!/usr/bin/python3

import sys, subprocess

if len(sys.argv) != 2:
    print(f"Usage: {sys.argv[0]} [folder to search]")
    sys.exit()

lightFiles = set()

# The original command that was run across the entire repo to convert all assets
# cmds = ("grep -Rl \"gDPSetFogColor\" levels actors bin","grep -Rl \"gsSPFogPosition\" levels actors bin")

# Operate on the folder passed as an argument to this program
cmds = [f"grep -Rl \"gDPSetFogColor\" {sys.argv[1]}",f"grep -Rl \"gsSPFogPosition\" {sys.argv[1]}"]

for cmd in cmds:
    try:
        output = subprocess.check_output(cmd, shell=True).decode('UTF-8').split("\n")[:-1] 
        lightFiles = set.union(lightFiles,set(output))
    except subprocess.CalledProcessError:
        continue

lightStructs = {}

# FIRST PASS - REMOVE FOG COMMANDS
for file in lightFiles:
    changed = False
    with open(file, "r") as f:
        fileLines = f.readlines()
        index = 0
        while index < len(fileLines):
            curLine = fileLines[index].strip()

            if curLine.startswith("gsDPSetFogColor"):
                fileLines[index] = curLine.replace("gsDPSetFogColor", "//gsDPSetFogColor") + '\n'
            if curLine.startswith("gsSPFogPosition"):
                fileLines[index] = curLine.replace("gsSPFogPosition", "//gsSPFogPosition") + '\n'
                changed = True

            index += 1
    # Don't write to the file if the contents haven't been changed, as this will trigger a rebuild by Make
    if changed:
        with open(file, "w") as f:
            f.writelines(fileLines)

