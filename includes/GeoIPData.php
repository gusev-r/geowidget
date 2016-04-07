<?php
require_once 'lib/MaxMind/Db/Reader/Decoder.php';
require_once 'lib/MaxMind/Db/Reader/InvalidDatabaseException.php';
require_once 'lib/MaxMind/Db/Reader/Metadata.php';
require_once 'lib/MaxMind/Db/Reader/Util.php';
require_once 'lib/MaxMind/Db/Reader.php';
class GeoIPData {
  private $reader;
  function __construct($filePath) {
    $this->reader = new Reader($filePath);
  }
  public function get($ipAddress){
    return $this->reader->get($ipAddress);
  }
  public function close() {
    $this->reader->close();
  }
  function __destruct() {
    $this->reader->close();
  }
}