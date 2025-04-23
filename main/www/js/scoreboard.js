/* 
<!-- <div class="class">
<h1>CLASSS_NAME</h1>
<div class="table">
    <div class="headers">
        <p></p>
        <p>User1</p>
        <p>User2</p>
        <p>User3</p>
        <p>User4</p>
        <p>User5</p>
    </div>
    <div class="content">
        <p>CONTEST_1</p>
        <p>1.0</p>
        <p>2.0</p>
        <p>3.0</p>
        <p>4.0</p>
        <p>5.0</p>
    </div>
    <div class="content">
        <p>CONTEST_2</p>
        <p>1.0</p>
        <p>2.0</p>
        <p>3.0</p>
        <p>4.0</p>
        <p>5.0</p>
    </div>
</div>
</div>

<div class="class">
<h1>CLASSS_NAME</h1>
<div class="table">
    <div class="headers">
        <p></p>
        <p>User1</p>
        <p>User2</p>
        <p>User3</p>
        <p>User4</p>
        <p>User5</p>
    </div>
    <div class="content">
        <p>CONTEST_1</p>
        <p>1.0</p>
        <p>2.0</p>
        <p>3.0</p>
        <p>4.0</p>
        <p>5.0</p>
    </div>
</div>
</div>
*/

// Convert object to array of entries, sort it, and convert back to object
function sortByTotalScores(studentData) {
    // 1. Convert to array of [key, value] pairs
    const entries = Object.entries(studentData);
    
    // 2. Sort based on sum of scores
    entries.sort((a, b) => {
        // Calculate total for student a
        const sumA = Object.values(a[1]).reduce((sum, score) => sum + score, 0);
        // Calculate total for student b
        const sumB = Object.values(b[1]).reduce((sum, score) => sum + score, 0);
        
        // Sort in descending order
        return sumB - sumA;
    });
    
    // 3. Convert back to object
    return Object.fromEntries(entries);
}

function refreshScoreboard() {
    // console.log("Refreshing scoreboard!")
    fetch("/api/getscore", {
        method: "GET",
        headers: {
            "TOKEN": getCookie("token")
        }
    }).then(response => response.json()).then((json) => {
        // console.log(json);
        if (json["success"]) {
            // If we successfully got the score data. We put it into the format described at the start
            classes = Object.keys(json["contestslist"]);
            // console.log(classes)

            // Clean up
            var oldElement = document.getElementById("scoreboardElement");
            while (oldElement) {
                oldElement.remove()
                oldElement = document.getElementById("scoreboardElement");
            }


            // Now with classes on our hands, we should go one by one with forEach
            classes.forEach(_class => {
                // _class is the string of the class
                json["result"][_class] = sortByTotalScores(json["result"][_class]);
                // First, we create a new div representing the class's area
                const containerElement = document.createElement("div");
                containerElement.className = "class";
                containerElement.id = "scoreboardElement"; // This is added in to make it easier to delete the things later on.
                document.getElementById("scoreArea").appendChild(containerElement);

                // Okay, now we add the class label
                const labelElement = document.createElement("h1");
                labelElement.id = "scoreboardElement";
                labelElement.textContent = _class;
                containerElement.appendChild(labelElement);

                // Now moving to the table
                const tableElement = document.createElement("div");
                tableElement.id = "scoreboardElement";
                tableElement.className = "table";
                containerElement.appendChild(tableElement);

                // Next step is the number. A ranking without a number is not good
                const numbersElement = document.createElement("div");
                numbersElement.id = "scoreboardElement";
                numbersElement.className = "headers";
                // In the headers, there is always an empty element first, and the other comes later
                numbersElement.innerHTML = "<p id=\"scoreboardElement\"><strong>#</strong></p>"
                tableElement.appendChild(numbersElement);
                
                users = Object.keys(json["result"][_class]);
                for(let i = 1; i <= users.length; i++) {
                    const numBox = document.createElement("p");
                    numBox.id = "scoreboardElement";
                    numBox.textContent = i;
                    numbersElement.appendChild(numBox);
                }

                // Now we need to make a header. This would be something like a list of all users, I guess
                const headersElement = document.createElement("div");
                headersElement.id = "scoreboardElement";
                headersElement.className = "headers";
                // In the headers, there is always an empty element first, and the other comes later
                headersElement.innerHTML = "<p id=\"scoreboardElement\"><strong>Tên</strong></p>"
                tableElement.appendChild(headersElement);

                // Casually*, adds the users into the headers.
                let userPosition = 0;
                users.forEach(user => {
                    userPosition++;
                    const userBox = document.createElement("a");
                    userBox.id = "scoreboardElement";
                    userBox.textContent = user;
                    userBox.href = "/user/" + user;
                    headersElement.appendChild(userBox);

                    switch(userPosition) {
                        case 1:
                            userBox.style.color = "var(--score-first)";
                            break;
                        case 2:
                            userBox.style.color = "var(--score-second)";
                            break;
                        case 3:
                            userBox.style.color = "var(--score-third)";
                            break;
                    }
                })

                // Add the Total column header
                const totalHeaderElement = document.createElement("div");
                totalHeaderElement.id = "scoreboardElement";
                totalHeaderElement.className = "headers";
                totalHeaderElement.innerHTML = "<p id=\"scoreboardElement\"><strong>Tổng</strong></p>";
                tableElement.appendChild(totalHeaderElement);
                
                // Calculate and add total scores for each user
                users.forEach(user => {
                    const totalScore = Object.values(json["result"][_class][user]).reduce((sum, score) => sum + (score || 0), 0);
                    const totalScoreBox = document.createElement("p");
                    totalScoreBox.id = "scoreboardElement";
                    totalScoreBox.textContent = totalScore.toFixed(1);
                    totalScoreBox.style.fontWeight = "bold";
                    totalHeaderElement.appendChild(totalScoreBox);
                });

                // Onto the next one, we should make it for each contest in the class being given
                contestsList = json["contestslist"][_class];
                contestsList.forEach(contest => {
                    const contestElement = document.createElement("div");
                    contestElement.id = "scoreboardElement";
                    contestElement.className = "content";
                    tableElement.appendChild(contestElement);

                    const contestName = document.createElement("p");
                    contestName.id = "scoreboardElement";
                    contestName.className = "header";
                    // contestName.className = ""
                    contestName.textContent = contest;
                    contestElement.appendChild(contestName);

                    users.forEach(user => {
                        const userScore = document.createElement("p");
                        userScore.id = "scoreboardElement";
                        if (json["result"][_class][user][contest])
                            userScore.textContent = json["result"][_class][user][contest];
                        else {
                            userScore.textContent = "-";
                            userScore.style.color = "var(--text-deactive)";
                        }
                        contestElement.appendChild(userScore);
                    })
                })
            });

            // This will be refreshed occasionally
            setTimeout(() => {
                refreshScoreboard();
            }, 30000) // Timeout of 30 seconds. So every 30 seconds the function is gonna be called once using recursive.
        } else {
            alert("Lỗi: " + json["message"]);
            window.location.href = "/";
        }
    });
}

refreshScoreboard();