<?php

namespace IPP\Student;

use IPP\Core\ReturnCode;

//used for storing arguments on stack
class Stack
{
    /** @var Argument[] */
    private array $arguments = [];
    private int $argumentsCount = 0;

    public function __construct()
    {
        $this->arguments = [];
        $this->argumentsCount = 0;
    }

    public function PUSHS(Frames $frames, Instruction $instr) : void
    {
        $variable = $instr->getArgN(0)->getName();
        if($instr->getArgN(0)->getType() == "var")
        {
            $parts = explode("@", $variable);
            $frame = $parts[0];
            if(!isset($parts[1]))
            {
                exit(ReturnCode::VARIABLE_ACCESS_ERROR);
            }
            $name = $parts[1];
            $variable =  $frames->isInFrames($name, $frame);
        }
        if($variable == null)
        {
            exit(ReturnCode::INVALID_SOURCE_STRUCTURE);
        }

        if($variable instanceof Variable)
        {
            $this->arguments[] = new Argument($variable->getType(), $variable->getValue());
        }
        else
        {
            $this->arguments[] = new Argument($instr->getArgN(0)->getType(), $variable);
        }
        $this->argumentsCount++;
    }

    public function POPS(Frames $frames, Instruction $instr) : void
    {
        if($this->argumentsCount < 1)
        {
            exit(ReturnCode::VALUE_ERROR);
        }
        $argument = $this->arguments[--$this->argumentsCount];
        array_pop($this->arguments);

        $variable = $instr->getArgN(0)->getName();
        if($instr->getArgN(0)->getType() == "var")
        {
            $parts = explode("@", $variable);
            $frame = $parts[0];
            if(!isset($parts[1]))
            {
                exit(ReturnCode::VARIABLE_ACCESS_ERROR);
            }
            $name = $parts[1];
            $variable =  $frames->isInFrames($name, $frame);
        }
        else
        {
            exit(ReturnCode::VARIABLE_ACCESS_ERROR);
        }
        if($variable == null)
        {
            exit(ReturnCode::INVALID_SOURCE_STRUCTURE);
        }

        $variable->setValue($argument->getName());
        $variable->setType($argument->getType());
    }
}
?>