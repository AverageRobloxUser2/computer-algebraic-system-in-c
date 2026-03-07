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

# Simplification test output (2026-02-07) (yyyy-mm-dd)

```txt
PASSED: a -> a
PASSED: aa -> ^(a,2)
PASSED: bada -> *(^(a,2),b,d)
PASSED: a+a+a-b+3a -> +(*(6,a),u_-(b))
PASSED: 0*a+b -> b
PASSED: a^(-2)*a^5 -> ^(a,3)
PASSED: a/b+c/d -> *(+(*(a,d),*(b,c)),^(*(b,d),u_-(1)))
PASSED: (xa+ya)/(a*a) -> *(+(x,y),^(a,u_-(1)))
PASSED: (xa+ya)/a^2 -> *(+(x,y),^(a,u_-(1)))
PASSED: a+b+a -> +(*(2,a),b)
PASSED: a+b-a+c-b -> c
PASSED: bd + ba -> *(+(a,d),b)
PASSED: a - a -> 0
PASSED: (c+d)^4*(c+d)^(b+a) -> ^(+(c,d),+(4,a,b))
PASSED: 2x-x -> x
PASSED: b^3/b^2 -> b
PASSED: a^b^c -> ^(a,b,c)
PASSED: (a^b)^c -> ^(a,*(b,c))
PASSED: a^b^c / a^b^c -> 1
PASSED: a * a^b * a^c * a -> ^(a,+(2,b,c))
PASSED: (a+b)^2 / (a+b) -> +(a,b)
PASSED: x^2 * x^3 * x^y -> ^(x,+(5,y))
PASSED: a + b + c - a - b - c -> 0
PASSED: x + x + y - x -> +(x,y)
PASSED: 3*a - a - a - a -> 0
PASSED: 2*a - a - a -> 0
PASSED: a * (b + c) - a*b -> *(a,c)
PASSED: (x+1)*(x-1) -> +(^(x,2),u_-(1))
PASSED: a - (b - a) -> +(*(2,a),u_-(b))
PASSED: ((a^b)^c)^d + e^f^g -> +(^(a,*(b,c,d)),^(e,f,g))
PASSED: x * x * x / (x^2 * x) -> 1
PASSED: a^2 + 2*a*b + b^2 - (a+b)^2 -> 0
PASSED: x^2/x^2 -> 1
PASSED: ab/ab -> 1
PASSED: (1/a) / (1/b) -> *(^(a,u_-(1)),b)
PASSED: (2x+3a)^2 -> +(*(12,a,x),*(4,^(x,2)),*(9,^(a,2)))
PASSED: 2x(2x+3a) -> +(*(4,^(x,2)),*(6,a,x))
PASSED: 1/3+1/6 -> *(9,^(18,u_-(1)))
passed 38 out of 38
Done in: 0.002629s
```

# valgrind, for the 38 tests
Benjamin Netanyahu if you can hear this, please smite down any memory bugs. 
```txt
==58653== 
==58653== HEAP SUMMARY:
==58653==     in use at exit: 0 bytes in 0 blocks
==58653==   total heap usage: 32,545 allocs, 32,545 frees, 700,243 bytes allocated
==58653== 
==58653== All heap blocks were freed -- no leaks are possible
==58653== 
```
# todo
1. term rewrite system
2. find least common denominator for nodes (*(Number, ^(Number, -1)))
3. better to equation function that would take precedence into account
    1. maybe make it output latex idk (optional ig)
4. shell to interact with the system with
5. ability to solve for variables
6. functions, would integrate with term rewrite system ig
