<?php
namespace CERN\Alice\DAQ\O2;

/**
 * Prints logs and then saves them to the file
 */
class Log {
	/**
	 * Path to the file, where logs are stored
 	 */
 	private static $prefix = '/var/log/ecs-sd-ws/';
 	/**
 	 * State if the path is writable or not
 	 */
 	private static $isWritable;
	/**
	 * Preparse the log message: adds the date, time and rank. Echos the message and saves it to the file specified in $file variable
	 */
	public static function write($message, $rank = 'info', $code = null) {
		$filename = sprintf("%s%s.log", self::$prefix, date("Y-m-d"));
		$parsed = sprintf("%s %s: %s".PHP_EOL, date("Y-m-d H:i:s"), $rank, $message);
		echo $parsed;
		if (self::$isWritable) file_put_contents($filename, $parsed, FILE_APPEND | LOCK_EX);
	}
	/**
	 * Check is the path ($prefix) is writable or not
	 */
	public static function checkPermission() {
		self::$isWritable = is_writable(self::$prefix);
	}
 }
