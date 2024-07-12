<?php

namespace IPP\Student;

use IPP\Core\AbstractInterpreter;
use IPP\Core\ReturnCode;
use phpDocumentor\Reflection\Types\AbstractList;

class Instruction
{
    private string $opcode;
    /** @var Argument[] */
    private array $args;
    private int $argCount;

    public function __construct(string $opcode)
    {
        $this->opcode = $opcode;
        $this->args = [];
        $this->argCount = 0;
    }

    //some instruction (mostly get set) are self-explanatory
    //so i wont explain them, i will of course explain the more complex ones
    public function getOpcode() : string
    {
        return $this->opcode;
    }

    //add new argument to the argument array of this instruction
    public function addArg(?string $argType,?string $argValue,int $argNumber) : void
    {
        if(isset($this->args[$argNumber - 1]))
        {
            exit(ReturnCode::INVALID_SOURCE_STRUCTURE);
        }
        $this->args[$argNumber - 1] = new Argument($argType, $argValue);
        $this->argCount++;
    }

    //get Nth argument of this instruction
    public function getArgN(int $N) : Argument 
    {
        if(!isset($this->args[$N]))
        {
            exit(ReturnCode::INVALID_SOURCE_STRUCTURE);
        }
        return $this->args[$N];
    }

    public function getArgCount() : int
    {
        return $this->argCount;
    }

    public function checkVariable(Frames $frames) : ?Variable
    {
        $variable = $this->getArgN(0)->getName();
        if($this->getArgN(0)->getType() == "var")
        {
            $parts = explode("@", $variable);
            $frame = $parts[0];
            if(!isset($parts[1]))
            {
                exit(ReturnCode::VARIABLE_ACCESS_ERROR);
            }
            $name = $parts[1];
            //look for variable in frames
            $variable =  $frames->isInFrames($name, $frame);
            return $variable;
        }
        else
        {
            exit(ReturnCode::VARIABLE_ACCESS_ERROR);
        }
    }

    public function checkSymb(Frames $frames, int $N) : mixed
    {
        $variable = $this->getArgN($N)->getName();
        if($this->getArgN($N)->getType() == "var")
        {
            $parts = explode("@", $variable);
            $frame = $parts[0];
            if(!isset($parts[1]))
            {
                exit(ReturnCode::VARIABLE_ACCESS_ERROR);
            }
            $name = $parts[1];
            //look for variable in frames
            $variable =  $frames->isInFrames($name, $frame);
            return $variable;
        }
        return $variable;
    }

    /************************* MOVE *************************/
    public function MOVE(Frames $frames) : void
    {
        //this part is nearly always the same, but im to lazy to make it a function
        //will explain it once, in other function leave it comment-less
        //TODO function of this :)
        //checks if the name is variable and if its correct
        $variable = $this->checkVariable($frames);
        $variable2 = $this->checkSymb($frames, 1);
        if($variable == null)
        {
            exit(ReturnCode::VARIABLE_ACCESS_ERROR);
        }
        //checks for incorrect number of arguments 
        if($this->getArgCount() > 2)
        {
            exit(ReturnCode::INVALID_SOURCE_STRUCTURE);
        }
        //move the value and type
        if($variable2 instanceof Variable)
        {
            $variable->setValue($variable2->getValue());
            $variable->setType($variable2->getType());
        }
        else
        {
            $variable->setValue($variable2);
            $variable->setType($this->getArgN(1)->getType());
        }
    }

    /************************* ADD *************************/
    public function ADD(Frames $frames) : void
    {
        $variable = $this->checkVariable($frames);
        $variable2 = $this->checkSymb($frames, 1);
        $variable3 = $this->checkSymb($frames, 2);
        if($variable == null || $variable2 == null || $variable3 == null)
        {
            exit(ReturnCode::VARIABLE_ACCESS_ERROR);
        }
        if($this->getArgCount() > 3)
        {
            exit(ReturnCode::INVALID_SOURCE_STRUCTURE);
        }
        
        //checks if both the operands are type of int
        if(($variable2 instanceof Variable ? $variable2->getType() : $this->getArgN(1)->getType()) != "int" || ($variable3 instanceof Variable ? $variable3->getType() : $this->getArgN(2)->getType()) != "int")
        {
            exit(ReturnCode::OPERAND_TYPE_ERROR);
        }
        //getting the right values
        $int1 = ($variable2 instanceof Variable ? $variable2->getValue() : $variable2);
        $int2 = ($variable3 instanceof Variable ? $variable3->getValue() : $variable3);
        //checking if there is only positive and negative int values
        if(!ctype_digit(strval($int1)))
        {
            if($int1[0] != "-" || !ctype_digit(substr($int1, 1)))
            {
                exit(ReturnCode::INVALID_SOURCE_STRUCTURE);
            }
        }
        if(!ctype_digit(strval($int2)))
        {
            if($int2[0] != "-" || !ctype_digit(substr($int2, 1)))
            {
                exit(ReturnCode::INVALID_SOURCE_STRUCTURE);
            } 
        }
        $result = (string ) ($int1 + $int2);
        $variable->setValue($result);
        $variable->setType("int");
    }

