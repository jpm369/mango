# mango

Now featuring 64bit windows binaries for mango/hardklor. 

Put mango.exe, hardklor, ISOTOPE.dat, hardklor.dat into one directory, and it should run.

Extracts precursor masses from chimeric spectra of cleavable cross-linkers

Compiling instructions:
Navigate to /mango-master/ or /mango/ (whatever directory contains the Makefile and Hardklor/mstoolkit directories) depending on how you 
grabbed the repo and run Make. Depending on the environment you are running, you might have to compile hardklor separately. 

Install notes: 
After compiling, add the hardklor executable to your path environment as it's called directly by mango. Additionally, there are two
lines in mango_ProcessManager.cpp (Lines 700,701) that specify the path of the hardklor .dat files used in the analysis. Those two lines
need to be updated with the full path to where those files will be stored on your system (e.g. /usr/etc/hardklor.dat). After updating
those lines in that file, it is necessary to recompile (run make clean followed by make) to update their path in the .conf file.
At some point I hope to update this to make it slightly less cumbersome of an install process.

Quick usage steps:
1. Run mango.exe -p to generate mango.params.new. It must be renamed to mango.params for it to be used.
2. Change params file to match acquistion method/cross-linker used.
3. Run mango.exe on an mzXML, it will return a .hk1, .hk2, .peaks, and .ms2/.mgf (depending on export parameter). The .hk1/.hk2 files
are hardklor's output for ms1/ms2 scans. The .peaks file lists the masses that fulfilled the mass relationship. The .ms2 contains the
extracted precursor masses with their peaklist. The .ms2 is supported natively with Comet 2017 onwards, just add mango_search=1 to the
comet.params file. If using a different search tool, use Mango's .mgf output, which stores the details to pair spectra in the spectrum
title.
4. Search data using comet or whatever other tool (use mango_search=1 in the comet.params if using comet).
5. Pair results using spectrum title identifier. 

Other notes:

-Mango won't rerun hardklor if the appropriate .hk files are already in the directory. Hardklor is the majority of the runtime, so this
makes it quick to rerun mango on the same set of files if some parameters are initially incorrect. Consequently, if you change the 
hardklor passthrough parameters, you have to get rid of those .hk files first, or mango won't rerun hardklor. 

Parameter notes:

MANGO PARAMS

-mass_tolerance_relationship: Determines what tolerance Mango will call a pair of masses as fulfilling the mass relationship. This
relationship also considers a -2/-1/0/1/2 C13 offsets because deconvolution is not perfect, and sometimes the wrong peak is labeled
as the monoisotopic. 

-mass_tolerance_peptide: Determines the tolerance at which Mango will consider a pair of masses as different in the relationship. It 
cuts down on redundancy by not having the same mass pair at slightly offset ppms reported many times in the same scan.

-reporter_neutral_mass: Constant mass offset used to identify precursor masses. This is dependent entirely on the cross-linker used.
The default mass is for BDP-NHP, and a derivation example for DSSO is provided in the supplement, that could be readily extended to
any cross-linkers that have assymetric fragmentation (DSBU,DSSO, etc).

-export_mgf: Writes an .ms2 file if this value is 0, writes a .mgf file if it is 1. The .mgf files are highly redundant and can
be very large depending on how many relationships are found, as each potential precursor is written to an individual scan. Depending
on the search software used, it might be necessary to break the .mgf into a few separate files, as some software struggles to read in
very large files.

HARDKLOR PARAMS 

instrument1/2: Mass analyzer used for the ms1/ms2 scans. Usually Orbitrap or FTICR.

resolution1/2: This is the resolution at 400 m/z. Depending on the type of instrument, the resolution specified in the method file refers to either resolution at 200 or 400 m/z. For most orbitrap instruments it is the value at 200 m/z, so divide the method resolution by 1.414 to convert it to the resolution at 400 m/z. FTICRs report their resolution at 400 m/z, so they can be used without modification.

centroid1/2: This is a boolean indicating whether or not the input spectra are centroided or not.

There are other hardklor params, but they are all hardcoded. You can see what they are the .conf1 and .conf2 files returned by Mango.
Note that changing these values in those .conf files won't do change the parameters used to run Hardklor, as they are overwritten
whenever Mango runs Hardklor. The values used should work fairly well for the majority of experiments.
