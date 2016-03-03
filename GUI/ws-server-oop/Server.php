<?php
namespace CERN\Alice\DAQ\O2;
require_once __DIR__.'/TcpSocket/ServerSocket.php';
require_once __DIR__.'/Log.php';
require_once __DIR__.'/Config.php';

// Not to rely on system's timezone
date_default_timezone_set('Europe/Zurich');

// Load configuration file
Config::load('Config.ini');

// Create a server socket
$s = new ServerSocket();

// Start listening for clients
$s->listen();
?>
