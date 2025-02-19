import sys
from lark import Lark

grammar = r'''
    start: program

    program: class_def program
           | block_stat program
           |  // empty

    class_def: "class" CNAME ":" CNAME "{" method "}"
    
    method: selector block_stat method
          | // empty

    selector: ID
            | ID ":" selector_tail

    selector_tail: ID ":" selector_tail
                | // empty
    
    block: "[" block_par "|" block_stat "]"

    block_par: ":" ID block_par
             | // empty
    
    block_stat: ID ":=" expr "."
              | selector ":=" expr "."
              | selector ":=" ID "."
              | // empty
    
    expr: expr_base

    expr_base: INT
             | STRING
             | ID
             | CNAME
             | block
             | "(" expr ")"

    // Token imports
    %import common.INT
    %import common.ESCAPED_STRING -> STRING
    %import common.CNAME  // For class names (CID)
    %import common.WORD   // For identifiers (ID)

    // Explicitly define ID as WORD for clarity
    ID: WORD

    %ignore " "
    %ignore "\t"
    %ignore "\n"
'''

# Create the parser
parser = Lark(grammar, start='start')

# Parse the input
parse_tree = parser.parse(sys.stdin)

# Print the parse tree  
print(parse_tree.pretty())
