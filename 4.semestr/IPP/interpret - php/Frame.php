<?php

namespace IPP\Student;

use IPP\Core\ReturnCode;

//Frame(GF, LT, TF) used for storing variables
class Frame
{
    /** @var Variable[] */
    private array $variables = [];
    private int $variableCount;

    public function __construct()
    {
        $this->variables = [];
        $this->variableCount = 0;
    }

    public function addVariable(string $name) : void
    {
        if(isset($this->variables[$name]))
        {
            exit(ReturnCode::SEMANTIC_ERROR);
        }
        $this->variables[$name] = new Variable();
        $this->variableCount++;
    }

    public function variableInFrame(string $name) : bool
    {
        return isset($this->variables[$name]);
    }
    
    public function getVariable(string $name) : ?Variable
    {
        if (isset($this->variables[$name])) {
            return $this->variables[$name];
        } else {
            return null;
        }
    }

}
?>