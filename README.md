
# 🎵 MP3 Tag Reader & Editor (ID3v2.3)

A simple C program to **read** and **edit** metadata (ID3v2.3 tags) from `.mp3` audio files using the terminal.

---

## 📁 Project Structure

```
.
├── main.c          # Main CLI handler (view/edit tags)
├── tag.c           # Core logic for reading and writing tags
├── tag.h           # Header file with declarations and structures
├── sample.mp3      # Sample MP3 file to test
└── README.md       # You're reading it!
```

---

## ✅ Supported ID3 Tags

- **Title**       (`TIT2`)
- **Artist**      (`TPE1`)
- **Album**       (`TALB`)
- **Year**        (`TYER`)
- **Composer**    (`TCOM`)
- **Genre**       (`TCON`)

---

## ⚙️ How to Compile

```bash
gcc main.c tag.c -o mp3tag
```

---

## ▶️ How to Run

### 1. **View Tags**

```bash
./mp3tag -v sample.mp3
```

#### Sample Output:

```
Title       : Sunny Sunny
Artist      : Yo Yo Honey Singh
Album       : Yaariyan
Year        : 2013
Composer    : Mithoon
Content Type: Bollywood Music
```

---

### 2. **Edit Tags**

```bash
./mp3tag -e -<option> <new_value> <file.mp3>
```

### Options:

| Flag | Field    | Example                      |
|------|----------|------------------------------|
| `-t` | Title    | `./mp3tag -e -t "My Song" sample.mp3` |
| `-a` | Artist   | `./mp3tag -e -a "Rehan M." sample.mp3` |
| `-A` | Album    | `./mp3tag -e -A "Greatest Hits" sample.mp3` |
| `-y` | Year     | `./mp3tag -e -y 2025 sample.mp3` |
| `-c` | Composer | `./mp3tag -e -c "A. R. Rahman" sample.mp3` |
| `-g` | Genre    | `./mp3tag -e -g "Electronic" sample.mp3` |

---

## 🚨 Validations

- Year must be **4 digits only**
- Tags should not contain **special characters** like `@`, `#`, `!`, etc.
- Max length for:
  - Year: 4 chars
  - Genre: 64 chars
  - Others: 128 chars

---

## 💡 Implementation Notes

- Works only on MP3 files with **ID3v2.3** tag format.
- If new value is longer than old value, it **creates a new temp file** and shifts the remaining bytes.
- Updates the **frame size** and overall **ID3 tag size** (bytes 6–9 of header).
- Old file is **safely replaced** by new file after successful operation.

---

## 🛠 Dependencies

- Standard C libraries only (`stdio.h`, `string.h`)
- Compatible with **GCC/Linux environment**

---

## 📞 Maintainer

**Rehan Mokashi**  
Emertxe – Embedded Systems & IoT Trainee  
[GitHub](https://github.com/rmokashi01) | [LinkedIn](https://www.linkedin.com/in/rehan-mokashi-7b32472a2/)

---

## 📜 License

MIT License – Free to use and modify.


./a.out -v sample.mp3               # View tags
./a.out -e -y 2025 sample.mp3       # Edit year
./a.out -v sample.mp3 

1. Open original file (Daavudi.mp3) → fread entire file into memory

2. Find frame position (e.g., TYER), get old size

3. Replace frame content with new value:
   - Update frame size field in header
   - Replace frame body with new data

4. Adjust all following data (shift ahead by diff in size)

5. Update ID3 tag size in main 10-byte header

6. Write all updated content to a temp file → e.g., Daavudi_temp.mp3

7. If everything successful:
   - remove("Daavudi.mp3")
   - rename("Daavudi_temp.mp3", "Daavudi.mp3")

8. Return success ✅
