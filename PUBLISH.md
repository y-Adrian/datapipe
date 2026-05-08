# 发布指南

## 发布流程

### 1. 创建 GitHub Release（最简单）

```bash
# 1. 更新版本号
# 编辑 CMakeLists.txt: project(DataPipe VERSION X.Y.Z ...)

# 2. 打标签
git tag v1.0.0
git push origin v1.0.0

# 3. GitHub Actions 会自动构建并上传产物
```

**使用 Release Workflow（推荐）:**
1. 点击 GitHub 仓库 → Actions → "Release" workflow
2. 点击 "Run workflow"
3. 输入版本号（如 `1.0.0`）
4. 等待构建完成，系统会创建 Draft Release
5. 检查产物无误后，点击 "Publish release"

---

### 2. Homebrew（macOS/Linux）

```bash
# 1. 创建 Formula
brew create https://github.com/y-Adrian/datapipe/archive/v1.0.0.tar.gz

# 2. 编辑 Formula（路径: $(brew --prefix)/Library/Taps/homebrew/homebrew-core/Formula/datapipe.rb）
# 参考下方模板

# 3. 测试
brew install --build-from-source ./datapipe.rb

# 4. 提交到 homebrew-core（需要 PR）
brew audit --online datapipe.rb
git push origin $(git branch | grep '*')[1]:refs/heads/homebrew
gh pr create --title "datapipe 1.0.0" --body "New release"
```

**Homebrew Formula 模板:**

```ruby
class Datapipe < Formula
  desc "High-performance C data pipeline middleware"
  homepage "https://github.com/y-Adrian/datapipe"
  url "https://github.com/y-Adrian/datapipe/archive/refs/tags/v1.0.0.tar.gz"
  sha256 "替换为实际SHA256"
  license "MIT"

  depends_on "cmake" => :build

  def install
    system "cmake", "-S", ".", "-B", "build",
                    "-DCMAKE_BUILD_TYPE=Release",
                    "-DBUILD_TESTS=OFF",
                    "-DCMAKE_INSTALL_PREFIX=#{prefix}"
    system "cmake", "--build", "build"
    system "cmake", "--install", "build"
  end

  test do
    system "ls", "#{lib}/libdatapipe.so"
  end
end
```

---

### 3. Conan（跨平台 C/C++）

```bash
# 1. 安装 Conan
pip install conan

# 2. 添加远程仓库（可选，发布到 Conan Center）
conan export . datapipe/1.0.0@y-adrian/stable

# 3. 上传到 Conan Center（需要 PR）
# https://github.com/conan-io/conan-center-index
```

**使用 Conan:**
```bash
# 安装依赖
conan install datapipe/1.0.0@ -r conan-center

# 在 CMakeLists.txt 中使用
find_package(datapipe REQUIRED)
target_link_libraries(myapp datapipe::datapipe)
```

---

### 4. vcpkg（Windows C++）

```bash
# 1. Fork https://github.com/microsoft/vcpkg
# 2. 在 ports/ 目录下创建 datapipe 文件夹
# 3. 提交 PR

# 或者本地安装
vcpkg install datapipe
```

---

### 5. Linux 包（AUR, .deb, .rpm）

**Arch Linux (AUR):**
```bash
# 创建 PKGBUILD
git clone https://aur.archlinux.org/datapipe.git
cd datapipe
makepkg -si
```

**Debian/Ubuntu (.deb):**
```bash
# 使用 dh_make + debuild
apt-get install build-essential debhelper
dh_make -t -y
dpkg-buildpackage -b -us -uc
```

---

## 多平台预编译包

Release workflow 会自动构建:

| 平台 | 文件 |
|------|------|
| Linux x86_64 | `datapipe-X.Y.Z-linux-x86_64.tar.gz` |
| Linux ARM64 | `datapipe-X.Y.Z-linux-aarch64.tar.gz` |
| macOS Universal | `datapipe-X.Y.Z-macos-universal.tar.gz` |
| Windows x64 | `datapipe-X.Y.Z-windows-x64.tar.gz` |

---

## 安装示例

```bash
# Linux/macOS
tar -xzf datapipe-1.0.0-linux-x86_64.tar.gz
cd usr
sudo cp -r include/* /usr/local/include/
sudo cp -r lib/* /usr/local/lib/
sudo ldconfig

# CMAKE 使用
find_package(datapipe REQUIRED)
target_link_libraries(myapp datapipe)
```
