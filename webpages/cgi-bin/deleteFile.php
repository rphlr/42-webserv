<?php
// deleteFile.php
$fileName = getenv('FILENAME');
$fileName = urldecode($fileName);
$pwd = getcwd();
$filePath = "$pwd/webpages/default_webpages/uploads/$fileName";

if (file_exists($filePath)) {
    unlink($filePath);
    echo "File " . $fileName . " deleted successfully.";
} else {
    echo "File " . $fileName . " does not exist in " . $pwd . "/webpages/default_webpages/uploads/";
}

?>