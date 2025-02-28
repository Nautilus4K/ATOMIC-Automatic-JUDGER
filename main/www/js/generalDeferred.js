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

function toggleUser() {
    // console.log("User button clicked");
    userOptionsShown = !userOptionsShown;

    var buttons = document.querySelectorAll(".dropdownbtn");  // Get all matching elements
    document.querySelector(".dropdown").classList.toggle("show")
    buttons.forEach(button => {
        button.classList.toggle("show")
    });
}