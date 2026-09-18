# MarkTek
A Markdown editor built with **C++ and Qt** with live preview and PDF export.
> **Status:** Beta — v0.1.0

## Features
* Markdown editor with live preview
* Headings, paragraphs, lists, bold, and italic formatting
* Markdown, Preview, and Split views
* Undo, redo, copy, paste, and zoom controls
* Open, save, and create Markdown files
* Multiple document windows
* PDF export with A4 formatting
* Cursor position indicator

## Installation
MarkTek is currently distributed as both an **AppImage** and a **tar.gz** package for 64-bit Linux systems.

### AppImage
The AppImage is the easiest way to run MarkTek without installing the application.
1. Download:
   `MarkTek-x86_64.AppImage`
2. Make the file executable:
   ```bash
   chmod +x MarkTek-x86_64.AppImage
   ```
3. Run MarkTek:
   ```bash
   ./MarkTek-x86_64.AppImage
   ```
You can move the AppImage to another folder and run it from there.
> Desktop application-menu integration and application icons may depend on your Linux desktop environment and AppImage integration support.

### tar.gz
The tarball contains the MarkTek application, required Qt libraries, plugins, and installation scripts.
1. Download:
   `MarkTex-release.tar.gz`
2. Extract the archive:
   ```bash
   tar -xzf MarkTex-release.tar.gz
   ```
3. Enter the release directory:
   ```bash
   cd MarkTex-release
   ```
4. Run the installer:
   ```bash
   ./install.sh
   ```
The installer creates a desktop entry so MarkTek can appear in your application menu.
You can also run MarkTek directly from the release directory:
```bash
./run.sh
```

## About
MarkTek is a personal C++/Qt project created to learn desktop GUI development, text processing, Markdown parsing, file handling, and document rendering.
The project currently uses a custom, limited Markdown parser for the initial version, with plans to expand its Markdown and LaTeX support over time.

## Beta
MarkTek is currently a beta project. Some features are incomplete and bugs may still exist.

## License
MarkTek is licensed under the **GNU Lesser General Public License, version 2.1 (LGPL-2.1)**.
See the [LICENSE](LICENSE) file for the full license text.
MarkTek uses the Qt framework, which is distributed under its own applicable licensing terms.

## Author
Ivan Chen