    /************************* SUB *************************/
    public function SUB(Frames $frames) : void
    {
        //same as ADD
        $variable = $this->checkVariable($frames);
        $variable2 = $this->checkSymb($frames, 1);
        $variable3 = $this->checkSymb($frames, 2);
        if($variable == null || $variable2 == null || $variable3 == null)
        {
            exit(ReturnCode::VARIABLE_ACCESS_ERROR);
        }
        if($this->getArgCount() > 3)
        {
            exit(ReturnCode::INVALID_SOURCE_STRUCTURE);
        }

        if(($variable2 instanceof Variable ? $variable2->getType() : $this->getArgN(1)->getType()) != "int" || ($variable3 instanceof Variable ? $variable3->getType() : $this->getArgN(2)->getType()) != "int")
        {
            exit(ReturnCode::OPERAND_TYPE_ERROR);
        }
        $int1 = ($variable2 instanceof Variable ? $variable2->getValue() : $variable2);
        $int2 = ($variable3 instanceof Variable ? $variable3->getValue() : $variable3);
        if(!ctype_digit(strval($int1)))
        {
            if($int1[0] != "-" || !ctype_digit(substr($int1, 1)))
            {
                exit(ReturnCode::INVALID_SOURCE_STRUCTURE);
            }
        }
        if(!ctype_digit(strval($int2)))
        {
            if($int2[0] != "-" || !ctype_digit(substr($int2, 1)))
            {
                exit(ReturnCode::INVALID_SOURCE_STRUCTURE);
            } 
        }
        $result = (string) ($int1 - $int2);
        $variable->setValue($result);
        $variable->setType("int");
    }

    /************************* MUL *************************/
    public function MUL(Frames $frames) : void
    {
        //same as ADD
        $variable = $this->checkVariable($frames);
        $variable2 = $this->checkSymb($frames, 1);
        $variable3 = $this->checkSymb($frames, 2);
        if($variable == null || $variable2 == null || $variable3 == null)
        {
            exit(ReturnCode::VARIABLE_ACCESS_ERROR);
        }
        if($this->getArgCount() > 3)
        {
            exit(ReturnCode::INVALID_SOURCE_STRUCTURE);
        }

        if(($variable2 instanceof Variable ? $variable2->getType() : $this->getArgN(1)->getType()) != "int" || ($variable3 instanceof Variable ? $variable3->getType() : $this->getArgN(2)->getType()) != "int")
        {
            exit(ReturnCode::OPERAND_TYPE_ERROR);
        }
        $int1 = ($variable2 instanceof Variable ? $variable2->getValue() : $variable2);
        $int2 = ($variable3 instanceof Variable ? $variable3->getValue() : $variable3);
        if(!ctype_digit(strval($int1)))
        {
            if($int1[0] != "-" || !ctype_digit(substr($int1, 1)))
            {
                exit(ReturnCode::INVALID_SOURCE_STRUCTURE);
            }
        }
        if(!ctype_digit(strval($int2)))
        {
            if($int2[0] != "-" || !ctype_digit(substr($int2, 1)))
            {
                exit(ReturnCode::INVALID_SOURCE_STRUCTURE);
            } 
        }
        $result = (string) ($int1 * $int2);
        $variable->setValue($result);
        $variable->setType("int");
    }

    /************************* IDIV *************************/
    public function IDIV(Frames $frames) : void
    {
        //same as ADD
        $variable = $this->checkVariable($frames);
        $variable2 = $this->checkSymb($frames, 1);
        $variable3 = $this->checkSymb($frames, 2);
        if($variable == null || $variable2 == null || $variable3 == null)
        {
            exit(ReturnCode::VARIABLE_ACCESS_ERROR);
        }
        if($this->getArgCount() > 3)
        {
            exit(ReturnCode::INVALID_SOURCE_STRUCTURE);
        }

        if(($variable2 instanceof Variable ? $variable2->getType() : $this->getArgN(1)->getType()) != "int" || ($variable3 instanceof Variable ? $variable3->getType() : $this->getArgN(2)->getType()) != "int")
        {
            exit(ReturnCode::OPERAND_TYPE_ERROR);
        }
        $int1 = ($variable2 instanceof Variable ? $variable2->getValue() : $variable2);
        $int2 = ($variable3 instanceof Variable ? $variable3->getValue() : $variable3);
        if(!ctype_digit(strval($int1)))
        {
            if($int1[0] != "-" || !ctype_digit(substr($int1, 1)))
            {
                exit(ReturnCode::INVALID_SOURCE_STRUCTURE);
            }
        }
        if(!ctype_digit(strval($int2)))
        {
            if($int2[0] != "-" || !ctype_digit(substr($int2, 1)))
            {
                exit(ReturnCode::INVALID_SOURCE_STRUCTURE);
            } 
        }
        if($int2 == "0")
        {
            exit(ReturnCode::OPERAND_VALUE_ERROR);
        }
        $result = (string) ($int1 / $int2);
        $variable->setValue($result);
        $variable->setType("int");
    }

