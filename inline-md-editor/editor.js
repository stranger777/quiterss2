/**
 * InlineEditor — creates an inline Markdown editor for any DOM node.
 * Original HTML is captured immediately and can be toggled with Ctrl+O.
 * Auto-applies on blur. Escape to cancel.
 */

class InlineEditor {
  /**
   * @param {HTMLElement} node — The DOM node to edit.
   */
  constructor(node) {
    // Capture ORIGINAL state immediately
    this.node = node;
    this.originalHTML = node.innerHTML;
    this.originalText = node.textContent.trim();
    this.originalAttributes = {};
    for (const attr of node.attributes) {
      this.originalAttributes[attr.name] = attr.value;
    }

    // Convert to Markdown
    this.mdContent = InlineMD.htmlToMarkdown(this.originalHTML);
    this.showingOriginal = false;

    // DOM references
    this.container = null;
    this.shadow = null;
    this.textarea = null;
    this.preview = null;
    this.originalEl = null;
    this.toolbar = null;
  }

  /** Open the inline editor. */
  open() {
    this.container = document.createElement("div");
    this.container.className = "md-inline-editor";
    this.node.parentNode.insertBefore(this.container, this.node);

    // Hide the original node while editing
    this.node.style.display = "none";

    // Create Shadow DOM
    this.shadow = this.container.attachShadow({ mode: "open" });

    // Inject content
    this.shadow.innerHTML = this._template();

    // Cache elements
    this.textarea = this.shadow.querySelector("textarea");
    this.preview = this.shadow.querySelector(".preview");
    this.originalEl = this.shadow.querySelector(".original-content");
    this.toolbar = this.shadow.querySelector(".toolbar");

    // Set initial values
    this.originalEl.innerHTML = this.node.outerHTML;
    this.textarea.value = this.mdContent;
    this.textarea.setAttribute("aria-label", "Markdown editor");

    // Update preview
    this._updatePreview();

    // Bind events
    this._bindEvents();

    // Focus textarea
    this.textarea.focus();
  }

  /** Generate Shadow DOM template. */
  _template() {
    return `
      <link rel="stylesheet" href="${chrome.runtime.getURL("styles.css")}">
      <div class="editor-panel">
        <div class="original" hidden>
          <div class="original-label">Оригинал (Ctrl+O)</div>
          <div class="original-content"></div>
        </div>
        <div class="toolbar" role="toolbar" aria-label="Formatting">
          <button data-action="bold" title="Bold (Ctrl+B)"><b>B</b></button>
          <button data-action="italic" title="Italic (Ctrl+I)"><i>I</i></button>
          <button data-action="code" title="Code (Ctrl+\`)"><span>\`</span></button>
          <button data-action="link" title="Link (Ctrl+K)">🔗</button>
          <button data-action="h1" title="Heading (Ctrl+Shift+H)">H</button>
          <button data-action="list" title="List (Ctrl+Shift+L)">•</button>
          <button data-action="quote" title="Quote (Ctrl+Shift+Q)">❝</button>
        </div>
        <textarea class="md-input" spellcheck="false" aria-label="Markdown input"></textarea>
        <div class="preview" aria-label="HTML preview"></div>
        <div class="status-bar">
          <span class="hint">Escape: закрыть · Ctrl+Enter: применить · Ctrl+O: оригинал</span>
        </div>
      </div>
    `;
  }

  /** Bind all event listeners. */
  _bindEvents() {
    // Live preview
    this.textarea.addEventListener("input", () => this._updatePreview());

    // Toolbar clicks
    this.toolbar.addEventListener("click", (e) => {
      const btn = e.target.closest("button[data-action]");
      if (!btn) return;
      e.preventDefault();
      const action = btn.dataset.action;
      const syntax = InlineMD.getActionSyntax(action);
      InlineMD.wrapSelection(this.textarea, syntax.before, syntax.after);
      this._updatePreview();
      this.textarea.focus();
    });

    // Keyboard
    this.textarea.addEventListener("keydown", (e) => this._onKey(e));

    // Auto-apply on blur (click outside)
    this.container.addEventListener("focusout", () => {
      // Delay to check if focus moved outside the editor
      setTimeout(() => {
        if (
          this.shadow &&
          !this.shadow.activeElement &&
          !this.container.contains(document.activeElement)
        ) {
          this.apply();
        }
      }, 100);
    });
  }

