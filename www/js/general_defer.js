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

// 0 -> Light
// 1 -> Dark
var displayModeValue = getCookie("displayMode")
if (displayModeValue == "0" || displayModeValue == "") var darkMode = false
else var darkMode = true

if (darkMode) document.getElementById("darklight_img").src = "/assets/dark.png"

function switchMode() {
    console.log("Switch dark mode from " + darkMode + " to " + !darkMode)
    darkMode = !darkMode
    if (darkMode) { 
        var targetMode = "1"
        document.documentElement.classList.add('dark');
        document.getElementById("darklight_img").src = "/assets/dark.png"
    }
    else {
        var targetMode = "0"
        document.documentElement.classList.remove('dark');
        document.getElementById("darklight_img").src = "/assets/light.png"
    }
    setCookie("displayMode", targetMode, 3650)
    // document.location.href = document.location.href
}
