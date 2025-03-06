// Function for setting cookies
// Also can be used to delete cookies by overwriting with a ghost
// cookie with the exdays of 0
function setCookie(cname, cvalue, exdays) {
    const d = new Date();
    d.setTime(d.getTime() + (exdays*24*60*60*1000));
    let expires = "expires="+ d.toUTCString();
    document.cookie = cname + "=" + cvalue + ";" + expires + ";path=/";
}

// Getting cookies. s1mple as that.
function getCookie(cname) {
    // "a"
    let name = cname + "=";
    let decodedCookie = decodeURIComponent(document.cookie);
    let ca = decodedCookie.split(';');
    for(let i = 0; i <ca.length; i++) {
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

var darkmode = getCookie("darkmode")
if (darkmode == "0") {
    // If cookie is not set or set to light
    document.documentElement.classList.remove("dark")
} else if (window.matchMedia && window.matchMedia('(prefers-color-scheme: dark)').matches && (darkmode == "" || !darkmode)) {
    setCookie("darkmode", "1", 365)
    document.documentElement.classList.add("dark")
} else if (darkmode != "1"){
    setCookie("darkmode", "0", 365)
    document.documentElement.classList.remove("dark")
} else {
    document.documentElement.classList.add("dark")
}