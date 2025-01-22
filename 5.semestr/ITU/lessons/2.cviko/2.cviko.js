/***
* XMLHttpRequest object constructor (for compatibility with various browsers)
*/

const READY_STATE_FINISHED = 4;
const STATUS_OK = 200;
const URL = "https://pckiss.fit.vutbr.cz/itu/api.php";
const login = "xmicha94";
let lastMessageID = 0;

function createXmlHttpRequestObject() 
{
    var request;

    try
    {
        request = new XMLHttpRequest(); // should work on all browsers except IE6 or older
    } 
    catch (e) 
    { 
        try 
        {
            request = new ActiveXObject("Microsoft.XMLHttp"); // browser is IE6 or older
        }
        catch (e) 
        {
            // ignore error
        }
    }

    if (!request) 
    {
        alert("Error creating the XMLHttpRequest object.");
    } 
    else 
    {
        return request;
    }
}

function uploadData()
{
    document.getElementById("status").innerHTML = "uploadData()";

    try 
    {
        var request = createXmlHttpRequestObject(); // stores XMLHttpRequestObject
        request.open("POST", URL, true);
        request.onreadystatechange = downloadData;
        request.setRequestHeader("Content-Type", "application/x-www-form-urlencoded");
        var pole = document.getElementById("newMessageString").value;
        request.send("data=" + pole + "&user=" + login);
    } 
    catch (e) 
    {
        alert(e.toString());
    }

    return false; // to avoid default form submit behavior 
}

function downloadData() 
{
    document.getElementById("status").innerHTML = "downloadData()";

    try 
    {
        var request = createXmlHttpRequestObject(); // stores XMLHttpRequestObject
        request.open("GET", URL, true);
        request.onreadystatechange = function() // anonymous function (a function without a name).
        {
            if ((request.readyState == READY_STATE_FINISHED) && (request.status == STATUS_OK)) // process is completed and http status is OK
            {
                var pole = JSON.parse(request.responseText);
                for (var i in pole)
                {
                    let messageID = parseInt(pole[i].id);
                    if (messageID > lastMessageID) {
                        document.getElementById("chatArea").innerHTML += pole[i].dts.split(' ')[1] + " " + pole[i].login + ": " + pole[i].cnt + "<br>"; 
                        lastMessageID = messageID;
                    }
                }
            }
        }
        request.send(null);
    } 
    catch (e) 
    {
        alert(e.toString());
    }
}

setInterval(downloadData, 1000);