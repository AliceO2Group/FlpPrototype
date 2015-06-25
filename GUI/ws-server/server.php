<?php
namespace CERN\Alice\DAQ\O2;
require_once __DIR__.'/TCP/ServerSocket.php';
require_once __DIR__.'/Log.php';

date_default_timezone_set('Europe/Zurich');
$s = new ServerSocket();
$s->listen();
?>