<?php
namespace CERN\Alice\DAQ\WebSocket;

/**
 * Reads configuration data from Config.ini file, and stores them in static arrays
 */

class Config {

	public function __construct($filename) {
		$ini_array = parse_ini_file($filename, true);
		self::$socket = $ini_array['socket'];
		self::$ssl = $ini_array['ssl'];
		self::$data_src = $ini_array['data_src'];
		self::$general = $ini_array['general'];
		self::$calibrations = $ini_array['calibrations'];
		self::$ddls = $ini_array['ddls'];
		self::$calibration_timeouts = $ini_array['calibration_timeouts'];
	}
	/** Socket configuration:
	 *	ip		- ip address
	 *	port		- server port
	 *	socket_timeout	- timeout for accepting a client socket (seconds)
	 *	fread_timeout	- timeout for reading data for a client (check main client loop)
	 *	chunk_size	- chunk size of data read from TCP socket 
	 *	server_name	- Server name, that occurs in each HTTP response
	 */
	public static $socket;

	
	/**
	 * TLS connection configuration:
	 * - switch 	- turn on/of TLS (true/false)
	 * - filename 	- path to server's certificate
	 * - passphrase	- password to encrypt certificate
	 */
	public static $ssl;
	/**
	 * List of data sourcers
	 */
	public static $data_src;

	/**
	 * General settings:
	 * - WS_GUID	- Websocket GUID - RFC6455 (Page 24)
	 * - basicauth	- states if basic authentication is on/off
	 */
	public static $general;

	/**
	 * Associative array between full name of calibration run and 3-letter shorten name
	 */
	public static $calibrations;

	/**
	 * Number of DDLs per detector
	 */
	public static $ddls;

	public static $calibration_timeouts;
}