  /** Handle keyboard shortcuts. */
  _onKey(e) {
    // Escape — close, discard
    if (e.key === "Escape") {
      e.preventDefault();
      this.destroy();
      return;
    }

    // Ctrl+Enter — apply
    if (e.ctrlKey && e.key === "Enter") {
      e.preventDefault();
      this.apply();
      return;
    }

    // Ctrl+O — toggle original
    if (e.ctrlKey && e.key === "o") {
      e.preventDefault();
      this.toggleOriginal();
      return;
    }

    // Ctrl+B — bold
    if (e.ctrlKey && !e.shiftKey && e.key === "b") {
      e.preventDefault();
      InlineMD.wrapSelection(this.textarea, "**", "**");
      this._updatePreview();
      return;
    }

    // Ctrl+I — italic
    if (e.ctrlKey && !e.shiftKey && e.key === "i") {
      e.preventDefault();
      InlineMD.wrapSelection(this.textarea, "*", "*");
      this._updatePreview();
      return;
    }

    // Ctrl+K — link
    if (e.ctrlKey && e.key === "k") {
      e.preventDefault();
      InlineMD.wrapSelection(this.textarea, "[", "](url)");
      this._updatePreview();
      return;
    }

    // Ctrl+` — code
    if (e.ctrlKey && e.key === "`") {
      e.preventDefault();
      InlineMD.wrapSelection(this.textarea, "`", "`");
      this._updatePreview();
      return;
    }

    // Ctrl+Shift+H — heading
    if (e.ctrlKey && e.shiftKey && e.key === "H") {
      e.preventDefault();
      InlineMD.wrapSelection(this.textarea, "# ", "");
      this._updatePreview();
      return;
    }

    // Ctrl+Shift+L — list
    if (e.ctrlKey && e.shiftKey && e.key === "L") {
      e.preventDefault();
      InlineMD.wrapSelection(this.textarea, "- ", "");
      this._updatePreview();
      return;
    }

    // Ctrl+Shift+Q — quote
    if (e.ctrlKey && e.shiftKey && e.key === "Q") {
      e.preventDefault();
      InlineMD.wrapSelection(this.textarea, "> ", "");
      this._updatePreview();
      return;
    }
  }

  /** Update the HTML preview. */
  _updatePreview() {
    if (this.preview) {
      this.preview.innerHTML = InlineMD.markdownToHTML(this.textarea.value);
    }
  }

  /** Toggle between Markdown and original view. */
  toggleOriginal() {
    this.showingOriginal = !this.showingOriginal;
    const originalDiv = this.shadow.querySelector(".original");

    if (this.showingOriginal) {
      originalDiv.hidden = false;
      this.textarea.value = this.originalText;
      this.textarea.classList.add("original-mode");
      this.textarea.setAttribute("readonly", "");
    } else {
      originalDiv.hidden = true;
      this.textarea.value = this.mdContent;
      this.textarea.classList.remove("original-mode");
      this.textarea.removeAttribute("readonly");
    }
  }

  /** Apply changes: convert MD → HTML, replace original node. */
  apply() {
    if (this.showingOriginal) {
      this.destroy();
      return;
    }

    const md = this.textarea.value;
    const html = InlineMD.markdownToHTML(md);

    // Restore original attributes
    Object.entries(this.originalAttributes).forEach(([key, val]) => {
      this.node.setAttribute(key, val);
    });

    this.node.innerHTML = html;
    this.destroy();
  }

  /** Reset to original HTML. */
  reset() {
    this.node.innerHTML = this.originalHTML;
    Object.entries(this.originalAttributes).forEach(([key, val]) => {
      this.node.setAttribute(key, val);
    });
    this.destroy();
  }

  /** Remove the editor, show the node. */
  destroy() {
    if (this.container) {
      this.container.remove();
    }
    this.node.style.display = "";
    this.shadow = null;
    this.container = null;
    this.textarea = null;
  }
}

if (typeof module !== "undefined" && module.exports) {
  module.exports = InlineEditor;
}