    /************************* LT *************************/
    public function LT(Frames $frames) : void
    {
        $variable = $this->checkVariable($frames);
        $variable2 = $this->checkSymb($frames, 1);
        $variable3 = $this->checkSymb($frames, 2);
        if($variable == null || $variable2 == null || $variable3 == null)
        {
            exit(ReturnCode::VARIABLE_ACCESS_ERROR);
        }
        if($this->getArgCount() > 3)
        {
            exit(ReturnCode::INVALID_SOURCE_STRUCTURE);
        }

        //getting right values from argument/variables and comparing
        if ($variable2 instanceof Variable && $variable3 instanceof Variable)
        {
            if ($variable2->getType() === $variable3->getType() && $variable2->getType() != "nil") {
                $variable->setValue($variable2->getValue() < $variable3->getValue() ? true : false);
            } else {
                exit(ReturnCode::OPERAND_TYPE_ERROR);
            }
        } elseif ($variable2 instanceof Variable) {
            if ($variable2->getType() === $this->getArgN(2)->getType() && $variable2->getType() != "nil") {
                $variable->setValue($variable2->getValue() < $variable3 ? true : false);
            } else {
                exit(ReturnCode::OPERAND_TYPE_ERROR);
            }
        } elseif ($variable3 instanceof Variable) {
            if ($this->getArgN(1)->getType() === $variable3->getType() && $variable3->getType() != "nil") {
                $variable->setValue($variable2 < $variable3->getValue() ? true : false);
            } else {
                exit(ReturnCode::OPERAND_TYPE_ERROR);
            }
        } else {
            if ($this->getArgN(1)->getType() === $this->getArgN(2)->getType() && $this->getArgN(1)->getType() != "nil") {
                $variable->setValue($variable2 < $variable3 ? true : false);
            } else {
                exit(ReturnCode::OPERAND_TYPE_ERROR);
            }
        }
        $variable->setType("bool");
    }

    /************************* GT *************************/
    public function GT(Frames $frames) : void
    {
        //same as LT
        $variable = $this->checkVariable($frames);
        $variable2 = $this->checkSymb($frames, 1);
        $variable3 = $this->checkSymb($frames, 2);
        if($variable == null || $variable2 == null || $variable3 == null)
        {
            exit(ReturnCode::VARIABLE_ACCESS_ERROR);
        }
        if($this->getArgCount() > 3)
        {
            exit(ReturnCode::INVALID_SOURCE_STRUCTURE);
        }

        if ($variable2 instanceof Variable && $variable3 instanceof Variable)
        {
            if ($variable2->getType() === $variable3->getType() && $variable2->getType() != "nil") {
                $variable->setValue($variable2->getValue() > $variable3->getValue() ? true : false);
            } else {
                exit(ReturnCode::OPERAND_TYPE_ERROR);
            }
        } elseif ($variable2 instanceof Variable) {
            if ($variable2->getType() === $this->getArgN(2)->getType() && $variable2->getType() != "nil") {
                $variable->setValue($variable2->getValue() > $variable3 ? true : false);
            } else {
                exit(ReturnCode::OPERAND_TYPE_ERROR);
            }
        } elseif ($variable3 instanceof Variable) {
            if ($this->getArgN(1)->getType() === $variable3->getType() && $variable3->getType() != "nil") {
                $variable->setValue($variable2 > $variable3->getValue() ? true : false);
            } else {
                exit(ReturnCode::OPERAND_TYPE_ERROR);
            }
        } else {
            if ($this->getArgN(1)->getType() === $this->getArgN(2)->getType() && $this->getArgN(1)->getType() != "nil") {
                $variable->setValue($variable2 > $variable3 ? true : false);
            } else {
                exit(ReturnCode::OPERAND_TYPE_ERROR);
            }
        }
        $variable->setType("bool");
    }

