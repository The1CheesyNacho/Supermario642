#!/usr/bin/python3

import sys, subprocess

if len(sys.argv) != 2:
    print(f"Usage: {sys.argv[0]} [folder to search]")
    sys.exit()

lightFiles = set()

# The original command that was run across the entire repo to convert all assets
# cmds = ("grep -Rl \"gsSPSetLight\" levels actors bin","grep -Rl \"gsSPLight\" levels actors bin","grep -Rl \"Lights1\" levels actors bin")

# Operate on the folder passed as an argument to this program
cmds = [f"grep -Rl \"gsSPSetLight\" {sys.argv[1]}",f"grep -Rl \"gsSPLight\" {sys.argv[1]}",f"grep -Rl \"Lights1\" {sys.argv[1]}"]

for cmd in cmds:
    try:
        output = subprocess.check_output(cmd, shell=True).decode('UTF-8').split("\n")[:-1] 
        lightFiles = set.union(lightFiles,set(output))
    except subprocess.CalledProcessError:
        continue

lightStructs = {}

# FOURTH/FIFTH PASS - CHANGE LIGHT COMMANDS AGAIN
for file in lightFiles:
    changed = False
    with open(file, "r") as f:
        fileLines = f.readlines()
        index = 0
        while index < len(fileLines):
            curLine = fileLines[index].strip()
            # Run this after you've run the command below, and comment it out
            #if curLine.startswith("gsSPLightColor(LIGHT_1"):
                #fileLines[index] = curLine.replace("LIGHT_1", "LIGHT_2") + '\n'
            # Run this first, then run the above a separate and second time    
            if curLine.startswith("gsSPLightColor(LIGHT_2"):
                fileLines[index] = curLine.replace("LIGHT_2", "LIGHT_3") + '\n'
                changed = True

            index += 1
    # Don't write to the file if the contents haven't been changed, as this will trigger a rebuild by Make
    if changed:
        with open(file, "w") as f:
            f.writelines(fileLines)

