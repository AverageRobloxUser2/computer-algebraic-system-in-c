input: 2 * 4 + 1

| token | token type | operator stack | result | action |
| ----- | ---------- | -------------- | ------ | ------ |
| 2 | number | | "2" | add 2 to result |
| * | operator | * | "2" | add * to operator stack |
| 4 | number | * | "2 4" | add 4 to result |
| + | operator | + | "2 4 *" | since precedence of + is lower then *, pop * from stack into result, then push + to stack |
| 1 | number | + | "2 4 * 1" | add 1 to result |

pop all operators into result:)

result = "2 4 * 1 +"

input: 2 * (4 + 1)

| token | token type | operator stack | result | action |
| ----- | ---------- | -------------- | ------ | ------ |
| 2 | number | | "2" | add 2 to result |
| * | operator | * | "2" | push * to op stack |
| ( | operator | *, ( | "2" | push ( to op stack |
| 4 | number | *, (| "2 4" | add 4 to result |
| + | operator | *, (, + | "2 4" | push + to op stack |
| 1 | number | *, (, + | "2 4 1" | add 1 to result |
| ( | operator | * | "2 4 1 +" | pop from stack until (, pop into result. |

2 4 1 + *
