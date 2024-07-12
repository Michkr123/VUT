### IPP projekt
### autor: Michálek Kryštof
### login: xmicha94

import sys
import xml.etree.ElementTree as ET
import xml.dom.minidom

#################### HELP ####################
if len(sys.argv) == 2:
    if (sys.argv[1] == "--help"):
        print("napoveda")
        sys.exit(0)
    else:
        sys.exit(10)

if len(sys.argv) > 2:
    sys.exit(10)

#################### SCANNER ####################
def scanner(tokens):
    #searching header ".IPPcode24"
    while True:
        try:
            line = input()
            line = line.strip()
            flag = False
            if line:
                if(line[0] == "#"):
                    pass
                elif(line[0] == "."):
                    break
                else:
                    sys.exit(21)
        except EOFError:
            sys.exit(21)
    flag = header_check(line)
    if (not flag): #wrong header
        sys.exit(21)
    #reading the code itself
    instructions = ['MOVE', 'CREATEFRAME', 'PUSHFRAME', 'POPFRAME', 'DEFVAR', 'CALL', 'RETURN', 'PUSHS', 'POPS', 'ADD', 'SUB', 'MUL', 'IDIV', 'LT', 'GT', 'EQ', 'AND', 'OR', 'NOT', 'INT2CHAR', 'STRI2INT', 'READ', 'WRITE', 'CONCAT', 'STRLEN', 'GETCHAR', 'SETCHAR', 'TYPE', 'LABEL', 'JUMP', 'JUMPIFEQ', 'JUMPIFNEQ', 'EXIT', 'DPRINT', 'BREAK']
    spec_chars = ['_', '-', '$', '&', '%', '*', '!', '?']
    frames = ['LF', 'GF', 'TF']
    types = ['int', 'bool', 'string', 'nil', 'label', 'type', 'var']
    while flag:
        try:
            line = input()
            second_header_check = line.split("#")
            second_header_check = second_header_check[0].strip()
            if(second_header_check == ".IPPcode24"):
                sys.exit(23) 
            words = line.split("#")
            words = words[0].split(" ")
            more_instruction_on_one_line = False
            for word in words:
                #checking for multiple instructons on one line
                if word in instructions:
                    if(not more_instruction_on_one_line):
                        more_instruction_on_one_line = True
                    else:
                        sys.exit(23)
                #continue with scanner part in other function for better readability
                scanner_2(word, instructions, frames, types, spec_chars, tokens);                     
        
        except EOFError:
            expected_arguments = ""
            #calling parser function giving it one token at the time
            for token in tokens:
                expected_arguments = parser(token, expected_arguments, spec_chars)
            #there is no other token and there is expected more 
            if "var" in expected_arguments or "symb" in expected_arguments or "label" in expected_arguments or "type" in expected_arguments:
                sys.exit(23)
            return tokens

def scanner_2(word, instructions, frames, types, spec_chars, tokens):
    if(word != ''):
        token_length = len(tokens)
        word_upper = word.upper()
        if word_upper in instructions:
            if(len(tokens) > 0):
                is_prev_instr =  tokens[len(tokens) - 1].split("'")
                if(is_prev_instr[1] == "instruction"): #little tricky here with the labels named same as instruction, but it works 
                    prev_instr = is_prev_instr[0].upper()
                    if(prev_instr == "JUMP" or prev_instr == "CALL" or prev_instr == "JUMPIFEQ" or prev_instr == "JUMPIFNEQ" or prev_instr == "LABEL"):
                        tokens.append(word + "'label")
                    else:
                        tokens.append(word_upper + "'instruction")
                else:
                    tokens.append(word_upper + "'instruction")  
            else:
                tokens.append(word_upper + "'instruction")
        else:
            if "@" in word:
                split_word = word.split("@")
                if (len(split_word) == 2):
                    frame_test_upper = split_word[0].upper()
                    if frame_test_upper in frames:
                        if split_word[0] in frames:
                            tokens.append(word + "'var")
                        else:
                            sys.exit(23)
                    type_test_lower = split_word[0].lower()
                    if type_test_lower in types:
                        if split_word[0] in types:
                            tokens.append(word + "'symb")
                        else:
                            sys.exit(23)
                elif(len(split_word) > 2):
                    if(split_word[0] != "string"):
                        sys.exit(23)
                    else:
                        tokens.append(word + "'symb")
            else:
                if(len(tokens) == 0):
                    sys.exit(22)
                is_prev_instr = tokens[len(tokens)-1]
                is_prev_instr = is_prev_instr.split("'")
                if(is_prev_instr[1] == "instruction"):
                    tokens.append(word + "'label")
                else:
                    type_check_lower = word.lower()
                    if type_check_lower in types:
                        if word in types:
                            tokens.append(word + "'type")
                        else:
                            sys.exit(23)
            return tokens

