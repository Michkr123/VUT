<?php

namespace IPP\Student;

use DOMElement;
use IPP\Core\AbstractInterpreter;
use IPP\Core\Exception\NotImplementedException;
use IPP\Core\FileSourceReader;
use IPP\Core\FileInputReader;
use IPP\Core\ReturnCode;
use phpDocumentor\Reflection\DocBlock\Tags\Return_;

class Interpreter extends AbstractInterpreter
{
    public function execute(): int
    {
        $dom = $this->source->getDOMDocument();
        $rootElement = $dom->documentElement;
        //$val = $this->input->readString();

        //checks if the root tag and language are correct
        if ($rootElement->tagName !== 'program' || !$rootElement->hasAttribute('language') || strtoupper($rootElement->getAttribute('language')) !== 'IPPCODE24') 
        {
            exit(ReturnCode::INVALID_SOURCE_STRUCTURE);
        }

        //checks if the tags in the xml document are correct
        foreach ($rootElement->childNodes as $node) {
            if ($node->nodeType !== XML_ELEMENT_NODE) {
                continue;
            }
            
            if ($node->nodeName !== 'instruction' && $node->nodeName !== 'arg') {
                exit(ReturnCode::INVALID_SOURCE_STRUCTURE);
            }
        }
        
        //inicialization of later used arrays and objects of classes
        //I hope the names are self-explanatory (i put comments where its not)
        //explanation of individual classes is inside the classes 
        $instructions = $dom->getElementsByTagName('instruction');
        $instrs = array();
        $orders = array();
        $frames = new Frames();
        //Pushing the global frame to the frames class
        $frames->CREATEFRAME();
        $frames->PUSHFRAME();
        $calls = new Calls();
        $labels = new Labels();
        $stack = new Stack();

        //each instruction is own object of class Instruction having array of arguments, which are object of the argument class
        foreach ($instructions as $instruction)
        {
            $order = $instruction->getAttribute('order');
            //checks if order is not missing and have a correct value
            if($order == null || intval($order) < 1)
            {
                exit(ReturnCode::INVALID_SOURCE_STRUCTURE);
            }
            $opcode = strtoupper($instruction->getAttribute('opcode'));
            //checks if opcode is not missing
            if($opcode == null)
            {
                exit(ReturnCode::INVALID_SOURCE_STRUCTURE);
            }
            $instrs[$order] = new Instruction($opcode);
            //checks if order is unique
            if(in_array($order, $orders))
            {
                exit(ReturnCode::INVALID_SOURCE_STRUCTURE);
            }
            $orders[] = $order;
            foreach ($instruction->childNodes as $node) {
                if ($node->nodeType === XML_ELEMENT_NODE && strpos($node->nodeName, 'arg') === 0) {
                    //getting arguments for the argument class
                    $argNumber = substr($node->nodeName, 3);
                    $value = $node->nodeValue;
                    if($value != null)
                    {
                        $value = trim($value);
                    }
                    if($node instanceof DOMElement)
                    {
                        $type = $node->hasAttribute('type') ? $node->getAttribute('type') : null;
                        if($type == null)
                        {
                            exit(ReturnCode::INVALID_SOURCE_STRUCTURE);
                        }
                        //saving each argument in the array of arguments of each instruction
                        $instrs[$order]->addArg($type, $value, intval($argNumber));
                    }
                }
            }
        }

        //sorting orders in case they are in wrong order
        sort($orders);

        //this is little extra, but didn't find a way to do it in one go
        //saving positions of labels and their names
        for($i = 0; $i < count($orders); $i++)
        {
            if($instrs[$orders[$i]]->getOpcode() == "LABEL")
            {
                if($instrs[$orders[$i]]->getArgN(0)->getType() == "label")
                {
                    $name = $instrs[$orders[$i]]->getArgN(0)->getName();
                    $labels->LABEL($name, $i);
                }
            }
        }
    
        //going through all the instructions
        for($i = 0; $i < count($orders); $i++)
        {
            //based on opcode select case
            //most of the instructions are implemented in Instruction class
            //i will describe each inside the function
            switch($instrs[$orders[$i]]->getOpcode())
            {   
                case 'MOVE':
                    $instrs[$orders[$i]]->MOVE($frames);
                    break;
                case 'CREATEFRAME':
                    $frames->CREATEFRAME();
                    break;
                case 'PUSHFRAME':
                    $frames->PUSHFRAME();
                    break;
                case 'POPFRAME':
                    $frames->POPFRAME();
                    break;
                case 'DEFVAR':
                    if($instrs[$orders[$i]]->getArgN(0)->getType() != "var")
                    {
                        exit(ReturnCode::INVALID_SOURCE_STRUCTURE);
                    }
                    //checks if the variable name is correct
                    $variable = $instrs[$orders[$i]]->getArgN(0)->getName();
                    $parts = explode("@", $variable);
                    $frame = $parts[0];
                    if(!isset($parts[1]))
                    {
                        exit(ReturnCode::VARIABLE_ACCESS_ERROR);
                    }
                    $name = $parts[1];
                    if($instrs[$orders[$i]]->getArgCount() > 1)
                    {
                        exit(ReturnCode::INVALID_SOURCE_STRUCTURE);
                    }
                    //add it to the right frame
                    if($frame == "LF")
                    {
                        $topFrame = $frames->getTopFrame();
                        if($topFrame != null)
                        {
                            $topFrame->addVariable($name);
                        }
                        else
                        {
                            exit(ReturnCode::FRAME_ACCESS_ERROR);
                        }
                    }
                    elseif($frame == "GF")
                    {
                        $globalFrame =  $frames->getFrameN(0);
                        if($globalFrame != null)
                        {
                            $globalFrame->addVariable($name);
                        }
                        else
                        {
                            exit(ReturnCode::FRAME_ACCESS_ERROR);
                        }
                    }
                    elseif($frame == "TF")
                    {
                        if($frames->issetTF())
                        {
                            $temporaryFrame = $frames->getTF();
                            if($temporaryFrame != null)
                            {
                                $temporaryFrame->addVariable($name);
                            }
                            else
                            {
                                exit(ReturnCode::FRAME_ACCESS_ERROR);
                            }
                        }
                        else
                        {
                            exit(ReturnCode::FRAME_ACCESS_ERROR);
                        }
                    }
                    else
                    {
                        exit(ReturnCode::INVALID_SOURCE_STRUCTURE);
                    }
                    break;
                case 'CALL':
                    $calls->CALL($i);
                    $i = $labels->getLabelLine($instrs[$orders[$i]]->getArgN(0)->getName());
                    break;
                case 'RETURN':
                    $i = $calls->RETURN();
                    break;
                case 'PUSHS':
                    $stack->PUSHS($frames, $instrs[$orders[$i]]);
                    break;
                case 'POPS':
                    $stack->POPS($frames, $instrs[$orders[$i]]);
                    break;
                case 'ADD':
                    $instrs[$orders[$i]]->ADD($frames);
                    break;
                case 'SUB':
                    $instrs[$orders[$i]]->SUB($frames);
                    break;
                case 'MUL':
                    $instrs[$orders[$i]]->MUL($frames);
                    break;
                case 'IDIV':
                    $instrs[$orders[$i]]->IDIV($frames);
                    break;
                case 'LT':
                    $instrs[$orders[$i]]->LT($frames);
                    break;
                case 'GT':
                    $instrs[$orders[$i]]->GT($frames);
                    break;
                case 'EQ':
                    $instrs[$orders[$i]]->EQ($frames);
                    break;
                case 'AND':
                    $instrs[$orders[$i]]->AND($frames);
                    break;
                case 'OR':
                    $instrs[$orders[$i]]->OR($frames);
                    break;
                case 'NOT':
                    $instrs[$orders[$i]]->NOT($frames);
                    break;
                case 'INT2CHAR':
                    $instrs[$orders[$i]]->INT2CHAR($frames);
                    break;
                case 'STRI2INT':
                    $instrs[$orders[$i]]->STRI2INT($frames);
                    break;
                case 'READ':
                    $variable = $instrs[$orders[$i]]->READ($frames);
                    $type = $instrs[$orders[$i]]->getArgN(1)->getName();
                    if($type == null)
                    {
                        exit(ReturnCode::VARIABLE_ACCESS_ERROR);
                    }
                    if($instrs[$orders[$i]]->getArgCount() > 2)
                    {
                        exit(ReturnCode::INVALID_SOURCE_STRUCTURE);
                    }

                    if($type == "string")
                    {
                        $val = $this->input->readString();
                    }
                    elseif($type == "int")
                    {
                        $val = $this->input->readInt();
                    }
                    elseif($type == "bool")
                    {
                        $val = $this->input->readBool();
                    }
                    else
                    {
                        $val = "nil";
                        $type = "nil";
                    }

                    if($val == "")
                    {
                        $val = "nil";
                        $type = "nil";
                    }

                    $variable->setValue($val);
                    $variable->setType($type);
                    break;
                case 'WRITE':
                    //special characters that will be replaced
                    $specialChars = [
                        "\\032" => " ",
                        "\\092" => "\\",
                        "\\035" => "#",
                        "\\010" => "\n",
                        "\\009" => "    "
                    ];

                    $variable = $instrs[$orders[$i]]->WRITE($frames);
                    if($variable instanceof Variable)
                    {
                        $output = $variable->getValue();
                        //variable defined but have no value nor type
                        if($output == null && $variable->getType() == null)
                        {
                            exit(ReturnCode::VALUE_ERROR);
                        }
                    }
                    else
                    {
                        //string can be empty
                        $output = $variable;
                    }
                    //replacing if not empty
                    if($output != null)
                    {
                        $output = str_replace(array_keys($specialChars), $specialChars, $output);
                    }
                    if(is_string($output) || is_bool($output) || is_int($output) || $output == "")
                    {
                        if($instrs[$orders[$i]]->getArgN(0)->getType() == "bool" || $instrs[$orders[$i]]->getArgN(0)->getType() == "var" && $variable->getType() == "bool")
                        {
                            if($output == "false")
                            {
                                $output = 0;
                            }
                            $this->stdout->writeBool((bool) $output);
                        }
                        elseif($instrs[$orders[$i]]->getArgN(0)->getType() == "string" || $instrs[$orders[$i]]->getArgN(0)->getType() == "var" && $variable->getType() == "string")
                        {
                            $this->stdout->writeString((string) $output);
                        }
                        elseif($instrs[$orders[$i]]->getArgN(0)->getType() == "int" || $instrs[$orders[$i]]->getArgN(0)->getType() == "var" && $variable->getType() == "int")
                        {
                            $this->stdout->writeInt(intval($output));
                        }
                        else {}
                    }
                    else
                    {
                        exit(ReturnCode::STRING_OPERATION_ERROR);
                    }
                    break;
                case 'CONCAT':
                    $instrs[$orders[$i]]->CONCAT($frames);
                    break;
                case 'STRLEN':
                    $instrs[$orders[$i]]->STRLEN($frames);
                    break;
                case 'GETCHAR':
                    $instrs[$orders[$i]]->GETCHAR($frames);
                    break;
                case 'SETCHAR':
                    $instrs[$orders[$i]]->SETCHAR($frames);
                    break;
                case 'TYPE':
                    $instrs[$orders[$i]]->TYPE($frames);
                    break;
                case 'LABEL':
                    //its done, but i had to implement it before 
                    //i started to go trough all the instructions
                    break;
                case 'JUMP':
                    $i = $labels->getLabelLine($instrs[$orders[$i]]->getArgN(0)->getName());
                    break;
                case 'JUMPIFEQ':
                    $line = $labels->JUMPIFEQ($frames, $instrs[$orders[$i]]);
                    if($line != null) 
                    {
                        $i = $line;
                    }
                    break;
                case 'JUMPIFNEQ':
                    $line = $labels->JUMPIFNEQ($frames, $instrs[$orders[$i]]);
                    if($line != null) 
                    {
                        $i = $line;
                    }
                    break;
                case 'EXIT':
                    $instrs[$orders[$i]]->EXIT($frames);
                    break;
                case 'DPRINT':
                    //$instrs[$orders[$i]]->DPRINT($frames);
                    break;
                case 'BREAK':
                    //$instrs[$orders[$i]]->BREAK($frames);
                    break;
                default:
                    exit(ReturnCode::INVALID_SOURCE_STRUCTURE);
            }
        }
        return 0;
    }
}