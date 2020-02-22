# Run diff with golden output

Simply running `./rundiff.sh` works. When the changes are found, you can accept the changes by running the following command.

    diff -ruN testdata/golden_output testdata/output | patch -p2 -d testdata/golden_output

# How to prepare golden input

`testdata/root` is read by `linkerconfig` as a root dir for golden output.

    testdata/root/
        system/etc/
            sanitizer.libraries.txt
            vndkcorevariant.libraries.txt
        apex/
            <apex>/
                apex_manifest.json   # copied from intermediate apex_manifest_full.json
                bin/.keep            # indicates to generate ld.config.txt for this apex
