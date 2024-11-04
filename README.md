# Notice
ExtIO dll for HDSDR.  
~~Previously hosted at [deprecated](http://scirocco.pagesperso-orange.fr/hdsdr/)~~

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
- Grab `LC_ExtIO_Types.h` from https://www.hdsdr.de/faq.html and place it in `common` directory
- Open `ExtIO_Alinco.sln` in Visual Studio 2022
- Select `Release` and `x86` target (**NOT `x64` target!**)
- [Build] -> [Build Solution] (or press Ctrl+Shift+B)
- You should find dll files in `ExtIO_Alinco\Release` directory

# Current Work In Progress
- alinco.c files factorization
- Configuration GUI instead of `.ini` file.
