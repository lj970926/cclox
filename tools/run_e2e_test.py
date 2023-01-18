import argparse
import os
import subprocess
import typing
import re

EXPECTED_OUPUT_PATTERN = re.compile(r"// expected: ?(.*)")
EXPECTED_ERROR_PATTERN = re.compile(r"// (Error.*)")
NON_TEST_PATTERN = re.compile(r"// nontest")

sample_basedir = "sample"

class Test:
    def __init__(self, expected_outputs=list()) -> None:
        self.expected_ouputs = expected_outputs


def run_test(test_path: str):
    lines = list()
    with open(test_path, "r", encoding="utf-8") as fp:
        lines.extend(fp.readlines())
    if not parse(lines):
        return
    
    run(test_path, lines)

def  parse(file_path, lines: typing.List[str]):
    expected_outputs = list();
    for line in lines:
        match = NON_TEST_PATTERN.search(line)
        if match:
            print(f"Skip nontest file {file_path}")
            return False
        match = EXPECTED_OUPUT_PATTERN.search(line)
        if match:
            expected_outputs.append(match[1])

def run(lines):
    pass

def main(args):
    cclox_path = args.executable
    suites = args.suites
    for suite_name in suites:
        suite_dir = os.path.join(sample_basedir, suite_name)
        for test_name in os.listdir(suite_dir):
            run_test(os.path.join(suite_dir, test_name))


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("-e", "--executable", type=str, default="build/cclox")
    parser.add_argument("suites", nargs='*')
    args = parser.parse_args()
    main(args)
