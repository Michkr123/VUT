### IPP projekt
### autor: Michálek Kryštof
### login: xmicha94

import sys
from lark import lark

#################### HELP ####################
if len(sys.argv) == 2:
    if sys.argv[1] == "--help":
        print("Usage: python3 parse.py < input_file")  # TODO
        sys.exit(0)
    else:
        sys.exit(10)

#################### SCANNER ####################

def tokenize():
    """Tokenizes a line, removing comments enclosed in double quotes."""
    
    tokens_filtered_all = []
    inside_comment = False  # Track if we are inside a comment

    
    for line in sys.stdin:

        tokens = line.split()  # Split by spaces

        filtered_tokens = []
        for token in tokens:
            if '"' in token:
                inside_comment = not inside_comment  # Toggle comment mode
                continue  # Skip the `"`

            if not inside_comment:
                filtered_tokens.append(token)

        tokens_filtered_all += filtered_tokens
    
    return tokens_filtered_all

#################### MAIN ####################

tokens = tokenize()

for token in tokens:
    print(f"Token: {token}")  # Print tokens outside comments

