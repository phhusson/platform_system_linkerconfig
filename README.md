# LinkerConfig

## Introduction

Linkerconfig is a program to generate linker configuration based on the runtime
environment. Linkerconfig generates one or more ld.config.txt files and some
other files under /linkerconfig during init. Linker will read this generated
configuration file(s) to find out link relationship between libraries and
executable.

## File Formats

There are several file formats used by linkerconfig.

### Input

#### APEX Linker configuration

This document describes format of `linker.config.txt` file under APEX `etc`
directory. This file can be used to add extra information while linkerconfig
creates linker configuration with the APEX module.

##### Format

This configuration has multiple sections, which is defined with `[section_name]`
and there are multiple properties for each section. Format of the property can
be varied based on the section, but in usual it is list of items or assiging
value to name with `<property_name> = <property_value>` format. Available
sections are pre-defined as below.

###### Extra permitted path section

Extra permitted path section has section name as `permitted_paths`. This section
contains property as a list of items, and each item is an extra permitted path
which should be added in the APEX namespace.

##### Example

```
# This is a example APEX linker configuration

# Extra permitted path section
[permitted_paths]
/system/${LIB}
/data

# EOF
```

## Output

### ld.config.txt

Check
[ld.config.format.md](https://android.googlesource.com/platform/bionic/+/master/linker/ld.config.format.md).

### apex.libraries.txt