    /************************* EQ *************************/
    public function EQ(Frames $frames) : void
    {
        //similar to LT, but can compare nil values
        $variable = $this->checkVariable($frames);
        $variable2 = $this->checkSymb($frames, 1);
        $variable3 = $this->checkSymb($frames, 2);
        if($variable == null || $variable2 == null || $variable3 == null)
        {
            exit(ReturnCode::VARIABLE_ACCESS_ERROR);
        }
        if($this->getArgCount() > 3)
        {
            exit(ReturnCode::INVALID_SOURCE_STRUCTURE);
        }

        if ($variable2 instanceof Variable && $variable3 instanceof Variable)
        {
            if ($variable2->getType() === $variable3->getType() || $variable2->getType() == "nil" || $variable3->getType() == "nil") {
                $variable->setValue($variable2->getValue() == $variable3->getValue() ? true : false);
            } else {
                exit(ReturnCode::OPERAND_TYPE_ERROR);
            }
        } elseif ($variable2 instanceof Variable) {
            if ($variable2->getType() === $this->getArgN(2)->getType() || $variable2->getType() == "nil" || $this->getArgN(2)->getType() == "nil") {
                $variable->setValue($variable2->getValue() == $variable3 ? true : false);
            } else {
                exit(ReturnCode::OPERAND_TYPE_ERROR);
            }
        } elseif ($variable3 instanceof Variable) {
            if ($this->getArgN(1)->getType() === $variable3->getType() || $this->getArgN(1)->getType() == "nil" || $variable3->getType() == "nil") {
                $variable->setValue($variable2 == $variable3->getValue() ? true : false);
            } else {
                exit(ReturnCode::OPERAND_TYPE_ERROR);
            }
        } else {
            if ($this->getArgN(1)->getType() === $this->getArgN(2)->getType() || $this->getArgN(1)->getType() !== "nil" || $this->getArgN(2)->getType() !== "nil") {
                $variable->setValue($variable2 == $variable3 ? true : false);
            } else {
                exit(ReturnCode::OPERAND_TYPE_ERROR);
            }
        }
        $variable->setType("bool");
    }

    /************************* AND *************************/
    public function AND(Frames $frames) : void
    {
        $variable = $this->checkVariable($frames);
        $variable2 = $this->checkSymb($frames, 1);
        $variable3 = $this->checkSymb($frames, 2);
        if($variable == null || $variable2 == null || $variable3 == null)
        {
            exit(ReturnCode::VARIABLE_ACCESS_ERROR);
        }
        if($this->getArgCount() > 3)
        {
            exit(ReturnCode::INVALID_SOURCE_STRUCTURE);
        }

        //getting right values from argument/variables and applying logic AND 
        if ($variable2 instanceof Variable && $variable3 instanceof Variable)
        {
            if ($variable2->getType() == "bool" && $variable3->getType() == "bool") {
                $variable->setValue($variable2->getValue() == "true" && $variable3->getValue() == "true" ? true : false);
            } else {
                exit(ReturnCode::OPERAND_TYPE_ERROR);
            }
        } elseif ($variable2 instanceof Variable) {
            if ($variable2->getType() == "bool" && $this->getArgN(2)->getType() == "bool") {
                $variable->setValue($variable2->getValue()  == "true" && $variable3  == "true" ? true : false);
            } else {
                exit(ReturnCode::OPERAND_TYPE_ERROR);
            }
        } elseif ($variable3 instanceof Variable) {
            if ($this->getArgN(1)->getType() == "bool" && $variable3->getType() == "bool") {
                $variable->setValue($variable2  == "true" && $variable3->getValue()  == "true" ? true : false);
            } else {
                exit(ReturnCode::OPERAND_TYPE_ERROR);
            }
        } else {
            if ($this->getArgN(1)->getType() == "bool" && $this->getArgN(2)->getType() == "bool") {
                $variable->setValue($variable2  == "true" && $variable3  == "true" ? true : false);
            } else {
                exit(ReturnCode::OPERAND_TYPE_ERROR);
            }
        }
        $variable->setType("bool");
    }

    /************************* OR *************************/
    public function OR(Frames $frames) : void
    {
        //same as AND
        $variable = $this->checkVariable($frames);
        $variable2 = $this->checkSymb($frames, 1);
        $variable3 = $this->checkSymb($frames, 2);
        if($variable == null || $variable2 == null || $variable3 == null)
        {
            exit(ReturnCode::VARIABLE_ACCESS_ERROR);
        }
        if($this->getArgCount() > 3)
        {
            exit(ReturnCode::INVALID_SOURCE_STRUCTURE);
        }

        if ($variable2 instanceof Variable && $variable3 instanceof Variable)
        {
            if ($variable2->getType() == "bool" && $variable3->getType() == "bool") {
                $variable->setValue($variable2->getValue() == "true" || $variable3->getValue()  == "true" ? true : false);
            } else {
                exit(ReturnCode::OPERAND_TYPE_ERROR);
            }
        } elseif ($variable2 instanceof Variable) {
            if ($variable2->getType() == "bool" && $this->getArgN(2)->getType() == "bool") {
                $variable->setValue($variable2->getValue()  == "true" || $variable3 == "true"? true : false);
            } else {
                exit(ReturnCode::OPERAND_TYPE_ERROR);
            }
        } elseif ($variable3 instanceof Variable) {
            if ($this->getArgN(1)->getType() == "bool" && $variable3->getType() == "bool") {
                $variable->setValue($variable2  == "true" || $variable3->getValue()  == "true" ? true : false);
            } else {
                exit(ReturnCode::OPERAND_TYPE_ERROR);
            }
        } else {
            if ($this->getArgN(1)->getType() == "bool" && $this->getArgN(2)->getType() == "bool") {
                $variable->setValue($variable2  == "true" || $variable3  == "true" ? true : false);
            } else {
                exit(ReturnCode::OPERAND_TYPE_ERROR);
            }
        }
        $variable->setType("bool");
    }

