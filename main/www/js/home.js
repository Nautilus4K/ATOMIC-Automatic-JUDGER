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

        document.getElementById("info_amount").textContent = "Số bài đã làm: " + json["contestAmount"];
    }
})

fetch("/api/getcontests", {
    method: "GET",
    headers: {
        "TOKEN": getCookie("token")
    }
}).then(response => response.json()).then((json) => { 
    if (json["success"]) {
        // console.log(json["contests"]);
        var innerHTML = ""
        for (let contest of json["contests"]) {
            let contest_name = contest[0]
            let contest_score = contest[1]

            // For each contests. We will have this little string for the innerHTML
            // Each contest would be in the form of a DIV
            // Heres how it looks like:
            // <a><div class="contestform">
            //     <p class="name">contest_name</p>
            //     <p class="score"></p>
            // </div></a>

            innerHTML += "<a href=\"/contest/"+ contest_name + "\">" +
                        "<div class=\"contestform\">"
                        // <p class=\"name\">" + contest_name + </p>;
            innerHTML += "<p class=\"name\">" + contest_name + "</p>";
            innerHTML += "<p class=\"score\">[" + 
                        contest_score.toPrecision(2) + "]</p><p class=\"status\"></div>";
        }
        document.getElementById("list").innerHTML = innerHTML;
    }
})
