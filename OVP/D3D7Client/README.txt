SPACEXPANSE VISUALISATION PROJECT -
D3D7 Client Installation Instructions

1. Required components
----------------------

To compile the sources of the D3D7 graphics client, you need:

* The latest SpaceXpanse release (base + SDK packages), available from the
  download page at the SpaceXpanse site http://orbit.medphys.ucl.ac.uk/

* The latest SpaceXpanse beta diffs, available under
  download.orbit.m6.net/betaNG/spacexpanse_beta.html

* A Windows C++ compiler (VC++6 is ok, later should be fine, other compilers
  may or may not work).

* For the various graphics clients, you also need the respective 3-D
  graphics SDKs. The D3D7 client was built with the MS DX7 SDK. This is no
  longer officially supported by MS, but can still be found on the web,
  for example at

  http://bcb-tools.com/Downloads/dx7adxf.exe

  (I don't know whether later DX SDK's still support the DX7
  interface, but it may be worth a try.)

* To recompile the D3D7 source documentation you need Doxygen from

  http://www.stack.nl/~dimitri/doxygen/

  There is a doxygen configuration file in D3D7Client/doc/Doxyfile which
  was used to generate the compressed html help file
  D3D7Client/doc/D3D7Client.chm also included in the repository.


2. Installation
---------------

For a first-time installation only:

* Create a directory for the SpaceXpanse distribution, e.g.
  Program Files\SpaceXpanse_OVP

* Install the latest SpaceXpanse release by unpacking the base package
  (spacexpanse060929_base.zip) and the SDK package (spacexpanse060929_sdk.zip) in
  that directory. Make sure to preserve the directory structure of the
  zip archives.

For each update of the OVP sources:

* Download the latest spacexpanse beta diffs from
  download.orbit.m6.net/betaNG/spacexpanse_beta.html
  Unpack in the SpaceXpanse directory. Make sure to preserve the directory
  structure of the zip archive. Answer "yes" if asked whether to overwrite
  existing files.
  The graphics API linking the spacexpanse core and external graphics clients
  is continuously evolving, so it is important to make sure you have the
  latest spacexpanse beta installed when compiling the current OVP snapshot.

* Make sure that the SpaceXpanse_NG shortcut in the SpaceXpanse root directory points
  to Modules/Server/spacexpanse.exe and is working properly.

* Download the OVP source tree into the SpaceXpanse directory:

  cvs -d:pserver:anonymous@spacexpansevis.cvs.sourceforge.net:/cvsroot/spacexpansevis login
  cvs -z3 -d:pserver:anonymous@spacexpansevis.cvs.sourceforge.net:/cvsroot/spacexpansevis co -P spacexpansevis

  You should end up with a directory "spacexpansevis" inside you SpaceXpanse root
  directory.

* Open the D3D7Client project (for VC++, the project file is in
  spacexpansevis/D3D7Client/D3D7Client.dsw) and compile (this requires the
  DX7 SDK to be installed).

* Note that the VS2005 project files included in the sources tend to get out
  of sync occasionally, because they are provided by other developers (I have
  no means of updating them). In case of problems, just delete those project
  files and have them regenerated by VC++ automatically from the VC++6
  project files.


3. Testing the D3D7 client
--------------------------

* If the compilation of the D3D7 client was successful, it has created a
  plugin in Modules/Plugin/D3D7Client.dll.

* Run the "no-graphics" version of spacexpanse (spacexpanse_ng) and activate
  the D3D7Client plugin from the Modules tab.

* You should now get a video tab in the Launchpad dialog. Configure your
  graphics driver and screen options, then launch a scenario.

* If you run a windowed mode, you will get the main render window, and
  an additional diagnosis window opened by the spacexpanse core (which does
  currently not have any functionality).

* The current version of the D3D7 client is not yet complete. Some of the
  features of the spacexpanse built-in graphics engine are still missing.
  However, the client is slowly taking shape, and new features are added on
  a regular basis.