    /************************* NOT *************************/
    public function NOT(Frames $frames) : void
    {
        //similar to AND, only one parameter
        $variable = $this->checkVariable($frames);
        $variable2 = $this->checkSymb($frames, 1);
        if($variable == null || $variable2 == null)
        {
            exit(ReturnCode::VARIABLE_ACCESS_ERROR);
        }
        if($this->getArgCount() > 2)
        {
            exit(ReturnCode::INVALID_SOURCE_STRUCTURE);
        }

        if ($variable2 instanceof Variable)
        {
            if ($variable2->getType() == "bool") {
                $variable->setValue($variable2->getValue()  == "true" ? false : true);
            } else {
                exit(ReturnCode::OPERAND_TYPE_ERROR);
            }
        } else {
            if ($this->getArgN(1)->getType() == "bool") {
                $variable->setValue($variable2  == "true" ? false : true);
            } else {
                exit(ReturnCode::OPERAND_TYPE_ERROR);
            }
        }
        $variable->setType("bool");
    }

    /************************* INT2CHAR *************************/
    public function INT2CHAR(Frames $frames) : void
    {
        $variable = $this->checkVariable($frames);
        $variable2 = $this->checkSymb($frames, 1);
        if($variable == null || $variable2 == null)
        {
            exit(ReturnCode::VARIABLE_ACCESS_ERROR);
        }
        if($this->getArgCount() > 2)
        {
            exit(ReturnCode::INVALID_SOURCE_STRUCTURE);
        }

        if ($variable2 instanceof Variable)
        {
            if ($variable2->getType() == "int") {
                $variable->setValue(mb_chr($variable2->getValue(), 'UTF-8'));
                if ($variable->getValue() === false) {
                    exit(ReturnCode::STRING_OPERATION_ERROR);
                }
            } else {
                exit(ReturnCode::OPERAND_TYPE_ERROR);
            }
        } else {
            if ($this->getArgN(1)->getType() == "int") {
                $variable->setValue(mb_chr($variable2, 'UTF-8'));
                if ($variable->getValue() === false) {
                    exit(ReturnCode::STRING_OPERATION_ERROR);
                }
            } else {
                exit(ReturnCode::OPERAND_TYPE_ERROR);
            }
        }
        $variable->setType("string");
    }

    /************************* STRI2INT *************************/
    public function STRI2INT(Frames $frames) : void
    {
        $variable = $this->checkVariable($frames);
        $variable2 = $this->checkSymb($frames, 1);
        $variable3 = $this->checkSymb($frames, 2);
        if($variable == null || $variable2 == null || $variable3 == null)
        {
            exit(ReturnCode::VARIABLE_ACCESS_ERROR);
        }
        if($this->getArgCount() > 3)
        {
            exit(ReturnCode::INVALID_SOURCE_STRUCTURE);
        }

        if ($variable2 instanceof Variable && $variable3 instanceof Variable)
        {
            if ($variable2->getType() == "string" && $variable3->getType() == "int") {
                if(!is_string($variable2->getValue()))
                {
                    exit(ReturnCode::OPERAND_TYPE_ERROR);
                }
                if($variable3->getValue() >= strlen($variable2->getValue()))
                {
                    exit(ReturnCode::STRING_OPERATION_ERROR);
                }
                $variable->setValue(mb_ord($variable2->getValue()[$variable3->getValue()], 'UTF-8'));
            } else {
                exit(ReturnCode::OPERAND_TYPE_ERROR);
            }
        } elseif ($variable2 instanceof Variable) {
            if ($variable2->getType() == "string" && $this->getArgN(2)->getType() == "int") {
                if(!is_string($variable2->getValue()))
                {
                    exit(ReturnCode::OPERAND_TYPE_ERROR);
                }
                if($this->getArgN(2)->getName() >= strlen($variable2->getValue()))
                {
                    exit(ReturnCode::STRING_OPERATION_ERROR);
                }
                $variable->setValue(mb_ord($variable2->getValue()[$this->getArgN(2)->getName()], 'UTF-8'));
            } else {
                exit(ReturnCode::OPERAND_TYPE_ERROR);
            }
        } elseif ($variable3 instanceof Variable) {
            if ($this->getArgN(1)->getType() == "string" && $variable3->getType() == "int") {
                if(!is_string($this->getArgN(1)->getName()))
                {
                    exit(ReturnCode::OPERAND_TYPE_ERROR);
                }
                if($variable3->getValue() >= strlen($this->getArgN(1)->getName()))
                {
                    exit(ReturnCode::STRING_OPERATION_ERROR);
                }
                $variable->setValue(mb_ord($this->getArgN(1)->getName()[$variable3->getValue()], 'UTF-8'));
            } else {
                exit(ReturnCode::OPERAND_TYPE_ERROR);
            }
        } else {
            if ($this->getArgN(1)->getType() == "string" && $this->getArgN(2)->getType() == "int") {
                if(!is_string($this->getArgN(1)->getName()))
                {
                    exit(ReturnCode::OPERAND_TYPE_ERROR);
                }
                if($this->getArgN(2)->getName() >= strlen($this->getArgN(1)->getName()))
                {
                    exit(ReturnCode::STRING_OPERATION_ERROR);
                }
                $variable->setValue(mb_ord($this->getArgN(1)->getName()[$this->getArgN(2)->getName()], 'UTF-8'));
            } else {
                exit(ReturnCode::OPERAND_TYPE_ERROR);
            }
        }
        $variable->setType("int");
    }

