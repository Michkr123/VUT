<?php

namespace IPP\Student;

//used for storing variables
class Variable
{
    private mixed $value;
    private ?string $type;

    public function __construct() 
    {
        $this->value = null;
        $this->type = null;
    }

    public function getValue() : mixed
    {
        return $this->value;
    }

    public function setValue(mixed $value) : void
    {
        $this->value = $value;
    }

    public function setType(?string $type) : void
    {
        $this->type = $type;
    }

    public function getType() : ?string
    {
        return $this->type;
    }

}
?>