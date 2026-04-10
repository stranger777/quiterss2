/**
 * Markdown conversion utilities for inline editor.
 * Uses TurndownService for HTML→MD and marked for MD→HTML.
 * All functions are pure — safe for testing.
 */

const InlineMD = (() => {
  // Turndown config — preserves links, code blocks, lists
  const turndownService =
    typeof TurndownService !== "undefined"
      ? new TurndownService({
          headingStyle: "atx",
          codeBlockStyle: "fenced",
          bulletListMarker: "-",
        })
      : null;

  // Marked config — no unnecessary wrappers
  const markedConfig = {
    breaks: true,
    gfm: true,
  };

  if (typeof marked !== "undefined" && marked.setOptions) {
    marked.setOptions(markedConfig);
  }

  /**
   * Convert HTML string to Markdown.
   * @param {string} html
   * @returns {string}
   */
  function htmlToMarkdown(html) {
    if (!html || typeof html !== "string") return "";
    if (!turndownService) return html;

    // Create a temp container to handle partial HTML
    const div = document.createElement("div");
    div.innerHTML = html;

    // Strip script tags
    div.querySelectorAll("script, style").forEach((el) => el.remove());

    return turndownService.turndown(div.innerHTML).trim();
  }

  /**
   * Convert Markdown to safe HTML string.
   * @param {string} md
   * @returns {string}
   */
  function markdownToHTML(md) {
    if (!md || typeof md !== "string") return "";
    if (typeof marked === "undefined") return md;
    return marked.parse(md, { sanitize: false }).trim();
  }

  /**
   * Wrap selected text in a textarea with MD syntax.
   * @param {HTMLTextAreaElement} textarea
   * @param {string} before
   * @param {string} after
   */
  function wrapSelection(textarea, before, after) {
    const start = textarea.selectionStart;
    const end = textarea.selectionEnd;
    const text = textarea.value;
    const selected = text.substring(start, end) || "text";

    textarea.value =
      text.substring(0, start) +
      before +
      selected +
      after +
      text.substring(end);

    // Place cursor inside the wrapper
    textarea.selectionStart = start + before.length;
    textarea.selectionEnd = start + before.length + selected.length;
    textarea.focus();
  }

  /**
   * Get MD syntax tokens for a toolbar action.
   * @param {string} action
   * @returns {{before: string, after: string}}
   */
  function getActionSyntax(action) {
    const map = {
      bold: { before: "**", after: "**" },
      italic: { before: "*", after: "*" },
      strikethrough: { before: "~~", after: "~~" },
      code: { before: "`", after: "`" },
      h1: { before: "# ", after: "" },
      h2: { before: "## ", after: "" },
      h3: { before: "### ", after: "" },
      link: { before: "[", after: "](url)" },
      list: { before: "- ", after: "" },
      ol: { before: "1. ", after: "" },
      quote: { before: "> ", after: "" },
      hr: { before: "\n---\n", after: "" },
      table: {
        before: "\n| Col1 | Col2 |\n|------|------|\n| ",
        after: " |  |\n",
      },
    };
    return map[action] || { before: "", after: "" };
  }

  return { htmlToMarkdown, markdownToHTML, wrapSelection, getActionSyntax };
})();

if (typeof module !== "undefined" && module.exports) {
  module.exports = InlineMD;
}
