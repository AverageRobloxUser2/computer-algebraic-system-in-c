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
passed 31 out of 31
Done in: 0.003032s
```
# to fix
