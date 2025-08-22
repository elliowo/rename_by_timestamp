# Rename by Timestamp

A lightweight C utility that renames files with a timestamp based on their last modification time.

## Features

- Renames files to include a timestamp in the format: `YYYY-MM-DD_HH-MM-SS`
- Avoids renaming files that are already properly timestamped
- Supports wildcard patterns (`*`, `?`, `[]`)
- Preserves original file extensions
- Skips renaming directories

## Usage

```bash
./timestamp_rename <filepath>
./timestamp_rename document.txt
./timestamp_rename file1.txt file2.pdf document.docx
```

Use wildcards to rename multiple files
```
./timestamp_rename *.txt
./timestamp_rename *.jpg *.png
./timestamp_rename backup_*.tar.gz
```

## Installation

1. Compile the program within directory:
```bash
make
```

2. (Root) system-wide:
```bash
make install
make uninstall
```