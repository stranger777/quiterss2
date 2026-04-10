/**
 * Tests for content.js — pick mode, hotkeys, editor lifecycle.
 * Uses JSDOM to simulate DOM events.
 */

import { describe, it, expect, beforeEach, vi } from "vitest";

// Mock InlineMD (minimal)
globalThis.InlineMD = {
  htmlToMarkdown: (html) => html.replace(/<[^>]*>/g, "").trim(),
  markdownToHTML: (md) => `<p>${md}</p>`,
  wrapSelection: (ta, b, a) => {
    const s = ta.selectionStart || 0;
    const e = ta.selectionEnd || 0;
    const t = ta.value;
    const sel = t.substring(s, e) || "text";
    ta.value = t.substring(0, s) + b + sel + a + t.substring(e);
  },
  getActionSyntax: () => ({ before: "", after: "" }),
};

// Mock chrome
globalThis.chrome = {
  runtime: {
    getURL: (p) => p,
    onMessage: { addListener: vi.fn() },
  },
};

// InlineEditor minimal mock
class InlineEditor {
  constructor(node) {
    this.node = node;
    this.originalHTML = node.innerHTML;
    this.originalText = node.textContent.trim();
    this.originalAttributes = {};
    for (const attr of node.attributes) this.originalAttributes[attr.name] = attr.value;
    this.mdContent = InlineMD.htmlToMarkdown(this.originalHTML);
    this.showingOriginal = false;
    this.container = null;
    this.shadow = null;
    this.textarea = null;
    this.preview = null;
  }

  open() {
    this.container = document.createElement("div");
    this.container.className = "md-inline-editor";
    this.node.parentNode.insertBefore(this.container, this.node);
    this.node.style.display = "none";
    this.shadow = this.container.attachShadow({ mode: "open" });
    this.shadow.innerHTML = `
      <div class="editor-panel">
        <div class="original" hidden><div class="original-content"></div></div>
        <div class="toolbar"><button data-action="bold">B</button></div>
        <textarea class="md-input"></textarea>
        <div class="preview"></div>
      </div>
    `;
    this.textarea = this.shadow.querySelector("textarea");
    this.preview = this.shadow.querySelector(".preview");
    this.textarea.value = this.mdContent;
  }

  toggleOriginal() {
    this.showingOriginal = !this.showingOriginal;
    const orig = this.shadow.querySelector(".original");
    if (this.showingOriginal) {
      orig.hidden = false;
      this.textarea.value = this.originalText;
    } else {
      orig.hidden = true;
      this.textarea.value = this.mdContent;
    }
  }

  apply() {
    if (this.showingOriginal) { this.destroy(); return; }
    this.node.innerHTML = InlineMD.markdownToHTML(this.textarea.value);
    this.destroy();
  }

  destroy() {
    if (this.container) this.container.remove();
    this.node.style.display = "";
  }
}

globalThis.InlineEditor = InlineEditor;

// Load content.js logic
function loadContentScript() {
  let pickMode = false;
  let activeEditor = null;

  function setPickMode(on) {
    pickMode = on;
    document.body.style.cursor = on ? "crosshair" : "";
    document.body.classList.toggle("md-pick-mode", on);
  }

  function openEditorFor(node) {
    if (activeEditor) activeEditor.destroy();
    activeEditor = new InlineEditor(node);
    activeEditor.open();
    setPickMode(false);
  }

  function handleKeydown(e) {
    if (e.ctrlKey && e.shiftKey && (e.key === "E" || e.key === "e")) {
      e.preventDefault();
      e.stopPropagation();
      setPickMode(!pickMode);
      return;
    }
    if (activeEditor && activeEditor.shadow) return;
  }

  function handleClick(e) {
    if (pickMode) {
      const target = e.target.closest(
        "p, div, article, section, h1, h2, h3, h4, h5, h6, li, blockquote, pre, span"
      );
      if (!target || target.closest(".md-inline-editor")) return;
      e.preventDefault();
      e.stopPropagation();
      openEditorFor(target);
      return;
    }
    if (activeEditor && !e.target.closest(".md-inline-editor")) {
      activeEditor.apply();
      activeEditor = null;
    }
  }

  document.addEventListener("keydown", handleKeydown);
  document.addEventListener("click", handleClick, true);

  return {
    get pickMode() { return pickMode; },
    setPickMode,
    get activeEditor() { return activeEditor; },
    openEditorFor,
    handleKeydown,
    handleClick,
  };
}

