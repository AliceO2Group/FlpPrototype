<?php
namespace CERN\Alice\DAQ\O2;

/// Reads configuration data from Config.ini file, and stores it in static arrays
class Config {

	public static function load($filename) {
		$ini_array = parse_ini_file($filename, true);
		self::$socket = $ini_array['socket'];
		self::$zmq = $ini_array['zmq'];
	}
	/// TCP socket and TLS configuration
	/// \see Config.ini
	public static $socket;
	
	/// ZeroMQ configuration
	/// \see Config.ini
	public static $zmq;
}
