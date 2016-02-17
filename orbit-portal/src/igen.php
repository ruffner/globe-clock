<?php

if( isset($_POST['newText']) ){
   
   shell_exec("sh -e /home/matt/Documents/globe-clock/text2image.sh \"" . $_POST['newText'] . "\" /tmp/out.bmp");
   shell_exec("python /home/matt/Documents/globe-clock/img2globe-nox.py /tmp/out.bmp /tmp/out.asc");
   shell_exec("cp /tmp/out.asc /var/www/html/orbit-portal/clouds/out.asc");

   header("Location: ../");
}

if( isset($_POST['fromImage']) ){
    $bmpData = base64_decode($_POST['fromImage']);


    file_put_contents("/tmp/in.bmp", $bmpData);
    shell_exec("python /home/matt/Documents/globe-clock/img2globe-nox.py /tmp/in.bmp /tmp/out.asc");
    shell_exec("cp /tmp/out.asc /var/www/html/orbit-portal/clouds/out.asc");
}
?>
