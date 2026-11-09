/* Addresses for Ocarina of Time (Switch 2), program 0400486024DEC000, cartridge v0,
 * main build 33FA6C2C23848282A77E8B88A0A93F4887C55920.
 *
 * Every value is an offset into the main module (main+0x...), the same convention as
 * docs/FINDINGS.md. The module finds main's base at run time from the nnMain export.
 * "*" in a comment = one pointer dereference of main+OFF, "**" = two.
 */
#ifndef Z64HD_V0_H
#define Z64HD_V0_H

#define Z64HD_BUILD_ID          "33FA6C2C23848282A77E8B88A0A93F4887C55920"
#define Z64HD_PROGRAM_ID        "0400486024DEC000"

/* module layout ---------------------------------------------------------------------- */
#define Z64HD_NNMAIN            0x11C1FB0   /* exported by main; base = &nnMain - this */
#define Z64HD_CAVE_START        0x3BE6C00   /* mapped RX padding after .text; validated against exact v0 main */
#define Z64HD_CAVE_END          0x3BE7000   /* next mapped segment */
/* The slot follows MOD0's declared BSS end 0x4e2a068 and lies inside the
 * NSO-mapped BSS ending at 0x4e30000. Live use still requires a watchpoint. */
#define Z64HD_HOOK_SLOT         0x4E2A070

/* frame hook (FINDINGS §3) -------------------------------------------------------------- */
/* Every frame main+0x3af2a30 locks GameData, applies its command queue (main+0xb9a200)
 * and tail-calls the unlock at this site. The IPS sends that tail call to the cave stub. */
#define Z64HD_FRAME_SITE        0x3AF2A6C
#define Z64HD_FRAME_SITE_ORIG   0x1403C02Du /* b main+0x3be2b20 */
#define Z64HD_GD_UNLOCK         0x3BE2B20   /* (lock) */

/* GameData (FINDINGS §1, §4) ------------------------------------------------------------ */
#define Z64HD_GD_MGR_PTR        0x4C184D8   /* ** = GameData manager */
#define Z64HD_GD_GETINT         0x420D50    /* bool (mgr, s32 *out, u32 hash) */
#define Z64HD_GD_SETINT         0x4211F0    /* void (mgr, s32 value, u32 hash): queued, applied at the flush */
#define Z64HD_GD_INT_TYPE       0xC8        /* manager+: the Int type entry (see gamedata.h) */
#define Z64HD_GD_TYPES_SCAN     0x800       /* other type entries are found by vtable within manager+0..this */
#define Z64HD_VT_INT_FIELD      0x4BBAB10   /* vtable of each Int field object (not the type entry) */
#define Z64HD_VT_BOOL_TYPE      0x4BB9538
#define Z64HD_VT_FLOAT_TYPE     0x4BB97B8
#define Z64HD_VT_ENUM_TYPE      0x4BB98F8
/* String64 descriptor at manager+0x608; current buffer pointer at record+0x68. */
#define Z64HD_STR_ANCHOR_HASH   0x1D6189DAu /* Sequence_CurrentBanc */
#define Z64HD_STR_ANCHOR_IDX    0x1D
#define Z64HD_STR_FIRST_VALUE   0x207C     /* used by update path only */
#define Z64HD_STR_STRIDE        0x108

/* pouch (inventory) code (FINDINGS §3, §4) ---------------------------------------------- */
#define Z64HD_POUCH_MGR_PTR     0x4C18428   /* ** = pouch manager */
#define Z64HD_POUCH_ADD         0x11378F0   /* see pouch.c */
#define Z64HD_POUCH_NOTIFY      0x0000000
#define Z64HD_POUCH_NOTIFY_PTR  0x0000000   /* unresolved on v0; caller skips notification */
#define Z64HD_POUCH_REMOVE      0x33456F0   /* (pouch mgr, &name, count, 0, 0, 0) */

