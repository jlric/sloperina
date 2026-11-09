# Cartridge 1.0 port

This branch adds the native sloperinaGZ module for Ocarina of Time Switch 2
cartridge 1.0, title `0400486024DEC000`, main BuildID
`33FA6C2C23848282A77E8B88A0A93F4887C55920`.

Build with `./docker-make VERSIONS=v0`. The outputs are
`bin/v0/subsdk9` and the BuildID-named IPS32 file. The 1.1.1 source build
remains available with the default `./docker-make` command.

The v0 header maps the frame hook, GameData manager and functions, pouch,
equipment, event request, UI screens, Link object, and frame counter to
cartridge 1.0. The frame-site original instruction and empty RX cave were
checked against the exact decompressed cartridge main before generating IPS.
The v0 String64 implementation uses the manager descriptor at `+0x608` and
the current buffer pointer at `record+0x68`, as checked by the independent
1.0 live GameData binding in this workspace. The sequence fields start two
indices earlier in cartridge 1.0: `Sequence_CurrentBanc` is index `0x1d`,
followed by the four reserved travel fields at `0x1e` through `0x21`.

The 1.1.1 pouch notification callback and legacy camera singleton are not
mapped in cartridge 1.0. The port skips the notification call. Its `camera()`
now resolves the hashed `3D` view through the stock 1.0 registry and view
manager, then selects the render camera with the game's getter rules. The
Castle Town live probe read a valid camera matrix, position, look-at,
50-degree FOV, and 16:9 aspect at the native offsets. The resolver rechecks
the selected view on each call, rejects the static fallback and unmapped
objects, and supplies the existing air-wall projection and camera watch.
Other camera-related functions need separate functional tests. The second
and fifth speculative Link paths were omitted; the retained
paths and their static vtable were matched to cartridge 1.0 code/data.

Lichii build 116 tests in the workspace loaded the user's cartridge 1.0
Castle Town save and rendered gameplay with the native module, frame/input/NVN
hooks, and on-screen watch. The R + right-stick-down menu opened over gameplay,
and L opened its Link submenu. The exact 1.0 Bool, Int, and String64 live
tables bound correctly, including the saved rupee value. The corrected
String64 path displayed "market (child)". The Link-menu fill action changed
the live rupee Int from 50 to 200. A later audit captured all 11 other
top-level sections opening in Castle Town. The infinite-rupees cheat changed
the live Int from 50 to 200, and the child-items menu changed Boomerang stock
from 0 to 1. The audit's first warp reached Hyrule Field but triggered owl
dialogue, so its ordinary HUD predicate timed out. A focused retry passed:
the stock String64 map became `HyruleField_Child` and the rendered field frame
was nonblank. Evidence is in `artifacts/sloperina-port/smoke-v0-audit4` and
`smoke-v0-warp3` in the workspace. These are emulator results, not a Switch 2
hardware test. Other individual game-changing actions and hook-slot lifetime
are still unverified.

The 1.0 collision audit in `artifacts/sloperina-port/collision-v0-first`
loaded the same Castle Town save, opened Scene > Collision, toggled Show, and
read the module's state from guest memory. It changed Show from 0 to 1,
identified 42 nearby air walls, and published 134 projected segments. Its
captured gameplay frame visibly contains cyan outlines, and the camera watch
displays an angle. This validates the air-wall overlay in Lichii; the mod does
not draw the full collision mesh, and Switch 2 hardware is still untested.
The same rebuilt `subsdk9` then passed
`artifacts/sloperina-port/camera-regression-v0b`: Link-menu rupees changed
50 to 200, the warp set the live map to `HyruleField_Child`, and the
destination rendered. The package gate requires both reports and the tested
module hash before creating the Switch 2 ZIP.

This module uses `subsdk9` and a main frame-site IPS. Remove any other mod
using either location before installing it. In particular, the earlier OOTr
mod uses `subsdk9` too.
