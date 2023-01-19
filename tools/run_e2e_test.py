import argparse
import os
import subprocess
import typing
import re

EXPECTED_OUPUT_PATTERN = re.compile(r"// expected: ?(.*)")
EXPECTED_ERROR_PATTERN = re.compile(r"// (Error.*)")
NON_TEST_PATTERN = re.compile(r"// nontest")
ERROR_LINE_PATTERN = re.compile(r"// \[((java|c) )?line (\d+)\] (Error.*)")
EXPECTED_RUNTIME_ERROR_PATTERN = re.compile(r"// expect runtime error: (.+)")

sample_basedir = "sample"

class Test:
    def __init__(self, expected_outputs=list(), expected_errors=list(), expected_exit_code=0, expected_runtime_error=None):
        self.expected_outputs = expected_outputs
        self.expected_errors = expected_errors
        self.expected_exit_code = expected_exit_code
        self.expected_runtime_error = expected_runtime_error
        self.failures = list()
    
    def parse(self, file_path: str, lines: typing.List[str]) -> bool:

        for lineno, line in enumerate(lines):
            match = NON_TEST_PATTERN.search(line)
            if match:
                self.failures.append(f"Skip nontest file {file_path}")
                return False

            match = EXPECTED_OUPUT_PATTERN.search(line)
            if match:
                self.expected_outputs.append({
                    "line": lineno + 1,
                    "output": match[1]
                })
                continue
            
            match = EXPECTED_ERROR_PATTERN.search(line)
            if match:
                self.expected_errors.append(f"[line {lineno + 1}] {match[1]}")
                self.expected_exit_code = 65
                continue

            match = ERROR_LINE_PATTERN.search(line)
            if match:
                self.expected_errors.append(f"[line {match[3]}] {match[4]}")
                self.expected_exit_code = 65
                continue

            match = EXPECTED_RUNTIME_ERROR_PATTERN.search(line)
            if match:
                self.expected_runtime_error = f"[line {lineno + 1}] {match[1]}"
                self.expected_exit_code = 70
        
        if len(self.expected_errors) > 0 and self.expected_runtime_error:
            self.failures.append("Can not expect both compile and runtime errors.")
            return False
        
        return True
    
    def print_failures(self):
        for failure in self.failures:
            print(failure)

    def run(self):
        pass


def run_test(test_path: str):
    lines = list()
    with open(test_path, "r", encoding="utf-8") as fp:
        lines.extend(fp.readlines())
    test = Test()

    if not test.parse(test_path, lines):
        test.print_failures()
        return
    
    test.run()

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
