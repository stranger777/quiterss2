/**
 * Inline MD Editor — Content Script
 * Injected into every page. Handles pick mode and global hotkeys.
 */

(() => {
  "use strict";

  let pickMode = false;
  let activeEditor = null;

  /** Enable/disable pick mode. */
  function setPickMode(on) {
    pickMode = on;
    document.body.style.cursor = on ? "crosshair" : "";
    document.body.classList.toggle("md-pick-mode", on);

    // Visual feedback — brief flash on body
    if (on) {
      document.body.classList.add("md-pick-flash");
      setTimeout(() => document.body.classList.remove("md-pick-flash"), 300);
    }
  }

  /** Create an editor for the given node. */
  function openEditorFor(node) {
    if (activeEditor) {
      activeEditor.destroy();
    }
    activeEditor = new InlineEditor(node);
    activeEditor.open();
    setPickMode(false);
  }

  // === Event Listeners ===

  // Keyboard: pick mode toggle
  document.addEventListener("keydown", (e) => {
    // Ctrl+Shift+E — toggle pick mode
    if (e.ctrlKey && e.shiftKey && (e.key === "E" || e.key === "e" || e.key === "У" || e.key === "у")) {
      e.preventDefault();
      e.stopPropagation();
      setPickMode(!pickMode);
      return;
    }

    // If editor is active, let it handle its own keys
    if (activeEditor && activeEditor.shadow) {
      // Keys are handled inside InlineEditor._onKey()
      return;
    }
  });

  // Click: pick element or apply editor
  document.addEventListener(
    "click",
    (e) => {
      if (pickMode) {
        const target = e.target.closest(
          "p, div, article, section, h1, h2, h3, h4, h5, h6, li, blockquote, pre, span, td, th"
        );

        // Don't pick our own editor
        if (!target || target.closest(".md-inline-editor")) return;

        e.preventDefault();
        e.stopPropagation();
        openEditorFor(target);
        return;
      }

      // If clicking outside active editor, apply
      if (activeEditor && !e.target.closest(".md-inline-editor")) {
        activeEditor.apply();
      }
    },
    true // Capture phase
  );

  // Message from popup or background
  if (typeof chrome !== "undefined" && chrome.runtime && chrome.runtime.onMessage) {
    chrome.runtime.onMessage.addListener((msg) => {
      if (msg.action === "toggle") {
        setPickMode(!pickMode);
      }
    });
  }

  // Expose for testing
  if (typeof window !== "undefined") {
    window.__InlineMD = {
      get pickMode() { return pickMode; },
      setPickMode,
      get activeEditor() { return activeEditor; },
      openEditorFor,
    };
  }
})();