    /************************* READ *************************/
    public function READ(Frames $frames) : mixed
    {
        $variable = $this->checkVariable($frames);
        if($variable == null)
        {
            exit(ReturnCode::VARIABLE_ACCESS_ERROR);
        }
        //checks the right type
        if($this->getArgN(1)->getType() != "type")
        {
            exit(ReturnCode::INVALID_SOURCE_STRUCTURE);
        }
        return $variable;
    }

    /************************* WRITE *************************/
    public function WRITE(Frames $frames) : mixed
    {
        $variable = $this->checkSymb($frames, 0);
        if($variable == null)
        {
            exit(ReturnCode::VARIABLE_ACCESS_ERROR);
        }
        if($this->getArgCount() > 1)
        {
            exit(ReturnCode::INVALID_SOURCE_STRUCTURE);
        }

        return $variable;
    }

    /************************* CONCAT *************************/
    public function CONCAT(Frames $frames) : void
    {
        $variable = $this->checkVariable($frames);
        $variable2 = $this->checkSymb($frames, 1);
        $variable3 = $this->checkSymb($frames, 2);
        if($variable == null || $variable2 == null || $variable3 == null)
        {
            exit(ReturnCode::VARIABLE_ACCESS_ERROR);
        }
        if($this->getArgCount() > 3)
        {
            exit(ReturnCode::INVALID_SOURCE_STRUCTURE);
        }

        //getting right values from argument/variables and concatenating them
        if ($variable2 instanceof Variable && $variable3 instanceof Variable)
        {
            if ($variable2->getType() == "string" && $variable3->getType() == "string") {
                $variable->setValue($variable2->getValue() . $variable3->getValue());
            } else {
                exit(ReturnCode::OPERAND_TYPE_ERROR);
            }
        } elseif ($variable2 instanceof Variable) {
            if ($variable2->getType() == "string" && $this->getArgN(2)->getType() == "string") {
                $variable->setValue($variable2->getValue() . $variable3);
            } else {
                exit(ReturnCode::OPERAND_TYPE_ERROR);
            }
        } elseif ($variable3 instanceof Variable) {
            if ($this->getArgN(1)->getType() == "string" && $variable3->getType() == "string") {
                $variable->setValue($variable2 . $variable3->getValue());
            } else {
                exit(ReturnCode::OPERAND_TYPE_ERROR);
            }
        } else {
            if ($this->getArgN(1)->getType() == "string" && $this->getArgN(2)->getType() == "string") {
                $variable->setValue($variable2 . $variable3);
            } else {
                exit(ReturnCode::OPERAND_TYPE_ERROR);
            }
        }
        $variable->setType("string");
    }

