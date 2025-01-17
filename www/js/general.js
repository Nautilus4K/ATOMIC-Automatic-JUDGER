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
console.log("DarkMode? " + darkMode)

if (darkMode) {
  document.documentElement.classList.add('dark');
}
// else {
//     document.documentElement.classList.toggle('light');
//     document.getElementById("darklight_img").src = "/assets/light.png"
// }