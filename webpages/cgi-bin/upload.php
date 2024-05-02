<?php
error_reporting(E_ALL);

$filename = getenv('FILENAME');
$tempFile = "/tmp/$filename";
$pwd = getcwd();
$targetDir = "$pwd/webpages/default_webpages/uploads";
$targetFile = $targetDir . "/" . $filename;
$content_type = getenv('CONTENT_TYPE');
$size = getenv('CONTENT_LENGTH');
$port = getenv('SERVER_PORT');
$method = getenv('REQUEST_METHOD');
$server_software = getenv('SERVER_SOFTWARE');
$redirect_status = getenv('REDIRECT_STATUS');

if (!is_dir($targetDir)) {
    if (!mkdir($targetDir, 0777, true)) {
        error_log("Failed to create directory: " . $targetDir);
        exit("Failed to create upload directory.");
    }
}


$targetFile = $targetDir . "/" . $filename;
error_log("Uploaded file target name : " . $targetFile);

$uploadOk = 1;

// Check if file is an image
if(isset($_POST["submit"])) {
    $check = getimagesize($_FILES["FILENAME"]["tmp_name"]);
    if($check !== false) {
        $uploadOk = 1;
    } else {
        echo "File is not an image.";
        $uploadOk = 0;
    }
}

// Check if file already exists
if (file_exists($targetFile)) {
    echo "Sorry, file already exists.\n";
    $uploadOk = 0;
}

// Check file size
else if ($size > 500000) {
    echo "Sorry, your file is too large.\n";
    $uploadOk = 0;
}

// print all the environment variables and print the file content
echo "Content-type: " . $content_type . "\n";
echo "Content-length: " . $size . "\n";
echo "Port: " . $port . "\n";
echo "Method: " . $method . "\n";
echo "Server software: " . $server_software . "\n";
echo "Filename: " . $filename . "\n";
echo "Target file: " . $targetFile . "\n";

if (file_exists($targetFile))
    echo "Sorry, file already exists.\n";
else {
    // Tentative de déplacement du fichier temporaire vers le répertoire cible
    if (rename($tempFile, $targetFile))
        echo "The file " . htmlspecialchars($filename) . " has been uploaded successfully.";
    else {
        error_log("Failed to move the file from temp directory.");
        echo "Sorry, there was an error uploading your file.";
    }
}

?>
