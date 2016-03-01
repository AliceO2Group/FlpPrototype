<?php
namespace CERN\Alice\DAQ\O2\Exceptions;

use CERN\Alice\DAQ\O2\Log;

class CustomException extends \Exception {
    public function __construct($message = null, $code = null, $previous = null)
    {
        parent::__construct($message, $code, $previous);
        Log::write(sprintf("%s (%d)", $message, $code), 'error');
    }
}
?>
