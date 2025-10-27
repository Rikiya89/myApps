#!/bin/bash

# openFrameworks Project Browser / openFrameworks プロジェクトブラウザ
# Quick navigation and information script for project collection
# プロジェクトコレクションのためのクイックナビゲーション・情報表示スクリプト

echo "🎨 openFrameworks Project Collection Browser"
echo "🎨 openFrameworks プロジェクトコレクション ブラウザ"
echo "=============================================="

# Function to get project info
get_project_info() {
    local project_dir="$1"
    if [ -d "$project_dir/src" ]; then
        local main_file="$project_dir/src/ofApp.cpp"
        if [ -f "$main_file" ]; then
            # Extract window title if available
            local title=$(grep -o 'ofSetWindowTitle("[^"]*")' "$main_file" | sed 's/ofSetWindowTitle("//g' | sed 's/")//g')
            if [ -n "$title" ]; then
                echo "  📝 Title / タイトル: $title"
            fi
            
            # Check for interesting techniques / 興味深い技法をチェック
            if grep -q "ofNoise" "$main_file"; then
                echo "  🌊 Uses / 使用技術: Perlin noise / パーリンノイズ"
            fi
            if grep -q "particle" "$main_file" -i; then
                echo "  ✨ Uses / 使用技術: Particle systems / パーティクルシステム"
            fi
            if grep -q "OF_BLENDMODE" "$main_file"; then
                echo "  🎭 Uses / 使用技術: Blend modes / ブレンドモード"
            fi
        fi
    fi
}

# List recent projects (last 10) / 最新プロジェクト一覧（最新10件）
echo ""
echo "📅 Recent Projects (Last 10) / 最新プロジェクト（最新10件）:"
echo "--------------------------------------------------------"
ls -dt */ | head -10 | while read dir; do
    project_name=${dir%/}
    echo "🗂️  $project_name"
    get_project_info "$project_name"
    echo ""
done

echo ""
echo "🔍 Commands / コマンド:"
echo "  ls                     - List all projects / 全プロジェクト一覧"
echo "  cd [project]           - Enter project directory / プロジェクトディレクトリに移動"  
echo "  make && make RunRelease - Build and run project / プロジェクトをビルドして実行"
echo "  open [project].xcodeproj - Open in Xcode / Xcodeで開く"

echo ""
echo "💡 Pro tip / プロのコツ: Use 'find . -name \"*.cpp\" | xargs grep -l \"keyword\"' to find projects using specific techniques!"
echo "   特定の技法を使用しているプロジェクトを見つけるには上記のコマンドを使用してください！"