String message, XML;
char datatable[] PROGMEM = R"=====(
<!DOCTYPE html>
<html lang="en">

<head>
    <title>Data ketinggian Air</title>
<style>
    body {
        margin: 0;
        background-color: rgb(209, 207, 207);
        display: flex;
        justify-content: center;
    }

    .container {
        width: 450px;
        padding: 20px;
        height: 100vh;
        background-color: whitesmoke;
    }
    #isi {
      margin: 20px
    }
</style>
</head>
<body onload="process()">
  <div class="container">
    <button onclick="window.print()">Download</button>
    <div id="isi"></div>
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

   xmldoc = xmlResponse.getElementsByTagName('dataKetinggian');
   message = xmldoc[0].firstChild.nodeValue;
   document.getElementById('isi').innerHTML=message;
 }
}

</script>
</html>
)=====";
