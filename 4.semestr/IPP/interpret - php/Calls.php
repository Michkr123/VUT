<?php

namespace IPP\Student;

use IPP\Core\ReturnCode;

//used to store line where some label was called, so it can return to the right place
class Calls
{
    /** @var int[] */
    private array $calls = [];
    private int $callsCount;

    public function __construct()
    {
        $this->calls = [];
        $this->callsCount = 0;
    }

    public function CALL(int $line) : void
    {
        $this->calls[] = $line;
        $this->callsCount++;
    }

    public function RETURN() : int
    {
        if ($this->callsCount > 0) {
            $line = $this->calls[--$this->callsCount];
            array_pop($this->calls);
            return $line;
        } else {
            exit(ReturnCode::VALUE_ERROR);
        }
    }
}
?>