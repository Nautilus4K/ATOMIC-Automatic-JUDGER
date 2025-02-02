function setCookie(cname, cvalue, exdays) {
    const d = new Date();
    d.setTime(d.getTime() + (exdays*24*60*60*1000));
    let expires = "expires="+ d.toUTCString();
    document.cookie = cname + "=" + cvalue + ";" + expires + ";path=/";
}

function getCookie(cname) {
    let name = cname + "=";
    let ca = document.cookie.split(';');
    for(let i = 0; i < ca.length; i++) {
      let c = ca[i];
      while (c.charAt(0) == ' ') {
        c = c.substring(1);
      }
      if (c.indexOf(name) == 0) {
        return c.substring(name.length, c.length);
      }
    }
    return "";
}

// In case user already logged in, move back to homescreen
if (getCookie("token") != "")
    window.location.href = "/"

// Login the user
function login() {
    // Take username and password inputs
    username = document.getElementById("username").value
    password = document.getElementById("password").value

    // Make these borders transparent so that the latter will be less abrupt
    document.getElementById("username").style.border = "1px solid transparent"
    document.getElementById("password").style.border = "1px solid transparent"

    // Element for notifying about errors
    noticeElement = document.getElementById("notice")
    noticeElement.textContent = ""

    // DEBUG
    // console.log("Username: " + username)
    // console.log("Password: " + password)

    // Send API
    fetch('/api/login', {
        "method": "GET",
        "headers": {
            "USERNAME": username,
            "PASSWD": password
        }
    })
    .then((response) => response.json())
    .then((json) => {
        // console.log(json)
        // Logged in successfully?
        if (json["success"]) {
            // If logged in, set cookie and refresh to homepage
            setCookie("token", json["message"], 36500)
            window.location.replace("/")
        }
        else {
            // If we did not log in successfully
            noticeElement.textContent = json["message"]
            document.getElementById("username").style.border = "1px solid var(--fail)"
            document.getElementById("password").style.border = "1px solid var(--fail)"
        }
    })
}