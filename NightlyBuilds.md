# [This information may be obsolete, please refer to the wiki](https://wiki.mtasa.com/wiki/Nightly_Builds) #
The below information has been retained for historical purposes.

---

<table>
<tr>
<td width='48%' cellspacing='10' valign='top'>
<h1>Introduction</h1>

Nightly builds are experimental builds and thus do not work straight out-of-the-box, as they exclude a few files that rarely change but are necessary to work. This is because of size and bandwidth issues, and the fact that they are meant as incremental updates for those that already have the necessary files installed.<br>
<br>
Please bear in mind that these builds are developer builds and will be<br>
unstable. For a list of changes for each revision, head over to our<br>
project page for more information.<br>
<br>
Our automated build server takes care of building (and subsequently uploading) the latest revision. The current status of our automated builds can be seen in the widget to the right. The integration build starts at certain intervals during the day to check for build errors while the nightly build is built at 23:00 (GMT/DST).<br>
<br>
</td>
<td width='4%'></td>
<td width='48%' cellspacing='10' valign='top'>
<h1>Already know how to get Nightly builds running?</h1>
If you know what you're doing, you can get the files you need from here:<br>
<br>
<b><a href='http://mirror.mtasa.com/mtasa/data/'>Go to the data files download page</a> <i>for data, and files required to build locally</i></b>

<a href='http://mirror.mtasa.com/mtasa/resources/'>Go to the resources download page</a> <i>for the latest resource packages</i>

<a href='http://nightly.multitheftauto.com/'>Go to the nightly builds</a> <i>for the latest MTA nightly build</i>

<wiki:gadget url="http://www.mtasa.com/gadget/nightly.xml" height="150" width="610" border="0" /><br>
</td>

</tr>
</table>

# Setting up nightly builds #
Nightly builds are designed in a manner in which minimal updates are required in order to install a new nightly.  This however means that you **must** first follow some specific steps to get your nightly build working.

### Step 1: Download the latest nightly build ###
First off you should download the latest available nightly build.
  * You should pick the nightly with the **highest revision number**.
  * You can find all available nightly builds here: **http://nightly.multitheftauto.com/**
  * Run the installer

### Step 2: Download the run dependencies ###
You must also download and install run dependencies.  These are files which are required by MTA to run, but usually do not change and therefore are not included in the nightly build.
  * You can find the latest available data archive **[here.](http://code.google.com/p/mtasa-blue/downloads/list?q=label:Data)**
  * Run the installer, ensuring it correctly points to your MTA: San Andreas installation directory.

_Run dependencies rarely need to be updated.  If your build starts malfunctioning for any reason it may be because you need to update your dependencies.  They are updated whenever needed._

### Step 3: Download the latest resources ###
The latest resources are also available.   These components are only required if you wish to run a server or the map editor.  Default resources from previous versions of MTASA are outdated, and will not function.
  * You should pick the resource archive with the **highest revision number**.
  * You can find the latest available resources **[here](http://code.google.com/p/mtasa-resources/downloads/list)**.
  * The contents of the archive should be placed inside your **MTA San Andreas\server\mods\deathmatch\resources** directory. You will need to create a resources directory if you do not already have one.

_Resources should also be updated fairly frequently.  They are built at 2300GMT, depending on changes that are made during that day.  You should always ensure you always have the latest resourecs package._

### Optional: Download DirectX ###
You need DirectX (March 2009) installed. If you haven't updated DirectX since then, install [this](http://www.microsoft.com/downloads/details.aspx?FamilyId=2DA43D38-DB71-4C1B-BC6A-9B6652CD92A3&displaylang=en) - it only takes a minute or so.

### Step 4: Start the game ###
You have now installed all the components required to run a nightly build.  You should now be able to run both the client and the server, or join any servers that may be available.

Bugs, suggestions or feedback can go in either our forums or the bugtracker
  * http://forum.multitheftauto.com
  * http://bugs.multitheftauto.com

For general scripting or server setup help, please refer to our [wiki](http://development.mtasa.com):
  * [Client Manual](http://development.mtasa.com/index.php?title=Deathmatch_Client_Manual)
  * [Server Manual](http://development.mtasa.com/index.php?title=Deathmatch_Server_Manual)
  * [Setting up admin on your server](http://development.mtasa.com/index.php?title=Admin)