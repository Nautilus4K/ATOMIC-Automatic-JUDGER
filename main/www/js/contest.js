// Initialize Ace Editor on the #editor div
var editor = ace.edit("editor");
editor.setTheme("ace/theme/dynamic");
// Set default language mode to C++ (ace/mode/c_cpp)
editor.session.setMode("ace/mode/c_cpp");
editor.setFontSize("16px");
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
      break;
    case 'pas':
      mode = "ace/mode/pascal";
      break;
    case 'py':
      mode = "ace/mode/python";
      break;
    default:
      mode = "ace/mode/plain_text";
  }
  // Set the editor's mode accordingly
  editor.session.setMode(mode);
}