# openFrameworks Creative Projects Collection
# openFrameworks クリエイティブプロジェクトコレクション

A comprehensive collection of creative coding experiments and generative art projects built with openFrameworks 0.12.0.

openFrameworks 0.12.0を使用したクリエイティブコーディング実験とジェネラティブアートプロジェクトの包括的なコレクションです。

## 🎨 About This Collection | このコレクションについて

This repository contains over 130 creative coding projects spanning from early 2023 to 2025, showcasing various techniques in generative art, interactive graphics, and computational design using openFrameworks.

このリポジトリには2023年初頭から2025年にかけての130以上のクリエイティブコーディングプロジェクトが含まれており、openFrameworksを使用したジェネラティブアート、インタラクティブグラフィックス、コンピュテーショナルデザインの様々な技法を紹介しています。

### Recent Highlights | 最新のハイライト
- **20250908**: Generative Grid Art - Dynamic pattern generation with noise-based color theming and particle systems | ジェネラティブグリッドアート - ノイズベースのカラーテーマとパーティクルシステムによる動的パターン生成
- **20250901**: Interactive visual experiments with advanced blending modes | 高度なブレンドモードを使用したインタラクティブビジュアル実験
- **20250831**: Procedural art generation with custom algorithm implementations | カスタムアルゴリズム実装によるプロシージャルアート生成

## 🛠️ Technical Setup | 技術的なセットアップ

### Requirements | 必要環境
- **openFrameworks**: Version 0.12.0 | バージョン 0.12.0
- **Platform**: macOS (Darwin) | プラットフォーム: macOS (Darwin)
- **IDE**: Xcode (projects include `.xcodeproj` files) | IDE: Xcode（`.xcodeproj`ファイルを含む）
- **C++**: Modern C++ with openFrameworks framework | モダンC++ with openFrameworksフレームワーク

### Project Structure | プロジェクト構造
Each project follows the standard openFrameworks structure: | 各プロジェクトは標準的なopenFrameworksの構造に従っています：
```
projectName/
├── src/
│   ├── ofApp.h          # Main application header
│   ├── ofApp.cpp        # Main application implementation
│   └── main.cpp         # Entry point
├── bin/                 # Compiled binaries and resources
├── projectName.xcodeproj # Xcode project file
├── Makefile            # Make build configuration
├── addons.make         # openFrameworks addons
└── config.make         # Project configuration
```

## 🚀 Getting Started | はじめに

### Prerequisites | 前提条件
1. Download and install [openFrameworks 0.12.0](https://openframeworks.cc/download/) | [openFrameworks 0.12.0](https://openframeworks.cc/download/)をダウンロードしてインストール
2. Ensure Xcode is installed with command line tools | Xcodeとコマンドラインツールがインストールされていることを確認
3. Clone or navigate to this project collection | このプロジェクトコレクションをクローンまたはナビゲート

### Running a Project | プロジェクトの実行
```bash
# Navigate to any project directory
cd 20250908

# Compile and run using make
make && make RunRelease

# Or open in Xcode
open 20250908.xcodeproj
```

## 📁 Project Organization

Projects are organized chronologically with the naming convention `YYYYMMDD` (and version suffixes like `-v2`, `-v3` for iterations):

- **2023**: Early experimental work
- **2024**: Major development period with consistent daily practice
- **2025**: Recent advanced implementations

### Key Project Categories

**Generative Art**: Projects featuring algorithmic art generation
- Pattern-based compositions
- Noise-driven animations
- Procedural color schemes

**Interactive Graphics**: Mouse/keyboard responsive visualizations
- Real-time parameter control
- Interactive drawing tools
- Responsive animations

**Particle Systems**: Dynamic particle-based simulations
- Physics-based animations
- Emergent behaviors
- Complex visual effects

## 🎯 Common Techniques Used

### Graphics & Rendering
- **Alpha blending** with `ofEnableAlphaBlending()`
- **Additive blending** for luminous effects
- **Anti-aliasing** for smooth graphics
- **Transform matrices** for complex positioning

### Animation & Time
- **Perlin noise** for organic movement patterns
- **Sine/cosine waves** for rhythmic animations
- **Frame-based timing** with `ofGetLastFrameTime()`
- **Multi-layered animation** combining different time scales

### Visual Elements
- **Dynamic color palettes** using hex color schemes
- **Geometric primitives** (circles, rectangles, triangles)
- **Custom shape generation** with `ofBeginShape()/ofEndShape()`
- **Grid-based layouts** for systematic compositions

## 🔧 Development Workflow

### Typical Development Cycle
1. **Experimentation**: Start with basic shapes and colors
2. **Iteration**: Create versioned copies (`-v2`, `-v3`) for major changes
3. **Refinement**: Polish timing, colors, and interaction
4. **Documentation**: Comment complex algorithms

### Code Style Conventions
- Modern C++ with `override` specifiers
- Consistent function organization following oF structure
- Descriptive variable names for creative parameters
- Modular design with private helper methods

## 🎨 Visual Themes

### Color Palettes
Most projects feature carefully curated color schemes:
- **Purple/Blue gradients**: Deep purples to bright blues
- **Monochromatic variations**: Single hue with varying saturation
- **High contrast**: Dark backgrounds with bright elements

### Animation Patterns
- **Organic motion**: Noise-based movement feeling natural
- **Rhythmic pulsing**: Time-based scaling and opacity changes
- **Layered complexity**: Multiple animation systems working together

## 📚 Learning Resources

### openFrameworks Documentation
- [Official Documentation](https://openframeworks.cc/documentation/)
- [Learning Resources](https://openframeworks.cc/learning/)
- [Community Forum](https://forum.openframeworks.cc/)

### Creative Coding References
- Generative art principles
- Mathematical functions for animation
- Color theory for digital art

## 🗂️ Project Index

Browse projects by date or explore the most recent developments in the `202509XX` folders for the latest techniques and implementations.

---

*This collection represents an ongoing exploration of creative coding, computational art, and the expressive potential of algorithmic design.*

*このコレクションはクリエイティブコーディング、コンピュテーショナルアート、そしてアルゴリズムデザインの表現可能性の継続的な探求を表しています。*

## Language / 言語
- [English README](README.md)
- [日本語 README](README_JP.md)