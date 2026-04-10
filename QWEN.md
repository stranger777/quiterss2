# QuiteRSS2 — Project Context

## Project Overview

**QuiteRSS2** is an open-source, cross-platform RSS/Atom news feed reader written in **Qt/C++/QML**. It is the successor to the original [QuiteRSS](https://github.com/QuiteRSS/quiterss) project. The application aims to be fast and user-friendly, supporting Windows, Linux, macOS, and Android.

Key features include:
- RSS/Atom feed aggregation and reading
- Embedded web browser (Qt WebEngine / WebView)
- System tray integration
- Sound notifications
- Multi-language support (translations via Transifex)
- Portable and installer-based distribution on Windows

## Tech Stack

| Category       | Technology                          |
|----------------|-------------------------------------|
| Language       | C++11, QML                          |
| Framework      | Qt 5.9+ (QtCore, QtQml, QtQuick, QtWebEngine, QtWidgets) |
| Build System   | qmake / `.pro` project file          |
| CI/CD          | AppVeyor (Windows), Travis CI (Linux/macOS) |
| Translations   | Transifex                           |
| 3rd-party libs | qtsingleapplication, ganalytics     |

## Directory Structure

```
quiterss2/
├── src/                    # Main application source code
│   ├── application/        # Application entry point, settings, logging
│   ├── common/             # Shared utilities
│   ├── network/            # Network manager
│   ├── systemtray/         # System tray icon & menu
│   ├── webengine/          # Embedded browser (WebEngine/WebView)
│   ├── 3rdparty/           # Third-party dependencies
│   └── main.cpp            # Application entry point
├── resources/              # Icons, images, QML files, sounds, .qrc resources
├── platforms/              # Platform-specific files (iOS, macOS, Linux desktop, icons)
├── android/                # Android-specific build files
├── translations/           # Translation files (.ts/.qm)
├── tools/                  # Build and utility scripts
├── quiterss2.pro           # Main qmake project file
└── .appveyor.yml / .travis.yml  # CI configuration
```

## Building and Running

### Prerequisites

- **Qt 5.9 or higher** with the following modules:
  - QtQml, QtQuick, QtWebEngine (or WebView for mobile)
- **C++11**-compatible compiler (MSVC on Windows, GCC/Clang on Linux/macOS)

### Windows

```bat
call vcvarsall.bat x64
mkdir build && cd build
qmake CONFIG+=release -spec win32-msvc ..\quiterss2.pro
jom -j3
windeployqt bin --qmldir=..\resources\qml\+webview --no-compiler-runtime
```

### Linux

```bash
mkdir build && cd build
qmake CONFIG+=release PREFIX=/usr ../quiterss2.pro
make -j3
make install
```

### macOS

```bash
mkdir build && cd build
qmake -recursive -spec macx-g++ CONFIG+=release CONFIG+=x86_64 ../quiterss2.pro
make -j3
rm -r -f ../resources/qml/+mobile
macdeployqt bin/QuiteRSS.app/ -dmg -qmldir=../resources/qml
```

### Build Options

| Flag                  | Description                              |
|-----------------------|------------------------------------------|
| `SYSTEMQTSA=true`     | Use system qtsingleapplication           |
| `DISABLE_BROWSER=true`| Build without embedded browser           |
| `NO_LOG_FILE=true`    | Disable log file output                  |
| `PREFIX=/usr`         | Installation prefix (Linux, default: `/usr`) |

Use `-j<N>` where N = CPU cores + 1 for faster builds.

## Architecture Notes

- **Entry point:** `src/main.cpp` — initializes `LogFile`, sets high-DPI attributes, then creates and runs the `Application` singleton.
- **Application class** (`src/application/application.h|cpp`): Manages the application lifecycle, settings, and single-instance behavior (via qtsingleapplication).
- **Settings** (`src/application/settings.h|cpp`): Persistent configuration storage.
- **Logging** (`src/application/logfile.h|cpp`): File-based logging system.
- **Network** (`src/network/networkmanager.h|cpp`): HTTP/network request handling.
- **System Tray** (`src/systemtray/systemtray.h|cpp`): System tray icon and context menu.
- **Web Engine** (`src/webengine/webengine.h|cpp`): Embedded browser component for displaying feed content. On desktop uses QtWebEngine; on mobile uses QtWebView.
- **QML UI:** The user interface is built using QML/Qt Quick, located in `resources/qml/`.

## Development Conventions

- **Coding style:** Based on [Qt Creator coding style](https://doc-snapshots.qt.io/qtcreator-extending/coding-style.html) and [QML Coding Conventions](http://doc.qt.io/qt-5/qml-codingconventions.html).
- **GPL v3:** All contributions must be compatible with the GNU General Public License v3.0.
- **Contributions:** Start small, follow the coding style, be prepared for detailed review and revisions. Each PR is built on CI for Windows, Linux, and macOS.
- **Translations:** Managed via [Transifex](https://www.transifex.com/quiterss_team/quiterss2/).

## Useful Commands

```bash
# Build (out-of-source)
mkdir build && cd build
qmake CONFIG+=release ../quiterss2.pro
make -j$(nproc)

# Run
./bin/quiterss          # Linux
./bin/QuiteRSS.exe      # Windows

# Install (Linux)
sudo make install

# Check for build artifacts
ls bin/
```

## Output Directories (configured in `.pro`)

| Variable      | Path           |
|---------------|----------------|
| `DESTDIR`     | `$OUT_PWD/bin` |
| `OBJECTS_DIR` | `$OUT_PWD/obj` |
| `MOC_DIR`     | `$OUT_PWD/moc` |
| `RCC_DIR`     | `$OUT_PWD/rcc` |
| `UI_DIR`      | `$OUT_PWD/ui`  |
