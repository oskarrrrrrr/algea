import subprocess
import os
import time
import math

def find_tests(tests_dir):
    test_files = os.listdir(tests_dir)
    tests = []
    for f in test_files:
        if f.startswith("t"):
            test_num = int(f.split(".")[0][1:])
            e_path = os.path.join(tests_dir, f"e{test_num}.txt")
            if not os.path.exists(e_path):
                e_path = None
            tests.append(
                (os.path.join(tests_dir, f"t{test_num}.txt"), e_path)
            )
    tests.sort(key=lambda t: int(os.path.basename(t[0]).split(".")[0][1:]))
    return tests

def run_program_with_file_input(program_path, file_path, timeout=None):
    process = subprocess.Popen(
        [program_path],
        stdin=subprocess.PIPE,
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE
    )

    start_t = time.time_ns()

    with open(file_path, "rb") as file:
        assert process.stdin is not None
        for line in file:
            process.stdin.write(line)

    stdout, stderr = process.communicate(timeout=timeout)

    end_t = time.time_ns()
    t = (end_t - start_t) // 10**6

    stdout = stdout.decode()
    stderr = stderr.decode()

    return stdout, stderr, t

def parse_ans(ans):
    points = []
    for line in ans.splitlines():
        points.append(tuple(map(int, line.split(" "))))
    return points

def dist(p1, p2):
    return (p1[0]-p2[0])**2 + (p1[1]-p2[1])**2

def calc_circ(points):
    return math.sqrt(
        dist(points[0], points[1])
        + dist(points[1], points[2])
        + dist(points[2], points[0])
    )

def run_test(program, test_file, exp_file):
    timeout = 5
    try:
        stdout, stderr, t = run_program_with_file_input(
            program, test_file, timeout
        )
    except subprocess.TimeoutExpired:
        print(f"{test_file} TIMEOUT (>{timeout}s)")
        return

    if stderr:
        print(f"ERROR RUNNING TEST {test_file}")
        print("STDOUT:", stdout)
        print("STDERR:", stderr)
    else:
        ans = calc_circ(parse_ans(stdout))

        if exp_file is None:
            print(f"{test_file} n/a ({t}ms) (circ. {ans})")
        else:
            exp_str = None
            with open(exp_file, encoding="utf-8") as f:
                exp_str = f.read()
            assert exp_str is not None
            exp_points = parse_ans(exp_str)
            exp_circ = calc_circ(exp_points)

            if ans == exp_circ:
                print(f"{test_file} ok ({t}ms)")
            else:
                print(f"{test_file} FAILED\n")
                print(f"expected circ: {exp_circ}, got: {ans}\n")
                print("STDOUT:\n", stdout)
                print("Expected points:")
                print(exp_str)
                exit(1)

def run_tests():
    tests = find_tests("tests")
    print("Smart")
    for t, e in tests:
        run_test("./triangles", t, e)
    print()
    print("Brute")
    for t, e in tests:
        run_test("./brute", t, e)

run_tests()
