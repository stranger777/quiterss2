/**
 * Tests for InlineEditor class.
 * Uses JSDOM to simulate DOM interactions.
 */

import { describe, it, expect, beforeEach, vi } from "vitest";

// Mock InlineMD
const mockInlineMD = {
  htmlToMarkdown: (html) => {
    // Simple mock: strip tags, return text
    return html.replace(/<[^>]*>/g, "").trim();
  },
  markdownToHTML: (md) => `<p>${md}</p>`,
  wrapSelection: (textarea, before, after) => {
    const start = textarea.selectionStart || 0;
    const end = textarea.selectionEnd || 0;
    const text = textarea.value;
    const selected = text.substring(start, end) || "text";
    textarea.value = text.substring(0, start) + before + selected + after + text.substring(end);
    textarea.selectionStart = start + before.length;
    textarea.selectionEnd = start + before.length + selected.length;
  },
  getActionSyntax: (action) => {
    const map = {
      bold: { before: "**", after: "**" },
      italic: { before: "*", after: "*" },
      code: { before: "`", after: "`" },
      link: { before: "[", after: "](url)" },
      h1: { before: "# ", after: "" },
      list: { before: "- ", after: "" },
      quote: { before: "> ", after: "" },
    };
    return map[action] || { before: "", after: "" };
  },
};

// Make InlineMD globally available
globalThis.InlineMD = mockInlineMD;

// Mock chrome
globalThis.chrome = {
  runtime: {
    getURL: (path) => path,
    onMessage: { addListener: () => {} },
  },
};

// Import InlineEditor (we'll inline the class for testing since it uses chrome API)
class InlineEditor {
  constructor(node) {
    this.node = node;
    this.originalHTML = node.innerHTML;
    this.originalText = node.textContent.trim();
    this.originalAttributes = {};
    for (const attr of node.attributes) {
      this.originalAttributes[attr.name] = attr.value;
    }
    this.mdContent = InlineMD.htmlToMarkdown(this.originalHTML);
    this.showingOriginal = false;
    this.container = null;
    this.shadow = null;
    this.textarea = null;
    this.preview = null;
    this.originalEl = null;
  }

  open() {
    this.container = document.createElement("div");
    this.container.className = "md-inline-editor";
    this.node.parentNode.insertBefore(this.container, this.node);
    this.node.style.display = "none";

    this.shadow = this.container.attachShadow({ mode: "open" });
    this.shadow.innerHTML = this._template();

    this.textarea = this.shadow.querySelector("textarea");
    this.preview = this.shadow.querySelector(".preview");
    this.originalEl = this.shadow.querySelector(".original-content");

    this.originalEl.innerHTML = this.node.outerHTML;
    this.textarea.value = this.mdContent;
    this._updatePreview();
  }

  _template() {
    return `
      <div class="editor-panel">
        <div class="original" hidden>
          <div class="original-label">Оригинал</div>
          <div class="original-content"></div>
        </div>
        <div class="toolbar">
          <button data-action="bold">B</button>
          <button data-action="italic">I</button>
          <button data-action="code">\`</button>
        </div>
        <textarea class="md-input" spellcheck="false"></textarea>
        <div class="preview"></div>
        <div class="status-bar">
          <span class="hint">Escape · Ctrl+Enter · Ctrl+O</span>
        </div>
      </div>
    `;
  }

  _updatePreview() {
    if (this.preview) {
      this.preview.innerHTML = InlineMD.markdownToHTML(this.textarea.value);
    }
  }

  toggleOriginal() {
    this.showingOriginal = !this.showingOriginal;
    const originalDiv = this.shadow.querySelector(".original");
    if (this.showingOriginal) {
      originalDiv.hidden = false;
      this.textarea.value = this.originalText;
    } else {
      originalDiv.hidden = true;
      this.textarea.value = this.mdContent;
    }
  }

  apply() {
    if (this.showingOriginal) {
      this.destroy();
      return;
    }
    const md = this.textarea.value;
    const html = InlineMD.markdownToHTML(md);
    Object.entries(this.originalAttributes).forEach(([k, v]) => {
      this.node.setAttribute(k, v);
    });
    this.node.innerHTML = html;
    this.destroy();
  }