def header_check(line):
    words = line.split("#")
    if(words[0].strip() == ".IPPcode24"):
        return True
    else:
        return False

#################### PARSER ####################
def parser(token, expected_argument, spec_chars):
    token_split = token.split("'")
    token_type = token_split[1]
    if (token_type == "instruction"):
        #all arguments from previous incstruction hasn't been used 
        if "var" in expected_argument or "symb" in expected_argument or "label" in expected_argument or "type" in expected_argument:
            sys.exit(23)
        expected_argument = instructions_switch(token_split[0])
        return expected_argument
    else:
        #got argument but not expecting one
        if(expected_argument == "x"):
            sys.exit(23)
        if not "," in expected_argument:
            expected_argument = expected_argument + ", "
        operand = expected_argument.split(",")
        if (operand[0] == token_type or operand[0] == "symb" and token_type == "var"):
            if(token_type == "symb"):
                data_type = token_split[0].split("@")
                type_check(data_type)
            elif(token_type == "var"):
                data_type = token_split[0].split("@")
                if(not data_type[1][0].isalpha() and not data_type[1][0] in spec_chars):
                    sys.exit(23)
            elif(token_type == "label"):
                if(not token_split[0][0].isalpha() and not token_split[0][0] in spec_chars):
                    sys.exit(23)
        else:
            sys.exit(23)
        #deleting used argument and joining remaining together
        if len(operand) > 1:
            need_put_together = ",".join(operand[1:])
            return need_put_together
        else:
            return ""
        
#checking if the type matches the value type
def type_check(data_type):
    hexa_letters = ["a", "b", "c", "d", "e", "f", "A", "B", "C", "D", "E", "F"]
    octal_numbers = ["0", "1", "2", "3", "4", "5" , "6", "7"]

    if(data_type[1] == ""):
        if(data_type[0] == "int"):
            sys.exit(23)
        elif(data_type[0] == "string"):
            pass
    elif(data_type[0] == "int"):
        if(data_type[1][0] == "-" or data_type[1][0] == "+"): #negative or explicitly positive
            if(len(data_type[1]) > 3 and data_type[1][1] == "0" and data_type[1][2] == "o"): #octal
                for char in data_type[1][3:]:
                    if(not char in octal_numbers):
                        sys.exit(23)
            elif(data_type[1][1:].isdecimal()): #decimal
                pass
            elif(data_type[1][1:3] == "0x"): #hexa
                for char in data_type[3:]:
                    if(not char.isdigit() and not char in hexa_letters):
                        sys.exit(23)
            else:
                sys.exit(23) 
        else: #positive
            if(len(data_type[1]) > 2 and data_type[1][0] == "0" and data_type[1][1] == "o"): #octal
                for char in data_type[1][2:]:
                    if(not char in octal_numbers):
                        sys.exit(23)
            elif(data_type[1].isdecimal()): #decimal
                pass
            elif(data_type[1][0:2] == "0x"): #hexa
                for char in data_type[2:]:
                    if(not char.isdigit() and not char in hexa_letters):
                        sys.exit(23)
            else:
                sys.exit(23)
    elif(data_type[0] == "string"):
        #again tricky things with special characters - \ need to be folowed by 3 numbers
        for i in range(len(data_type[1]) - 3):
            substr = data_type[1][i:i + 4]
            if(substr[0] == "\\"):
                if(not substr[1:].isdecimal()):
                    sys.exit(23)
        if "\\" in data_type[1][len(data_type[1])-3:len(data_type[1])]:
            sys.exit(23)
    elif(data_type[0] == "bool"):
        if(data_type[1] != "true" and data_type[1] != "false"):
            sys.exit(23)
    elif(data_type[0] == "nil"):
        if (data_type[1] != "nil"):
            sys.exit(23)

