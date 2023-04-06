#include "tablePage.h"

char settingUp[] PROGMEM = R"=====(
<!DOCTYPE html>
<html lang="en">

<head>
    <title>Data Ketinggian Air</title>
<style>
    body {
        margin: 0;
        background-color: rgb(209, 207, 207);
        display: flex;
        justify-content: center;
    }

    .container {
        width: 357px;
        padding: 20px;
        height: 100vh;
        background-color: whitesmoke;
        text-align: center;
    }

    h1 {
        font-size: 25px;
        color: rgb(73, 73, 73);
    }

    .form {
        display: flex;
        justify-content: space-between;
        padding: 10px 20px;
    }

    .form1 {
        display: flex;
        justify-content: center;
        padding: 10px 20px;
    }

    .input {
        margin: 0 0 1em 0;
    }

    form {
        display: inline-block;
        text-align: center;
    }

    .tombol {
        margin: 20px 0 100px 0;
        text-align: center;
    }

    .tombol>a {
        text-decoration: none;
        background-color: rgb(70, 105, 105);
        padding: .5em 2em;
        color: whitesmoke;
        margin: 0 10px;
        border-radius: 1em;
        cursor: pointer;
    }

    .tombol>a:hover {
        background-color: rgb(57, 85, 85);
    }

    .tombol>.kembali {
        background-color: rgb(146, 146, 146);
    }
</style>
</head>
<body onload="process()">
    <div class="container">
        <h1>Atur Waktu</h1>
        <hr>
        <div class="form">
            <form>
                <input class="input" type="date" name="date"><br>
                <input type="submit">
            </form>
            <form>
                <input class="input" type="time" name="time"><br>
                <input type="submit">
            </form>
        </div>
        <hr>
        <div>
            Waktu Deep Sleep : <span id="deepTime"></span> Menit
        </div>
        <div class="form1">
            <form>
                <span>Deep Sleep:</span>
                <input class="input1" type="number" name="deeptime"><br>
                <input type="submit">
            </form>
        </div>
        <div class="tombol">
            <a class="kembali" href="/">Kembali</a>
        </div>
    </div>
</body>
<script>
var xmlHttp=createXmlHttpObject();

function createXmlHttpObject(){
 if(window.XMLHttpRequest){
    xmlHttp=new XMLHttpRequest();
 }else{
    xmlHttp=new ActiveXObject('Microsoft.XMLHTTP');// code for IE6, IE5
 }
 return xmlHttp;
}

function process(){
 if(xmlHttp.readyState==0 || xmlHttp.readyState==4){
   xmlHttp.open('PUT','xml',true);
   xmlHttp.onreadystatechange=handleServerResponse;
   xmlHttp.send(null);
 }
}

function handleServerResponse(){
 if(xmlHttp.readyState==4 && xmlHttp.status==200){
   xmlResponse=xmlHttp.responseXML;

   xmldoc = xmlResponse.getElementsByTagName('deeptime');
   message = xmldoc[0].firstChild.nodeValue;
   document.getElementById('deepTime').innerHTML=message;
 }
}

</script>
</html>
)=====";
