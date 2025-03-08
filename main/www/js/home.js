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
            let contest_locked = contest[1]
            let contest_score = contest[2]

            // For each contests. We will have this little string for the innerHTML
            // Each contest would be in the form of a DIV
            // Heres how it looks like:
            // <a><div class="contestform">
            //     <p class="name">contest_name</p>
            //     <p class="score"></p>
            //     <p class="status">contest_locked</p>
            // </div></a>

            if (contest_locked) contest_locked = "KHÓA";
            else contest_locked = "";

            innerHTML += "<a href=\"/contest/"+ contest_name + "\">" +
                        "<div class=\"contestform\">"
                        // <p class=\"name\">" + contest_name + </p>;
            if (contest_locked) innerHTML += "<p class=\"namedeactive\">" + contest_name + "</p>";
            else innerHTML += "<p class=\"name\">" + contest_name + "</p>";
            innerHTML += "<p class=\"score\">[" + 
                        contest_score.toPrecision(2) + "]</p><p class=\"status\">" +
                        contest_locked + "</p></div></a>";
        }
        document.getElementById("list").innerHTML = innerHTML;
    }
})
