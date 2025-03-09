// Initialize Ace Editor on the #editor div
var editor = ace.edit("editor");
editor.setTheme("ace/theme/dynamic");
// Set default language mode to C++ (ace/mode/c_cpp)
editor.session.setMode("ace/mode/c_cpp");
editor.setFontSize("18px");
editor.setShowPrintMargin(false)
editor.session.setUseWrapMode(true);

let lang = "cpp";

// Function to switch the language mode of the editor
function switchMode(lang) {
  // Remove "active" class from all buttons
  var buttons = document.querySelectorAll('.languages button');
  buttons.forEach(function(btn) {
    btn.classList.remove('active');
  });
  // Add "active" class to the clicked button
  var activeButton = document.querySelector(`.languages button[onclick="switchMode('${lang}')"]`);
  if (activeButton) {
    activeButton.classList.add('active');
  }
  
  // Determine the mode based on the language parameter
  var mode;
  switch (lang) {
    case 'cpp':
      mode = "ace/mode/c_cpp";
      lang = "cpp";
      break;
    case 'pas':
      mode = "ace/mode/pascal";
      lang="pas";
      break;
    case 'py':
      mode = "ace/mode/python";
      lang="py";
      break;
    default:
      mode = "ace/mode/plain_text";
      lang="txt";
  }
  // Set the editor's mode accordingly
  editor.session.setMode(mode);
}

function submit(contestName) {
  const value = editor.getValue();
  console.log(value)
  // console.log(value)
  
  const noticeElement = document.getElementById("notice");
  if (value == "" || !value) {
    noticeElement.textContent = "Bài làm trống";
    noticeElement.style.display = "block";
    noticeElement.style.color = "var(--dangerous)";
  } else {
    fetch("/api/submitcode", {
      method: "POST",
      headers: {
        "TOKEN": getCookie("token"),
        "LANG": lang,
        "CONT": contestName
      },
      body: value
    }).then(response => response.json()).then((json) => {
      if (json["success"]) {
        noticeElement.textContent = "Nộp bài thành công";
        noticeElement.style.display = "block";
        noticeElement.style.color = "var(--primary)";
      } else {
        noticeElement.textContent = "Lỗi: " + json["message"];
        noticeElement.style.display = "block";
        noticeElement.style.color = "var(--dangerous)";
      }
    })
  }
}

// When out of focus, hide notify element
addEventListener("keydown", (event) => {
  if (editor.isFocused()) {
    const noticeElement = document.getElementById("notice");
    noticeElement.style.display = "none";
  }
});