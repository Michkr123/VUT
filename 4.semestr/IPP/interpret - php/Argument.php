<?php

namespace IPP\Student;

//this class is used to store arguments in argument array of instruction
class Argument
{
    private ?string $type;
    private mixed $value;

    public function __construct(?string $type, mixed $value)
    {
        $this->type = $type;
        $this->value = $value;
    }

    public function getName() : mixed
    {
        return $this->value;
    }

    public function getType() : ?string
    {
        return $this->type;
    }

}
?>