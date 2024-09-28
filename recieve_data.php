<?php
    include_once "db_tools.php";

    $host = "localhost";
    $user = "jia";
    $pwd = "1234";
    $db = "nodeMCU";

    if(isset($_GET['temp']) && isset($_GET['humi'])){
        if($_GET['temp']!=0 && $_GET['humi']!=0){
            echo "temp: ".$_GET['temp'].'<br>';
            echo "humi: ".$_GET['humi'].'<br>';
            $temp = $_GET['temp'];
            $humi = $_GET['humi'];
            
            try{
                //建立資料庫
                $conn = create_conn($host, $user, $pwd, $db);
                //新增資料
                try{
                    $sql = "INSERT INTO temp_humi (temperature, humidity) VALUES ($temp, $humi)";
                    create_data($conn, $sql);
                }catch(Exception $err){
                    echo "mesage: ".$err->getMessage();                
                    die();
                }
            }catch(Exception $err){
                echo "mesage: ".$err->getMessage();                
                die();
            }
        }else{
            echo "資料不正確!";
        }
    }else{
        echo "無資料!";
    }   

?>