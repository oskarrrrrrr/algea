import random

MAX_COORD = 10**7
MIN_COORD = -MAX_COORD

def gen_test(name, n):
    with open(name, "w", encoding="utf-8") as f:
        f.write(f"{n}\n")
        for _ in range(n):
            x = random.randint(MIN_COORD, MAX_COORD)
            y = random.randint(MIN_COORD, MAX_COORD)
            f.write(f"{x} {y}\n")

def main():
    gen_test("tests/t8.txt", 100)
    gen_test("tests/t9.txt", 500)
    gen_test("tests/t10.txt", 1000)
    gen_test("tests/t11.txt", 5000)
    gen_test("tests/t12.txt", 10000)
    gen_test("tests/t13.txt", 100000)
    gen_test("tests/t14.txt", 1000000)
    gen_test("tests/t15.txt", 2000000)
    gen_test("tests/t16.txt", 3500000)

main()
