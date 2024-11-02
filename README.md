# Notice
ExtIO dll for HDSDR.

# Usage
Copy `ExtIO_Alinco_*.dll` in your `HDSDR` installation directory (default: `C:\Program (x86)\HDSDR`).
COM port configuration is currently done with an `.ini` file.
The `.ini` file MUST have the same name as the associated .dll (eq: `ExtIO_alinco_djx11.ini` for `ExtIO_alinco_djx11.dll`) and contains somthing like following:
```
[CONFIG]
PORT=COM4
```

Note:
As pointed by LC from HDSDR, `C:\Program (x86)\HDSDR` can be a problem for file editing (admin access rights).
An `Windows Registry` configuration with a GUI is currently 

# Build
I finally switched from `MinGW` to [Visual Studio 2022 Community Edition](https://visualstudio.microsoft.com/)..
- Grab `LC_ExtIO_Types.h` from https://www.hdsdr.de/faq.html and place it in `common` directory
- Open `ExtIO_Alinco.sln` in Visual Studio 2022
- Select `Release` and `x86` target (**NOT `x64` target!**)
- [Build] -> [Build Solution] (or press Ctrl+Shift+B)
- You should find dll files in ExtIO_Alinco\Release directory

# Current Work In Progress
- alinco.c files factorization
- Configuration GUI instead of `.ini` file.