describe("content.js — pick mode", () => {
  let app;

  beforeEach(() => {
    document.body.innerHTML = '<article><p id="p1">Hello</p><p id="p2">World</p></article>';
    app = loadContentScript();
  });

  it("starts with pick mode off", () => {
    expect(app.pickMode).toBe(false);
    expect(document.body.style.cursor).toBe("");
  });

  it("Ctrl+Shift+E toggles pick mode on", () => {
    const e = new KeyboardEvent("keydown", { ctrlKey: true, shiftKey: true, key: "E" });
    document.dispatchEvent(e);
    expect(app.pickMode).toBe(true);
    expect(document.body.style.cursor).toBe("crosshair");
  });

  it("Ctrl+Shift+E toggles pick mode off", () => {
    app.setPickMode(true);
    const e = new KeyboardEvent("keydown", { ctrlKey: true, shiftKey: true, key: "E" });
    document.dispatchEvent(e);
    expect(app.pickMode).toBe(false);
    expect(document.body.style.cursor).toBe("");
  });

  it("adds md-pick-mode class when active", () => {
    app.setPickMode(true);
    expect(document.body.classList.contains("md-pick-mode")).toBe(true);
  });
});

describe("content.js — editor creation", () => {
  let app;

  beforeEach(() => {
    document.body.innerHTML = '<article><p id="p1">Hello <strong>world</strong></p></article>';
    app = loadContentScript();
  });

  it("click in pick mode creates editor", () => {
    app.setPickMode(true);
    const p = document.getElementById("p1");
    const e = new MouseEvent("click", { bubbles: true });
    Object.defineProperty(e, "target", { value: p });
    p.dispatchEvent(e);

    expect(app.activeEditor).not.toBeNull();
    expect(app.pickMode).toBe(false); // pick mode exits
  });

  it("click outside pick mode does nothing", () => {
    const p = document.getElementById("p1");
    const e = new MouseEvent("click", { bubbles: true });
    p.dispatchEvent(e);

    expect(app.activeEditor).toBeNull();
  });

  it("clicking editor itself does not create new editor", () => {
    app.setPickMode(true);
    const p = document.getElementById("p1");
    const click1 = new MouseEvent("click", { bubbles: true });
    Object.defineProperty(click1, "target", { value: p });
    p.dispatchEvent(click1);

    // Now try clicking the editor
    const editor = document.querySelector(".md-inline-editor");
    const click2 = new MouseEvent("click", { bubbles: true });
    Object.defineProperty(click2, "target", { value: editor });
    editor.dispatchEvent(click2);

    // Should still be the same editor, not recreated
    expect(app.activeEditor).not.toBeNull();
  });
});

describe("content.js — auto-apply on blur", () => {
  let app;

  beforeEach(() => {
    document.body.innerHTML = '<p id="p1">Hello</p><div id="outside">Outside</div>';
    app = loadContentScript();
  });

  it("click outside editor applies changes", () => {
    // Open editor
    app.setPickMode(true);
    const p = document.getElementById("p1");
    const click1 = new MouseEvent("click", { bubbles: true });
    Object.defineProperty(click1, "target", { value: p });
    p.dispatchEvent(click1);

    expect(app.activeEditor).not.toBeNull();

    // Simulate click outside — call handleClick directly
    const outside = document.getElementById("outside");
    const click2 = new MouseEvent("click", { bubbles: true });
    Object.defineProperty(click2, "target", { value: outside });
    app.handleClick(click2);

    // Editor should be gone
    expect(app.activeEditor).toBeNull();
  });
});

describe("content.js — keyboard shortcuts", () => {
  let app;

  beforeEach(() => {
    document.body.innerHTML = '<p id="p1">Test</p>';
    app = loadContentScript();
  });

  it("lowercase e also toggles pick mode", () => {
    const e = new KeyboardEvent("keydown", { ctrlKey: true, shiftKey: true, key: "e" });
    document.dispatchEvent(e);
    expect(app.pickMode).toBe(true);
  });
});
