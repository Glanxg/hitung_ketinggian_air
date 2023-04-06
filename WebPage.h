#include "setupPage.h"

char dashboard[] PROGMEM = R"=====(
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
    }

    h1 {
        font-size: 25px;
        color: rgb(73, 73, 73);
    }

    .waktu {
        text-align: center;
        background-color: rgb(131, 129, 129);
        color: whitesmoke;
        padding: 10px 20px;
        border-radius: 5px;
        margin: 0 0 10px 0;
    }

    .box {
        text-align: center;
        background: #579fb1;
        color: whitesmoke;
        margin: 30px 0 0 0;
        padding: 1em;
        border-radius: 10px;
    }

    #tinggiAir {
        font-size: 40px;
    }

    .tombol {
        margin: 20px 0 0 0;
        text-align: center;
    }

    .tombol a {
        text-decoration: none;
        background-color: rgb(70, 105, 105);
        padding: .5em 2em;
        color: whitesmoke;
        margin: 0 10px;
        border-radius: 1em;
        cursor: pointer;
    }

    .tombol a:hover {
        background-color: rgb(57, 85, 85);
    }
</style>
</head>
<body onload="process()">
    <div class="container">
        <h1>Data Ketinggian Air</h1>
        <hr>
        <div class="waktu">
            <div>Waktu RTC Sekarang</div>
            <span id="tahun"></span> / 
            <span id="bulan"></span> / 
            <span id="tanggal"></span> ~ 
            <span id="jam"></span>:
            <span id="menit"></span>
        </div>
        <div class="box">
            <div>Ketinggian Air Sekarang</div>
            <span id="tinggiAir"></span>
        </div>
        <div class="tombol">
            <a href="/setup">Setting Up</a>
        </div>
        <div class="tombol">
            <a onclick="alert('apakah ingin menghapus data?')" href="/?reset=true">Reset</a>
            <a href="/table">Download</a>
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
  setTimeout('process()',500);
}

function handleServerResponse(){
 if(xmlHttp.readyState==4 && xmlHttp.status==200){
   xmlResponse=xmlHttp.responseXML;

   xmldoc = xmlResponse.getElementsByTagName('rYear');
   message = xmldoc[0].firstChild.nodeValue;
   document.getElementById('tahun').innerHTML=message;

   xmldoc = xmlResponse.getElementsByTagName('rMonth');
   message = xmldoc[0].firstChild.nodeValue;
   document.getElementById('bulan').innerHTML=message;

   xmldoc = xmlResponse.getElementsByTagName('rDay');
   message = xmldoc[0].firstChild.nodeValue;
   document.getElementById('tanggal').innerHTML=message;

   xmldoc = xmlResponse.getElementsByTagName('rHour');
   message = xmldoc[0].firstChild.nodeValue;
   document.getElementById('jam').innerHTML=message;

   xmldoc = xmlResponse.getElementsByTagName('rMinute');
   message = xmldoc[0].firstChild.nodeValue;
   document.getElementById('menit').innerHTML=message;
  
   xmldoc = xmlResponse.getElementsByTagName('tinggiair');
   message = xmldoc[0].firstChild.nodeValue;
   document.getElementById('tinggiAir').innerHTML=message;
 }
}
</script>
</html>
)=====";
