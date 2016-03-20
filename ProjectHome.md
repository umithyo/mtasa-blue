If you just want to play Multi Theft Auto, you've come to the wrong place. Click [here](https://www.mtasa.com) to visit the main Multi Theft Auto page.

On the other hand, if you're interested in developing MTA, read on...

## [This project has moved to GitHub](https://github.com/multitheftauto/mtasa-blue) ##
The below information has been retained for historical purposes, please refer to [our wiki](https://wiki.mtasa.com) for up to date information about this project.


---


### Introduction ###
Multi Theft Auto (MTA) is a software project that adds network play functionality to Rockstar North's Grand Theft Auto game series, in which this functionality is not originally found. It is a unique modification that incorporates an extendable network play element into a proprietary commercial single-player PC game.

The engine which MTA uses for its multiplayer functionality, codenamed _Blue_, provides a modular implementation for installing a gaming engine that itself acts as an extension of the original game through various hooking and code injection techniques.  Essentially, it serves as the main codebase for implementing MTA's multiplayer into GTA.

The Blue core then goes onto further expose it's functionality to a _Lua_ virtual machine so that third-party content can be used to develop the gamemodes and maps to extend the sandbox nature of the game, which in turn allows countless possibilities for each server.  These Lua scripts are placed into packages called _resources_ which are installed into the MTASA server.  MTASA's official resources package can be found at our [Resources project](https://github.com/multitheftauto/mtasa-resources/).

### Structure ###
Our project is divided into a series of modularized pieces for the client and server. Our original aim was to create a platform capable of running on any game. Over the years, this has changed, but our original design is outlined as follows:

  * **core**: all game independent facilities and ties all subsystems together (hooking, configuration, input binding, main menu, GUI windows, etc.)
  * **game\_sa**: interface to Grand Theft Auto: San Andreas by means of implementing classes identical to the game and attaching these through memory
  * **multiplayer\_sa**: second interface to Grand Theft Auto: San Andreas to isolate low-level hacks away from the cleaner game\_sa module
  * **gui**: graphical user interface rendering
  * **deathmatch**: Multi Theft Auto deathmatch module, implementing nearly all engine logic for the game itself along with things such as Lua VMs
  * **net**: network handling (this module is covered by a different license and is only available as a binary release)

Because of the nature of our project, this design has not been strictly followed everywhere. Our core module is not really game independent, our deathmatch module contains logic that should have been implemented in core and there is a certain amount of redundant (duplicated) classes and code that is less than ideal. Granted, this project has been fully written by a fair amount of volunteers in a non-profit fashion. We will be working towards cleaning this up gradually.

## Development ##
Our nightly builds are not currently built nightly, but are built fairly often. For more information about these builds, head over to our [Nightly Builds](https://nightly.mtasa.com) page or our [wiki page](https://wiki.multitheftauto.com/wiki/Nightly_Builds).

We encourage anyone that is interested in participating in development to contribute! Our code is structured and easy to get around with. We offer our [own bug tracker](http://bugs.mtasa.com/) that contains bugs and feature reports and also allows any developer to submit their own patches. These patches will then be reviewed by us.

We generally are keen to give svn access to new developers. If you contribute a couple of good patches, we'll probably give you commit access!

All development stuff is available through your personal account at our [Multi Theft Auto community](https://community.mtasa.com/) website.