#all instructions and expected arguments after each instruction
def instructions_switch(token):
    instr_dictionary = {
        "MOVE": "var,symb",
        "CREATEFRAME": "x",
        "PUSHFRAME": "x",
        "POPFRAME": "x",
        "DEFVAR": "var",
        "CALL": "label",
        "RETURN": "x",
        "PUSHS": "symb",
        "POPS": "var",
        "ADD": "var,symb,symb",
        "SUB": "var,symb,symb",
        "MUL": "var,symb,symb",
        "IDIV": "var,symb,symb",
        "LT": "var,symb,symb",
        "GT": "var,symb,symb",
        "EQ": "var,symb,symb",
        "AND": "var,symb,symb",
        "OR": "var,symb,symb",
        "NOT": "var,symb",
        "INT2CHAR": "var,symb",
        "STRI2INT": "var,symb,symb",
        "READ": "var,type",
        "WRITE": "symb",
        "CONCAT": "var,symb,symb",
        "STRLEN": "var,symb",
        "GETCHAR": "var,symb,symb",
        "SETCHAR": "var,symb,symb",
        "TYPE": "var,symb",
        "LABEL": "label",
        "JUMP": "label",
        "JUMPIFEQ": "label,symb,symb",
        "JUMPIFNEQ": "label,symb,symb",
        "EXIT": "symb",
        "DPRINT": "symb",
        "BREAK": "x", 
    }
    return instr_dictionary.get(token, "none")

#################### CODE GENERATOR ####################
def code_gen(tokens):
    root = ET.Element("program", attrib={"language": "IPPcode24"})
    arg_cnt = 1
    instr_cnt = 1
    for token in tokens:
        token_split = token.split("'")
        token_type = token_split[1]
        if(token_type == "instruction"):
            instruction = ET.SubElement(root, "instruction", attrib={"order": str(instr_cnt), "opcode": token_split[0]})
            arg_cnt = 1
            instr_cnt = instr_cnt + 1
        else:
            if(token_type == "var" or token_type == "label" or token_type == "type"):
                arg = ET.SubElement(instruction, "arg" + str(arg_cnt), attrib={"type": token_type})
                arg.text = token_split[0]
                arg_cnt = arg_cnt + 1
            else:
                data_type = token_split[0].split("@")
                arg = ET.SubElement(instruction, "arg" + str(arg_cnt), attrib={"type": data_type[0]})
                if(data_type[0] == "string"):
                    pass
                    if len(data_type) > 2:
                        arg.text = data_type[1] + "@"
                        arg.text = arg.text + ",".join(data_type[2:])
                    else:
                        arg.text = data_type[1]
                else:
                    arg.text = data_type[1]
                arg_cnt = arg_cnt + 1
    xml_string = ET.tostring(root, encoding="UTF-8")
    dom = xml.dom.minidom.parseString(xml_string)
    pretty_xml_string = dom.toprettyxml(indent="    ", encoding="UTF-8").decode("UTF-8")
    print(pretty_xml_string)
        
#################### MAIN ####################
tokens = []
scanner(tokens)
code_gen(tokens)