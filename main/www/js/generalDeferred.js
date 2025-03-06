// Light and dark mode stuffs
if (darkmode == "" || !darkmode || darkmode == "0") {
    // If cookie is not set, we default to light mode
    document.getElementById("darklighticon").src = "/assets/light.png"
} else {
    document.getElementById("darklighticon").src = "/assets/dark.png"
}

function modeToggle() {
    document.documentElement.classList.toggle("dark")

    // Check if the darkmode cookie is something, we change it to anothers
    // The cookie would exists for estimated time of 1 years
    const darkmodeState = getCookie("darkmode")
    // console.log(darkmodeState)

    if (darkmodeState == "0" || darkmodeState == "" || !darkmodeState) {
        // To dark mode
        setCookie("darkmode", "1", 365) // -> Dark mode
        document.getElementById("darklighticon").src = "/assets/dark.png"
    } else {
        setCookie("darkmode", "0", 365) // -> Light mode
        document.getElementById("darklighticon").src = "/assets/light.png"
    }
}

// User buttons
let userOptionsShown = false;  // Ensure this is declared outside
var buttons = document.querySelectorAll(".dropdownbtn");  // Get all matching elements

function toggleUser() {
    // console.log("User button clicked");
    userOptionsShown = !userOptionsShown;

    document.querySelector(".dropdown").classList.toggle("show")
    buttons.forEach(button => {
        button.classList.toggle("show")
    });
}


// Hide dropdown if user clicks anywhere else
document.addEventListener("click", function(event) {
    buttons.forEach(button => {
        if (!button.contains(event.target) && 
            button.classList.contains("show") && 
            !document.getElementById("userbtn").contains(event.target)) 
        {
            button.classList.remove("show")
            document.getElementById("dropdownmenu").classList.remove("show")
        }
    })
});

// User account manager
const session_token = getCookie("token")
if ((session_token == "" || !session_token) && window.location.href != "/login") window.location.href = "/login"
else {
    fetch("/api/getbroadinfo", {
        method: "GET",
        headers: {
            "TOKEN": session_token
        }
    }).then(response => response.json()).then((json) => {
        if (json["username"] != "") {
            if (window.location.href == "/") {
                document.getElementById("info_name").textContent = "Tên: " + json["fullname"]
                document.getElementById("info_class").textContent = "Lớp: " + json["class"]
                
                if (json["priv"] == false) {
                    document.getElementById("info_priv").textContent = "Riêng tư: Không"
                } else {
                    document.getElementById("info_priv").textContent = "Riêng tư: Có"
                }
            }

            document.getElementById("usersettings").textContent = json["fullname"]
            document.getElementById("profilepage").href = "/user/"+json["username"]

            // Profile picture
            if (json["picture"] == true) {
                document.getElementById("userpic").src = "/userpictures/"+json["username"]+".jpg"
            }
        } else {
            setCookie("token", "", 0)
            window.location.href = "/login"
        }
    })
}

// Handles user sign out of account
function signout() {
    fetch("/api/delsession", {
        headers: {
            "TOKEN": session_token
        }
    }).then(response => response.json()).then((json) => {
        setCookie("token", "0", 0)
        window.location.href = "/login"
    })
}