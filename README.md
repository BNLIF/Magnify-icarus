# Magnify

## A magnifier to investigate raw and deconvoluted waveforms.

### Usage

```
cd scripts/
root -l loadClasses.C 'Magnify.C("path/to/rootfile")'
# or
root -l loadClasses.C 'Magnify.C("path/to/rootfile", "<tpcNo>", "<frame>", "<rebin>")'
```

The `tpcNo` argument can be 110, 120, 111, 121, 112, 122, 113, 123 
- the first digit is alway '1' by convention
- the 2nd digit: 1 means left side, 2 means right side
- the 3rd digit: 0-3 means EE, EW, WE, WW respectively (to be confirmed)

The `frame` argument names which output from the signal processing to display.  Likely names are:

- `decon` produced by the Wire Cell prototype (default).
- `wiener` produced by the Wire Cell toolkit, used to define ROI or "hits".
- `gauss` produced by the Wire Cell toolkit, used for charge measurement.

The call to ROOT can be be called somewhat more easily via a shell
script wrapper.  It assumes to stay in the source directory:

```
/path/to/magnify/magnify.sh /path/to/wcp-rootfile.root
# or
/path/to/magnify/magnify.sh /path/to/wct-rootfile.root 110 gauss 4
```

### Example files

<!-- An example ROOT file of waveforms can be found at http://www.phy.bnl.gov/xqian/talks/wire-cell/2D_display_3455_0_0.root

If one omits the file name, a dialog will open to let user select the file: -->


### Screenshot

<!-- ![screenshot](data/screenshot.png?raw=true "Screenshot") -->
