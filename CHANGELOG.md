# Changelog

## Unreleased

- [FO4] Remove TAA, optimized FXAA. For more quality pics.
- [FO4] Added hide/show ground plane in Model Window preview.
- [FO4] Fixed behavior graph for animation fire or another thing, now animation works with hk enabled.
- [SSE] Fixed AA when active gizmo mode.
- [SSE] Fixed toggled Markers in Show/Hide window.

## SSE 0.6 b821
- [SSE] Remove TAA, optimized FXAA. For more quality pics.
- [SSE] Added hide/show ground plane in Model Window preview.
- [SSE] Fixed BSString::Append for BSStringMemoryLeaks patch, now BSString truncate more than 64K string and fixed crashes CK.
- [SSE] Apply AA for preview window's.
- [SSE] Saves OFST for plugins.
- [SSE] Fixed the Render Window after leaving the orthographic view: the near clip stayed at 1000, resizing in ortho needed two presses of the toggle, and the title kept the old view mode.
- [SSE] Fixed the crash and corrupted plugin when saving a door with entries in "Randomly Teleports to these Interiors / Worldspaces only" (#59).