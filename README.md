# ttaverify
Checks True Audio (TTA) lossless audio files for corruption

Many TTA decoders do not verify the various checksums inside TTA files when decoding them, leading to possible unnoticed corruption.

This program will verify the header, seek table, and individual audio frames for corruption.

To use, simply pass one or more filenames into the program and it will output the results.
```
$ ttaverify MyAlbum.tta CorruptHeader.tta CorruptSeek.tta CorruptFrames.tta

MyAlbum.tta: OK
CorruptHeader.tta: File has a corrupt or invalid header
CorruptSeek.tta: File has a corrupt seek table
CorruptFrames.tta: File has one or more corrupt frames
```

Windows binaries are avaliable in [Releases](https://github.com/Hakkin/ttaverify/releases)
