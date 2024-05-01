<?php
error_reporting(E_ALL);
$pwd = getcwd();
$targetDir = "webpages/uploads";

if (!is_dir($targetDir)) {
    if (!mkdir($targetDir, 0777, true)) {
        error_log("Failed to create directory: " . $targetDir);
        exit("Failed to create upload directory.");
    }
}

$targetFile = $targetDir . basename($_FILES["file"]["name"]);
error_log("Uploaded file target name : " . $targetFile);

$uploadOk = 1;

// Check if file is an image
if(isset($_POST["submit"])) {
    $check = getimagesize($_FILES["file"]["tmp_name"]);
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
else if ($_FILES["file"]["size"] > 500000) {
    echo "Sorry, your file is too large.\n";
    $uploadOk = 0;
}

// Allow certain file formats
else if(!in_array($imageFileType, ['jpg', 'png', 'jpeg', 'gif'])) {
    echo "Sorry, only JPG, JPEG, PNG & GIF files are allowed.\n";
    $uploadOk = 0;
}

// Check if $uploadOk is set to 0 by an error
if ($uploadOk == 0) {
    echo "Sorry, your file was not uploaded.";
} else {
    if (move_uploaded_file($_FILES["file"]["tmp_name"], $targetFile)) {
        echo "The file ". htmlspecialchars(basename($_FILES["file"]["name"])). " has been uploaded.";
    } else {
        error_log("Failed to move uploaded file.");
        echo "Sorry, there was an error uploading your file.";
    }
}
?>
