# to add
[ ] error messages, for unknown variables, invalid syntax or arguments. 
[ ] ast based simplify for expressions (impossible)
    - convert to ast
    - flatten simplify based on operators like join all add nodes together
        3 + 4 + 5
        for example:

            +
          / | \
          3 4 5

        instead of:
            +
           / \
           3  +
             / \
             4  5
    - evaluate differnt types individualy. count like 2x + 3x to 5x and 2 + 3 to 5
        

# added
[X] support for implicit multiplication