    /************************* STRLEN *************************/
    public function STRLEN(Frames $frames) : void
    {
        $variable = $this->checkVariable($frames);
        $variable2 = $this->checkSymb($frames, 1);
        if($variable == null || $variable2 == null)
        {
            exit(ReturnCode::VARIABLE_ACCESS_ERROR);
        }
        if($this->getArgCount() > 2)
        {
            exit(ReturnCode::INVALID_SOURCE_STRUCTURE);
        }

        if ($variable2 instanceof Variable)
        {
            if ($variable2->getType() == "string") {
                if(!is_string($variable2->getValue()))
                {
                    exit(ReturnCode::OPERAND_TYPE_ERROR);
                }
                $variable->setValue(strlen($variable2->getValue()));
            } else {
                exit(ReturnCode::OPERAND_TYPE_ERROR);
            }
        } else {
            if ($this->getArgN(1)->getType() == "string") {
                $variable->setValue(strlen($variable2));
            } else {
                exit(ReturnCode::OPERAND_TYPE_ERROR);
            }
        }
        $variable->setType("int");
    }

    /************************* GETCHAR *************************/
    public function GETCHAR(Frames $frames) : void
    {
        $variable = $this->checkVariable($frames);
        $variable2 = $this->checkSymb($frames, 1);
        $variable3 = $this->checkSymb($frames, 2);
        if($variable == null || $variable2 == null || $variable3 == null)
        {
            exit(ReturnCode::VARIABLE_ACCESS_ERROR);
        }
        if($this->getArgCount() > 3)
        {
            exit(ReturnCode::INVALID_SOURCE_STRUCTURE);
        }

        if ($variable2 instanceof Variable && $variable3 instanceof Variable)
        {
            if ($variable2->getType() == "string" && $variable3->getType() == "int") {
                if(!is_string($variable2->getValue()))
                {
                    exit(ReturnCode::OPERAND_TYPE_ERROR);
                }
                if($variable3->getValue() >= strlen($variable2->getValue()))
                {
                    exit(ReturnCode::STRING_OPERATION_ERROR);
                }
                $variable->setValue($variable2->getValue()[$variable3->getValue()]);
            } else {
                exit(ReturnCode::OPERAND_TYPE_ERROR);
            }
        } elseif ($variable2 instanceof Variable) {
            if ($variable2->getType() == "string" && $this->getArgN(2)->getType() == "int") {
                if(!is_string($variable2->getValue()))
                {
                    exit(ReturnCode::OPERAND_TYPE_ERROR);
                }
                if($this->getArgN(2)->getName() >= strlen($variable2->getValue()))
                {
                    exit(ReturnCode::STRING_OPERATION_ERROR);
                }
                $variable->setValue($variable2->getValue()[$this->getArgN(2)->getName()]);
            } else {
                exit(ReturnCode::OPERAND_TYPE_ERROR);
            }
        } elseif ($variable3 instanceof Variable) {
            if ($this->getArgN(1)->getType() == "string" && $variable3->getType() == "int") {
                if(!is_string($this->getArgN(1)->getName()))
                {
                    exit(ReturnCode::OPERAND_TYPE_ERROR);
                }
                if($variable3->getValue() >= strlen($this->getArgN(1)->getName()))
                {
                    exit(ReturnCode::STRING_OPERATION_ERROR);
                }
                $variable->setValue($this->getArgN(1)->getName()[$variable3->getValue()]);
            } else {
                exit(ReturnCode::OPERAND_TYPE_ERROR);
            }
        } else {
            if ($this->getArgN(1)->getType() == "string" && $this->getArgN(2)->getType() == "int") {
                if(!is_string($this->getArgN(1)->getName()))
                {
                    exit(ReturnCode::OPERAND_TYPE_ERROR);
                }
                if($this->getArgN(2)->getName() >= strlen($this->getArgN(1)->getName()))
                {
                    exit(ReturnCode::STRING_OPERATION_ERROR);
                }
                $variable->setValue($this->getArgN(1)->getName()[$this->getArgN(2)->getName()]);
            } else {
                exit(ReturnCode::OPERAND_TYPE_ERROR);
            }
        }
        $variable->setType("string");
    }

