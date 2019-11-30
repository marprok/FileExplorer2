# FileExplorer2
A better version of FileExplorer.

## Have you ever thought that **ls** is useless? No? Neither have I!

This project was not created as a replacement of the ls command. It was built as a way to improve the architecure of the first
FileExplorer. For the most part it works the same way, but this time more information about a file is displayed onto the screen
as well as the ability to (c)reate and (d)elete files is supported. A new entity was created, the Scene, which is a class that 
manages all the windows that are displayed on the screen. Furthermore, the windows are now either resizable and permanent to the
screen or fixed sized and short lived(as long as user interaction lasts). To run the program you will need ncurses, the newest
g++, as well as cmake to compile the source code. The code is not perfect but each namespace was designed to be independant of the 
other and easily used into other projects. Last but not least, the placement of the DialogWindows on screen is not the optimal
and that is because I hate trying to find the best position to place the GUI elements. As a result the UI will look ugly.
