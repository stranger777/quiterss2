/**
 * Tests for md.js — HTML↔Markdown conversion.
 * Pure functions, no DOM needed (except turndown needs document).
 */

import { describe, it, expect, beforeEach } from "vitest";
import TurndownService from "turndown";
import { marked } from "marked";

// Simple equivalents of our md.js functions for testing
function htmlToMarkdown(html) {
  const td = new TurndownService({
    headingStyle: "atx",
    codeBlockStyle: "fenced",
    bulletListMarker: "-",
  });
  // Strip scripts/styles
  const cleaned = html
    .replace(/<script[\s\S]*?<\/script>/gi, "")
    .replace(/<style[\s\S]*?<\/style>/gi, "");
  return td.turndown(cleaned).trim();
}

function markdownToHTML(md) {
  return marked.parse(md, { gfm: true, breaks: true }).trim();
}

// --- htmlToMarkdown tests ---

describe("htmlToMarkdown", () => {
  it("converts plain paragraph", () => {
    expect(htmlToMarkdown("<p>Hello world</p>")).toBe("Hello world");
  });

  it("converts bold text", () => {
    const result = htmlToMarkdown("<p>Hello <strong>world</strong></p>");
    expect(result).toContain("**world**");
  });

  it("converts italic text", () => {
    const result = htmlToMarkdown("<p>Hello <em>world</em></p>");
    // Turndown uses _ for italic by default
    expect(result).toMatch(/[_*]world[_*]/);
  });

  it("converts links", () => {
    const result = htmlToMarkdown('<a href="https://example.com">Click here</a>');
    expect(result).toContain("[Click here](https://example.com)");
  });

  it("converts h1 heading", () => {
    expect(htmlToMarkdown("<h1>Title</h1>")).toBe("# Title");
  });

  it("converts h2 heading", () => {
    expect(htmlToMarkdown("<h2>Subtitle</h2>")).toBe("## Subtitle");
  });

  it("converts h3 heading", () => {
    expect(htmlToMarkdown("<h3>Section</h3>")).toBe("### Section");
  });

  it("converts unordered list", () => {
    const result = htmlToMarkdown("<ul><li>one</li><li>two</li></ul>");
    // Turndown may add extra spaces after the marker
    expect(result).toMatch(/-\s*one/);
    expect(result).toMatch(/-\s*two/);
  });

  it("converts ordered list", () => {
    const result = htmlToMarkdown("<ol><li>first</li><li>second</li></ol>");
    expect(result).toContain("first");
    expect(result).toContain("second");
  });

  it("converts inline code", () => {
    const result = htmlToMarkdown("<p>Use <code>foo()</code></p>");
    expect(result).toContain("`foo()`");
  });

  it("converts code block", () => {
    const result = htmlToMarkdown("<pre><code>function foo() {}\n</code></pre>");
    expect(result).toContain("```");
    expect(result).toContain("function foo()");
  });

  it("converts blockquote", () => {
    const result = htmlToMarkdown("<blockquote><p>Quote</p></blockquote>");
    expect(result).toContain("> Quote");
  });

  it("strips script tags", () => {
    const result = htmlToMarkdown('<p>Hi</p><script>alert(1)</script>');
    expect(result).toBe("Hi");
    expect(result).not.toContain("alert");
  });

  it("handles empty input", () => {
    expect(htmlToMarkdown("")).toBe("");
    // null/undefined handled by turndown returning empty string or throwing
    // We test the string paths that are safe
    expect(htmlToMarkdown("<p></p>")).toBe("");
  });
});

// --- markdownToHTML tests ---

describe("markdownToHTML", () => {
  it("converts plain text", () => {
    const result = markdownToHTML("Hello world");
    expect(result).toBe("<p>Hello world</p>");
  });

  it("converts bold", () => {
    const result = markdownToHTML("Hello **world**");
    expect(result).toBe("<p>Hello <strong>world</strong></p>");
  });

  it("converts italic", () => {
    const result = markdownToHTML("Hello *world*");
    expect(result).toBe("<p>Hello <em>world</em></p>");
  });

  it("converts links", () => {
    const result = markdownToHTML("[Click](https://example.com)");
    expect(result).toBe(
      '<p><a href="https://example.com">Click</a></p>'
    );
  });

  it("converts headings", () => {
    expect(markdownToHTML("# Title")).toBe("<h1>Title</h1>");
    expect(markdownToHTML("## Subtitle")).toBe("<h2>Subtitle</h2>");
  });

  it("converts unordered list", () => {
    const result = markdownToHTML("- one\n- two");
    expect(result).toContain("<ul>");
    expect(result).toContain("<li>one</li>");
    expect(result).toContain("<li>two</li>");
  });

  it("converts inline code", () => {
    const result = markdownToHTML("Use `foo()`");
    expect(result).toContain("<code>foo()</code>");
  });

  it("converts code block", () => {
    const result = markdownToHTML("```\ncode\n```");
    expect(result).toContain("<pre");
    expect(result).toContain("<code");
  });

  it("converts blockquote", () => {
    const result = markdownToHTML("> Quote");
    expect(result).toContain("<blockquote>");
    expect(result).toContain("Quote");
  });

  it("converts horizontal rule", () => {
    const result = markdownToHTML("---");
    expect(result).toContain("<hr");
  });

  it("converts table", () => {
    const md = "| A | B |\n|---|---|\n| 1 | 2 |";
    const result = markdownToHTML(md);
    expect(result).toContain("<table>");
    expect(result).toContain("<th>A</th>");
  });

  it("handles empty input", () => {
    expect(markdownToHTML("")).toBe("");
    // marked throws on null — we guard in the real md.js
    expect(() => markdownToHTML(null)).toThrow();
  });
});

// --- Roundtrip tests ---

describe("roundtrip: HTML → MD → HTML", () => {
  it("preserves bold semantics", () => {
    const original = "<p>Hello <strong>world</strong></p>";
    const md = htmlToMarkdown(original);
    const back = markdownToHTML(md);
    expect(back.toLowerCase()).toContain("<strong");
  });

  it("preserves link href", () => {
    const original = '<p><a href="https://x.com">link</a></p>';
    const md = htmlToMarkdown(original);
    const back = markdownToHTML(md);
    expect(back).toContain('href="https://x.com"');
  });

  it("preserves heading level", () => {
    const original = "<h2>Title</h2>";
    const md = htmlToMarkdown(original);
    expect(md).toBe("## Title");
    const back = markdownToHTML(md);
    expect(back).toBe("<h2>Title</h2>");
  });
});
