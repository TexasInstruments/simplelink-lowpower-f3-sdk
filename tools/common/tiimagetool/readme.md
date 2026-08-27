# TI image tool

TI image tool (or tiimagetool) is a python script that can split a single binary into 2 parts or assemble 2 binaries into a single binary.

## Commands

### Split

To split a binary, the `split` command should be used. The user must provide the location to the input binary and the address to split the binary.
The output names are optional but can be inputs. The output file names will default to 'partition1.bin' and 'partition2.bin'.  

```
tiimagetool split [OPTIONS] IMGFILE ADDRESS
tiimagetool split --verbose --partition1 <part1> --partition2 <part2> <input> <address>
```

### Assemble

To assemble a binary, the `assemble` command should be used. The user must provide the location to 2 binaries. The output names are optional but can be inputs. The output file name will default to 'assembledImage.bin' . The output binary consists of 'image1' and 'image2' appended to 'binaryout'. 

```
tiimagetool assemble [OPTIONS] IMAGE1 IMAGE2
tiimagetool assemble --verbose --outfile <binaryout> <image1> <image2>
```

Other flags include:


### Version
```
tiimagetool --version
```

### Help
```
tiimagetool --help
```

### Verbose
```
tiimagetool [split|assemble] --verbose
```

## **Note that**
1. Only 2 binaries can be assembled.
2. A binary image is split into only 2 partitions.
3. All inputs must be binary files.