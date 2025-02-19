import sys
import re

#################### HELP ####################
if len(sys.argv) == 2:
    if sys.argv[1] == "--help":
        print("Usage: python3 parse.py < input_file")  # Help message
        sys.exit(0)
    else:
        sys.exit(10)

#################### SCANNER ####################

# Regular expressions for tokenizing based on the provided grammar
TOKENIZERS = {
    "whitespace": r"[ \t\r\n]+",  # space, tab, newline
    "comment": r'\"[^\"]*\"',  # block comments, starting and ending with a double quote
    "identifier": r"[a-z][a-zA-Z0-9_]*",  # identifiers (variables, methods)
    "Cid": r"[A-Z][a-zA-Z0-9_]*",  # class identifiers
    "keyword": r"\b(class)\b",  # keywords
    "assignment": r":=",  # assignment operator
    "block_start": r"\[",  # block start
    "block_end": r"\]",  # block end
    "operator": r"(\+|-|\*|\/|\%|=)",  # operators
    "string": r"'[^']*'",  # strings enclosed in single quotes
    "number": r"\b\d+\b",  # numbers
    "punctuation": r"[.,;!?(){}]",  # punctuation characters (e.g., dots, commas, semicolons)
    "colon": r":",  # colon used in various places
}

# Combine all regex patterns into one for tokenization
combined_regex = "|".join(f"(?P<{key}>{pattern})" for key, pattern in TOKENIZERS.items())

def tokenize(source_code):
    tokens = []
    for match in re.finditer(combined_regex, source_code):
        kind = match.lastgroup
        value = match.group()
        if kind != "whitespace" and kind != "comment":  # Ignore whitespace and comments
            tokens.append((kind, value))
    return tokens

#################### PARSE ####################

def parse(tokens):
    print("ok")

#################### MAIN ####################

# Read input from stdin
input_text = sys.stdin.read()

# Tokenize the input text
tokens = tokenize(input_text)

parse(tokens)

# Print the tokens
for token in tokens:
    print(token)
