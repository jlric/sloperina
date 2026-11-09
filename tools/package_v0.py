#!/usr/bin/env python3
"""Package the tested cartridge 1.0 build with its corresponding GPL source."""
from __future__ import annotations

import hashlib
import json
from pathlib import Path
import struct
import zipfile


SOURCE = Path(__file__).resolve().parents[1]
WORKSPACE = SOURCE.parents[1]
BUILD_ID = "33FA6C2C23848282A77E8B88A0A93F4887C55920"
TITLE = "0400486024DEC000"
BIN = SOURCE / "bin/v0"
PROFILE_EXEFS = (WORKSPACE / "tools/lichii/profile/sloperina-v0/data/eden/load" /
                 TITLE / "sloperinaGZ-v0/exefs")
REPORT = WORKSPACE / "artifacts/sloperina-port/camera-regression-v0b/report.json"
COLLISION = WORKSPACE / "artifacts/sloperina-port/collision-v0-first/report.json"
OUTPUT = WORKSPACE / "artifacts/sloperinaGZ-switch2-cartridge-1.0-test.zip"
SOURCE_DIRS = ("src", "lib", "data", "lang", "docs", "tools")
SOURCE_FILES = ("Makefile", "docker-make", "VERSION", "LICENSE", "README.md",
                "INSTALLATION.md", "SWITCH_V0_README.txt", "CONTRIBUTING.md",
                "CONTRIBUTORS.md", "THANKS.md")


def sha(path: Path) -> str:
    return hashlib.sha256(path.read_bytes()).hexdigest()


def check_ips(ips: bytes) -> None:
    if not (ips.startswith(b"IPS32") and ips.endswith(b"EEOF") and len(ips) == 67):
        raise ValueError("expected exact three-record IPS32 patch")
    i, records = 5, []
    while i < len(ips) - 4:
        off = int.from_bytes(ips[i:i + 4], "big") - 0x100
        n = int.from_bytes(ips[i + 4:i + 6], "big")
        records.append((off, ips[i + 6:i + 6 + n]))
        i += 6 + n
    if (i != len(ips) - 4 or len(records) != 3 or
            records[0][0] != 0x3BE6C00 or records[0][1][:8] != b"OOTGZ\0\0\0" or
            records[1][0] != 0x3BE6C40 or
            records[2] != (0x3AF2A6C, struct.pack("<I", 0x1403D075))):
        raise ValueError("1.0 hook patch differs from the reviewed layout")


def main() -> None:
    module, ips_path = BIN / "subsdk9", BIN / f"{BUILD_ID}.ips"
    report = json.loads(REPORT.read_text())
    collision = json.loads(COLLISION.read_text())
    if report.get("result") != "PASS" or not {"gameplay", "mod-menu", "ootgz-link",
                                               "ootgz-rupees-filled",
                                               "ootgz-warp-loaded-hyrule-field"}.issubset(report.get("steps", [])):
        raise ValueError("final emulator action report has not passed")
    if (report.get("ootgz_rupees_fill", {}).get("after", {}).get("value") != 200 or
            "map: -" in report.get("ootgz_link_ocr", "") or
            report.get("ootgz_audit", {}).get("warp", {}).get("map", {}).get("value") != "HyruleField_Child"):
        raise ValueError("1.0 String64 or rupee result differs from the reviewed save")
    c = collision.get("ootgz_collision", {})
    if (collision.get("result") != "PASS" or "ootgz-airwall-dots-published" not in
            collision.get("steps", []) or c.get("module_sha256") != sha(module) or
            c.get("before", {}).get("shown") != 0 or c.get("after", {}).get("shown") != 1 or
            c.get("after", {}).get("near", 0) < 1 or
            max(c.get("after", {}).get("dots", [0])) < 1):
        raise ValueError("1.0 collision overlay report does not match the current binary")
    check_ips(ips_path.read_bytes())
    if sha(module) != sha(PROFILE_EXEFS / "subsdk9") or sha(ips_path) != sha(PROFILE_EXEFS / ips_path.name):
        raise ValueError("tested emulator files differ from current build")
    OUTPUT.parent.mkdir(parents=True, exist_ok=True)
    tmp = OUTPUT.with_suffix(".tmp")
    with zipfile.ZipFile(tmp, "w", compression=zipfile.ZIP_DEFLATED, compresslevel=9) as archive:
        archive.write(module, f"atmosphere/contents/{TITLE}/exefs/subsdk9")
        archive.write(ips_path, f"atmosphere/exefs_patches/sloperinaGZ/{BUILD_ID}.ips")
        archive.write(SOURCE / "SWITCH_V0_README.txt", "README-1.0.txt")
        archive.writestr("SHA256SUMS.txt", f"{sha(module)}  subsdk9\n{sha(ips_path)}  {BUILD_ID}.ips\n")
        for name in SOURCE_FILES:
            archive.write(SOURCE / name, f"source/sloperinaGZ/{name}")
        for directory in SOURCE_DIRS:
            for path in sorted((SOURCE / directory).rglob("*")):
                if path.is_file() and "__pycache__" not in path.parts and path.suffix != ".pyc":
                    archive.write(path, "source/sloperinaGZ/" + path.relative_to(SOURCE).as_posix())
    tmp.replace(OUTPUT)
    print(json.dumps({"package": str(OUTPUT), "bytes": OUTPUT.stat().st_size,
                      "subsdk9_sha256": sha(module), "ips_sha256": sha(ips_path)}))


if __name__ == "__main__":
    main()
