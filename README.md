# fluxrename
A batch rename GUI utility for macOS

## Features
- Recursive batch rename
- Input filename filters (with regex avaliable)
- Multiple modes of operations, can be operated with or without file extensions
    - Replace (with regex avaliable)
    - Trim from start/end or from range A to B
    - Add string in front, at the back, or anywhere in the middle of the filename
    - String Manipulations (upper/lowercase) to both the filename and/or extension part
- Non-destructive rename preview before any disk operations, less likely to do unexpected renames
- Written with C++ to provide a blazing fast performance
- Reliable, fluxrename will not touch any of your files unless you click `Commit Rename`

## Screenshots
<img alt="Source selection screen" src="https://raw.githubusercontent.com/fluxTH/fluxrename/main/docs/screenshots/screen_1.png">
<img alt="Filter screen" src="https://raw.githubusercontent.com/fluxTH/fluxrename/main/docs/screenshots/screen_2.png">
<img alt="Rename configuration screen" src="https://raw.githubusercontent.com/fluxTH/fluxrename/main/docs/screenshots/screen_3.png">
<img alt="Rename preview screen" src="https://raw.githubusercontent.com/fluxTH/fluxrename/main/docs/screenshots/screen_4.png">
