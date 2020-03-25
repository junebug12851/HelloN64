# Hello N64

So this is sort of a side project I'm doing for fun. It may or may not turn into anything but I'm hoping it may grow
into something. I'm super new to learning the N64.

## Setting up

Setting up a development enviroment is not easy and that's because everythign is old, the sdk, the tools
the enviroment... Everything is really designed for Windows 95/98. It will run on Windows XP, but that's 
as modern as you can get so you have to fire up a Windows XP virtual machine.

* You need to get a working Windows XP OS setup in some way or another either on a physical machine
  or virtualized. Trust me there isn't any way around it.
* Visit this website https://n64.dev. On that website you'll find all of your answers.
  You may also use this article but it's more confusing
  https://n64squid.com/homebrew/n64-sdk/ide/

Setting up the enviroment is tricky and you can ask the community sites on the website for help, once you 
get it setup you need to clone or download this repo.

If you install the tools in a non-standard location you can easily update either the `.bat` file or the
`Makefile`

## Building

If all is set correctly you just double click `build.bat` and everything will run and compile. You'll find the build 
artifacts in the `out` folder. You'll need a way to play it using either an emulator or the real machine. There's
lots of help out there for that.

In `build.bat` there is a define that defaults to `NTSC_TV`, to correctly compile it must compile for a format
however other options are `PAL_TV` and `MPAL_TV` which will all compile for that format.

## Why Visual Studio?

You may wonder why I'm using `Visual Studio 2019 Community Edition`. The reason is because I want all the modern
perks of a modern IDE complete with code hinting, syntax highlighting, and error checking. Now you cannot install
this into XP and you cannot build with it. It's there to help you write code more efficiently.

** You don't have to use `VS2019` ** : The project is setup in a way where you can use one of several text editors
in windows XP comepltely forgoing VS.

## Setting up and using VS (Completely Optional)

1. I clone this repo to my Windows 10 machine and share the folder with my XP machine
2. I install the tools to Windows 10 (Please note that they are installable to Win10 cannot be used above XP)
3. I tell VS to references the includes from that (So that I get completion, error checking, correct syntax highlighting, amongst others)
4. I edit until I'm ready to build
5. To build I have to switch to XP and double click `build.bat`
6. I can run it in an emulator on either Windows 10 or Windows XP or the real machine

It's a little more annoying to switch back and forth but this way I can use `SourceTree` and `VS2019` which are both
great modern tools to help me.

## Flat Folder Structure and Filters

So you may have noticed all the files are on top, this is not how I like to do my projects. I prefer
them to be deeply organized in folders nice and neat. However the tools have many limitations and bugs.

In short I have a few options:

* Use sub-folders and deal with a more complicated build. This is because Makefile doesn't like to deal with
  files inside folders and I think it goofs up on long paths as well. In short there's various more complicated
  work-arounds that are simple but fragile to implement and add to build complexity.
* Don't use folders and deal with all the files top-level. This is the simplest because it keeps the build really
  simple and is expandable but can get very messy.

This is yet another reason why I choose VS filters. Mnay people, myself included, normally hate the filters VS defaults
to because they look like folders but all the files are not really in those folders, they're all top-level. However given the
limitations I can opt for the lesser evil and choose to have all my files top-level but under VS filters or "Virtual Folders".

This means, to me, they're organized properly in folders but in reality their all top-level which satifies the build system
and keeps things simple.

This way I can refactor and re-organize files and deeply nest them into folders as much as I want and not have to worry about
dealing with a complicated and fragile build system that would need careful refacotring if I were to move any files or 
folders around.

Again this also works for people that don't want to use VS and who just want to edit directly with Notepad++ or the like although
you won't have the organized appearance filters can give.

## License

* My code is Licensed Apache2 (Only to my code)
* I claim no ownership or rights to any of Nintendo's specific code or names
* I have great help from Buu342 who has written wonderful tutorials and guides and has been there
  to answer many of my questions on Discord. Much of the boiler plate code comes from him and it's
  public domain except for any of the Nintendo specific code.
