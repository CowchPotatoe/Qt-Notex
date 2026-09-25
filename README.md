# Notex
A Markdown editor built with **C++ and Qt** with live preview and PDF export.
> **Status:** Beta — v0.2.0

## Features
* Markdown editor with live preview
* Markdown parsing powered by [md4qt](https://github.com/KDE/md4qt)
* Latex rendering handled by [KaTex](https://github.com/KaTeX/KaTeX)
* Headings, paragraphs, lists, bold, italic, code blocks, and other Markdown formatting
* Markdown, Preview, and Split views
* Undo, redo, copy, paste, and zoom controls
* Open, save, and create Markdown files
* Multiple document tabs
* PDF export with A4 formatting
* Cursor position indicator
* Dark and light themes
* File system browser

## Installation
Notex is currently distributed as both an **AppImage** and a **tar.gz** package for **64-bit Linux (x86_64)** systems.
### AppImage
The AppImage is the easiest way to run Notex without installing it.
1. Download `Notex-x86_64.AppImage`.
2. Make the file executable:
   ```bash
   chmod +x Notex-x86_64.AppImage
   ```
3. Run Notex:
   ```bash
   ./Notex-x86_64.AppImage
   ```
The AppImage can be moved to another folder and run from there.
> **Note:** The AppImage contains the Notex application, Qt libraries, plugins, and application icon. However, application-menu and dock integration can depend on the Linux desktop environment. Some desktop environments may require additional AppImage integration support for the application to appear as an installed application with its icon.
### tar.gz
The tarball contains the Notex application, required Qt libraries, plugins, application icon, and installation scripts.
1. Download `Notex-release.tar.gz`.
2. Extract the archive:
   ```bash
   tar -xzf Notex-release.tar.gz
   ```
3. Enter the release directory:
   ```bash
   cd Notex-release
   ```
4. Install the desktop launcher:
   ```bash
   ./install.sh
   ```
The installer creates a desktop entry in the user's application menu and registers the Notex icon.
Notex can also be run directly from the release directory without installing the desktop launcher:
```bash
./run.sh
```

## About
Notex is a personal C++/Qt project created to learn desktop GUI development, text processing, Markdown parsing, file handling, and document rendering.

## Beta
Notex is currently a beta project. Some features are incomplete, and bugs may still exist.

## Third-Party Software
Notex uses [md4qt](https://github.com/KDE/md4qt) for Markdown parsing
and [KaTeX](https://github.com/KaTeX/KaTeX) for LaTeX math rendering.
The third-party source and applicable license information are retained
under `thirdparty/`.

## License
Notex is licensed under the **GNU Lesser General Public License, version 2.1 (LGPL-2.1)**.
See the [LICENSE](LICENSE) file for the full license text.
Notex also uses the Qt framework, which is distributed under its own applicable licensing terms.

## Author
Ivan Chen
