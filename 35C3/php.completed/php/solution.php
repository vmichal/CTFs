#!/usr/bin/php
<?php

class B {
}

$b = new B();
$b->inner = new B();
$b->outer = new B();

print serialize($b) . PHP_EOL;

?>
