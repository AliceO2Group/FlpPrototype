<?php
namespace CERN\Alice\DAQ\O2;
require_once __DIR__.'/ServerSocket.php';


$s = new ServerSocket();
$s->listen();
?>