    /************************* SETCHAR *************************/
    public function SETCHAR(Frames $frames) : void
    {
        $variable = $this->checkVariable($frames);
        $variable2 = $this->checkSymb($frames, 1);
        $variable3 = $this->checkSymb($frames, 2);
        if($variable == null || $variable2 == null || $variable3 == null)
        {
            exit(ReturnCode::VARIABLE_ACCESS_ERROR);
        }
        if($this->getArgCount() > 3)
        {
            exit(ReturnCode::INVALID_SOURCE_STRUCTURE);
        }

        if ($variable2 instanceof Variable && $variable3 instanceof Variable)
        {
            if ($variable2->getType() == "int" && $variable3->getType() == "string") {
                if(!is_string($variable->getValue()))
                {
                    exit(ReturnCode::OPERAND_TYPE_ERROR);
                }
                if($variable2->getValue() >= strlen($variable->getValue()))
                {
                    exit(ReturnCode::STRING_OPERATION_ERROR);
                }
                $string = $variable->getValue();
                if(!is_string($string) || !is_string($variable3->getValue()) || !is_string($variable2->getValue()))
                {
                    exit(ReturnCode::OPERAND_TYPE_ERROR);
                }
                $string[intval($variable2->getValue())] = $variable3->getValue()[0];
                $variable->setValue($string);
            } else {
                exit(ReturnCode::OPERAND_TYPE_ERROR);
            }
        } elseif ($variable2 instanceof Variable) {
            if ($variable2->getType() == "int" && $this->getArgN(2)->getType() == "string") {
                if(!is_string($variable->getValue()))
                {
                    exit(ReturnCode::OPERAND_TYPE_ERROR);
                }
                if($variable2->getValue() >= strlen($variable->getValue()))
                {
                    exit(ReturnCode::STRING_OPERATION_ERROR);
                }
                $string = $variable->getValue();
                if(!is_string($string) || !is_string($this->getArgN(2)->getName()) || !is_string($variable2->getValue()))
                {
                    exit(ReturnCode::OPERAND_TYPE_ERROR);
                }
                $string[intval($variable2->getValue())] = $this->getArgN(2)->getName()[0];
                $variable->setValue($string);
            } else {
                exit(ReturnCode::OPERAND_TYPE_ERROR);
            }
        } elseif ($variable3 instanceof Variable) {
            if ($this->getArgN(1)->getType() == "int" && $variable3->getType() == "string") {
                if(!is_string($variable->getValue()))
                {
                    exit(ReturnCode::OPERAND_TYPE_ERROR);
                }
                if($this->getArgN(1)->getName() >= strlen($variable->getValue()))
                {
                    exit(ReturnCode::STRING_OPERATION_ERROR);
                }
                $string = $variable->getValue();
                if(!is_string($string) || !is_string($variable3->getValue()) || !is_string($this->getArgN(1)->getName()))
                {
                    exit(ReturnCode::OPERAND_TYPE_ERROR);
                }
                $string[intval($this->getArgN(1)->getName())] = $variable3->getValue()[0];
                $variable->setValue($string);            
            } else {
                exit(ReturnCode::OPERAND_TYPE_ERROR);
            }
        } else {
            if ($this->getArgN(1)->getType() == "int" && $this->getArgN(2)->getType() == "string") {
                if(!is_string($variable->getValue()))
                {
                    exit(ReturnCode::OPERAND_TYPE_ERROR);
                }
                if($this->getArgN(1)->getName() >= strlen($variable->getValue()))
                {
                    exit(ReturnCode::STRING_OPERATION_ERROR);
                }
                $string = $variable->getValue();
                if(!is_string($string) || !is_string($this->getArgN(1)->getName()) || !is_string($this->getArgN(2)->getType()))
                {
                    exit(ReturnCode::OPERAND_TYPE_ERROR);
                }
                $string[intval($this->getArgN(1)->getName())] = $this->getArgN(2)->getName()[0];
                $variable->setValue($string);
            } else {
                exit(ReturnCode::OPERAND_TYPE_ERROR);
            }
        }
        $variable->setType("string");
    }

    /************************* TYPE *************************/
    public function TYPE(Frames $frames) : void
    {
        $variable = $this->checkVariable($frames);
        $variable2 = $this->checkSymb($frames, 1);
        if($variable == null)
        {
            exit(ReturnCode::VARIABLE_ACCESS_ERROR);
        }
        if($this->getArgCount() > 2)
        {
            exit(ReturnCode::INVALID_SOURCE_STRUCTURE);
        }

        if ($variable2 instanceof Variable)
        {
            if($variable2->getType() == null)
            {
                $variable->setValue("");
            }
            else
            {
                $variable->setValue($variable2->getType());
            }
        } else {
            $variable->setValue($this->getArgN(1)->getType());
        }
        $variable->setType("string");
    }

    /************************* EXIT *************************/
    public function EXIT(Frames $frames) : void
    {
        $variable = $this->checkSymb($frames, 0);
        if($this->getArgCount() > 1)
        {
            exit(ReturnCode::INVALID_SOURCE_STRUCTURE);
        }

        if ($variable instanceof Variable)
        {
            if ($variable->getType() == "int") {
                if($variable->getValue() >= 0 && $variable->getValue() <= 9)
                {
                    exit($variable->getValue());
                }
                exit(ReturnCode::OPERAND_VALUE_ERROR);
            } else {
                exit(ReturnCode::OPERAND_TYPE_ERROR);
            }
        } else {
            if ($this->getArgN(0)->getType() == "int") {
                if($this->getArgN(0)->getName() >= 0 && $this->getArgN(0)->getName() <= 9)
                {
                    exit(intval($this->getArgN(0)->getName()));
                }
                exit(ReturnCode::OPERAND_VALUE_ERROR);
            } else {
                exit(ReturnCode::OPERAND_TYPE_ERROR);
            }
        }
    }
}
?>