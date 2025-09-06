# 🎵 MP3 Tag Reader & Editor (ID3v2.3)

A simple C program to **read** and **edit** ID3v2.3 metadata from `.mp3` audio files via the terminal — enhanced with a few advanced features for safer editing and better usability.



## 📁 Project Structure

```

.
├── main.c          # Main CLI handler (view/edit/list/dry-run)
├── tag.c           # Core logic for reading, writing, listing frames
├── tag.h           # Header file with declarations and structures
├── sample.mp3      # (optional) Sample MP3 for manual testing — *do not upload large audio files to GitHub*
└── README.md       # You're reading it!

````

---

## ✅ Supported ID3 Tags (common)
- **Title**       (`TIT2`)
- **Artist**      (`TPE1`)
- **Album**       (`TALB`)
- **Year**        (`TYER`)
- **Composer**    (`TCOM`)
- **Genre**       (`TCON`)



---

## ⚙️ How to Compile

```bash
gcc main.c tag.c -o mp3tag -std=c99 -Wall -Wextra
````

---

## ▶️ How to Run

### View tags (pretty)

```bash
./mp3tag -v file.mp3
```

### List all frames (raw frame IDs & sizes)

```bash
./mp3tag -l file.mp3
```

### Edit a field (safe edit with backup)

```bash
./mp3tag -e -t "My Song" file.mp3           # edit title
./mp3tag -e -a "Rehan M." file.mp3          # edit artist
./mp3tag -e -y 2025 file.mp3                # edit year
```

### Batch edit (globbing supported by your shell)

```bash
./mp3tag -e -g "Electronic" *.mp3
```

### Dry-run (preview changes)

```bash
./mp3tag --dry -e -A "Greatest Hits" file.mp3
```

---

### CLI Flags & Options

| Flag            | Action          | Field / Note                                |
| --------------- | --------------- | ------------------------------------------- |
| `-v`            | View tags       | Pretty output                               |
| `-l`            | List frames     | Shows all frame IDs, sizes                  |
| `-e`            | Edit mode       | Requires a field flag + value + filename(s) |
| `--dry`         | Dry run         | Simulate edits without writing              |
| `-t`            | Title (TIT2)    | `-e -t "New Title" file.mp3`                |
| `-a`            | Artist (TPE1)   | `-e -a "Artist Name" file.mp3`              |
| `-A`            | Album (TALB)    | `-e -A "Album Name" file.mp3`               |
| `-y`            | Year (TYER)     | 4-digit year only                           |
| `-c`            | Composer (TCOM) | Composer string                             |
| `-g`            | Genre (TCON)    | Genre string                                |
| `-h` / `--help` | Help            | Show usage                                  |

---

## 🚨 Validations & Limits

* **Year**: must be 4 digits (e.g., `2025`). Non-numeric or wrong-length will be rejected.
* **Forbidden chars**: tags will be sanitized; certain control characters are removed.
* **Max lengths**:

  * Year: **4** chars
  * Genre: **64** chars
  * Other text frames: **1024** chars (hard cap to avoid excessive rewrites)
* The program only edits files with a valid **ID3v2.3** header. If an MP3 has ID3v2.4 or ID3v2.2, you'll be warned and asked to convert first.

---

## 💡 Implementation Notes (how edits work)

1. Program reads the ID3v2 header (10 bytes) and verifies version `0x03` (ID3v2.3).
2. Parses frames sequentially to find the target frame (e.g., `TYER`).
3. Computes the size difference between new and old frame content.
4. If new content is longer:

   * Creates a temp file `file.mp3.tmp`
   * Writes updated tag header with new total tag size
   * Writes updated frames (modified frame with new size)
   * Streams remaining audio bytes (no full re-reading into memory for large files)
   * fsyncs and atomically renames temp file to original after creating a `.bak` backup
5. If new content is shorter or equal, updates in-place in temp file with the same atomic replacement strategy.
6. On success, prints a summary table with old/new sizes and locations.

---

## 🛠 Dependencies & Compatibility

* Written in **C (C99)** using standard libraries: `<stdio.h>`, `<stdlib.h>`, `<string.h>`, `<stdint.h>`, `<errno.h>`, `<unistd.h>`, `<fcntl.h>`, `<sys/stat.h>`.
* Designed for **GCC on Linux** (should compile on macOS with minor adjustments).
* No external libraries required.

---

## 🔁 Backup & Restore

* Before any edit, a backup `<filename>.mp3.bak` is created.
* To restore:

```bash
cp file.mp3.bak file.mp3
```

---

## 🧭 Troubleshooting

* If the program warns "Not ID3v2.3", use an external tool (e.g., `eyeD3` or `ffmpeg`) to convert or remove tags before using this tool.
* If an edit fails mid-way, the original `.bak` will remain. Do **not** delete `.bak` until you're satisfied.

---

## ✅ Example Workflows

View tags:

```bash
./mp3tag -v sample.mp3
```

Edit year with dry-run first:

```bash
./mp3tag --dry -e -y 2025 sample.mp3
# If output looks good:
./mp3tag -e -y 2025 sample.mp3
```

Batch set genre:

```bash
./mp3tag -e -g "Bollywood" track1.mp3 track2.mp3 track3.mp3
```

---

## 🧾 Author & Context

**Rehan Mokashi**
Emertxe – Embedded Systems & IoT Trainee
*During my Emertxe journey I made this project to learn practical file I/O, frame-level manipulation, and safe binary editing in C.*

[GitHub](https://github.com/rmokashi01) | [LinkedIn](https://www.linkedin.com/in/rehan-mokashi-7b32472a2/)

---

## 📜 License

MIT License — Free to use and modify. See `LICENSE` for details.

---

## ⚠️ Notes for GitHub repo

* **Do not commit large `.mp3` files**. Instead commit only code and a small, intentionally permissive sample or instructions for how to test locally.
* Add a `.gitignore`:

```
a.out
*.o
*.mp3
*.mp3.bak
.vscode/
*.tmp
```

