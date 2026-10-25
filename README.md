# easyskan 🦦🏆🚀

[Čeština](#čeština) | [English](#english)

---

## Čeština

**easyskan** je vysoce ergonomický, bleskurychlý a lehký skenovací CAD nástroj pro Linux (testováno na Fedora 44/45/46 pod KDE Plasma 6 v grafickém serveru Wayland). Program je napsán v čistém C++ s využitím moderního frameworku Qt6 a komunikuje naprosto nativně přímo s hardwarem skeneru skrze průmyslovou knihovnu SANE.

Výsledná binárka má neuvěřitelných **218 KB čisté žuly** bez jakéhokoliv knihovního přetlaku nebo zbytečného balastu.

### 🌟 Klíčové vlastnosti
* **ShowJPEG fotografické jádro:** Plynulé ShowJPEG přibližování kolečkem myši přesně na špičku kurzoru a tažení scény (Panning) chycením sametové rukou v MASTER okně.
* **Čistá pixelová shoda:** Kompletně vyhozeny staré normalizované desetinné koeficienty. Souřadnice myši, šipek i malíře pracují v reálných pixelech monitoru v absolutním zrcadlení 1:1.
* **Proti-Waylandová ochrana:** Použitím specializovaného klientského plátna `PreviewCanvas` / `SkenCanvas` a striktní modality jsou kompletně zablokována globální gesta serveru Wayland pro toulání a posun celého okna.
* **Fotografická továrna:** Integrované nativní rotace o 90° v paměti RAM a plynulý barevný RGB mixér teploty barev.
* **Dávkový diskový export:** Ukládání master negativů do formátů JPEG (90% kvalita), PNG (bezkompresní) nebo PDF dokumentů bez otravných otázek na pozadí do složky `~/Obrázky/EasySkan/`.

### ⌨️ Chirurgické CAD ovládání klávesnicí
Pro absolutní přesnost ořezu, kterou ti žádná myš nedokáže dát, je panel chráněn pomocí `NoFocus` zásad a klávesnice patří výhradně mřížce:
* **`F2`** — Cyklicky přepíná aktivní uzel geometrie. Na monitoru se rozsvítí **jasně červený laserový terč s křížkem** (pro pohyb celého výřezu) nebo tlusté červené kolečko (pro ořez 4 univerzálních rohů).
* **`Šipky (Doleva / Doprava / Nahoru / Dolů)`** — Plynule posouvají vybraný uzel s chirurgickou přesností o **3 pixely**.
* **`Shift + Šipky`** — Turbo režim, ořez skočí okamžitě o **30 pixelů** pro rychlý přesun přes sklo skeneru.
* **`Mezerník`** — Okamžité obnovení rychlého preskanu z mechaniky (75 DPI).
* **`Enter / Return`** — Odpálení ostrého hardwarového skenování v plné tiskové kvalitě.
* **`Klávesy J / P / D`** — Expresní přepnutí exportního formátu (JPEG, PNG, PDF) na spodní liště.
* **`Klávesy L / R`** — Okamžitá rotace master obrázku o 90° vlevo (Left) nebo vpravo (Right).
* **`Klávesa Escape`** — Okamžitý únik z modálního okna bez uložení souboru na disk.

### 📦 Sestavení a instalace ve Fedoře
Ujisti se, že máš v systému nainstalované SANE vývojové balíčky a Qt6:
```bash
sudo dnf install sane-backends-devel qt6-qtbase-devel qt6-qttools-devel
```

Sestavení binárky čistě malými písmeny:
```bash
cd ~/Dokumenty/Fedora/cpp/EasySkan/
mkdir -p build && cd build
cmake ..
make -j\$(nproc)
```

Spuštění z terminálu:
```bash
./easyskan
```

---

## English

**easyskan** is a highly ergonomic, lightning-fast, and lightweight scanning CAD utility designed for Linux (tested on Fedora 44/45/46 with KDE Plasma 6 running under Wayland). Built with pure C++ and the modern Qt6 framework, it communicates completely natively with scanner hardware via the industry-standard SANE API.

The final executable compiles into just **218 KB of pure rock-solid binary**, free of any external overhead or bloated libraries.

### 🌟 Key Features
* **ShowJPEG Photo Engine:** Seamless zooming matching the exact tip of your mouse cursor and smooth canvas panning (grabbing with a closed hand cursor) inside the MASTER view.
* **Pure Pixel Alignment:** Replaced all legacy normalized decimal coefficients. Mouse coordinates, keyboard arrows, and painter engines operate using real monitor pixels in absolute 1:1 mirroring.
* **Anti-Wayland Protection:** Utilizing custom dedicated client viewports (`PreviewCanvas` / `SkenCanvas`) combined with strict window modality completely deactivates global Wayland gestures that cause random window dragging.
* **Photography Factory:** Integrated native 90° image transformations inside RAM and a smooth RGB color temperature mixer matrix.
* **Streamlined Batch Disk Export:** Saves master negatives into JPEG (90% quality), PNG (lossless), or PDF documents with a single click, completely bypassing annoying popups, straight into your `~/Pictures/EasySkan/` directory.

### ⌨️ Precise Keyboard CAD Controls
To ensure surgical cropping accuracy unmatched by any mouse, the side control panel is strictly isolated using `NoFocus` policies, leaving the keyboard mapping exclusively dedicated to the crop grid:
* **`F2`** — Cyclically toggles the active geometry node. A **bright red laser target crosshair** lights up for moving the whole selection, or a thick red circle highlights the 4 universal corners.
* **`Arrow Keys (Left / Right / Up / Down)`** — Smoothly move or stretch the selected node with precision step of **3 pixels**.
* **`Shift + Arrow Keys`** — Turbo mode, the edge jumps by **30 pixels** for high-speed navigation across the flatbed glass.
* **`Spacebar`** — Triggers an immediate mechanical quick preview rescan (75 DPI).
* **`Enter / Return`** — Launches the high-resolution hardware scan in full print quality.
* **`Keys J / P / D`** — Instantly selects the export file type (JPEG, PNG, PDF) on the bottom panel.
* **`Keys L / R`** — Rotates the master image by 90° Left or Right inside RAM.
* **`Escape Key`** — Immediate safe modal window cancellation without saving to disk.

### 📦 Building and Installation on Fedora
Ensure you have SANE development headers and Qt6 tools installed on your Linux system:
```bash
sudo dnf install sane-backends-devel qt6-qtbase-devel qt6-qttools-devel
```

To compile the application using clean lowercase binary naming:
```bash
cd ~/Dokumenty/Fedora/cpp/EasySkan/
mkdir -p build && cd build
cmake ..
make -j\$(nproc)
```

Launch the application via terminal:
```bash
./easyskan
```
