# OpenCore Auxiliary Tools (OCAT) — Restored Edition

![GUI](https://github.com/user-attachments/assets/5f28f770-9248-4275-b18a-49708c5bd8ac)

This is a restored and cleaned-up edition of the **OpenCore Auxiliary Tools (OCAT)**, originally developed by **ic005k**. 

Since the original repository was abandoned, this project has been **painstakingly recreated and rebuilt directly from the original source files**. To clean up the codebase and make it fully uniform for the global community, **all Chinese language localizations and related assets have been completely removed**.

The defining philosophy of OCAT remains untouched: **One single OCAT application can manage almost all OpenCore versions, making it a true "All-in-One" solution.**

## About
OpenCore Auxiliary Tools is a GUI-based Configurator for editing `config.plist` files for Acidanthera's OpenCore Boot Manager.

Unlike other Configurator apps, OCAT doesn't mess up the config if the Devs of OpenCore add new features/keys to the config file structure. Instead, it adapts and integrates them in the interface automatically.

## Key Changes in this Edition
* **Source Restoration:** Reassembled, fixed, and compiled directly from the original source code.
* **Code Cleanup:** Completely "de-Chinesed" — all Chinese interfaces, menus, and hidden localization clutter have been removed.
* **Pure Performance:** Kept the original smart adaptation system without extra regional bloat.

## Features
Besides being a plist Configurator it can do much more:

* **Mount the EFI partition** and automatically open the `config.plist`.
* **Update and migrate configs** to the latest specs simply by clicking on the "Save" button. No more copying of keys and manually validating the config is required (huge timesaver).
* **Automatically performs config validation** and points to possible conflicts/configuration issues.
* **Sync Feature:** check for and apply updates for OpenCore, Resources, and Kexts with a few clicks.
* **Select between Official/Dev Release** and Debug builds of OpenCore.
* **Dropdown Menus** with suggested Quirks for Intel and AMD CPUs.
* **Preset Menus** for ACPI, Kernel, and other lists.
* **Editable list** with URLs of Kext Repos.
* **Integrated ASCII < > HEX Converter**.
* **Database with complete base configs** for Intel and AMD CPUs based on Dortania's OpenCore Install Guide.
* **Ability to generate EFI folders** from a config with a single click.
* **Ability to create EFI folder Backups**.


## Guides
* [OC-Little Translated: ACPI Hotpatch Samples and Guides for OpenCore](https://github.com/5T33Z0/OC-Little-Translated)
* AMD CPU BaseConfigs plug-in package, pre-selected for AMD CPU Quirks, AMD kernel patch package, Italian description documentation provided and maintained by [fabiosun](https://github.com/fabiosun).
* [OpenCore Auxiliary Tools User's Guide (by chriswayg)](https://chriswayg.gitbook.io/opencore-visual-beginners-guide/oc_auxiliary_tools)


## Credits
* [vit9696](https://github.com/vit9696) OCAT feature suggestions etc.
* [5T33Z0](https://github.com/5T33Z0) Intel CPU plug-in package for OCAT, suggestions for user interaction and functionality, Write Readme, etc.
* [fabiosun](https://github.com/fabiosun) AMD related content.
* [chriswayg](https://github.com/chriswayg) OpenCore Auxiliary Tools User's Guide.
* APP Icon Design: Mirone (Brazil).
* [OpenCore](https://github.com/acidanthera/OpenCorePkg)&nbsp; &nbsp; &nbsp; &nbsp;
[qtplist](https://github.com/reillywatson/qtplist)&nbsp; &nbsp; &nbsp; &nbsp;
[FindESP](https://github.com/bluer007/FindESP)&nbsp; &nbsp; &nbsp; &nbsp;
[winfile](https://github.com/microsoft/winfile)&nbsp; &nbsp; &nbsp; &nbsp;
[PlistCpp](https://github.com/animetrics/PlistCpp)&nbsp; &nbsp; &nbsp; &nbsp;
[pugixml](https://github.com/zeux/pugixml)&nbsp;&nbsp; &nbsp; &nbsp;
[aria2](https://github.com/aria2/aria2)&nbsp; &nbsp; &nbsp;&nbsp;
[wget](http://wget.addictivecode.org/)&nbsp; &nbsp; &nbsp;&nbsp;
[DirectionalToolTip](https://github.com/scondratev/DirectionalToolTip)&nbsp; &nbsp; &nbsp;&nbsp;
[dortania build-repo](https://github.com/dortania/build-repo)&nbsp; &nbsp; &nbsp;&nbsp;
[HackinPlugins](https://github.com/bugprogrammer/HackinPlugins)&nbsp; &nbsp; &nbsp;&nbsp;

---

## Release [list](https://github.com/YBronst/OCAuxiliaryTools/releases)
