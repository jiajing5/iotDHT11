<?php
    function create_conn($host, $user, $pwd, $db){
        $conn = mysqli_connect($host, $user, $pwd);

        if(!$conn){
            throw new Exception("資料庫連線錯誤");
        }
            

        // mysqli_query($conn, "SET NAmsg utf8");

        if(!mysqli_select_db($conn, $db)){
            throw new Exception("開啟資料庫失敗");
        }
        return $conn;
    }

    function update_data($conn, $sql){
        $result = mysqli_query($conn, $sql);
        if(!$result){
            throw new Exception("更新資料有誤");
        }
        return $result;
    }

    function create_data($conn, $sql){
        $result = mysqli_query($conn, $sql);
        if(!$result){
            throw new Exception("創建資料有誤");
        }
        return $result;
    }

    function read_data($conn, $sql){
        $result = mysqli_query($conn, $sql);
        if(!$result){
            throw new Exception("讀取資料有誤");
        }
        return $result;
    }

    function delete_data($conn, $sql){
        $result = mysqli_query($conn, $sql);
        if(!$result){
            throw new Exception("刪除資料有誤");
        }
        return $result;
    }

?>