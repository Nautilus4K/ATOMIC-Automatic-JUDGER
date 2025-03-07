fetch("/api/getbroadinfo", {
    method: "GET",
    headers: {
        "TOKEN": getCookie("token")
    }
}).then(response => response.json()).then((json) => {
    if (json["username"] != "") {
        document.getElementById("info_name").textContent = "Tên: " + json["fullname"];
        document.getElementById("info_class").textContent = "Lớp: " + json["class"];
        
        if (json["priv"] == false) {
            document.getElementById("info_priv").textContent = "Riêng tư: Không";
        } else {
            document.getElementById("info_priv").textContent = "Riêng tư: Có";
        }
    }
})