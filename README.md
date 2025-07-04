# WinAPI-Display-Rotation
Display Rotation Utility with Multi-Monitor Support (Windows)

This utility allows you to programmatically rotate the screen orientation on Windows using native WinAPI calls. It intelligently detects if a second monitor is connected and applies the rotation to the second monitor; if no second monitor is found, it rotates the primary display instead.

Features:

Detects connected monitors and selects the appropriate target display

Supports rotation angles: 0°, 90°, 180°, and 270°

Uses ChangeDisplaySettingsEx to apply orientation changes without affecting other monitors

Handles resolution adjustments automatically when rotating between portrait and landscape modes

Designed as a lightweight, no-console GUI application for seamless integration

Use cases include automated screen rotation scripts, multi-monitor setup management, kiosk applications, and custom display configuration tools.
