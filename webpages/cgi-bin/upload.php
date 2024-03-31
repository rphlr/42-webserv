<?php
$targetDir = "webpages/uploads/";

if (!is_dir($targetDir)) {
    mkdir($targetDir, 0777, true);
}

$targetFile = $targetDir . basename($_FILES["file"]["name"]);
print "Uploaded file target name : " . getcwd() . $targetFile . "\n";

$uploadOk = 1;

// Obtenir l'extension du fichier
$imageFileType = strtolower(pathinfo($targetFile, PATHINFO_EXTENSION));
if(isset($_POST["submit"])) {
    $check = getimagesize($_FILES["file"]["tmp_name"]);
    if($check !== false) {
        $uploadOk = 1;
    } else {
        $uploadOk = 0;
    }
}

if (file_exists($targetFile)) {
    echo "Désolé, le fichier existe déjà.\n";
    $uploadOk = 0;
}
else if ($_FILES["file"]["size"] > 500000) {
    echo "Désolé, votre fichier est trop volumineux.\n";
    $uploadOk = 0;
}
else if($imageFileType != "jpg" && $imageFileType != "png" && $imageFileType != "jpeg"
&& $imageFileType != "gif" ) {
    echo "Désolé, seuls les fichiers JPG, JPEG, PNG & GIF sont autorisés.\n";
    $uploadOk = 0;
}

if ($uploadOk == 0) {
    echo "Désolé, votre fichier n'a pas été téléchargé.";
} else {
    if (move_uploaded_file($_FILES["file"]["tmp_name"], $targetFile)) {
        echo "Le fichier ". htmlspecialchars( basename( $_FILES["file"]["name"])). " a été téléchargé.";
    } else {
        echo "Désolé, il y a eu une erreur lors du téléchargement de votre fichier.";
    }
}

?>
