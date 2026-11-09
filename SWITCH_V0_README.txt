sloperinaGZ for Ocarina of Time Switch 2 cartridge 1.0
=====================================================

Game title: 0400486024DEC000
Main BuildID: 33FA6C2C23848282A77E8B88A0A93F4887C55920

Before copying, move any existing
"atmosphere/contents/0400486024DEC000" folder to a backup outside the
"atmosphere" directory, and disable the earlier OoTr test mod's IPS patch
directory under "atmosphere/exefs_patches". The old RomFS overlay could
otherwise still crash when loading a save. Then copy the "atmosphere" folder
from this ZIP to the root of your Switch 2 SD card. The installed files are:

  atmosphere/contents/0400486024DEC000/exefs/subsdk9
  atmosphere/exefs_patches/sloperinaGZ/33FA6C2C23848282A77E8B88A0A93F4887C55920.ips

Another mod using subsdk9 or this frame hook cannot run at the same time.
This package contains no game dump or save.

In gameplay, hold R and push the right stick down to open the menu. Use the
D-pad to move, L to select, and R + D-pad left to go back. The menu is usable
away from an NPC.

This is an experimental cartridge 1.0 port. It booted, loaded the user's
Castle Town save, displayed the menu, read live 1.0 GameData in Lichii, and
changed rupees from 50 to 200 using the Link menu. A separate emulator audit
opened every top-level section, used the infinite-rupees cheat (50 to 200),
added the Boomerang (live stock 0 to 1), and warped from Castle Town to Hyrule
Field; the destination map and rendered scene were checked. Other individual
actions and Switch 2 hardware remain untested. The 1.0 camera lookup now
resolves the active 3D view: Scene > Collision > Show drew cyan air-wall
outlines in the Castle Town emulator test, and the camera watch showed an
angle. These are the mod's air-wall shapes, not the complete collision mesh.
The pouch notification call is omitted. Keep a copy of your save.

To remove the port, delete the two files listed above. The "source" folder
contains the corresponding GPLv3 source code and build instructions.