/* Link's gear (FINDINGS §5.1, §5.4) ----------------------------------------------------- */
#define Z64HD_EQUIP_SLOT_FN     0x2DA26F0   /* (equipment component, slot) */
#define Z64HD_EQUIP_ALL_FN      0xEE0840    /* (owner, 0): re-apply all, takes off empty slots */
#define Z64HD_VT_EQUIP          0x4AE4690   /* equipment component; +0x18 = owning actor */
#define Z64HD_VT_EQUIP_OWNER    0x4B7AA20   /* holds the component at +0x470 */

/* UI (FINDINGS §4) ---------------------------------------------------------------------- */
#define Z64HD_UI_MGR_PTR        0x4C183F0   /* ** = UI manager; +8 count, +0x10 screens */
#define Z64HD_VT_DPAD           0x4B7BC20   /* D-pad widget, at HUD screen +0x330 */
#define Z64HD_VT_HUD            0x4B85EB0   /* PlayerStatus screen; method 98 = main+0xa03f50 */
#define Z64HD_VT_MAP_SCREEN     0x4B82AA0   /* pause map screen; method 98 = main+0x3380f20 */
#define Z64HD_UI_REFRESH        (0x310 / 8) /* vtable slot 98: "refresh contents" */

/* events (warps) -------------------------------------------------------------------------- */
/* How an exit starts a map change: FieldAndDungeonEntranceEventRequestActor requests event
 * "Dm_SY_BancChangeGo" with string params NextBanc / NextStartPos / NextStartPosAir /
 * BootEventName and bool IsCheckAir (built by QueryBancChangeEventRequestArg, main+0x29bf380);
 * the flow's GameSystemActor.EventReserveBancChange then does the rest. Requests are made like
 * the SeqOneShotRequestEvent node does (main+0x32db1a0). */
#define Z64HD_EVENT_MGR_PTR     0x4C183E8   /* ** = event manager */
#define Z64HD_EVENT_REQ_CTOR    0xD3C9A0    /* (request) */
#define Z64HD_EVENT_REQ_SUBMIT  0xD3D2B0    /* bool (event manager, request) */
#define Z64HD_EVENT_REQ_SUB_DTOR 0x27C630   /* destructor of the two sub-objects below */
#define Z64HD_EVENT_REQ_SUB_VT  0x49101E8   /* (+0x10) their base vtable, set before destroying */
#define Z64HD_EVENT_REQ_SIZE    0x1B20
#define Z64HD_EVENT_REQ_PARAMS  0x08        /* param entry array (32 inline entries at +0x18) */
#define Z64HD_EVENT_REQ_NPARAMS 0x10        /* u32 count; capacity (u32) at +0 */
#define Z64HD_EVENT_REQ_SUB_A   0x1950
#define Z64HD_EVENT_REQ_SUB_B   0x1968
#define Z64HD_EVENT_REQ_NAME    0x1A90      /* char *event name; capacity (u32) at +0x1A98 */
#define Z64HD_EVENT_PARAM_SIZE  0xC8        /* entry: u32 type, char *name (+8, cap u32 +0x10), value +0x98 */
#define Z64HD_EVENT_PARAM_STRING 1          /* value: char * */
#define Z64HD_EVENT_PARAM_BOOL   4          /* value: u8 */

/* Link, camera, frame (game.py) --------------------------------------------------------- */
#define Z64HD_VT_LINKOBJ        0x4AC0E30
#define Z64HD_VT_PARAM          0x4AFE430
#define Z64HD_DISP_PTR          0x4C18A98   /* ** + 0x29C = Link's drawn position (3 floats) */
#define Z64HD_VIEW_REG_CELL     0x4C18BD8   /* ** -> view-name registry, +0x38 = hash buckets */
#define Z64HD_VIEW_MGR_CELL     0x4C20ED8   /* ** -> indexed view manager */
#define Z64HD_FALLBACK_CAMERA   0x4E11838   /* static camera selected when a view has none */
#define Z64HD_FRAME_COUNTER     0x4E14740   /* u32, +1 per update; runs in the pause menus too */

/* (main+a) -> +b -> Link's actor, tried in order; entries 2 and 5 provisional */
#define Z64HD_LINKOBJ_PTRS \
  { { 0x4DC7210, 0x480 }, { 0x4E23158, 0xEF8 }, { 0x4E24400, 0x328 } }

#endif
