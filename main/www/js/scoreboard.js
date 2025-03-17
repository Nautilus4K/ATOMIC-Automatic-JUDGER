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

// First, let's create a function that will sort students within each section
function sortStudentsByTotal(data) {
    // Process each section (D0, A0, etc.)
    for (const section in data) {
        const students = data[section];
        
        // Convert the students object to array of [studentName, studentData] pairs
        const studentEntries = Object.entries(students);
        
        // Sort the array based on sum of scores
        studentEntries.sort((a, b) => {
            // Calculate total score for student a
            const totalA = Object.values(a[1]).reduce((sum, score) => sum + score, 0);
            // Calculate total score for student b
            const totalB = Object.values(b[1]).reduce((sum, score) => sum + score, 0);
            
            // Sort in descending order
            return totalB - totalA;
        });
        
        // Convert back to object with new order
        data[section] = Object.fromEntries(studentEntries);
    }
    
    return data;
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
            // Example response json:

            // Example json
            json = {
                "api_version": "0.3.1", 
                "success": true, 
                "message": "", 
                "result": {
                    "D0": {
                        "Student1": {
                            "Test1": 5.0, 
                            "SumOfAB": 10.0
                        },
                        "Student2": {
                            "Test1": 10.0,
                            "Test2": 2.0,
                        }
                    }, 
                    "A0": {
                        "Student1": {
                            "Test1": 5.0
                        },
                        "Student3": {
                            "Test1": 2.0,
                            "Test2": 10.0
                        }
                    }
                }, 
                "contestslist": {
                    "D0": ["Test1", "Test2", "SumOfAB"], 
                    "A0": ["Test1", "Test2"]
                }
            } 

            json = sortStudentsByTotal(json); // So yeah, let's just sort out the json object first
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
                users.forEach(user => {
                    const userBox = document.createElement("p");
                    userBox.id = "scoreboardElement";
                    userBox.textContent = user;
                    headersElement.appendChild(userBox);
                })

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