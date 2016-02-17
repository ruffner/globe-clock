<?php

if( isset($_POST['newText']) ) {
   
   shell_exec("sh -e /home/matt/Documents/globe-clock/text2image.sh \"" . $_POST['newText'] . "\" /tmp/out.bmp");
   shell_exec("python /home/matt/Documents/globe-clock/img2globe-nox.py /tmp/out.bmp /tmp/out.asc");
   shell_exec("cp /tmp/out.asc /var/www/html/orbit-portal/clouds/out.asc");

   header("Location: ../");
}
?>
