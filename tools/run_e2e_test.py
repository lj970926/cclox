import argparse
import os
import subprocess

sample_basedir = "sample"

def run_test(test_path):
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
