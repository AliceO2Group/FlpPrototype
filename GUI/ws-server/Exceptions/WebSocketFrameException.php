<?php
namespace CERN\Alice\DAQ\O2\Exceptions;

use CERN\Alice\DAQ\O2\Logging;

class WebSocketFrameException extends \Exception {
    public function __construct($message = null, $code = null, $previous = null)
    {
        parent::__construct($message, $code, $previous);
        Log::write($message, 'erro');
    }
}
?>