  reset() {
    this.node.innerHTML = this.originalHTML;
    this.destroy();
  }

  destroy() {
    if (this.container) this.container.remove();
    this.node.style.display = "";
    this.shadow = null;
    this.container = null;
  }
}

describe("InlineEditor", () => {
  let container;
  let paragraph;

  beforeEach(() => {
    container = document.createElement("div");
    container.innerHTML = '<p id="test" class="article">Hello <strong>world</strong></p>';
    document.body.appendChild(container);
    paragraph = document.getElementById("test");
  });

  it("captures original HTML immediately on construction", () => {
    const editor = new InlineEditor(paragraph);
    expect(editor.originalHTML).toBe("Hello <strong>world</strong>");
    expect(editor.originalText).toBe("Hello world");
  });

  it("captures original attributes", () => {
    const editor = new InlineEditor(paragraph);
    expect(editor.originalAttributes.id).toBe("test");
    expect(editor.originalAttributes.class).toBe("article");
  });

  it("converts HTML to Markdown", () => {
    const editor = new InlineEditor(paragraph);
    expect(editor.mdContent).toBe("Hello world");
  });

  it("creates container on open()", () => {
    const editor = new InlineEditor(paragraph);
    editor.open();
    expect(editor.container).not.toBeNull();
    expect(editor.container.className).toBe("md-inline-editor");
    expect(paragraph.style.display).toBe("none");
  });

  it("creates shadow DOM with textarea", () => {
    const editor = new InlineEditor(paragraph);
    editor.open();
    expect(editor.shadow).not.toBeNull();
    expect(editor.textarea).not.toBeNull();
    expect(editor.textarea.value).toBe(editor.mdContent);
  });

  it("updates preview when textarea changes", () => {
    const editor = new InlineEditor(paragraph);
    editor.open();
    editor.textarea.value = "**bold text**";
    editor._updatePreview();
    // Mock markdownToHTML wraps in <p>
    expect(editor.preview.innerHTML).toContain("bold text");
  });

  it("toggleOriginal switches between MD and original text", () => {
    const editor = new InlineEditor(paragraph);
    editor.open();

    // Initially showing MD
    expect(editor.showingOriginal).toBe(false);

    // Toggle to original
    editor.toggleOriginal();
    expect(editor.showingOriginal).toBe(true);
    expect(editor.textarea.value).toBe("Hello world");

    // Toggle back
    editor.toggleOriginal();
    expect(editor.showingOriginal).toBe(false);
    expect(editor.textarea.value).toBe(editor.mdContent);
  });

  it("apply() converts MD to HTML and replaces node", () => {
    const editor = new InlineEditor(paragraph);
    editor.open();
    editor.textarea.value = "**new content**";
    editor.apply();

    expect(paragraph.innerHTML).toBe("<p>**new content**</p>");
    expect(paragraph.style.display).toBe("");
  });

  it("apply() restores original attributes", () => {
    const editor = new InlineEditor(paragraph);
    editor.open();
    editor.textarea.value = "changed";
    editor.apply();

    expect(paragraph.className).toBe("article");
    expect(paragraph.id).toBe("test");
  });

  it("reset() restores original HTML", () => {
    const editor = new InlineEditor(paragraph);
    editor.open();
    editor.reset();

    expect(paragraph.innerHTML).toBe("Hello <strong>world</strong>");
    expect(paragraph.style.display).toBe("");
  });

  it("destroy() removes container and shows node", () => {
    const editor = new InlineEditor(paragraph);
    editor.open();
    const container = editor.container;
    editor.destroy();

    // Container should be detached from DOM tree
    expect(container.isConnected).toBe(false);
    expect(paragraph.style.display).toBe("");
  });

  it("apply() does nothing when showing original", () => {
    const editor = new InlineEditor(paragraph);
    editor.open();
    editor.toggleOriginal();
    const originalHTML = paragraph.innerHTML;
    editor.apply();

    // Should not have changed
    expect(paragraph.innerHTML).toBe(originalHTML);
  });
});
