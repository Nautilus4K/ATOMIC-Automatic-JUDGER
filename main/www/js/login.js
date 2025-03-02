const session_token = getCookie("token")
if (session_token != "") window.location.href = "/"

const notifyLabel = document.getElementById("label");

// Set the notify label hidden first
notifyLabel.style.display = "none";

function login() {

    // Get username and password the user entered
    const username = document.getElementById("username").value;
    const password = document.getElementById("password").value;

    // GET request -> server to check if the username and password is correct
    // if so, it will return a session token.
    fetch("/api/login", {
        method: "GET",
        headers: {
            "USERNAME": username,
            "PASSWD": password
        }
    }).then (response => response.json()).then((json) => {
        if (json["success"] == true) {
            setCookie("token", json["message"], 365);
            window.location.href = "/";
        } else {
            notifyLabel.style.display = "block";
            notifyLabel.textContent = json["message"];
        }
    })
}

addEventListener("keydown", (event) => {
    if (event.key == "Enter" && document.activeElement.id == "username") {
        document.getElementById("password").focus();
    } else if (event.key == "Enter" && document.activeElement.id == "password") {
        login();
    } else {
        console.log(event.key + " --- " + document.activeElement.id)
    }
});