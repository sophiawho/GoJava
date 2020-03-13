# Golite

## Group Members
- Matthew Lesko 260692352
- Sophia Hu 260705681

[Milestone 2 Dropbox Paper](https://paper.dropbox.com/doc/Golite-Milestone-2--AwDbpX81PV0nzKifOwLyDc~xAQ-nTimwbzrqj6beL9ysXVZA)

## Grading Instructions: Milestone 2
Our incorrect programs for submission are found in `programs/2-typecheck/invalid/example/`. Please ignore the rest of the programs, they are for our tests.

## Grading Instructions: Milestone 1
**Syntactically-Correct Example Programs**:
Our correct programs are found in `programs/1-scan+parse/valid/` and contian the prefix `meaningful*.go`. Please ignore the rest of the programs, they are for our tests.

**Syntactically-Incorrect Example Programs**:
Please ignore all the programs in `programs/1-scan+parse/invalid/` that contain the prefix `weed*.go`. The rest of the programs are the programs that are minimally correct and are for marking, eg: `assign1.go`.

## Rebasing on master
```
$ git fetch origin
$ git rebase origin/master
```

## Coding convention:

`branching`: Branch naming should follow convention `name`-`feature/bug`-`short_title`, eg: `sophia-feature-scanner` where possible.

## Report:

[Report Link.](https://paper.dropbox.com/doc/GoLite-Deliverable-1-Report--AurU1nCuNuN3fKv1OgcMTFcHAg-18wB5Wgr4INOjytUhIiwh)

# Assignment Template

To organize your assignments and project for class, please follow the skeleton provided in this repository

We provide 3 scripts for convenience and two directories for organization:

* `programs`: Test programs are organized by compilation phase and by expected result. Valid programs should output `OK` and status code `0`, while invalid programs should output `Error: <description>` and status code `1`.
  * `Scan+parse`: Runs both the scanner and parser phases
  * `Typecheck`: Runs until the end of the typechecker phase
  * `Codegen`: Runs until your compiler outputs the target code
* `src`: Source code for your assignment
* `build.sh`: Builds your compiler using `Make` or similar. You should replace the commands here if necessary to build your compiler
* `run.sh`: Runs your compiler using two arguments (mode - $1 and input file - $2). You should replace the commands here if necessary to invoke your compiler
* `test.sh`: Automatically runs your compiler against test programs in the programs directory and checks the output

Comments found in both files provide more details as to the requirements. Since a large portion of grading is performed automatically, please ensure to follow the input/output specifications **exactly**. You must be able to run the provided scripts on the SOCS Trottier machines.
