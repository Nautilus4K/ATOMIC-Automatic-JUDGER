const token = getCookie("token");  // Get the session token

document.getElementById("file-upload").addEventListener("change", function(event) {
    const file = event.target.files[0]; // Get the uploaded file
    if (file) {
        convertFileToBase64(file);
    }
    // Reset the input to allow the same file to be selected again
    event.target.value = "";  // Clear the input value after the file is uploaded
});

function convertFileToBase64(file) {
    // Create a FileReader to read the file as base64
    const reader = new FileReader();

    reader.onloadend = function() {
        // The result is the base64-encoded string
        const base64String = reader.result.split(',')[1]; // Remove the "data:image/jpeg;base64," part

        // Now send the base64 string to the server
        uploadProfilePicture(base64String);
    };

    // Read the file as a data URL (base64-encoded string)
    reader.readAsDataURL(file);
}

function uploadProfilePicture(base64String) {
    // Prepare the data to send to the server
    let filetype = base64String.startsWith("data:image/jpeg") ? "jpeg" : "png"; // You can check for other file types

    // // Create a JSON object with the base64 string
    // const data = {
    //     profilepic: base64String,  // Send the base64 string of the image
    //     filetype: filetype,        // Send the file type (jpeg or png)
    // };

    // Send the base64 string to the server using Fetch API
    fetch('/api/uploadprofilepic', {
        method: 'POST',
        headers: {
            'TOKEN': token,  // Include the session token
        },
        body: JSON.stringify(base64String),  // Send the base64-encoded data in the body
    })
    .then(response => response.json())
    .then(json => {
        if (json.success) {
            alert("Thành công tải ảnh.");
            window.location.href = window.location.href;
        } else {
            alert("Lỗi: " + json.message);
        }
    })
    .catch(error => {
        console.error('Error uploading profile picture:', error);
        alert("An error occurred during the upload.");
    });
}

// Set the display of the profile picture
fetch("/api/getfullinfo", {
    method: "GET",
    headers: {
        "TOKEN": token
    }
}).then(response => response.json()).then((json) => {

    // Profile picture
    if (json["picture"] == true) {
        // document.getElementById("userpic").src = "/userpictures/"+json["username"]+".jpg"
        document.getElementById("currentuserprofilepic").src = "/userpictures/"+json["username"]+".jpg";
        document.getElementById("displayname").value = json["fullname"];
        document.getElementById("desc").value = json["desc"];

        if (json["username"] == "ErrorHandling2␀") {
            window.location.href = window.location.href;
        }
    } else {
        console.error("Error. Cannot find picture???")
        document.getElementById("currentuserprofilepic").src = "/assets/defaultuser.png";
        document.getElementById("displayname").value = json["fullname"];
        document.getElementById("desc").value = json["desc"];

        if (json["username"] == "ErrorHandling2␀") {
            window.location.href = window.location.href;
        }
        // window.location.href = "/";
    }
})

function updateDetails() {
    const name = document.getElementById("displayname").value;
    const desc = document.getElementById("desc").value;

    fetch("/api/updatedetails", {
        method: "GET",
        headers: {
            "TOKEN": token,
            "NAME": name,
            "DESC": desc
        }
    }).then(response => response.json()).then((json) => {
        if (json["success"]) {
            const detailsnoticeobj = document.getElementById("detailsnotice");
            detailsnoticeobj.style.display = "block";
            detailsnoticeobj.style.color = "var(--primary)";
            detailsnoticeobj.textContent = "Cập nhật dữ liệu thành công";
        }
        else {
            const detailsnoticeobj = document.getElementById("detailsnotice");
            detailsnoticeobj.style.display = "block";
            detailsnoticeobj.style.color = "var(--dangerous)";
            detailsnoticeobj.textContent = "Lỗi: " + json["message"];
        }
    })
}

// Make sure that anytime the users is typing the details to update them, the old
// notification label will be hidden
addEventListener("keydown", (event) => {
    if (document.activeElement.id == "displayname" || document.activeElement.id == "desc") {
        const detailsnoticeobj = document.getElementById("detailsnotice");
        detailsnoticeobj.style.display = "none";
    } else if (document.activeElement.id == "oldpass" || document.activeElement.id == "newpass" || document.activeElement.id == "newpassre") {
        const passwdnoticeobj = document.getElementById("passwdnotice");
        passwdnoticeobj.style.display = "none";
    }
});

function updatePassword() {
    const oldpasswd = document.getElementById("oldpass").value;
    const newpasswd = document.getElementById("newpass").value;
    const newpasswdre = document.getElementById("newpassre").value;

    const notifyObj = document.getElementById("passwdnotice");

    if (newpasswd != newpasswdre) {
        notifyObj.style.display = "block";
        notifyObj.style.color = "var(--dangerous)";
        notifyObj.textContent = "Mật khẩu không trùng khớp!";
    } else {
        // If all client-side checks are done, we move on to the more network-dependent checks
        // Now its all APIs
        fetch("/api/setpasswd", {
            method: "GET",
            headers: {
                "TOKEN": token,
                "OLDPASS": oldpasswd,
                "NEWPASS": newpasswd
            }
        }).then(response => response.json()).then((json) => {
            console.log(json);
            if (json["success"]) {
                notifyObj.style.display = "block";
                notifyObj.style.color = "var(--primary)";
                notifyObj.textContent = "Cập nhật mật khẩu thành công";
            } else {
                // If an error happened
                notifyObj.style.display = "block";
                notifyObj.style.color = "var(--dangerous)";
                notifyObj.textContent = json["message"];
            }
        })
    }
}