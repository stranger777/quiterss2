/**
 * Popup UI — shows keyboard shortcuts and toggles pick mode.
 */

const toggleBtn = document.getElementById("toggle-btn");

// Query active tab and send toggle message
toggleBtn.addEventListener("click", () => {
  chrome.tabs.query({ active: true, currentWindow: true }, (tabs) => {
    if (tabs[0] && tabs[0].id) {
      chrome.tabs.sendMessage(tabs[0].id, { action: "toggle" });
      window.close();
    }
  });
});
