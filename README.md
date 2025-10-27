# openFrameworks Creative Projects Collection

A curated archive of creative coding sketches built with openFrameworks 0.12.0. The repository spans from early 2023 through 2025 and now includes more than 130 explorations in generative art, interactive graphics, and computational design on macOS.

## Highlights
- **2025-09-08** – Generative grid study with noise-driven palettes, lightweight particle systems, and evolving compositions.
- **2025-09-01** – Interactive blends experiment showcasing custom blend modes and responsive inputs.
- **2025-08-31** – Procedural art series driven by bespoke algorithmic rules and palette modulation.

## Requirements
- openFrameworks 0.12.0
- macOS (Darwin), tested with Xcode toolchain
- Xcode with command line tools or the standard openFrameworks make system

## Project Layout
Each sketch follows the conventional openFrameworks folder structure:
```
projectName/
├── src/
│   ├── ofApp.h          # Main application header
│   ├── ofApp.cpp        # Main application implementation
│   └── main.cpp         # Entry point
├── bin/                 # Compiled binaries and assets
├── projectName.xcodeproj # Xcode project file
├── Makefile             # Make build configuration
├── addons.make          # openFrameworks addon declarations
└── config.make          # Project configuration
```

## Getting Started
1. Install [openFrameworks 0.12.0](https://openframeworks.cc/download/) and extract it into your development directory.
2. Ensure Xcode (with command line tools) or the openFrameworks make toolchain is available on your machine.
3. Clone or copy this project collection into the `apps/myApps/` directory inside your openFrameworks installation.

### Run a Project
```bash
# Navigate to one of the dated project folders
cd 20250908

# Build and run from the command line
make && make RunRelease

# Or open the Xcode project to run from the IDE
open 20250908.xcodeproj
```

## Repository Organization
Projects are named with the `YYYYMMDD` convention. Incremental variations add suffixes such as `-v2`, `-v3`, or `_v2`.

- **2023** – Early experiments laying the foundation for daily creative practice.
- **2024** – Rapid iteration period exploring color, rhythm, and interaction.
- **2025** – Latest studies featuring refined algorithms and layered animation systems.

## Project Themes
- **Generative Art** – Pattern construction, noise-driven motion, and procedural palettes.
- **Interactive Graphics** – Mouse and keyboard responsive sketches with real-time controls.
- **Particle Systems** – Physics-inspired simulations and emergent visual behavior.

## Techniques at a Glance
- **Graphics & Rendering** – Alpha and additive blending, anti-aliasing, and custom transformation matrices.
- **Animation & Timing** – Perlin noise, trigonometric motion, and frame-time aware sequencing.
- **Visual Building Blocks** – Geometric primitives, custom shapes with `ofBeginShape()/ofEndShape()`, and grid-based compositions.

## Preferred Workflow
1. **Experiment** – Start with simple geometry, motion, or palette explorations.
2. **Iterate** – Duplicate a project (e.g., append `-v2`) when trying a different direction.
3. **Refine** – Balance timing, color, and interactions for polished presentation.
4. **Document** – Annotate complex algorithms or parameter maps for future reference.

## Learning Resources
- [openFrameworks Documentation](https://openframeworks.cc/documentation/)
- [Learning Resources](https://openframeworks.cc/learning/)
- [Community Forum](https://forum.openframeworks.cc/)
- Generative art references, animation mathematics, and digital color theory texts

## Language
- English README (this document)
- [日本語 README](README_JP.md)

*This collection documents an ongoing exploration of creative coding, computational art, and the expressive potential of algorithmic design.*
