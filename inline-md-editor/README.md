# Inline MD Editor

Браузерное расширение для inline-редактирования любой веб-страницы через Markdown.

## Как это работает

1. Нажмите `Ctrl+Shift+E` → курсор становится крестиком
2. Кликните на любой текстовый узел (параграф, заголовок, статью...)
3. Редактируйте в Markdown — внизу live-preview HTML
4. Кликните в другое место — изменения применятся автоматически
5. `Ctrl+O` — показать/скрыть оригинальный текст для сравнения

## Горячие клавиши

### Управление
| Клавиша | Действие |
|---------|----------|
| `Ctrl+Shift+E` | Включить/выключить режим выбора |
| `Escape` | Закрыть редактор, отменить изменения |
| `Ctrl+Enter` | Применить изменения и закрыть |
| `Ctrl+O` | Показать/скрыть оригинальный текст |

### Форматирование
| Клавиша | Действие | Markdown |
|---------|----------|----------|
| `Ctrl+B` | **Жирный** | `**text**` |
| `Ctrl+I` | *Курсив* | `*text*` |
| `Ctrl+K` | Ссылка | `[text](url)` |
| `` Ctrl+` `` | Код | `` `code` `` |
| `Ctrl+Shift+H` | Заголовок | `# text` |
| `Ctrl+Shift+L` | Список | `- text` |
| `Ctrl+Shift+Q` | Цитата | `> text` |

## Установка

### Chrome / Edge / Яндекс.Браузер
1. Откройте `chrome://extensions/` (или `edge://extensions/`)
2. Включите **Режим разработчика** (переключатель справа вверху)
3. Нажмите **Загрузить распакованное расширение**
4. Выберите папку `inline-md-editor`
5. Готово — иконка появится в тулбаре

### Firefox
1. Откройте `about:debugging#/runtime/this-firefox`
2. **Загрузить временное дополнение**
3. Выберите любой файл из папки `inline-md-editor`

## Архитектура

```
inline-md-editor/
├── manifest.json       # Manifest V3 — метаданные, permissions
├── content.js          # Content Script — pick mode, hotkeys
├── editor.js           # InlineEditor — класс редактора (Shadow DOM)
├── md.js               # HTML↔Markdown конвертация (turndown + marked)
├── styles.css          # Стили редактора
├── popup/
│   ├── popup.html      # Попап с шорткатами
│   └── popup.js        # Обработчик кнопки
├── tests/
│   ├── md.test.js      # Тесты конвертации (30 тестов)
│   ├── editor.test.js  # Тесты InlineEditor (12 тестов)
│   └── content.test.js # Тесты content script (10 тестов)
├── vitest.config.js    # Конфиг тестов
└── package.json        # Зависимости
```

### Поток данных

```
Пользователь кликает на узел
  → InlineEditor(node)
    → Запомнить originalHTML, originalText, originalAttributes
    → htmlToMarkdown(originalHTML) → mdContent
    → Создать Shadow DOM с textarea
    → Пользователь редактирует MD
    → markdownToHTML(md) → HTML preview
    → Клик вне / Ctrl+Enter
      → Заменить node.innerHTML = HTML
      → Восстановить атрибуты
      → Удалить редактор
```

### Безопасность

- Shadow DOM изолирует стили от страницы
- `marked` с `sanitize: false` — рендерит как есть, но ввод контролируется пользователем
- Оригинальные атрибуты (class, id, style) сохраняются при применении

## Разработка

```bash
cd inline-md-editor
npm install
npm test          # Запустить все тесты
npm run test:watch # Тесты в режиме watching
```

### Зависимости

| Пакет | Назначение |
|-------|-----------|
| `turndown` | HTML → Markdown |
| `marked` | Markdown → HTML |
| `vitest` | Тест-раннер |
| `jsdom` | DOM-эмуляция для тестов |

## Принципы дизайна

- **Ноль когнитивной нагрузки** — никаких видимых кнопок по умолчанию
- **Фокус** — всё управляется клавиатурой
- **Контекст** — оригинал всегда доступен через `Ctrl+O`
- **Автосохранение** — клик вне = применение
- **Безопасный откат** — `Escape` отменяет всё

## Лицензия

MIT
