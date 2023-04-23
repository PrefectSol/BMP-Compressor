# BMP Compressor - 8 bit indexed images

## Run

```bash
git clone https://github.com/PrefectSol/BMP-Compressor.git

cd BMP-Compressor

make

./output/main <input.bmp> <output.bmp>
```

## Result codes:

- success - 0
- no_action - 1
- cannot_open_bmp - 2
- free_not_required - 3
- memory_allocation_error - 4
- already_compressed - 5
- not_indexed_color_scheme - 6
