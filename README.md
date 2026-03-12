# how to run?
```
git clone https://github.com/AverageRobloxUser2/computer-algebraic-system-in-c/
cd computer-algebraic-system-in-c
./setup.sh
```

# binaries explained
## rpn
allows custom user input, legacy name since this project used to be a rpn parser.
now its more complex. see output of simplification tests.
## all test stuff just tests stuff :confetti:
who could have guessed that

# Simplification test output (2026-02-12) (yyyy-mm-dd)
```txt
PASSED: a -> a
PASSED: --(b+d) -> b+d
PASSED: 0-(b-d) -> d+(-b)
PASSED: -(b-d) -> d+(-b)
PASSED: aa -> a^2
PASSED: bada -> b*d*a^2
PASSED: a+a+a-b+3a -> 6*a+(-b)
PASSED: 0*a+b -> b
PASSED: a^(-2)*a^5 -> a^3
PASSED: a/b+c/d -> (a*d+b*c)*(b*d)^(-1)
PASSED: (xa+ya)/(a*a) -> (x+y)*a^(-1)
PASSED: (xa+ya)/a^2 -> (x+y)*a^(-1)
PASSED: a+b+a -> b+2*a
PASSED: a+b-a+c-b -> c
PASSED: bd + ba -> b*(a+d)
PASSED: a - a -> 0
PASSED: (c+d)^4*(c+d)^(b+a) -> (c+d)^(4+a+b)
PASSED: 2x-x -> x
PASSED: b^3/b^2 -> b
PASSED: a^b^c -> a^b^c
PASSED: (a^b)^c -> a^(b*c)
PASSED: a^b^c / a^b^c -> 1
PASSED: a * a^b * a^c * a -> a^(2+b+c)
PASSED: (a+b)^2 / (a+b) -> a+b
PASSED: x^2 * x^3 * x^y -> x^(5+y)
PASSED: a + b + c - a - b - c -> 0
PASSED: x + x + y - x -> x+y
PASSED: 3*a - a - a - a -> 0
PASSED: 2*a - a - a -> 0
PASSED: a * (b + c) - a*b -> a*c
PASSED: (x+1)*(x-1) -> x^2+(-1)
PASSED: a - (b - a) -> 2*a+(-b)
PASSED: ((a^b)^c)^d + e^f^g -> a^(b*c*d)+e^f^g
PASSED: x * x * x / (x^2 * x) -> 1
PASSED: a^2 + 2*a*b + b^2 - (a+b)^2 -> 0
PASSED: x^2/x^2 -> 1
PASSED: ab/ab -> 1
PASSED: (1/a) / (1/b) -> b*a^(-1)
PASSED: (2x+3a)^2 -> 4*x^2+12*a*x+9*a^2
PASSED: 2x(2x+3a) -> 4*x^2+6*a*x
PASSED: 1/3+1/6 -> 2^(-1)
PASSED: 1/2+1/3 -> 5*6^(-1)
Passed 42 out of 42
0.002606s
```

# valgrind, for the 38 tests
Benjamin Netanyahu if you can hear this, please smite down any memory bugs. 
```txt
==38720== 
==38720== HEAP SUMMARY:
==38720==     in use at exit: 0 bytes in 0 blocks
==38720==   total heap usage: 53,317 allocs, 53,317 frees, 980,297 bytes allocated
==38720== 
==38720== All heap blocks were freed -- no leaks are possible
==38720== 
==38720== For lists of detected and suppressed errors, rerun with: -s
==38720== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)
```

# hyperfine benchmark for 1k runs
```
$ hyperfine --runs 1000 -N --warmup 100 "./test_simplify"
Benchmark 1: ./test_simplify
  Time (mean ± σ):       5.0 ms ±   2.2 ms    [User: 4.0 ms, System: 0.9 ms]
  Range (min … max):     2.7 ms …   9.1 ms    1000 runs
```

# todo
- term rewrite system
- maybe make it output latex idk (optional ig)
- shell to interact with the system with result of previous call could be 
    stored in variables _p1 for the most recent, and _p2 for the second most recent etc.
- ability to solve for variables
- functions, would integrate with term rewrite system ig
- solve_for function solve_for(left, right, variable)
    example usage solve_for(mv^2/2, E, v) would result in (2E/m)^(1/2)
