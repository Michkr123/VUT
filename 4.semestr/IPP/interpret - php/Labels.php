<?php

namespace IPP\Student;

use IPP\Core\ReturnCode;

//stores labels name and position on code
class Labels
{
    /** @var Int[] */
    private $labels = [];

    public function __construct()
    {
        $this->labels = [];
    }

    public function LABEL(string $name, int $line) : void
    {
        if(isset($this->labels[$name]))
        {
            exit(ReturnCode::SEMANTIC_ERROR);
        }
        $this->labels[$name] = $line;
    }

    public function getLabelLine(string $name) : int
    {
        if(!isset($this->labels[$name]))
        {
            exit(ReturnCode::SEMANTIC_ERROR);
        }
        return $this->labels[$name];
    }

    public function JUMPIFEQ(Frames $frames, Instruction $instr) : ?int
    {
        $name = $instr->getArgN(0)->getName();
        if($instr->getArgN(0)->getType() == "label")
        {
            $line = $this->getLabelLine($name);
        }
        else
        {
            exit(ReturnCode::OPERAND_TYPE_ERROR);
        }  
        $variable2 = $instr->getArgN(1)->getName();
        if($instr->getArgN(1)->getType() == "var")
        {
            $parts = explode("@", $variable2);
            $frame2 = $parts[0];
            if(!isset($parts[1]))
            {
                exit(ReturnCode::VARIABLE_ACCESS_ERROR);
            }
            $name2 = $parts[1];
            $variable2 =  $frames->isInFrames($name2, $frame2);
        }
        $variable3 = $instr->getArgN(2)->getName();
        if($instr->getArgN(2)->getType() == "var")
        {
            $parts = explode("@", $variable3);
            $frame3 = $parts[0];
            if(!isset($parts[1]))
            {
                exit(ReturnCode::VARIABLE_ACCESS_ERROR);
            }
            $name3 = $parts[1];
            $variable3 =  $frames->isInFrames($name3, $frame3);
        }
        if($variable2 == null || $variable3 == null)
        {
            exit(ReturnCode::VARIABLE_ACCESS_ERROR);
        }

        //checking right type, comparing (same as EQ)
        if ($variable2 instanceof Variable && $variable3 instanceof Variable)
        {
            if ($variable2->getType() === $variable3->getType() || $variable2->getType() == "nil" || $variable3->getType() == "nil") {
                $jump = ($variable2->getValue() == $variable3->getValue() ? true : false);
            } else {
                exit(53);
            }
        } elseif ($variable2 instanceof Variable) {
            if ($variable2->getType() === $instr->getArgN(2)->getType() || $variable2->getType() == "nil" || $instr->getArgN(2)->getType() == "nil") {
                $jump = ($variable2->getValue() == $variable3 ? true : false);
            } else {
                exit(53);
            }
        } elseif ($variable3 instanceof Variable) {
            if ($instr->getArgN(1)->getType() === $variable3->getType() || $instr->getArgN(1)->getType() == "nil" || $variable3->getType() == "nil") {
                $jump = ($variable2 == $variable3->getValue() ? true : false);
            } else {
                exit(53);
            }
        } else {
            if ($instr->getArgN(1)->getType() === $instr->getArgN(2)->getType() || $instr->getArgN(1)->getType() !== "nil" || $instr->getArgN(2)->getType() !== "nil") {
                $jump = ($variable2 == $variable3 ? true : false);
            } else {
                exit(53);
            }
        }
        
        if($jump)
        {
            return $line;
        }
        else
        {
            return null;
        }
    }

    //same is JUMPIFEQ
    public function JUMPIFNEQ(Frames $frames, Instruction $instr) : ?int
    {
        $name = $instr->getArgN(0)->getName();
        if($instr->getArgN(0)->getType() == "label")
        {
            $line = $this->getLabelLine($name);
        }
        else
        {
            exit(ReturnCode::OPERAND_TYPE_ERROR);
        }
        
        $variable2 = $instr->getArgN(1)->getName();
        if($instr->getArgN(1)->getType() == "var")
        {
            $parts = explode("@", $variable2);
            $frame2 = $parts[0];
            if(!isset($parts[1]))
            {
                exit(ReturnCode::VARIABLE_ACCESS_ERROR);
            }
            $name2 = $parts[1];
            $variable2 =  $frames->isInFrames($name2, $frame2);
        }
        $variable3 = $instr->getArgN(2)->getName();
        if($instr->getArgN(2)->getType() == "var")
        {
            $parts = explode("@", $variable3);
            $frame3 = $parts[0];
            if(!isset($parts[1]))
            {
                exit(ReturnCode::VARIABLE_ACCESS_ERROR);
            }
            $name3 = $parts[1];
            $variable3 =  $frames->isInFrames($name3, $frame3);
        }
        if($variable2 == null || $variable3 == null)
        {
            exit(ReturnCode::VARIABLE_ACCESS_ERROR);
        }

        if ($variable2 instanceof Variable && $variable3 instanceof Variable)
        {
            if ($variable2->getType() === $variable3->getType() || $variable2->getType() == "nil" || $variable3->getType() == "nil") {
                $jump = ($variable2->getValue() == $variable3->getValue() ? true : false);
            } else {
                exit(53);
            }
        } elseif ($variable2 instanceof Variable) {
            if ($variable2->getType() === $instr->getArgN(2)->getType() || $variable2->getType() == "nil" || $instr->getArgN(2)->getType() == "nil") {
                $jump = ($variable2->getValue() == $variable3 ? true : false);
            } else {
                exit(53);
            }
        } elseif ($variable3 instanceof Variable) {
            if ($instr->getArgN(1)->getType() === $variable3->getType() || $instr->getArgN(1)->getType() == "nil" || $variable3->getType() == "nil") {
                $jump = ($variable2 == $variable3->getValue() ? true : false);
            } else {
                exit(53);
            }
        } else {
            if ($instr->getArgN(1)->getType() === $instr->getArgN(2)->getType() || $instr->getArgN(1)->getType() !== "nil" || $instr->getArgN(2)->getType() !== "nil") {
                $jump = ($variable2 == $variable3 ? true : false);
            } else {
                exit(53);
            }
        }
        
        if(!$jump)
        {
            return $line;
        }
        else
        {
            return null;
        } 
    }
}
?>