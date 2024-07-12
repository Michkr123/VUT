<?php

namespace IPP\Student;

use IPP\Core\ReturnCode;

//stores frames
class Frames
{
    /** @var Frame[] */
    private $frames = [];
    private int $framesCount;
    private ?Frame $TF;

    public function __construct()
    {
        $this->frames = [];
        $this->framesCount = 0;
    }

    public function getFramesCount() : int
    {
        return $this->framesCount;
    }

    public function CREATEFRAME() : void
    {
        $this->TF = new Frame();
    }

    public function PUSHFRAME() : void
    {
        if(!isset($this->TF))
        {
            exit(ReturnCode::FRAME_ACCESS_ERROR);
        }
        $this->frames[] = $this->TF;
        $this->TF = null;
        $this->framesCount++;
    }

    public function POPFRAME() : void
    {
        if ($this->framesCount > 1) {
            $this->TF = $this->getTopFrame();
            array_pop($this->frames);
            $this->framesCount--;
        } else {
            exit(ReturnCode::FRAME_ACCESS_ERROR);
        }
    }

    public function getTopFrame() : ?Frame
    {
        return $this->frames[$this->framesCount - 1];
    }
    
    public function getFrameN(int $N) : ?Frame
    {
        return $this->frames[$N];
    }

    public function getTF() : ?Frame
    {
        return $this->TF;
    }

    //checks if variable exist in all local frames
    public function existsInLocal(string $name) : ?Variable
    {
        for($i = $this->framesCount - 1; $i > 0; $i--)
        {
            if($this->frames[$i]->variableInFrame($name))
            {
                return $this->frames[$i]->getVariable($name);
            }
        }
        return null;
    }

    //checks if variable exist in global frames
    public function existsInGlobal(string $name) : ?Variable
    {
        if($this->frames[0]->variableInFrame($name))
        {
            return $this->frames[0]->getVariable($name);
        }
        return null;
    }

    //checks if variable exist in temporary frames
    public function existsInTemporary(string $name) :?Variable
    {
        if($this->TF == null)
        {
            exit(ReturnCode::FRAME_ACCESS_ERROR);
        }
        if($this->TF->variableInFrame($name))
        {
            return $this->TF->getVariable($name);
        }
        return null;
    }

    //checks if variable exist in any frame using the previous three functions
    public function isInFrames(string $name,string $frame) : ?Variable
    {
        $variable = null;
        if($frame == "GF")
        {
            $variable = $this->existsInGlobal($name);
        }
        elseif($frame == "LF")
        {
            $variable = $this->existsInLocal($name);
        }
        elseif($frame == "TF")
        {
            $variable = $this->existsInTemporary($name);
        }
        else
        {
            exit(ReturnCode::INVALID_SOURCE_STRUCTURE);
        }
        return $variable;
    }

    public function issetTF()  : bool
    {
        return (isset($this->TF));
    }

}
?>