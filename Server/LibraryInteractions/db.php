<?php
  $Host = "localhost";
  $User = "arduino-client";
  $PW = "arduino";
  $dbName = "LibraryInteractions";

  $cxn = mysqli_connect($Host, $User, $PW, $dbName) or die("Could not connect to server: " . $Host);
?>