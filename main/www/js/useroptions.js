document.getElementById("file-upload").addEventListener("change", function(event) {
    const file = event.target.files[0]; // Get the uploaded file
    if (file) {
        //   console.log("File uploaded:", file.name);
        //   console.log("File type:", file.type);
        uploadProfilePicture(file.type);
    }
});

function uploadProfilePicture(filetype) {
    if (filetype == "image/jpeg") filetype = "jpeg";
    else filetype = "png";
    console.log(filetype);

    
}
