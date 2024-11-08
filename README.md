# Notice
ExtIO dll for HDSDR.  
~~Previously hosted at http://scirocco.pagesperso-orange.fr/hdsdr/~~ **DEPRECATED**  
See [Releases assets](https://github.com/Quadricopter/ExtIO_Alinco/releases)

Note:
- I only own a `DJ-X11` device and _"It works on my computer"_ :-p  
- I'm looking for `DX-R8` device testers (previously working with `v0.0.2`)

# Usage
- Get zip archive in [Releases assets](https://github.com/Quadricopter/ExtIO_Alinco/releases)
- Copy `ExtIO_*.dll` and `ExtIO_*.ini` files in your `HDSDR` installation directory (default: `C:\Program (x86)\HDSDR`).
- COM port configuration is currently done with an `.ini` file. The `.ini` file MUST have the same name as the associated .dll (eq: `ExtIO_alinco_djx11.ini` for `ExtIO_alinco_djx11.dll`) and contains something like following:
```
[CONFIG]
PORT=COM4
```

Note:  
As pointed by LC from HDSDR, `C:\Program (x86)\HDSDR` can be a problem for file editing (admin access rights).  
A `Windows Registry` configuration with a GUI is currently in progress.

# Build
Project is currently built with [Visual Studio 2022 Community Edition](https://visualstudio.microsoft.com/), free for open source projects.  
- Grab `LC_ExtIO_Types.h` from https://www.hdsdr.de/faq.html and place it in `src` directory
- Open `ExtIO_Alinco.sln` in Visual Studio 2022
- Select `Release` and `x86` target (**NOT `x64` target!**)
- [Build] -> [Build Solution] (or press Ctrl+Shift+B)
- You should find dll files in `ExtIO_Alinco\Release` directory

# Current Work In Progress
- Configuration GUI and `Windows Registry` instead of `.ini` file.
- Unique DLL for ever Alinco device? (pro/cons)
