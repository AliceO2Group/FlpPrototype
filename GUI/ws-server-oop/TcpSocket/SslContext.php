<?php
namespace CERN\Alice\DAQ\O2;

/// Creates stream context for TLS transport using data provided in configuration file
class SslContext {
	
	/// Stores SSL context
	private static $context = null;

	/// Set context options according to config file:
        /// - Path and filename of the server certificate
        /// - Certificate passphrase (optional)
        /// - If self-signed certificates are allowed
        /// - Verify peers
        /// - Certificate passphrase if set in config file
        /// \see http://php.net/manual/en/context.ssl.php
	private function __construct() {
	 	self::$context = stream_context_create();
                stream_context_set_option(self::$context, 'ssl', 'local_cert', Config::$socket['local_cert']);
                stream_context_set_option(self::$context, 'ssl', 'local_pk', Config::$socket['local_pk']);
		stream_context_set_option(self::$context, 'ssl', 'allow_self_signed', true);
                stream_context_set_option(self::$context, 'ssl', 'verify_peer', false);
		stream_context_set_option(self::$context, 'ssl', 'verify_peer_name', false);
	}
	
	
	/// Context getter
	/// \return created context
	public static function getContext() {
		if (self::$context == null) {
			new SslContext();	
		}
		return self::$context;
	}
}
?>
